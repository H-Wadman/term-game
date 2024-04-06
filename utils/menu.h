#ifndef MENU_H
#define MENU_H
#define choices_len(arr) ((int)(sizeof(arr) / sizeof(char*)))

/*! \file menu.h
 * \brief Contains declarations relating to menu.c
 *
 * This header files contains declarations relating to menu.c as well as a macro
 * for declaring
 */

int get_menu_width(char const* const* choices, int sz);

struct menu
{
    char const* const* const choices;
    int const choices_height;
    int const choices_width;
    char const* const* const banner;
    int const banner_height;
    int const banner_width;
    int const start_x;
    int const start_y;
};

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
 * \param right_pad A unicde code_point (in the
 * form of a null-terminated string) to use for right padding
 */

#define make_menu_verbose(menu_name, menu_banner, min_width, justification,    \
                          left_pad, right_pad)                                 \
    const struct menu menu_name##_menu = {                                     \
        .choices        = (const char**)(menu_name),                           \
        .choices_height = choices_len(menu_name),                              \
        .banner         = (const char**)(menu_banner),                         \
        .banner_height  = (int)(sizeof(menu_banner) / sizeof(char*))}

#define make_menu(name, banner, min_width)                                     \
    make_menu_verbose(name, banner, min_width, left_just, " ", " ")

#define extern_menu(name) extern const struct menu name##_menu

void implementation_initialise_menu(struct menu* menu);

int print_menu(const struct menu* menu);

#undef string_arr_len
#endif
