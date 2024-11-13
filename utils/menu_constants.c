
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
Func command_exit(void* _ __attribute__((unused)))
{
    return (Func){.func = NULL};
};

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

struct Return_command
{
    struct Command base;
    Func return_value;
};

#define MAKE_RETURN_COMMAND(cmd_name, label_val, ret_val)                      \
    Func impl_return_##ret_val(void* _ __attribute__((unused)))                \
    {                                                                          \
        return (Func){ret_val};                                                \
    }                                                                          \
    const struct Command cmd_name = {.label     = (label_val),                 \
                                     .on_select = impl_return_##ret_val}

/********************* START MENU *********************/
int intro([[maybe_unused]] void* _)
{
    show_opening(NULL);

    return COMMAND_RETURN;
}

MAKE_RETURN_COMMAND(start_play, "Play", show_glade);
const struct Command start_options = {.label     = "Options",
                                      .on_select = show_options};
const struct Command start_exit = {.label = "Exit", .on_select = command_exit};

struct Command const* const start[] = {&start_play, &start_options,
                                       &start_exit};

make_menu(start, title, 77, -1, -1); //NOLINT

/********************** GLADE **********************/

const struct Command glade_cabin = {.label     = "Cabin",
                                    .on_select = command_exit};

MAKE_RETURN_COMMAND(glade_well, "Well", show_well);

const struct Command glade_forest   = {.label     = "Forest",
                                       .on_select = command_exit};
struct Command const* const glade[] = {&glade_cabin, &glade_well,
                                       &glade_forest};

make_menu(glade, questionmark, 100, -1, -1); //NOLINT

/************************ WELL ************************/

MAKE_RETURN_COMMAND(well_raise_bucket, "Raise bucket", well_raise_bucket_func);
const struct Command well_back = {.label = "Back", .on_select = pop_func};

struct Command const* const well[] = {&well_raise_bucket, &well_back};

make_menu(well, well_art, 50, -1, -1); //NOLINT

/*****************************************************/
void initialise_menus()
{
    implementation_initialise_menu(&implementation_start_menu);
    implementation_initialise_menu(&implementation_options_menu);
    implementation_initialise_menu(&implementation_glade_menu);
    implementation_initialise_menu(&implementation_well_menu);
}

// const int outer_menu_width = 2 * title_width / 3 + 10;
//const int outer_menu_width = 77;
