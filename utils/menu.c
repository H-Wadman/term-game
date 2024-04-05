#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "utf8.h"

const char* const title[7] = {
    " ___________  _______   _______   ___      ___       _______       __     "
    "  ___      ___   _______  \n",
    "(\"     _   \")/\"     \"| /\"      \\ |\"  \\    /\"  |     /\" _   \"|  "
    "   /\"\"\\     |\"  \\    /\"  | /\"     \"| \n",
    " )__/  \\\\__/(: ______)|:        | \\   \\  //   |    (: ( \\___)    /   "
    " \\     \\   \\  //   |(: ______) \n",
    "    \\\\_ /    \\/    |  |_____/   ) /\\\\  \\/.    |     \\/ \\        "
    "/' /\\  \\    /\\\\  \\/.    | \\/    |   \n",
    "    |.  |    // ___)_  //      / |: \\.        |     //  \\ ___  //  __'  "
    "\\  |: \\.        | // ___)_  \n",
    "    \\:  |   (:      \"||:  __   \\ |.  \\    /:  |    (:   _(  _|/   /  "
    "\\\\  \\ |.  \\    /:  |(:      \"| \n",
    "     \\__|    \\_______)|__|  \\___)|___|\\__/|___|     \\_______)(___/   "
    " \\___)|___|\\__/|___| \\_______) \n",
};

const char* const menu_choices[] = {"Play", "Options", "Potatisfläsk", "Exit"};

const int title_height = (int)(sizeof(title) / sizeof(char*));
const int title_width  = (int)strlen(title[0]);

//const int outer_menu_width = 2 * title_width / 3 + 10;
const int outer_menu_width = 77;

//Note: This currently only works because the '\0' character knocks off one
//character and the '◇' counts for two c characters in a string but 1 character
//on the screen
const int inner_menu_width = outer_menu_width;

Menu new_menu()
{
    static const int n_choices = (int)(sizeof(menu_choices) / sizeof(char*));
    const int width            = outer_menu_width;

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

void print_menu_highlight(WINDOW* menu_win, int highlight, int x_align)
{
    //Compensate if menus contain non-ascii characters
    int const u8_ascii_diff = (int)strlen(menu_choices[highlight]) -
                              u8_strlen(menu_choices[highlight]);
    int const total_len = inner_menu_width + u8_ascii_diff;
    char* option        = (char*)malloc(total_len);

    int const sz =
        snprintf(option, total_len, u8"◇ %s", menu_choices[highlight]);
    if (sz > inner_menu_width - 1 || sz == -1) {
        fprintf(stderr, //NOLINT
                "Option string exceeded inner_menu_width or sprintf had an "
                "error.\n Aborting...");
        exit(-1); //NOLINT
    }
    memset(option + sz, ' ', total_len - sz);
    option[total_len - 1] = '\0';

    wattron(menu_win, A_STANDOUT);
    mvwprintw(menu_win, 1 + highlight, x_align, option,
              menu_choices[highlight]);
    wattroff(menu_win, A_STANDOUT);
}

bool refresh_menu_win(WINDOW* menu_win, const Menu* menu, int highlight)
{
    //Printing menu box
    wattroff(menu_win, A_STANDOUT);
    werase(menu_win);
    box(menu_win, 0, 0);
    const int x_align = 1;
    int y_align       = 1;
    for (int i = 0; i < menu->n_choices; ++i) {
        mvwprintw(menu_win, y_align, x_align, "%s", menu_choices[i]);
        ++y_align;
    }

    print_menu_highlight(menu_win, highlight, x_align);
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
