#include <assert.h>
#include <stdbool.h>

#include "games/sudoku.h"

bool valid_row(int const* board, int r);
bool valid_col(int const* board, int c);
bool valid_sq(int const* board, int y, int x);

//NOLINTBEGIN
Sudoku sc_solved __attribute__((unused)) = {
    .command = (Command){.execute = paint_sudoku},
    .board   = {
                         {0, 8, 5, 4, 7, 9, 1, 3, 2},
                         {7, 3, 4, 1, 6, 2, 5, 9, 8},
                         {2, 1, 9, 5, 3, 8, 7, 6, 4},
                         {9, 2, 6, 3, 4, 5, 8, 7, 1},
                         {8, 5, 1, 7, 2, 6, 3, 4, 9},
                         {4, 7, 3, 8, 9, 1, 2, 5, 6},
                         {3, 4, 2, 6, 8, 7, 9, 1, 5},
                         {5, 6, 8, 9, 1, 3, 4, 2, 7},
                         {1, 9, 7, 2, 5, 4, 6, 8, 3},
                         }
};
Sudoku sc = {
    .command = (Command){.execute = paint_sudoku},
    .board   = {
                         {6, 0, 0, 0, 7, 9, 0, 3, 2},
                         {0, 0, 0, 0, 6, 0, 5, 0, 0},
                         {2, 0, 9, 0, 0, 8, 7, 0, 0},
                         {9, 0, 6, 3, 0, 5, 0, 0, 1},
                         {8, 5, 0, 0, 0, 0, 3, 0, 0},
                         {4, 7, 3, 0, 0, 1, 2, 5, 0},
                         {0, 4, 2, 6, 8, 0, 9, 0, 0},
                         {0, 0, 0, 0, 1, 3, 4, 2, 7},
                         {0, 9, 0, 2, 0, 0, 6, 0, 0},
                         }
};

void general_test()
{
    int test[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    assert(!valid_row(test, 0));
    test[0] = 9;
    assert(valid_row(test, 0));
    test[8] = 9;
    assert(!valid_row(test, 0));

    int test2[3][9] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 1, 2, 3, 4, 5, 6}
    };
    assert(valid_sq((int*)test2, 0, 0));
    assert(valid_sq((int*)test2, 0, 1));
    assert(valid_sq((int*)test2, 0, 2));
}

void test_valid_row()
{
    {
        int test[9] = {0};
        assert(!valid_row(test, 0));
    }
    int test[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(valid_row(test, 0));
    test[2] = 2;
    assert(!valid_row(test, 0));
    test[2] = 0;
    assert(!valid_row(test, 0));

    assert(!valid_row((int*)sc_solved.board, 0));

    for (int i = 1; i < 9; ++i) { assert(valid_row((int*)sc_solved.board, i)); }
}

void test_valid_col()
{
    assert(!valid_col((int*)sc_solved.board, 0));

    for (int i = 1; i < 9; ++i) { assert(valid_col((int*)sc_solved.board, i)); }
}

void test_valid_sq()
{
    assert(!valid_sq((int*)sc_solved.board, 0, 0));

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 0 && j == 0) { continue; }
            assert(valid_sq((int*)sc_solved.board, i, j));
        }
    }
}

void test()
{
    general_test();
    test_valid_row();
    test_valid_col();
    test_valid_sq();
}

//NOLINTEND
int main() { test(); }
