#ifndef START_H
#define START_H

#include "base.h"

void init_game(void);

Command* start_game(void);

Command* show_opening(void*);

extern Command const show_glade;

extern Command const show_well;

extern Command const well_raise_bucket_command;

extern Command const show_options;

extern Command const knock;

extern Command const switch_katte_mode;

extern Command const go_to_cabin;


#endif
