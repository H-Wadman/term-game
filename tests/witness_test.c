#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "games/witness.h"
#include "vec.h"

typedef enum Dir
{
    dir_up,
    dir_right,
    dir_down,
    dir_left
} Dir;

bool wit_coord_valid_sq(Witness* wc, coord c);
bool wit_coord_valid_grid(Witness* wc, coord c);
Vec_coord get_area(Witness* wc, coord c);
coord step(coord c, Dir d);
coord get_scr_pos(coord c);
Sq get(Witness* wc, coord c);
void get_walls(coord c, coord cs[2], Dir d);
Dir get_direction(coord from, coord to);

//NOLINTBEGIN
void test_wit_coord_valid_sq(void)
{
    Witness wc = {NULL, .height = 3, .width = 9};

    assert(wit_coord_valid_sq(&wc, (coord){0, 0}));
    assert(wit_coord_valid_sq(&wc, (coord){1, 0}));
    assert(wit_coord_valid_sq(&wc, (coord){2, 0}));
    assert(!wit_coord_valid_sq(&wc, (coord){3, 0}));

    assert(wit_coord_valid_sq(&wc, (coord){2, 8}));
    assert(!wit_coord_valid_sq(&wc, (coord){2, 9}));
    assert(!wit_coord_valid_sq(&wc, (coord){3, 8}));
    assert(!wit_coord_valid_sq(&wc, (coord){3, 9}));
}

void test_wit_coord_valid_grid(void)
{
    Witness wc = {.width = 6, .height = 9};
    assert(wit_coord_valid_grid(&wc, (coord){.x = 0, .y = 9}));
    assert(wit_coord_valid_grid(&wc, (coord){.x = 6, .y = 9}));
    assert(!wit_coord_valid_grid(&wc, (coord){.x = 6, .y = 10}));
    assert(wit_coord_valid_grid(&wc, (coord){.x = 5, .y = 5}));
    assert(wit_coord_valid_grid(&wc, (coord){.x = 0, .y = 0}));
    assert(!wit_coord_valid_grid(&wc, (coord){.x = -1, .y = 0}));
    assert(!wit_coord_valid_grid(&wc, (coord){.x = -1, .y = 9}));

    wc.width = 3;
    assert(wit_coord_valid_grid(&wc, (coord){.x = 3, .y = 9}));
    assert(!wit_coord_valid_grid(&wc, (coord){.x = 4, .y = 9}));
    assert(wit_coord_valid_grid(&wc, (coord){.x = 0, .y = 9}));
}

void test_get(void)
{
    Witness wc = {.width = 6, .height = 8};
    wc.board   = malloc(wc.width * wc.height * sizeof(Sq));
    memset(wc.board, 0, wc.width * wc.height * sizeof(Sq));
    wc.board[0] = (Sq){.group.color = 1};
    Sq s        = get(&wc, (coord){0, 0});
    assert(s.group.color == 1);
    assert(s.group.symbol[0] == '\0');

    for (int i = 0; i < wc.height; ++i) {
        for (int j = 0; j < wc.width; ++j) {
            if (i == 0 && j == 0) { continue; }
            Sq s = get(&wc, (coord){.y = i, .x = j});
            assert(s.group.color == 0);
            assert(strcmp(s.group.symbol, "") == 0);
            for (int i = 0; i < 4; ++i) { assert(s.walls[i] == 0); }
        }
    }
}

void test_step(void)
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

void test_get_walls(void)
{
    Witness wc = {.width = 4, .height = 3};
    wc.board   = malloc(wc.width * wc.height * sizeof(Sq));
    memset(wc.board, 0, wc.width * wc.height * sizeof(Sq));

    coord cs[2];
    get_walls((coord){0, 0}, cs, dir_right);
    assert(wit_coord_valid_sq(&wc, cs[0]) || wit_coord_valid_sq(&wc, cs[1]));
    assert(!wit_coord_valid_sq(&wc, cs[0]) || !wit_coord_valid_sq(&wc, cs[1]));

    coord valid = wit_coord_valid_sq(&wc, cs[0]) ? cs[0] : cs[1];
    assert(valid.y == 0 && valid.x == 0);

    get_walls((coord){0, 0}, cs, dir_down);
    assert(wit_coord_valid_sq(&wc, cs[0]) || wit_coord_valid_sq(&wc, cs[1]));
    assert(!wit_coord_valid_sq(&wc, cs[0]) || !wit_coord_valid_sq(&wc, cs[1]));

    valid = wit_coord_valid_sq(&wc, cs[0]) ? cs[0] : cs[1];
    assert(valid.y == 0 && valid.x == 0);

    get_walls((coord){0, 0}, cs, dir_up);
    assert(!wit_coord_valid_sq(&wc, cs[0]) && !wit_coord_valid_sq(&wc, cs[1]));

    for (int i = 1; i < wc.height - 1; ++i) {
        for (int j = 1; j < wc.width - 1; ++j) {
            for (int d = 0; d < 4; ++d) {
                get_walls((coord){i, j}, cs, (Dir)d);
                assert(wit_coord_valid_sq(&wc, cs[0]));
                assert(wit_coord_valid_sq(&wc, cs[1]));
            }
        }
    }

    get_walls((coord){2, 1}, cs, dir_right);
    assert((cs[0].y == 1 && cs[0].x == 1) || (cs[1].y == 1 && cs[1].x == 1));
    assert((cs[0].y == 2 && cs[0].x == 1) || (cs[1].y == 2 && cs[1].x == 1));
}

void test_get_direction(void)
{
    coord o  = {0, 0};
    coord up = {-1, 0};
    assert(get_direction(o, up) == dir_up);
    coord down = {1, 0};
    assert(get_direction(o, down) == dir_down);
    coord left = {0, -1};
    assert(get_direction(o, left) == dir_left);
    coord right = {0, 1};
    assert(get_direction(o, right) == dir_right);
}

//Needs more tests
void test_get_area_full_board(void)
{
    Sq board[3][2];
    memset(&board, 0, sizeof board);
    Witness wc   = {(Sq*)board, .height = 3, .width = 2};
    Vec_coord v1 = get_area(&wc, (coord){0, 0});

    assert(v1.sz == 6);

    Sq board2[3][4];
    memset(&board2, 0, sizeof board2);
    Witness wc2  = {(Sq*)board2, .height = 3, .width = 4};
    Vec_coord v2 = get_area(&wc2, (coord){0, 0});

    assert(v2.sz == 12);

    Sq board5[3][5];
    memset(&board5, 0, sizeof board5);
    Witness wc5  = {(Sq*)board5, .height = 3, .width = 5};
    Vec_coord v5 = get_area(&wc5, (coord){0, 0});

    assert(v5.sz == 15);

    Sq board3[3][6];
    memset(&board3, 0, sizeof board3);
    Witness wc3  = {(Sq*)board3, .height = 3, .width = 6};
    Vec_coord v3 = get_area(&wc3, (coord){0, 0});

    assert(v3.sz == 18);

    Sq board4[3][9];
    memset(&board4, 0, sizeof board4);
    Witness wc4  = {(Sq*)board4, .height = 3, .width = 9};
    Vec_coord v4 = get_area(&wc4, (coord){0, 0});

    assert(v4.sz == 27);

    free_vec(&v1);
    free_vec(&v2);
    free_vec(&v3);
    free_vec(&v4);
}

void test_get_area_regions(void)
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
    board[0][0].walls[dir_up]   = we_empty;
    board[0][0].walls[dir_left] = we_empty;

    board[1][0].walls[dir_up] = we_empty;
    Witness wc                = {(Sq*)board, .height = 3, .width = 2};
    Vec_coord v1              = get_area(&wc, (coord){0, 0});

    assert(v1.sz == 2);
    assert(VEC_CONTAINS(v1, ((coord){0, 0})));
    assert(VEC_CONTAINS(v1, ((coord){1, 0})));

    free_vec(&v1);

    v1 = get_area(&wc, (coord){0, 1});

    assert(v1.sz == 1);

    free_vec(&v1);

    v1 = get_area(&wc, (coord){1, 1});

    assert(v1.sz == 1);
}

void test_get_scr_pos(void)
{
    coord c = {0, 0};
    coord p = get_scr_pos(c);
    assert(p.y == 0 && p.x == 0);

    coord c2 = {0, 1};
    coord p2 = get_scr_pos(c2);
    assert(p2.y == 0 && p2.x == 4);

    coord c3 = {0, 2};
    coord p3 = get_scr_pos(c3);
    assert(p3.y == 0 && p3.x == 8);

    coord c4 = {0, 3};
    coord p4 = get_scr_pos(c4);
    assert(p4.y == 0 && p4.x == 12);

    coord c5 = {1, 0};
    coord p5 = get_scr_pos(c5);
    assert(p5.y == 2 && p5.x == 0);

    coord c6 = {2, 0};
    coord p6 = get_scr_pos(c6);
    assert(p6.y == 4 && p6.x == 0);

    coord c7 = {3, 0};
    coord p7 = get_scr_pos(c7);
    assert(p7.y == 6 && p7.x == 0);

    coord c8 = {1, 1};
    coord p8 = get_scr_pos(c8);
    assert(p8.y == 2 && p8.x == 4);
}

//NOLINTEND

void test(void)
{
    test_wit_coord_valid_sq();
    test_wit_coord_valid_grid();
    test_step();
    test_get_area_full_board();
    test_get_area_regions();
    test_get_scr_pos();
    test_get();
    test_get_walls();
    test_get_direction();
}

int main(void) { test(); }
