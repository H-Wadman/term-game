/*!
 * \file vec.h
 * \brief Resizeable runtime array to the tune of a C++ vector
 */

#pragma once

#include <stdbool.h>

/*!
 * \brief Macro for forward_declaring a vector
 *
 * This macro will define a vector struct (resizeable array) for a specific type
 * (e.g. Vec_int for forward_declare(int)). This will allow creating a new
 * vector with new_vec_type, (e.g. new_vec_int).
 *
 * After that calling the generic
 * vector functions will be possible, as soon as they've been added into the
 * _Generic declarations
 *
 * \param type The type that the vector will contain
 */
#define FORWARD_DECLARE_VEC(type)                                              \
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
    Vec_##type new_vec_##type(int cap)

enum
{
    GROWTH_FACTOR = 2
};

#define SHRINK_FACTOR 1.5

//! Bounds checked element access
#define VEC_GET(v, i)                                                          \
    _Generic((v), Vec_int: Vec_get_int, Vec_coord: Vec_get_coord)(v, i)

//! Add an element to the back of the vector
#define VEC_PUSH(v, e)                                                         \
    _Generic((v), Vec_int *: Vec_push_int, Vec_coord *: Vec_push_coord)(v, e)

//! Remove the element at the back of the vector and return it
#define VEC_POP(v)                                                             \
    _Generic((v), Vec_int *: vec_pop_int, Vec_coord *: vec_pop_coord)(v)

//O(n)
#define VEC_CONTAINS(v, e)                                                     \
    _Generic((v), Vec_int: Vec_contains_int, Vec_coord: Vec_contains_coord)(v, \
                                                                            e)

//! Get the last element
#define VEC_BACK(v)                                                            \
    _Generic((v), Vec_int: Vec_back_int, Vec_coord: Vec_back_coord)(v)

typedef struct Coord
{
    int y;
    int x;
} coord;

//! Free the resources associated with vector
void free_vec(void* v);

FORWARD_DECLARE_VEC(int);
FORWARD_DECLARE_VEC(coord);
