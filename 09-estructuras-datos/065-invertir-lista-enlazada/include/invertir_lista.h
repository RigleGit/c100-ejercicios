#ifndef INVERTIR_LISTA_H
#define INVERTIR_LISTA_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Estructura de un nodo en una lista enlazada simple
 * 
 * Esta estructura representa un nodo individual en una lista enlazada,
 * conteniendo un valor entero y un puntero al siguiente nodo.
 */
typedef struct Nodo {
    int valor;                  ///< Valor almacenado en el nodo
    struct Nodo* siguiente;     ///< Puntero al siguiente nodo
} Nodo;

/**
 * @brief Estructura que representa una lista enlazada completa
 * 
 * Incluye metadatos adicionales para optimizar operaciones comunes
 * y mantener la integridad de la estructura.
 */
typedef struct {
    Nodo* cabeza;               ///< Primer nodo de la lista
    Nodo* cola;                 ///< Último nodo de la lista
    size_t longitud;            ///< Número de elementos en la lista
} Lista;

// === FUNCIONES DE CREACIÓN Y GESTIÓN ===

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

// === FUNCIONES DE INVERSIÓN (PRINCIPAL) ===

/**
 * @brief Invierte el orden de los nodos en la lista
 * @param lista Puntero a la lista a invertir
 * @return true si la inversión fue exitosa, false en caso de error
 */
bool invertir_lista(Lista* lista);

/**
 * @brief Invierte una sublista entre dos posiciones
 * @param lista Puntero a la lista
 * @param inicio Posición inicial (inclusiva, 0-indexada)
 * @param fin Posición final (inclusiva, 0-indexada)
 * @return true si la inversión fue exitosa, false en caso de error
 */
bool invertir_sublista(Lista* lista, size_t inicio, size_t fin);

/**
 * @brief Invierte la lista recursivamente
 * @param lista Puntero a la lista a invertir
 * @return true si la inversión fue exitosa, false en caso de error
 */
bool invertir_lista_recursivo(Lista* lista);

/**
 * @brief Invierte grupos de k nodos consecutivos
 * @param lista Puntero a la lista
 * @param k Tamaño de cada grupo a invertir
 * @return true si la inversión fue exitosa, false en caso de error
 */
bool invertir_por_grupos(Lista* lista, size_t k);

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

// === FUNCIONES PARA VERSIÓN SIMPLE (COMPATIBILIDAD) ===

/**
 * @brief Versión simple: invierte la lista usando solo puntero a cabeza
 * @param cabeza Puntero al puntero de la cabeza de la lista
 * @return true si la inversión fue exitosa, false en caso de error
 */
bool invertir_lista_simple(Nodo** cabeza);

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

// === FUNCIONES DE CONSULTA Y VALIDACIÓN ===

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
 * @brief Obtiene el nodo en la posición especificada
 * @param lista Puntero a la lista
 * @param posicion Posición del nodo (0 = primer nodo)
 * @return Puntero al nodo en la posición, o NULL si la posición es inválida
 */
Nodo* obtener_en_posicion(const Lista* lista, size_t posicion);

/**
 * @brief Valida la integridad de la estructura de la lista
 * @param lista Puntero a la lista a validar
 * @return true si la lista es válida, false si hay inconsistencias
 */
bool validar_integridad_lista(const Lista* lista);

/**
 * @brief Compara dos listas para verificar si son iguales
 * @param lista1 Primera lista a comparar
 * @param lista2 Segunda lista a comparar
 * @return true si las listas son iguales, false en caso contrario
 */
bool listas_iguales(const Lista* lista1, const Lista* lista2);

// === FUNCIONES EDUCATIVAS Y DE ANÁLISIS ===

/**
 * @brief Demuestra diferentes métodos de inversión
 */
void demostrar_metodos_inversion(void);

/**
 * @brief Analiza el rendimiento de los diferentes algoritmos de inversión
 */
void analizar_rendimiento_inversion(void);

/**
 * @brief Demuestra casos especiales de inversión
 */
void demostrar_casos_especiales_inversion(void);

/**
 * @brief Visualiza paso a paso el proceso de inversión
 * @param lista Lista a invertir (se hace una copia)
 */
void visualizar_proceso_inversion(const Lista* lista);

// === FUNCIONES AUXILIARES PARA RECURSIÓN ===

/**
 * @brief Función auxiliar recursiva para invertir lista
 * @param nodo_actual Nodo actual en la recursión
 * @return Puntero al nuevo primer nodo (antigua cola)
 */
Nodo* invertir_recursivo_auxiliar(Nodo* nodo_actual);

/**
 * @brief Función auxiliar para invertir grupos de k nodos
 * @param cabeza Puntero al primer nodo del grupo
 * @param k Tamaño del grupo
 * @return Puntero al nuevo primer nodo del grupo invertido
 */
Nodo* invertir_grupo_k_auxiliar(Nodo* cabeza, size_t k);

// === FUNCIONES AVANZADAS DE INVERSIÓN ===

/**
 * @brief Invierte la lista manteniendo ciertos valores en su posición original
 * @param lista Puntero a la lista
 * @param mantener_valor Valor que no debe moverse de su posición
 * @return true si la operación fue exitosa, false en caso de error
 */
bool invertir_excepto_valor(Lista* lista, int mantener_valor);

/**
 * @brief Invierte alternativamente grupos de nodos
 * @param lista Puntero a la lista
 * @param k Tamaño de cada grupo
 * @return true si la operación fue exitosa, false en caso de error
 */
bool invertir_alternativo(Lista* lista, size_t k);

/**
 * @brief Crea una copia invertida de la lista sin modificar la original
 * @param lista_original Lista original
 * @return Nueva lista que es la inversión de la original, o NULL si hay error
 */
Lista* crear_copia_invertida(const Lista* lista_original);

#endif // INVERTIR_LISTA_H
