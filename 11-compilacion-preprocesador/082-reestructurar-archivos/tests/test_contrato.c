#ifdef NDEBUG
#undef NDEBUG
#endif
#include "mimath.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(suma(7, -3) == 4);
    assert(division(-7, 2) == -3);
    assert(division(3, 0) == 0);
    assert(modulo(7, 3) == 1);
    assert(potencia(2, 10) == 1024);
    assert(factorial(0) == 1 && factorial(5) == 120);
    assert(mcd(-54, 24) == 6 && mcm(6, 8) == 24);
    assert(es_primo(2) && es_primo(97) && !es_primo(1) && !es_primo(100));
    int a[] = {-3, 8, 1};
    assert(suma_array(a, 3) == 6 && promedio_array(a, 3) == 2.0);
    assert(raiz_cuadrada(0) == 0 && raiz_cuadrada(1) == 1 && raiz_cuadrada(9) == 3);
    assert(raiz_cuadrada(3) == 1 && raiz_cuadrada(8) == 2 && raiz_cuadrada(15) == 3);
    return 0;
}
