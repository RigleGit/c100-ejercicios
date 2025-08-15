#ifndef COMPILACION_CONDICIONAL_H
#define COMPILACION_CONDICIONAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// ============================================================================
// CONFIGURACIÓN DE COMPILACIÓN CONDICIONAL
// ============================================================================

// Descomenta la siguiente línea para habilitar debug globalmente
// #define DEBUG

// Niveles de debug (pueden combinarse)
// #define DEBUG_VERBOSE    // Información muy detallada
// #define DEBUG_TRACE      // Trazado de funciones
// #define DEBUG_MEMORY     // Información de memoria
// #define DEBUG_TIMING     // Medición de tiempos
// #define DEBUG_ERROR      // Información extendida de errores

// Configuración de características
// #define FEATURE_LOGGING   // Habilitar sistema de logging
// #define FEATURE_STATS     // Habilitar estadísticas
// #define FEATURE_PROFILING // Habilitar profiling
// #define FEATURE_TESTS     // Habilitar funciones de test

// Configuración de versión
#ifndef VERSION_MAYOR
    #define VERSION_MAYOR 1
#endif

#ifndef VERSION_MENOR
    #define VERSION_MENOR 0
#endif

#ifndef VERSION_PATCH
    #define VERSION_PATCH 0
#endif

// ============================================================================
// MACROS DE DEBUG BÁSICAS
// ============================================================================

#ifdef DEBUG
    #define DEBUG_PRINT(msg) printf("[DEBUG] %s\n", (msg))
    #define DEBUG_PRINTF(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
    #define DEBUG_VAR(var) printf("[DEBUG] %s = %d\n", #var, (var))
    #define DEBUG_FLOAT(var) printf("[DEBUG] %s = %.2f\n", #var, (var))
    #define DEBUG_STRING(var) printf("[DEBUG] %s = \"%s\"\n", #var, (var))
    #define DEBUG_FUNC_ENTER(func) printf("[DEBUG] Entrando en %s\n", (func))
    #define DEBUG_FUNC_EXIT(func) printf("[DEBUG] Saliendo de %s\n", (func))
#else
    #define DEBUG_PRINT(msg)
    #define DEBUG_PRINTF(fmt, ...)
    #define DEBUG_VAR(var)
    #define DEBUG_FLOAT(var)
    #define DEBUG_STRING(var)
    #define DEBUG_FUNC_ENTER(func)
    #define DEBUG_FUNC_EXIT(func)
#endif

// ============================================================================
// MACROS DE DEBUG AVANZADAS
// ============================================================================

#ifdef DEBUG_VERBOSE
    #define VERBOSE_PRINT(msg) printf("[VERBOSE] %s\n", (msg))
    #define VERBOSE_PRINTF(fmt, ...) printf("[VERBOSE] " fmt "\n", ##__VA_ARGS__)
    #define VERBOSE_STRING(var) printf("[VERBOSE] %s = \"%s\"\n", #var, (var))
#else
    #define VERBOSE_PRINT(msg)
    #define VERBOSE_PRINTF(fmt, ...)
    #define VERBOSE_STRING(var)
#endif

#ifdef DEBUG_TRACE
    #define TRACE_ENTER() printf("[TRACE] Entrando en %s:%d\n", __func__, __LINE__)
    #define TRACE_EXIT() printf("[TRACE] Saliendo de %s:%d\n", __func__, __LINE__)
    #define TRACE_POINT() printf("[TRACE] Punto de control en %s:%d\n", __func__, __LINE__)
#else
    #define TRACE_ENTER()
    #define TRACE_EXIT()
    #define TRACE_POINT()
#endif

#ifdef DEBUG_MEMORY
    #define MEM_PRINT(ptr, size) printf("[MEMORY] Asignado %zu bytes en %p\n", (size), (ptr))
    #define MEM_FREE_PRINT(ptr) printf("[MEMORY] Liberando memoria en %p\n", (ptr))
#else
    #define MEM_PRINT(ptr, size)
    #define MEM_FREE_PRINT(ptr)
#endif

#ifdef DEBUG_TIMING
    #define TIME_START(name) \
        clock_t _time_start_##name = clock(); \
        printf("[TIMING] Iniciando %s\n", #name)
    
    #define TIME_END(name) \
        clock_t _time_end_##name = clock(); \
        double _time_diff_##name = ((double)(_time_end_##name - _time_start_##name)) / CLOCKS_PER_SEC; \
        printf("[TIMING] %s completado en %.6f segundos\n", #name, _time_diff_##name)
#else
    #define TIME_START(name)
    #define TIME_END(name)
#endif

#ifdef DEBUG_ERROR
    #define ERROR_PRINT(msg) printf("[ERROR] %s (archivo: %s, línea: %d)\n", (msg), __FILE__, __LINE__)
    #define ERROR_ERRNO() printf("[ERROR] %s (errno: %d)\n", strerror(errno), errno)
#else
    #define ERROR_PRINT(msg) printf("[ERROR] %s\n", (msg))
    #define ERROR_ERRNO()
#endif

// ============================================================================
// MACROS DE CARACTERÍSTICAS OPCIONALES
// ============================================================================

#ifdef FEATURE_LOGGING
    #define LOG_INFO(msg) printf("[INFO] %s\n", (msg))
    #define LOG_WARNING(msg) printf("[WARNING] %s\n", (msg))
    #define LOG_ERROR(msg) printf("[ERROR] %s\n", (msg))
#else
    #define LOG_INFO(msg)
    #define LOG_WARNING(msg)
    #define LOG_ERROR(msg)
#endif

#ifdef FEATURE_STATS
    extern int operaciones_realizadas;
    extern int memoria_total_asignada;
    
    #define STATS_INC_OPS() (operaciones_realizadas++)
    #define STATS_ADD_MEM(size) (memoria_total_asignada += (size))
    #define STATS_PRINT() printf("[STATS] Operaciones: %d, Memoria: %d bytes\n", \
                                operaciones_realizadas, memoria_total_asignada)
#else
    #define STATS_INC_OPS()
    #define STATS_ADD_MEM(size)
    #define STATS_PRINT()
#endif

#ifdef FEATURE_PROFILING
    #define PROFILE_FUNC() printf("[PROFILE] Función %s llamada\n", __func__)
#else
    #define PROFILE_FUNC()
#endif

// ============================================================================
// CONFIGURACIÓN CONDICIONAL DE PLATAFORMA
// ============================================================================

#ifdef __linux__
    #define PLATFORM_NAME "Linux"
    #define PATH_SEPARATOR "/"
#elif defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_NAME "Windows"
    #define PATH_SEPARATOR "\\"
#elif defined(__APPLE__)
    #define PLATFORM_NAME "macOS"
    #define PATH_SEPARATOR "/"
#else
    #define PLATFORM_NAME "Desconocido"
    #define PATH_SEPARATOR "/"
#endif

// ============================================================================
// CONFIGURACIÓN CONDICIONAL DE COMPILADOR
// ============================================================================

#ifdef __GNUC__
    #define COMPILER_NAME "GCC"
    #define COMPILER_VERSION __GNUC__
#elif defined(__clang__)
    #define COMPILER_NAME "Clang"
    #define COMPILER_VERSION __clang_major__
#elif defined(_MSC_VER)
    #define COMPILER_NAME "MSVC"
    #define COMPILER_VERSION _MSC_VER
#else
    #define COMPILER_NAME "Desconocido"
    #define COMPILER_VERSION 0
#endif

// ============================================================================
// MACROS DE ASERCIONES CONDICIONALES
// ============================================================================

#ifdef DEBUG
    #define ASSERT(condition) \
        if (!(condition)) { \
            printf("[ASSERT] Falla en %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            exit(1); \
        }
#else
    #define ASSERT(condition)
#endif

#ifdef DEBUG
    #define ASSERT_NOT_NULL(ptr) \
        if ((ptr) == NULL) { \
            printf("[ASSERT] Puntero nulo en %s:%d: %s\n", __FILE__, __LINE__, #ptr); \
            exit(1); \
        }
#else
    #define ASSERT_NOT_NULL(ptr)
#endif

// ============================================================================
// CONFIGURACIÓN DE MODO DE COMPILACIÓN
// ============================================================================

#ifdef NDEBUG
    #define MODO_COMPILACION "Release"
    #define OPTIMIZACIONES_HABILITADAS 1
#else
    #define MODO_COMPILACION "Debug"
    #define OPTIMIZACIONES_HABILITADAS 0
#endif

// ============================================================================
// ESTRUCTURAS Y TIPOS CONDICIONALES
// ============================================================================

#ifdef FEATURE_STATS
typedef struct {
    int operaciones_totales;
    int errores_detectados;
    int memoria_maxima_usada;
    double tiempo_total_ejecucion;
} estadisticas_t;
#endif

#ifdef FEATURE_LOGGING
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3
} log_level_t;
#endif

// ============================================================================
// DECLARACIONES DE FUNCIONES
// ============================================================================

/**
 * @brief Función principal del usuario (ejemplo del enunciado)
 */
int main_usuario_basico(void);

/**
 * @brief Procesa datos con debug condicional (del enunciado)
 */
void procesar_datos(int x);

/**
 * @brief Demuestra diferentes niveles de debug
 */
void demo_niveles_debug(void);

/**
 * @brief Demuestra características opcionales
 */
void demo_caracteristicas_opcionales(void);

/**
 * @brief Demuestra compilación condicional por plataforma
 */
void demo_plataforma(void);

/**
 * @brief Demuestra información del compilador
 */
void demo_compilador(void);

/**
 * @brief Demuestra aserciones condicionales
 */
void demo_aserciones(void);

/**
 * @brief Simula una operación que puede fallar
 */
int operacion_que_puede_fallar(int entrada);

/**
 * @brief Función con logging detallado
 */
void funcion_con_logging(const char* mensaje);

/**
 * @brief Función con medición de tiempo
 */
void funcion_con_timing(int iteraciones);

/**
 * @brief Función con gestión de memoria instrumentada
 */
void* asignar_memoria_instrumentada(size_t size);

/**
 * @brief Libera memoria instrumentada
 */
void liberar_memoria_instrumentada(void* ptr);

/**
 * @brief Muestra información de configuración
 */
void mostrar_info_configuracion(void);

/**
 * @brief Muestra estadísticas si están habilitadas
 */
void mostrar_estadisticas(void);

/**
 * @brief Ejecuta todas las demostraciones
 */
void ejecutar_todas_demos(void);

/**
 * @brief Ejemplo de función con múltiples niveles de debug
 */
void funcion_compleja_con_debug(int param1, const char* param2);

/**
 * @brief Simula procesamiento de archivo con debug
 */
int procesar_archivo_simulado(const char* nombre_archivo);

// ============================================================================
// MACROS DE INFORMACIÓN ÚTIL
// ============================================================================

#define MOSTRAR_INFO_COMPILACION() do { \
    printf("Información de compilación:\n"); \
    printf("  Plataforma: %s\n", PLATFORM_NAME); \
    printf("  Compilador: %s\n", COMPILER_NAME); \
    printf("  Modo: %s\n", MODO_COMPILACION); \
    printf("  Versión: %d.%d.%d\n", VERSION_MAYOR, VERSION_MENOR, VERSION_PATCH); \
    printf("  Fecha: %s %s\n", __DATE__, __TIME__); \
} while(0)

/* Estados como cadenas para uso en printf sin directivas dentro de macros */
#ifdef DEBUG
#define DEBUG_STATE "Habilitado"
#else
#define DEBUG_STATE "Deshabilitado"
#endif

#ifdef DEBUG_VERBOSE
#define DEBUG_VERBOSE_STATE "Habilitado"
#else
#define DEBUG_VERBOSE_STATE "Deshabilitado"
#endif

#ifdef DEBUG_TRACE
#define DEBUG_TRACE_STATE "Habilitado"
#else
#define DEBUG_TRACE_STATE "Deshabilitado"
#endif

#ifdef DEBUG_MEMORY
#define DEBUG_MEMORY_STATE "Habilitado"
#else
#define DEBUG_MEMORY_STATE "Deshabilitado"
#endif

#ifdef DEBUG_TIMING
#define DEBUG_TIMING_STATE "Habilitado"
#else
#define DEBUG_TIMING_STATE "Deshabilitado"
#endif

#define MOSTRAR_CONFIGURACION_DEBUG() do { \
    printf("Configuración de debug:\n"); \
    printf("  DEBUG: %s\n", DEBUG_STATE); \
    printf("  DEBUG_VERBOSE: %s\n", DEBUG_VERBOSE_STATE); \
    printf("  DEBUG_TRACE: %s\n", DEBUG_TRACE_STATE); \
    printf("  DEBUG_MEMORY: %s\n", DEBUG_MEMORY_STATE); \
    printf("  DEBUG_TIMING: %s\n", DEBUG_TIMING_STATE); \
} while(0)

#endif // COMPILACION_CONDICIONAL_H
