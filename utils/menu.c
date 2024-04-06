#include "string.h"
#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>

#include "menu.h"
#include "utf8.h"

WINDOW* add_banner(const struct menu* menu)
{
    WINDOW* banner_win =
        newwin(menu->banner_height, menu->banner_width,
               (LINES - menu->banner_height - (menu->choices_height + 2)) / 2,
               (COLS - menu->banner_width) / 2);
    for (int i = 0; i < menu->banner_height; ++i) {
        mvwprintw(banner_win, i, 0, "%s", menu->banner[i]);
    }

    wrefresh(banner_win);
    return banner_win;
}

void print_menu_highlight(WINDOW* menu_win, const struct menu* menu,
                          int highlight, int x_align)
{
    // Compensate if menus contain non-ascii characters
    int const u8_ascii_diff = (int)strlen(menu->choices[highlight]) -
                              utf8_strlen(menu->choices[highlight]);
    // The + 2 compensates for the '\0'-character and for the ◇ taking up to
    // C-character but only one character on screen
    int const total_len = menu->choices_width + u8_ascii_diff + 2;
    char* option        = (char*)malloc(total_len);

    int const sz =
        snprintf(option, total_len, u8"◇ %s", menu->choices[highlight]);
    if (sz > menu->choices_width + 1 || sz == -1) {
        fprintf(stderr, // NOLINT
                "Option string exceeded inner_menu_width or sprintf had an "
                "error.\n Aborting...");
        exit(-1); // NOLINT
    }
    memset(option + sz, ' ', total_len - sz);
    option[total_len - 1] = '\0';

    wattron(menu_win, A_STANDOUT);
    mvwprintw(menu_win, 1 + highlight, x_align, option,
              menu->choices[highlight]);
    wattroff(menu_win, A_STANDOUT);
}

bool refresh_menu_win(WINDOW* menu_win, const struct menu* menu, int highlight)
{
    // Printing menu box
    wattroff(menu_win, A_STANDOUT);
    werase(menu_win);
    box(menu_win, 0, 0);
    const int x_align = 1;
    int y_align       = 1;
    for (int i = 0; i < menu->choices_height; ++i) {
        mvwprintw(menu_win, y_align, x_align, "%s", menu->choices[i]);
        ++y_align;
    }

    print_menu_highlight(menu_win, menu, highlight, x_align);
    wrefresh(menu_win);

    return true;
}

int get_menu_width(struct menu const* menu)
{
    int max = 0;
    for (int i = 0; i < menu->choices_height; ++i) {
        int temp = utf8_strlen(menu->choices[i]);
        if (temp > max) { max = temp; }
    }

    return max > menu->choices_width ? max : menu->choices_width;
}

int print_menu(const struct menu* menu)
{
    WINDOW* menu_win = newwin(menu->choices_height + 2, menu->choices_width + 2,
                              menu->start_y, menu->start_x);
    WINDOW* title_win = add_banner(menu);
    intrflush(menu_win, false);
    keypad(menu_win, TRUE);

    box(menu_win, 0, 0);

    int option = 0;
    refresh_menu_win(menu_win, menu, option);

    int ch              = 0;
    const int line_feed = 13;
    while (true) {
        ch = wgetch(menu_win);
        switch (ch) {
            case KEY_UP:
                option =
                    (option + menu->choices_height - 1) % menu->choices_height;
                break;
            case KEY_DOWN: option = (option + 1) % menu->choices_height; break;
            case line_feed:
                delwin(menu_win);
                delwin(title_win);
                return option;
            default:;
        }
        refresh_menu_win(menu_win, menu, option);
    }
}

void implementation_initialise_menu(struct menu* menu)
{
    int* choices_width = (int*)&(menu->choices_width);
    *choices_width     = get_menu_width(menu);
    assert(*choices_width <= COLS);

    int* banner_width = (int*)&menu->banner_width;
    *banner_width     = utf8_strlen(menu->banner[0]);
    assert(*banner_width <= COLS);

    int* x = (int*)&menu->start_x;
    *x     = (COLS - (menu->choices_width + 2)) / 2;
    int* y = (int*)&menu->start_y;
    *y     = (LINES - (menu->choices_height + 2) + menu->banner_height) / 2;
}
