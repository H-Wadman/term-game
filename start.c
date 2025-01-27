#include <assert.h>
#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "menu_constants.h"
#include "start.h"
#include "utf8.h"
#include "utils/menu.h"
#include "utils/state.h"

// static inline void win_cleanup(WINDOW* win)
// {
//     werase(win);
//     wrefresh(win);
//     delwin(win);
// }

#define get_and_print_dia(file, width)                                         \
    {                                                                          \
        char buf[1024];                                                        \
        get_dialogue_path(buf, 1024);                                          \
        strcat(buf, (file));                                                   \
        print_dia(buf, (width));                                               \
    }

typedef struct Sudoku_command
{
    Command c;
    int board[9][9]; //NOLINT
} Sudoku_command;

//len of str should fit in an int

/*! \brief This function prints the string passed in as a dialogue
 *
 * This funcion will print a dialogue with an identical result to print_dia,
 * however instead of printing the contents of a file, it will print the passed
 * in string.
 *
 * \param str A string containing the text you want to print with length fitting
 * into an int
 *
 * \returns 0 on success and a negative integer on failure depending on the
 * error
 */
int print_diastr(char const* const str)
{
    char const tmpl[] = "/tmp/tmpdia.XXXXXX";
    int fd            = mkstemp((char*)tmpl);
    FILE* temp        = fdopen(fd, "w");
    int b             = fputs(str, temp);

    assert(b == (int)strlen(str));
    if (b != (int)strlen(str)) {
        fprintf( //NOLINT
            stderr,
            "fputs did not manage to print entire string in print_diastr\n");
        return -1;
    }

    int err = fputs(u8"\n§\n", temp);
    if (err == EOF) {
        fprintf(stderr, //NOLINT
                "fputs failed in print_diastr\n");
        return -2;
    }
    err = fflush(temp);
    if (err == EOF) {
        fprintf(stderr, //NOLINT
                "fflush failed in print_diastr, with errno value: %s\n",
                strerror(errno));
        return -3;
    }

    err = print_dia(tmpl, utf8_strlen(str));
    if (err == -1) {
        fprintf(stderr, "print_dia failed in print_diastr\n"); //NOLINT
        return -6;                                             //NOLINT
    }

    err = fclose(temp);
    if (err == EOF) {
        fprintf(stderr, //NOLINT
                "fclose failed in print_diastr\n");
        return -4;
    }

    err = remove(tmpl);
    if (err == -1) {
        fprintf(stderr, //NOLINT
                "failed to remove file in print_diastr\n");
        return -5; //NOLINT
    }


    return 0;
}

Func show_opening(void* _ __attribute__((unused)))
{
    get_and_print_dia("opening.txt", COLS / 3);
    return (Func){.func = show_main_menu};
}

//This type of function with call to print_menu and return in order to avoid
//arguments should be made obsolete
Func show_options(void* _ __attribute__((unused)))
{
    push_func((Func){show_main_menu});
    Func op = print_menu(options_menu);
    return op;
}

Func show_main_menu(void* _ __attribute__((unused)))
{
    Func op = print_menu(start_menu);
    return op;
}

Func show_glade(void* _ __attribute__((unused)))
{
    if (!player_visited_glade_val()) {
        get_and_print_dia("intro.txt", COLS / 2);
        player_visited_glade_set();
    }
    Func op = print_menu(glade_menu);

    return op;
}

Func show_well(void* _ __attribute__((unused)))
{
    push_func((Func){show_glade});
    if (player_visited_well_val()) {
        get_and_print_dia("well.txt", COLS / 2);
        player_visited_well_set();
    }

    Func op = print_menu(well_menu);

    return op;
}

// clang-format off
    char const* const bucket[] = {
        " ───┴───",
        "/_______\\",
        "\\\\      /",
        " \\\\    /",
        "  -----",
    };
// clang-format on

void wpaint_bucket(WINDOW* win, int const y)
{
    int const max_x = getmaxx(win);

    int const height = sizeof(bucket) / sizeof(char*);
    int const width  = get_banner_width(bucket, height);

    int const x = (max_x - width) / 2;
    for (int i = 0; i < height; ++i) { mvwaddstr(win, y + i, x, bucket[i]); }

    wrefresh(win);
}

void wpaint_rope(WINDOW* win, int count, int piece_len, int bucket_width)
{
    for (int i = 0; i < count; ++i) {
        int const curr = piece_len * i;
        mvwaddstr(win, curr, bucket_width / 2, "O");
        for (int j = 1; j <= piece_len - 1; ++j) {
            mvwaddstr(win, curr + j, bucket_width / 2, "|");
        }
    }
}

int bucket_iteration(WINDOW* win, int count, int piece_len, int bucket_width,
                     int bucket_height)
{
    werase(win);
    wpaint_rope(win, count, piece_len, bucket_width);
    wpaint_bucket(win, count * piece_len);
    wrefresh(win);

    int ch = wgetch(win);
    switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W'     : --count; break;
        case KEY_DOWN:
        case 's'     :
        case 'S'     : {
            int total_height = count * piece_len + bucket_height;
            if (total_height + piece_len <= LINES) { ++count; }
        } break;
        default:;
    }

    return count;
}

Func well_raise_bucket_func(void* _ __attribute__((unused)))
{
    if (player_has_key_val()) {
        print_diastr("You've already got the key!");
        return (Func){.func = show_well};
    }
    int const bucket_height = sizeof(bucket) / sizeof(char*);
    int const bucket_width  = get_banner_width(bucket, bucket_height);

    int const mid_x = COLS / 2;
    // Make centered window with bucket_width
    WINDOW* bucket_win =
        newwin(LINES, bucket_width, 0, mid_x - bucket_width / 2);
    intrflush(bucket_win, false);
    keypad(bucket_win, true);


    int const piece_len = 4;
    assert(piece_len > 1);
    int count = (LINES - bucket_height) / piece_len;

    assert(count > 0);
    while (count > 0) {
        count = bucket_iteration(bucket_win, count, piece_len, bucket_width,
                                 bucket_height);
    }

    werase(bucket_win);
    wrefresh(bucket_win);
    delwin(bucket_win);

    print_diastr("There's an old rusty key at the bottom of the bucket.");
    print_diastr("Grab it?");

    int res = quick_print_menu(2, "Yes", "No");
    if (res == 0) { player_has_key_set(); }

    return (Func){.func = show_well};
}

char const* const sudoku_board[] = {
    "╔═══╦═══╦═══╦═══╦═══╦═══╦═══════════╗",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠───┼───┼───╬───┼───┼───║───┼───┼───║",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠───┼───┼───╬───┼───┼───║───┼───┼───║",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠═══╬═══╬═══╬═══╬═══╬═══╬═══════════╣",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠───┼───┼───╬───┼───┼───║───┼───┼───║",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠───┼───┼───╬───┼───┼───║───┼───┼───║",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠═══╬═══╬═══╬═══╬═══╬═══╬═══════════╣",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠───┼───┼───╬───┼───┼───║───┼───┼───║",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╠───┼───┼───╬───┼───┼───║───┼───┼───║",
    "║   │   │   ║   │   │   ║   │   │   ║",
    "╚═══╩═══╩═══╩═══╩═══╩═══╩═══════════╝",
};

#define SUDOKU_HEIGHT 19
#define SUDOKU_WIDTH 37

int sudoku_xcoord(int x) { return 2 + 4 * x; }

int sudoku_ycoord(int y) { return 1 + 2 * y; }

void paint_sudoku_sq(WINDOW* s_win, int y, int x, int dig)
{
    int s_y = sudoku_ycoord(y);
    int s_x = sudoku_xcoord(x);
    for (int j = -1; j <= 1; ++j) { mvwaddch(s_win, s_y, s_x + j, ' '); }

    mvwaddch(s_win, s_y, s_x, (dig == 0) ? ' ' : dig + '0');
}

WINDOW* paint_sudoku_board(int* board)
{
    int const height = SUDOKU_HEIGHT;
    int const width  = SUDOKU_WIDTH;
    int x_pos        = (COLS - width) / 2;
    int y_pos        = (LINES - height) / 2;
    WINDOW* s_win    = newwin(height, width, y_pos, x_pos);

    intrflush(s_win, true);
    keypad(s_win, true);


    for (int i = 0; i < height; ++i) {
        mvwaddstr(s_win, i, 0, sudoku_board[i]);
    }

    int const side_len = 9;
    if (board) {
        for (int i = 0; i < side_len; ++i) {
            for (int j = 0; j < side_len; ++j) {
                mvwaddch(s_win, sudoku_ycoord(i), sudoku_xcoord(j),
                         (board[9 * i + j] == 0) ? ' '
                                                 : board[9 * i + j] + '0');
            }
        }
    }

    wrefresh(s_win);
    return s_win;
}

bool valid_row(int const* board, int r)
{
    bool digs[9]; //NOLINT
    int const sz = 9;
    memset(digs, false, sizeof digs);

    for (int i = 0; i < sz; ++i) {
        int curr = board[sz * r + i];
        if (curr == 0) { return false; }
        else {
            assert(1 <= curr && curr <= 9);
            if (digs[curr - 1]) { return false; }
            else {
                digs[curr - 1] = true;
            }
        }
    }

    return true;
}

bool valid_col(int const* board, int c)
{
    bool digs[9]; //NOLINT
    int const sz = 9;
    memset(digs, false, sizeof digs);

    for (int i = 0; i < sz; ++i) {
        int curr = board[sz * i + c];
        assert(0 <= curr && curr <= 9);
        if (curr == 0) { return false; }
        else {
            if (digs[curr - 1]) { return false; }
            else {
                digs[curr - 1] = true;
            }
        }
    }

    return true;
}

bool valid_sq(int const* board, int y, int x)
{
    int const sz = 9;

    bool digs[9]; //NOLINT
    memset(digs, false, sizeof digs);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int curr = board[sz * (3 * y + i) + j + 3 * x];
            assert(0 <= curr && curr <= sz);
            if (curr == 0) { return false; }
            else {
                if (digs[curr - 1]) { return false; }
                else {
                    digs[curr - 1] = true;
                }
            }
        }
    }

    return true;
}

bool is_solved(int const* board)
{
    int const sz = 9;
    for (int i = 0; i < sz; ++i) {
        if (!valid_row(board, i)) {
            fprintf(stderr, "Invalid row %d\n", i + 1);
            return false;
        }
        if (!valid_col(board, i)) {
            fprintf(stderr, "Invalid col %d\n", i + 1);
            return false;
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!valid_sq(board, i, j)) {
                fprintf(stderr, "Invalid sq (%d, %d)\n", j, i);
                return false;
            }
        }
    }

    return true;
}

void play_sudoku(WINDOW* suk_win, Sudoku_command* sc)
{
    int y = 0;
    int x = 0;
    int board[9][9]; //NOLINT

    memcpy(board, sc->board, sizeof board);

    wattrset(suk_win, A_BLINK | A_REVERSE);
    paint_sudoku_sq(suk_win, 0, 0, board[0][0]);
    wrefresh(suk_win);

    while (!is_solved((int*)board)) {
        int ch = wgetch(suk_win);

        if (sc->board[y][x] == 0 && board[y][x] != 0) {
            wattrset(suk_win, A_REVERSE);
            paint_sudoku_sq(suk_win, y, x, board[y][x]);
        }
        else {
            wattrset(suk_win, A_NORMAL);
            paint_sudoku_sq(suk_win, y, x, board[y][x]);
        }

        switch (ch) {
            case KEY_UP:
                if (y > 0) { --y; }
                break;
            case KEY_DOWN:
                if (y < 8) { ++y; }
                break;
            case KEY_LEFT:
                if (x > 0) { --x; }
                break;
            case KEY_RIGHT:
                if (x < 8) { ++x; }
                break;
            default:
                if ('0' <= ch && ch <= '9' && sc->board[y][x] == 0) {
                    board[y][x] = ch - '0';
                    paint_sudoku_sq(suk_win, y, x, board[y][x]);
                }
                break;
        }

        wattrset(suk_win, A_BLINK | A_REVERSE);
        paint_sudoku_sq(suk_win, y, x, board[y][x]);
    }
}

void sudoku_test()
{
    //NOLINTBEGIN
    int test[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    assert(!valid_row(test, 0));
    test[0] = 9;
    assert(valid_row(test, 0));
    test[8] = 9;
    assert(!valid_row(test, 0));

    int test2[3][9] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 1, 2, 3, 4, 5, 6}
    };
    assert(valid_sq((int*)test2, 0, 0));
    assert(valid_sq((int*)test2, 0, 1));
    assert(valid_sq((int*)test2, 0, 2));
    //NOLINTEND
}

Func paint_sudoku(void* this)
{
#ifndef NDEBUG
    sudoku_test();
#endif

    Sudoku_command* sc = (Sudoku_command*)this;
    WINDOW* suk_win    = paint_sudoku_board((int*)sc->board);

    play_sudoku(suk_win, sc);

    werase(suk_win);
    wrefresh(suk_win);
    delwin(suk_win);

    return (Func){.func = NULL};
}

void test_sudoku()
{
    //NOLINTBEGIN
    Sudoku_command sc_solved __attribute__((unused)) = {
        .c     = (Command){.label = NULL, .on_select = NULL},
        .board = {
                           {0, 8, 5, 4, 7, 9, 1, 3, 2},
                           {7, 3, 4, 1, 6, 2, 5, 9, 8},
                           {2, 1, 9, 5, 3, 8, 7, 6, 4},
                           {9, 2, 6, 3, 4, 5, 8, 7, 1},
                           {8, 5, 1, 7, 2, 6, 3, 4, 9},
                           {4, 7, 3, 8, 9, 1, 2, 5, 6},
                           {3, 4, 2, 6, 8, 7, 9, 1, 5},
                           {5, 6, 8, 9, 1, 3, 4, 2, 7},
                           {1, 9, 7, 2, 5, 4, 6, 8, 3},
                           }
    };
    Sudoku_command sc = {
        .c     = (Command){.label = NULL, .on_select = NULL},
        .board = {
                           {6, 0, 0, 0, 7, 9, 0, 3, 2},
                           {0, 0, 0, 0, 6, 0, 5, 0, 0},
                           {2, 0, 9, 0, 0, 8, 7, 0, 0},
                           {9, 0, 6, 3, 0, 5, 0, 0, 1},
                           {8, 5, 0, 0, 0, 0, 3, 0, 0},
                           {4, 7, 3, 0, 0, 1, 2, 5, 0},
                           {0, 4, 2, 6, 8, 0, 9, 0, 0},
                           {0, 0, 0, 0, 1, 3, 4, 2, 7},
                           {0, 9, 0, 2, 0, 0, 6, 0, 0},
                           }
    };
    //NOLINTEND

    paint_sudoku(&sc);
}
