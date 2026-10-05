#include <assert.h>
#include <limits.h>
#include "factorial.h"
int main(void) {
    assert(factorial(0) == 1);
    assert(factorial(5) == 120);
    assert(factorial(21) == -1);
    assert(factorial(-1) == -1);
    if (LONG_MAX >= 2432902008176640000LL) assert(factorial(20) == 2432902008176640000LL);
    return 0;
}
