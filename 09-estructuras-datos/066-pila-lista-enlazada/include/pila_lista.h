#ifndef PILA_LISTA_H
#define PILA_LISTA_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Estructura de un nodo en la pila
 * 
 * Esta estructura representa un nodo individual en la pila,
 * conteniendo un valor entero y un puntero al siguiente nodo.
 */
typedef struct Nodo {
    int valor;                  ///< Valor almacenado en el nodo
    struct Nodo* siguiente;     ///< Puntero al siguiente nodo (hacia abajo en la pila)
} Nodo;

/**
 * @brief Estructura que representa una pila completa
 * 
 * Incluye metadatos adicionales para optimizar operaciones comunes
 * y mantener información sobre el estado de la pila.
 */
typedef struct {
    Nodo* cima;                 ///< Nodo en la cima de la pila
    size_t tamaño;              ///< Número de elementos en la pila
    size_t capacidad_maxima;    ///< Capacidad máxima (0 = ilimitada)
} Pila;

/**
 * @brief Códigos de resultado para operaciones de la pila
 */
typedef enum {
    PILA_EXITO = 0,             ///< Operación exitosa
    PILA_ERROR_MEMORIA = -1,    ///< Error de asignación de memoria
    PILA_VACIA = -2,            ///< Pila vacía cuando se esperaba contenido
    PILA_LLENA = -3,            ///< Pila llena (si tiene límite de capacidad)
    PILA_ERROR_PARAMETRO = -4   ///< Parámetros inválidos
} ResultadoPila;

// === FUNCIONES DE CREACIÓN Y GESTIÓN ===

/**
 * @brief Crea una nueva pila vacía
 * @param capacidad_maxima Capacidad máxima (0 para ilimitada)
 * @return Puntero a la nueva pila o NULL si hay error de memoria
 */
Pila* crear_pila(size_t capacidad_maxima);

/**
 * @brief Crea un nuevo nodo con el valor especificado
 * @param valor Valor a almacenar en el nodo
 * @return Puntero al nuevo nodo o NULL si hay error de memoria
 */
Nodo* crear_nodo(int valor);

/**
 * @brief Libera toda la memoria ocupada por la pila
 * @param pila Puntero a la pila a liberar
 */
void destruir_pila(Pila* pila);

/**
 * @brief Libera la memoria de un nodo individual
 * @param nodo Puntero al nodo a liberar
 */
void destruir_nodo(Nodo* nodo);

// === OPERACIONES PRINCIPALES DE LA PILA (LIFO) ===

/**
 * @brief Inserta un elemento en la cima de la pila (push)
 * @param pila Puntero a la pila
 * @param valor Valor a insertar
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila push(Pila* pila, int valor);

/**
 * @brief Retira y retorna el elemento de la cima de la pila (pop)
 * @param pila Puntero a la pila
 * @param valor Puntero donde almacenar el valor retirado
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila pop(Pila* pila, int* valor);

/**
 * @brief Consulta el elemento de la cima sin retirarlo (peek)
 * @param pila Puntero a la pila
 * @param valor Puntero donde almacenar el valor consultado
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila peek(const Pila* pila, int* valor);

// === OPERACIONES DE CONSULTA ===

/**
 * @brief Verifica si la pila está vacía
 * @param pila Puntero a la pila
 * @return true si la pila está vacía, false en caso contrario
 */
bool esta_vacia(const Pila* pila);

/**
 * @brief Verifica si la pila está llena (solo si tiene capacidad máxima)
 * @param pila Puntero a la pila
 * @return true si la pila está llena, false en caso contrario
 */
bool esta_llena(const Pila* pila);

/**
 * @brief Obtiene el tamaño actual de la pila
 * @param pila Puntero a la pila
 * @return Número de elementos en la pila
 */
size_t obtener_tamaño(const Pila* pila);

/**
 * @brief Obtiene la capacidad máxima de la pila
 * @param pila Puntero a la pila
 * @return Capacidad máxima (0 si es ilimitada)
 */
size_t obtener_capacidad(const Pila* pila);

/**
 * @brief Verifica si la pila contiene un valor específico
 * @param pila Puntero a la pila
 * @param valor Valor a buscar
 * @return true si el valor está en la pila, false en caso contrario
 */
bool contiene_valor(const Pila* pila, int valor);

// === OPERACIONES AVANZADAS ===

/**
 * @brief Vacía completamente la pila
 * @param pila Puntero a la pila
 */
void vaciar_pila(Pila* pila);

/**
 * @brief Duplica el elemento de la cima
 * @param pila Puntero a la pila
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila duplicar_cima(Pila* pila);

/**
 * @brief Intercambia los dos elementos superiores de la pila
 * @param pila Puntero a la pila
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila intercambiar_dos_superiores(Pila* pila);

/**
 * @brief Rota los n elementos superiores de la pila
 * @param pila Puntero a la pila
 * @param n Número de elementos a rotar
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila rotar_superiores(Pila* pila, size_t n);

/**
 * @brief Crea una copia de la pila
 * @param pila_original Pila a copiar
 * @return Nueva pila que es copia de la original, o NULL si hay error
 */
Pila* clonar_pila(const Pila* pila_original);

/**
 * @brief Invierte el orden de los elementos en la pila
 * @param pila Puntero a la pila
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila invertir_pila(Pila* pila);

// === FUNCIONES DE VISUALIZACIÓN ===

/**
 * @brief Imprime todos los valores de la pila
 * @param pila Puntero a la pila
 */
void imprimir_pila(const Pila* pila);

/**
 * @brief Imprime la pila con formato detallado
 * @param pila Puntero a la pila
 * @param titulo Título descriptivo para la impresión
 */
void imprimir_pila_detallada(const Pila* pila, const char* titulo);

/**
 * @brief Imprime la pila en formato visual (vertical)
 * @param pila Puntero a la pila
 */
void imprimir_pila_visual(const Pila* pila);

/**
 * @brief Imprime información de depuración de la pila
 * @param pila Puntero a la pila
 */
void imprimir_debug_pila(const Pila* pila);

// === FUNCIONES PARA VERSIÓN SIMPLE (COMPATIBILIDAD) ===

/**
 * @brief Versión simple: inserta elemento en la cima usando solo puntero a cima
 * @param cima Puntero al puntero de la cima de la pila
 * @param valor Valor a insertar
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila push_simple(Nodo** cima, int valor);

/**
 * @brief Versión simple: retira elemento de la cima usando solo puntero a cima
 * @param cima Puntero al puntero de la cima de la pila
 * @param valor Puntero donde almacenar el valor retirado
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila pop_simple(Nodo** cima, int* valor);

/**
 * @brief Versión simple: consulta elemento de la cima usando solo puntero a cima
 * @param cima Puntero a la cima de la pila
 * @param valor Puntero donde almacenar el valor consultado
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila peek_simple(const Nodo* cima, int* valor);

/**
 * @brief Versión simple: imprime la pila usando solo puntero a cima
 * @param cima Puntero a la cima de la pila
 */
void imprimir_pila_simple(const Nodo* cima);

/**
 * @brief Versión simple: libera toda la pila usando solo puntero a cima
 * @param cima Puntero al puntero de la cima de la pila
 */
void destruir_pila_simple(Nodo** cima);

// === FUNCIONES DE VALIDACIÓN ===

/**
 * @brief Valida la integridad de la estructura de la pila
 * @param pila Puntero a la pila a validar
 * @return true si la pila es válida, false si hay inconsistencias
 */
bool validar_integridad_pila(const Pila* pila);

/**
 * @brief Compara dos pilas para verificar si son iguales
 * @param pila1 Primera pila a comparar
 * @param pila2 Segunda pila a comparar
 * @return true si las pilas son iguales, false en caso contrario
 */
bool pilas_iguales(const Pila* pila1, const Pila* pila2);

// === FUNCIONES EDUCATIVAS Y DE ANÁLISIS ===

/**
 * @brief Demuestra las operaciones básicas de la pila
 */
void demostrar_operaciones_basicas(void);

/**
 * @brief Demuestra operaciones avanzadas de la pila
 */
void demostrar_operaciones_avanzadas(void);

/**
 * @brief Demuestra casos especiales y manejo de errores
 */
void demostrar_casos_especiales(void);

/**
 * @brief Analiza el rendimiento de las operaciones de la pila
 */
void analizar_rendimiento_pila(void);

/**
 * @brief Demuestra aplicaciones prácticas de pilas
 */
void demostrar_aplicaciones_practicas(void);

// === APLICACIONES PRÁCTICAS ===

/**
 * @brief Evalúa una expresión en notación postfija (RPN)
 * @param expresion Cadena con la expresión a evaluar
 * @param resultado Puntero donde almacenar el resultado
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila evaluar_postfija(const char* expresion, int* resultado);

/**
 * @brief Verifica si los paréntesis están balanceados en una expresión
 * @param expresion Cadena con la expresión a verificar
 * @return true si están balanceados, false en caso contrario
 */
bool verificar_parentesis_balanceados(const char* expresion);

/**
 * @brief Convierte un número decimal a binario usando pila
 * @param numero Número decimal a convertir
 * @param binario Buffer donde almacenar el resultado
 * @param tamaño_buffer Tamaño del buffer
 * @return PILA_EXITO si es exitoso, código de error en caso contrario
 */
ResultadoPila decimal_a_binario(int numero, char* binario, size_t tamaño_buffer);

/**
 * @brief Simula el mecanismo de llamadas de función (call stack)
 * @param funciones Array con nombres de funciones
 * @param num_funciones Número de funciones
 */
void simular_call_stack(const char* funciones[], size_t num_funciones);

// === FUNCIONES DE UTILIDAD ===

/**
 * @brief Convierte código de resultado a cadena descriptiva
 * @param resultado Código de resultado
 * @return Cadena descriptiva del resultado
 */
const char* resultado_a_cadena(ResultadoPila resultado);

/**
 * @brief Obtiene estadísticas de uso de la pila
 * @param pila Puntero a la pila
 * @param tamaño_actual Puntero donde almacenar el tamaño actual
 * @param capacidad Puntero donde almacenar la capacidad
 * @param porcentaje_uso Puntero donde almacenar el porcentaje de uso
 */
void obtener_estadisticas(const Pila* pila, size_t* tamaño_actual, 
                         size_t* capacidad, double* porcentaje_uso);

#endif // PILA_LISTA_H
