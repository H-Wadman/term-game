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

void intro() { get_and_print_dia("/dialogue/intro_placeholder.txt", COLS / 3); }

void show_options()
{
    int op = print_menu(options_menu);
    switch (op) {
        case options_language: break;
        case options_volume  : break;
        case options_colour  : break;
        case options_exit    : return;
        default              : exit(1);
    }
}

void show_main_menu()
{
    while (true) {
        int op = print_menu(start_menu);
        switch (op) {
            case start_play_ch:
                //if new game
                intro();
                break;
            case start_options_ch: show_options(); break;
            case start_exit_ch   : return; break;
            default              : exit(1);
        }
    }
}
