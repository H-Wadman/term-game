#include <assert.h>
#include <ncurses.h>
#include <string.h>

#include "base.h"
#include "sudoku.h"

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

enum
{
    SUDOKU_CHAR_HEIGHT = 19,
    SUDOKU_CHAR_WIDTH  = 37,
    SUDOKU_SZ          = 9
};

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
    int const height = SUDOKU_CHAR_HEIGHT;
    int const width  = SUDOKU_CHAR_WIDTH;
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
    bool digs[SUDOKU_SZ];
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
    bool digs[SUDOKU_SZ];
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

    bool digs[SUDOKU_SZ];
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

bool sudoku_is_solved(int const* board)
{
    int const sz = 9;
    for (int i = 0; i < sz; ++i) {
        if (!valid_row(board, i)) { return false; }
        if (!valid_col(board, i)) { return false; }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!valid_sq(board, i, j)) { return false; }
        }
    }

    return true;
}

void play_sudoku(WINDOW* suk_win, Sudoku_command* sc)
{
    int y = 0;
    int x = 0;
    int board[SUDOKU_SZ][SUDOKU_SZ];

    memcpy(board, sc->board, sizeof board);

    wattrset(suk_win, A_BLINK | A_REVERSE);
    paint_sudoku_sq(suk_win, 0, 0, board[0][0]);
    wrefresh(suk_win);

    while (!sudoku_is_solved((int*)board)) {
        int ch = wgetch(suk_win);

        //If square we're leaving was originally empty but has been filled in,
        //we give it reverse effect
        if (sc->board[y][x] == 0 && board[y][x] != 0) {
            wattrset(suk_win, A_REVERSE);
            paint_sudoku_sq(suk_win, y, x, board[y][x]);
        }
        //Otherwise (empty square or one that was originally filled) we leave it
        //with the normal effect
        else {
            wattrset(suk_win, A_NORMAL);
            paint_sudoku_sq(suk_win, y, x, board[y][x]);
        }

        switch (ch) {
            case KEY_UP:
                if (y > 0) { --y; }
                break;
            case KEY_DOWN:
                if (y < SUDOKU_SZ - 1) { ++y; }
                break;
            case KEY_LEFT:
                if (x > 0) { --x; }
                break;
            case KEY_RIGHT:
                if (x < SUDOKU_SZ - 1) { ++x; }
                break;
#ifdef DEBUG_FUNCTIONALITY
            case ' ': return;
#endif
            default:
                //If input was a digit, we fill the square in if possible
                if ('0' <= ch && ch <= '9' && sc->board[y][x] == 0) {
                    board[y][x] = ch - '0';
                    paint_sudoku_sq(suk_win, y, x, board[y][x]);
                }
                break;
        }

        //Give newly selected square blink effect
        wattrset(suk_win, A_BLINK | A_REVERSE);
        paint_sudoku_sq(suk_win, y, x, board[y][x]);
    }
}

void sudoku_test(void)
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

Command* paint_sudoku(void* this)
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

    return (Command*)&pop;
}

//TODO: Remove
void test_sudoku(void)
{
    //NOLINTBEGIN
    Sudoku_command sc_solved __attribute__((unused)) = {
        .command = (Command){.execute = paint_sudoku},
        .board   = {
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
        .command = (Command){.execute = paint_sudoku},
        .board   = {
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
