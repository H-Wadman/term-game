#ifndef VEC_H
#define VEC_H

#include <stdbool.h>

#define GROWTH_FACTOR 2
#define SHRINK_FACTOR 1.5

#define forward_declare_vec(type)                                              \
    typedef struct Vec_##type                                                  \
    {                                                                          \
        int sz;                                                                \
        int cap;                                                               \
        type* data;                                                            \
    } Vec_##type;                                                              \
    type Vec_get_##type(Vec_##type vec, int i);                                \
    void Vec_push_##type(Vec_##type* v, type e);                               \
    bool Vec_contains_##type(Vec_##type v, type e);                            \
    type Vec_back_##type(Vec_##type v);                                        \
    type vec_pop_##type(Vec_##type* v);                                        \
    Vec_##type new_vec_##type(int cap);

#define vec_get(v, i)                                                          \
    _Generic((v), Vec_int: Vec_get_int, Vec_coord: Vec_get_coord)(v, i)

#define vec_push(v, e)                                                         \
    _Generic((v), Vec_int *: Vec_push_int, Vec_coord *: Vec_push_coord)(v, e)

#define vec_pop(v)                                                             \
    _Generic((v), Vec_int *: vec_pop_int, Vec_coord *: vec_pop_coord)(v)

//O(n^2)
#define vec_contains(v, e)                                                     \
    _Generic((v), Vec_int: Vec_contains_int, Vec_coord: Vec_contains_coord)(v, \
                                                                            e)

#define vec_back(v)                                                            \
    _Generic((v), Vec_int: Vec_back_int, Vec_coord: Vec_back_coord)(v)

typedef struct Coord
{
    int y;
    int x;
} coord;

void free_vec(void* v);

forward_declare_vec(int);
forward_declare_vec(coord);

#endif
