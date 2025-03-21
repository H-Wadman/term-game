#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>

#include "base.h"
#include "io/logging.h"
#include "menu/menu_constants.h"
#include "menu/start.h"

//! Command linked list structure
typedef struct Node
{
    struct Node* next;
    Command* val;
} Node;

/*!
 * Global command stack for storing commands that are to be restored later.
 *
 * If for example you have a command that prints some options, and that can be
 * called from state A, B and C, you could store a command that will restore the
 * correct state out of A, B and C and then have the options menu return the
 * command on the stack whenever it is done (i.e the \ref Option of the exit
 * option has \ref pop_command as it's command member).
 *
 * This variable should be manipulated using \ref push_command and \ref pop_func
 */
static Node* command_stack = NULL; //NOLINT

Command const null_command = {.execute = NULL};

Command const pop = {.execute = pop_command};

void push_command(Command* f)
{
    Node* curr = (Node*)malloc(sizeof(Node));

    curr->val  = f;
    curr->next = command_stack;

    command_stack = curr;
}

Command* pop_command(void* _ __attribute__((unused)))
{
    if (!command_stack) { log_and_exit("Empty command stack popped\n"); }

    Node* popped = command_stack;

    command_stack = command_stack->next;

    Command* res = popped->val;
    free(popped);

    return res;
}

static void perform_atexit()
{
    endwin();
    close_log_stream();
}

static void init_color_pairs()
{
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

//! \brief Sets the locale, initialises ncurses with common defaults, and
//! registers a function to be called on exit
static void ncurses_set_up()
{
    const char* locale = setlocale(LC_ALL, "");
    if (locale == NULL) {
        fprintf(stderr, "Error setting locale, aborting.../\n"); //NOLINT
        exit(1);
    }
    initscr();
    start_color();
    init_color_pairs();
    clear();
    noecho();
    cbreak();
    nonl();
    intrflush(stdscr, false);
    //Add this?
    // raw();
    keypad(stdscr, TRUE);

    int err = atexit(perform_atexit);
    if (err != 0) {
        fprintf( //NOLINT
            stderr,
            "Couldn't register atexit function in ncurses_set_up, quitting "
            "program...\n");
        endwin();
        exit(1);
    }
    err = curs_set(0);
    if (err == ERR) {
        fprintf(stderr, //NOLINT
                "Terminal doesn't support invisible cursor, support to be "
                "added.\n Aborting...\n");
        exit(1);
    }
}

void init_game()
{
    ncurses_set_up();
    initialise_menus();
    set_log_output(stderr);
}

Command* start_game()
{
    Command* start    = (Command*)malloc(sizeof(Command));
    start->execute    = show_opening;
    start->persistent = false;

    return start;
}
