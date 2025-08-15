#ifndef EVALUADOR_RPN_H
#define EVALUADOR_RPN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>

// Constantes
#define MAX_TOKEN_LENGTH 64
#define MAX_EXPRESSION_LENGTH 1024
#define MAX_OPERADORES 50

// Códigos de error
typedef enum {
    RPN_SUCCESS = 0,
    RPN_ERROR_PILA_VACIA = 1,
    RPN_ERROR_EXPRESION_MALFORMADA = 2,
    RPN_ERROR_DIVISION_CERO = 3,
    RPN_ERROR_OPERADOR_DESCONOCIDO = 4,
    RPN_ERROR_TOKEN_INVALIDO = 5,
    RPN_ERROR_MEMORIA = 6,
    RPN_ERROR_ENTRADA_INVALIDA = 7,
    RPN_ERROR_OVERFLOW = 8,
    RPN_ERROR_UNDERFLOW = 9
} rpn_error_t;

// Estructura para el nodo de la pila
typedef struct nodo_pila {
    double valor;
    struct nodo_pila *siguiente;
} nodo_pila_t;

// Estructura para la pila
typedef struct {
    nodo_pila_t *cima;
    size_t tamaño;
    size_t capacidad_maxima;
} pila_t;

// Estructura para el resultado de evaluación
typedef struct {
    double resultado;
    rpn_error_t error;
    char mensaje_error[256];
    size_t tokens_procesados;
    size_t operaciones_realizadas;
} resultado_evaluacion_t;

// Estructura para operador
typedef struct {
    char simbolo;
    const char *nombre;
    int precedencia;
    bool asociativo_izquierda;
    double (*funcion)(double a, double b);
} operador_t;

// Estructura para estadísticas de evaluación
typedef struct {
    size_t total_evaluaciones;
    size_t evaluaciones_exitosas;
    size_t errores_encontrados;
    size_t operaciones_aritmeticas;
    double tiempo_total_ms;
} estadisticas_rpn_t;

// ========================================
// FUNCIONES DE MANEJO DE PILA
// ========================================

/**
 * @brief Inicializa una nueva pila
 * @param capacidad_maxima Capacidad máxima de la pila (0 para ilimitada)
 * @return Puntero a la pila inicializada, NULL en caso de error
 */
pila_t *pila_crear(size_t capacidad_maxima);

/**
 * @brief Libera la memoria de una pila
 * @param pila Puntero a la pila a liberar
 */
void pila_destruir(pila_t *pila);

/**
 * @brief Apila un valor en la pila
 * @param pila Puntero a la pila
 * @param valor Valor a apilar
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t pila_push(pila_t *pila, double valor);

/**
 * @brief Desapila un valor de la pila
 * @param pila Puntero a la pila
 * @param valor Puntero donde almacenar el valor desapilado
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t pila_pop(pila_t *pila, double *valor);

/**
 * @brief Obtiene el valor de la cima sin desapilar
 * @param pila Puntero a la pila
 * @param valor Puntero donde almacenar el valor de la cima
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t pila_peek(const pila_t *pila, double *valor);

/**
 * @brief Verifica si la pila está vacía
 * @param pila Puntero a la pila
 * @return true si está vacía, false en caso contrario
 */
bool pila_esta_vacia(const pila_t *pila);

/**
 * @brief Obtiene el tamaño actual de la pila
 * @param pila Puntero a la pila
 * @return Tamaño de la pila
 */
size_t pila_tamaño(const pila_t *pila);

/**
 * @brief Limpia todos los elementos de la pila
 * @param pila Puntero a la pila
 */
void pila_limpiar(pila_t *pila);

/**
 * @brief Imprime el contenido de la pila (para debugging)
 * @param pila Puntero a la pila
 */
void pila_imprimir(const pila_t *pila);

// ========================================
// FUNCIONES DE EVALUACIÓN RPN
// ========================================

/**
 * @brief Evalúa una expresión en notación postfija
 * @param expresion Cadena con la expresión RPN
 * @return Resultado de la evaluación con información de error
 */
resultado_evaluacion_t evaluar_rpn(const char *expresion);

/**
 * @brief Evalúa una expresión RPN usando una pila específica
 * @param expresion Cadena con la expresión RPN
 * @param pila Pila a usar para la evaluación
 * @return Resultado de la evaluación
 */
resultado_evaluacion_t evaluar_rpn_con_pila(const char *expresion, pila_t *pila);

/**
 * @brief Verifica si un token es un número válido
 * @param token Token a verificar
 * @return true si es número, false en caso contrario
 */
bool es_numero(const char *token);

/**
 * @brief Verifica si un token es un operador válido
 * @param token Token a verificar
 * @return true si es operador, false en caso contrario
 */
bool es_operador(const char *token);

/**
 * @brief Convierte un token numérico a double
 * @param token Token a convertir
 * @param valor Puntero donde almacenar el valor
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t token_a_numero(const char *token, double *valor);

/**
 * @brief Aplica un operador a dos operandos
 * @param operador Carácter del operador
 * @param a Primer operando
 * @param b Segundo operando
 * @param resultado Puntero donde almacenar el resultado
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t aplicar_operador(char operador, double a, double b, double *resultado);

// ========================================
// FUNCIONES DE UTILIDAD Y VALIDACIÓN
// ========================================

/**
 * @brief Valida una expresión RPN antes de evaluarla
 * @param expresion Expresión a validar
 * @return RPN_SUCCESS si es válida, código de error en caso contrario
 */
rpn_error_t validar_expresion_rpn(const char *expresion);

/**
 * @brief Convierte un código de error a mensaje descriptivo
 * @param error Código de error
 * @return Mensaje descriptivo del error
 */
const char *error_a_string(rpn_error_t error);

/**
 * @brief Obtiene información sobre un operador
 * @param simbolo Símbolo del operador
 * @return Puntero a la estructura del operador, NULL si no existe
 */
const operador_t *obtener_operador(char simbolo);

/**
 * @brief Lista todos los operadores soportados
 * @return Array de operadores terminado en NULL
 */
const operador_t *obtener_operadores_soportados(void);

// ========================================
// FUNCIONES DE CONVERSIÓN Y UTILIDAD
// ========================================

/**
 * @brief Convierte expresión infija a postfija usando algoritmo Shunting Yard
 * @param infija Expresión en notación infija
 * @param postfija Buffer para almacenar la expresión postfija
 * @param tamaño_buffer Tamaño del buffer de salida
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t infija_a_postfija(const char *infija, char *postfija, size_t tamaño_buffer);

/**
 * @brief Tokeniza una expresión en tokens individuales
 * @param expresion Expresión a tokenizar
 * @param tokens Array donde almacenar los tokens
 * @param max_tokens Máximo número de tokens
 * @param num_tokens Puntero donde almacenar el número de tokens encontrados
 * @return RPN_SUCCESS en éxito, código de error en caso contrario
 */
rpn_error_t tokenizar_expresion(const char *expresion, char tokens[][MAX_TOKEN_LENGTH], 
                               size_t max_tokens, size_t *num_tokens);

// ========================================
// FUNCIONES DE DEMOSTRACIÓN Y TESTING
// ========================================

/**
 * @brief Ejecuta una suite de pruebas para el evaluador RPN
 * @return true si todas las pruebas pasan, false en caso contrario
 */
bool ejecutar_pruebas_rpn(void);

/**
 * @brief Demuestra el funcionamiento del evaluador con ejemplos
 */
void demostrar_evaluador_rpn(void);

/**
 * @brief Modo interactivo para evaluación de expresiones
 */
void modo_interactivo(void);

/**
 * @brief Procesa un archivo con expresiones RPN
 * @param nombre_archivo Nombre del archivo a procesar
 * @param mostrar_pasos Si mostrar los pasos de evaluación
 * @return Número de expresiones procesadas exitosamente
 */
size_t procesar_archivo_rpn(const char *nombre_archivo, bool mostrar_pasos);

// ========================================
// FUNCIONES DE ESTADÍSTICAS Y ANÁLISIS
// ========================================

/**
 * @brief Inicializa las estadísticas globales
 */
void inicializar_estadisticas(void);

/**
 * @brief Obtiene las estadísticas actuales
 * @return Estructura con las estadísticas
 */
estadisticas_rpn_t obtener_estadisticas(void);

/**
 * @brief Imprime un reporte de estadísticas
 */
void imprimir_estadisticas(void);

/**
 * @brief Reinicia las estadísticas
 */
void reiniciar_estadisticas(void);

// ========================================
// FUNCIONES DE CONFIGURACIÓN Y DEBUGGING
// ========================================

/**
 * @brief Establece el nivel de verbosidad para debugging
 * @param nivel Nivel de verbosidad (0=silencioso, 3=muy verboso)
 */
void establecer_verbosidad(int nivel);

/**
 * @brief Habilita o deshabilita el modo de depuración paso a paso
 * @param habilitar true para habilitar, false para deshabilitar
 */
void establecer_modo_paso_a_paso(bool habilitar);

/**
 * @brief Obtiene la versión del evaluador
 * @return Cadena con la versión
 */
const char *obtener_version(void);

#endif // EVALUADOR_RPN_H
