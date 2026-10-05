#ifdef NDEBUG
#undef NDEBUG
#endif
#include "quicksort.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    const int in[] = {3, -4, 3, 0, 9, -4, 2};
    const int e[] = {-4, -4, 0, 2, 3, 3, 9};
    int a[7];
    memcpy(a, in, sizeof a);
    quicksort_basico(a, 0, 6);
    for (int i = 0; i < 7; i++)
        assert(a[i] == e[i]);
    memcpy(a, in, sizeof a);
    quicksort_iterativo(a, 7);
    for (int i = 0; i < 7; i++)
        assert(a[i] == e[i]);
    int one = 42;
    quicksort_basico(&one, 0, 0);
    assert(one == 42);
    quicksort_basico(&one, 0, -1);
    assert(one == 42);
    return 0;
}
