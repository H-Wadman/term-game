#include "string.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "build-path.h"
#include "menu.h"
#include "utf8.h"

#define LINE_FEED 13
#define MAX_UTF8_WORD_LEN 40

const char selection_string[] = u8"◇ ";
//Needs to be changed if selection_string is changed
int const selection_offset      = 2;
int const menu_box_width_offset = 2 + selection_offset;

/*! Loads the path to the dialogue directory
 *
 *  \param buf A character buffer in which the path to the dialogue directory
 * will be stored. it is the responsibility of the caller that the buffer is of
 * sufficient size
 */
void get_dialogue_path(char* buf) { strcpy(buf, SOURCE_DIR); }

WINDOW* add_banner(const struct menu* menu, WINDOW* menu_win)
{
    if (!menu->banner) { return NULL; }

    int x                         = 0;
    int y __attribute__((unused)) = 0;
    getmaxyx(menu_win, y, x);
    int const menu_middle = x / 2;
    WINDOW* banner_win =
        newwin(menu->banner_height, menu->banner_width,
               menu->start_y - menu->banner_height,
               menu->start_x + menu_middle - menu->banner_width / 2);
    for (int i = 0; i < menu->banner_height; ++i) {
        mvwprintw(banner_win, i, 0, "%s", menu->banner[i]);
    }

    wrefresh(banner_win);
    return banner_win;
}

static inline void win_cleanup(WINDOW* win)
{
    werase(win);
    wrefresh(win);
    delwin(win);
}

void print_menu_highlight(WINDOW* menu_win, const struct menu* menu,
                          int highlight, int x_align)
{
    // Compensate if menus contain non-ascii characters
    int const u8_ascii_diff = (int)strlen(menu->choices[highlight]) -
                              utf8_strlen(menu->choices[highlight]);
    // +1 in order to account for '\0'-termination
    int const total_len =
        menu->choices_width + u8_ascii_diff + (int)strlen(selection_string) + 1;

    char* option = (char*)malloc(total_len);

    int const sz = snprintf(option, total_len, u8"%s%s", selection_string,
                            menu->choices[highlight]);
    assert(sz == (int)strlen(menu->choices[highlight]) +
                     (int)strlen(selection_string));
    if (sz > total_len - 1) {
        fprintf( //NOLINT
            stderr,
            "Option string length exceeded total_len in "
            "print_menu_highlight (%d and %d respectively).\n Aborting...\n",
            sz + 1, total_len);
        exit(-1);
    }
    if (sz == -2) {
        fprintf(stderr, // NOLINT
                "sprintf encountered an error in print_menu_highlight.\n"
                "Aborting...");
        exit(-1);
    }

    memset(option + sz, ' ', total_len - sz);
    option[total_len - 1] = '\0';

    wattron(menu_win, A_STANDOUT);
    mvwaddstr(menu_win, 1 + highlight, x_align, option);
    wattroff(menu_win, A_STANDOUT);
    free(option);
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

/*! \brief Calculates the width of a menu
 *
 *  \param menu A menu struct whose width (in unicode points) is to be
 * calculated \returns The width of the menu as number of unicode points
 */
int get_menu_width(struct menu const* menu)
{
    int max = 0;
    for (int i = 0; i < menu->choices_height; ++i) {
        int temp = utf8_strlen(menu->choices[i]);
        if (temp > max) { max = temp; }
    }

    return max > menu->choices_width ? max : menu->choices_width;
}

/*! Prints the passed in menu according to its parameters and then blocks until
 * a choice has been selected. Afterwards an integer corresponding to the menu
 * choice selected (its index in the menu's choice array)
 */
int print_menu(const struct menu* menu)
{
    WINDOW* menu_win =
        newwin(menu->choices_height + 2,
               menu->choices_width + 2 + utf8_strlen(selection_string),
               menu->start_y, menu->start_x);
    WINDOW* title_win = add_banner(menu, menu_win);
    intrflush(menu_win, false);
    keypad(menu_win, TRUE);

    box(menu_win, 0, 0);

    int option = 0;
    refresh_menu_win(menu_win, menu, option);

    int ch = 0;
    while (true) {
        ch = wgetch(menu_win);
        switch (ch) {
            case KEY_UP:
                option =
                    (option + menu->choices_height - 1) % menu->choices_height;
                break;
            case KEY_DOWN: option = (option + 1) % menu->choices_height; break;
            case LINE_FEED:
                win_cleanup(menu_win);
                win_cleanup(title_win);

                return option;
            default:;
        }
        refresh_menu_win(menu_win, menu, option);
    }
}

void implementation_initialise_menu(struct menu* menu)
{
    menu->choices_width = get_menu_width(menu);
    assert(menu->choices_width <= COLS);

    menu->banner_width = utf8_strlen(menu->banner[0]);
    assert(menu->banner_width <= COLS);

    if (menu->start_x < 0) {
        menu->start_x = (COLS - (menu->choices_width + 2)) / 2;
    }
    if (menu->start_y < 0) {
        menu->start_y =
            (LINES - (menu->choices_height + 2) + menu->banner_height) / 2;
    }
}

struct dia_print
{
    FILE* file;
    const char* path;
    int line;
    int line_len;
    int width;
};

#define handle_floadw_error(code, file)                                        \
    {                                                                          \
        if ((code) == 0) {                                                     \
            fprintf(stderr,                                                    \
                    "Error occurred while reading utf8 word from file in "     \
                    "print_dia\n");                                            \
            return -1;                                                         \
        }                                                                      \
    }

/*! \brief Prints the next word of a dia_print struct to screen and updates it
 * accordingly
 *
 *  \param A dia_print struct containing win, file and position information
 *  \returns A 0 (or 1/2 for EOF/§ respectively) indicating if the end of a
 * dialogue window has been reached (or not), that is if a EOF or § has been
 * encountered in the file. \returns -1 on error
 */
int print_next_word(WINDOW* win, struct dia_print* dia)
{
    char buf[ASCII_BUF_SZ * MAX_UTF8_WORD_LEN];

    int code = floadw_utf8(buf, dia->file);
    //fprintf(stderr, "%s\n", buf);

    handle_floadw_error(code, dia->file); //NOLINT

    if (dia->line_len + (int)strlen(buf) > dia->width) {
        dia->line_len = (int)strlen(buf) + 1;
        ++dia->line;
        wmove(win, dia->line, 1);
    }
    else {
        dia->line_len += (int)strlen(buf) + 1;
    }

    if (code == EOF) { return 1; }
    else if ((strcmp(u8"§", buf) == 0)) {
        return 2;
    }
    else {
        char end = (dia->line_len == dia->width + 1) ? '\0' : ' ';
        wprintw(win, "%s%c", buf, end);
        //wgetch(win);
        return 0;
    }
}

/*! \brief Calculates the height needed to print a dialogue
 *
 *  \param A dia_print struct
 *  \returns The number of lines needed to print a dialogue window given the
 * specified width
 */
int get_dia_height(const struct dia_print* dia)
{
    FILE* file = fopen(dia->path, "r");
    int err    = fseek(file, ftell(dia->file), SEEK_SET);
    if (err != 0) {
        fprintf(stderr, //NOLINT
                "fseek encountered an error in get_dia_height");
    }

    //char buf[ASCII_BUF_SZ * MAX_UTF8_WORD_LEN];

    int word_len = 0;
    int height   = 1;
    int len      = 0;
    char buf[ASCII_BUF_SZ];
    while (true) {
        int const err = fload_utf8(buf, file);
        if (err == 1) { return -1; }
        if (err == EOF || strcmp(buf, u8"§") == 0) { break; }

        ++len;
        if (isspace(buf[0])) { word_len = 0; }
        else {
            ++word_len;
        }
        if (len > dia->width) {
            ++height;
            len = word_len;
        }
    }

    return height;
}

typedef struct print_dia_win_res
{
    bool error;
    int res;
} print_dia_win_res;

/*! \brief Prints a dialogue to screen
 *
 * Note: It is the responsibility of the caller that the width of the dialogue
 * is larger than any word contained in the dialogue
 *
 * \param A dia_print structure containing the necessary printing information
 *
 * \returns A print_dia_win_res structure indicating if an error occurred, and
 * if not an integer
 */
print_dia_win_res print_dia_win(struct dia_print dia_p)
{
    //struct dia_print dia_p = {fopen(dia.path, "r"), 0, 0, dia.width};

    int height = get_dia_height(&dia_p);
    // Create a centered window with padding for borders
    WINDOW* dia_win =
        newwin(2 + height, 2 + dia_p.width, (LINES - (2 + height)) / 2,
               (COLS - (2 + dia_p.width)) / 2);

    box(dia_win, 0, 0);
    wmove(dia_win, 1, 1);

    int r_code = 0;
    while (r_code == 0) {
        r_code = print_next_word(dia_win, &dia_p);
        if (r_code == -1) {
            print_dia_win_res res = {.error = true};
            return res;
        }
    }
    if (r_code == 2) {
        wrefresh(dia_win);
        wgetch(dia_win);
    }

    win_cleanup(dia_win);

    print_dia_win_res res = {.error = false, .res = r_code};
    return res;
}

/*! This function will print a dialogue to screen, according to the content of a
 * specified file, and a specified width In order for the dialogue file to
 * function properly, the file should contain text (UTF8) and all "dialogue
 * bubbles" should be finished with a '§' character (even the last one). The '§'
 * character is special and cannot occur in normal dialogue.
 *
 * \param file_path A string containing a path to the file containing the
    dialogue
 * \param width An int stating the width of the dialogue to print; it
 * is the responsibility of the caller that the width of any word in the
 * dialogue (counted as unicode code points) does not exceed this width
 *
 * \returns 0 on success, -1 on failure
 */
int print_dia(const char* file_path, int width)
{
    if (width + 2 > COLS) {
        fprintf(stderr, //NOLINT
                "Dialogue with width wider than COLS (columns) passed to "
                "print_dia\n");
    }
    FILE* f = fopen(file_path, "r");

    if (!f) {
        fprintf( //NOLINT
            stderr,
            "Error ocurred while opening file in print_dia with errno: %s\n",
            strerror(errno));
    }
    struct dia_print dia = {f, .path = file_path, .line = 1, .line_len = 0,
                            .width = width};

    while (true) {
        print_dia_win_res code = print_dia_win(dia);
        if (code.error) {
            fclose(f); //NOLINT
            return -1;
        }
        else if (code.res == 1) {
            fclose(f); //NOLINT
            return 0;
        }
    }
}
