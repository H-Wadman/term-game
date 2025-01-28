#include "vec.h"
#include <assert.h>

//NOLINTBEGIN
void test_new_free()
{
    Vec_int v = new_vec_int(5);

    assert(v.cap == 5);
    assert(v.sz == 0);
    assert(v.data);
    free_vec(&v);

    v = new_vec_int(100);
    assert(v.cap == 100);
    assert(v.sz == 0);
    assert(v.data);
    free_vec(&v);
}

void test_push()
{
    Vec_int v = new_vec_int(16);
    vec_push(&v, 0);
    vec_push(&v, 1);
    vec_push(&v, 2);
    vec_push(&v, 3);
    vec_push(&v, 4);

    for (int i = 0; i < v.sz; ++i) { assert(vec_get(v, i) == i); }

    for (int i = 0; i < v.sz; ++i) { assert(v.data[i] == i); }

    free_vec(&v);
    v = new_vec_int(4);
    for (int i = 1; i <= 4; ++i) {
        vec_push(&v, i);
        assert(v.cap == 4);
        assert(v.sz == i);
    }

    vec_push(&v, 5);
    assert(v.sz == 5);
    assert(v.cap == GROWTH_FACTOR * 4);

    for (int i = 0; i < 5; ++i) { assert(v.data[i] == i + 1); }
}

void test_pop()
{
    Vec_int v = new_vec_int(16);
    vec_push(&v, 0);
    vec_push(&v, 1);
    vec_push(&v, 2);
    vec_push(&v, 3);
    vec_push(&v, 4);

    assert(v.sz == 5);
    assert(vec_pop(&v) == 4);
    assert(v.sz == 4);
}

void test()
{
    test_new_free();
    test_push();
    test_pop();
}

//NOLINTEND

int main() { test(); }
