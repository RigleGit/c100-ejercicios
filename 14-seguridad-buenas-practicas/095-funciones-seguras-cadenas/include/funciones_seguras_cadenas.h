/**
 * @file funciones_seguras_cadenas.h
 * @brief Funciones seguras para manipulación de cadenas de caracteres
 * @version 1.0
 * @date 2025-08-05
 * 
 * Este ejercicio demuestra el uso seguro de funciones de manipulación de cadenas,
 * reemplazando funciones inseguras (strcpy, strcat, sprintf) por sus versiones
 * seguras (strncpy, strncat, snprintf) para prevenir buffer overflows.
 */

#ifndef FUNCIONES_SEGURAS_CADENAS_H
#define FUNCIONES_SEGURAS_CADENAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

/**
 * @brief Configuración y constantes
 */
#define FUNCIONES_SEGURAS_VERSION "1.0"
#define BUFFER_PEQUEÑO 32
#define BUFFER_MEDIANO 128
#define BUFFER_GRANDE 512
#define BUFFER_MAXIMO 2048
#define MAX_NOMBRE 64
#define MAX_MENSAJE 256

/**
 * @brief Códigos de error para operaciones de cadenas
 */
typedef enum {
    CADENA_OK = 0,
    CADENA_ERROR_PARAMETRO_NULO = -1,
    CADENA_ERROR_BUFFER_PEQUEÑO = -2,
    CADENA_ERROR_CADENA_TRUNCADA = -3,
    CADENA_ERROR_FORMATO_INVALIDO = -4,
    CADENA_ERROR_MEMORIA = -5,
    CADENA_ERROR_LONGITUD_INVALIDA = -6
} resultado_cadena_t;

/**
 * @brief Información sobre el resultado de una operación
 */
typedef struct {
    resultado_cadena_t codigo;
    size_t bytes_usados;
    size_t bytes_disponibles;
    bool truncado;
    char mensaje_error[128];
} info_operacion_t;

/**
 * @brief Estructura para validación de buffers
 */
typedef struct {
    void *buffer;
    size_t tamaño;
    size_t usado;
    bool es_valido;
    char nombre[32];
} info_buffer_t;

/* ================================
 * FUNCIONES SEGURAS DE COPIA
 * ================================ */

/**
 * @brief Copia segura de cadenas usando strncpy con terminación garantizada
 * @param destino Buffer de destino
 * @param origen Cadena origen
 * @param tamaño_destino Tamaño del buffer de destino
 * @param info Información sobre la operación (puede ser NULL)
 * @return Código de resultado
 */
resultado_cadena_t copia_segura(char *destino, const char *origen, 
                                size_t tamaño_destino, info_operacion_t *info);

/**
 * @brief Copia segura con validación de parámetros
 * @param destino Buffer de destino
 * @param origen Cadena origen
 * @param tamaño_destino Tamaño del buffer de destino
 * @return Código de resultado
 */
resultado_cadena_t copia_segura_validada(char *destino, const char *origen, 
                                         size_t tamaño_destino);

/**
 * @brief Copia de una subcadena de forma segura
 * @param destino Buffer de destino
 * @param origen Cadena origen
 * @param inicio Posición de inicio en origen
 * @param longitud Número de caracteres a copiar
 * @param tamaño_destino Tamaño del buffer de destino
 * @return Código de resultado
 */
resultado_cadena_t copia_subcadena_segura(char *destino, const char *origen,
                                          size_t inicio, size_t longitud,
                                          size_t tamaño_destino);

/* ================================
 * FUNCIONES SEGURAS DE CONCATENACIÓN
 * ================================ */

/**
 * @brief Concatenación segura usando strncat
 * @param destino Buffer de destino (debe contener una cadena válida)
 * @param origen Cadena a concatenar
 * @param tamaño_destino Tamaño total del buffer de destino
 * @param info Información sobre la operación (puede ser NULL)
 * @return Código de resultado
 */
resultado_cadena_t concatena_segura(char *destino, const char *origen,
                                    size_t tamaño_destino, info_operacion_t *info);

/**
 * @brief Concatenación múltiple segura (varargs)
 * @param destino Buffer de destino
 * @param tamaño_destino Tamaño del buffer de destino
 * @param num_cadenas Número de cadenas a concatenar
 * @param ... Cadenas a concatenar (const char*)
 * @return Código de resultado
 */
resultado_cadena_t concatena_multiple_segura(char *destino, size_t tamaño_destino,
                                            int num_cadenas, ...);

/**
 * @brief Concatenación con separador
 * @param destino Buffer de destino
 * @param origen Cadena a concatenar
 * @param separador Separador a usar
 * @param tamaño_destino Tamaño del buffer de destino
 * @return Código de resultado
 */
resultado_cadena_t concatena_con_separador(char *destino, const char *origen,
                                           const char *separador,
                                           size_t tamaño_destino);

/* ================================
 * FUNCIONES SEGURAS DE FORMATEO
 * ================================ */

/**
 * @brief Formateo seguro usando snprintf
 * @param destino Buffer de destino
 * @param tamaño_destino Tamaño del buffer de destino
 * @param formato Cadena de formato
 * @param ... Argumentos del formato
 * @return Código de resultado
 */
resultado_cadena_t formatea_seguro(char *destino, size_t tamaño_destino,
                                   const char *formato, ...);

/**
 * @brief Formateo seguro con validación de argumentos
 * @param destino Buffer de destino
 * @param tamaño_destino Tamaño del buffer de destino
 * @param formato Cadena de formato
 * @param args Lista de argumentos
 * @return Código de resultado
 */
resultado_cadena_t vformatea_seguro(char *destino, size_t tamaño_destino,
                                    const char *formato, va_list args);

/**
 * @brief Construcción de ruta de archivo de forma segura
 * @param destino Buffer para la ruta resultante
 * @param tamaño_destino Tamaño del buffer
 * @param directorio Directorio base
 * @param archivo Nombre del archivo
 * @return Código de resultado
 */
resultado_cadena_t construye_ruta_segura(char *destino, size_t tamaño_destino,
                                         const char *directorio, const char *archivo);

/* ================================
 * FUNCIONES DE VALIDACIÓN Y UTILIDAD
 * ================================ */

/**
 * @brief Valida que un buffer sea seguro para operaciones de cadena
 * @param buffer Puntero al buffer
 * @param tamaño Tamaño declarado del buffer
 * @param info Información sobre el buffer (puede ser NULL)
 * @return true si el buffer es válido, false en caso contrario
 */
bool validar_buffer_seguro(const void *buffer, size_t tamaño, info_buffer_t *info);

/**
 * @brief Calcula el espacio restante en un buffer de cadena
 * @param cadena Cadena existente en el buffer
 * @param tamaño_buffer Tamaño total del buffer
 * @return Bytes disponibles para más contenido
 */
size_t espacio_restante_buffer(const char *cadena, size_t tamaño_buffer);

/**
 * @brief Verifica si una cadena puede caber en un buffer
 * @param cadena Cadena a verificar
 * @param tamaño_buffer Tamaño del buffer destino
 * @return true si cabe, false en caso contrario
 */
bool cadena_cabe_en_buffer(const char *cadena, size_t tamaño_buffer);

/**
 * @brief Trunca una cadena de forma segura
 * @param cadena Cadena a truncar (modificada in-place)
 * @param longitud_maxima Longitud máxima permitida
 * @return Código de resultado
 */
resultado_cadena_t truncar_cadena_segura(char *cadena, size_t longitud_maxima);

/* ================================
 * FUNCIONES DE COMPARACIÓN AVANZADA
 * ================================ */

/**
 * @brief Comparación segura de cadenas con límite
 * @param cadena1 Primera cadena
 * @param cadena2 Segunda cadena
 * @param max_longitud Máximo número de caracteres a comparar
 * @return 0 si son iguales, <0 si cadena1 < cadena2, >0 si cadena1 > cadena2
 */
int compara_seguro(const char *cadena1, const char *cadena2, size_t max_longitud);

/**
 * @brief Búsqueda segura de subcadena
 * @param cadena_principal Cadena donde buscar
 * @param subcadena Subcadena a buscar
 * @param max_longitud Máximo número de caracteres a examinar
 * @return Puntero a la primera ocurrencia o NULL si no se encuentra
 */
const char* buscar_subcadena_segura(const char *cadena_principal, 
                                   const char *subcadena,
                                   size_t max_longitud);

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN Y EJEMPLOS
 * ================================ */

/**
 * @brief Demuestra el problema con funciones inseguras
 * @return Código de resultado
 */
int demo_funciones_inseguras(void);

/**
 * @brief Demuestra el uso de funciones seguras
 * @return Código de resultado
 */
int demo_funciones_seguras(void);

/**
 * @brief Demuestra operaciones complejas con cadenas seguras
 * @return Código de resultado
 */
int demo_operaciones_complejas(void);

/**
 * @brief Benchmark comparando funciones seguras vs inseguras
 * @param num_iteraciones Número de iteraciones para el benchmark
 * @return Código de resultado
 */
int benchmark_funciones_cadenas(int num_iteraciones);

/**
 * @brief Prueba de estrés para detectar vulnerabilidades
 * @return Código de resultado
 */
int prueba_estres_seguridad(void);

/* ================================
 * FUNCIONES DE UTILIDAD Y CONVERSIÓN
 * ================================ */

/**
 * @brief Convierte código de resultado a cadena descriptiva
 * @param codigo Código de resultado
 * @return Descripción del código
 */
const char* resultado_cadena_string(resultado_cadena_t codigo);

/**
 * @brief Imprime información detallada de una operación
 * @param info Información de la operación
 */
void imprimir_info_operacion(const info_operacion_t *info);

/**
 * @brief Imprime información de un buffer
 * @param info Información del buffer
 */
void imprimir_info_buffer(const info_buffer_t *info);

/**
 * @brief Inicializa estructura de información de operación
 * @param info Estructura a inicializar
 */
void init_info_operacion(info_operacion_t *info);

/**
 * @brief Inicializa estructura de información de buffer
 * @param info Estructura a inicializar
 * @param buffer Puntero al buffer
 * @param tamaño Tamaño del buffer
 * @param nombre Nombre descriptivo del buffer
 */
void init_info_buffer(info_buffer_t *info, void *buffer, size_t tamaño, const char *nombre);

/* ================================
 * MACROS DE SEGURIDAD
 * ================================ */

/**
 * @brief Copia segura con verificación en tiempo de compilación
 */
#define COPIA_SEGURA(destino, origen) \
    copia_segura((destino), (origen), sizeof(destino), NULL)

/**
 * @brief Concatenación segura con verificación en tiempo de compilación
 */
#define CONCATENA_SEGURA(destino, origen) \
    concatena_segura((destino), (origen), sizeof(destino), NULL)

/**
 * @brief Formateo seguro con verificación en tiempo de compilación
 */
#define FORMATEA_SEGURO(destino, formato, ...) \
    formatea_seguro((destino), sizeof(destino), (formato), ##__VA_ARGS__)

/**
 * @brief Verifica que un buffer sea válido
 */
#define VERIFICAR_BUFFER(buffer) \
    assert((buffer) != NULL && "Buffer no puede ser NULL")

/**
 * @brief Verifica que el tamaño sea válido
 */
#define VERIFICAR_TAMAÑO(tamaño) \
    assert((tamaño) > 0 && "Tamaño debe ser mayor que 0")

/* ================================
 * CONFIGURACIÓN DE COMPILACIÓN
 * ================================ */

#ifdef DEBUG
    #define CADENAS_DEBUG(fmt, ...) \
        fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define CADENAS_DEBUG(fmt, ...)
#endif

#define CADENAS_LOG(fmt, ...) \
    fprintf(stdout, "[CADENAS] " fmt "\n", ##__VA_ARGS__)

#define CADENAS_ERROR(fmt, ...) \
    fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

#define CADENAS_WARNING(fmt, ...) \
    fprintf(stderr, "[WARNING] " fmt "\n", ##__VA_ARGS__)

/* ================================
 * CONSTANTES DE CONFIGURACIÓN
 * ================================ */

// Límites de seguridad
#define MAX_LONGITUD_CADENA 4096
#define MAX_ARGUMENTOS_FORMATO 32
#define MAX_PROFUNDIDAD_CONCATENACION 16

// Caracteres especiales
#define TERMINADOR_CADENA '\0'
#define SEPARADOR_RUTA '/'
#define SEPARADOR_RUTA_WIN '\\'

// Helper macros
#define CADENAS_SAFE_FREE(ptr) do { \
    if (ptr) { \
        free(ptr); \
        ptr = NULL; \
    } \
} while(0)

#define CADENAS_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CADENAS_MAX(a, b) ((a) > (b) ? (a) : (b))

/* ================================
 * API COMPATIBLE CON TESTS
 * ================================ */
int strncpy_seguro(char *destino, const char *origen, size_t tamaño_destino);
int strncat_seguro(char *destino, const char *origen, size_t tamaño_destino);
int snprintf_seguro(char *destino, size_t tamaño_destino, const char *formato, ...);
int demostracion_completa(char *salida, size_t tamaño_salida, const char *nombre, int edad);

#endif // FUNCIONES_SEGURAS_CADENAS_H
