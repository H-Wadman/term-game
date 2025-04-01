#ifndef START_H
#define START_H

#include "base.h"

void init_game();

Command* start_game();

Command* show_opening(void*);

extern Command const show_glade;

extern Command const show_well;

extern Command const well_raise_bucket_command;

extern Command const show_options;

extern Command const knock;

extern Command const switch_katte_mode;


#endif
