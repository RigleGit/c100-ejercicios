#include <stdio.h>
#include "../include/arbol_bst.h"

int main(void) {
    BST* bst = bst_crear();
    if (!bst) return 1;
    bst_insertar(bst, 50);
    bst_insertar(bst, 30);
    bst_insertar(bst, 70);
    bst_insertar(bst, 20);
    bst_insertar(bst, 40);
    bst_insertar(bst, 60);
    bst_insertar(bst, 80);

    printf("Recorrido in-order: ");
    bst_recorrido_inorder(bst);
    bst_mostrar_estadisticas(bst);
    bst_destruir(bst);
    return 0;
}

