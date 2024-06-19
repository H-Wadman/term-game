/*! \file utf8.c
 * \brief UTF-8 string handling and io
 *
 *  A file containing functions related to UTF-8 string handling and io.
 */

#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "gettext_defs.h"
#include "utf8.h"

//! Checks if the passed in byte c is an ASCII character
extern bool is_ascii(unsigned int c); //NOLINT

//! Checks if the passed in byte c is a continuation byte in the UTF-8 format
extern bool is_continuation(unsigned int c); //NOLINT

/*! \brief Reads the length in bytes of a UTF-8 unicode code point from the
 * first byte
 *
 * \param c The first byte of a UTF-8 unicode code point
 * \returns An integer in the range [1, 4] indicating the length in bytes of the
 * unicode code point, or -1 on error
 */
int get_utf8_len(unsigned int c)
{
    if (is_ascii(c)) { return 1; }
    unsigned const int mask = 0b11000000;
    if ((c & mask) != mask) { return -1; }

    // \cond
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
    // \endcond
}

/*!
 * Reads a UTF-8 character from an ncurses window into a char buffer
 * \param buf A character buffer in which the unicode code point will be written
 * \param win A ncurses window from which the unicode code point will be read
 * \returns 0 on success -1 on failure
 */
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
                    "load_utf8: Length indicated by first byte doesn't "
                    "correspond to length of string");
            return -1;
        }

        buf[i] = (char)c;
    }
    buf[len] = '\0';

    return 0;
}

/*!
 * Reads a UTF-8 character from a file pointer into a char buffer
 * \param buf A character buffer in which the unicode code point will be written
 * \param file A file pointer from which the unicode code point will be read
 * \returns 0 on success and -1 on failure
 */
int fload_utf8(char* buf, FILE* file)
{
    int c = fgetc(file);
    if (c == EOF) { return c; }
    else if (c < 0) {
        fprintf(stderr, //NOLINT
                "A character with negative value has been read in wgetch\n");
        exit(1); //NOLINT
    }

    const int len = get_utf8_len(c);
    if (len == -1) {
        fprintf(stderr, //NOLINT
                "wget_utf8, first byte (= %d) did not conform to UTF-8", c);
        return -1;
    }
    assert(1 <= len && len <= 4);
    buf[0] = (char)c;
    for (int i = 1; i < len; ++i) {
        c = fgetc(file);
        if (!is_continuation(c)) {
            fprintf(stderr, //NOLINT
                    "file_load_utf8: Length indicated by first byte doesn't "
                    "correspond to length of string");
            return -1;
        }

        buf[i] = (char)c;
    }
    buf[len] = '\0';

    return 0;
}

/*!
 * Reads a UTF-8 word from a file pointer into a char buffer
 * \param buf A character buffer in which the unicode code point will be written
 * \param file A file pointer from which the unicode code point will be read
 * \returns The whitespace that ended the word on success (could also be EOF)
 * and 0 on failure
 */
int floadw_utf8(char* buf, FILE* file)
{
    buf[0]     = '\0';
    size_t len = 0;
    do {
        int err = fload_utf8(buf + len, file);
        if (err == EOF) { return EOF; }
        if (err == 0) {
            fprintf(stderr, //NOLINT
                    "fload_utf8 encountered an error in floadw_utf8");
            return 0;
        }

        len = strlen(buf);
    } while (!isspace(buf[len - 1]));

    return buf[len - 1];
}

/*!
 * Reads a UTF-8 character from the specified window and returns a malloced
 * string containing it
 * \param win The window from which to read the UTF-8 character
 * \returns A pointer to the malloced string on success, NULL on failure
 */
const char* wget_utf8(WINDOW* win)
{
    // A unicode character can be 1-4 bytes + null termination
    char* res = (char*)malloc(ASCII_BUF_SZ);

    int err = load_utf8(res, win);

    if (err != 0) {
        free(res);
        return NULL;
    }

    return res;
}

/*!
 * Returns the length of a UTF-8 string in unicode code points
 * \param str The string whose length will be calculated
 */
int utf8_strlen(const char* str)
{
    int count = 0;
    int i     = 0;
    while (str[i] != '\0') {
        i += get_utf8_len(str[i]);
        ++count;
    }

    return count;
}
