#ifndef START_H
#define START_H

#include "utils/menu.h"
#include <ncurses.h>

Command* show_menu(void*);

Command* show_opening(void*);

extern Command const show_glade;

extern Command const show_well;

extern Command const well_raise_bucket_command;

extern Command const show_options;

int print_diastr(char const* const str);

typedef struct Sudoku_command
{
    Command command;
    int board[9][9]; //NOLINT
} Sudoku;

Command* paint_sudoku(void* this);

#endif
