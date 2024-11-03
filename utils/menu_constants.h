#ifndef MENU_CONSTANTS_H
#define MENU_CONSTANTS_H

#include <limits.h>
#define COMMAND_RETURN INT_MIN
#include "menu.h"

/*! \file menu.h
 * \brief Contains declarations relating to menu.c
 *
 * This header files contains declarations relating to menu.c as well as a macro
 * for declaring
 */

extern_menu(start);

enum Start_menu_choices
{
    start_play_ch,
    start_options_ch,
    start_exit_ch
};

extern_menu(options);

enum Options_choices
{
    options_language_ch,
    options_volume_ch,
    options_colour_ch,
    options_exit_ch
};

extern_menu(encounter);

enum Encounter_menu_choices
{
    encounter_no_ch,
    encounter_scream_ch,
    encounter_fight_ch,
    encounter_debug_ch,
};

extern_menu(glade);

void initialise_menus();

#endif
