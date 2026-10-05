#ifdef NDEBUG
#undef NDEBUG
#endif
#include "eliminar_nodo.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Lista *l = crear_lista();
    assert(l && !eliminar_primero(l));
    const int v[] = {1, 2, 1, 3, 1};
    for (int i = 0; i < 5; i++)
        assert(insertar_final(l, v[i]));
    assert(eliminar_todos(l, 1) == 3);
    assert(obtener_longitud(l) == 2 && obtener_en_posicion(l, 0)->valor == 2 &&
           obtener_en_posicion(l, 1)->valor == 3);
    assert(!eliminar_nodo(l, 99));
    assert(eliminar_ultimo(l));
    assert(eliminar_primero(l));
    assert(esta_vacia(l));
    assert(insertar_final(l, 7));
    assert(obtener_en_posicion(l, 0)->valor == 7 && obtener_longitud(l) == 1);
    destruir_lista(l);
    return 0;
}
