/*
 * Ejercicio 070: Lista Doblemente Enlazada
 * 
 * Header con definiciones de estructuras y funciones para una lista
 * doblemente enlazada completa con operaciones bidireccionales.
 * 
 * Una lista doblemente enlazada permite navegación en ambas direcciones
 * y operaciones eficientes en ambos extremos.
 */

#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ================================
 * ESTRUCTURAS DE DATOS
 * ================================ */

/**
 * @brief Estructura de un nodo de la lista doblemente enlazada
 */
typedef struct Nodo {
    int dato;                    // Valor almacenado en el nodo
    struct Nodo* siguiente;      // Puntero al siguiente nodo
    struct Nodo* anterior;       // Puntero al nodo anterior
} Nodo;

/**
 * @brief Estructura de la lista doblemente enlazada con metadatos
 */
typedef struct ListaDoble {
    Nodo* cabeza;               // Primer nodo de la lista
    Nodo* cola;                 // Último nodo de la lista
    size_t tamanio;             // Número de elementos en la lista
} ListaDoble;

/**
 * @brief Estructura para iterador bidireccional
 */
typedef struct IteradorDoble {
    Nodo* actual;               // Nodo actual del iterador
    ListaDoble* lista;          // Lista asociada al iterador
    bool adelante;              // Dirección del iterador (true = adelante)
} IteradorDoble;

/* ================================
 * FUNCIONES DE GESTIÓN DE LISTA
 * ================================ */

/**
 * @brief Crea una nueva lista doblemente enlazada vacía
 * @return Puntero a la lista creada o NULL si hay error
 */
ListaDoble* lista_crear(void);

/**
 * @brief Destruye la lista y libera toda la memoria
 * @param lista Puntero a la lista a destruir
 */
void lista_destruir(ListaDoble* lista);

/**
 * @brief Crea un nuevo nodo con el valor dado
 * @param dato Valor a almacenar en el nodo
 * @return Puntero al nodo creado o NULL si hay error
 */
Nodo* nodo_crear(int dato);

/**
 * @brief Libera un nodo individual
 * @param nodo Puntero al nodo a liberar
 */
void nodo_destruir(Nodo* nodo);

/* ================================
 * FUNCIONES DE INSERCIÓN
 * ================================ */

/**
 * @brief Inserta un elemento al inicio de la lista
 * @param lista Puntero a la lista
 * @param dato Valor a insertar
 * @return true si se insertó correctamente, false en caso contrario
 */
bool lista_insertar_inicio(ListaDoble* lista, int dato);

/**
 * @brief Inserta un elemento al final de la lista
 * @param lista Puntero a la lista
 * @param dato Valor a insertar
 * @return true si se insertó correctamente, false en caso contrario
 */
bool lista_insertar_final(ListaDoble* lista, int dato);

/**
 * @brief Inserta un elemento en una posición específica
 * @param lista Puntero a la lista
 * @param posicion Posición donde insertar (0-indexada)
 * @param dato Valor a insertar
 * @return true si se insertó correctamente, false en caso contrario
 */
bool lista_insertar_posicion(ListaDoble* lista, size_t posicion, int dato);

/**
 * @brief Inserta un elemento antes de un nodo específico
 * @param lista Puntero a la lista
 * @param nodo_referencia Nodo antes del cual insertar
 * @param dato Valor a insertar
 * @return true si se insertó correctamente, false en caso contrario
 */
bool lista_insertar_antes(ListaDoble* lista, Nodo* nodo_referencia, int dato);

/**
 * @brief Inserta un elemento después de un nodo específico
 * @param lista Puntero a la lista
 * @param nodo_referencia Nodo después del cual insertar
 * @param dato Valor a insertar
 * @return true si se insertó correctamente, false en caso contrario
 */
bool lista_insertar_despues(ListaDoble* lista, Nodo* nodo_referencia, int dato);

/**
 * @brief Inserta múltiples elementos desde un array
 * @param lista Puntero a la lista
 * @param datos Array de valores a insertar
 * @param cantidad Número de elementos a insertar
 * @param al_final true para insertar al final, false al inicio
 * @return Número de elementos insertados correctamente
 */
size_t lista_insertar_array(ListaDoble* lista, int* datos, size_t cantidad, bool al_final);

/* ================================
 * FUNCIONES DE ELIMINACIÓN
 * ================================ */

/**
 * @brief Elimina el primer elemento de la lista
 * @param lista Puntero a la lista
 * @return true si se eliminó un elemento, false si la lista estaba vacía
 */
bool lista_eliminar_inicio(ListaDoble* lista);

/**
 * @brief Elimina el último elemento de la lista
 * @param lista Puntero a la lista
 * @return true si se eliminó un elemento, false si la lista estaba vacía
 */
bool lista_eliminar_final(ListaDoble* lista);

/**
 * @brief Elimina un elemento en una posición específica
 * @param lista Puntero a la lista
 * @param posicion Posición del elemento a eliminar (0-indexada)
 * @return true si se eliminó correctamente, false en caso contrario
 */
bool lista_eliminar_posicion(ListaDoble* lista, size_t posicion);

/**
 * @brief Elimina la primera ocurrencia de un valor
 * @param lista Puntero a la lista
 * @param dato Valor a eliminar
 * @return true si se encontró y eliminó, false en caso contrario
 */
bool lista_eliminar_valor(ListaDoble* lista, int dato);

/**
 * @brief Elimina todas las ocurrencias de un valor
 * @param lista Puntero a la lista
 * @param dato Valor a eliminar
 * @return Número de elementos eliminados
 */
size_t lista_eliminar_todas_ocurrencias(ListaDoble* lista, int dato);

/**
 * @brief Elimina un nodo específico
 * @param lista Puntero a la lista
 * @param nodo Nodo a eliminar
 * @return true si se eliminó correctamente, false en caso contrario
 */
bool lista_eliminar_nodo(ListaDoble* lista, Nodo* nodo);

/**
 * @brief Vacía completamente la lista
 * @param lista Puntero a la lista
 */
void lista_limpiar(ListaDoble* lista);

/* ================================
 * FUNCIONES DE BÚSQUEDA Y ACCESO
 * ================================ */

/**
 * @brief Busca un elemento en la lista
 * @param lista Puntero a la lista
 * @param dato Valor a buscar
 * @return Puntero al nodo encontrado o NULL si no existe
 */
Nodo* lista_buscar(ListaDoble* lista, int dato);

/**
 * @brief Busca un elemento desde el final de la lista
 * @param lista Puntero a la lista
 * @param dato Valor a buscar
 * @return Puntero al nodo encontrado o NULL si no existe
 */
Nodo* lista_buscar_desde_final(ListaDoble* lista, int dato);

/**
 * @brief Obtiene el elemento en una posición específica
 * @param lista Puntero a la lista
 * @param posicion Posición del elemento (0-indexada)
 * @return Puntero al nodo o NULL si la posición es inválida
 */
Nodo* lista_obtener_posicion(ListaDoble* lista, size_t posicion);

/**
 * @brief Obtiene el valor en una posición específica
 * @param lista Puntero a la lista
 * @param posicion Posición del elemento (0-indexada)
 * @param valor Puntero donde almacenar el valor encontrado
 * @return true si se obtuvo el valor, false en caso contrario
 */
bool lista_obtener_valor(ListaDoble* lista, size_t posicion, int* valor);

/**
 * @brief Encuentra la posición de un elemento
 * @param lista Puntero a la lista
 * @param dato Valor a buscar
 * @return Posición del elemento o SIZE_MAX si no se encuentra
 */
size_t lista_encontrar_posicion(ListaDoble* lista, int dato);

/* ================================
 * FUNCIONES DE RECORRIDO
 * ================================ */

/**
 * @brief Recorre la lista hacia adelante imprimiendo los elementos
 * @param lista Puntero a la lista
 */
void lista_imprimir_adelante(ListaDoble* lista);

/**
 * @brief Recorre la lista hacia atrás imprimiendo los elementos
 * @param lista Puntero a la lista
 */
void lista_imprimir_atras(ListaDoble* lista);

/**
 * @brief Recorre la lista hacia adelante con función personalizada
 * @param lista Puntero a la lista
 * @param funcion Función a aplicar a cada elemento
 * @param contexto Contexto adicional para la función
 */
void lista_recorrer_adelante(ListaDoble* lista, void (*funcion)(int dato, void* contexto), void* contexto);

/**
 * @brief Recorre la lista hacia atrás con función personalizada
 * @param lista Puntero a la lista
 * @param funcion Función a aplicar a cada elemento
 * @param contexto Contexto adicional para la función
 */
void lista_recorrer_atras(ListaDoble* lista, void (*funcion)(int dato, void* contexto), void* contexto);

/* ================================
 * FUNCIONES DE ITERADOR
 * ================================ */

/**
 * @brief Crea un iterador para la lista
 * @param lista Puntero a la lista
 * @param adelante true para iterar hacia adelante, false hacia atrás
 * @return Iterador creado
 */
IteradorDoble iterador_crear(ListaDoble* lista, bool adelante);

/**
 * @brief Verifica si el iterador tiene más elementos
 * @param iter Puntero al iterador
 * @return true si hay más elementos, false en caso contrario
 */
bool iterador_tiene_siguiente(IteradorDoble* iter);

/**
 * @brief Obtiene el siguiente elemento del iterador
 * @param iter Puntero al iterador
 * @return Valor del siguiente elemento o 0 si no hay más
 */
int iterador_siguiente(IteradorDoble* iter);

/**
 * @brief Reinicia el iterador al inicio
 * @param iter Puntero al iterador
 */
void iterador_reiniciar(IteradorDoble* iter);

/**
 * @brief Cambia la dirección del iterador
 * @param iter Puntero al iterador
 */
void iterador_cambiar_direccion(IteradorDoble* iter);

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

/**
 * @brief Verifica si la lista está vacía
 * @param lista Puntero a la lista
 * @return true si está vacía, false en caso contrario
 */
bool lista_vacia(ListaDoble* lista);

/**
 * @brief Obtiene el tamaño de la lista
 * @param lista Puntero a la lista
 * @return Número de elementos en la lista
 */
size_t lista_tamanio(ListaDoble* lista);

/**
 * @brief Obtiene el primer elemento sin eliminarlo
 * @param lista Puntero a la lista
 * @param valor Puntero donde almacenar el valor
 * @return true si se obtuvo el valor, false si la lista está vacía
 */
bool lista_primero(ListaDoble* lista, int* valor);

/**
 * @brief Obtiene el último elemento sin eliminarlo
 * @param lista Puntero a la lista
 * @param valor Puntero donde almacenar el valor
 * @return true si se obtuvo el valor, false si la lista está vacía
 */
bool lista_ultimo(ListaDoble* lista, int* valor);

/* ================================
 * FUNCIONES DE VALIDACIÓN
 * ================================ */

/**
 * @brief Verifica la integridad de los enlaces de la lista
 * @param lista Puntero a la lista
 * @return true si la lista es válida, false en caso contrario
 */
bool lista_validar_integridad(ListaDoble* lista);

/**
 * @brief Verifica que un nodo pertenece a la lista
 * @param lista Puntero a la lista
 * @param nodo Nodo a verificar
 * @return true si el nodo pertenece a la lista, false en caso contrario
 */
bool lista_contiene_nodo(ListaDoble* lista, Nodo* nodo);

/**
 * @brief Cuenta los enlaces hacia adelante y verifica consistencia
 * @param lista Puntero a la lista
 * @return Número de nodos contados hacia adelante
 */
size_t lista_contar_adelante(ListaDoble* lista);

/**
 * @brief Cuenta los enlaces hacia atrás y verifica consistencia
 * @param lista Puntero a la lista
 * @return Número de nodos contados hacia atrás
 */
size_t lista_contar_atras(ListaDoble* lista);

/* ================================
 * FUNCIONES AVANZADAS
 * ================================ */

/**
 * @brief Invierte completamente la lista
 * @param lista Puntero a la lista
 */
void lista_invertir(ListaDoble* lista);

/**
 * @brief Ordena la lista usando un algoritmo estable
 * @param lista Puntero a la lista
 * @param ascendente true para orden ascendente, false para descendente
 */
void lista_ordenar(ListaDoble* lista, bool ascendente);

/**
 * @brief Combina dos listas ordenadas en una sola lista ordenada
 * @param lista1 Primera lista (será modificada)
 * @param lista2 Segunda lista (será vaciada)
 * @return true si se combinaron correctamente
 */
bool lista_merge_ordenadas(ListaDoble* lista1, ListaDoble* lista2);

/**
 * @brief Divide la lista en dos partes en una posición específica
 * @param lista Lista original
 * @param posicion Posición donde dividir
 * @return Nueva lista con la segunda parte o NULL si hay error
 */
ListaDoble* lista_dividir(ListaDoble* lista, size_t posicion);

/**
 * @brief Crea una copia de la lista
 * @param lista Lista a copiar
 * @return Nueva lista copiada o NULL si hay error
 */
ListaDoble* lista_clonar(ListaDoble* lista);

/**
 * @brief Elimina elementos duplicados de la lista
 * @param lista Puntero a la lista
 * @return Número de elementos eliminados
 */
size_t lista_eliminar_duplicados(ListaDoble* lista);

/* ================================
 * FUNCIONES DE VISUALIZACIÓN
 * ================================ */

/**
 * @brief Muestra la estructura interna de la lista con punteros
 * @param lista Puntero a la lista
 */
void lista_mostrar_estructura(ListaDoble* lista);

/**
 * @brief Muestra estadísticas de la lista
 * @param lista Puntero a la lista
 */
void lista_mostrar_estadisticas(ListaDoble* lista);

/**
 * @brief Imprime la lista en formato JSON
 * @param lista Puntero a la lista
 */
void lista_imprimir_json(ListaDoble* lista);

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

/**
 * @brief Demuestra operaciones básicas de la lista
 * @param lista Puntero a la lista
 */
void demo_operaciones_basicas(ListaDoble* lista);

/**
 * @brief Demuestra recorridos bidireccionales
 * @param lista Puntero a la lista
 */
void demo_recorridos_bidireccionales(ListaDoble* lista);

/**
 * @brief Demuestra el uso de iteradores
 * @param lista Puntero a la lista
 */
void demo_iteradores(ListaDoble* lista);

/**
 * @brief Demuestra operaciones avanzadas
 * @param lista Puntero a la lista
 */
void demo_operaciones_avanzadas(ListaDoble* lista);

#endif // LISTA_DOBLE_H
