#include <ncurses.h>
#include <stdbool.h>

#define ASCII_MAX 127
#define ASCII_MIN 0

inline bool is_ascii(unsigned int c)
{
    return ASCII_MIN <= c && c <= ASCII_MAX;
}

inline bool is_continuation(unsigned int c)
{
#define BIT1 0b10000000U
#define BIT2 0b01000000U

    return (BIT1 & c) != 0 && (BIT2 & c) == 0;
#undef BIT1
#undef BIT2
}

//! Reads a UTF8 unicode point from win and returns a malloced string
const char* wget_utf8(WINDOW* win);

//! Reads a UTF8 unicode point from win into buf with at least size 5
int load_utf8(char* buf, WINDOW* win);
