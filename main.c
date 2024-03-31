#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>

#define ASCII_MAX 127
#define ASCII_MIN 0

int const height;
int const width;

void ncurses_set_up()
{
    setlocale(LC_ALL, "");
    initscr();
    clear();
    noecho();
    cbreak();
    nonl();
    intrflush(stdscr, false);
    //Add this?
    //raw()
    keypad(stdscr, TRUE);
}

inline bool is_ascii(unsigned int c)
{
    return ASCII_MIN <= c && c <= ASCII_MAX;
}

//! Reads the length in bytes of a UTF-8 unicode code point from the first byte

/*!
 * @param c The first byte of a UTF-8 unicode code point
 * @returns An integer in the range [1, 4] indicating the length in bytes of the
 * unicode code point, or -1 on error
 */
int get_utf8_len(unsigned int c)
{
    if (is_ascii(c)) { return 1; }
    unsigned const int mask = 0b1100000;
    if ((c & mask) != mask) { return -1; }

#define check(mask, return_val)                                                \
    do {                                                                       \
        if ((c & (mask)) != 0) {                                               \
            unsigned int const check = ((mask) >> 1U);                         \
            return ((check & c) != 0) ? -1 : (return_val);                     \
        }                                                                      \
    } while (0);

    check(0b00010000U, 4);
    check(0b00100000U, 3);
    check(0b01000000U, 2);

#undef check
}

inline bool is_continuation(unsigned int c)
{
#define BIT1 0b10000000U
#define BIT2 0b01000000U

    return (BIT1 & c) != 0 && (BIT2 & c) == 0;
}

const char* wget_utf8(WINDOW* win)
{
    // A unicode character can be 1-4 bytes + null termination
    char* res = (char*)malloc(5);

    int c = wgetch(win);

    if (c < 0) {
        int err = fprintf(
            stderr,
            "A character with negative value has been read in wgetch\n");
        exit(err);

        return
    }

    if (is_ascii(c)) {
        res[0] = (char)c;
        res[1] = '\0';
    }

    int i = 0;
}

WINDOW* paint_start_menu() { WINDOW* menu_win = newwin(); }

int main() { ncurses_set_up(); }
