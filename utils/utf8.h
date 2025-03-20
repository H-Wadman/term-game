/*!
 * \file utf8.h
 *
 * \brief UTF-8 string handling and io
 *
 *  A file containing functions related to UTF-8 string handling and io.
 */

#ifndef UTF8_H
#define UTF8_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#define ASCII_MAX 127
#define ASCII_MIN 0
//! The size a buffer has to be for loading a utf8 char
#define ASCII_BUF_SZ 5

//! Reads a UTF-8 unicode point from win and returns it as a malloced string
const char* wget_utf8(WINDOW* win);

//! Reads a UTF-8 unicode point from win into buf
int load_utf8(char* buf, WINDOW* win);

//! Reads a UTF-8 unicode point from file into buf
int fload_utf8(char* buf, FILE* file);

//! Reads a UTF-8 word (until whitespace) from file into buf
int floadw_utf8(char* buf, FILE* file);

//! Returns the length of a UTF-8 string in unicode code points
int utf8_strlen(const char* str);


#endif
