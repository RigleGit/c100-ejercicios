#ifdef NDEBUG
#undef NDEBUG
#endif
#include "redimensionar_array.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    ArrayRedimensionable *a = crearArrayRedimensionable(2);
    assert(a && a->cantidad == 0);
    for (int i = 0; i < 7; i++)
        assert(agregarElemento(a, i * 10));
    assert(a->cantidad == 7);
    for (int i = 0; i < 7; i++)
        assert(a->datos[i] == i * 10);
    assert(insertarEnPosicion(a, 0, -5));
    assert(insertarEnPosicion(a, 4, 99));
    assert(eliminarEnPosicion(a, 1));
    const int e[] = {-5, 10, 20, 99, 30, 40, 50, 60};
    assert(a->cantidad == 8);
    for (size_t i = 0; i < 8; i++)
        assert(a->datos[i] == e[i]);
    assert(!eliminarEnPosicion(a, 8));
    liberarArrayRedimensionable(a);
    return 0;
}
