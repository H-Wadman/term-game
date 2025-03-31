/*!
 * \file menu.c
 * \brief Implementation file for \ref menu.h
 *
 * This file contains the implementation details of the functions declared in
 * the \ref menu.h file. Unless you are debugging, it is probably a better place
 * to look for interesting/useful functions.
 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "build-path.h"
#include "io/logging.h"
#include "io/utf8.h"
#include "menu.h"
#include "menu/start.h"

enum
{
    LINE_FEED         = 13,
    MAX_UTF8_WORD_LEN = 40
};

const char selection_string[] = u8"◇ ";
//Needs to be changed if selection_string is changed
int const selection_offset      = 2;
int const menu_box_width_offset = 2 + selection_offset;

/*!
 * \param[in] menu The menu to print
 * \param[in] highlight The \ref Menu_command::highlight value
 *
 * \returns A malloced Menu_command*
 */
Command* new_menu_command(Menu const* menu, int highlight)
{
    Menu_command* res = (Menu_command*)malloc(sizeof(Menu_command));
    res->command      = (Command){.execute = show_menu, .persistent = false};
    res->menu         = menu;
    res->highlight    = highlight;
    return (Command*)res;
}

/*!
 * Loads the path to the dialogue directory
 *
 *  \param[out] buf A character buffer in which the path to the dialogue
 * directory will be stored.
 *
 * \param[in] sz An int specifying the size of the receiving buffer. If the
 * receing buffer is too small, the program will abort
 */
void get_dialogue_path(char* buf, int sz)
{
    int const extra_len = (int)strlen("/dialogue/");
    if (!((int)strlen(ASSET_DIR) + extra_len < sz - 1)) {
        log_and_exit(
            "Buffer for dialogue path was not large enough, aborting...\n");
    }
    strcpy(buf, ASSET_DIR);
    strcat(buf, "/dialogue/");
}

/*!
 * \brief Prints the banner of a menu to the window
 *
 * \param[in] menu A menu struct
 * \param[in] menu_win The window for the choices of the menu
 *
 * \returns A new window on which the banner is printed
 */
WINDOW* add_banner(const struct Menu* menu, WINDOW* menu_win)
{
    if (!menu->banner) { return NULL; }

    int x                 = getmaxx(menu_win);
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

//TODO: Can probably be removed (check commented refresh banner)
void refresh_banner(WINDOW* banner_win, const struct Menu* menu,
                    WINDOW* menu_win)
{
    if (!banner_win) { return; }

    int x                         = 0;
    int y __attribute__((unused)) = 0;
    getmaxyx(menu_win, y, x);
    for (int i = 0; i < menu->banner_height; ++i) {
        mvwprintw(banner_win, i, 0, "%s", menu->banner[i]);
    }

    wrefresh(banner_win);
}

//! Convenience function to remove all artifacts of a window and release its
//! resources
void win_cleanup(WINDOW* win)
{
    werase(win);
    wrefresh(win);
    delwin(win);
}

//TODO: REMOVE X_ALIGN FROM INTERFACE
/*!
 * \brief Prints the choice labels of a menu, highlighting the specified option
 *
 * \param[out] menu_win Window to print on
 * \param[in] menu Menu struct with choices to print
 * \param[in] highlight the number of the choice to be printed (0 indexed)
 * \param[in] x_align Offset from the windows left border
 */
void print_menu_highlight(WINDOW* menu_win, const struct Menu* menu,
                          int highlight, int x_align)
{
    // Compensate if menus contain non-ascii characters
    int const u8_ascii_diff = (int)strlen(menu->choices[highlight]->label) -
                              utf8_strlen(menu->choices[highlight]->label);
    // +1 in order to account for '\0'-termination
    int const total_len =
        menu->choices_width + u8_ascii_diff + (int)strlen(selection_string) + 1;

    char* option = (char*)malloc(total_len);

    int const sz = snprintf(option, total_len, u8"%s%s", selection_string,
                            menu->choices[highlight]->label);
    assert(sz == (int)strlen(menu->choices[highlight]->label) +
                     (int)strlen(selection_string));
    if (sz > total_len - 1) {
        log_and_exit(
            "Option string length exceeded total_len in print_menu_highlight "
            "(%d and %d respectively).\n Aborting...\n",
            sz + 1, total_len);
    }
    if (sz == -2) {
        log_and_exit(
            "sprintf encountered an error in print_menu_highlight.\n"
            "Aborting...");
    }

    memset(option + sz, ' ', total_len - sz);
    option[total_len - 1] = '\0';

    wattron(menu_win, A_STANDOUT);
    mvwaddstr(menu_win, 1 + highlight, x_align, option);
    wattroff(menu_win, A_STANDOUT);
    free(option);
}

/*!
 * \brief Prints the menu on the specified window
 *
 * This function clears the WINDOW and prints the menu on it, with the correct
 * choice highlighted.
 *
 * \param[out] menu_win Window to print on
 * \param[in] menu Menu to print
 * \param[in] highlight The choice to be highlighted
 */
void refresh_menu_win(WINDOW* menu_win, const struct Menu* menu, int highlight)
{
    // Printing menu box
    wattroff(menu_win, A_STANDOUT);
    werase(menu_win);
    box(menu_win, 0, 0);
    const int x_align = 1;
    int y_align       = 1;
    for (int i = 0; i < menu->choices_height; ++i) {
        mvwprintw(menu_win, y_align, x_align, "%s", menu->choices[i]->label);
        ++y_align;
    }

    print_menu_highlight(menu_win, menu, highlight, x_align);
    wrefresh(menu_win);
}

/*!
 * \brief Calculate the width of a menu
 *
 * \param[in] menu A menu struct whose width is to be
 *  calculated
 * \returns The width of the menu as number of unicode points
 */
int get_menu_width(struct Menu const* menu)
{
    int max = 0;
    for (int i = 0; i < menu->choices_height; ++i) {
        int temp = utf8_strlen(menu->choices[i]->label);
        if (temp > max) { max = temp; }
    }

    return max > menu->choices_width ? max : menu->choices_width;
}

/*!
 * \brief Calculates the width of a banner
 *
 * \param[in] banner An array of strings
 * \param[in] size the size of banner
 * \returns The width of the banner in unicode points
 */
int get_banner_width(const char* const* banner, int size)
{
    int max = 0;
    for (int i = 0; i < size; ++i) {
        int curr = utf8_strlen(banner[i]);
        if (max < curr) { max = curr; }
    }
    return max;
}

/*!
 * Prints the passed in menu according to its parameters and then blocks until
 * a choice has been selected. At that point, the \ref Option::command::execute
 * associated with the choice is executed, and the returned Command is passed
 * back to the caller
 *
 * \param[in] menu Menu to be printed
 * \returns The \ref Command returned by the selected choice
 */
Command* print_menu(const struct Menu* menu, int select)
{
    WINDOW* menu_win =
        newwin(menu->choices_height + 2,
               menu->choices_width + 2 + utf8_strlen(selection_string),
               menu->start_y, menu->start_x);
    WINDOW* title_win = add_banner(menu, menu_win);
    intrflush(menu_win, false);
    keypad(menu_win, true);

    box(menu_win, 0, 0);

    refresh_menu_win(menu_win, menu, select);

    int ch = 0;
    while (true) {
        ch = wgetch(menu_win);
        switch (ch) {
            case KEY_UP:
                select =
                    (select + menu->choices_height - 1) % menu->choices_height;
                break;
            case KEY_DOWN : select = (select + 1) % menu->choices_height; break;
            case LINE_FEED: {
                struct Option const* const curr = menu->choices[select];
                if (curr->command->execute) {
                    win_cleanup(menu_win);
                    win_cleanup(title_win);
                    Command* res = curr->command->execute((void*)curr->command);

                    return res;
                }
            }
            default:;
        }
        refresh_menu_win(menu_win, menu, select);
        //TODO: Can probably be removed
        //refresh_banner(title_win, menu, menu_win);
    }
}

/*!
 * Prints the passed in menu according to its parameters and then blocks until
 * a choice has been selected. Afterwards an integer corresponding to choice
 * selectes is returned.
 *
 * \param[in] menu The menu to print
 * \returns An index in the range [0, menu.choices_height) indicating the
 *  selected choice
 */
int print_menu_old(const struct Menu* menu)
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

/*!
 * Prints the passed in strings as a menu to screen and returns the index of the
 * selected choice
 *
 * \param[in] count The number of variadic arguments passed
 * \param[in] ... Strings to be used as labels for the menu options
 *
 * \returns An integer in the range [0, count) indicating the selected choice
 */
int quick_print_menu(int width, int count, ...)
{
    assert(count > 0);

    va_list va = NULL;
    va_start(va, count);

    //Could be optimised by just allocating a Option* and filling it with
    //commands, then creating an array and filling that with pointers to
    //individual elements, thus saving on mallocs
    Option** choices = (Option**)malloc(count * sizeof(Option*));
    for (int i = 0; i < count; ++i) { choices[i] = malloc(sizeof(Option)); }

    assert(count > 0);
    for (int i = 0; i < count; ++i) {
        char const* ch      = va_arg(va, char const*);
        choices[i]->label   = ch;
        choices[i]->command = NULL;
    }

    Menu m = {(Option const**)choices, count, width, NULL, 0, 0, -1, -1};
    implementation_initialise_menu(&m);

    int res = print_menu_old(&m);
    for (int i = 0; i < count; ++i) { free(choices[i]); }
    free((void*)choices);
    va_end(va);
    return res;
}

/*!
 * \brief Initialise menu with information only available at runtime
 *
 * This function calculates the width of the menu, and centers the menu if it's
 * start position is not set
 *
 * \param[in,out] menu Menu to initialise
 */
void implementation_initialise_menu(struct Menu* menu)
{
    menu->choices_width = get_menu_width(menu);
    assert(menu->choices_width + utf8_strlen(selection_string) + 2 <= COLS);

    if (menu->banner) {
        menu->banner_width =
            get_banner_width(menu->banner, menu->banner_height);
        assert(menu->banner_width <= COLS);
    }
    else {
        menu->banner_width  = 0;
        menu->banner_height = 0;
    }

    if (menu->start_x < 0) {
        menu->start_x = (COLS - (menu->choices_width + 2)) / 2;
    }
    if (menu->start_y < 0) {
        int height    = (menu->banner) ? menu->banner_height : 0;
        menu->start_y = (LINES - (menu->choices_height + 2) + height) / 2;
    }
}

/*!
 * \brief Holds necessary information when a dialogue is being printed
 *
 * This structure is used only when printing a dialogue. It contains information
 * that will be continually used and updated until the printing is done.
 */
struct Dia_print
{
    //! File pointer to dialogue
    FILE* file;
    //! The path to the dialogue
    const char* path;
    //! The current line
    int line;
    //! The current horizontal position
    int line_len;
    //! Width of the dialogue window
    int width;
};

#define HANDLE_FLOADW_ERROR(code, file)                                        \
    {                                                                          \
        if ((code) == 0) {                                                     \
            log_msgln(                                                         \
                "Error occurred while reading utf8 word from file in "         \
                "print_dia");                                                  \
            return -1;                                                         \
        }                                                                      \
    }

/*!
 * \brief Prints the next word of a dia_print struct to screen and updates it
 * accordingly
 *
 * \param[out] win The window to print on
 *
 *  \param[in,out] dia A dia_print struct containing win, file and position
 * information
 *
 *  \returns A number indicating status
 *
 * \retval -1 Error
 * \retval 0 Word printed
 * \retval 1 EOF reached
 * \retval 2 § read
 */
int print_next_word(WINDOW* win, struct Dia_print* dia)
{
    char buf[ASCII_BUF_SZ * MAX_UTF8_WORD_LEN];

    int code = floadw_utf8(buf, dia->file);

    HANDLE_FLOADW_ERROR(code, dia->file);

    if (dia->line_len + utf8_strlen(buf) > dia->width) {
        dia->line_len = utf8_strlen(buf) + 1;
        ++dia->line;
        wmove(win, dia->line, 1);
    }
    else {
        dia->line_len += utf8_strlen(buf) + 1;
    }

    if (code == EOF) { return 1; }
    else if ((strcmp(u8"§", buf) == 0)) {
        return 2;
    }
    else {
        char end = (dia->line_len == dia->width + 1) ? '\0' : ' ';
        wprintw(win, "%s%c", buf, end);
        return 0;
    }
}

/*!
 * \brief Calculates the height needed to print a dialogue
 *
 *  \param A dia_print struct
 *
 *  \returns The number of lines needed to print a dialogue window given the
 * specified width
 */
int get_dia_height(const struct Dia_print* dia)
{
    FILE* file = fopen(dia->path, "r");
    if (!file) {
        log_and_exit("fopen encountered an error in get_dia_height\n");
    }
    int err = fseek(file, ftell(dia->file), SEEK_SET);
    if (err != 0) {
        log_and_exit("fseek encountered an error in get_dia_height\n");
    }

    int word_len = 0;
    int height   = 1;
    //Temp fix 1 to stop line break when text exactly fits
    //int len = 0;
    int len = -1;
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
            //Temp fix 2 to stop line break when text exactly fits
            //len = word_len;
            len = word_len - 1;
        }
    }

    return height;
}

typedef struct print_dia_win_res
{
    bool error;
    int res;
} Print_dia_win_res;

/*!
 * \brief Prints a dialogue to screen
 *
 * Note: It is the responsibility of the caller that the width of the dialogue
 * is larger than any word contained in the dialogue
 *
 * \param A dia_print structure containing the necessary printing information
 *
 * \returns A print_dia_win_res structure indicating if an error occurred, and
 * if not an integer
 */
Print_dia_win_res print_dia_win(struct Dia_print dia_p)
{
    //struct dia_print dia_p = {fopen(dia.path, "r"), 0, 0, dia.width};

    int height = get_dia_height(&dia_p);
    // Create a centered window with padding for borders
    WINDOW* dia_win =
        newwin(2 + height, 2 + dia_p.width, (LINES - (2 + height)) / 2,
               (COLS - (2 + dia_p.width)) / 2);

    intrflush(dia_win, false);
    keypad(dia_win, true);

    box(dia_win, 0, 0);
    wmove(dia_win, 1, 1);

    int r_code = 0;
    while (r_code == 0) {
        r_code = print_next_word(dia_win, &dia_p);
        if (r_code == -1) {
            Print_dia_win_res res = {.error = true};
            return res;
        }
    }

    char buf[ASCII_BUF_SZ];
    if (r_code == 2) {
        wrefresh(dia_win);
        load_utf8(buf, (Input){.win = dia_win, .tag = tag_win});
    }

    win_cleanup(dia_win);

    Print_dia_win_res res = {.error = false, .res = r_code};
    return res;
}

/*!
 * This function will print a dialogue to screen, according to the content of a
 * specified file, and a specified width In order for the dialogue file to
 * function properly, the file should contain text (UTF8) and all "dialogue
 * bubbles" should be finished with a '§' character on a new line(even the last
 * one). The '§' character is special and cannot occur in normal dialogue.
 *
 * \param file_path A string containing a path to the file containing the
 *  dialogue
 *
 * \param width An int stating the width of the dialogue to print; it
 * is the responsibility of the caller that the width of any word in the
 * dialogue (counted as unicode code points) does not exceed this width
 *
 * \returns 0 on success, -1 on failure
 */
int print_dia(const char* file_path, int width)
{
    if (width + 2 > COLS) {
        log_msgln(
            "Dialogue with width wider than COLS (columns) passed to "
            "print_dia\n");
    }
    FILE* f = fopen(file_path, "r");

    if (!f) {
        log_and_exit(
            "Error ocurred while opening file '%s' in print_dia with errno: "
            "%s\n",
            file_path, strerror(errno));
    }
    struct Dia_print dia = {f, .path = file_path, .line = 1, .line_len = 0,
                            .width = width};

    while (true) {
        Print_dia_win_res code = print_dia_win(dia);
        if (code.error) {
            int err = fclose(f);
            if (err) {
                log_msgf("fclose failed in %s with error: '%s'", __func__,
                         strerror(errno));
            }
            return -1;
        }
        else if (code.res == 1) {
            (void)fclose(f);
            return 0;
        }
    }
}

/*!
 *
 * This commandion will print a dialogue with an identical result to print_dia,
 * however instead of printing the contents of a file, it will print the passed
 * in string.
 *
 * \param str A string containing the text you want to print with length fitting
 * into an int
 *
 * \returns 0 on success and a negative integer on failure depending on the
 * error
 */
int print_diastr(char const* const str)
{
    char const tmpl[] = "/tmp/tmpdia.XXXXXX";
    int fd            = mkstemp((char*)tmpl);
    if (fd == -1) {
        log_and_exit("Failed to make temporary file in %s, aborting...\n",
                     __func__);
    }
    FILE* temp = fdopen(fd, "w");
    if (!temp) {
        log_and_exit("Call to fdopen failed in %s, aborting...\n", __func__);
    }
    int b = fputs(str, temp);

    assert(b == (int)strlen(str));
    if (b != (int)strlen(str)) {
        (void)fclose(temp);
        log_msgln(
            "fputs did not manage to print entire string in print_diastr");
        return -1;
    }

    int err = fputs(u8"\n§\n", temp);
    if (err == EOF) {
        (void)fclose(temp);
        log_msgln("fputs failed in print_diastr");
        return -2;
    }
    err = fflush(temp);
    if (err == EOF) {
        (void)fclose(temp);
        log_msgf("fflush failed in print_diastr, with errno value: %s\n",
                 strerror(errno));
        return -3;
    }

    err = print_dia(tmpl, utf8_strlen(str));
    if (err == -1) {
        (void)fclose(temp);
        log_msgln("print_dia failed in print_diastr");
        return -6; //NOLINT
    }

    err = fclose(temp);
    if (err == EOF) {
        log_msgln("fclose failed in print_diastr");
        return -4;
    }

    err = remove(tmpl);
    if (err == -1) {
        log_msgln("failed to remove file in print_diastr");
        return -5; //NOLINT
    }


    return 0;
}
