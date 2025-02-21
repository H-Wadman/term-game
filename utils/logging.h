#include <stdbool.h>
#include <stdio.h>

//!Logging library providing a few convenience functions as well as avoiding
//!littering code with NOLINT statements

void set_log_output(FILE* f);
void close_log_stream();

void log_msg(char const* msg);
void log_msgln(char const* msg);

void log_msgf(char const* fmt, ...);

void log_and_exit(char const* fmt, ...);

struct log_def_args
{
    const char* msg;
    bool location_info;
};

void impl_log_def(struct log_def_args args);

#define log_def(mesg, ...)                                                     \
    impl_log_def((struct log_def_args){                                        \
        .msg = (mesg), .location_info = true, __VA_ARGS__})
