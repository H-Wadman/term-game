#ifndef UTF8_H
#define UTF8_H

#include <ncurses.h>
#include <stdbool.h>

#define ASCII_MAX 127
#define ASCII_MIN 0
#define ASCII_BUF_SZ 5

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

//! Reads a UTF-8 unicode point from win and returns it as a malloced string
const char* wget_utf8(WINDOW* win);

//! Reads a UTF8 unicode point from win into buf
int load_utf8(char* buf, WINDOW* win);

//! Reads a UTF8 unicode point from file into buf
int fload_utf8(char* buf, FILE* file);

//! Reads a UTF8 word (until whitespace) from file into buf
int floadw_utf8(char* buf, FILE* file);

//! \brief Sets the locale, initialises ncurses with common defaults, and
//! registers endwin to be called on exit
void ncurses_set_up();

//! Returns the length of a UTF-8 string in unicode code points
int utf8_strlen(const char* str);


#endif
