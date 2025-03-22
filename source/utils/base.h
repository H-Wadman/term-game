#pragma once

#include <stdbool.h>

enum color
{
    col_default,
    col_yellow,
    col_green,
    col_red
};

/*!
 * \brief Recursive command at the center of the games functionality
 *
 * The Command struct is a struct defined to both serve as the "base class" for
 * the command design pattern as well as to circumvent the issues of declaring
 * a function with a recursive return type.
 *
 * The game starts by calling the first \ref Command::execute printing the
 * opening sequence. When that sequence has finished it will return a new
 * Command that will call \ref Command::execute "execute" to advance the game
 * state and return the Command that determines what happens next, and so on.
 */
typedef struct Command
{
    //! The "member function" associated with the Command
    struct Command* (*execute)(void*);
    bool persistent;
} Command;

/* <--- Command members ---> */
/*!
 * \var Command::execute
 * This function will be used as a means to execute an arbitrary function that
 * returns a Command*.
 *
 * *Note: A sizeable part of this is simply an explanation in context of the
 * command dessign pattern*
 *
 * The general idea is as follows: we want to run the game by executing a loop
 * that calls functions that advance the game and then return the next function
 * to be called. However, most functions will require arguments in order to be
 * flexible or do what we want, and in the main loop we have to have the same
 * signature for every function.
 */

//! Pushes a command on to the global \ref func_stack
void push_command(Command* f);
//! Pops a command of the global \ref func_stack
Command* pop_command(void*);

//Standard commands

extern Command const null_command;
extern Command const pop;

void init_game();
Command* start_game();
