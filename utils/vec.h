#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//NOLINTBEGIN
struct Vec
{
    int sz;
    int cap;
    void* data;
};

void free_vec(void* v) { free(((struct Vec*)v)->data); }

#define GROWTH_FACTOR 2
#define SHRINK_FACTOR 1.5
#define create_vec(type)                                                       \
    typedef struct Vec_##type                                                  \
    {                                                                          \
        int sz;                                                                \
        int cap;                                                               \
        type* data;                                                            \
    } Vec_##type;                                                              \
                                                                               \
    Vec_##type new_vec_##type(int cap)                                         \
    {                                                                          \
        return (Vec_##type){                                                   \
            .sz = 0, .cap = cap, .data = (type*)malloc(cap * sizeof(type))};   \
    }                                                                          \
    type Vec_get_##type(Vec_##type vec, int i) { return vec.data[i]; }         \
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
            if (v.data[i] == e) { return true; }                               \
        }                                                                      \
        return false;                                                          \
    }                                                                          \
    type vec_pop_##type(Vec_##type* v)                                         \
    {                                                                          \
        if (v->sz == 1) { v->data = NULL; }                                    \
        return v->data[--v->sz];                                               \
    }


create_vec(int)

#define vec_get(v, i) _Generic((v), Vec_int: Vec_get_int)(v, i)

#define vec_push(v, e) _Generic((v), Vec_int *: Vec_push_int)(v, e)

#define vec_pop(v) _Generic((v), Vec_int *: vec_pop_int)(v)

//O(n^2)
#define vec_contains(v, e) _Generic((v), Vec_int: Vec_contains_int)(v, e)
//NOLINTEND
#endif
