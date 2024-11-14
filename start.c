#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "menu.h"
#include "menu_constants.h"
#include "start.h"
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

    player_has_key_set();

    werase(bucket_win);
    wrefresh(bucket_win);
    delwin(bucket_win);

    return (Func){.func = show_well};
}
