#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include <stdbool.h>

/**
 * @brief Estructura de un nodo en una lista enlazada simple
 * 
 * Una lista enlazada es una estructura de datos dinámica donde cada elemento
 * (nodo) contiene datos y un puntero al siguiente elemento. Esto permite
 * crear secuencias de longitud variable que pueden crecer y decrecer
 * dinámicamente durante la ejecución del programa.
 * 
 * Características:
 * - Inserción y eliminación O(1) en posiciones conocidas
 * - Acceso secuencial O(n)
 * - Uso eficiente de memoria (solo la necesaria)
 * - Tamaño dinámico
 */
typedef struct Nodo {
    int valor;                  ///< Datos almacenados en el nodo
    struct Nodo* siguiente;     ///< Puntero al siguiente nodo (NULL si es el último)
} Nodo;

/**
 * @brief Estructura que representa una lista enlazada completa
 * 
 * Mantiene punteros al primer y último nodo para optimizar operaciones
 * comunes como inserción al final.
 */
typedef struct {
    Nodo* cabeza;               ///< Primer nodo de la lista
    Nodo* cola;                 ///< Último nodo de la lista (optimización)
    size_t longitud;            ///< Número de elementos en la lista
} Lista;

// === FUNCIONES DE CREACIÓN Y DESTRUCCIÓN ===

/**
 * @brief Crea una nueva lista enlazada vacía
 * @return Puntero a la nueva lista o NULL si hay error de memoria
 */
Lista* crear_lista(void);

/**
 * @brief Crea un nuevo nodo con el valor especificado
 * @param valor Valor a almacenar en el nodo
 * @return Puntero al nuevo nodo o NULL si hay error de memoria
 */
Nodo* crear_nodo(int valor);

/**
 * @brief Libera toda la memoria ocupada por la lista
 * @param lista Puntero a la lista a liberar
 */
void destruir_lista(Lista* lista);

/**
 * @brief Libera la memoria de un nodo individual
 * @param nodo Puntero al nodo a liberar
 */
void destruir_nodo(Nodo* nodo);

// === FUNCIONES DE INSERCIÓN ===

/**
 * @brief Inserta un nuevo nodo al final de la lista
 * @param lista Puntero a la lista
 * @param valor Valor a insertar
 * @return true si la inserción fue exitosa, false en caso de error
 */
bool insertar_final(Lista* lista, int valor);

/**
 * @brief Inserta un nuevo nodo al inicio de la lista
 * @param lista Puntero a la lista
 * @param valor Valor a insertar
 * @return true si la inserción fue exitosa, false en caso de error
 */
bool insertar_inicio(Lista* lista, int valor);

/**
 * @brief Inserta un nuevo nodo en la posición especificada
 * @param lista Puntero a la lista
 * @param posicion Posición donde insertar (0 = inicio)
 * @param valor Valor a insertar
 * @return true si la inserción fue exitosa, false en caso de error
 */
bool insertar_en_posicion(Lista* lista, size_t posicion, int valor);

/**
 * @brief Inserta un valor manteniendo la lista ordenada (ascendente)
 * @param lista Puntero a la lista (debe estar previamente ordenada)
 * @param valor Valor a insertar
 * @return true si la inserción fue exitosa, false en caso de error
 */
bool insertar_ordenado(Lista* lista, int valor);

// === FUNCIONES DE BÚSQUEDA Y ACCESO ===

/**
 * @brief Busca un valor en la lista
 * @param lista Puntero a la lista
 * @param valor Valor a buscar
 * @return Puntero al nodo que contiene el valor, o NULL si no se encuentra
 */
Nodo* buscar_valor(const Lista* lista, int valor);

/**
 * @brief Obtiene el nodo en la posición especificada
 * @param lista Puntero a la lista
 * @param posicion Posición del nodo (0 = primer nodo)
 * @return Puntero al nodo en la posición, o NULL si la posición es inválida
 */
Nodo* obtener_en_posicion(const Lista* lista, size_t posicion);

/**
 * @brief Obtiene la posición de un valor en la lista
 * @param lista Puntero a la lista
 * @param valor Valor a buscar
 * @return Posición del valor (0-indexada) o SIZE_MAX si no se encuentra
 */
size_t obtener_posicion(const Lista* lista, int valor);

/**
 * @brief Verifica si la lista contiene un valor específico
 * @param lista Puntero a la lista
 * @param valor Valor a verificar
 * @return true si el valor está en la lista, false en caso contrario
 */
bool contiene_valor(const Lista* lista, int valor);

// === FUNCIONES DE CONSULTA ===

/**
 * @brief Verifica si la lista está vacía
 * @param lista Puntero a la lista
 * @return true si la lista está vacía, false en caso contrario
 */
bool esta_vacia(const Lista* lista);

/**
 * @brief Obtiene la longitud de la lista
 * @param lista Puntero a la lista
 * @return Número de elementos en la lista
 */
size_t obtener_longitud(const Lista* lista);

/**
 * @brief Obtiene el primer valor de la lista
 * @param lista Puntero a la lista
 * @param valor Puntero donde almacenar el valor
 * @return true si se obtuvo el valor, false si la lista está vacía
 */
bool obtener_primero(const Lista* lista, int* valor);

/**
 * @brief Obtiene el último valor de la lista
 * @param lista Puntero a la lista
 * @param valor Puntero donde almacenar el valor
 * @return true si se obtuvo el valor, false si la lista está vacía
 */
bool obtener_ultimo(const Lista* lista, int* valor);

// === FUNCIONES DE VISUALIZACIÓN ===

/**
 * @brief Imprime todos los valores de la lista
 * @param lista Puntero a la lista
 */
void imprimir_lista(const Lista* lista);

/**
 * @brief Imprime la lista con formato detallado
 * @param lista Puntero a la lista
 * @param titulo Título descriptivo para la impresión
 */
void imprimir_lista_detallada(const Lista* lista, const char* titulo);

/**
 * @brief Imprime información de depuración de la lista
 * @param lista Puntero a la lista
 */
void imprimir_debug_lista(const Lista* lista);

// === FUNCIONES DE UTILIDAD ===

/**
 * @brief Copia una lista completa
 * @param lista Puntero a la lista a copiar
 * @return Puntero a la nueva lista copiada, o NULL si hay error
 */
Lista* copiar_lista(const Lista* lista);

/**
 * @brief Invierte el orden de los elementos en la lista
 * @param lista Puntero a la lista a invertir
 */
void invertir_lista(Lista* lista);

/**
 * @brief Compara dos listas elemento por elemento
 * @param lista1 Puntero a la primera lista
 * @param lista2 Puntero a la segunda lista
 * @return true si las listas son idénticas, false en caso contrario
 */
bool comparar_listas(const Lista* lista1, const Lista* lista2);

// === FUNCIONES PARA VERSIÓN SIMPLE (SIN ESTRUCTURA Lista) ===

/**
 * @brief Versión simple: inserta al final usando solo puntero a cabeza
 * @param cabeza Puntero al puntero de la cabeza de la lista
 * @param valor Valor a insertar
 * @return true si la inserción fue exitosa, false en caso de error
 */
bool insertar_final_simple(Nodo** cabeza, int valor);

/**
 * @brief Versión simple: imprime la lista usando solo puntero a cabeza
 * @param cabeza Puntero a la cabeza de la lista
 */
void imprimir_lista_simple(const Nodo* cabeza);

/**
 * @brief Versión simple: libera toda la lista usando solo puntero a cabeza
 * @param cabeza Puntero al puntero de la cabeza de la lista
 */
void destruir_lista_simple(Nodo** cabeza);

// === FUNCIONES EDUCATIVAS ===

/**
 * @brief Demuestra diferentes formas de crear y manipular listas
 */
void demostrar_operaciones_basicas(void);

/**
 * @brief Compara el rendimiento de diferentes operaciones
 */
void demostrar_rendimiento(void);

/**
 * @brief Muestra el uso de memoria de la lista
 */
void analizar_memoria(void);

#endif // LISTA_ENLAZADA_H
