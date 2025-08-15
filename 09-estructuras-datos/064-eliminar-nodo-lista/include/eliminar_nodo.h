#ifndef ELIMINAR_NODO_H
#define ELIMINAR_NODO_H

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

/**
 * @brief Inserta un nuevo nodo en la posición especificada
 * @param lista Puntero a la lista
 * @param posicion Posición donde insertar (0 = inicio)
 * @param valor Valor a insertar
 * @return true si la inserción fue exitosa, false en caso de error
 */
bool insertar_en_posicion(Lista* lista, size_t posicion, int valor);

// === FUNCIONES DE ELIMINACIÓN (PRINCIPAL) ===

/**
 * @brief Elimina el primer nodo que contenga el valor especificado
 * @param lista Puntero a la lista
 * @param valor Valor del nodo a eliminar
 * @return true si se eliminó un nodo, false si no se encontró el valor
 */
bool eliminar_nodo(Lista* lista, int valor);

/**
 * @brief Elimina el nodo en la posición especificada
 * @param lista Puntero a la lista
 * @param posicion Posición del nodo a eliminar (0 = primer nodo)
 * @return true si se eliminó el nodo, false si la posición es inválida
 */
bool eliminar_en_posicion(Lista* lista, size_t posicion);

/**
 * @brief Elimina el primer nodo de la lista
 * @param lista Puntero a la lista
 * @return true si se eliminó el nodo, false si la lista estaba vacía
 */
bool eliminar_primero(Lista* lista);

/**
 * @brief Elimina el último nodo de la lista
 * @param lista Puntero a la lista
 * @return true si se eliminó el nodo, false si la lista estaba vacía
 */
bool eliminar_ultimo(Lista* lista);

/**
 * @brief Elimina todos los nodos que contengan el valor especificado
 * @param lista Puntero a la lista
 * @param valor Valor de los nodos a eliminar
 * @return Número de nodos eliminados
 */
size_t eliminar_todos(Lista* lista, int valor);

/**
 * @brief Elimina todos los nodos de la lista
 * @param lista Puntero a la lista
 */
void vaciar_lista(Lista* lista);

// === FUNCIONES DE BÚSQUEDA Y CONSULTA ===

/**
 * @brief Busca un valor en la lista
 * @param lista Puntero a la lista
 * @param valor Valor a buscar
 * @return Puntero al primer nodo que contiene el valor, o NULL si no se encuentra
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
 * @brief Verifica si la lista contiene un valor específico
 * @param lista Puntero a la lista
 * @param valor Valor a verificar
 * @return true si el valor está en la lista, false en caso contrario
 */
bool contiene_valor(const Lista* lista, int valor);

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
 * @brief Versión simple: elimina el primer nodo con el valor dado
 * @param cabeza Puntero al puntero de la cabeza de la lista
 * @param valor Valor del nodo a eliminar
 * @return true si se eliminó un nodo, false si no se encontró
 */
bool eliminar_nodo_simple(Nodo** cabeza, int valor);

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

// === FUNCIONES EDUCATIVAS Y DE ANÁLISIS ===

/**
 * @brief Demuestra diferentes tipos de eliminación
 */
void demostrar_eliminaciones(void);

/**
 * @brief Analiza el rendimiento de las operaciones de eliminación
 */
void analizar_rendimiento_eliminacion(void);

/**
 * @brief Demuestra casos especiales de eliminación
 */
void demostrar_casos_especiales(void);

/**
 * @brief Valida la integridad de la estructura de la lista
 * @param lista Puntero a la lista a validar
 * @return true si la lista es válida, false si hay inconsistencias
 */
bool validar_integridad_lista(const Lista* lista);

// === FUNCIONES AVANZADAS ===

/**
 * @brief Elimina nodos que cumplan con un predicado
 * @param lista Puntero a la lista
 * @param predicado Función que retorna true para nodos a eliminar
 * @return Número de nodos eliminados
 */
size_t eliminar_con_predicado(Lista* lista, bool (*predicado)(int valor));

/**
 * @brief Elimina nodos duplicados de la lista
 * @param lista Puntero a la lista
 * @return Número de duplicados eliminados
 */
size_t eliminar_duplicados(Lista* lista);

/**
 * @brief Elimina nodos en un rango de posiciones
 * @param lista Puntero a la lista
 * @param inicio Posición inicial (inclusiva)
 * @param fin Posición final (inclusiva)
 * @return Número de nodos eliminados
 */
size_t eliminar_rango(Lista* lista, size_t inicio, size_t fin);

#endif // ELIMINAR_NODO_H
