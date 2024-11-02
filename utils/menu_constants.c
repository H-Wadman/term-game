
/*! \file menu.c
 *
 * \brief Functions and constants pertaining to printing the main menu
 *
 * A file that contains the string constants used for printing to screen,
 * functions that will paint menus and handle input. This documentation will
 * refer to banners and options. A banner is most commonly a larger ascii art
 * indicating the name of the current menu, whereas options are just a series of
 * strings that indicate the possible choices in the menu. As an umbrella term
 * for these "menu component" will be used
 */

#include "menu_constants.h"
#include "menu.h"
#include "start.h"
#include <limits.h>
#include <ncurses.h>

// clang-format off
const char* const title[7] = {
    " ___________  _______   _______   ___      ___       _______       __       ___      ___   _______ \n",
    "(\"     _   \")/\"     \"| /\"      \\ |\"  \\    /\"  |     /\" _   \"|     /\"\"\\     |\"  \\    /\"  | /\"     \"|\n",
    " )__/  \\\\__/(: ______)|:        | \\   \\  //   |    (: ( \\___)    /    \\     \\   \\  //   |(: ______)\n",
    "    \\\\_ /    \\/    |  |_____/   ) /\\\\  \\/.    |     \\/ \\        /' /\\  \\    /\\\\  \\/.    | \\/    |  \n",
    "    |.  |    // ___)_  //      / |: \\.        |     //  \\ ___  //  __'  \\  |: \\.        | // ___)_ \n",
    "    \\:  |   (:      \"||:  __   \\ |.  \\    /:  |    (:   _(  _|/   /  \\\\  \\ |.  \\    /:  |(:      \"|\n",
    "     \\__|    \\_______)|__|  \\___)|___|\\__/|___|     \\_______)(___/    \\___)|___|\\__/|___| \\_______)\n",
};

const char* const run[9] = {
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
// clang-format on

/********************** COMMANDS **********************/
Func command_exit() { return (Func){.func = NULL}; };

//int no_op([[maybe_unused]] void* _) { return 0; }

/**************** START MENU -> OPTIONS ****************/
const struct Command options_language = {.label     = "Language",
                                         .on_select = NULL};
const struct Command options_volume   = {.label = "Volume", .on_select = NULL};
const struct Command options_colour   = {.label = "Colour", .on_select = NULL};
const struct Command options_back = {.label = "Back", .on_select = pop_func};

struct Command const* const options[] = {&options_language, &options_volume,
                                         &options_colour, &options_back};
make_menu(options, NULL, 60, -1, -1); //NOLINT

// int select_options([[maybe_unused]] void* _)
// {
//     int i = print_menu(options_menu);
//     return i;
// }

/********************* START MENU *********************/
int intro([[maybe_unused]] void* _)
{
    show_intro();

    return COMMAND_RETURN;
}

const struct Command start_play = {.label = "Play", .on_select = command_exit};
const struct Command start_options = {.label     = "Options",
                                      .on_select = show_options};
const struct Command start_exit = {.label = "Exit", .on_select = command_exit};

struct Command const* const start[] = {&start_play, &start_options,
                                       &start_exit};

make_menu(start, title, 77, -1, -1); //NOLINT
/*****************************************************/

struct Command const encounter_no      = {.label     = "No! >:(",
                                          .on_select = command_exit};
struct Command const encounter_aah     = {.label     = "AAAAAAAAAAAAAAAAAAAAH",
                                          .on_select = command_exit};
struct Command const encounter_fight   = {.label     = "󰞇 Time to fight",
                                          .on_select = command_exit};
struct Command const encounter_fight_2 = {.label     = "󰞇 Time to fight",
                                          .on_select = command_exit};

// const char* const encounter[] = {"No! >:(", "AAAAAAAAAAAAAAAAAAAAH",
//                                  "󰞇 Time to fight", " Time to debug!"};
struct Command const* const encounter[] = {&encounter_no, &encounter_aah,
                                           &encounter_fight, &encounter_fight};

make_menu(encounter, run, 0, 15, -1); //NOLINT

void initialise_menus()
{
    implementation_initialise_menu(&implementation_start_menu);
    implementation_initialise_menu(&implementation_encounter_menu);
    implementation_initialise_menu(&implementation_options_menu);
}

// const int outer_menu_width = 2 * title_width / 3 + 10;
//const int outer_menu_width = 77;
