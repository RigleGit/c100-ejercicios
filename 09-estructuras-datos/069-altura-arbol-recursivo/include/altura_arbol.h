/*
 * Ejercicio 069: Altura de Árbol Binario Recursivo
 * 
 * Header con definiciones de estructuras y funciones para calcular
 * la altura de un árbol binario usando recursión.
 * 
 * La altura se define como la longitud del camino más largo desde
 * la raíz hasta una hoja. Un árbol vacío tiene altura 0.
 */

#ifndef ALTURA_ARBOL_H
#define ALTURA_ARBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* ================================
 * ESTRUCTURAS DE DATOS
 * ================================ */

/**
 * @brief Estructura de un nodo del árbol binario
 */
typedef struct NodoArbol {
    int valor;                    // Dato almacenado en el nodo
    struct NodoArbol* izq;        // Puntero al hijo izquierdo
    struct NodoArbol* der;        // Puntero al hijo derecho
} NodoArbol;

/**
 * @brief Estructura del árbol binario con metadatos
 */
typedef struct ArbolBinario {
    NodoArbol* raiz;              // Nodo raíz del árbol
    int tamanio;                  // Número de nodos en el árbol
} ArbolBinario;

/* ================================
 * FUNCIONES BÁSICAS DEL ÁRBOL
 * ================================ */

/**
 * @brief Crea un nuevo árbol binario vacío
 * @return Puntero al árbol creado o NULL si hay error
 */
ArbolBinario* arbol_crear(void);

/**
 * @brief Libera toda la memoria del árbol
 * @param arbol Puntero al árbol a destruir
 */
void arbol_destruir(ArbolBinario* arbol);

/**
 * @brief Crea un nuevo nodo con el valor dado
 * @param valor Valor a almacenar en el nodo
 * @return Puntero al nodo creado o NULL si hay error
 */
NodoArbol* nodo_crear(int valor);

/**
 * @brief Inserta un valor en el árbol (BST)
 * @param arbol Puntero al árbol
 * @param valor Valor a insertar
 * @return true si se insertó correctamente, false en caso contrario
 */
bool arbol_insertar(ArbolBinario* arbol, int valor);

/**
 * @brief Función auxiliar recursiva para insertar en BST
 * @param raiz Nodo raíz del subárbol
 * @param valor Valor a insertar
 * @return Puntero al nodo raíz (actualizado)
 */
NodoArbol* insertar_recursivo(NodoArbol* raiz, int valor);

/* ================================
 * FUNCIONES DE ALTURA
 * ================================ */

/**
 * @brief Calcula la altura del árbol completo
 * @param arbol Puntero al árbol
 * @return Altura del árbol (0 si está vacío)
 */
int arbol_altura(ArbolBinario* arbol);

/**
 * @brief Función recursiva principal para calcular altura
 * @param raiz Nodo raíz del subárbol
 * @return Altura del subárbol (0 si raiz es NULL)
 */
int altura_recursiva(NodoArbol* raiz);

/**
 * @brief Versión iterativa para calcular altura (comparación)
 * @param raiz Nodo raíz del subárbol
 * @return Altura del subárbol
 */
int altura_iterativa(NodoArbol* raiz);

/**
 * @brief Calcula la altura de un nodo específico
 * @param arbol Puntero al árbol
 * @param valor Valor del nodo cuya altura se quiere calcular
 * @return Altura del nodo o -1 si no existe
 */
int altura_nodo(ArbolBinario* arbol, int valor);

/* ================================
 * FUNCIONES AUXILIARES DE ALTURA
 * ================================ */

/**
 * @brief Encuentra la profundidad de un nodo desde la raíz
 * @param raiz Nodo raíz del árbol
 * @param valor Valor a buscar
 * @param profundidad Profundidad actual (inicialmente 0)
 * @return Profundidad del nodo o -1 si no existe
 */
int encontrar_profundidad(NodoArbol* raiz, int valor, int profundidad);

/**
 * @brief Calcula la altura balanceada (diferencia entre subárboles)
 * @param raiz Nodo raíz del subárbol
 * @return Factor de balance (-1, 0, 1 para árbol balanceado)
 */
int factor_balance(NodoArbol* raiz);

/**
 * @brief Verifica si el árbol está balanceado en altura
 * @param raiz Nodo raíz del subárbol
 * @return true si está balanceado, false en caso contrario
 */
bool es_balanceado(NodoArbol* raiz);

/**
 * @brief Calcula la altura máxima posible con n nodos
 * @param n Número de nodos
 * @return Altura máxima teórica
 */
int altura_maxima_teorica(int n);

/**
 * @brief Calcula la altura mínima posible con n nodos
 * @param n Número de nodos
 * @return Altura mínima teórica
 */
int altura_minima_teorica(int n);

/* ================================
 * FUNCIONES DE ANÁLISIS
 * ================================ */

/**
 * @brief Cuenta nodos en un nivel específico
 * @param raiz Nodo raíz del subárbol
 * @param nivel Nivel a contar (0 = raíz)
 * @return Número de nodos en el nivel
 */
int contar_nodos_nivel(NodoArbol* raiz, int nivel);

/**
 * @brief Encuentra el nivel con más nodos
 * @param arbol Puntero al árbol
 * @return Nivel con más nodos
 */
int nivel_mas_poblado(ArbolBinario* arbol);

/**
 * @brief Calcula el número de hojas en el árbol
 * @param raiz Nodo raíz del subárbol
 * @return Número de hojas
 */
int contar_hojas(NodoArbol* raiz);

/**
 * @brief Calcula el número de nodos internos
 * @param raiz Nodo raíz del subárbol
 * @return Número de nodos internos
 */
int contar_nodos_internos(NodoArbol* raiz);

/* ================================
 * FUNCIONES DE VISUALIZACIÓN
 * ================================ */

/**
 * @brief Imprime el árbol con información de altura
 * @param arbol Puntero al árbol
 */
void imprimir_arbol_con_altura(ArbolBinario* arbol);

/**
 * @brief Función auxiliar para imprimir con indentación
 * @param raiz Nodo raíz del subárbol
 * @param nivel Nivel actual (para indentación)
 */
void imprimir_recursivo(NodoArbol* raiz, int nivel);

/**
 * @brief Muestra estadísticas completas del árbol
 * @param arbol Puntero al árbol
 */
void mostrar_estadisticas_altura(ArbolBinario* arbol);

/**
 * @brief Imprime todos los nodos de un nivel específico
 * @param raiz Nodo raíz del subárbol
 * @param nivel Nivel a imprimir
 */
void imprimir_nivel(NodoArbol* raiz, int nivel);

/**
 * @brief Imprime el árbol por niveles (BFS)
 * @param arbol Puntero al árbol
 */
void imprimir_por_niveles(ArbolBinario* arbol);

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

/**
 * @brief Demuestra diferentes formas de calcular altura
 * @param arbol Puntero al árbol
 */
void demo_calculo_altura(ArbolBinario* arbol);

/**
 * @brief Compara rendimiento entre versión recursiva e iterativa
 * @param arbol Puntero al árbol
 */
void comparar_algoritmos_altura(ArbolBinario* arbol);

/**
 * @brief Demuestra casos especiales de altura
 */
void demo_casos_especiales(void);

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

/**
 * @brief Libera memoria de un subárbol recursivamente
 * @param raiz Nodo raíz del subárbol a liberar
 */
void liberar_subarbol(NodoArbol* raiz);

/**
 * @brief Busca un nodo con valor específico
 * @param raiz Nodo raíz del subárbol
 * @param valor Valor a buscar
 * @return Puntero al nodo encontrado o NULL
 */
NodoArbol* buscar_nodo(NodoArbol* raiz, int valor);

/**
 * @brief Verifica si el árbol está vacío
 * @param arbol Puntero al árbol
 * @return true si está vacío, false en caso contrario
 */
bool arbol_vacio(ArbolBinario* arbol);

/**
 * @brief Obtiene el tamaño del árbol
 * @param arbol Puntero al árbol
 * @return Número de nodos en el árbol
 */
int arbol_tamanio(ArbolBinario* arbol);

#endif // ALTURA_ARBOL_H
