#include <string.h>
const char* const title[7] = {
    " ___________  _______   _______   ___      ___       _______       __     "
    "  ___      ___   _______  \n",
    "(\"     _   \")/\"     \"| /\"      \\ |\"  \\    /\"  |     /\" _   \"|  "
    "   /\"\"\\     |\"  \\    /\"  | /\"     \"| \n",
    " )__/  \\\\__/(: ______)|:        | \\   \\  //   |    (: ( \\___)    /   "
    " \\     \\   \\  //   |(: ______) \n",
    "    \\\\_ /    \\/    |  |_____/   ) /\\\\  \\/.    |     \\/ \\        "
    "/' /\\  \\    /\\\\  \\/.    | \\/    |   \n",
    "    |.  |    // ___)_  //      / |: \\.        |     //  \\ ___  //  __'  "
    "\\  |: \\.        | // ___)_  \n",
    "    \\:  |   (:      \"||:  __   \\ |.  \\    /:  |    (:   _(  _|/   /  "
    "\\\\  \\ |.  \\    /:  |(:      \"| \n",
    "     \\__|    \\_______)|__|  \\___)|___|\\__/|___|     \\_______)(___/   "
    " \\___)|___|\\__/|___| \\_______) \n",
};

const int title_height = (int)(sizeof(title) / sizeof(char*));
const int title_width  = (int)strlen(title[0]);

const int outer_menu_width = 2 * title_width / 3;
//Note: This currently only works because the '\0' character knocks off one
//character and the '◇' counts for two c characters in a string but 1 character
//on the screen
const int inner_menu_width = outer_menu_width;
