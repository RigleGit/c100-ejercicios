/**
 * @file entrada_segura.h
 * @brief Funciones para entrada segura de datos - Comparación gets() vs fgets()
 * @details Este header demuestra la diferencia entre funciones inseguras y seguras
 *          para la lectura de datos desde la entrada estándar. Se enfoca en
 *          prevenir vulnerabilidades de buffer overflow.
 * 
 * VULNERABILIDAD PRINCIPAL:
 * - gets() NO VERIFICA LÍMITES y puede causar buffer overflow
 * - fgets() VERIFICA LÍMITES y previene desbordamientos
 * 
 * @author Curso C - Ejercicio 094
 * @date 2024
 * @version 1.0
 */

#ifndef ENTRADA_SEGURA_H
#define ENTRADA_SEGURA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Tamaño por defecto de buffer para demostraciones
 */
#define BUFFER_SIZE 50

/**
 * @brief Tamaño pequeño de buffer para mostrar vulnerabilidades
 */
#define BUFFER_PEQUEÑO 10

/**
 * @brief Tamaño máximo de línea para entrada
 */
#define MAX_LINEA 1024

/**
 * @brief Códigos de retorno para las funciones
 */
typedef enum {
    ENTRADA_EXITOSA = 0,        ///< Entrada leída exitosamente
    ENTRADA_TRUNCADA = 1,       ///< Entrada fue truncada
    ENTRADA_ERROR = -1,         ///< Error en la lectura
    ENTRADA_EOF = -2            ///< Fin de archivo alcanzado
} resultado_entrada_t;

/**
 * @brief Estructura para almacenar estadísticas de entrada
 */
typedef struct {
    size_t caracteres_leidos;   ///< Número de caracteres leídos
    size_t lineas_procesadas;   ///< Número de líneas procesadas
    bool truncamiento_ocurrido; ///< Si ocurrió truncamiento
    int codigo_ultimo_error;    ///< Código del último error
} estadisticas_entrada_t;

// =============================================================================
// FUNCIONES INSEGURAS (SOLO PARA DEMOSTRACIÓN - ¡NO USAR EN PRODUCCIÓN!)
// =============================================================================

/**
 * @brief FUNCIÓN INSEGURA: Demuestra el uso peligroso de gets()
 * @warning ¡NUNCA USAR ESTA FUNCIÓN EN CÓDIGO REAL!
 * @details Esta función muestra por qué gets() es peligroso:
 *          - No verifica límites del buffer
 *          - Puede causar buffer overflow
 *          - Puede sobrescribir memoria adyacente
 * 
 * @param buffer Buffer donde almacenar la entrada
 * @note Esta función se mantiene solo con propósitos educativos
 * @deprecated gets() está deprecado desde C99 y removido en C11
 * @return Puntero al buffer si exitoso, NULL si error
 */
char* entrada_insegura_gets(char* buffer);

/**
 * @brief FUNCIÓN INSEGURA: Demuestra scanf sin límites
 * @warning ¡PELIGROSO! No usar en código de producción
 * @details Muestra por qué scanf("%s") es inseguro:
 *          - No limita la cantidad de caracteres leídos
 *          - Puede desbordar el buffer fácilmente
 * 
 * @param buffer Buffer donde almacenar la entrada
 * @return Número de elementos leídos exitosamente
 */
int entrada_insegura_scanf(char* buffer);

// =============================================================================
// FUNCIONES SEGURAS
// =============================================================================

/**
 * @brief FUNCIÓN SEGURA: Lee una línea usando fgets()
 * @details Alternativa segura a gets() que:
 *          - Limita la cantidad de caracteres leídos
 *          - Previene buffer overflow
 *          - Preserva el carácter de nueva línea si cabe
 * 
 * @param buffer Buffer donde almacenar la entrada
 * @param tamaño Tamaño del buffer (incluyendo espacio para '\0')
 * @param archivo Stream de entrada (normalmente stdin)
 * @return resultado_entrada_t indicando el estado de la operación
 */
resultado_entrada_t entrada_segura_fgets(char* buffer, size_t tamaño, FILE* archivo);

/**
 * @brief FUNCIÓN SEGURA: Lee una línea y elimina el '\n' final
 * @details Versión mejorada de fgets que:
 *          - Elimina automáticamente el '\n' final
 *          - Maneja líneas que exceden el buffer
 *          - Proporciona feedback sobre truncamiento
 * 
 * @param buffer Buffer donde almacenar la entrada
 * @param tamaño Tamaño del buffer
 * @param archivo Stream de entrada
 * @return resultado_entrada_t con información detallada
 */
resultado_entrada_t entrada_segura_linea_limpia(char* buffer, size_t tamaño, FILE* archivo);

/**
 * @brief FUNCIÓN SEGURA: Lee entrada con validación
 * @details Función robusta que:
 *          - Valida parámetros de entrada
 *          - Maneja errores gracefully
 *          - Proporciona estadísticas detalladas
 * 
 * @param buffer Buffer de destino
 * @param tamaño Tamaño del buffer
 * @param archivo Stream de entrada
 * @param stats Puntero a estructura de estadísticas (puede ser NULL)
 * @return resultado_entrada_t con código de estado
 */
resultado_entrada_t entrada_segura_validada(char* buffer, size_t tamaño, 
                                           FILE* archivo, estadisticas_entrada_t* stats);

/**
 * @brief FUNCIÓN SEGURA: scanf con límites especificados
 * @details Alternativa segura a scanf("%s") que:
 *          - Especifica límite máximo de caracteres
 *          - Previene buffer overflow en scanf
 * 
 * @param buffer Buffer de destino
 * @param tamaño Tamaño máximo a leer
 * @param formato Formato de scanf (con límite)
 * @return Número de elementos leídos exitosamente
 */
int entrada_segura_scanf_limitado(char* buffer, size_t tamaño, const char* formato);

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN Y COMPARACIÓN
// =============================================================================

/**
 * @brief Demuestra la vulnerabilidad de gets()
 * @details Función educativa que muestra:
 *          - Cómo gets() puede causar buffer overflow
 *          - Los riesgos de seguridad asociados
 *          - Cómo detectar el problema
 * 
 * @note SOLO PARA PROPÓSITOS EDUCATIVOS
 */
void demostrar_vulnerabilidad_gets(void);

/**
 * @brief Demuestra el uso seguro de fgets()
 * @details Muestra las ventajas de fgets():
 *          - Prevención de buffer overflow
 *          - Manejo controlado de entrada
 *          - Comportamiento predecible
 */
void demostrar_seguridad_fgets(void);

/**
 * @brief Comparación lado a lado de gets() vs fgets()
 * @details Función interactiva que permite al usuario:
 *          - Ver ambos enfoques en acción
 *          - Entender las diferencias
 *          - Experimentar con diferentes entradas
 */
void comparacion_gets_vs_fgets(void);

/**
 * @brief Ejecuta demostración completa del ejercicio
 * @details Función principal que ejecuta:
 *          - Explicación teórica
 *          - Demostraciones prácticas
 *          - Comparaciones interactivas
 *          - Mejores prácticas
 */
void demostracion_completa_entrada_segura(void);

// =============================================================================
// FUNCIONES AUXILIARES
// =============================================================================

/**
 * @brief Convierte resultado_entrada_t a string descriptivo
 * @param resultado Código de resultado a convertir
 * @return String descriptivo del resultado
 */
const char* resultado_a_string(resultado_entrada_t resultado);

/**
 * @brief Limpia el buffer de entrada para evitar caracteres residuales
 * @details Útil después de operaciones de entrada que pueden dejar
 *          caracteres en el stream
 */
void limpiar_buffer_entrada(void);

/**
 * @brief Imprime estadísticas de entrada de forma formateada
 * @param stats Puntero a estructura de estadísticas
 */
void imprimir_estadisticas(const estadisticas_entrada_t* stats);

/**
 * @brief Inicializa estructura de estadísticas
 * @param stats Puntero a estructura a inicializar
 */
void inicializar_estadisticas(estadisticas_entrada_t* stats);

#endif // ENTRADA_SEGURA_H
