#ifdef NDEBUG
#undef NDEBUG
#endif
#include "macros_con_argumentos.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(CUADRADO(3 + 2) == 25);
    assert(CUBO(-2) == -8);
    assert(2 * SUMA(3, 4) == 14);
    assert(DIVISION_SEGURA(8, 2) == 4 && DIVISION_SEGURA(8, 0) == 0);
    assert(LIMITAR(-1, 0, 10) == 0 && LIMITAR(11, 0, 10) == 10 && LIMITAR(5, 0, 10) == 5);
    int a = 3, b = 4;
    if (a == 3)
        INTERCAMBIAR(a, b);
    else
        a = 0;
    assert(a == 4 && b == 3);
    assert(ES_PAR(-4) && ES_IMPAR(-3));
    return 0;
}
