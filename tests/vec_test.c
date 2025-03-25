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
    VEC_PUSH(&v, 0);
    VEC_PUSH(&v, 1);
    VEC_PUSH(&v, 2);
    VEC_PUSH(&v, 3);
    VEC_PUSH(&v, 4);

    for (int i = 0; i < v.sz; ++i) { assert(VEC_GET(v, i) == i); }

    for (int i = 0; i < v.sz; ++i) { assert(v.data[i] == i); }

    free_vec(&v);
    v = new_vec_int(4);
    for (int i = 1; i <= 4; ++i) {
        VEC_PUSH(&v, i);
        assert(v.cap == 4);
        assert(v.sz == i);
    }

    VEC_PUSH(&v, 5);
    assert(v.sz == 5);
    assert(v.cap == GROWTH_FACTOR * 4);

    for (int i = 0; i < 5; ++i) { assert(v.data[i] == i + 1); }
}

void test_pop()
{
    Vec_int v = new_vec_int(16);
    VEC_PUSH(&v, 0);
    VEC_PUSH(&v, 1);
    VEC_PUSH(&v, 2);
    VEC_PUSH(&v, 3);
    VEC_PUSH(&v, 4);

    assert(v.sz == 5);
    assert(VEC_POP(&v) == 4);
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
