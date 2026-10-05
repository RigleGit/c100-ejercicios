/**
 * @file entrada_segura.h
 * @brief Funciones para entrada de datos segura en C
 * @version 1.0
 * @date 2025-08-05
 * 
 * Este header proporciona alternativas seguras a funciones peligrosas como gets(),
 * scanf() sin límites, y otras funciones que pueden causar buffer overflow.
 * Implementa buenas prácticas de seguridad para entrada de datos.
 */

#ifndef ENTRADA_SEGURA_H
#define ENTRADA_SEGURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/**
 * @brief Configuración por defecto
 */
#define ENTRADA_SEGURA_VERSION "1.0"
#define BUFFER_SIZE_DEFAULT 256
#define BUFFER_SIZE_SMALL 64
#define BUFFER_SIZE_LARGE 1024
#define MAX_REINTENTOS 3

/**
 * @brief Códigos de error
 */
typedef enum {
    ENTRADA_OK = 0,
    ENTRADA_ERROR_BUFFER_OVERFLOW = -1,
    ENTRADA_ERROR_EOF = -2,
    ENTRADA_ERROR_FORMATO = -3,
    ENTRADA_ERROR_RANGO = -4,
    ENTRADA_ERROR_MEMORIA = -5,
    ENTRADA_ERROR_PARAMETRO = -6,
    ENTRADA_ERROR_TIMEOUT = -7,
    ENTRADA_ERROR_DEMASIADOS_INTENTOS = -8
} resultado_entrada_t;

/**
 * @brief Configuración para entrada de datos
 */
typedef struct {
    size_t buffer_size;
    int max_reintentos;
    bool trim_whitespace;
    bool permitir_vacio;
    bool mostrar_errores;
    char prompt[128];
    char error_message[256];
} config_entrada_t;

/**
 * @brief Estadísticas de entrada
 */
typedef struct {
    int intentos_realizados;
    int errores_overflow;
    int errores_formato;
    int errores_rango;
    size_t caracteres_leidos;
    double tiempo_total;
} stats_entrada_t;

/* ================================
 * FUNCIONES SEGURAS DE ENTRADA
 * ================================ */

/**
 * @brief Alternativa segura a gets() usando fgets()
 * @param buffer Buffer donde almacenar la entrada
 * @param size Tamaño del buffer
 * @param stream Stream de entrada (normalmente stdin)
 * @return Código de resultado
 */
resultado_entrada_t entrada_cadena_segura(char *buffer, size_t size, FILE *stream);

/**
 * @brief Lee una línea de texto de forma segura con configuración
 * @param buffer Buffer donde almacenar la entrada
 * @param size Tamaño del buffer
 * @param config Configuración de entrada
 * @param stats Estadísticas (puede ser NULL)
 * @return Código de resultado
 */
resultado_entrada_t entrada_linea_configurada(char *buffer, size_t size, 
                                              const config_entrada_t *config,
                                              stats_entrada_t *stats);

/**
 * @brief Lee un entero de forma segura
 * @param valor Puntero donde almacenar el valor
 * @param min Valor mínimo permitido
 * @param max Valor máximo permitido
 * @param prompt Mensaje para mostrar al usuario
 * @return Código de resultado
 */
resultado_entrada_t entrada_entero_seguro(int *valor, int min, int max, const char *prompt);

/**
 * @brief Lee un número flotante de forma segura
 * @param valor Puntero donde almacenar el valor
 * @param min Valor mínimo permitido
 * @param max Valor máximo permitido
 * @param prompt Mensaje para mostrar al usuario
 * @return Código de resultado
 */
resultado_entrada_t entrada_flotante_seguro(double *valor, double min, double max, const char *prompt);

/**
 * @brief Lee un carácter de forma segura
 * @param caracter Puntero donde almacenar el carácter
 * @param validos Cadena con caracteres válidos (NULL para cualquiera)
 * @param prompt Mensaje para mostrar al usuario
 * @return Código de resultado
 */
resultado_entrada_t entrada_caracter_seguro(char *caracter, const char *validos, const char *prompt);

/**
 * @brief Lee una respuesta sí/no de forma segura
 * @param respuesta Puntero donde almacenar true (sí) o false (no)
 * @param prompt Mensaje para mostrar al usuario
 * @return Código de resultado
 */
resultado_entrada_t entrada_si_no(bool *respuesta, const char *prompt);

/* ================================
 * FUNCIONES DE VALIDACIÓN
 * ================================ */

/**
 * @brief Valida que una cadena solo contenga caracteres alfabéticos
 * @param cadena Cadena a validar
 * @return true si es válida, false en caso contrario
 */
bool validar_solo_letras(const char *cadena);

/**
 * @brief Valida que una cadena solo contenga dígitos
 * @param cadena Cadena a validar
 * @return true si es válida, false en caso contrario
 */
bool validar_solo_numeros(const char *cadena);

/**
 * @brief Valida que una cadena sea alfanumérica
 * @param cadena Cadena a validar
 * @return true si es válida, false en caso contrario
 */
bool validar_alfanumerico(const char *cadena);

/**
 * @brief Valida formato de email básico
 * @param email Cadena a validar
 * @return true si tiene formato válido, false en caso contrario
 */
bool validar_email(const char *email);

/**
 * @brief Valida longitud de cadena
 * @param cadena Cadena a validar
 * @param min_len Longitud mínima
 * @param max_len Longitud máxima
 * @return true si está en el rango, false en caso contrario
 */
bool validar_longitud(const char *cadena, size_t min_len, size_t max_len);

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

/**
 * @brief Limpia el buffer de entrada (elimina caracteres residuales)
 * @param stream Stream a limpiar
 */
void limpiar_buffer_entrada(FILE *stream);

/**
 * @brief Elimina espacios en blanco al inicio y final de una cadena
 * @param cadena Cadena a limpiar (se modifica in-place)
 * @return Puntero al inicio de la cadena limpia
 */
char* trim_whitespace(char *cadena);

/**
 * @brief Convierte una cadena a minúsculas
 * @param cadena Cadena a convertir (se modifica in-place)
 * @return Puntero a la cadena convertida
 */
char* a_minusculas(char *cadena);

/**
 * @brief Convierte una cadena a mayúsculas
 * @param cadena Cadena a convertir (se modifica in-place)
 * @return Puntero a la cadena convertida
 */
char* a_mayusculas(char *cadena);

/**
 * @brief Convierte código de error a mensaje descriptivo
 * @param resultado Código de error
 * @return Mensaje descriptivo del error
 */
const char* entrada_error_string(resultado_entrada_t resultado);

/* ================================
 * FUNCIONES DE CONFIGURACIÓN
 * ================================ */

/**
 * @brief Inicializa configuración por defecto
 * @param config Estructura de configuración
 */
void entrada_init_config(config_entrada_t *config);

/**
 * @brief Inicializa estadísticas
 * @param stats Estructura de estadísticas
 */
void entrada_init_stats(stats_entrada_t *stats);

/**
 * @brief Imprime estadísticas de entrada
 * @param stats Estadísticas a imprimir
 */
void entrada_imprimir_stats(const stats_entrada_t *stats);

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

/**
 * @brief Demostración de entrada insegura vs segura
 * @return Código de resultado
 */
int demo_comparacion_seguridad(void);

/**
 * @brief Demostración de diferentes tipos de entrada
 * @return Código de resultado
 */
int demo_tipos_entrada(void);

/**
 * @brief Demostración de validación de entrada
 * @return Código de resultado
 */
int demo_validacion(void);

/**
 * @brief Demostración de configuración avanzada
 * @return Código de resultado
 */
int demo_configuracion_avanzada(void);

/**
 * @brief Prueba de stress con entradas grandes
 * @return Código de resultado
 */
int demo_stress_test(void);

/* ================================
 * FUNCIONES EDUCATIVAS
 * ================================ */

/**
 * @brief Muestra ejemplos de vulnerabilidades comunes
 */
void mostrar_vulnerabilidades_comunes(void);

/**
 * @brief Muestra buenas prácticas de seguridad
 */
void mostrar_buenas_practicas(void);

/**
 * @brief Compara funciones inseguras vs seguras
 */
void comparar_funciones_seguridad(void);

/**
 * @brief Tutorial interactivo sobre entrada segura
 * @return Código de resultado
 */
int tutorial_entrada_segura(void);

/* ================================
 * MACROS Y CONSTANTES DE SEGURIDAD
 * ================================ */

// Macro para entrada segura de cadenas
#define ENTRADA_SEGURA(buffer) \
    entrada_cadena_segura(buffer, sizeof(buffer), stdin)

// Macro para entrada con prompt
#define ENTRADA_CON_PROMPT(buffer, prompt) do { \
    printf("%s", prompt); \
    fflush(stdout); \
    entrada_cadena_segura(buffer, sizeof(buffer), stdin); \
} while(0)

// Macro para validar resultado
#define VERIFICAR_ENTRADA(resultado) do { \
    if (resultado != ENTRADA_OK) { \
        fprintf(stderr, "Error de entrada: %s\n", entrada_error_string(resultado)); \
        return resultado; \
    } \
} while(0)

/* ================================
 * CONFIGURACIÓN DE DEBUG
 * ================================ */

#ifdef DEBUG
    #define ENTRADA_DEBUG(fmt, ...) \
        fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define ENTRADA_DEBUG(fmt, ...)
#endif

#define ENTRADA_LOG(fmt, ...) \
    fprintf(stdout, "[ENTRADA] " fmt "\n", ##__VA_ARGS__)

#define ENTRADA_ERROR(fmt, ...) \
    fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

#define ENTRADA_WARNING(fmt, ...) \
    fprintf(stderr, "[WARNING] " fmt "\n", ##__VA_ARGS__)

/* ================================
 * FUNCIONES WRAPPER SEGURAS
 * ================================ */

/**
 * @brief Wrapper seguro para scanf
 * @param formato Formato de scanf
 * @param buffer Buffer donde almacenar
 * @param size Tamaño del buffer
 * @return Número de elementos leídos o -1 en error
 */
int scanf_seguro(const char *formato, void *buffer, size_t size);

/**
 * @brief Wrapper seguro para gets (reemplazado por fgets)
 * @param buffer Buffer donde almacenar
 * @param size Tamaño del buffer
 * @return Puntero al buffer o NULL en error
 */
char* gets_seguro(char *buffer, size_t size);

/**
 * @brief Copia segura de cadenas (alternativa a strcpy)
 * @param destino Buffer destino
 * @param origen Cadena origen
 * @param size Tamaño del buffer destino
 * @return Código de resultado
 */
resultado_entrada_t strcpy_seguro(char *destino, const char *origen, size_t size);

/**
 * @brief Concatenación segura de cadenas (alternativa a strcat)
 * @param destino Buffer destino
 * @param origen Cadena a concatenar
 * @param size Tamaño del buffer destino
 * @return Código de resultado
 */
resultado_entrada_t strcat_seguro(char *destino, const char *origen, size_t size);

/* ================================
 * CONSTANTES ÚTILES
 * ================================ */

#define MAX_NOMBRE_LEN 64
#define MAX_EMAIL_LEN 256
#define MAX_PASSWORD_LEN 128
#define MAX_TELEFONO_LEN 32
#define MAX_DIRECCION_LEN 512

// Caracteres especiales comunes
#define CARACTERES_NUMERICOS "0123456789"
#define CARACTERES_ALFABETICOS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CARACTERES_ALFANUMERICOS CARACTERES_ALFABETICOS CARACTERES_NUMERICOS
#define CARACTERES_SI_NO "SsNn"
#define CARACTERES_PUNTUACION ".,;:!?-_"

// Helper para inicialización segura
#define INIT_BUFFER(buffer) memset(buffer, 0, sizeof(buffer))

#endif // ENTRADA_SEGURA_H
