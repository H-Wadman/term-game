#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build-path.h"
#include "menu.h"
#include "menu_constants.h"
#include "utf8.h"

void endwin_atexit() { endwin(); }

void ncurses_set_up()
{
    const char* locale = setlocale(LC_ALL, "");
    if (locale == NULL) {
        fprintf(stderr, "Error setting locale, aborting.../\n"); //NOLINT
        exit(1);
    }
    initscr();
    clear();
    noecho();
    cbreak();
    nonl();
    intrflush(stdscr, false);
    //Add this?
    // raw();
    keypad(stdscr, TRUE);

    int err = atexit(endwin_atexit);
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

int main()
{
    ncurses_set_up();
    initialise_menus();

    char buf[128]; //NOLINT
    strcpy(buf, SOURCE_DIR);
    strcat(buf, "/dialogue/test.txt");
    print_dia(buf, 80); //NOLINT

    int op = print_menu(start_menu);
    if (op == start_play_ch) { print_menu(encounter_menu); }

    return 0;
}
