
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

//! Initialises all the runtime information of the menus defined in \ref
//! menu_constants.c
void initialise_menus()
{
    implementation_initialise_menu(&implementation_start_menu);
    implementation_initialise_menu(&implementation_options_menu);
    implementation_initialise_menu(&implementation_glade_menu);
    implementation_initialise_menu(&implementation_well_menu);
}
