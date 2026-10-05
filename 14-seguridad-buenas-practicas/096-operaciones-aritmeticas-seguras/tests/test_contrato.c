#include "operaciones_aritmeticas_seguras.h"
#include <assert.h>
#include <limits.h>
int main(void) {
    int r = 99;
    assert(suma_segura(7, -3, &r) == ARITMETICA_EXITOSA && r == 4);
    r = 99;
    assert(suma_segura(INT_MAX, 1, &r) == ARITMETICA_OVERFLOW && r == 99);
    assert(suma_segura(INT_MIN, -1, &r) == ARITMETICA_UNDERFLOW && r == 99);
    assert(resta_segura(INT_MIN, 1, &r) == ARITMETICA_UNDERFLOW);
    assert(division_segura(8, 0, &r) == ARITMETICA_DIVISION_CERO);
    assert(division_segura(INT_MIN, -1, &r) == ARITMETICA_OVERFLOW);
    assert(modulo_seguro(INT_MIN, -1, &r) == ARITMETICA_EXITOSA && r == 0);
    assert(multiplicacion_segura(-6, 7, &r) == ARITMETICA_EXITOSA && r == -42);
    assert(multiplicacion_segura(INT_MIN / 2, 2, &r) == ARITMETICA_EXITOSA && r == INT_MIN);
    assert(suma_segura(1, 2, NULL) == ARITMETICA_ERROR_PARAMETRO);
    int min, max;
    assert(calcular_rango_suma_segura(1, &min, &max));
    assert(min == INT_MIN && max == INT_MAX - 1);
    assert(calcular_rango_suma_segura(-1, &min, &max));
    assert(min == INT_MIN + 1 && max == INT_MAX);
    return 0;
}
