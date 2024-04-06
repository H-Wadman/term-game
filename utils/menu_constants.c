
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
#include <ncurses.h>

// clang-format off
const char* const title[7] = {
    " ___________  _______   _______   ___      ___       _______       __       ___      ___   _______  \n",
    "(\"     _   \")/\"     \"| /\"      \\ |\"  \\    /\"  |     /\" _   \"|     /\"\"\\     |\"  \\    /\"  | /\"     \"| \n",
    " )__/  \\\\__/(: ______)|:        | \\   \\  //   |    (: ( \\___)    /    \\     \\   \\  //   |(: ______) \n",
    "    \\\\_ /    \\/    |  |_____/   ) /\\\\  \\/.    |     \\/ \\        /' /\\  \\    /\\\\  \\/.    | \\/    |   \n",
    "    |.  |    // ___)_  //      / |: \\.        |     //  \\ ___  //  __'  \\  |: \\.        | // ___)_  \n",
    "    \\:  |   (:      \"||:  __   \\ |.  \\    /:  |    (:   _(  _|/   /  \\\\  \\ |.  \\    /:  |(:      \"| \n",
    "     \\__|    \\_______)|__|  \\___)|___|\\__/|___|     \\_______)(___/    \\___)|___|\\__/|___| \\_______) \n",
};

const char* const run[9] = {
" (            )                \n",
" )\\ )      ( /(                \n",
"(()/(   (  )\\())               \n",
" /(_))  )\\((_)\\                \n",
"(_)) _ ((_)_((_) __    _   __  \n",
"| _ | | | | \\| | \\ \\  (_) | _| \n",
"|   | |_| | .` |  > >  _  | |  \n",
"|_|_\\\\___/|_|\\_| /_/  (_) | |  \n",
"                          |__| \n",
};
// clang-format on


const char* const start[]     = {"Play", "Options", "Potatisfläsk", "Exit"};
const char* const encounter[] = {"No! >:(", "AAAAAAAAAAAAAAAAAAAAH",
                                 "󰞇 Time to fight"};

make_menu(start, title, 77); //NOLINT

make_menu(encounter, run, 60); //NOLINT

void initialise_menus()
{
    implementation_initialise_menu(&implementation_start_menu);
    implementation_initialise_menu(&implementation_encounter_menu);
}

// const int outer_menu_width = 2 * title_width / 3 + 10;
//const int outer_menu_width = 77;
