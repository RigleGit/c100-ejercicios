#ifndef CONSTANTES_PREPROCESADOR_H
#define CONSTANTES_PREPROCESADOR_H

#include <stdio.h>
#include <stdbool.h>

// ============================================================================
// CONSTANTES BÁSICAS (del enunciado)
// ============================================================================

/**
 * @brief Tamaño del array principal (según enunciado)
 */
#define TAMANIO 5

// ============================================================================
// CONSTANTES NUMÉRICAS AVANZADAS
// ============================================================================

/**
 * @brief Tamaños de arrays para diferentes propósitos
 */
#define TAMANIO_PEQUENO 10
#define TAMANIO_MEDIANO 50
#define TAMANIO_GRANDE 100
#define TAMANIO_MAXIMO 1000

/**
 * @brief Constantes matemáticas
 */
#define PI 3.14159265359
#define E 2.71828182846
#define GOLDEN_RATIO 1.61803398875
#define SQRT_2 1.41421356237

/**
 * @brief Límites de datos
 */
#define MAX_NOMBRE 256
#define MAX_LINEA 1024
#define MAX_BUFFER 4096
#define MIN_EDAD 0
#define MAX_EDAD 150

/**
 * @brief Códigos de error
 */
#define ERROR_NINGUNO 0
#define ERROR_MEMORIA -1
#define ERROR_ARCHIVO -2
#define ERROR_PARAMETRO -3
#define ERROR_RANGO -4

// ============================================================================
// CONSTANTES DE CADENAS
// ============================================================================

/**
 * @brief Mensajes estándar
 */
#define MSG_EXITO "Operación exitosa"
#define MSG_ERROR "Error en la operación"
#define MSG_BIENVENIDA "Bienvenido al programa"
#define MSG_DESPEDIDA "¡Hasta luego!"

/**
 * @brief Formatos de salida
 */
#define FORMATO_ENTERO "%d"
#define FORMATO_FLOAT "%.2f"
#define FORMATO_DOUBLE "%.6lf"
#define FORMATO_CADENA "%s"

/**
 * @brief Separadores y delimitadores
 */
#define SEPARADOR_LINEA "=============================="
#define SEPARADOR_SECCION "------------------------------"
#define DELIMITADOR_CAMPO ","
#define DELIMITADOR_REGISTRO "\n"

// ============================================================================
// CONSTANTES CONDICIONALES
// ============================================================================

/**
 * @brief Configuración de compilación
 */
#ifndef DEBUG
    #define DEBUG 0
#endif

#ifndef VERSION_MAYOR
    #define VERSION_MAYOR 1
#endif

#ifndef VERSION_MENOR
    #define VERSION_MENOR 0
#endif

#ifndef VERSION_PATCH
    #define VERSION_PATCH 0
#endif

/**
 * @brief Configuración de características
 */
#define HABILITAR_LOGGING 1
#define HABILITAR_ESTADISTICAS 1
#define HABILITAR_VALIDACION 1
#define HABILITAR_DEMOS_AVANZADAS 1

// ============================================================================
// CONSTANTES CALCULADAS
// ============================================================================

/**
 * @brief Constantes derivadas de otras constantes
 */
#define TAMANIO_DOBLE (TAMANIO * 2)
#define TAMANIO_MITAD (TAMANIO / 2)
#define AREA_CIRCULO_UNITARIO (PI * 1 * 1)
#define PERIMETRO_CIRCULO_UNITARIO (2 * PI * 1)

/**
 * @brief Máscaras de bits
 */
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)

/**
 * @brief Combinaciones de bits
 */
#define MASCARA_INFERIOR (BIT_0 | BIT_1 | BIT_2 | BIT_3)
#define MASCARA_SUPERIOR (BIT_4 | BIT_5 | BIT_6 | BIT_7)
#define MASCARA_COMPLETA (MASCARA_INFERIOR | MASCARA_SUPERIOR)

// ============================================================================
// PSEUDO-FUNCIONES (MACROS SIMPLES)
// ============================================================================

/**
 * @brief Macros para valores absolutos y comparaciones
 */
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
/* SWAP compatible con C99 (sin typeof): pensado para enteros en estas demos */
#define SWAP(a, b) do { int tmp = (a); (a) = (b); (b) = tmp; } while(0)

/**
 * @brief Macros para arrays
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ARRAY_END(arr) ((arr) + ARRAY_SIZE(arr))

/**
 * @brief Macros para logging (si está habilitado)
 */
#if HABILITAR_LOGGING
    #define LOG(msg) printf("[LOG] %s\n", (msg))
    #define LOG_DEBUG(msg) do { if (DEBUG) printf("[DEBUG] %s\n", (msg)); } while(0)
    #define LOG_ERROR(msg) printf("[ERROR] %s\n", (msg))
#else
    #define LOG(msg)
    #define LOG_DEBUG(msg)
    #define LOG_ERROR(msg)
#endif

// ============================================================================
// DECLARACIONES DE FUNCIONES
// ============================================================================

/**
 * @brief Demuestra el uso básico de constantes de preprocesador
 */
void demo_constantes_basicas(void);

/**
 * @brief Demuestra constantes numéricas avanzadas
 */
void demo_constantes_numericas(void);

/**
 * @brief Demuestra constantes de cadenas
 */
void demo_constantes_cadenas(void);

/**
 * @brief Demuestra constantes calculadas
 */
void demo_constantes_calculadas(void);

/**
 * @brief Demuestra macros condicionales
 */
void demo_macros_condicionales(void);

/**
 * @brief Demuestra pseudo-funciones (macros simples)
 */
void demo_pseudo_funciones(void);

/**
 * @brief Demuestra el uso de constantes para arrays
 */
void demo_arrays_con_constantes(void);

/**
 * @brief Demuestra máscaras de bits
 */
void demo_mascaras_bits(void);

/**
 * @brief Muestra información de compilación y versión
 */
void mostrar_info_compilacion(void);

/**
 * @brief Ejecuta todas las demostraciones
 */
void ejecutar_todas_demos(void);

/**
 * @brief Función principal del usuario (versión básica del enunciado)
 */
int main_usuario_basico(void);

// ============================================================================
// MACROS DE INFORMACIÓN DEL COMPILADOR
// ============================================================================

/**
 * @brief Información del compilador y compilación
 */
#define MOSTRAR_INFO_COMPILADOR() do { \
    printf("Compilador: %s\n", __VERSION__); \
    printf("Archivo: %s\n", __FILE__); \
    printf("Fecha compilación: %s\n", __DATE__); \
    printf("Hora compilación: %s\n", __TIME__); \
} while(0)

/**
 * @brief Macro para mostrar el valor de una constante
 */
#define MOSTRAR_CONSTANTE(const) printf(#const " = %d\n", const)

/**
 * @brief Macro para mostrar el valor de una constante de cadena
 */
#define MOSTRAR_CONSTANTE_STR(const) printf(#const " = \"%s\"\n", const)

/**
 * @brief Macro para mostrar el valor de una constante flotante
 */
#define MOSTRAR_CONSTANTE_FLOAT(const) printf(#const " = %.6f\n", const)

#endif // CONSTANTES_PREPROCESADOR_H
