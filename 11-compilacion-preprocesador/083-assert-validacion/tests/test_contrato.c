#ifdef NDEBUG
#undef NDEBUG
#endif
#include "assert_validacion.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(raiz_cuadrada(25) == 5 && raiz_cuadrada(0) == 0);
    double out = 99;
    assert(!raiz_cuadrada_segura(-1, &out) && out == 99);
    assert(!raiz_cuadrada_segura(4, NULL));
    assert(raiz_cuadrada_segura(9, &out) && out == 3);
    assert(division_segura(9, 2) == 4.5);
    const double a[] = {-2, 4, 7};
    assert(promedio_array(a, 3) == 3);
    assert(buscar_elemento(a, 3, 4) == 1 && buscar_elemento(a, 3, 8) == -1);
    char target[8];
    copiar_cadena("Hola", target);
    assert(strcmp(target, "Hola") == 0 && longitud_cadena("") == 0);
    return 0;
}
