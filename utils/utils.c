#include <assert.h>
#include <stdlib.h>
#include <utils.h>

extern bool is_ascii(unsigned int c);        //NOLINT
extern bool is_continuation(unsigned int c); //NOLINT

//! Reads the length in bytes of a UTF-8 unicode code point from the first byte

/*!
 * @param c The first byte of a UTF-8 unicode code point
 * @returns An integer in the range [1, 4] indicating the length in bytes of the
 * unicode code point, or -1 on error
 */
int get_utf8_len(unsigned int c)
{
    if (is_ascii(c)) { return 1; }
    unsigned const int mask = 0b11000000;
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

    return -1;

#undef check
}

int load_utf8(char* buf, WINDOW* win)
{
    int c = wgetch(win);

    if (c < 0) {
        fprintf(stderr, //NOLINT
                "A character with negative value has been read in wgetch\n");
        exit(1); //NOLINT
    }

    const int len = get_utf8_len(c);
    if (len == -1) {
        free(buf);
        fprintf(stderr, //NOLINT
                "wget_utf8, first byte (= %d) did not conform to UTF-8", c);
        return -1;
    }
    assert(1 <= len && len <= 4);
    buf[0] = (char)c;
    for (int i = 1; i < len; ++i) {
        c = wgetch(win);
        if (!is_continuation(c)) {
            fprintf(stderr, //NOLINT
                    "wget_utf8: Length indicated by first byte doesn't "
                    "correspond to length of string");
            free(buf);
            return -1;
        }

        buf[i] = (char)c;
    }
    buf[len] = '\0';

    return 0;
}

const char* wget_utf8(WINDOW* win)
{
    // A unicode character can be 1-4 bytes + null termination
    char* res = (char*)malloc(5);

    int err = load_utf8(res, win);

    if (err != 0) {
        free(res);
        return NULL;
    }

    return res;
}
