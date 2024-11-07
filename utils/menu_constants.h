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

extern_menu(options);


extern_menu(encounter);

extern_menu(glade);

extern_menu(well);

void initialise_menus();

#endif
