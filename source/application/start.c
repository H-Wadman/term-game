#include <assert.h>
#include <errno.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "games/sudoku.h"
#include "io/logging.h"
#include "io/utf8.h"
#include "menu.h"
#include "menu_constants.h"
#include "start.h"
#include "state.h"

#define GET_AND_PRINT_DIA(file, width)                                         \
    {                                                                          \
        char buf[1024];                                                        \
        get_dialogue_path(buf, 1024);                                          \
        strcat(buf, (file));                                                   \
        print_dia(buf, (Banner){.art = NULL}, (width));                        \
    }

#define GET_AND_PRINT_DIA_BANNER(file, banner, width)                          \
    {                                                                          \
        char buf[1024];                                                        \
        get_dialogue_path(buf, 1024);                                          \
        strcat(buf, (file));                                                   \
        print_dia(buf, (banner), (width));                                     \
    }

//len of str should fit in an int


static void perform_atexit()
{
    endwin();
    close_log_stream();
}

//! \brief Sets the locale, initialises ncurses with common defaults, and
//! registers a function to be called on exit
static void ncurses_set_up()
{
    const char* locale = setlocale(LC_ALL, "");
    if (locale == NULL) {
        fprintf(stderr, "Error setting locale, aborting.../\n"); //NOLINT
        exit(1);
    }
    initscr();
    start_color();
    init_color_pairs();
    clear();
    noecho();
    cbreak();
    nonl();
    intrflush(stdscr, false);
    //Add this?
    // raw();
    keypad(stdscr, TRUE);

    int err = atexit(perform_atexit);
    if (err != 0) {
        fprintf( //NOLINT
            stderr,
            "Couldn't register atexit function in ncurses_set_up, quitting "
            "program...\n");
        endwin();
        exit(1);
    }
    err = curs_set(0);
    if (err == ERR) {
        fprintf(stderr, //NOLINT
                "Terminal doesn't support invisible cursor, support to be "
                "added.\n Aborting...\n");
        exit(1);
    }
}

void init_game()
{
    ncurses_set_up();
    initialise_menus();
    set_log_output(stderr);
}

Command* start_game() { return new_command(show_opening, false); }

Command* show_opening(void* _ __attribute__((unused)))
{
    GET_AND_PRINT_DIA("opening.txt", COLS / 3);
    return new_menu_command(start_menu, 0);
}

static Command* show_options_execute(void* _ __attribute__((unused)))
{
    push_command(new_menu_command(start_menu, 1));
    Command* op = print_menu(options_menu, 0);
    return op;
}

Command const show_options = {.execute    = show_options_execute,
                              .persistent = true};

static Command* show_glade_execute(void* _ __attribute__((unused)))
{
    if (!player_visited_glade_val()) {
        GET_AND_PRINT_DIA("intro.txt", COLS / 2);
        player_visited_glade_set();
    }
    Command* op = print_menu(glade_menu, 0);

    return op;
}

Command const show_glade = {.execute = show_glade_execute, .persistent = true};

static Command* show_well_execute(void* _ __attribute__((unused)))
{
    Command* res = new_command(show_glade_execute, false);
    push_command(res);
    if (player_visited_well_val()) {
        GET_AND_PRINT_DIA("well.txt", COLS / 2);
        player_visited_well_set();
    }

    Command* op = print_menu(well_menu, 0);

    return op;
}

Command const show_well = {.execute = show_well_execute, .persistent = true};

// static Command* show_cabin_execute(void* _ __attribute__((unused)))
// {
//     if (!player_visited_cabin_val()) {
//         player_visited_cabin_set();
//         print_diastr("");
//     }
// }

static Banner gudrun_banner()
{
    static bool first_call = true;
    static Banner res      = {0};

    if (first_call) {
        first_call = false;
        res = make_banner(gudrun_art, sizeof(gudrun_art) / sizeof(char*));
    }

    return res;
}

Sudoku_command const gertrud_sudoku = {
    .command = {.execute = paint_sudoku, .persistent = true},
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

static Command* knock_freaky()
{
    GET_AND_PRINT_DIA("freaky.txt", COLS / 3);
    int h = sizeof(freaky_apple_art) / sizeof(freaky_apple_art[0]);
    int w = utf8_strlen(freaky_apple_art[0]);
    WINDOW* freaky_apple_win = newwin(h, w, 0, 0);
    intrflush(freaky_apple_win, false);
    keypad(freaky_apple_win, true);

    for (int i = 0; i < h; ++i) {
        mvwaddstr(freaky_apple_win, i, 0, freaky_apple_art[i]);
    }
    wrefresh(freaky_apple_win);
    get_input_char((Input){.win = freaky_apple_win, .tag = tag_win});
    win_cleanup(freaky_apple_win);

    return (Command*)&show_cabin;
}

Command* go_to_cabin_execute(void* _ __attribute__((unused)))
{
    if (player_has_forest_map_val()) { return (Command*)&show_gudrun; }
    else {
        return (Command*)&show_cabin;
    }
}

Command const go_to_cabin = {.execute    = go_to_cabin_execute,
                             .persistent = true};

Command* gudruns_mission(void* _ __attribute__((unused)))
{
    GET_AND_PRINT_DIA_BANNER("gudruns_mission.txt", gudrun_banner(), COLS / 3);

    print_diastr("You have received the forest map!");
    player_has_forest_map_set();

    return (Command*)&go_to_cabin;
}

static Command* knock_execute(void* _ __attribute__((unused)))
{
    static bool has_knocked = false;
    if (!has_knocked) {
        print_diastr("You approach the door and knock.");
        print_diastr(".");
        print_diastr("..");
        print_diastr("...");
        print_diastr("Seems like nobody's answering.");
    }

    print_diastr("The door seems to be locked.");


    if (!player_has_key_val()) { return (Command*)&show_cabin; }

    print_diastr("Use key?");
    int res = quick_print_menu(0, 2, "Yes", "No");
    if (res == 0) {
        if (is_katte_mode()) { return knock_freaky(); }
        GET_AND_PRINT_DIA_BANNER("meeting_gudrun.txt", gudrun_banner(),
                                 COLS / 4);
        push_command(new_command(gudruns_mission, false));
        return (Command*)&gertrud_sudoku;
    }
    else if (res == 1) {
        return (Command*)&show_cabin;
    }

    log_and_exit("Invalid result returned by quick_print_menu\n");
}

Command const knock = {.execute = knock_execute, .persistent = true};

Command* switch_katte_mode_execute(void* _ __attribute__((unused)))
{
    set_katte_mode(!is_katte_mode());
    bool katte_mode = is_katte_mode();

    char const* str =
        katte_mode ? "Katte mode enabled!" : "Katte mode disabled!";
    print_diastr(str);

    return new_menu_command(options_menu, 1);
}

Command const switch_katte_mode = {.execute    = switch_katte_mode_execute,
                                   .persistent = true};

// clang-format off
    char const* const bucket[] = {
        " ───┴───",
        "/_______\\",
        "\\\\      /",
        " \\\\    /",
        "  -----",
    };
// clang-format on

static void wpaint_bucket(WINDOW* win, int const y)
{
    int const max_x = getmaxx(win);

    Banner b = make_banner(bucket, sizeof(bucket) / sizeof(char*));

    int const x = (max_x - b.dim.width) / 2;
    for (int i = 0; i < b.dim.height; ++i) {
        mvwaddstr(win, y + i, x, bucket[i]);
    }

    wrefresh(win);
}

static void wpaint_rope(WINDOW* win, int count, int piece_len, int bucket_width)
{
    for (int i = 0; i < count; ++i) {
        int const curr = piece_len * i;
        mvwaddstr(win, curr, bucket_width / 2, "O");
        for (int j = 1; j <= piece_len - 1; ++j) {
            mvwaddstr(win, curr + j, bucket_width / 2, "|");
        }
    }
}

static int bucket_iteration(WINDOW* win, int count, int piece_len,
                            Dim bucket_dim)
{
    werase(win);
    wpaint_rope(win, count, piece_len, bucket_dim.width);
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
            int total_height = count * piece_len + bucket_dim.height;
            if (total_height + piece_len <= LINES) { ++count; }
        } break;
        default:;
    }

    return count;
}

static Command* well_raise_bucket_execute(void* _ __attribute__((unused)))
{
    if (player_has_key_val()) {
        print_diastr("You've already got the key!");
        return (Command*)&show_well;
    }
    Banner b = make_banner(bucket, sizeof(bucket) / sizeof(char*));

    int const mid_x = COLS / 2;
    // Make centered window with bucket_width
    WINDOW* bucket_win = newwin(LINES, b.dim.width, 0, mid_x - b.dim.width / 2);
    intrflush(bucket_win, false);
    keypad(bucket_win, true);


    int const piece_len = 4;
    assert(piece_len > 1);
    int count = (LINES - b.dim.height) / piece_len;

    assert(count > 0);
    while (count > 0) {
        count = bucket_iteration(bucket_win, count, piece_len, b.dim);
    }

    werase(bucket_win);
    wrefresh(bucket_win);
    delwin(bucket_win);

    print_diastr("There's an old rusty key at the bottom of the bucket.");
    print_diastr("Grab it?");

    int res = quick_print_menu(COLS / 8, 2, "Yes", "No"); //NOLINT(*magic*)
    if (res == 0) { player_has_key_set(); }

    return (Command*)&show_well;
}

Command const well_raise_bucket_command = {.execute = well_raise_bucket_execute,
                                           .persistent = true};
