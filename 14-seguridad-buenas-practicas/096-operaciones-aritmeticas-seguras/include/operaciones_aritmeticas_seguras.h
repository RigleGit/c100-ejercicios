/**
 * @file operaciones_aritmeticas_seguras.h
 * @brief Funciones para operaciones aritméticas seguras con detección de overflow
 * @details Este header proporciona implementaciones seguras de operaciones
 *          aritméticas básicas que detectan y previenen overflow/underflow,
 *          un problema crítico de seguridad en programas C.
 * 
 * PROBLEMAS DE SEGURIDAD EN ARITMÉTICA:
 * - Integer Overflow: Resultado excede el valor máximo representable
 * - Integer Underflow: Resultado es menor al valor mínimo representable
 * - Comportamiento indefinido: C no define el comportamiento en overflow
 * - Vulnerabilidades: Pueden permitir bypass de validaciones de seguridad
 * 
 * @author Curso C - Ejercicio 096
 * @date 2024
 * @version 1.0
 */

#ifndef OPERACIONES_ARITMETICAS_SEGURAS_H
#define OPERACIONES_ARITMETICAS_SEGURAS_H

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

/**
 * @brief Códigos de resultado para operaciones aritméticas
 */
typedef enum {
    ARITMETICA_EXITOSA = 0,         ///< Operación exitosa sin overflow
    ARITMETICA_OVERFLOW = 1,        ///< Overflow positivo detectado
    ARITMETICA_UNDERFLOW = 2,       ///< Underflow (overflow negativo) detectado
    ARITMETICA_DIVISION_CERO = 3,   ///< División por cero
    ARITMETICA_ERROR_PARAMETRO = -1 ///< Error en parámetros de entrada
} resultado_aritmetica_t;

/**
 * @brief Estructura para almacenar estadísticas de operaciones
 */
typedef struct {
    size_t operaciones_exitosas;    ///< Número de operaciones exitosas
    size_t overflows_detectados;    ///< Número de overflows detectados
    size_t underflows_detectados;   ///< Número de underflows detectados
    size_t divisiones_cero;         ///< Número de divisiones por cero
    size_t errores_parametro;       ///< Número de errores de parámetro
} estadisticas_aritmetica_t;

// =============================================================================
// OPERACIONES BÁSICAS SEGURAS PARA INT
// =============================================================================

/**
 * @brief Suma segura de dos enteros con detección de overflow
 * @details Verifica si la suma de dos enteros produciría overflow/underflow
 *          antes de realizar la operación. Usa las siguientes condiciones:
 *          - Overflow: b > 0 && a > INT_MAX - b
 *          - Underflow: b < 0 && a < INT_MIN - b
 * 
 * @param a Primer operando
 * @param b Segundo operando  
 * @param resultado Puntero donde almacenar el resultado (solo si exitoso)
 * @return resultado_aritmetica_t indicando el estado de la operación
 * 
 * @example
 * int resultado;
 * if (suma_segura(INT_MAX, 1, &resultado) == ARITMETICA_OVERFLOW) {
 *     printf("Overflow detectado!\n");
 * }
 */
resultado_aritmetica_t suma_segura(int a, int b, int* resultado);

/**
 * @brief Resta segura de dos enteros con detección de overflow
 * @details Verifica si la resta produciría overflow/underflow:
 *          - Overflow: b < 0 && a > INT_MAX + b
 *          - Underflow: b > 0 && a < INT_MIN + b
 * 
 * @param a Minuendo
 * @param b Sustraendo
 * @param resultado Puntero donde almacenar el resultado
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t resta_segura(int a, int b, int* resultado);

/**
 * @brief Multiplicación segura de dos enteros
 * @details Verifica overflow en multiplicación considerando múltiples casos:
 *          - Casos especiales: 0, 1, -1
 *          - División para verificar límites
 *          - Manejo de signos diferentes
 * 
 * @param a Primer factor
 * @param b Segundo factor
 * @param resultado Puntero donde almacenar el resultado
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t multiplicacion_segura(int a, int b, int* resultado);

/**
 * @brief División segura de dos enteros
 * @details Verifica división por cero y el caso especial INT_MIN / -1
 *          que produce overflow (INT_MAX + 1)
 * 
 * @param a Dividendo
 * @param b Divisor
 * @param resultado Puntero donde almacenar el resultado
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t division_segura(int a, int b, int* resultado);

/**
 * @brief Módulo seguro de dos enteros
 * @details Similar a división pero retorna el residuo.
 *          También maneja división por cero y casos especiales.
 * 
 * @param a Dividendo
 * @param b Divisor
 * @param resultado Puntero donde almacenar el residuo
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t modulo_seguro(int a, int b, int* resultado);

// =============================================================================
// OPERACIONES SEGURAS PARA OTROS TIPOS
// =============================================================================

/**
 * @brief Suma segura para long long
 * @param a Primer operando
 * @param b Segundo operando
 * @param resultado Puntero donde almacenar el resultado
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t suma_segura_long(long long a, long long b, long long* resultado);

/**
 * @brief Suma segura para unsigned int
 * @param a Primer operando
 * @param b Segundo operando
 * @param resultado Puntero donde almacenar el resultado
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t suma_segura_uint(unsigned int a, unsigned int b, unsigned int* resultado);

/**
 * @brief Suma segura para size_t (importante para cálculos de memoria)
 * @param a Primer operando
 * @param b Segundo operando
 * @param resultado Puntero donde almacenar el resultado
 * @return resultado_aritmetica_t indicando el estado
 */
resultado_aritmetica_t suma_segura_size_t(size_t a, size_t b, size_t* resultado);

// =============================================================================
// FUNCIONES DE VALIDACIÓN Y VERIFICACIÓN
// =============================================================================

/**
 * @brief Verifica si una suma produciría overflow sin realizar la operación
 * @param a Primer operando
 * @param b Segundo operando
 * @return true si la suma sería segura, false si produciría overflow
 */
bool verificar_suma_segura(int a, int b);

/**
 * @brief Verifica si una multiplicación produciría overflow
 * @param a Primer factor
 * @param b Segundo factor
 * @return true si la multiplicación sería segura
 */
bool verificar_multiplicacion_segura(int a, int b);

/**
 * @brief Calcula el rango seguro para sumar a un valor dado
 * @param base Valor base
 * @param min_suma Puntero donde almacenar el mínimo valor sumable
 * @param max_suma Puntero donde almacenar el máximo valor sumable
 * @return true si se pudo calcular el rango
 */
bool calcular_rango_suma_segura(int base, int* min_suma, int* max_suma);

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN Y TESTING
// =============================================================================

/**
 * @brief Demuestra vulnerabilidades de overflow aritmético
 * @details Muestra ejemplos de cómo los overflows pueden causar
 *          vulnerabilidades de seguridad en validaciones y cálculos
 */
void demostrar_vulnerabilidades_overflow(void);

/**
 * @brief Demuestra el uso de operaciones aritméticas seguras
 * @details Muestra cómo las funciones seguras previenen vulnerabilidades
 *          y proporcionan feedback sobre problemas detectados
 */
void demostrar_operaciones_seguras(void);

/**
 * @brief Ejecuta batería de pruebas con casos límite
 * @details Prueba operaciones con valores en los límites de int:
 *          - INT_MAX, INT_MIN
 *          - Valores cercanos a los límites
 *          - Casos que causan overflow/underflow
 * 
 * @param stats Puntero a estructura para almacenar estadísticas
 */
void ejecutar_pruebas_casos_limite(estadisticas_aritmetica_t* stats);

/**
 * @brief Comparación entre aritmética normal e insegura vs segura
 * @details Demuestra lado a lado el comportamiento de operaciones
 *          normales (que pueden tener overflow silencioso) vs
 *          operaciones seguras con detección
 */
void comparacion_aritmetica_normal_vs_segura(void);

/**
 * @brief Demostración completa del ejercicio
 * @details Función principal que ejecuta todas las demostraciones:
 *          - Explicación teórica
 *          - Ejemplos de vulnerabilidades
 *          - Uso de funciones seguras
 *          - Casos límite y testing
 */
void demostracion_completa_aritmetica_segura(void);

// =============================================================================
// FUNCIONES AUXILIARES Y UTILIDADES
// =============================================================================

/**
 * @brief Convierte resultado_aritmetica_t a string descriptivo
 * @param resultado Código de resultado a convertir
 * @return String descriptivo del resultado
 */
const char* resultado_aritmetica_a_string(resultado_aritmetica_t resultado);

/**
 * @brief Inicializa estructura de estadísticas
 * @param stats Puntero a estructura a inicializar
 */
void inicializar_estadisticas_aritmetica(estadisticas_aritmetica_t* stats);

/**
 * @brief Actualiza estadísticas basado en el resultado de una operación
 * @param stats Puntero a estructura de estadísticas
 * @param resultado Resultado de la operación
 */
void actualizar_estadisticas_aritmetica(estadisticas_aritmetica_t* stats, 
                                       resultado_aritmetica_t resultado);

/**
 * @brief Imprime estadísticas de forma formateada
 * @param stats Puntero a estructura de estadísticas
 */
void imprimir_estadisticas_aritmetica(const estadisticas_aritmetica_t* stats);

/**
 * @brief Imprime los límites de diferentes tipos de datos
 * @details Muestra INT_MAX, INT_MIN, UINT_MAX, etc. para referencia
 */
void imprimir_limites_tipos_datos(void);

/**
 * @brief Calcula la distancia a los límites para un valor dado
 * @param valor Valor a analizar
 * @param distancia_max Distancia a INT_MAX
 * @param distancia_min Distancia a INT_MIN
 */
void calcular_distancia_limites(int valor, int* distancia_max, int* distancia_min);

// =============================================================================
// MACROS PARA VERIFICACIÓN RÁPIDA (OPCIONAL)
// =============================================================================

/**
 * @brief Macro para verificación rápida de suma segura
 * @param a Primer operando
 * @param b Segundo operando
 * @return true si la suma es segura
 */
#define ES_SUMA_SEGURA(a, b) \
    (!((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)))

/**
 * @brief Macro para verificación rápida de multiplicación segura
 * @param a Primer factor
 * @param b Segundo factor
 * @return true si la multiplicación es segura
 */
#define ES_MULT_SEGURA(a, b) \
    (verificar_multiplicacion_segura(a, b))

// =============================================================================
// CONSTANTES ÚTILES
// =============================================================================

/**
 * @brief Valores de prueba comunes para testing
 */
extern const int VALORES_PRUEBA_OVERFLOW[];
extern const size_t NUM_VALORES_PRUEBA;

/**
 * @brief Casos límite específicos para testing exhaustivo
 */
extern const int CASOS_LIMITE_INT[];
extern const size_t NUM_CASOS_LIMITE;

#endif // OPERACIONES_ARITMETICAS_SEGURAS_H
