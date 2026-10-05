#ifdef NDEBUG
#undef NDEBUG
#endif
#include "lista_doble.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    ListaDoble *l = lista_crear();
    assert(l && lista_vacia(l));
    assert(lista_insertar_final(l, 2));
    assert(lista_insertar_inicio(l, 1));
    assert(lista_insertar_final(l, 3));
    const int f[] = {1, 2, 3}, r[] = {3, 2, 1};
    IteradorDoble it = iterador_crear(l, true);
    for (int i = 0; i < 3; i++) {
        assert(iterador_tiene_siguiente(&it));
        assert(iterador_siguiente(&it) == f[i]);
    }
    assert(!iterador_tiene_siguiente(&it));
    it = iterador_crear(l, false);
    for (int i = 0; i < 3; i++)
        assert(iterador_siguiente(&it) == r[i]);
    assert(!iterador_tiene_siguiente(&it));
    assert(lista_eliminar_posicion(l, 1));
    assert(lista_tamanio(l) == 2);
    it = iterador_crear(l, false);
    assert(iterador_siguiente(&it) == 3);
    assert(iterador_siguiente(&it) == 1);
    assert(lista_eliminar_inicio(l));
    assert(lista_eliminar_final(l));
    assert(lista_vacia(l));
    assert(lista_insertar_final(l, 9));
    int value;
    assert(lista_primero(l, &value) && value == 9);
    lista_destruir(l);
    return 0;
}
