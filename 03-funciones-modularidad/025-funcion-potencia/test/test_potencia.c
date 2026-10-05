#include <assert.h>
#include <limits.h>
#include <math.h>
#include "potencia.h"
int main(void) {
    assert(potencia(2, 3) == 8);
    assert(potencia(2, -3) == 0.125);
    assert(potencia(0, 5) == 0);
    assert(potencia(2, 0) == 1);
    assert(isnan(potencia(0, -1)));
    assert(potencia(1, INT_MIN) == 1);
    assert(isnan(potencia(2, INT_MAX)));
    return 0;
}
