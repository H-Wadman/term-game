#include <locale.h>
#include <stdlib.h>

#include "menu.h"
#include "menu_constants.h"
#include "utf8.h"

void endwin_atexit() { endwin(); }

void ncurses_set_up()
{
    setlocale(LC_ALL, ""); //NOLINT
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
            stderr, "Couldn't register atexit function, quitting program...\n");
        endwin();
        exit(1); //NOLINT
    }
}

int main()
{
    ncurses_set_up();
    initialise_menus();
    print_menu(&start_menu);

    return 0;
}
