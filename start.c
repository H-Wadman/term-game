#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "menu_constants.h"
#include "utils/menu.h"

// static inline void win_cleanup(WINDOW* win)
// {
//     werase(win);
//     wrefresh(win);
//     delwin(win);
// }

#define get_and_print_dia(file, width)                                         \
    {                                                                          \
        char buf[1024];                                                        \
        get_dialogue_path(buf);                                                \
        strcat(buf, (file));                                                   \
        print_dia(buf, (width));                                               \
    }

void show_intro()
{
    get_and_print_dia("/dialogue/intro_placeholder.txt", COLS / 3);
}

void show_options()
{
    int op = print_menu(options_menu);
    switch (op) {
        case options_language_ch: break;
        case options_volume_ch  : break;
        case options_colour_ch  : break;
        case options_exit_ch    : return;
        default                 : exit(1);
    }
}

int show_main_menu()
{
    int op = print_menu(start_menu);
    switch (op) {
        case start_play_ch:
            //if new game
            return start_play_ch;
        case start_options_ch: show_options(); break;
        case start_exit_ch   : return start_exit_ch; break;
        default:
            fprintf(stderr, //NOLINT
                    "Unrecognized option in show_main_menu");
            exit(1);
    }

    return 5;
}
