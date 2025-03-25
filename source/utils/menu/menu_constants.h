#ifndef MENU_CONSTANTS_H
#define MENU_CONSTANTS_H

#include <limits.h>
#define COMMAND_RETURN INT_MIN
#include "menu.h"

/*! \file menu_constants.h
 * \brief Declarations pertaining to menu literals defined in \ref
 * menu_constants.c
 *
 * Contains declarations of menu instances, as well as a initialisation function
 * to be called before any other menu related functions.
 */

EXTERN_MENU(start);

EXTERN_MENU(options);


EXTERN_MENU(encounter);

EXTERN_MENU(glade);

EXTERN_MENU(well);

void initialise_menus();

#endif
