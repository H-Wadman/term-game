#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>

#include "base.h"
#include "io/logging.h"

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

Command const null_command = {.execute = NULL, .persistent = true};

Command const pop = {.execute = pop_command, .persistent = true};

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

void init_color_pairs()
{
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}
