/*!
 * \file utf8.h
 *
 * \brief UTF-8 string handling and io
 *
 *  A file containing functions related to UTF-8 string handling and io.
 */

#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

enum
{
    ASCII_MAX = 127,
    ASCII_MIN = 0,
    //! The size a buffer has to be for loading a utf8 char
    ASCII_BUF_SZ = 5
};

typedef struct Input
{
    union
    {
        WINDOW* win;
        char const* str;
    };

    enum
    {
        tag_win,
        tag_str
    } tag;
} Input;

//! Interactive get input
const char* get_input_utf8(Input inp);

//! Interactive get input
int get_input_char(Input inp);

//! Reads a UTF-8 unicode point from inp and returns it as a malloced string
const char* get_utf8(Input inp);

//! Reads a UTF-8 unicode point from inp into buf
int load_utf8(char* buf, Input inp);

//! Reads a UTF-8 unicode point from file into buf
int fload_utf8(char* buf, FILE* file);

//! Reads a UTF-8 word (until whitespace) from file into buf
int floadw_utf8(char* buf, FILE* file);

//! Returns the length of a UTF-8 string in unicode code points
int utf8_strlen(const char* str);
