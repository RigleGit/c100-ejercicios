#ifdef NDEBUG
#undef NDEBUG
#endif
#include "invertir_lista.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Lista *l = crear_lista();
    assert(l && invertir_lista(l));
    for (int i = 1; i <= 5; i++)
        assert(insertar_final(l, i));
    assert(invertir_sublista(l, 1, 3));
    const int a[] = {1, 4, 3, 2, 5};
    for (size_t i = 0; i < 5; i++)
        assert(obtener_en_posicion(l, i)->valor == a[i]);
    assert(invertir_lista_recursivo(l));
    const int b[] = {5, 2, 3, 4, 1};
    for (size_t i = 0; i < 5; i++)
        assert(obtener_en_posicion(l, i)->valor == b[i]);
    assert(!invertir_sublista(l, 0, 5));
    assert(insertar_final(l, 6));
    assert(obtener_en_posicion(l, 5)->valor == 6);
    destruir_lista(l);
    return 0;
}
