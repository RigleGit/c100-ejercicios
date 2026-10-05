#ifdef NDEBUG
#undef NDEBUG
#endif
#include "busqueda_binaria.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    int a[] = {-5, 0, 2, 2, 2, 9};
    assert(busqueda_binaria_iterativa(a, 6, -5) == 0);
    assert(busqueda_binaria_recursiva(a, 0, 5, 9) == 5);
    assert(busqueda_binaria_iterativa(a, 6, 3) == -1);
    assert(busqueda_binaria_iterativa(a, 0, 2) == -1);
    assert(busqueda_primera_ocurrencia(a, 6, 2) == 2);
    assert(busqueda_ultima_ocurrencia(a, 6, 2) == 4);
    ResultadoBusquedaMultiple r = busqueda_todas_ocurrencias(a, 6, 2);
    assert(r.primera_posicion == 2 && r.ultima_posicion == 4 && r.total_ocurrencias == 3);
    assert(busqueda_floor(a, 6, 3) == 4 && busqueda_ceiling(a, 6, 3) == 5);
    assert(busqueda_floor(a, 6, -6) == -1 && busqueda_ceiling(a, 6, 10) == -1);
    return 0;
}
