#ifndef MENU_H
#define MENU_H
#define choices_len(arr) ((int)(sizeof(arr) / sizeof(char*)))

/*! \file menu.h
 * \brief Contains declarations relating to menu.c
 *
 * This header files contains declarations relating to menu.c as well as a macro
 * for declaring
 */

/*! \brief structure to hold menu information
 *
 */
struct Menu
{
    char const** choices;
    int choices_height;
    int choices_width;
    char const** banner;
    int banner_height;
    int banner_width;
    int start_x;
    int start_y;
};

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

//! Enum used to specify text justification in menus
enum Justification
{
    left_just,
    center_just,
    right_just
};

/*! \brief Macro to create a menu structure
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
 * \param min_width A minimum width for the choices box. Will be exceeded if the
 * length of the options demands it
 *
 *  \param justification A constant of the Justification enum that indicates if
 * the text should be left justified, right justified, or centered.
 *
 * \param left_pad A unicode code_point (in the form of a null-terminated
 * string) to use for left padding
 *
 * \param right_pad A unicode code_point (in the
 * form of a null-terminated string) to use for right padding
 *
 * \param start_x The x-coordinate for the starting point of the menu. If
 * negative will be set such that menu is centered on screen.
 *
 * \parm start_y The y-coordinate for the starting point of the menu. If
 * negative will be set such that menu is centered on screen.
 */

#define make_menu_verbose(menu_name, menu_banner, min_choice_width,            \
                          justification, left_pad, right_pad, x, y)            \
    static struct Menu implementation_##menu_name##_menu = {                   \
        .choices        = (const char**)(menu_name),                           \
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

#define make_menu(name, banner, min_width, start_x, start_y)                   \
    make_menu_verbose(name, banner, min_width, left_just, " ", " ", start_x,   \
                      start_y)

#define extern_menu(name) extern const struct Menu* const name##_menu

//! Loads the path to the dialogue directory into buf
void get_dialogue_path(char* buf);

void implementation_initialise_menu(struct Menu* menu);

//! Prints the passed in menu to the screen and returns the number of the first
//! selected choice
int print_menu(const struct Menu* menu);

//! Prints the passed in dialogue file to screen with indicated width
int print_dia(const char* file_path, int width);

#undef string_arr_len
#endif
