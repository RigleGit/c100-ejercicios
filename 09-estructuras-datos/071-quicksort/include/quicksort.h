/*
 * Ejercicio 071: Algoritmo Quicksort
 * 
 * Header con definiciones de funciones para implementar el algoritmo
 * de ordenamiento Quicksort con diferentes variantes y optimizaciones.
 * 
 * Quicksort es un algoritmo divide y vencerás que ordena arrays
 * con complejidad promedio O(n log n) y peor caso O(n²).
 */

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/* ================================
 * ESTRUCTURAS Y TIPOS
 * ================================ */

typedef struct {
    int* array;
    int tamanio;
    int capacidad;
} ArrayDinamico;

typedef struct {
    int comparaciones;
    int intercambios;
    int llamadas_recursivas;
    clock_t tiempo_inicio;
    clock_t tiempo_fin;
} EstadisticasOrdenamiento;

typedef enum {
    PIVOT_PRIMERO,
    PIVOT_ULTIMO,
    PIVOT_MEDIO,
    PIVOT_MEDIANA_DE_TRES,
    PIVOT_ALEATORIO
} TipoPivot;

/* ================================
 * FUNCIONES BÁSICAS
 * ================================ */

/**
 * @brief Implementación básica de Quicksort
 * @param arr Array a ordenar
 * @param bajo Índice inferior del rango
 * @param alto Índice superior del rango
 */
void quicksort_basico(int arr[], int bajo, int alto);

/**
 * @brief Función de partición estándar (último elemento como pivot)
 * @param arr Array a particionar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @return Posición final del pivot
 */
int particion_basica(int arr[], int bajo, int alto);

/**
 * @brief Intercambia dos elementos en un array
 * @param arr Array
 * @param i Primer índice
 * @param j Segundo índice
 */
void intercambiar(int arr[], int i, int j);

/* ================================
 * VARIANTES DE PARTICIÓN
 * ================================ */

/**
 * @brief Partición usando el primer elemento como pivot
 */
int particion_primer_elemento(int arr[], int bajo, int alto);

/**
 * @brief Partición usando el elemento medio como pivot
 */
int particion_elemento_medio(int arr[], int bajo, int alto);

/**
 * @brief Partición usando mediana de tres como pivot
 */
int particion_mediana_tres(int arr[], int bajo, int alto);

/**
 * @brief Partición usando pivot aleatorio
 */
int particion_aleatoria(int arr[], int bajo, int alto);

/**
 * @brief Partición de Hoare (dos punteros)
 */
int particion_hoare(int arr[], int bajo, int alto);

/* ================================
 * QUICKSORT OPTIMIZADO
 * ================================ */

/**
 * @brief Quicksort con diferentes estrategias de pivot
 * @param arr Array a ordenar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @param tipo_pivot Estrategia de selección de pivot
 * @param stats Estructura para recopilar estadísticas (puede ser NULL)
 */
void quicksort_optimizado(int arr[], int bajo, int alto, TipoPivot tipo_pivot, EstadisticasOrdenamiento* stats);

/**
 * @brief Quicksort híbrido (cambia a insertion sort para arrays pequeños)
 * @param arr Array a ordenar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @param umbral_insertion Tamaño mínimo para usar insertion sort
 */
void quicksort_hibrido(int arr[], int bajo, int alto, int umbral_insertion);

/**
 * @brief Insertion sort para arrays pequeños
 */
void insertion_sort(int arr[], int bajo, int alto);

/* ================================
 * QUICKSORT ITERATIVO
 * ================================ */

/**
 * @brief Implementación iterativa de Quicksort usando pila
 * @param arr Array a ordenar
 * @param tamanio Tamaño del array
 */
void quicksort_iterativo(int arr[], int tamanio);

/**
 * @brief Pila simple para la versión iterativa
 */
typedef struct {
    int* datos;
    int cima;
    int capacidad;
} Pila;

Pila* pila_crear(int capacidad);
void pila_destruir(Pila* pila);
bool pila_vacia(Pila* pila);
bool pila_llena(Pila* pila);
void pila_push(Pila* pila, int valor);
int pila_pop(Pila* pila);

/* ================================
 * VARIANTE PARA DUPLICADOS
 * ================================ */

/**
 * @brief Quicksort de tres vías (manejo eficiente de duplicados)
 * @param arr Array a ordenar
 * @param bajo Índice inferior
 * @param alto Índice superior
 */
void quicksort_tres_vias(int arr[], int bajo, int alto);

/**
 * @brief Partición de tres vías (elementos menores, iguales, mayores)
 * @param arr Array a particionar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @param menor Puntero para guardar límite de elementos menores
 * @param mayor Puntero para guardar límite de elementos mayores
 */
void particion_tres_vias(int arr[], int bajo, int alto, int* menor, int* mayor);

/* ================================
 * UTILIDADES Y ANÁLISIS
 * ================================ */

/**
 * @brief Inicializa estructura de estadísticas
 */
void estadisticas_inicializar(EstadisticasOrdenamiento* stats);

/**
 * @brief Imprime estadísticas de ordenamiento
 */
void estadisticas_imprimir(EstadisticasOrdenamiento* stats, int tamanio_array);

/**
 * @brief Verifica si un array está ordenado
 * @param arr Array a verificar
 * @param tamanio Tamaño del array
 * @return true si está ordenado, false en caso contrario
 */
bool esta_ordenado(int arr[], int tamanio);

/**
 * @brief Imprime un array
 * @param arr Array a imprimir
 * @param tamanio Tamaño del array
 * @param titulo Título descriptivo
 */
void imprimir_array(int arr[], int tamanio, const char* titulo);

/**
 * @brief Copia un array
 * @param origen Array origen
 * @param destino Array destino
 * @param tamanio Tamaño de los arrays
 */
void copiar_array(int origen[], int destino[], int tamanio);

/**
 * @brief Genera un array con números aleatorios
 * @param arr Array a llenar
 * @param tamanio Tamaño del array
 * @param min Valor mínimo
 * @param max Valor máximo
 */
void generar_array_aleatorio(int arr[], int tamanio, int min, int max);

/**
 * @brief Genera un array ordenado inversamente
 * @param arr Array a llenar
 * @param tamanio Tamaño del array
 */
void generar_array_inverso(int arr[], int tamanio);

/**
 * @brief Genera un array con muchos duplicados
 * @param arr Array a llenar
 * @param tamanio Tamaño del array
 * @param num_valores_unicos Número de valores únicos
 */
void generar_array_duplicados(int arr[], int tamanio, int num_valores_unicos);

/* ================================
 * COMPARACIÓN DE ALGORITMOS
 * ================================ */

/**
 * @brief Compara diferentes variantes de Quicksort
 * @param arr Array original
 * @param tamanio Tamaño del array
 */
void comparar_variantes_quicksort(int arr[], int tamanio);

/**
 * @brief Mide el tiempo de ejecución de una función de ordenamiento
 * @param funcion_ordenamiento Función a medir
 * @param arr Array a ordenar
 * @param tamanio Tamaño del array
 * @param nombre_algoritmo Nombre del algoritmo para mostrar
 */
void medir_tiempo_ordenamiento(void (*funcion_ordenamiento)(int[], int), 
                              int arr[], int tamanio, const char* nombre_algoritmo);

/* ================================
 * DEMOSTRACIÓN EDUCATIVA
 * ================================ */

/**
 * @brief Demostración paso a paso del algoritmo
 * @param arr Array a ordenar
 * @param tamanio Tamaño del array
 */
void demo_quicksort_paso_a_paso(int arr[], int tamanio);

/**
 * @brief Quicksort con visualización paso a paso
 * @param arr Array a ordenar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @param nivel Nivel de recursión para indentación
 */
void quicksort_visualizado(int arr[], int bajo, int alto, int nivel);

/**
 * @brief Imprime espacios para indentación según el nivel
 * @param nivel Nivel de indentación
 */
void imprimir_indentacion(int nivel);

/* ================================
 * FUNCIÓN PRINCIPAL DE DEMOSTRACIÓN
 * ================================ */

/**
 * @brief Función principal que demuestra todas las características
 */
void demo_completa_quicksort(void);

/* ================================
 * IMPLEMENTACIÓN SIMPLE
 * ================================ */

/**
 * @brief Versión simple de intercambio usando punteros
 * @param a Puntero al primer elemento
 * @param b Puntero al segundo elemento
 */
void intercambiar_simple(int* a, int* b);

/**
 * @brief Versión simple de partición
 * @param arr Array a particionar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @return Posición final del pivot
 */
int particionar_simple(int arr[], int bajo, int alto);

/**
 * @brief Versión simple de Quicksort
 * @param arr Array a ordenar
 * @param bajo Índice inferior
 * @param alto Índice superior
 */
void quicksort_simple(int arr[], int bajo, int alto);

/**
 * @brief Imprime un array de forma simple
 * @param arr Array a imprimir
 * @param n Tamaño del array
 */
void imprimirArray_simple(int arr[], int n);

/**
 * @brief Ejemplo básico de uso de Quicksort
 */
void ejemplo_basico_quicksort(void);

#endif /* QUICKSORT_H */
