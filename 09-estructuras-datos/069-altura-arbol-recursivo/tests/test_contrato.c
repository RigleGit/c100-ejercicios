#ifdef NDEBUG
#undef NDEBUG
#endif
#include "altura_arbol.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    ArbolBinario *a = arbol_crear();
    assert(a && arbol_altura(a) == 0);
    assert(arbol_insertar(a, 5));
    assert(arbol_altura(a) == 1 && arbol_tamanio(a) == 1);
    assert(arbol_insertar(a, 3));
    assert(arbol_insertar(a, 7));
    assert(arbol_insertar(a, 8));
    assert(arbol_altura(a) == 3 && arbol_tamanio(a) == 4);
    assert(!arbol_insertar(a, 7) && arbol_tamanio(a) == 4);
    assert(altura_iterativa(a->raiz) == 3);
    assert(contar_nodos_nivel(a->raiz, 2) == 1);
    arbol_destruir(a);
    a = arbol_crear();
    assert(a);
    for (int i = 0; i < 5; i++)
        assert(arbol_insertar(a, i));
    assert(arbol_altura(a) == 5);
    arbol_destruir(a);
    return 0;
}
