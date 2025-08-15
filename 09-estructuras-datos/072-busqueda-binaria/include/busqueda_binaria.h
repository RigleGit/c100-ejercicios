/*
 * Ejercicio 072: Algoritmo de Búsqueda Binaria
 * 
 * Header con definiciones de funciones para implementar búsqueda binaria
 * y sus variantes en arrays ordenados.
 * 
 * La búsqueda binaria es un algoritmo eficiente que busca un elemento
 * en un array ordenado con complejidad O(log n).
 */

#ifndef BUSQUEDA_BINARIA_H
#define BUSQUEDA_BINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ================================
 * ESTRUCTURAS Y TIPOS
 * ================================ */

typedef struct {
    int posicion;           // Posición del elemento encontrado (-1 si no existe)
    int comparaciones;      // Número de comparaciones realizadas
    bool encontrado;        // Si el elemento fue encontrado
} ResultadoBusqueda;

typedef struct {
    int primera_posicion;   // Primera ocurrencia del elemento
    int ultima_posicion;    // Última ocurrencia del elemento
    int total_ocurrencias;  // Número total de ocurrencias
    int comparaciones;      // Comparaciones realizadas
} ResultadoBusquedaMultiple;

typedef struct {
    int* array;
    int tamanio;
    int capacidad;
} ArrayDinamico;

/* ================================
 * BÚSQUEDA BINARIA BÁSICA
 * ================================ */

/**
 * @brief Búsqueda binaria básica (recursiva)
 * @param arr Array ordenado a buscar
 * @param izquierda Índice izquierdo del rango
 * @param derecha Índice derecho del rango
 * @param objetivo Elemento a buscar
 * @return Posición del elemento o -1 si no se encuentra
 */
int busqueda_binaria_recursiva(int arr[], int izquierda, int derecha, int objetivo);

/**
 * @brief Búsqueda binaria iterativa
 * @param arr Array ordenado a buscar
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Posición del elemento o -1 si no se encuentra
 */
int busqueda_binaria_iterativa(int arr[], int tamanio, int objetivo);

/**
 * @brief Búsqueda binaria con información detallada
 * @param arr Array ordenado a buscar
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Estructura con información detallada del resultado
 */
ResultadoBusqueda busqueda_binaria_detallada(int arr[], int tamanio, int objetivo);

/* ================================
 * VARIANTES DE BÚSQUEDA BINARIA
 * ================================ */

/**
 * @brief Encuentra la primera ocurrencia de un elemento
 * @param arr Array ordenado (puede contener duplicados)
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Posición de la primera ocurrencia o -1 si no existe
 */
int busqueda_primera_ocurrencia(int arr[], int tamanio, int objetivo);

/**
 * @brief Encuentra la última ocurrencia de un elemento
 * @param arr Array ordenado (puede contener duplicados)
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Posición de la última ocurrencia o -1 si no existe
 */
int busqueda_ultima_ocurrencia(int arr[], int tamanio, int objetivo);

/**
 * @brief Cuenta todas las ocurrencias de un elemento
 * @param arr Array ordenado (puede contener duplicados)
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Información completa sobre las ocurrencias
 */
ResultadoBusquedaMultiple busqueda_todas_ocurrencias(int arr[], int tamanio, int objetivo);

/**
 * @brief Encuentra el elemento más pequeño mayor que el objetivo
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Valor de referencia
 * @return Posición del ceiling o -1 si no existe
 */
int busqueda_ceiling(int arr[], int tamanio, int objetivo);

/**
 * @brief Encuentra el elemento más grande menor que el objetivo
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Valor de referencia
 * @return Posición del floor o -1 si no existe
 */
int busqueda_floor(int arr[], int tamanio, int objetivo);

/* ================================
 * BÚSQUEDA EN ARRAYS ESPECIALES
 * ================================ */

/**
 * @brief Búsqueda en array rotado ordenado
 * @param arr Array rotado ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Posición del elemento o -1 si no se encuentra
 */
int busqueda_array_rotado(int arr[], int tamanio, int objetivo);

/**
 * @brief Encuentra el punto de rotación en un array rotado
 * @param arr Array rotado ordenado
 * @param tamanio Tamaño del array
 * @return Índice del elemento mínimo (punto de rotación)
 */
int encontrar_punto_rotacion(int arr[], int tamanio);

/**
 * @brief Búsqueda en array con tamaño desconocido
 * @param arr Array ordenado de tamaño desconocido
 * @param objetivo Elemento a buscar
 * @return Posición del elemento o -1 si no se encuentra
 */
int busqueda_tamanio_desconocido(int arr[], int objetivo);

/**
 * @brief Búsqueda en matriz ordenada (filas y columnas ordenadas)
 * @param matriz Matriz ordenada
 * @param filas Número de filas
 * @param columnas Número de columnas
 * @param objetivo Elemento a buscar
 * @param fila_encontrada Puntero para guardar fila encontrada
 * @param col_encontrada Puntero para guardar columna encontrada
 * @return true si se encuentra, false en caso contrario
 */
bool busqueda_matriz_ordenada(int** matriz, int filas, int columnas, int objetivo, 
                             int* fila_encontrada, int* col_encontrada);

/* ================================
 * BÚSQUEDA DE ELEMENTOS ESPECIALES
 * ================================ */

/**
 * @brief Encuentra el pico en un array (elemento mayor que sus vecinos)
 * @param arr Array de números
 * @param tamanio Tamaño del array
 * @return Posición de un pico
 */
int encontrar_pico(int arr[], int tamanio);

/**
 * @brief Encuentra el elemento que aparece una sola vez en un array donde
 *        todos los demás aparecen exactamente dos veces
 * @param arr Array ordenado con duplicados pares excepto uno
 * @param tamanio Tamaño del array
 * @return El elemento único
 */
int encontrar_elemento_unico(int arr[], int tamanio);

/**
 * @brief Encuentra la raíz cuadrada entera usando búsqueda binaria
 * @param numero Número del cual encontrar la raíz cuadrada
 * @return Raíz cuadrada entera (floor(sqrt(numero)))
 */
int raiz_cuadrada_binaria(int numero);

/**
 * @brief Encuentra la k-ésima raíz entera usando búsqueda binaria
 * @param numero Número base
 * @param k Índice de la raíz
 * @return k-ésima raíz entera
 */
int raiz_k_binaria(int numero, int k);

/* ================================
 * UTILIDADES Y ANÁLISIS
 * ================================ */

/**
 * @brief Verifica si un array está ordenado
 * @param arr Array a verificar
 * @param tamanio Tamaño del array
 * @return true si está ordenado, false en caso contrario
 */
bool esta_ordenado_busqueda(int arr[], int tamanio);

/**
 * @brief Genera un array ordenado para pruebas
 * @param arr Array a llenar
 * @param tamanio Tamaño del array
 * @param inicio Valor inicial
 * @param incremento Incremento entre elementos
 */
void generar_array_ordenado(int arr[], int tamanio, int inicio, int incremento);

/**
 * @brief Genera un array ordenado con duplicados
 * @param arr Array a llenar
 * @param tamanio Tamaño del array
 * @param num_valores_unicos Número de valores únicos
 */
void generar_array_con_duplicados(int arr[], int tamanio, int num_valores_unicos);

/**
 * @brief Rota un array ordenado
 * @param arr Array ordenado a rotar
 * @param tamanio Tamaño del array
 * @param posicion_rotacion Posición desde donde rotar
 */
void rotar_array(int arr[], int tamanio, int posicion_rotacion);

/**
 * @brief Imprime el resultado de una búsqueda
 * @param resultado Resultado de la búsqueda
 * @param objetivo Elemento buscado
 */
void imprimir_resultado_busqueda(ResultadoBusqueda resultado, int objetivo);

/**
 * @brief Imprime el resultado de una búsqueda múltiple
 * @param resultado Resultado de la búsqueda múltiple
 * @param objetivo Elemento buscado
 */
void imprimir_resultado_busqueda_multiple(ResultadoBusquedaMultiple resultado, int objetivo);

/**
 * @brief Imprime un array con formato
 * @param arr Array a imprimir
 * @param tamanio Tamaño del array
 * @param titulo Título descriptivo
 */
void imprimir_array_busqueda(int arr[], int tamanio, const char* titulo);

/* ================================
 * COMPARACIÓN Y ANÁLISIS
 * ================================ */

/**
 * @brief Compara búsqueda binaria vs búsqueda lineal
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 */
void comparar_busqueda_binaria_vs_lineal(int arr[], int tamanio, int objetivo);

/**
 * @brief Búsqueda lineal para comparación
 * @param arr Array a buscar
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 * @return Estructura con resultado detallado
 */
ResultadoBusqueda busqueda_lineal_detallada(int arr[], int tamanio, int objetivo);

/**
 * @brief Mide el rendimiento de diferentes algoritmos de búsqueda
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 */
void medir_rendimiento_busquedas(int arr[], int tamanio, int objetivo);

/* ================================
 * DEMOSTRACIÓN EDUCATIVA
 * ================================ */

/**
 * @brief Demostración paso a paso de búsqueda binaria
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Elemento a buscar
 */
void demo_busqueda_binaria_paso_a_paso(int arr[], int tamanio, int objetivo);

/**
 * @brief Búsqueda binaria con visualización paso a paso
 * @param arr Array ordenado
 * @param izquierda Índice izquierdo
 * @param derecha Índice derecho
 * @param objetivo Elemento a buscar
 * @param paso Número de paso actual
 * @return Posición del elemento o -1 si no se encuentra
 */
int busqueda_binaria_visualizada(int arr[], int izquierda, int derecha, int objetivo, int* paso);

/**
 * @brief Muestra el rango actual de búsqueda
 * @param arr Array
 * @param izquierda Índice izquierdo
 * @param derecha Índice derecho
 * @param medio Índice medio
 * @param objetivo Elemento buscado
 * @param paso Número de paso
 */
void mostrar_paso_busqueda(int arr[], int izquierda, int derecha, int medio, int objetivo, int paso);

/* ================================
 * APLICACIONES PRÁCTICAS
 * ================================ */

/**
 * @brief Simula búsqueda en un diccionario ordenado
 * @param palabras Array de palabras ordenadas
 * @param tamanio Número de palabras
 * @param palabra_buscar Palabra a buscar
 * @return Posición de la palabra o -1 si no existe
 */
int buscar_en_diccionario(char** palabras, int tamanio, const char* palabra_buscar);

/**
 * @brief Encuentra el valor más cercano en un array ordenado
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param objetivo Valor objetivo
 * @return Posición del valor más cercano
 */
int encontrar_valor_mas_cercano(int arr[], int tamanio, int objetivo);

/**
 * @brief Búsqueda en rango: encuentra todos los elementos entre min y max
 * @param arr Array ordenado
 * @param tamanio Tamaño del array
 * @param min Valor mínimo del rango
 * @param max Valor máximo del rango
 * @param inicio_rango Puntero para guardar inicio del rango
 * @param fin_rango Puntero para guardar fin del rango
 * @return Número de elementos en el rango
 */
int busqueda_en_rango(int arr[], int tamanio, int min, int max, int* inicio_rango, int* fin_rango);

/* ================================
 * FUNCIONES SIMPLES (VERSIÓN BÁSICA)
 * ================================ */

/**
 * @brief Búsqueda binaria simple iterativa (versión del ejercicio básico)
 * @param arr Array ordenado a buscar
 * @param n Tamaño del array
 * @param clave Elemento a buscar
 * @return Índice del elemento o -1 si no se encuentra
 */
int busquedaBinaria(int arr[], int n, int clave);

/**
 * @brief Búsqueda binaria simple recursiva
 * @param arr Array ordenado a buscar
 * @param inicio Índice inicial
 * @param fin Índice final
 * @param clave Elemento a buscar
 * @return Índice del elemento o -1 si no se encuentra
 */
int busquedaBinariaRecursiva_simple(int arr[], int inicio, int fin, int clave);

/**
 * @brief Función simple para imprimir array
 * @param arr Array a imprimir
 * @param n Tamaño del array
 */
void imprimirArray_simple(int arr[], int n);

/**
 * @brief Ejemplo básico del ejercicio 072
 */
void ejemplo_basico_072(void);

/**
 * @brief Función principal que demuestra todas las características
 */
void demo_completa_busqueda_binaria(void);

#endif /* BUSQUEDA_BINARIA_H */
