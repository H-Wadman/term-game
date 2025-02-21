#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "logging.h"

static FILE* errout = NULL; //NOLINT

void set_log_output(FILE* f) { errout = f; }

void close_log_stream() { (void)fclose(errout); }

void log_msg(char const* msg)
{
    int err = fputs(msg, errout);
    if (err == EOF) {
        fprintf(stderr, "Logging failed in %s\n", __func__); //NOLINT
        exit(1);
    }
}

void log_msgln(char const* msg)
{
    log_msg(msg);
    log_msg("\n");
}

void log_msgf(char const* fmt, ...)
{
    va_list args = NULL;
    va_start(args, fmt);
    (void)vfprintf(errout, fmt, args);

    va_end(args);
}

void log_and_exit(char const* fmt, ...)
{
    va_list args = NULL;
    va_start(args, fmt);

    (void)vfprintf(errout, fmt, args);
    va_end(args);
    exit(1);
}

void impl_log_def(struct log_def_args args); //NOLINT

#define log_def(mesg, ...)                                                     \
    impl_log_def((struct log_def_args){                                        \
        .msg = (mesg), .location_info = true, __VA_ARGS__})
