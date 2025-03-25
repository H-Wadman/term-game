#include <assert.h>
#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/logging.h"
#include "io/utf8.h"
#include "menu.h"
#include "menu_constants.h"
#include "start.h"
#include "state.h"

#define GET_AND_PRINT_DIA(file, width)                                         \
    {                                                                          \
        char buf[1024];                                                        \
        get_dialogue_path(buf, 1024);                                          \
        strcat(buf, (file));                                                   \
        print_dia(buf, (width));                                               \
    }

//len of str should fit in an int

/*! \brief This function prints the string passed in as a dialogue
 *
 * This commandion will print a dialogue with an identical result to print_dia,
 * however instead of printing the contents of a file, it will print the passed
 * in string.
 *
 * \param str A string containing the text you want to print with length fitting
 * into an int
 *
 * \returns 0 on success and a negative integer on failure depending on the
 * error
 */
int print_diastr(char const* const str)
{
    char const tmpl[] = "/tmp/tmpdia.XXXXXX";
    int fd            = mkstemp((char*)tmpl);
    if (fd == -1) {
        log_and_exit("Failed to make temporary file in %s, aborting...\n",
                     __func__);
    }
    FILE* temp = fdopen(fd, "w");
    if (!temp) {
        log_and_exit("Call to fdopen failed in %s, aborting...\n", __func__);
    }
    int b = fputs(str, temp);

    assert(b == (int)strlen(str));
    if (b != (int)strlen(str)) {
        log_msgln(
            "fputs did not manage to print entire string in print_diastr");
        return -1;
    }

    int err = fputs(u8"\n§\n", temp);
    if (err == EOF) {
        log_msgln("fputs failed in print_diastr");
        return -2;
    }
    err = fflush(temp);
    if (err == EOF) {
        log_msgf("fflush failed in print_diastr, with errno value: %s\n",
                 strerror(errno));
        return -3;
    }

    err = print_dia(tmpl, utf8_strlen(str));
    if (err == -1) {
        log_msgln("print_dia failed in print_diastr");
        return -6; //NOLINT
    }

    err = fclose(temp);
    if (err == EOF) {
        log_msgln("fclose failed in print_diastr");
        return -4;
    }

    err = remove(tmpl);
    if (err == -1) {
        log_msgln("failed to remove file in print_diastr");
        return -5; //NOLINT
    }


    return 0;
}

Command* show_opening(void* _ __attribute__((unused)))
{
    GET_AND_PRINT_DIA("opening.txt", COLS / 3);
    Menu_command* res       = (Menu_command*)malloc(sizeof(Menu_command));
    res->command.execute    = show_menu;
    res->menu               = start_menu;
    res->highlight          = 0;
    res->command.persistent = false;
    return (Command*)res;
}

Command* show_menu(void* this)
{
    Menu_command* mc = (Menu_command*)this;
    Command* option  = print_menu(mc->menu, mc->highlight);

    return option;
}

Command* show_options_execute(void* _ __attribute__((unused)))
{
    Menu_command* start       = (Menu_command*)malloc(sizeof(Menu_command));
    start->command.execute    = show_menu;
    start->menu               = start_menu;
    start->highlight          = 0;
    start->command.persistent = false;
    push_command((Command*)start);
    Command* op = print_menu(options_menu, 0);
    return op;
}

Command const show_options = {.execute    = show_options_execute,
                              .persistent = true};

Command* show_glade_execute(void* _ __attribute__((unused)))
{
    if (!player_visited_glade_val()) {
        GET_AND_PRINT_DIA("intro.txt", COLS / 2);
        player_visited_glade_set();
    }
    Command* op = print_menu(glade_menu, 0);

    return op;
}

Command const show_glade = {.execute = show_glade_execute, .persistent = true};

Command* show_well_execute(void* _ __attribute__((unused)))
{
    Command* res    = (Command*)malloc(sizeof(Command));
    res->execute    = show_glade_execute;
    res->persistent = false;
    push_command(res);
    if (player_visited_well_val()) {
        GET_AND_PRINT_DIA("well.txt", COLS / 2);
        player_visited_well_set();
    }

    Command* op = print_menu(well_menu, 0);

    return op;
}

Command const show_well = {.execute = show_well_execute, .persistent = true};

// clang-format off
    char const* const bucket[] = {
        " ───┴───",
        "/_______\\",
        "\\\\      /",
        " \\\\    /",
        "  -----",
    };
// clang-format on

static void wpaint_bucket(WINDOW* win, int const y)
{
    int const max_x = getmaxx(win);

    int const height = sizeof(bucket) / sizeof(char*);
    int const width  = get_banner_width(bucket, height);

    int const x = (max_x - width) / 2;
    for (int i = 0; i < height; ++i) { mvwaddstr(win, y + i, x, bucket[i]); }

    wrefresh(win);
}

static void wpaint_rope(WINDOW* win, int count, int piece_len, int bucket_width)
{
    for (int i = 0; i < count; ++i) {
        int const curr = piece_len * i;
        mvwaddstr(win, curr, bucket_width / 2, "O");
        for (int j = 1; j <= piece_len - 1; ++j) {
            mvwaddstr(win, curr + j, bucket_width / 2, "|");
        }
    }
}

static int bucket_iteration(WINDOW* win, int count, int piece_len,
                            int bucket_width, int bucket_height)
{
    werase(win);
    wpaint_rope(win, count, piece_len, bucket_width);
    wpaint_bucket(win, count * piece_len);
    wrefresh(win);

    int ch = wgetch(win);
    switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W'     : --count; break;
        case KEY_DOWN:
        case 's'     :
        case 'S'     : {
            int total_height = count * piece_len + bucket_height;
            if (total_height + piece_len <= LINES) { ++count; }
        } break;
        default:;
    }

    return count;
}

static Command* well_raise_bucket_execute(void* _ __attribute__((unused)))
{
    if (player_has_key_val()) {
        print_diastr("You've already got the key!");
        return (Command*)&show_well;
    }
    int const bucket_height = sizeof(bucket) / sizeof(char*);
    int const bucket_width  = get_banner_width(bucket, bucket_height);

    int const mid_x = COLS / 2;
    // Make centered window with bucket_width
    WINDOW* bucket_win =
        newwin(LINES, bucket_width, 0, mid_x - bucket_width / 2);
    intrflush(bucket_win, false);
    keypad(bucket_win, true);


    int const piece_len = 4;
    assert(piece_len > 1);
    int count = (LINES - bucket_height) / piece_len;

    assert(count > 0);
    while (count > 0) {
        count = bucket_iteration(bucket_win, count, piece_len, bucket_width,
                                 bucket_height);
    }

    werase(bucket_win);
    wrefresh(bucket_win);
    delwin(bucket_win);

    print_diastr("There's an old rusty key at the bottom of the bucket.");
    print_diastr("Grab it?");

    int res = quick_print_menu(COLS / 8, 2, "Yes", "No"); //NOLINT(*magic*)
    if (res == 0) { player_has_key_set(); }

    return (Command*)&show_well;
}

Command const well_raise_bucket_command = {.execute = well_raise_bucket_execute,
                                           .persistent = true};
