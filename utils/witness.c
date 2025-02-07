#include <assert.h>
#include <ncurses.h>
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
    Vec_coord pos;
    coord end;
} Witness_command;

bool wit_coord_valid_sq(Witness_command* wc, coord c)
{
    return c.y >= 0 && c.x >= 0 && c.y < wc->height && c.x < wc->width;
}

bool wit_coord_valid_grid(Witness_command* wc, coord c)
{
    return c.y >= 0 && c.x >= 0 && c.y <= wc->height && c.x <= wc->width;
}

Sq get(Witness_command* wc, coord c)
{
    if (!wit_coord_valid_sq(wc, c)) {
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
            if (!wit_coord_valid_sq(wc, s)) { continue; }
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
    if (vec_back(wc->pos).x != wc->end.x || vec_back(wc->pos).y != wc->end.y) {
        return false;
    }
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
            for (int i = 0; i < v.sz; ++i) { vec_push(&visited, v.data[i]); }

            free_vec(&v);
        }
    }

    free_vec(&visited);
    return true;
}

WINDOW* print_witness_board(Witness_command* wc)
{
    int const ht = 1 + 2 * wc->height;
    int const wd = 1 + 4 * wc->width;
    WINDOW* win  = newwin(ht, wd, (LINES - ht) / 2, (COLS - wd) / 2);
    for (int i = 0; i < wc->height; ++i) { print_witness_line(win, wc, i); }

    return win;
}

coord get_screen_pos(coord c) { return (coord){.x = 4 * c.x, .y = 2 * c.x}; }

/*void update_wit_board(Witness_command* wc, Dir d)
{
    {
        coord s = step(wc->pos, d);
        if (!wit_coord_valid_grid(wc, s)) { return; }
    }

    coord scr = get_screen_pos(wc->pos);

    switch (d) {
        case dir_up:;
    }
}*/

//Needs board to be > 2 in both dimensions
void paint_up_left(Witness_command* wc, WINDOW* win)
{
    assert(wc->pos.x == 0);
    if (!wit_coord_valid_grid(wc, step(wc->pos, dir_up))) {
        fprintf(stderr, "Move to invalid coord in paint_up_left\n"); //NOLINT
        exit(1);
    }
    coord const scr = get_screen_pos(wc->pos);

    //Going up to upper-left corner
    if (wc->pos.y == 1) {
        mvwaddstr(win, scr.y - 3, scr.x, "╓");
        mvwaddstr(win, scr.y - 2, scr.x, "║");
        mvwaddstr(win, scr.y - 1, scr.x, "║");
        //WRONG
        mvwaddstr(win, scr.y, scr.x, "╟");
        return;
    }

    //Going up from lower-left corner
    if (wc->pos.y == wc->height) {
        mvwaddstr(win, scr.y - 3, scr.x, "╟");
        mvwaddstr(win, scr.y - 2, scr.x, "║");
        mvwaddstr(win, scr.y - 1, scr.x, "║");
        mvwaddstr(win, scr.y, scr.x, "╙");
        return;
    }

    //Otherwise
    mvwaddstr(win, scr.y - 3, scr.x, "╟"); //Best we can do?
    mvwaddstr(win, scr.y - 2, scr.x, "║");
    mvwaddstr(win, scr.y - 1, scr.x, "║");
    mvwaddstr(win, scr.y, scr.x, "╟"); //WRONG can come from left
}

void paint_up_right(Witness_command* wc, WINDOW* win)
{
    assert(wc->pos.x == wc->width);

    if (!wit_coord_valid_grid(wc, step(wc->pos, dir_up))) {
        fprintf(stderr, "Move to invalid coord in paint_up_right\n"); //NOLINT
        exit(1);
    }
    coord const scr = get_screen_pos(wc->pos);

    //Going up to upper-right corner
    if (wc->pos.y == 1) {
        mvwaddstr(win, scr.y - 3, scr.x, "╖");
        mvwaddstr(win, scr.y - 2, scr.x, "║");
        mvwaddstr(win, scr.y - 1, scr.x, "║");
        mvwaddstr(win, scr.y, scr.x, "╢");
        return;
    }

    //Going up from lower-left corner
    if (wc->pos.y == wc->height) {
        mvwaddstr(win, scr.y - 3, scr.x, "╢");
        mvwaddstr(win, scr.y - 2, scr.x, "║");
        mvwaddstr(win, scr.y - 1, scr.x, "║");
        mvwaddstr(win, scr.y, scr.x, "╜");
        return;
    }

    //Otherwise
    mvwaddstr(win, scr.y - 3, scr.x, "╢");
    mvwaddstr(win, scr.y - 2, scr.x, "║");
    mvwaddstr(win, scr.y - 1, scr.x, "║");
    mvwaddstr(win, scr.y, scr.x, "╢");
}

void paint_up_middle(Witness_command* wc, WINDOW* win)
{
    if (!wit_coord_valid_grid(wc, step(wc->pos, dir_up))) {
        fprintf(stderr, "Move to invalid coord in paint_up_right\n"); //NOLINT
        exit(1);
    }
    coord const scr = get_screen_pos(wc->pos);

    //Going up to upper ledge corner
    if (wc->pos.y == 1) {
        mvwaddstr(win, scr.y - 3, scr.x, "╥");
        mvwaddstr(win, scr.y - 2, scr.x, "║");
        mvwaddstr(win, scr.y - 1, scr.x, "║");
        mvwaddstr(win, scr.y, scr.x, "╫");
        return;
    }

    //Going up from lower-left corner
    if (wc->pos.y == wc->height) {
        mvwaddstr(win, scr.y - 3, scr.x, "╢");
        mvwaddstr(win, scr.y - 2, scr.x, "║");
        mvwaddstr(win, scr.y - 1, scr.x, "║");
        mvwaddstr(win, scr.y, scr.x, "╨");
        return;
    }

    //Otherwise
    mvwaddstr(win, scr.y - 3, scr.x, "╫");
    mvwaddstr(win, scr.y - 2, scr.x, "║");
    mvwaddstr(win, scr.y - 1, scr.x, "║");
    mvwaddstr(win, scr.y, scr.x, "╫");
}

void paint_up(Witness_command* wc, WINDOW* win)
{
    if (wc->pos.x == 0) { paint_up_left(wc, win); }
    else if (wc->pos.x == wc->width) {
        paint_up_right(wc, win);
    }
    else {
        paint_up_middle(wc, win);
    }

    Sq s = get(wc, wc->pos);
}

Func paint_witness(void* this)
{
    Witness_command* wc = (Witness_command*)this;
    WINDOW* win         = print_witness_board(wc);
    intrflush(win, false);
    keypad(win, true);

    int y = 0;
    int x = 0;

    while (!witness_is_solved(wc)) {
        int ch = wgetch(win);

        switch (ch) {
            case KEY_UP:
            case KEY_LEFT:
            case KEY_DOWN:
            case KEY_RIGHT:
            default       :;
        }
    }


    werase(win);
    wrefresh(win);
    delwin(win);

    return pop_func(NULL);
}
