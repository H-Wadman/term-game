#include <ncurses.h>
#include <stdlib.h>

#include "base.h"

int main()
{
    init_game();

    Command* curr = start_game();
    while (curr->execute) {
        Command* old = curr;
        curr         = curr->execute(curr);
        if (!old->persistent) { free(old); }
    }

    return 0;
}
