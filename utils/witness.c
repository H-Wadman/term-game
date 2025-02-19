#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>

#include "color.h"
#include "menu.h"
#include "utf8.h"
#include "vec.h"

enum Witness_enum
{
    we_dot,
    we_filled,
    we_empty
};

char const* we_enum_to_str(enum Witness_enum we)
{
    switch (we) {
        case we_dot   : return "dot";
        case we_filled: return "filled";
        case we_empty : return "empty";
    }
}

typedef enum Dir
{
    dir_up,
    dir_right,
    dir_down,
    dir_left
} Dir;

char const* dir_to_str(Dir d)
{
    switch (d) {
        case dir_up   : return "up";
        case dir_down : return "down";
        case dir_left : return "left";
        case dir_right: return "right";
    }
}

Dir opposite(Dir d)
{
    switch (d) {
        case dir_up   : return dir_down;
        case dir_right: return dir_left;
        case dir_left : return dir_right;
        case dir_down : return dir_up;
        default       : assert(false);
    }
}

typedef struct Group
{
    enum color color;
    char symbol[ASCII_BUF_SZ];
} Group;

Group const groups[] = {
    {.color = col_default,  .symbol = ""},
    { .color = col_yellow, .symbol = "✪"},
    {  .color = col_green, .symbol = "⌘"},
    {    .color = col_red, .symbol = "֍"}
};

typedef struct Square
{
    Group group;
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

coord get_scr_pos(coord c) { return (coord){.x = 4 * c.x, .y = 2 * c.y}; }

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

Sq* get_p(Witness_command* wc, coord c)
{
    if (!wit_coord_valid_sq(wc, c)) {
        fprintf(stderr, //NOLINT
                "Out of bounds access by get (y = %d, x = %d) on witness board "
                "of dimensions %dx%d\n",
                c.y, c.x, wc->height, wc->width);
        exit(1);
    }
    return &wc->board[c.y * wc->width + c.x];
}

void print_witness_line(WINDOW* win, Witness_command* wc, int line)
{
    wmove(win, line, 0);
    if (line == 0) {
        waddstr(win, "┌───");
        for (int i = 1; i < wc->width; ++i) { waddstr(win, "┬───"); }
        waddstr(win, "┐");
        return;
    }
    if (line == 2 * wc->height) {
        waddstr(win, "└───");
        for (int i = 1; i < wc->width; ++i) { waddstr(win, "┴───"); }
        waddstr(win, "┘");
        return;
    }

    switch (line % 2) {
        case 0:
            waddstr(win, "├───");
            for (int i = 1; i < wc->width; ++i) { waddstr(win, "┼───"); }
            waddstr(win, "┤");
            return;
        case 1:
            waddstr(win, "│   ");
            for (int i = 1; i < wc->width; ++i) { waddstr(win, "│   "); }
            waddstr(win, "│");
            return;
        default:
            fprintf(stderr, //NOLINT
                    "Impossible branch reached, aborting...\n");
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

//Returns coords such that when d is up/down cs[0] is to the left and cs[1] is
//to the right and when d is left/right cs[0] is up and cs[1] is down
void get_walls(coord c, coord cs[2], Dir d)
{
    switch (d) {
        case dir_up:
            cs[0] = (coord){c.y - 1, c.x - 1};
            cs[1] = (coord){c.y - 1, c.x};
            return;
        case dir_down:
            cs[0] = (coord){c.y, c.x - 1};
            cs[1] = (coord){c.y, c.x};
            return;
        case dir_right:
            cs[0] = (coord){c.y - 1, c.x};
            cs[1] = (coord){c.y, c.x};
            return;
        case dir_left:
            cs[0] = (coord){c.y - 1, c.x - 1};
            cs[1] = (coord){c.y, c.x - 1};
            return;
        default:
            fprintf(stderr, "Non-valid Dir value passed to %s\n", //NOLINT
                    __func__);
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
                int clr = get(wc, c).group.color;
                if (clr != col_default) {
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

WINDOW* create_witness_win(Witness_command* wc)
{
    int const ht = 1 + 2 * wc->height;
    int const wd = 1 + 4 * wc->width;
    WINDOW* win  = newwin(ht, wd, (LINES - ht) / 2, (COLS - wd) / 2);
    intrflush(win, false);
    keypad(win, true);

    return win;
}

void paint_witness_board(Witness_command* wc, WINDOW* win)
{
    for (int i = 0; i <= wc->height * 2; ++i) {
        print_witness_line(win, wc, i);
    }

    for (int i = 0; i < wc->height; ++i) {
        for (int j = 0; j < wc->width; ++j) {
            Sq s = get(wc, (coord){i, j});
            char const* ch =
                (utf8_strlen(s.group.symbol) == 1) ? s.group.symbol : " ";
            coord scr_pos = get_scr_pos((coord){i, j});
            wattron(win, COLOR_PAIR(s.group.color));
            mvwaddstr(win, scr_pos.y + 1, scr_pos.x + 2, ch);
            wattroff(win, COLOR_PAIR(s.group.color));
        }
    }
}

//Has to pass coords directly below/above or to the right/left of each other
Dir get_direction(coord from, coord to)
{
    coord c = {to.y - from.y, to.x - from.x};

    if (!(c.x == 0 || c.y == 0)) {
        fprintf(stderr, //NOLINT
                "Invalid coords from = {%d, %d}, to = {%d, %d}, passed to %s",
                from.y, from.x, to.y, to.x, __func__);
    }
    if (!(abs(c.x) == 1 || abs(c.y) == 1)) {
        fprintf(stderr, //NOLINT
                "Invalid coords from = {%d, %d}, to = {%d, %d}, passed to %s",
                from.y, from.x, to.y, to.x, __func__);
    }
    if (c.y == 0 && c.x == 1) { return dir_right; }
    if (c.y == 0 && c.x == -1) { return dir_left; }
    if (c.y == 1 && c.x == 0) { return dir_down; }
    if (c.y == -1 && c.x == 0) { return dir_up; }

    fprintf(stderr, "Impossible branch reached in %s, aborting...\n", //NOLINT
            __func__);
    exit(1);
}

#define VERIFY_PAINT_CONDITIONS(wc, c, dir, point)                             \
    assert(wit_coord_valid_grid(wc, c));                                       \
    assert(wit_coord_valid_grid(wc, step(c, dir)));                            \
    assert(wit_coord_valid_grid(wc, step(step(c, dir), point)));               \
    assert((point) != opposite(dir));

void paint_up(Witness_command* wc, WINDOW* win, coord c, Dir point)
{
    VERIFY_PAINT_CONDITIONS(wc, c, dir_up, point);

    char const* final_pipe = NULL;
    switch (point) {
        case dir_up:
            if (c.x == wc->width) {
                final_pipe = "╢";
                break;
            }
            final_pipe = (c.x == 0) ? "╫" : "╟";
            break;
        case dir_left : final_pipe = "╗"; break;
        case dir_right: final_pipe = "╔"; break;
        default:
            fprintf(stderr, //NOLINT
                    "Invalid point direction in %s, aborting...\n", __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y - 2, scr_pos.x, final_pipe);
    mvwaddstr(win, scr_pos.y - 1, scr_pos.x, "║");
}

void paint_left(Witness_command* wc, WINDOW* win, coord c, Dir point)
{
    VERIFY_PAINT_CONDITIONS(wc, c, dir_left, point);
    char const* final_pipe = NULL;

    switch (point) {
        case dir_up: final_pipe = "╚"; break;
        case dir_left:
            if (c.y == 0) {
                final_pipe = "╤";
                break;
            }
            final_pipe = (c.y == wc->height) ? "╧" : "╪";
            break;
        case dir_down: final_pipe = "╔"; break;
        default:
            fprintf(stderr, //NOLINT
                    "Invalid point direction in %s, aborting...\n", __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y, scr_pos.x - 1, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x - 2, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x - 3, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x - 4, final_pipe);
}

void paint_right(Witness_command* wc, WINDOW* win, coord c, Dir point)
{
    VERIFY_PAINT_CONDITIONS(wc, c, dir_right, point);
    char const* final_pipe = NULL;

    switch (point) {
        case dir_up: final_pipe = "╝"; break;
        case dir_right:
            if (c.y == 0) {
                final_pipe = "╤";
                break;
            }
            final_pipe = (c.y == wc->height) ? "╧" : "╪";
            break;
        case dir_down: final_pipe = "╗"; break;
        default:
            fprintf(stderr, //NOLINT
                    "Invalid point direction in %s, aborting...\n", __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y, scr_pos.x + 1, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x + 2, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x + 3, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x + 4, final_pipe);
}

void paint_down(Witness_command* wc, WINDOW* win, coord c, Dir point)
{
    VERIFY_PAINT_CONDITIONS(wc, c, dir_down, point);

    char const* final_pipe = NULL;
    switch (point) {
        case dir_down:
            if (c.x == wc->width) {
                final_pipe = "╢";
                break;
            }
            final_pipe = (c.x == 0) ? "╟" : "╫";
            break;
        case dir_left : final_pipe = "╝"; break;
        case dir_right: final_pipe = "╚"; break;
        default:
            fprintf(stderr, //NOLINT
                    "Invalid point direction in %s, aborting...\n", __func__);
            exit(1);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y + 2, scr_pos.x, final_pipe);
    mvwaddstr(win, scr_pos.y + 1, scr_pos.x, "║");
}

void paint(Dir move, Witness_command* wc, WINDOW* win, coord c, Dir point)
{
    switch (move) {
        case dir_up   : paint_up(wc, win, c, point); break;
        case dir_left : paint_left(wc, win, c, point); break;
        case dir_right: paint_right(wc, win, c, point); break;
        case dir_down : paint_down(wc, win, c, point); break;
    }
}

void paint_last(Witness_command* wc, WINDOW* win)
{
    Vec_coord v   = wc->pos;
    coord scr_pos = get_scr_pos(v.data[v.sz - 2]);
    Dir move      = get_direction(v.data[v.sz - 2], v.data[v.sz - 1]);
    switch (move) {
        case dir_up: mvwaddstr(win, scr_pos.y - 1, scr_pos.x, "║"); break;
        case dir_left:
            mvwaddstr(win, scr_pos.y, scr_pos.x - 1, "═");
            mvwaddstr(win, scr_pos.y, scr_pos.x - 2, "═");
            mvwaddstr(win, scr_pos.y, scr_pos.x - 3, "═");
            break;
        case dir_right:
            mvwaddstr(win, scr_pos.y, scr_pos.x + 1, "═");
            mvwaddstr(win, scr_pos.y, scr_pos.x + 2, "═");
            mvwaddstr(win, scr_pos.y, scr_pos.x + 3, "═");
            break;
        case dir_down: mvwaddstr(win, scr_pos.y + 1, scr_pos.x, "║"); break;
        default      : assert(false);
    }
}

void paint_path(Witness_command* wc, WINDOW* win, enum color color)
{
    if (wc->pos.sz == 1) { return; }
    wattron(win, COLOR_PAIR(color));
    //For all but the last segment (i.e. the last two coords), the pipe drawn
    //depends on the next two coords
    Vec_coord v = wc->pos;
    for (int i = 0; i < v.sz - 2; ++i) {
        Dir move  = get_direction(v.data[i], v.data[i + 1]);
        Dir point = get_direction(v.data[i + 1], v.data[i + 2]);

        paint(move, wc, win, v.data[i], point);
    }

    //Handle the last segment separately
    paint_last(wc, win);
    wattroff(win, COLOR_PAIR(color));
}

/*
void print_vec_deb(Vec_coord v)
{
    if (v.sz == 0) { fprintf(stderr, "{}\n"); }
    fprintf(stderr, "{ {%d, %d}", v.data[0].y, v.data[0].x);

    for (int i = 1; i < v.sz; ++i) {
        fprintf(stderr, ", {%d, %d}", v.data[i].y, v.data[i].x);
    }

    fprintf(stderr, " }\n");
}
*/

bool is_backtrack(Witness_command* wc, Dir move)
{
    if (wc->pos.sz < 2) { return false; }

    return move == get_direction(wc->pos.data[wc->pos.sz - 1],
                                 wc->pos.data[wc->pos.sz - 2]);
}

void set_walls(Witness_command* wc, Dir d, enum Witness_enum we)
{
    coord cs[2];

    get_walls(vec_back(wc->pos), cs, d);

    if (wit_coord_valid_sq(wc, cs[0])) {
        Sq* s = get_p(wc, cs[0]);
        switch (d) {
            case dir_up:
            case dir_down : s->walls[dir_right] = we; break;
            case dir_left :
            case dir_right: s->walls[dir_down] = we; break;
            default:
                fprintf(stderr, "Non-valid Dir value passed to %s\n", //NOLINT
                        __func__);
                exit(1);
        }
    }

    if (wit_coord_valid_sq(wc, cs[1])) {
        Sq* s = get_p(wc, cs[1]);
        switch (d) {
            case dir_up:
            case dir_down : s->walls[dir_left] = we; break;
            case dir_left :
            case dir_right: s->walls[dir_up] = we; break;
            default:
                fprintf(stderr, "Non-valid Dir value passed to %s\n", //NOLINT
                        __func__);
                exit(1);
        }
    }
}

//Expects backtrack to be possible, undefined otherwise
void backtrack(Witness_command* wc)
{
    Dir d = get_direction(vec_back(wc->pos), wc->pos.data[wc->pos.sz - 2]);
    set_walls(wc, d, we_empty);
    vec_pop(&wc->pos);
}

Func play_witness(void* this)
{
    Witness_command* wc = (Witness_command*)this;
    WINDOW* win         = create_witness_win(wc);
    paint_witness_board(wc, win);
    wrefresh(win);

    while (!witness_is_solved(wc)) {
        int ch = wgetch(win);

        Dir next_dir = 0;
        bool move    = false;
        switch (ch) {
            case KEY_UP:
                move     = true;
                next_dir = dir_up;
                break;
            case KEY_LEFT:
                move     = true;
                next_dir = dir_left;
                break;
            case KEY_DOWN:
                move     = true;
                next_dir = dir_down;
                break;
            case KEY_RIGHT:
                move     = true;
                next_dir = dir_right;
                break;
            default:;
        }

        if (move) {
            if (is_backtrack(wc, next_dir)) { backtrack(wc); }
            else {
                coord next = step(vec_back(wc->pos), next_dir);
                //Guard against stepping outside the grid and
                //walking over already visited junctions
                if (wit_coord_valid_grid(wc, next) &&
                    !vec_contains(wc->pos, next)) {
                    set_walls(wc, next_dir, we_filled);
                    vec_push(&wc->pos, next);
                }
            }
            //Update screen
            paint_witness_board(wc, win);
            paint_path(wc, win, col_yellow);
            wrefresh(win);
        }
    }


    werase(win);
    wrefresh(win);
    delwin(win);

    return pop_func(NULL);
}

void test_play_witness()
{
    //NOLINTBEGIN
    assert(utf8_strlen("☘") == 1);

    Sq board[6][6] = {0};
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            board[i][j] = (Sq){
                .group = {.color = col_green, .symbol = "✪"},
                  .walls = {0}
            };
        }
    }
    board[0][0] = (Sq){
        .group = {.color = col_yellow, .symbol = "h"},
          .walls = {0}
    };
    Witness_command test_wc = {
        .c      = {0},
        .board  = (Sq*)board,
        .height = 6,
        .width  = 6,
        .pos    = new_vec_coord(36),
        .end    = {6, 6}
    };
    vec_push(&test_wc.pos, ((coord){0, 0}));

    play_witness(&test_wc);

    //NOLINTEND
}
