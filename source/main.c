#include <stdlib.h>

#include "base.h"
#include "start.h"

int main(void)
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
