#ifndef START_H
#define START_H

#include "utils/menu.h"
#include <ncurses.h>

Command show_opening(void*);

Command show_options(void*);

Command show_main_menu(void*);

Command show_glade(void*);

Command show_well(void*);

Command well_raise_bucket_command(void*);

int print_diastr(char const* const str);

WINDOW* paint_sudoku_board(int* board);

#endif
