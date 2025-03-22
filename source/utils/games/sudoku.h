#pragma once

#include "base.h"

typedef struct Sudoku_command
{
    Command command;
    int board[9][9]; //NOLINT
} Sudoku;

Command* paint_sudoku(void* this);
