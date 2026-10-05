#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
void intercambio_xor(int *, int *);
void intercambio_aritmetico(int *, int *);
void intercambio_multiplicativo(int *, int *);
int main(void) {
    int a = INT_MIN, b = INT_MAX;
    intercambio_xor(&a, &b);
    assert(a == INT_MAX && b == INT_MIN);
    a = 0;
    b = -7;
    intercambio_xor(&a, &b);
    assert(a == -7 && b == 0);
    a = 42;
    intercambio_xor(&a, &a);
    assert(a == 42);
    int a2 = INT_MIN, b2 = INT_MAX;
    intercambio_aritmetico(&a2, &b2);
    assert(a2 == INT_MAX && b2 == INT_MIN);
    a2 = INT_MAX;
    b2 = INT_MAX - 1;
    intercambio_aritmetico(&a2, &b2);
    assert(a2 == INT_MAX - 1 && b2 == INT_MAX);
    a2 = INT_MIN;
    b2 = -1;
    intercambio_multiplicativo(&a2, &b2);
    assert(a2 == -1 && b2 == INT_MIN);
    a2 = INT_MAX;
    b2 = INT_MIN;
    intercambio_multiplicativo(&a2, &b2);
    assert(a2 == INT_MIN && b2 == INT_MAX);
    a2 = 0;
    b2 = 9;
    intercambio_multiplicativo(&a2, &b2);
    assert(a2 == 9 && b2 == 0);
    return 0;
}
