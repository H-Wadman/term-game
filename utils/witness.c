#include <stdlib.h>

#include "menu.h"
#include "utf8.h"
#include "vec.h"

#define COLOR_NONE 0

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
} Witness_command;

bool wit_coord_valid(Witness_command* wc, coord c)
{
    return c.y >= 0 && c.x >= 0 && c.y < wc->height && c.x < wc->width;
}

Sq get(Witness_command* wc, coord c)
{
    if (!wit_coord_valid(wc, c)) {
        fprintf(stderr, //NOLINT
                "Out of bounds access by get (y = %d, x = %d) on witness board "
                "of dimensions %dx%d\n",
                c.y, c.x, wc->height, wc->width);
        exit(1);
    }
    return wc->board[c.y * wc->width + c.x];
}

void print_witness_line(WINDOW* win, Witness_command* wc, int line)
{
    wmove(win, line, 0);
    if (line == 0) {
        waddstr(win, "╔═══");
        for (int i = 1; i < wc->width; ++i) { waddstr(win, "╦═══"); }
        waddstr(win, "╗");
        return;
    }
    if (line == wc->height - 1) {
        waddstr(win, "╚═══");
        for (int i = 1; i < wc->width; ++i) { waddstr(win, "╩═══"); }
        waddstr(win, "╝");
        return;
    }

    switch (line % 3) {
        case 0:
            waddstr(win, "╠───");
            for (int i = 1; i < wc->width; ++i) { waddstr(win, "┼───"); }
            waddstr(win, "╣");
            return;
        case 1:
        case 2:
            waddstr(win, "║   ");
            for (int i = 1; i < wc->width; ++i) { waddstr(win, "│   "); }
            waddstr(win, "║");
            return;
    }
}

coord step(coord c, Dir d)
{
    switch (d) {
        case dir_up   : return (coord){.x = c.x, .y = c.y - 1};
        case dir_right: return (coord){.x = c.x + 1, .y = c.y};
        case dir_down : return (coord){.x = c.x, .y = c.y + 1};
        case dir_left : return (coord){.x = c.x - 1, .y = c.y};
        default:
            fprintf(stderr, "Non-valid Dir value passed to step\n"); //NOLINT
            exit(1);
    }
}

Vec_coord get_area(Witness_command* wc, coord c)
{
    int const init_cap   = 16;
    Vec_coord res        = new_vec_coord(init_cap);
    Vec_coord back_track = new_vec_coord(init_cap);
    vec_push(&back_track, c);

    while (back_track.sz > 0) {
        coord curr = vec_pop(&back_track);
        vec_push(&res, curr);

        for (int i = 0; i < 4; ++i) {
            coord s = step(curr, i);
            if (!wit_coord_valid(wc, s)) { continue; }
            if (get(wc, curr).walls[i] == we_filled) { continue; }
            if (!vec_contains(back_track, s) && !vec_contains(res, s)) {
                vec_push(&back_track, s);
            }
        }
    }

    free_vec(&back_track);
    return res;
}

bool witness_is_solved(Witness_command* wc)
{
    Vec_coord visited = new_vec_coord(wc->width * wc->height);
    for (int i = 0; i < wc->height; ++i) {
        for (int j = 0; j < wc->width; ++j) {
            coord temp = {i, j};
            if (vec_contains(visited, temp)) { continue; }

            Vec_coord v = get_area(wc, temp);
            //Verify that all squares in the same area are of same color (or
            //colorless)
            int color = -1;
            while (v.sz > 0) {
                coord c = vec_pop(&v);
                int clr = get(wc, c).color;
                if (clr != COLOR_NONE) {
                    if (color == -1) { color = clr; }
                    else if (color != clr) {
                        return false;
                    }
                }
            }

            free_vec(&v);
        }
    }

    free_vec(&visited);
    return false;
}

WINDOW* print_witness_board(Witness_command* wc)
{
    int const ht = 1 + 2 * wc->height;
    int const wd = 1 + 4 * wc->width;
    WINDOW* win  = newwin(ht, wd, (LINES - ht) / 2, (COLS - wd) / 2);
    for (int i = 0; i < wc->height; ++i) { print_witness_line(win, wc, i); }

    return win;
}

/*
Func paint_witness(void* this)
{
    Witness_command* wc = (Witness_command*)this;
    WINDOW* win         = print_witness_board(wc);

    int y = 0;
    int x = 0;
}
*/
