#ifndef SINCRONIZACION_MUTEX_H
#define SINCRONIZACION_MUTEX_H

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @file sincronizacion_mutex.h
 * @brief Ejercicio 085: Sincronización con Mutex
 * 
 * Este ejercicio demuestra el uso de mutex (mutual exclusion) para
 * sincronizar el acceso a recursos compartidos entre hilos.
 * 
 * Conceptos cubiertos:
 * - Mutex básicos (pthread_mutex_t)
 * - Sección crítica
 * - Condiciones de carrera (race conditions)
 * - Deadlock y su prevención
 * - Mutex recursivos
 * - Mutex con timeout
 */

// Estructura para demostrar acceso concurrente a recurso compartido
typedef struct {
    int64_t valor;              // Recurso compartido
    pthread_mutex_t mutex;      // Mutex para proteger el recurso
    bool inicializado;          // Flag de inicialización
} recurso_compartido_t;

// Estructura para estadísticas de concurrencia
typedef struct {
    uint64_t operaciones_lectura;
    uint64_t operaciones_escritura;
    uint64_t colisiones_detectadas;
    uint64_t tiempo_espera_ms;
    pthread_mutex_t mutex_stats;
} estadisticas_mutex_t;

// Estructura para prueba de rendimiento
typedef struct {
    int num_hilos;
    int operaciones_por_hilo;
    recurso_compartido_t* recurso;
    estadisticas_mutex_t* stats;
    bool usar_mutex;  // Para comparar con/sin mutex
} parametros_prueba_t;

// Estructura para demostrar diferentes tipos de mutex
typedef struct {
    pthread_mutex_t mutex_normal;
    pthread_mutex_t mutex_recursivo;
    pthread_mutex_t mutex_errorcheck;
    int contador_recursivo;
    bool inicializado;
} demo_tipos_mutex_t;

/**
 * @brief Inicializa un recurso compartido con mutex
 * @param recurso Puntero al recurso a inicializar
 * @param valor_inicial Valor inicial del recurso
 * @return true si la inicialización fue exitosa
 */
bool inicializar_recurso_compartido(recurso_compartido_t* recurso, int64_t valor_inicial);

/**
 * @brief Destruye un recurso compartido y libera el mutex
 * @param recurso Puntero al recurso a destruir
 * @return true si la destrucción fue exitosa
 */
bool destruir_recurso_compartido(recurso_compartido_t* recurso);

/**
 * @brief Incrementa el valor del recurso de forma thread-safe
 * @param recurso Puntero al recurso compartido
 * @param incremento Valor a incrementar
 * @return true si la operación fue exitosa
 */
bool incrementar_recurso_seguro(recurso_compartido_t* recurso, int64_t incremento);

/**
 * @brief Lee el valor del recurso de forma thread-safe
 * @param recurso Puntero al recurso compartido
 * @param valor Puntero donde almacenar el valor leído
 * @return true si la lectura fue exitosa
 */
bool leer_recurso_seguro(recurso_compartido_t* recurso, int64_t* valor);

/**
 * @brief Establece un nuevo valor en el recurso de forma thread-safe
 * @param recurso Puntero al recurso compartido
 * @param nuevo_valor Nuevo valor a establecer
 * @return true si la operación fue exitosa
 */
bool establecer_recurso_seguro(recurso_compartido_t* recurso, int64_t nuevo_valor);

/**
 * @brief Inicializa estadísticas de mutex
 * @param stats Puntero a las estadísticas
 * @return true si la inicialización fue exitosa
 */
bool inicializar_estadisticas_mutex(estadisticas_mutex_t* stats);

/**
 * @brief Destruye estadísticas de mutex
 * @param stats Puntero a las estadísticas
 * @return true si la destrucción fue exitosa
 */
bool destruir_estadisticas_mutex(estadisticas_mutex_t* stats);

/**
 * @brief Registra una operación de lectura en las estadísticas
 * @param stats Puntero a las estadísticas
 * @param tiempo_espera_ms Tiempo de espera en milisegundos
 */
void registrar_lectura(estadisticas_mutex_t* stats, uint64_t tiempo_espera_ms);

/**
 * @brief Registra una operación de escritura en las estadísticas
 * @param stats Puntero a las estadísticas
 * @param tiempo_espera_ms Tiempo de espera en milisegundos
 */
void registrar_escritura(estadisticas_mutex_t* stats, uint64_t tiempo_espera_ms);

/**
 * @brief Obtiene estadísticas de forma thread-safe
 * @param stats Puntero a las estadísticas fuente
 * @param copia Puntero donde copiar las estadísticas
 * @return true si la copia fue exitosa
 */
bool obtener_estadisticas(estadisticas_mutex_t* stats, estadisticas_mutex_t* copia);

/**
 * @brief Función de hilo para pruebas de concurrencia
 * @param arg Puntero a parametros_prueba_t
 * @return NULL
 */
void* hilo_prueba_concurrencia(void* arg);

/**
 * @brief Ejecuta una prueba de rendimiento con múltiples hilos
 * @param params Parámetros de la prueba
 * @return true si la prueba fue exitosa
 */
bool ejecutar_prueba_rendimiento(parametros_prueba_t* params);

/**
 * @brief Demuestra el problema de condiciones de carrera
 * @param num_hilos Número de hilos a usar
 * @param operaciones Operaciones por hilo
 * @return Valor final del contador (debería mostrar inconsistencias sin mutex)
 */
int64_t demostrar_race_condition(int num_hilos, int operaciones);

/**
 * @brief Inicializa demostración de tipos de mutex
 * @param demo Puntero a la estructura de demostración
 * @return true si la inicialización fue exitosa
 */
bool inicializar_demo_tipos_mutex(demo_tipos_mutex_t* demo);

/**
 * @brief Destruye demostración de tipos de mutex
 * @param demo Puntero a la estructura de demostración
 * @return true si la destrucción fue exitosa
 */
bool destruir_demo_tipos_mutex(demo_tipos_mutex_t* demo);

/**
 * @brief Demuestra el uso de mutex recursivo
 * @param demo Puntero a la estructura de demostración
 * @param nivel Nivel de recursión
 * @return true si la demostración fue exitosa
 */
bool demostrar_mutex_recursivo(demo_tipos_mutex_t* demo, int nivel);

/**
 * @brief Demuestra detección de errores con mutex errorcheck
 * @param demo Puntero a la estructura de demostración
 * @return true si la demostración fue exitosa
 */
bool demostrar_mutex_errorcheck(demo_tipos_mutex_t* demo);

/**
 * @brief Intenta adquirir un mutex con timeout
 * @param mutex Puntero al mutex
 * @param timeout_ms Timeout en milisegundos
 * @return true si se adquirió el mutex dentro del timeout
 */
bool mutex_trylock_timeout(pthread_mutex_t* mutex, int timeout_ms);

/**
 * @brief Función helper para medir tiempo de operaciones
 * @return Tiempo actual en microsegundos
 */
uint64_t obtener_tiempo_microsegundos(void);

/**
 * @brief Imprime estadísticas de rendimiento
 * @param stats Puntero a las estadísticas
 * @param tiempo_total_ms Tiempo total de la prueba
 */
void imprimir_estadisticas_rendimiento(estadisticas_mutex_t* stats, uint64_t tiempo_total_ms);

/**
 * @brief Ejecuta demostración completa de sincronización con mutex
 * @return true si todas las demostraciones fueron exitosas
 */
bool ejecutar_demo_completa_mutex(void);

#endif // SINCRONIZACION_MUTEX_H
