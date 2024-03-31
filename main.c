#include <assert.h>
#include <limits.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text.h"
#include "utils.h"

int const height;
int const width;

void endwin_atexit() { endwin(); }

void ncurses_set_up()
{
    setlocale(LC_ALL, "");
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
        fprintf(
            stderr,
            "Couldn't register atexit function, quitting program...\n"); //NOLINT
        endwin();
        exit(1); //NOLINT
    }
}

const char* const menu_choices[] = {"Play", "Options", "Exit"};

typedef enum Choices
{
    op_play,
    op_options,
    op_exit,
    op_N
} Choices;

int get_menu_width()
{
    int size   = (int)(sizeof(menu_choices) / sizeof(char*));
    size_t max = 0;
    for (int i = 0; i < size; ++i) {
        size_t temp = strlen(menu_choices[i]);
        if (temp > max) { max = temp; }
    }

    assert(max < INT_MAX);

    return (int)max;
}

typedef struct menu
{
    const int n_choices;
    const int height;
    const int width;
    const int y;
    const int x;
} Menu;

Menu new_menu()
{
    static const int n_choices = (int)(sizeof(menu_choices) / sizeof(char*));
    const int width            = get_menu_width() + 4;

    return (Menu){.n_choices = n_choices,
                  .height    = n_choices + 2,
                  .width     = width,
                  .y         = (LINES - (n_choices + 2) + title_height) / 2,
                  .x         = (COLS - width) / 2};
}

WINDOW* add_title(const Menu* menu)
{
    WINDOW* title_win = newwin(title_height, title_width,
                               (LINES - title_height - menu->height) / 2,
                               (COLS - title_width) / 2);
    for (int i = 0; i < title_height; ++i) {
        mvwprintw(title_win, i, 0, "%s", title[i]);
    }

    wrefresh(title_win);
    return title_win;
}

bool refresh_menu_win(WINDOW* menu_win, const Menu* menu, int highlight)
{
    wattroff(menu_win, A_STANDOUT);
    werase(menu_win);
    box(menu_win, 0, 0);
    const int x_align = 1;
    int y_align       = 1;
    for (int i = 0; i < menu->n_choices; ++i) {
        mvwprintw(menu_win, y_align, x_align, "%s", menu_choices[i]);
        ++y_align;
    }

    wattron(menu_win, A_STANDOUT);
    mvwprintw(menu_win, 1 + highlight, x_align, u8"◇ %s",
              menu_choices[highlight]);
    wattroff(menu_win, A_STANDOUT);

    wrefresh(menu_win);

    return true;
}

Choices start_menu()
{
    const Menu menu = new_menu();

    WINDOW* menu_win  = newwin(menu.height, menu.width, menu.y, menu.x);
    WINDOW* title_win = add_title(&menu);
    intrflush(menu_win, false);
    keypad(menu_win, TRUE);

    box(menu_win, 0, 0);

    int option = 0;
    refresh_menu_win(menu_win, &menu, option);

    int ch              = 0;
    const int line_feed = 13;
    while (true) {
        ch = wgetch(menu_win);
        switch (ch) {
            case KEY_UP  : option = (option + op_N - 1) % op_N; break;
            case KEY_DOWN: option = (option + 1) % op_N; break;
            case line_feed:
                delwin(menu_win);
                delwin(title_win);
                return option;
            default:;
        }
        refresh_menu_win(menu_win, &menu, option);
    }
}

int main()
{
    ncurses_set_up();
    start_menu();

    return 0;
}
