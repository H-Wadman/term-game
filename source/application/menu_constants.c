
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

#include "base.h"
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

char const* const door_art[] = {
    "              ________",
    "             / ______ \\",
    "             || _  _ ||",
    "             ||| || |||",
    "             |||_||_|||",
    "             || _  _o||    ",
    "             ||| || |||",
    "             |||_||_|||      ^~^  ,",
    "             ||______||     ('Y') )",
    "            /__________\\    /   \\/",
    "    ________|__________|__ (\\|||/) _________",
    "           /____________\\",
    "           |____________|",
};

char const* const freaky_apple_art[55] = {

"                                       *%##*-==--------=*@@                                         ",
"                              ******++*#%###+==---------===--+%                                     ",
"                      *=---=--========--*#***==------------=====-+##+=-======*#@                    ",
"                  %#*+==================-=***+=-------------==+==+++===========+=+#                 ",
"               %#***+=-====-----==++=======*+*========--====++++++====---====+++++++*%              ",
"             %#********+++==================*+*=====-====+=++++======------==++*+***+**@            ",
"           %#*#********++============+==++===+*+----:----============----==+++*+********#@          ",
"          #####*#**+*+++++===========-=======-======---=====---------====+++++***********#%         ",
"        %*####+*++===-=====++===-=============-=--=-----=----------====+++++****#*#########%        ",
"       #+###***+*+===-=--=============--==--=-----------------=--==-=+=++********########**#%       ",
"      %=*%#**+***+++==--===------======-----------------------=======+*+++**#+*##**##%%#*####       ",
"     @=+##****##***++=====---==-====-========----------============+=+++**+*###*##*#*#%%%####%      ",
"     +=##***########+*+==++==++++++++==+=====---=-----==========++=++++*****###%######*##%%%##@     ",
"    *-*#****#%#####+##**++++**+****+*+*++++============+++=++++=+++****+#***##%#########*##%%#%@    ",
"    -=#***###%##%####*********+*********+++++====+++++++++++=++*+***+***#*****##%###########%%%%    ",
"   %=****#%#%#%######*####***+=+*******+**+++++==++++++++***+=+**#******+*#**+*#%#%#**#####%#%%#%   ",
"   ++#####%%###%####***#**#**+++********++++++++++**+**+**+*+++*********##******%###*****##%#%%%#   ",
"   -*#######%#*#####*#**##**+++*#*+*#*+==+*++===-=+*+***+*++**+*#++##******##*##*##**++*##%%%%%%#%  ",
"   -*#####+*####%######**********++*+++==++=--:--==+**+*****#**##*+##**##**##*###**++=+*#%#%%%%%#%  ",
"  @=*#%###*+*##%######***###*****+++++++++=-:::--==++++*******###***######*#####*+=-:=**%%%%%%%###  ",
"  %**#%%###****#%#####***###*#***+++*++++==-:::=-=++++*******####*##**+#######*##==++*##%%%%%%%###  ",
"  ##**#%#%%####%%#%##**#*##*##***++++***+===--===+++*********###***#*###*####%#*#*+++#%##%%%%%####  ",
"  #####%%%#######%%##*#*####*#***+*******+++====+***+=******#***##*##*##########%%##%%%%%%%%%##%##  ",
"  ##*######%%####%%%########****#+***+****++++++************#*#*###########%####%%%%%%%%%%%%%%@%#%  ",
"  %*##########%###%%####**#***********#*******+****###**#*#*####*#%#%%%%#%%%%%%%%@@%%%%%%%%%%@@@*%  ",
"   +###########%%%#####***##+**#*#####***###*******#####*###%##%##%#%%%%*#%%%%%%%%@%@@@%@%%@@@%@*%  ",
"   +*##########%%%%%###**###*###*##*####*####*##########%#####%#%###%##%%%%%%%%%%%%%%%@%#%@@@@@@+@  ",
"   #+****#**#*#####%#############%#####***##%###*#####*####%##%%%%%%%%%%%%%%%@%%%%%%%%%#%%@@@%%%*   ",
"    *+**#******##%#=-%@%%%@@@%%#*%########*#%##*+*##*###%##%%%%%%%%%%%@@%%@%@@@#*##%%%%%%%%%%%%##   ",
"    *+*##******##*=:-=@@@@@@@@%==##*######*###########%%%%%%%%%%%%%+==%@@@@@@@#*##%%#%@%%%%%%%%#@   ",
"     =*#***+++**#%#%*=-=%@@@@#=--==+*%%##*##*#########%%%%%##%%%%##*****#%%%%%%#%%##%@%%%%##%##*@   ",
"     #***+*++++**##%%%%%########**#**#################%%%%%%%%%%%%%%%%#%%%%%%%%%#%%%%%%%######*#    ",
"      *+**++*+++*###%#%####*#%%####*#####*####*#######%%%%%%%%%%%%%%%%%@@%%#%%%%%%%%%%%######**@    ",
"       =+++++%+++***###%#######%####################*#%%%%%%%%%%%%%%%@@@@%%%@%%%%%%%%%##**##*+@     ",
"       *=+++++@*++**###%#####%##%####################*%%%%%%%%%%%%%%%%%%%@@%%%%%@@%%%%#**#*#*%      ",
"        #++++++%@+++**##########*####%#################%%%%%%##%###%%%%%@%%%%%%@@%%%@#**#***#       ",
"         *++++++#@%+***##**####*#######################%%%%%%%####%%%%%%%%%%%@@@@%@@%#***+*#        ",
"          *+++=+++@@@****#####################**%######%%%%%##%%%%%%#%%%%%%%%%%#%%@%%#*#*+*         ",
"           +++===++@@@-***####***#*############%%######%%#%%%##%%%%%%%%%%%#**+-@#%%%####**          ",
"            =+===++*%@@%:::-=+++*#####*############*###%%%#%%%####***+++=:-::=#%%%%%###*+@          ",
"            %===+++***@@@@-::...::.....:..........:.........:......:-...::-+==@@%%%%##*+@           ",
"             %+=++++***=%@@@-...::.....:..........-.........:......:-...*@@@@=@%%%%##**%            ",
"              #==++*****+:-@@@@+::.....:....    ..:.    ....:......:=*@@@@@@%-%%%%%##+%             ",
"               *==++***##:.:@@@@@@+-...:....    ..:.     ...:....=#@@@@@@*=%%:##%%##+#              ",
"                *==++***=-:.:%@@@@++++++++-.......-.......:=+++++++@@@@@:-%%%%%%%%#**               ",
"                 +=++*##=::::::@@%++++++++++++++++++++++++=++++++++@@+.:-*%%###%%#*#                ",
"                  *=+*##*:.::...@%++++++++++++++++++++++++-=+++++++@.::*++%%%##%%##                 ",
"                   *=+**#:.:+:::.-+++=========++++++++====--====+++-::-%*+%%%%%%#@                  ",
"                     =*###..#*#-::=+=====-=======++================--:*%-%%#%%%%                    ",
"                      *=+*:.*#***--=====--------=====-------======-**=%-+###%#%                     ",
"                        #=:.*##**=-=====--------=====---:-::--=====####-#####@                      ",
"                          :.*####*--=+===--:::--====---::::---====####=+###@                        ",
"                 #       *::* %**#+======---:---====---::---===++#####**#@                          ",
"                         *::-    @@%*=====------=====------====#######**                            ",
"                         =:.:       #=*:===-.:-======----===+:%%%%%%@@=+                            ",
};


char const* const gudrun_art[16] = {
"         .---.",
"        (_---_)",
"       (_/6 6\\_)",
"        (  v  )",
"         `\\ /'",
"      .-'': ;``-.",
"     /   \\,Y./   \\",
"    /     (:)___  \\",
"   :   .-'XXX`-.`\\_;",
"    `.__.-XXX-.__.'\\_",
"     /  / XXX \\  \\   `\\_",
"    /      XXX    \\     `\\",
"   /        XXX    \\     _`\\___",
"  /                 \\  (`--\"\"\"-')",
" /                   \\ (=-=-=-=-)",
" `--...___   ___...--' (________)",
};

// clang-format on

/********************** COMMANDS **********************/

static Return_command const exit_game = {
    (Command){.execute = return_command, .persistent = true},
    .return_value = (Command*)&null_command
};

/**************** START MENU -> OPTIONS ****************/


const struct Option options_language = {.label   = "Language",
                                        .command = (Command*)&null_command};
const struct Option options_katte    = {.label   = "Katte mode",
                                        .command = (Command*)&switch_katte_mode};
const struct Option options_colour   = {.label   = "Colour",
                                        .command = (Command*)&null_command};
const struct Option options_back = {.label = "Back", .command = (Command*)&pop};

struct Option const* const options[] = {&options_language, &options_katte,
                                        &options_colour, &options_back};
MAKE_MENU(options, NULL, 60, -1, -1); //NOLINT

/********************* START MENU *********************/

Option const start_play = {.label = "Play", .command = (Command*)&show_glade};

Option const start_options = {.label   = "Options", //NOLINT
                              .command = (Command*)&show_options};
Option const start_exit    = {.label = "Exit", .command = (Command*)&exit_game};

Option const* const start[] = {&start_play, &start_options, &start_exit};

MAKE_MENU(start, title, 77, -1, -1); //NOLINT

/************************ CABIN ************************/

Option const cabin_knock = {.label = "Knock", .command = (Command*)&knock};
Option const cabin_back  = {.label   = "Go back",
                            .command = (Command*)&show_glade};

Option const* const cabin[] = {&cabin_knock, &cabin_back};

MAKE_MENU(cabin, door_art, 100, -1, -1); //NOLINT
MAKE_MENU_COMMAND(cabin);

/********************** APPLE **********************/


/********************** GLADE **********************/

Option const glade_cabin  = {.label   = "Cabin",
                             .command = (Command*)&go_to_cabin};
Option const glade_well   = {.label = "Well", .command = (Command*)&show_well};
Option const glade_forest = {.label   = "Forest",
                             .command = (Command*)&null_command};
Option const* const glade[] = {&glade_cabin, &glade_well, &glade_forest};

MAKE_MENU(glade, questionmark, 100, -1, -1); //NOLINT

/************************ WELL ************************/

// MAKE_RETURN_OPTION(well_raise_bucket, "Raise bucket",
// well_raise_bucket_command,
//                    (Command*)&well_raise_bucket_command);

Option const well_raise_bucket = {
    .label = "Raise bucket", .command = (Command*)&well_raise_bucket_command};

const struct Option well_back = {.label = "Back", .command = (Command*)&pop};

struct Option const* const well[] = {&well_raise_bucket, &well_back};

MAKE_MENU(well, well_art, 50, -1, -1); //NOLINT

/************************ GUDRUN ************************/

Option const gudrun_speak = {.label   = "Speak",
                             .command = (Command*)&null_command};
Option const gudrun_back  = {.label   = "Return",
                             .command = (Command*)&show_glade};

Option const* const gudrun[] = {&gudrun_speak, &gudrun_back};

MAKE_MENU(gudrun, gudrun_art, 0, -1, -1); //NOLINT
MAKE_MENU_COMMAND(gudrun);

/*****************************************************/

//! Initialises all the runtime information of the menus defined in \ref
//! menu_constants.c
void initialise_menus(void)
{
    implementation_initialise_menu(&implementation_start_menu);
    implementation_initialise_menu(&implementation_options_menu);
    implementation_initialise_menu(&implementation_glade_menu);
    implementation_initialise_menu(&implementation_well_menu);
    implementation_initialise_menu(&implementation_cabin_menu);
    implementation_initialise_menu(&implementation_gudrun_menu);
}
