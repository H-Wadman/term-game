#include <assert.h>
#include <string.h>

#include "menu.h"
#include "utf8.h"
#include "vec.h"

enum Witness_enum
{
    we_dot,
    we_filled,
    we_empty
};

typedef enum Dir
{
    dir_up,
    dir_right,
    dir_down,
    dir_left
} Dir;

typedef struct Square
{
    int color;
    char symbol[ASCII_BUF_SZ];
    enum Witness_enum walls[4];
} Sq;

typedef struct Witness_command
{
    Command c;
    Sq* board;
    int height;
    int width;
    Vec_coord pos;
    coord end;
} Witness_command;

bool wit_coord_valid_sq(Witness_command* wc, coord c);
Vec_coord get_area(Witness_command* wc, coord c);
coord step(coord c, Dir d);

//NOLINTBEGIN
void test_wit_coord_valid()
{
    Witness_command wc = {{0}, NULL, .height = 3, .width = 9};

    assert(wit_coord_valid_sq(&wc, (coord){0, 0}));
    assert(wit_coord_valid_sq(&wc, (coord){1, 0}));
    assert(wit_coord_valid_sq(&wc, (coord){2, 0}));
    assert(!wit_coord_valid_sq(&wc, (coord){3, 0}));

    assert(wit_coord_valid_sq(&wc, (coord){2, 8}));
    assert(!wit_coord_valid_sq(&wc, (coord){2, 9}));
    assert(!wit_coord_valid_sq(&wc, (coord){3, 8}));
    assert(!wit_coord_valid_sq(&wc, (coord){3, 9}));
}

void test_step()
{
    coord c = {2, 9};
    coord s = step(c, dir_up);
    assert(s.x == c.x);
    assert(s.y + 1 == c.y);

    s = step(s, dir_down);
    assert(s.x == c.x);
    assert(s.y == c.y);

    s = step(s, dir_left);
    assert(s.y == c.y);
    assert(s.x + 1 == c.x);

    s = step(s, dir_right);
    assert(s.x == c.x);
    assert(s.y == c.y);
}

void test_get_area_full_board()
{
    Sq board[3][2];
    memset(&board, 0, sizeof board);
    Witness_command wc = {{0}, (Sq*)board, .height = 3, .width = 2};
    Vec_coord v1       = get_area(&wc, (coord){0, 0});

    assert(v1.sz == 6);

    Sq board2[3][4];
    memset(&board2, 0, sizeof board2);
    Witness_command wc2 = {{0}, (Sq*)board2, .height = 3, .width = 4};
    Vec_coord v2        = get_area(&wc2, (coord){0, 0});

    assert(v2.sz == 12);

    Sq board5[3][5];
    memset(&board5, 0, sizeof board5);
    Witness_command wc5 = {{0}, (Sq*)board5, .height = 3, .width = 5};
    Vec_coord v5        = get_area(&wc5, (coord){0, 0});

    assert(v5.sz == 15);

    Sq board3[3][6];
    memset(&board3, 0, sizeof board3);
    Witness_command wc3 = {{0}, (Sq*)board3, .height = 3, .width = 6};
    Vec_coord v3        = get_area(&wc3, (coord){0, 0});

    assert(v3.sz == 18);

    Sq board4[3][9];
    memset(&board4, 0, sizeof board4);
    Witness_command wc4 = {{0}, (Sq*)board4, .height = 3, .width = 9};
    Vec_coord v4        = get_area(&wc4, (coord){0, 0});

    assert(v4.sz == 27);

    free_vec(&v1);
    free_vec(&v2);
    free_vec(&v3);
    free_vec(&v4);
}

void test_get_area_regions()
{
    Sq board[2][2];
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            board[i][j] = (Sq){
                .walls = {we_filled, we_filled, we_filled, we_filled}
            };
        }
    }

    board[0][0].walls[dir_down] = we_empty;
    board[1][0].walls[dir_up]   = we_empty;
    Witness_command wc          = {{0}, (Sq*)board, .height = 3, .width = 2};
    Vec_coord v1                = get_area(&wc, (coord){0, 0});

    assert(v1.sz == 2);
    assert(vec_contains(v1, ((coord){0, 0})));
    assert(vec_contains(v1, ((coord){1, 0})));

    free_vec(&v1);

    v1 = get_area(&wc, (coord){0, 1});

    assert(v1.sz == 1);

    free_vec(&v1);

    v1 = get_area(&wc, (coord){1, 1});

    assert(v1.sz == 1);
}

//NOLINTEND

void test()
{
    test_wit_coord_valid();
    test_step();
    test_get_area_full_board();
    test_get_area_regions();
}

int main() { test(); }
