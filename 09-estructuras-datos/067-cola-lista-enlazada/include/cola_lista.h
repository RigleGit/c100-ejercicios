#ifndef COLA_LISTA_H
#define COLA_LISTA_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Estructura de un nodo en la cola
 * 
 * Esta estructura representa un nodo individual en la cola,
 * conteniendo un valor entero y un puntero al siguiente nodo.
 */
typedef struct Nodo {
    int valor;                  ///< Valor almacenado en el nodo
    struct Nodo* siguiente;     ///< Puntero al siguiente nodo
} Nodo;

/**
 * @brief Estructura que representa una cola completa
 * 
 * Incluye punteros al frente y final para operaciones eficientes
 * y metadatos adicionales para optimizar operaciones comunes.
 */
typedef struct {
    Nodo* frente;               ///< Nodo al frente de la cola (primero en salir)
    Nodo* final;                ///< Nodo al final de la cola (último en entrar)
    size_t tamaño;              ///< Número de elementos en la cola
    size_t capacidad_maxima;    ///< Capacidad máxima (0 = ilimitada)
} Cola;

/**
 * @brief Códigos de resultado para operaciones de la cola
 */
typedef enum {
    COLA_EXITO = 0,             ///< Operación exitosa
    COLA_ERROR_MEMORIA = -1,    ///< Error de asignación de memoria
    COLA_VACIA = -2,            ///< Cola vacía cuando se esperaba contenido
    COLA_LLENA = -3,            ///< Cola llena (si tiene límite de capacidad)
    COLA_ERROR_PARAMETRO = -4   ///< Parámetros inválidos
} ResultadoCola;

// === FUNCIONES DE CREACIÓN Y GESTIÓN ===

/**
 * @brief Crea una nueva cola vacía
 * @param capacidad_maxima Capacidad máxima (0 para ilimitada)
 * @return Puntero a la nueva cola o NULL si hay error de memoria
 */
Cola* crear_cola(size_t capacidad_maxima);

/**
 * @brief Crea un nuevo nodo con el valor especificado
 * @param valor Valor a almacenar en el nodo
 * @return Puntero al nuevo nodo o NULL si hay error de memoria
 */
Nodo* crear_nodo(int valor);

/**
 * @brief Libera toda la memoria ocupada por la cola
 * @param cola Puntero a la cola a liberar
 */
void destruir_cola(Cola* cola);

/**
 * @brief Libera la memoria de un nodo individual
 * @param nodo Puntero al nodo a liberar
 */
void destruir_nodo(Nodo* nodo);

// === OPERACIONES PRINCIPALES DE LA COLA (FIFO) ===

/**
 * @brief Inserta un elemento al final de la cola (enqueue)
 * @param cola Puntero a la cola
 * @param valor Valor a insertar
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola enqueue(Cola* cola, int valor);

/**
 * @brief Retira y retorna el elemento del frente de la cola (dequeue)
 * @param cola Puntero a la cola
 * @param valor Puntero donde almacenar el valor retirado
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola dequeue(Cola* cola, int* valor);

/**
 * @brief Consulta el elemento del frente sin retirarlo (front/peek)
 * @param cola Puntero a la cola
 * @param valor Puntero donde almacenar el valor consultado
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola front(const Cola* cola, int* valor);

/**
 * @brief Consulta el elemento del final sin retirarlo (back)
 * @param cola Puntero a la cola
 * @param valor Puntero donde almacenar el valor consultado
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola back(const Cola* cola, int* valor);

// === OPERACIONES DE CONSULTA ===

/**
 * @brief Verifica si la cola está vacía
 * @param cola Puntero a la cola
 * @return true si la cola está vacía, false en caso contrario
 */
bool esta_vacia(const Cola* cola);

/**
 * @brief Verifica si la cola está llena (solo si tiene capacidad máxima)
 * @param cola Puntero a la cola
 * @return true si la cola está llena, false en caso contrario
 */
bool esta_llena(const Cola* cola);

/**
 * @brief Obtiene el tamaño actual de la cola
 * @param cola Puntero a la cola
 * @return Número de elementos en la cola
 */
size_t obtener_tamaño(const Cola* cola);

/**
 * @brief Obtiene la capacidad máxima de la cola
 * @param cola Puntero a la cola
 * @return Capacidad máxima (0 si es ilimitada)
 */
size_t obtener_capacidad(const Cola* cola);

/**
 * @brief Verifica si la cola contiene un valor específico
 * @param cola Puntero a la cola
 * @param valor Valor a buscar
 * @return true si el valor está en la cola, false en caso contrario
 */
bool contiene_valor(const Cola* cola, int valor);

/**
 * @brief Obtiene la posición de un valor en la cola (0 = frente)
 * @param cola Puntero a la cola
 * @param valor Valor a buscar
 * @return Posición del valor o -1 si no se encuentra
 */
int obtener_posicion(const Cola* cola, int valor);

// === OPERACIONES AVANZADAS ===

/**
 * @brief Vacía completamente la cola
 * @param cola Puntero a la cola
 */
void vaciar_cola(Cola* cola);

/**
 * @brief Crea una copia de la cola
 * @param cola_original Cola a copiar
 * @return Nueva cola que es copia de la original, o NULL si hay error
 */
Cola* clonar_cola(const Cola* cola_original);

/**
 * @brief Invierte el orden de los elementos en la cola
 * @param cola Puntero a la cola
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola invertir_cola(Cola* cola);

/**
 * @brief Rota los elementos de la cola n posiciones hacia adelante
 * @param cola Puntero a la cola
 * @param n Número de posiciones a rotar
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola rotar_cola(Cola* cola, size_t n);

/**
 * @brief Combina dos colas en una nueva cola
 * @param cola1 Primera cola
 * @param cola2 Segunda cola
 * @return Nueva cola con elementos de ambas, o NULL si hay error
 */
Cola* combinar_colas(const Cola* cola1, const Cola* cola2);

// === FUNCIONES DE VISUALIZACIÓN ===

/**
 * @brief Imprime todos los valores de la cola
 * @param cola Puntero a la cola
 */
void imprimir_cola(const Cola* cola);

/**
 * @brief Imprime la cola con formato detallado
 * @param cola Puntero a la cola
 * @param titulo Título descriptivo para la impresión
 */
void imprimir_cola_detallada(const Cola* cola, const char* titulo);

/**
 * @brief Imprime la cola en formato visual (horizontal)
 * @param cola Puntero a la cola
 */
void imprimir_cola_visual(const Cola* cola);

/**
 * @brief Imprime información de depuración de la cola
 * @param cola Puntero a la cola
 */
void imprimir_debug_cola(const Cola* cola);

// === FUNCIONES PARA VERSIÓN SIMPLE (COMPATIBILIDAD) ===

/**
 * @brief Estructura simple de cola (solo punteros)
 */
typedef struct {
    Nodo* frente;
    Nodo* final;
} ColaSimple;

/**
 * @brief Versión simple: inserta elemento al final
 * @param cola Puntero a la cola simple
 * @param valor Valor a insertar
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola enqueue_simple(ColaSimple* cola, int valor);

/**
 * @brief Versión simple: retira elemento del frente
 * @param cola Puntero a la cola simple
 * @param valor Puntero donde almacenar el valor retirado
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola dequeue_simple(ColaSimple* cola, int* valor);

/**
 * @brief Versión simple: consulta elemento del frente
 * @param cola Puntero a la cola simple
 * @param valor Puntero donde almacenar el valor consultado
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola front_simple(const ColaSimple* cola, int* valor);

/**
 * @brief Versión simple: imprime la cola
 * @param cola Puntero a la cola simple
 */
void imprimir_cola_simple(const ColaSimple* cola);

/**
 * @brief Versión simple: libera toda la cola
 * @param cola Puntero a la cola simple
 */
void destruir_cola_simple(ColaSimple* cola);

// === FUNCIONES DE VALIDACIÓN ===

/**
 * @brief Valida la integridad de la estructura de la cola
 * @param cola Puntero a la cola a validar
 * @return true si la cola es válida, false si hay inconsistencias
 */
bool validar_integridad_cola(const Cola* cola);

/**
 * @brief Compara dos colas para verificar si son iguales
 * @param cola1 Primera cola a comparar
 * @param cola2 Segunda cola a comparar
 * @return true si las colas son iguales, false en caso contrario
 */
bool colas_iguales(const Cola* cola1, const Cola* cola2);

// === SIMULACIONES Y APLICACIONES PRÁCTICAS ===

/**
 * @brief Simula un sistema de atención al cliente
 * @param tiempos_llegada Array con tiempos de llegada de clientes
 * @param tiempos_atencion Array con tiempos de atención por cliente
 * @param num_clientes Número de clientes
 */
void simular_atencion_clientes(const int* tiempos_llegada, 
                              const int* tiempos_atencion, 
                              size_t num_clientes);

/**
 * @brief Simula un buffer de impresión
 * @param documentos Array con IDs de documentos
 * @param num_documentos Número de documentos
 */
void simular_buffer_impresion(const int* documentos, size_t num_documentos);

/**
 * @brief Simula un sistema de procesos (scheduler FIFO)
 * @param procesos Array con IDs de procesos
 * @param prioridades Array con prioridades (para comparación)
 * @param num_procesos Número de procesos
 */
void simular_scheduler_fifo(const int* procesos, const int* prioridades, 
                           size_t num_procesos);

/**
 * @brief Simula el algoritmo BFS (Breadth-First Search)
 * @param grafo Matriz de adyacencia
 * @param num_vertices Número de vértices
 * @param inicio Vértice de inicio
 */
void simular_bfs(int** grafo, size_t num_vertices, int inicio);

// === FUNCIONES EDUCATIVAS Y DE ANÁLISIS ===

/**
 * @brief Demuestra las operaciones básicas de la cola
 */
void demostrar_operaciones_basicas(void);

/**
 * @brief Demuestra operaciones avanzadas de la cola
 */
void demostrar_operaciones_avanzadas(void);

/**
 * @brief Demuestra casos especiales y manejo de errores
 */
void demostrar_casos_especiales(void);

/**
 * @brief Analiza el rendimiento de las operaciones de la cola
 */
void analizar_rendimiento_cola(void);

/**
 * @brief Compara cola vs pila (FIFO vs LIFO)
 */
void comparar_cola_vs_pila(void);

/**
 * @brief Demuestra aplicaciones prácticas de colas
 */
void demostrar_aplicaciones_practicas(void);

// === FUNCIONES DE UTILIDAD ===

/**
 * @brief Convierte código de resultado a cadena descriptiva
 * @param resultado Código de resultado
 * @return Cadena descriptiva del resultado
 */
const char* resultado_a_cadena(ResultadoCola resultado);

/**
 * @brief Obtiene estadísticas de uso de la cola
 * @param cola Puntero a la cola
 * @param tamaño_actual Puntero donde almacenar el tamaño actual
 * @param capacidad Puntero donde almacenar la capacidad
 * @param porcentaje_uso Puntero donde almacenar el porcentaje de uso
 */
void obtener_estadisticas(const Cola* cola, size_t* tamaño_actual, 
                         size_t* capacidad, double* porcentaje_uso);

/**
 * @brief Convierte cola a array (para procesamiento externo)
 * @param cola Puntero a la cola
 * @param array Array donde almacenar los elementos
 * @param tamaño_max Tamaño máximo del array
 * @return Número de elementos copiados
 */
size_t cola_a_array(const Cola* cola, int* array, size_t tamaño_max);

/**
 * @brief Crea cola desde array
 * @param array Array con elementos
 * @param tamaño Número de elementos
 * @param capacidad_maxima Capacidad máxima de la cola
 * @return Nueva cola con elementos del array, o NULL si hay error
 */
Cola* array_a_cola(const int* array, size_t tamaño, size_t capacidad_maxima);

// === FUNCIONES DE FILTRADO Y TRANSFORMACIÓN ===

/**
 * @brief Filtra elementos de la cola según un predicado
 * @param cola Puntero a la cola
 * @param predicado Función que determina si mantener un elemento
 * @return Nueva cola con elementos filtrados, o NULL si hay error
 */
Cola* filtrar_cola(const Cola* cola, bool (*predicado)(int));

/**
 * @brief Transforma elementos de la cola según una función
 * @param cola Puntero a la cola
 * @param transformador Función que transforma cada elemento
 * @return Nueva cola con elementos transformados, o NULL si hay error
 */
Cola* transformar_cola(const Cola* cola, int (*transformador)(int));

/**
 * @brief Reduce la cola a un solo valor
 * @param cola Puntero a la cola
 * @param reductor Función que combina dos elementos
 * @param valor_inicial Valor inicial para la reducción
 * @param resultado Puntero donde almacenar el resultado
 * @return COLA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoCola reducir_cola(const Cola* cola, int (*reductor)(int, int), 
                          int valor_inicial, int* resultado);

#endif // COLA_LISTA_H
