/*!
 * \file utf8.c
 * \brief Implementation file to \ref utf8.h
 *
 * This file contains the implementation details of the functions declared in
 * the \ref utf8.h file. Unless you are debugging, \ref utf8.h "it" is probably
 * a better place to look for interesting/useful functions.
 */

#include <assert.h>
#include <ctype.h>
#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "utf8.h"

int load_utf8_tail(char* buf, Input inp);

//! Checks if the passed in byte is an ASCII character
static inline bool is_ascii(unsigned int c)
{
    return ASCII_MIN <= c && c <= ASCII_MAX;
}

//! Checks if the passed in byte c is a continuation byte in the UTF-8 format
static inline bool is_continuation(unsigned int c)
{
#define BIT1 128U
#define BIT2 64U

    return (BIT1 & c) != 0 && (BIT2 & c) == 0;
#undef BIT1
#undef BIT2
}

/*!
 * \brief Reads the length in bytes of a UTF-8 unicode code point from the
 * first byte
 *
 * \param[in] c The first byte of a UTF-8 unicode code point
 *
 * \returns An integer in the range [1, 4] indicating the length in bytes of the
 * unicode code point, or -1 on error
 */
static int get_utf8_len(unsigned int c)
{
    if (is_ascii(c)) { return 1; }
    unsigned const int mask = 192;
    if ((c & mask) != mask) { return -1; }

    //! \cond
#define CHECK(mask, return_val)                                                \
    do {                                                                       \
        if ((c & (mask)) != 0) {                                               \
            unsigned int const check = ((mask) >> 1U);                         \
            return ((check & c) != 0) ? -1 : (return_val);                     \
        }                                                                      \
    } while (0);

    CHECK(16U, 4);
    CHECK(32U, 3);
    CHECK(64U, 2);

    return -1;

#undef CHECK
    //! \endcond
}

char const* get_input_utf8(Input inp)
{
    char const* res = get_utf8(inp);
    if (!res) {
        log_msgf("Failed to get input in %s\n", __func__);
        return NULL;
    }

    return res;
}

int get_input_char(Input i)
{
    int res = -1;
    switch (i.tag) {
        case tag_win: res = wgetch(i.win); break;
        case tag_str: res = (unsigned char)*i.str; break;
    }

    return res;
}

void wait_press(Input i)
{
    int ch = get_input_char(i);
    char buf[ASCII_BUF_SZ];
    if (KEY_MIN <= ch && ch <= KEY_MAX) { return; }
    else {
        int err = load_utf8_tail(buf, i);
        if (err) { log_and_exit("Failed to read a character\n"); }
    }
}

/*!
 * Given a buffer whose first byte is the first byte of a UTF-8 character, and
 * an input source, load the full UTF-8 character into the buffer
 *
 * \paran[in,out] buf A character buffer of at least \link ASCII_BUF_SZ
 * \param[in] inp An input source to read characters from
 * \returns 0 on sucess, a negative number otherwise
 */
int load_utf8_tail(char* buf, Input inp)
{
    const int len = get_utf8_len(buf[0]);
    if (len == -1) {
        log_msgf("wget_utf8, first byte (= %d) did not conform to UTF-8\n",
                 buf[0]);
        return -1;
    }
    assert(1 <= len && len <= 4);
    int c = 0;
    for (int i = 1; i < len; ++i) {
        c = get_input_char(inp);
        if (!is_continuation(c)) {
            log_msgln(
                "load_utf8_tail: Length indicated by first byte doesn't "
                "correspond "
                "to length of string");
            return -1;
        }

        buf[i] = (char)c;
    }
    buf[len] = '\0';

    return 0;
}

/*!
 * Reads a UTF-8 character from an ncurses window into a char buffer
 *
 * \param[out] buf A character buffer in which the unicode code point will be
 * written
 * \param[in] win A ncurses window from which the unicode code point will be
 * read
 *
 * The buf parameter has to be at least of length \ref ASCII_BUF_SZ. A NUL
 * character will be appended after the UTF-8 character.
 *
 * \returns 0 on success -1 on failure
 */
int load_utf8(char* buf, Input inp)
{
    int c = get_input_char(inp);

    if (c < 0) {
        log_and_exit(
            "A character with negative value has been read in wgetch\n");
    }

    buf[0] = (char)c;
    return load_utf8_tail(buf, inp);
}

/*!
 * Reads a UTF-8 character from a file pointer into a char buffer
 *
 * \param[out] buf A character buffer in which the unicode code point will be
 * written
 * \param[in,out] file A file pointer from which the unicode code point will be
 * read
 *
 * The buf parameter has to be at least of length \ref ASCII_BUF_SZ. A NUL
 * character will be appended after the UTF-8 character.
 *
 * \returns 0 on success, EOF on EOF and 1 on failure
 */
int fload_utf8(char* buf, FILE* file)
{
    int c = fgetc(file);
    if (c == EOF) { return c; }
    else if (c < 0) {
        log_and_exit(
            "A character with negative value has been read in wgetch\n");
    }

    const int len = get_utf8_len(c);
    if (len == -1) {
        log_msgf("wget_utf8, first byte (= %d) did not conform to UTF-8\n", c);
        return 1;
    }
    assert(1 <= len && len <= 4);
    buf[0] = (char)c;
    for (int i = 1; i < len; ++i) {
        c = fgetc(file);
        if (!is_continuation(c)) {
            log_msgln(
                "file_load_utf8: Length indicated by first byte doesn't "
                "correspond to length of string");
            return 1;
        }

        buf[i] = (char)c;
    }
    buf[len] = '\0';

    return 0;
}

/*!
 * Reads a UTF-8 word from a file pointer into a char buffer.
 * The function will skip any initial whitespace.
 *
 * \param[out] buf A character buffer in which the unicode code point will be
 * written
 *
 * \param[in,out] file A file pointer from which the unicode code point will be
 * read
 *
 * \returns The whitespace that ended the word on success (could also be EOF)
 *  and 0 on failure
 *
 * If, when the function is called, the file stream is exhausted or only
 * whitespace is left in the file buffer
 * a NUL character will be placed at buf[0]
 *
 * **It is the responsibility of the caller** that buf is long enough to contain
 * any word that might be read *plus a NUL character*.
 */
int floadw_utf8(char* buf, FILE* file)
{
    //If no characters are left we still want buf to be null terminated
    buf[0] = '\0';
    int ch = 0;
    while (isspace((ch = fgetc(file)))) {}

    if (ch == EOF) { return EOF; }

    int err = ungetc(ch, file);
    if (err == EOF) {
        log_msgln("ungetc failed in floadw_utf8");
        return 0;
    }

    size_t len = 0;
    do {
        int err = fload_utf8(buf + len, file);
        if (err == EOF) { return EOF; }
        if (err == 1) {
            log_msgln("fload_utf8 encountered an error in floadw_utf8");
            return 0;
        }

        len = strlen(buf);
    } while (!isspace(buf[len - 1]));

    int res      = (int)buf[len - 1];
    buf[len - 1] = '\0';

    return res;
}

/*!
 * Reads a UTF-8 character from the specified input source and returns a
 * malloced string containing it
 *
 * \param[in] win The \ref Input "input" from which to read the UTF-8 character
 *
 * \returns A pointer to the malloced string on success, NULL on failure
 */
const char* get_utf8(Input inp)
{
    // A unicode character can be 1-4 bytes + null termination
    char* res = (char*)malloc(ASCII_BUF_SZ);

    int err = load_utf8(res, inp);

    if (err != 0) {
        free(res);
        return NULL;
    }

    return res;
}

/*!
 * Returns the length of a UTF-8 string in unicode code points
 *
 * \param[in] str The string whose length will be calculated
 *
 * \returns The length of a UTF-8 string in terms of unicode points
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
