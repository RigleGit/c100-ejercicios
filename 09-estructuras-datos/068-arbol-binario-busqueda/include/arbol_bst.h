// API alineada a README/tests: BST con funciones bst_*
#ifndef ARBOL_BST_H
#define ARBOL_BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Nodo {
    int dato;
    struct Nodo* izquierdo;
    struct Nodo* derecho;
} Nodo;

typedef struct BST {
    Nodo* raiz;
    int tamanio;
} BST;

// Gestión
BST* bst_crear(void);
void bst_destruir(BST* bst);

// Operaciones básicas
bool bst_insertar(BST* bst, int dato);
bool bst_buscar(BST* bst, int dato);
bool bst_eliminar(BST* bst, int dato);

// Recorridos (imprimen con formato: "<val> ... \n")
void bst_recorrido_inorder(BST* bst);
void bst_recorrido_preorder(BST* bst);
void bst_recorrido_postorder(BST* bst);

// Utilidades de nodo (usadas por tests)
Nodo* bst_encontrar_minimo(Nodo* nodo);
Nodo* bst_encontrar_maximo(Nodo* nodo);

// Consultas
int bst_altura(BST* bst);            // -1 si vacío, 0 si una raíz
int bst_contar_nodos(BST* bst);
int bst_contar_hojas(BST* bst);
bool bst_es_valido(BST* bst);
int bst_suma_elementos(BST* bst);

// Visualización/estadísticas (no deben crashear)
void bst_imprimir_estructura(BST* bst);
void bst_mostrar_estadisticas(BST* bst);

#endif // ARBOL_BST_H
