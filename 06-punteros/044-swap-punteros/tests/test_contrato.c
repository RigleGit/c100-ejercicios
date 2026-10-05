#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
void swap(int *, int *);
void swap_xor(int *, int *);
void swap_generico(void *, void *, size_t);
void swap_punteros(int **, int **);
int swap_seguro(int *, int *);
int main(void) {
    int a = INT_MIN, b = INT_MAX;
    swap(&a, &b);
    assert(a == INT_MAX && b == INT_MIN);
    swap_xor(&a, &b);
    assert(a == INT_MIN && b == INT_MAX);
    swap_xor(&a, &a);
    assert(a == INT_MIN);
    assert(swap_seguro(NULL, &b) == 0 && b == INT_MAX);
    a = -4;
    b = 9;
    assert(swap_seguro(&a, &b) == 1 && a == 9 && b == -4);
    unsigned char x[] = {0, 128, 255}, y[] = {7, 6, 5};
    swap_generico(x, y, 3);
    const unsigned char ex[] = {7, 6, 5}, ey[] = {0, 128, 255};
    assert(memcmp(x, ex, 3) == 0 && memcmp(y, ey, 3) == 0);
    int *p = &a, *q = &b;
    swap_punteros(&p, &q);
    assert(p == &b && q == &a);
    return 0;
}
