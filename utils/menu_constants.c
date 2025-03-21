
/*!
 * \file menu_constants.c
 *
 * \brief Declarations for menu instances used in the game
 *
 * Contains definitions of banners and choices for menus appearing in the game,
 * as well as the definitions of those menus.
 */

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#include "menu.h"
#include "menu_constants.h"
#include "start.h"

// clang-format off
char const* const title[7] = {
    " ___________  _______   _______   ___      ___       _______       __       ___      ___   _______ ",
    "(\"     _   \")/\"     \"| /\"      \\ |\"  \\    /\"  |     /\" _   \"|     /\"\"\\     |\"  \\    /\"  | /\"     \"|",
    " )__/  \\\\__/(: ______)|:        | \\   \\  //   |    (: ( \\___)    /    \\     \\   \\  //   |(: ______)",
    "    \\\\_ /    \\/    |  |_____/   ) /\\\\  \\/.    |     \\/ \\        /' /\\  \\    /\\\\  \\/.    | \\/    |  ",
    "    |.  |    // ___)_  //      / |: \\.        |     //  \\ ___  //  __'  \\  |: \\.        | // ___)_ \n",
    "    \\:  |   (:      \"||:  __   \\ |.  \\    /:  |    (:   _(  _|/   /  \\\\  \\ |.  \\    /:  |(:      \"|",
    "     \\__|    \\_______)|__|  \\___)|___|\\__/|___|     \\_______)(___/    \\___)|___|\\__/|___| \\_______)",
};

char const* const run[9] = {
" (            )              \n",
" )\\ )      ( /(               \n",
"(()/(   (  )\\())              \n",
" /(_))  )\\((_)\\              \n",
"(_)) _ ((_)_((_) __    _   __\n",
"| _ | | | | \\| | \\ \\  (_) | _|\n",
"|   | |_| | .` |  > >  _  | |\n",
"|_|_\\\\___/|_|\\_| /_/  (_) | | \n",
"                          |__|\n",
};

char const* const questionmark[] = {
"       ________",
"   _jgN########Ngg_",
" _N##N@@\"\"  \"\"9NN##Np_",
"d###P            N####p",
"\"^^\"              T####",
"                  d###P",
"               _g###@F",
"            _gN##@P",
"          gN###F\"",
"         d###F",
"        0###F",
"        0###F",
"        0###",
"        \"NN@'",
"",
"         ___",
"        q###r",
"         \"\"",
};

const char* const well_art[] = {
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⣶⣶⠿⠿⠟⠛⢻⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣤⣤⣶⣶⣾⡿⣟⢻⡏⡹⢿⡀⡶⡖⠀⠀⣀⣠⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⣴⣶⡶⠿⠿⠛⡟⢻⠉⣿⣹⣇⣹⣸⠃⡟⢱⣏⣾⣷⠯⠗⠚⠫⣅⠀⠈⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣴⠾⠟⠛⠝⠻⢻⠁⢹⣄⠁⠰⠀⠆⠀⢁⣟⣼⠯⡽⠶⠛⠋⠉⠀⠀⠀⠀⠀⠀⠘⣆⣠⢬⢿⣦⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⢠⣾⠉⢧⡀⠀⠠⠂⠂⠀⠀⠀⠀⢘⣦⡴⠶⠒⡟⢹⠁⠀⠀⠀⠈⠀⢰⡀⢀⣀⣤⡤⠶⠒⠙⠉⢙⠈⠃⣿⣇⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⣠⡿⠃⢀⠈⢳⡀⠀⠀⠠⠀⠂⠈⠁⠀⠀⠀⠀⠀⠀⣧⠀⠆⢀⣀⣤⠴⠚⠋⠉⢀⠀⠁⢄⠀⠀⠀⡞⢸⣠⣬⣿⣧⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⣠⡿⠁⢠⡾⣦⠀⢳⡀⠁⠄⠰⠡⡄⠀⠀⠀⢐⣀⣄⠤⠾⠛⠋⠉⢣⡀⠀⠀⠀⡜⡜⠀⣀⣸⣶⠷⠚⠋⣯⡈⠐⢨⣿⣷⡀⠀⠀⠀⠀",
    "⠀⠀⠀⢠⣿⠁⢀⣾⡇⣯⣷⡀⠻⡄⠀⠀⠀⠱⡤⠴⠚⠛⡏⠀⠀⠀⠀⠀⠀⠀⢳⡀⢀⣴⠿⢳⠛⡉⠀⠀⠀⡄⢰⠘⣇⠆⢸⣀⣻⣷⡀⠀⠀⠀",
    "⠀⠀⢠⣿⠃⠀⣼⣿⢿⣿⠿⣷⡀⢹⡄⠀⠁⠀⠀⠀⠀⠀⠹⣄⠀⠀⣀⣠⠤⠶⠚⠋⠁⠀⠀⡼⡴⠁⢀⠀⢰⣁⣬⡼⢿⢛⠏⡍⠉⢻⣷⡀⠀⠀",
    "⠀⢀⣾⠏⠀⣼⢿⣿⢸⣿⠀⠃⢳⡀⢻⡀⠀⠀⠀⠀⣀⣠⠴⠞⠛⠉⠉⢀⠀⠀⠀⠀⠀⠀⢰⣁⣼⠶⡞⠋⠋⢱⡀⠇⡞⣸⣸⠀⠁⠈⣿⣷⡄⠀",
    "⠀⣼⡟⠀⣴⡏⣿⣿⣌⣿⣀⣀⣈⣳⡀⢿⡀⠀⠊⠉⣄⠀⠀⠀⠂⠐⠀⠘⢦⠀⣀⣤⠴⠚⠉⠉⠀⠐⠁⠰⠀⠈⢻⡐⠧⣿⣯⣴⣶⠿⠛⠋⠁⠀",
    "⣴⣿⠁⣰⣿⣱⣿⣿⠉⢹⠉⣿⡟⠻⣷⠀⢷⡀⠀⠀⠸⣆⢀⢀⣀⡤⠤⠖⠚⠋⠁⠀⠀⠀⢳⡀⠀⠀⠀⣀⣠⣴⣾⣿⣿⣿⣿⣿⣶⣤⠀⠀⠀⠀",
    "⢿⣷⣶⣿⡿⠟⠛⣿⡀⠀⡗⣽⡇⠀⠙⣦⠈⢧⡀⠀⠄⠘⡟⡍⢠⠳⡔⠀⠀⠀⠀⠀⠀⣀⣀⣷⣴⠿⠛⢻⣿⡏⠈⡶⣿⡿⣏⠉⠈⠹⡇⠀⠀⠀",
    "⠀⠀⠈⠀⠀⠈⠀⣿⡅⠀⣃⢾⡇⠀⠀⠹⣇⠈⢷⠀⠀⠀⠁⠃⠆⠀⢳⡀⣀⣤⡤⠶⡿⡿⠉⠁⣀⣤⣤⣾⠿⠷⢾⣄⢫⣿⣿⡿⠦⠄⣧⣤⣴⣶",
    "⠀⠀⠀⠀⠀⠀⠀⣿⠆⠀⣰⢿⡇⠀⠀⠀⢻⣇⠈⢷⠀⠀⠀⣀⣠⣶⣿⣿⣿⣿⣿⣿⣷⣴⠾⠟⠋⠉⣁⣠⣾⣶⡾⢿⣿⣿⠉⣷⡀⢠⣿⣿⣿⣿",
    "⠀⠀⠀⠀⠀⠀⠀⣿⡃⠀⠏⣿⡇⠀⠀⠀⠀⠻⣷⣬⣷⣾⡿⢻⣻⣷⣙⣷⣍⢿⣟⣿⣿⣦⣼⣿⣿⣾⠿⢿⣿⠇⠀⡝⣾⣿⡀⠹⣿⣿⣿⠟⠋⠀",
    "⠀⠀⠀⠀⠀⠀⠀⣿⡇⢀⣣⣿⡇⠀⠀⣀⣠⣤⣶⡿⠿⠟⢻⣉⣷⣯⣿⣿⣿⣿⣿⣿⣿⣿⡛⣯⣅⠀⠠⢸⣿⠀⠀⢞⡏⣿⡇⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⣷⣿⠷⠖⠛⠋⠉⣀⣡⣴⣶⣶⣿⣿⣿⣯⡿⠟⠿⢿⣿⢿⣷⡟⠛⠛⠛⢿⣶⣾⣿⠀⠀⢸⡻⣿⣇⡀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⢀⣴⣿⠃⠘⣿⣿⣠⣤⣤⣾⣟⣛⡉⠉⢿⡄⢀⠉⠉⠁⠀⠀⠀⠀⣹⣿⣿⡿⢄⣠⣠⣾⡁⠀⠹⣆⣀⢹⣿⣿⣛⡿⢷⣦⣄⠀⠀⠀",
    "⠀⠀⠀⢀⣴⡿⠃⣾⠀⢨⣿⣿⡏⠉⡉⢠⠀⠀⠤⣶⣾⣿⣞⣼⢖⢆⣆⡆⢀⣀⣸⣿⣿⡇⣤⡀⣸⡏⠉⠓⠲⠈⢉⣿⣿⠿⢟⠟⠖⠿⣿⣷⡀⠀",
    "⠀⠀⢠⣾⣿⠁⢠⣿⠀⢠⣻⣿⣿⣿⣱⣧⣶⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣾⣿⣷⣿⣿⣿⣿⣿⣿⣿⣳⠖⠢⡰⡏⠁⠀⠀⠀⠀⠁⠉⣿⣿⡇⠀",
    "⠀⢠⣿⡏⣿⡴⠟⡇⠀⠀⢪⣿⣿⢫⡽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣯⣿⣿⣿⣽⡿⣭⠻⡝⢿⣿⣿⡿⠶⣄⠀⠀⠀⠀⡴⠃⢸⡇⠀",
    "⠀⢸⣿⢳⡟⣆⠀⣯⠀⠀⠖⣼⡿⣿⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣾⣿⣿⣷⣿⢶⣼⡷⠟⠁⠀⠀⠈⢳⡄⢠⡞⢡⠀⣿⡇⠀",
    "⠀⢸⣿⣿⡁⢈⠀⠈⠙⠻⠿⠿⣧⡛⣙⢿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⠿⠟⠛⠛⠛⠿⣍⠀⠀⠀⠀⠀⣀⣤⣿⢉⠇⡾⣟⣿⡇⠀",
    "⠀⢸⣿⡏⠄⠀⠑⠀⡀⠀⢠⣀⠀⠀⢀⡾⠻⠃⠀⠀⠀⠈⠉⠉⠉⠉⠻⣏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣦⠀⠀⢀⣴⢧⢀⣼⣾⢰⢱⢻⣿⠁⠀",
    "⠀⢸⣿⡆⠐⠀⠀⠀⠀⠀⠀⠈⠙⢻⣟⡦⠄⠀⠀⠀⠀⠀⢀⣀⣀⣀⣀⣽⣦⣤⣤⡤⠤⠤⠖⠒⠋⠀⠘⢽⣇⠀⣼⡟⡌⣸⣼⢣⣟⣿⣾⣿⠀⠀",
    "⠀⠀⣿⠿⣦⡀⠀⠀⢄⠂⡀⠀⠀⣾⠀⠉⠰⢭⢔⣆⢀⠠⠀⡀⢀⠀⠠⢼⣧⡀⣄⣶⣼⠷⣄⢠⣷⢦⡟⣼⡿⣸⣻⢱⢣⣡⡟⣨⣹⡾⣿⣿⠀⠀",
    "⠀⠀⣿⣾⣬⠻⣦⣄⠀⠁⠀⠀⣸⠏⠀⠀⠀⠀⠀⠈⠑⠂⠱⣤⡎⢸⣏⣿⣿⡽⣿⣿⡟⣾⣾⣾⣟⣾⣰⣿⣷⣿⣇⣾⣾⣿⣿⢿⢋⣧⣿⣿⠀⠀",
    "⠀⢠⣿⣿⡯⠙⢹⠛⠛⠶⠦⣾⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣽⣿⣿⣿⣿⣹⣿⣿⣣⣿⣿⣾⣾⣿⣿⣿⣏⣅⣾⡞⣽⣸⣿⠀⠀",
    "⠀⢸⣿⠃⠀⠀⣹⠀⠀⠀⠀⠀⠈⠉⠙⠛⠷⣶⣦⣤⣄⣀⣀⣀⣀⣀⣠⣿⣥⣿⣿⣧⣯⣿⣿⣿⣿⡿⡿⣿⣿⣿⢿⠇⣿⡾⡽⣹⢱⢃⣿⣿⠀⠀",
    "⠀⢸⣿⡀⠀⠀⡿⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠈⢿⡉⠀⠉⠉⣉⠉⠛⠛⠛⠛⠙⡏⣿⣿⣿⣿⣿⣻⡟⢹⣿⢧⣏⡟⢸⢻⣷⢷⢇⣿⣿⢻⣿⠀⠀",
    "⠀⢸⣿⡷⣦⣰⣇⠀⠀⠀⠀⠀⢄⡔⠄⠛⠀⠀⣸⡅⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⣼⣿⣻⣻⢷⣿⣿⢃⣿⡟⡿⣸⢱⣿⣿⣷⣿⡿⣿⣿⣿⣿⠀⠀",
    "⠀⢸⣿⣽⡎⠛⠿⢷⣤⣤⣀⡀⠀⠀⠀⠀⠀⣸⡿⠀⠀⠀⡀⡀⢴⡖⢱⢄⠀⠀⢻⣷⣿⣟⡟⣼⣧⣼⣿⣿⣷⣧⣿⣾⣿⡿⢸⢳⣿⣧⢹⣿⠀⠀",
    "⠀⠸⣿⡏⠀⠀⠀⠀⠀⠈⠉⢻⡿⠛⣷⠶⠿⠿⠿⠶⣤⣿⣿⣯⣿⣧⣯⣿⣧⣤⣿⣿⣿⣾⣿⡿⣿⢿⢿⠏⣿⣿⢱⣯⣿⣡⣯⣿⣿⣿⣹⣿⠀⠀",
    "⠀⠀⠙⢿⣦⣄⡀⠀⠀⠀⠀⢸⡗⠀⠠⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⢿⠋⠉⠉⣹⣿⢳⣿⢧⣿⣿⡟⢰⢿⡏⣾⣿⢃⡟⣼⣽⣿⣿⡿⠋⠀⠀",
    "⠀⠀⠀⠀⠈⠛⠻⢷⣦⣤⣤⣼⣇⠀⠀⠀⠀⠀⠀⢀⠈⡀⡀⠄⠀⠀⠀⣸⠀⠀⠀⢻⣿⣿⡏⣾⡿⣹⡁⢿⣿⣀⣿⣿⣾⡿⠿⠛⠉⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠻⠿⢿⣷⣾⣶⣿⣶⣿⣍⣁⣁⣀⣀⣀⣰⣯⣀⣼⣿⣀⣘⣹⣧⣴⣷⣿⡿⠿⠟⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠋⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢀⠀⡀⢀⠀⡀⢀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ "
};


// clang-format on

/********************** COMMANDS **********************/

//Auxiliary definitions

/*!
 * \Brief \ref Command extension designed for immediately returning a value
 *
 * This structure is mainly useful in combination with \Option "Options".
 * \ref MAKE_RETURN_COMMAND is defined in order to simplify creating
 * Options with corresponding Return_commands.
 */
typedef struct Return_command
{
    Command this;
    Command* return_value;
} Return_command;

Command* return_command(void* this)
{
    return ((Return_command*)this)->return_value;
}

/*!
 * \brief Make an Options that returns a Command*
 *
 * The Command* will be returned by \ref Option::command::execute
 * \param opt_name Name of the \ref Option to be created.
 * \param label_val The label of the \Option to be created.
 * \param ret_val_name Decides the name of the Return_command to be created. Has
 *    to be valid characters in an identifier. Any such name that does not clash
 *    with another call to MAKE_RETURN_COMMAND is fine, but is usually be the
 *    same as ret_val without any operators.
 * \param ret_val (Command*) The return value of Option::command::execute
 */
#define MAKE_RETURN_COMMAND(opt_name, label_val, ret_val_name, ret_val)        \
    struct Return_command const impl_return_##ret_val_name = {                 \
        (Command){.execute = return_command, .persistent = true},              \
        .return_value = (ret_val) \
    };                                            \
                                                                               \
    const struct Option opt_name = {                                           \
        .label   = (label_val),                                                \
        .command = (Command*)&impl_return_##ret_val_name};

/********************* STANDARD COMMANDS *********************/
/********************** SHOULD BE CONST **********************/

// = no-op for menus, exit for main function
static Command const null_command = {.execute = NULL};

static Command const pop = {.execute = pop_command};

static Return_command const exit_game = {(Command){.execute = return_command},
                                         .return_value =
                                             (Command*)&null_command};

/**************** START MENU -> OPTIONS ****************/
const struct Option options_language = {.label   = "Language",
                                        .command = (Command*)&null_command};
const struct Option options_volume   = {.label   = "Volume",
                                        .command = (Command*)&null_command};
const struct Option options_colour   = {.label   = "Colour",
                                        .command = (Command*)&null_command};
const struct Option options_back = {.label = "Back", .command = (Command*)&pop};

struct Option const* const options[] = {&options_language, &options_volume,
                                        &options_colour, &options_back};
make_menu(options, NULL, 60, -1, -1); //NOLINT

/********************* START MENU *********************/

MAKE_RETURN_COMMAND(start_play, "Play", show_glade, (Command*)&show_glade);
//make_menu_command(options);


const struct Option start_options = {.label   = "Options", //NOLINT
                                     .command = (Command*)&show_options};
const struct Option start_exit    = {.label   = "Exit",
                                     .command = (Command*)&exit_game};

struct Option const* const start[] = {&start_play, &start_options, &start_exit};

make_menu(start, title, 77, -1, -1); //NOLINT

/********************** GLADE **********************/

const struct Option glade_cabin = {.label   = "Cabin",
                                   .command = (Command*)&exit_game};

MAKE_RETURN_COMMAND(glade_well, "Well", show_well, (Command*)&show_well);

const struct Option glade_forest   = {.label   = "Forest",
                                      .command = (Command*)&exit_game};
struct Option const* const glade[] = {&glade_cabin, &glade_well, &glade_forest};

make_menu(glade, questionmark, 100, -1, -1); //NOLINT

/************************ WELL ************************/

MAKE_RETURN_COMMAND(well_raise_bucket, "Raise bucket",
                    well_raise_bucket_command,
                    (Command*)&well_raise_bucket_command);
const struct Option well_back = {.label = "Back", .command = (Command*)&pop};

struct Option const* const well[] = {&well_raise_bucket, &well_back};

make_menu(well, well_art, 50, -1, -1); //NOLINT

/*****************************************************/

//! Initialises all the runtime information of the menus defined in \ref
//! menu_constants.c
void initialise_menus()
{
    implementation_initialise_menu(&implementation_start_menu);
    implementation_initialise_menu(&implementation_options_menu);
    implementation_initialise_menu(&implementation_glade_menu);
    implementation_initialise_menu(&implementation_well_menu);
}
