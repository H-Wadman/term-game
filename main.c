#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "menu.h"
#include "menu_constants.h"
#include "start.h"

void perform_atexit()
{
    endwin();
    close_log_stream();
}

void init_color_pairs()
{
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

//! \brief Sets the locale, initialises ncurses with common defaults, and
//! registers a function to be called on exit
void ncurses_set_up()
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

int main()
{
    ncurses_set_up();
    initialise_menus();
    set_log_output(stderr);


    //TODO: Transform into union
    Command curr = (Command){.command = show_opening};
    while (curr.command) { curr = curr.command(&curr); }

    return 0;
}
