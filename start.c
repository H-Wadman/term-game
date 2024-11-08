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
