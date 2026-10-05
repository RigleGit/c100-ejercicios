#include "potencia.h"
#include <math.h>

double potencia(double base, int exponente) {
    if (!isfinite(base) || (base == 0 && exponente < 0)) return NAN;
    unsigned int n = exponente < 0 ? 0u - (unsigned int)exponente : (unsigned int)exponente;
    if (exponente < 0) base = 1 / base;
    double resultado = 1;
    while (n) {
        if (n & 1u) resultado *= base;
        if (!isfinite(resultado)) return NAN;
        n >>= 1;
        if (n) {
            base *= base;
            if (!isfinite(base)) return NAN;
        }
    }
    return resultado;
}
