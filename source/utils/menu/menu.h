/*!
 * \file menu.h
 * \brief Contains declarations relating to menus, such as the Command,
 * Option and Menu structs
 *
 * This header file contains declarations relating to \ref menu.c as well as
 * macros for forward and normal declarations of menu structs
 *
 * In this documentation the following terminology will be used:
 * - "choices": refers to the options of a menu. On screen each and
 * every choice will appear on a separate line, and the width of the menu will
 * be at least the length of the longest choice string
 * - "banner": a two-dimensional char array that can be displayed alongside a
 * menus choices, for example unicode-art of a cat or a stylized ASCII-font
 * declaring the name of the menu
 */

#pragma once

#define choices_len(arr) ((int)(sizeof(arr) / sizeof(char*)))
#include <ncurses.h>

#include <base.h>

/*!
 * \brief Option for menus
 *
 * Specifies a label to be shown when a menu is printed, as well as command to
 * execute when the options with that label is selected.
 *
 * On selection \ref command::execute will be called with a pointer to the
 * Option itself as an argument. This permits you to pass extra arguments to
 * on_select via a struct of the form { Option _, extra args...}
 */
typedef struct Option
{
    //TODO: Rework into command
    //! Command to be executed
    Command* command;
    //! Label on the menu button corresponding to this option
    char const* label;
} Option;

/*!
 * \brief Holds menu information
 *
 * This structure is used to describe menus and is used by the
 * functions declared in \ref menu.h.
 *
 * Menu structs should rarely be created directly and rather the macros \ref
 * make_menu and \ref make_menu_verbose should be preferred. Please see their
 * documentation for options and end use.
 */
typedef struct Menu
{
    //! An array of pointers to \ref Option
    struct Option const** choices;
    //! The number of choices (i.e. the length of *choices)
    int choices_height;
    //! The minimal width of the menu
    int choices_width;
    //! 2D UTF-8 char array printed above the menu (can be null)
    char const** banner;
    //! Specifies the length of banner
    int banner_height;
    //! Specifies the length of banner[i]
    int banner_width;
    //! x-coordinate of the menus upper left corner
    int start_x;
    //! y-coordinate of the menus upper left corner
    int start_y;
} Menu;

/* <--- Menu members ---> */
/*!
 * \var Menu::choices
 * The Option array details the available choices for the menu (in the form
 * of strings), as well as the function that should be run on select.
 *
 * \var Menu::choices_width
 * If all choice labels are shorter than this, the menu is
 * padded with whitespace.
 */

/*! \brief structure for dialogues
 *
 * \param path The path to a file containing dialogue conforming to formatting
 * \param width The width of the dialogue box
 */
struct dia
{
    char const* path;
    int width;
};

int get_menu_width(struct Menu const* menu);
int get_banner_width(const char* const* banner, int size);

//! Enum used to specify text justification in menus
enum Justification
{
    left_just,
    center_just,
    right_just
};

void win_cleanup(WINDOW* win);

/*! \brief Create a menu structure
 *
 *  This macro creates a menu structure to be used with other functions in this
 * file for printing, etc.
 *
 *  \param menu_name This will be the name of the menu and also determines the
 * name of the struct created (will be ${menu_name}_menu). There has to be a
 * global char* array named the same thing containing the menu choices, e.g.
 * const char* const example[3] = [ "one", "two", "three"] has to be defined
 * when creating example_menu
 *
 *  \param banner The name of a global char* array containing the banner (ascii
 * art) to be printed above the menu's choices. Every string in the array has to
 * consist of the same number of unicode code points May be left NULL for no
 * banner.
 *
 * \param min_choice_width A minimum width for the choices box. Will be exceeded
 * if the length of the options demands it
 *
 *  \param justification A constant of the Justification enum that indicates if
 * the text should be left justified, right justified, or centered. (Not yet
 * implemented)
 *
 * \param left_pad A unicode code_point (in the form of a null-terminated
 * string) to use for left padding (Not yet implemented)
 *
 * \param right_pad A unicode code_point (in the form of a null-terminated
 * string) to use for right padding (Not yet implemented)
 *
 * \param start_x The x-coordinate for the upper left corner of the menu. If
 * negative will be set such that menu is horizontally centered on screen.
 *
 * \param start_y The y-coordinate for the upper left corner of the menu. If
 * negative will be set such that menu is vertically centered on screen.
 */

#define make_menu_verbose(menu_name, menu_banner, min_choice_width,            \
                          justification, left_pad, right_pad, x, y)            \
    static struct Menu implementation_##menu_name##_menu = {                   \
        .choices        = (struct Option const**)(menu_name),                  \
        .choices_height = choices_len(menu_name),                              \
        .choices_width  = (min_choice_width),                                  \
        .banner         = (const char**)(menu_banner),                         \
        .banner_height  = (int)(sizeof(menu_banner) / sizeof(char*)),          \
        .start_x        = (x),                                                 \
        .start_y        = (y),                                                 \
    };                                                                         \
                                                                               \
    const struct Menu* const menu_name##_menu =                                \
        &implementation_##menu_name##_menu

/*! \brief Convenient macro for creating a menu structure
 *
 *  This macro creates a menu structure to be used with other functions in this
 * file for printing, etc.
 *
 * For more options, and more detailed descriptions, see \ref make_menu_verbose,
 * although this macro should be preferred
 *
 *  \param name This will be the name of the menu and also determines the
 * name of the struct created (will be ${menu_name}_menu). There has to be a
 * global char* array named the same thing containing the menu choices, e.g.
 * const char* const example[3] = [ "one", "two", "three"] has to be defined
 * when creating example_menu
 *
 *  \param banner The name of a global char* array containing the banner (ascii
 * art) to be printed above the menu's choices. Every string in the array has to
 * consist of the same number of unicode code points May be left NULL for no
 * banner.
 *
 * \param min_width A minimum width for the menu choices. Will be exceeded if
 * the length of the options demands it
 *
 * \param start_x The x-coordinate for the upper left corner of the menu. If
 * negative will be set such that menu is horizontally centered on screen.
 *
 * \param start_y The y-coordinate for the upper left corner of the menu. If
 * negative will be set such that menu is vertically centered on screen.
 */
#define make_menu(name, banner, min_width, start_x, start_y)                   \
    make_menu_verbose(name, banner, min_width, left_just, " ", " ", start_x,   \
                      start_y)

/*!
 * \brief Menu class derived from \ref Command
 *
 * Contains the information necessary for the printing of a menu.
 */
typedef struct Menu_command
{
    //! Base class - see \Command documentation
    Command command;
    struct Menu const* menu;
    int highlight;
} Menu_command;

#define make_menu_command(menu_name)                                           \
    Menu_command const show_##menu_name = {                                    \
        .command   = (Command){.execute = show_menu},                          \
        .menu      = menu_name##_menu,                                         \
        .highlight = 0};

#define extern_menu(name) extern const struct Menu* const name##_menu

//! Loads the path to the dialogue directory into buf
void get_dialogue_path(char* buf, int sz);

void implementation_initialise_menu(struct Menu* menu);

//! Prints a menu with a \ref Option::on_select executed on select
Command* print_menu(const struct Menu* menu, int select);

//! Conveniently print a minimalistic menu
int quick_print_menu(int width, int count, ...);

//! Prints the passed in dialogue file to screen with indicated width
int print_dia(const char* file_path, int width);

#undef string_arr_len
