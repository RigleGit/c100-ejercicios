#ifdef NDEBUG
#undef NDEBUG
#endif
#include "arbol_bst.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    BST *b = bst_crear();
    assert(b && bst_altura(b) == -1);
    const int v[] = {8, 3, 10, 1, 6, 9, 12};
    for (int i = 0; i < 7; i++)
        assert(bst_insertar(b, v[i]));
    assert(!bst_insertar(b, 6));
    assert(bst_contar_nodos(b) == 7 && bst_altura(b) == 2 && bst_suma_elementos(b) == 49);
    assert(bst_eliminar(b, 8));
    assert(!bst_buscar(b, 8));
    const int survivors[] = {1, 3, 6, 9, 10, 12};
    for (int i = 0; i < 6; i++)
        assert(bst_buscar(b, survivors[i]));
    assert(bst_contar_nodos(b) == 6 && bst_suma_elementos(b) == 41);
    assert(!bst_eliminar(b, 88));
    bst_destruir(b);
    return 0;
}
