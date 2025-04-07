/*!
 * \file witness.c
 * \brief Implementation file for witness.h
 *
 *
 */
#include <assert.h>
#include <ncurses.h>
#include <stddef.h>
#include <stdlib.h>

#include "base.h"
#include "io/logging.h"
#include "io/utf8.h"
#include "vec.h"
#include "witness.h"

//! Commandtion for converting enum to string literal
char const* we_enum_to_str(enum Witness_enum we)
{
    switch (we) {
        case we_dot   : return "dot";
        case we_filled: return "filled";
        case we_empty : return "empty";
        default       : log_and_exit("Invalid enum value passed to %s\n", __func__);
    }
}

//! Direction
typedef enum Dir
{
    dir_up,
    dir_right,
    dir_down,
    dir_left
} Dir;

//! Commandtion for converting enum to string literal
char const* dir_to_str(Dir d)
{
    switch (d) {
        case dir_up   : return "up";
        case dir_down : return "down";
        case dir_left : return "left";
        case dir_right: return "right";
        default       : log_and_exit("Invalid enum value passed to %s\n", __func__);
    }
}

//! Returns the opposite direction of the one passed in
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

//! Example groups to be used in the game
Group const groups[] = {
    {.color = col_default,  .symbol = ""},
    { .color = col_yellow, .symbol = "✪"},
    {  .color = col_green, .symbol = "⌘"},
    {    .color = col_red, .symbol = "֍"}
};

/*!
 * \brief Returns the coordinate position in the witness window for the *corner*
 * at coordinate c
 *
 * \param[in] c Coordinate calculated on the board
 *
 * \returns Equivalent coordinate on the screen
 *
 * For example, the upper left corner of the square in the _first_ row, _second_
 * column is denoted (0, 1). It's position is get_scr_pos((0, 1)) = (0, 4)
 */
coord get_scr_pos(coord c) { return (coord){.x = 4 * c.x, .y = 2 * c.y}; }

/*!
 * \brief Checks if a coordinate refers to a valid square within a witness board
 *
 * \param[in] wc The witness command that contains the witness board in use
 * \param[in] c The coord to check for validity
 *
 * \returns true if the coord refers to a valid square on the board, false
 * otherwise
 */
bool wit_coord_valid_sq(Witness* wc, coord c)
{
    return c.y >= 0 && c.x >= 0 && c.y < wc->height && c.x < wc->width;
}

/*!
 * \brief Checks if a coordinate refers to a valid grid intersection within a
 * witness board
 *
 * Note that (0, 0) refers to the upper left corner of the square with the same
 * coordinates. (1, 1) refers to the lower right corner of the same square, that
 * is the upper left corner of the square with coordinates (1, 1).
 *
 * \param[in] wc The witness command that contains the witness board in use
 * \param[in] c The coord to check for validity
 *
 * \returns true if the coord refers to
 */
bool wit_coord_valid_grid(Witness* wc, coord c)
{
    return c.y >= 0 && c.x >= 0 && c.y <= wc->height && c.x <= wc->width;
}

/*!
 * \brief Retrieves a copy of the square at the specified coordinate
 *
 * \param[in] wc The witness command that holds the board to access
 * \param[in] c The coordinate of the square we want to read
 *
 * \returns A copy of the specified square (in other words wc->board[c.y *
 * wc->width + c.x * wc->height])
 */
Sq get(Witness* wc, coord c)
{
    if (!wit_coord_valid_sq(wc, c)) {
        log_and_exit(
            "Out of bounds access by get (y = %d, x = %d) on witness board "
            "of dimensions %dx%d\n",
            c.y, c.x, wc->height, wc->width);
    }
    return wc->board[c.y * wc->width + c.x];
}

//! Pointer version of \ref get
Sq* get_p(Witness* wc, coord c)
{
    if (!wit_coord_valid_sq(wc, c)) {
        log_and_exit(
            "Out of bounds access by get (y = %d, x = %d) on witness board "
            "of dimensions %dx%d\n",
            c.y, c.x, wc->height, wc->width);
    }
    return &wc->board[c.y * wc->width + c.x];
}

/*!
 * \brief Print a part of the witness *board* (i.e. just the walls)
 *
 * Only one row of characters will be printed. To print the entire board
 * (including paths and groups) use \ref paint_witness_board
 *
 * \param[out] win The window to print on
 * \param[in] wc The Witness_command specifying the dimensions of the board
 * \param[in] line The index of the line to print
 */
void print_witness_line(WINDOW* win, Witness* wc, int line)
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
        default: log_and_exit("Impossible branch reached, aborting...\n");
    }
}

//! Return the coordinate obtained by stepping one step in direction d from
//! coord c
coord step(coord c, Dir d)
{
    switch (d) {
        case dir_up   : return (coord){.x = c.x, .y = c.y - 1};
        case dir_right: return (coord){.x = c.x + 1, .y = c.y};
        case dir_down : return (coord){.x = c.x, .y = c.y + 1};
        case dir_left : return (coord){.x = c.x - 1, .y = c.y};
        default       : log_and_exit("Non-valid Dir value passed to step\n");
    }
}

/*!
 * \brief Returns the coordinates of the squares passed by when affecting a
 * movement
 *
 * Loads the coordinates of the squares passed by when walking from coord c in
 * direction d
 *
 * \param[in]  c The coordinate (of a grid position, not a square)
 * \param[out] cs A coord buffer to load into
 * \param[in]  d The direction to go from c
 *
 * The coords are loaded such that when d is up/down cs[0] is to the left and
 * cs[1] is to the right and when d is left/right cs[0] is up and cs[1] is down
 */
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
        default: log_and_exit("Non-valid Dir value passed to %s\n", __func__);
    }
}

/*!
 * \brief Returns all the squares connected to a specific square
 *
 * \param[in] wc The witness whose board we wish to inspect
 * \param[in] c The coordinate of the square to inspect
 *
 * \returns A vector of coordinates of squares that are connected to the one
 * specified (i.e have not been sectioned off from each other by the players
 * path)
 */
Vec_coord get_area(Witness* wc, coord c)
{
    int const init_cap   = 16;
    Vec_coord res        = new_vec_coord(init_cap);
    Vec_coord back_track = new_vec_coord(init_cap);
    VEC_PUSH(&back_track, c);

    while (back_track.sz > 0) {
        coord curr = VEC_POP(&back_track);
        VEC_PUSH(&res, curr);

        for (int i = 0; i < 4; ++i) {
            coord s = step(curr, i);
            if (!wit_coord_valid_sq(wc, s)) { continue; }
            if (get(wc, curr).walls[i] == we_filled) { continue; }
            if (!VEC_CONTAINS(back_track, s) && !VEC_CONTAINS(res, s)) {
                VEC_PUSH(&back_track, s);
            }
        }
    }

    free_vec(&back_track);
    return res;
}

/*!
 * \brief Verifies if a witness puzzle has been solved
 *
 * Checks if the witness puzzle has been correctly divided and if the player has
 * reached the end (notably does not verify if all points have been acquired
 * yet).
 */
bool witness_is_solved(Witness* wc)
{
    if (VEC_BACK(wc->pos).x != wc->end.x || VEC_BACK(wc->pos).y != wc->end.y) {
        return false;
    }
    Vec_coord visited = new_vec_coord(wc->width * wc->height);
    for (int i = 0; i < wc->height; ++i) {
        for (int j = 0; j < wc->width; ++j) {
            coord temp = {i, j};
            if (VEC_CONTAINS(visited, temp)) { continue; }

            Vec_coord v = get_area(wc, temp);
            //Verify that all squares in the same area are of same color (or
            //colorless)
            int color = -1;
            while (v.sz > 0) {
                coord c = VEC_POP(&v);
                int clr = get(wc, c).group.color;
                if (clr != col_default) {
                    if (color == -1) { color = clr; }
                    else if (color != clr) {
                        return false;
                    }
                }
            }
            for (int i = 0; i < v.sz; ++i) { VEC_PUSH(&visited, v.data[i]); }

            free_vec(&v);
        }
    }

    free_vec(&visited);
    return true;
}

/*!
 * \brief Create a window suitable for printing a witness puzzle
 *
 * Creates a centered window exactly large enough to hold a witness board of the
 * size specified in wc.
 *
 * \param[in] wc The Witness_command whose height and width members will
 * determine the window size
 */
WINDOW* create_witness_win(Witness* wc)
{
    int const ht = 1 + 2 * wc->height;
    int const wd = 1 + 4 * wc->width;
    WINDOW* win  = newwin(ht, wd, (LINES - ht) / 2, (COLS - wd) / 2);
    intrflush(win, false);
    keypad(win, true);

    return win;
}

/*!
 * \brief Paint the witness board, colored tiles, and the players path
 *
 * \param[in]  wc The witness puzzle to print
 * \param[out] win The window to print on
 */
void paint_witness_board(Witness* wc, WINDOW* win)
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

/*!
 * \brief Returns the direction needed to go from one coord to an adjacent one
 *
 * \param[in] from The coord to go from
 * \param[in] to A coord adjacent to `from` to go to
 *
 * \returns The direction from `from` to `to`
 *
 * If `from` and `to` are not adjacent this function will crash
 */
Dir get_direction(coord from, coord to)
{
    coord c = {to.y - from.y, to.x - from.x};

    //We need c to be of the form {+-1, 0} or {0, +-1}
    if (!(c.x == 0 || c.y == 0) || !(abs(c.x) == 1 || abs(c.y) == 1)) {
        log_and_exit(
            "Invalid coords from = {%d, %d}, to = {%d, %d}, passed to %s\n",
            from.y, from.x, to.y, to.x, __func__);
    }
    if (c.y == 0 && c.x == 1) { return dir_right; }
    if (c.y == 0 && c.x == -1) { return dir_left; }
    if (c.y == 1 && c.x == 0) { return dir_down; }
    if (c.y == -1 && c.x == 0) { return dir_up; }

    assert(false);
    log_and_exit("Fatal error encountered in %s.\n Aborting...", __func__);
}

#define VERIFY_PAINT_CONDITIONS(wc, c, dir, point)                             \
    assert(wit_coord_valid_grid(wc, c));                                       \
    assert(wit_coord_valid_grid(wc, step(c, dir)));                            \
    assert(wit_coord_valid_grid(wc, step(step(c, dir), point)));               \
    assert((point) != opposite(dir));

/*!
 * \brief Paint an up movement from the player
 *
 * This function is mainly an implementation detail of \ref paint
 *
 * \param[in]  wc     The witness we are currently painting
 * \param[out] win    Window to paint on
 * \param[in]  point  The direction that the player took after the current up
 * motion
 *
 * `point` has to be left, right or up. If it is `dir_down` then this function
 * will log an error and crash.
 */
void paint_up(Witness* wc, WINDOW* win, coord c, Dir point)
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
            log_and_exit("Invalid point direction in %s, aborting...\n",
                         __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y - 2, scr_pos.x, final_pipe);
    mvwaddstr(win, scr_pos.y - 1, scr_pos.x, "║");
}

//! Commandtion analogous to \ref paint_up
void paint_left(Witness* wc, WINDOW* win, coord c, Dir point)
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
            log_and_exit("Invalid point direction in %s, aborting...\n",
                         __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y, scr_pos.x - 1, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x - 2, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x - 3, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x - 4, final_pipe);
}

//! Commandtion analogous to \ref paint_up
void paint_right(Witness* wc, WINDOW* win, coord c, Dir point)
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
            log_and_exit("Invalid point direction in %s, aborting...\n",
                         __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y, scr_pos.x + 1, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x + 2, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x + 3, "═");
    mvwaddstr(win, scr_pos.y, scr_pos.x + 4, final_pipe);
}

//! Commandtion analogous to \ref paint_up
void paint_down(Witness* wc, WINDOW* win, coord c, Dir point)
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
            log_and_exit("Invalid point direction in %s, aborting...\n",
                         __func__);
    }

    coord scr_pos = get_scr_pos(c);

    mvwaddstr(win, scr_pos.y + 2, scr_pos.x, final_pipe);
    mvwaddstr(win, scr_pos.y + 1, scr_pos.x, "║");
}

/*!
 * \brief Paints the path of the player in the witness puzzle
 *
 * \param[in]  move   The direction the player moved in
 * \param[in]  wc     The witness puzzle in question
 * \param[out] win    The window to print on
 * \param[in]  c      The coord the player moved from
 * \param[in]  point  The direction the player moved afterward
 */
void paint(Dir move, Witness* wc, WINDOW* win, coord c, Dir point)
{
    switch (move) {
        case dir_up   : paint_up(wc, win, c, point); break;
        case dir_left : paint_left(wc, win, c, point); break;
        case dir_right: paint_right(wc, win, c, point); break;
        case dir_down : paint_down(wc, win, c, point); break;
    }
}

/*!
 * \brief Print the last step the player took (i.e. pointing nowhere)
 *
 * \param[in]  wc     The witness puzzle in question
 * \param[out] win    The window to print on
 */
void paint_last(Witness* wc, WINDOW* win)
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

/*!
 * \brief Paint the entire path that the player has taken in a witness puzzle
 *
 * \param[in]  wc     The witness puzzle we are working with
 * \param[out] win    The window to paint on
 * \param[in]  color  The color to print the path in
 *
 * The `color` parameter has to be a valid and previously initialised ncurses
 * color pair.
 */
void paint_path(Witness* wc, WINDOW* win, enum color color)
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

/*!
 * \brief Verifies if the player is moving back from where he came
 *
 * \param[in] wc    The witness puzzle we are concerned about
 * \param[in] move  The direction the player is moving in
 *
 * \returns True, if the player is backtracking (i.e. if last move was up and
 * current move is down)
 */
bool is_backtrack(Witness* wc, Dir move)
{
    if (wc->pos.sz < 2) { return false; }

    return move == get_direction(wc->pos.data[wc->pos.sz - 1],
                                 wc->pos.data[wc->pos.sz - 2]);
}

/*!
 * \brief Sets the appropriate walls for the next move
 *
 * Takes the current position of the player and places down the walls that would
 * be affected by a step in direction `d`
 *
 * \param[in,out] wc The active witness puzzle
 * \param[in]     d  The direction to step in
 * \param[in]     we The status to which the passe by walls will be set
 */
void set_walls(Witness* wc, Dir d, enum Witness_enum we)
{
    coord cs[2];

    get_walls(VEC_BACK(wc->pos), cs, d);

    if (wit_coord_valid_sq(wc, cs[0])) {
        Sq* s = get_p(wc, cs[0]);
        switch (d) {
            case dir_up:
            case dir_down : s->walls[dir_right] = we; break;
            case dir_left :
            case dir_right: s->walls[dir_down] = we; break;
            default:
                log_and_exit("Non-valid Dir value passed to %s\n", __func__);
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
                log_and_exit("Non-valid Dir value passed to %s\n", __func__);
        }
    }
}

//Expects backtrack to be possible (i.e the player is not in the starting
//position), undefined otherwise

/*!
 * \brief Makes the player take one step back
 *
 * \param[in,out] wc The witness puzzle where we make the player take on step
 * back
 *
 * This function expects a backtrack to be possible, i.e. the player to not be
 * in the starting position. If not the behaviour is undefined.
 */
void backtrack(Witness* wc)
{
    Dir d = get_direction(VEC_BACK(wc->pos), wc->pos.data[wc->pos.sz - 2]);
    set_walls(wc, d, we_empty);
    VEC_POP(&wc->pos);
}

/*!
 * \brief
 *
 * \param[in,out] this A pointer to a \ref Witness_command to play
 *
 * \returns The Command \ref func_pop "popped" of the top of the \ref func_stack
 */
Command* play_witness(Witness* this)
{
    Witness* wc = this;
    WINDOW* win = create_witness_win(wc);
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
            //TODO: Add space -> backtrack
            default:;
        }

        if (move) {
            if (is_backtrack(wc, next_dir)) { backtrack(wc); }
            else {
                coord next = step(VEC_BACK(wc->pos), next_dir);
                //Guard against stepping outside the grid and
                //walking over already visited junctions
                if (wit_coord_valid_grid(wc, next) &&
                    !VEC_CONTAINS(wc->pos, next)) {
                    set_walls(wc, next_dir, we_filled);
                    VEC_PUSH(&wc->pos, next);
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

    return pop_command(NULL);
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
    Witness test_wc = {
        .board  = (Sq*)board,
        .height = 6,
        .width  = 6,
        .pos    = new_vec_coord(36),
        .end    = {6, 6}
    };
    VEC_PUSH(&test_wc.pos, ((coord){0, 0}));

    play_witness(&test_wc);

    //NOLINTEND
}
