#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

//NOLINTBEGIN
struct Vec
{
    int sz;
    int cap;
    void* data;
};

void free_vec(void* v) { free(((struct Vec*)v)->data); }

#define create_vec(type, eq_fnc)                                               \
                                                                               \
    Vec_##type new_vec_##type(int cap)                                         \
    {                                                                          \
        return (Vec_##type){                                                   \
            .sz = 0, .cap = cap, .data = (type*)malloc(cap * sizeof(type))};   \
    }                                                                          \
    type Vec_get_##type(Vec_##type vec, int i)                                 \
    {                                                                          \
        assert(i < vec.sz);                                                    \
        return vec.data[i];                                                    \
    }                                                                          \
    void Vec_grow_##type(Vec_##type* vec)                                      \
    {                                                                          \
        type* new_data = (type*)malloc(                                        \
            GROWTH_FACTOR * (unsigned long)vec->cap * sizeof(type));           \
        memcpy(new_data, vec->data, vec->sz * sizeof(type));                   \
        free(vec->data);                                                       \
        vec->cap  = GROWTH_FACTOR * vec->cap;                                  \
        vec->data = new_data;                                                  \
    }                                                                          \
                                                                               \
    void Vec_push_##type(Vec_##type* v, type e)                                \
    {                                                                          \
        if (v->sz == v->cap) { Vec_grow_##type(v); }                           \
        v->data[v->sz++] = e;                                                  \
    }                                                                          \
                                                                               \
    bool Vec_contains_##type(Vec_##type v, type e)                             \
    {                                                                          \
        for (int i = 0; i < v.sz; ++i) {                                       \
            if (eq_fnc(v.data[i], e)) { return true; }                         \
        }                                                                      \
        return false;                                                          \
    }                                                                          \
    type Vec_back_##type(Vec_##type v) { return v.data[v.sz - 1]; }            \
    type vec_pop_##type(Vec_##type* v) { return v->data[--v->sz]; }

bool int_eq(int a, int b) { return a == b; }

bool coord_eq(coord c, coord c2) { return c.x == c2.x && c.y == c2.y; }

create_vec(int, int_eq);
create_vec(coord, coord_eq);

//NOLINTEND
