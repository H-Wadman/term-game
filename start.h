#ifndef START_H
#define START_H

#include "utils/menu.h"
#include <ncurses.h>

Func show_opening(void*);

Func show_options(void*);

Func show_main_menu(void*);

Func show_glade(void*);

Func show_well(void*);

Func well_raise_bucket_func(void*);

int print_diastr(char const* const str);

WINDOW* paint_sudoku_board(int* board);

#endif
