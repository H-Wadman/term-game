#ifndef START_H
#define START_H

#include <ncurses.h>

#include "base.h"

Command* show_menu(void*);

Command* show_opening(void*);

extern Command const show_glade;

extern Command const show_well;

extern Command const well_raise_bucket_command;

extern Command const show_options;

extern Command const knock;

extern Command const switch_katte_mode;


#endif
