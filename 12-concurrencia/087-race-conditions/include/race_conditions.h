#ifndef RACE_CONDITIONS_H
#define RACE_CONDITIONS_H

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/time.h>

/**
 * @file race_conditions.h
 * @brief Ejercicio 087: Demostración de Race Conditions
 * 
 * Este ejercicio demuestra las condiciones de carrera (race conditions)
 * que ocurren cuando múltiples hilos acceden concurrentemente a recursos
 * compartidos sin sincronización adecuada.
 * 
 * Conceptos cubiertos:
 * - Race conditions y sus efectos
 * - Variables compartidas sin protección
 * - Resultados no determinísticos
 * - Análisis estadístico de inconsistencias
 * - Comparación con versiones sincronizadas
 * - Detección y medición de race conditions
 */

// Configuración por defecto
#define NUM_INCREMENTOS_DEFAULT 100000
#define NUM_HILOS_DEFAULT 2
#define NUM_EXPERIMENTOS_DEFAULT 10

// Estructura para parámetros de un hilo incrementador
typedef struct {
    int* contador;              // Puntero al contador compartido
    int incrementos;            // Número de incrementos a realizar
    int id_hilo;                // Identificador del hilo
    uint64_t tiempo_inicio;     // Tiempo de inicio en microsegundos
    uint64_t tiempo_fin;        // Tiempo de finalización en microsegundos
    bool usar_delay;            // Si añadir delay para aumentar race conditions
    int delay_microsegundos;    // Microsegundos de delay entre incrementos
} parametros_incremento_t;

// Estructura para configuración de experimento
typedef struct {
    int num_hilos;              // Número de hilos a usar
    int incrementos_por_hilo;   // Incrementos que debe hacer cada hilo
    bool usar_delay;            // Si añadir delays para forzar race conditions
    int delay_microsegundos;    // Delay entre incrementos
    bool mostrar_detalles;      // Mostrar información detallada
} configuracion_experimento_t;

// Estructura para resultados de un experimento
typedef struct {
    int valor_esperado;         // Valor que debería tener el contador
    int valor_obtenido;         // Valor real obtenido
    int diferencia;             // Diferencia (valor_esperado - valor_obtenido)
    double porcentaje_error;    // Porcentaje de error
    uint64_t tiempo_total_us;   // Tiempo total del experimento
    int num_hilos;              // Número de hilos utilizados
    int incrementos_por_hilo;   // Incrementos por hilo
    parametros_incremento_t* resultados_hilos; // Resultados individuales de hilos
} resultado_experimento_t;

// Estructura para análisis estadístico de múltiples experimentos
typedef struct {
    int num_experimentos;       // Número de experimentos realizados
    resultado_experimento_t* experimentos; // Array de resultados
    
    // Estadísticas del valor obtenido
    int valor_esperado;         // Valor esperado constante
    int valor_min;              // Valor mínimo obtenido
    int valor_max;              // Valor máximo obtenido
    double valor_promedio;      // Valor promedio obtenido
    double desviacion_estandar; // Desviación estándar
    
    // Estadísticas de la diferencia
    int diferencia_min;         // Diferencia mínima (mejor caso)
    int diferencia_max;         // Diferencia máxima (peor caso)
    double diferencia_promedio; // Diferencia promedio
    
    // Estadísticas de error
    double error_min;           // Error mínimo en porcentaje
    double error_max;           // Error máximo en porcentaje
    double error_promedio;      // Error promedio en porcentaje
    
    // Estadísticas de tiempo
    uint64_t tiempo_min_us;     // Tiempo mínimo
    uint64_t tiempo_max_us;     // Tiempo máximo
    double tiempo_promedio_us;  // Tiempo promedio
    
    // Análisis de consistencia
    int experimentos_correctos; // Experimentos con resultado correcto
    double tasa_inconsistencia; // Porcentaje de experimentos incorrectos
} analisis_estadistico_t;

// Estructura para comparación con versión sincronizada
typedef struct {
    resultado_experimento_t experimento_race;     // Resultado con race condition
    resultado_experimento_t experimento_seguro;   // Resultado sincronizado
    uint64_t overhead_sincronizacion_us;          // Overhead de la sincronización
    double factor_slowdown;                       // Factor de ralentización
} comparacion_sincronizacion_t;

/**
 * @brief Función que ejecuta un hilo incrementador (con race condition)
 * @param arg Puntero a parametros_incremento_t
 * @return NULL
 */
void* hilo_incrementar_race_condition(void* arg);

/**
 * @brief Función que ejecuta un hilo incrementador (con mutex)
 * @param arg Puntero a parametros_incremento_t
 * @return NULL
 */
void* hilo_incrementar_sincronizado(void* arg);

/**
 * @brief Ejecuta un experimento de race condition
 * @param config Configuración del experimento
 * @param resultado Estructura donde almacenar los resultados
 * @return true si el experimento fue exitoso
 */
bool ejecutar_experimento_race_condition(const configuracion_experimento_t* config, 
                                        resultado_experimento_t* resultado);

/**
 * @brief Ejecuta un experimento con sincronización (referencia)
 * @param config Configuración del experimento
 * @param resultado Estructura donde almacenar los resultados
 * @return true si el experimento fue exitoso
 */
bool ejecutar_experimento_sincronizado(const configuracion_experimento_t* config, 
                                      resultado_experimento_t* resultado);

/**
 * @brief Inicializa una estructura de resultado de experimento
 * @param resultado Puntero a la estructura a inicializar
 * @param num_hilos Número de hilos que participarán
 * @return true si la inicialización fue exitosa
 */
bool inicializar_resultado_experimento(resultado_experimento_t* resultado, int num_hilos);

/**
 * @brief Libera recursos de una estructura de resultado de experimento
 * @param resultado Puntero a la estructura a limpiar
 */
void limpiar_resultado_experimento(resultado_experimento_t* resultado);

/**
 * @brief Ejecuta múltiples experimentos y realiza análisis estadístico
 * @param config Configuración base de los experimentos
 * @param num_experimentos Número de experimentos a realizar
 * @param analisis Estructura donde almacenar el análisis
 * @return true si el análisis fue exitoso
 */
bool ejecutar_analisis_estadistico(const configuracion_experimento_t* config, 
                                   int num_experimentos, 
                                   analisis_estadistico_t* analisis);

/**
 * @brief Inicializa una estructura de análisis estadístico
 * @param analisis Puntero a la estructura a inicializar
 * @param num_experimentos Número de experimentos que contendrá
 * @return true si la inicialización fue exitosa
 */
bool inicializar_analisis_estadistico(analisis_estadistico_t* analisis, int num_experimentos);

/**
 * @brief Libera recursos de una estructura de análisis estadístico
 * @param analisis Puntero a la estructura a limpiar
 */
void limpiar_analisis_estadistico(analisis_estadistico_t* analisis);

/**
 * @brief Compara experimentos con y sin sincronización
 * @param config Configuración del experimento
 * @param comparacion Estructura donde almacenar la comparación
 * @return true si la comparación fue exitosa
 */
bool ejecutar_comparacion_sincronizacion(const configuracion_experimento_t* config, 
                                        comparacion_sincronizacion_t* comparacion);

/**
 * @brief Imprime los resultados de un experimento individual
 * @param resultado Puntero a los resultados del experimento
 */
void imprimir_resultado_experimento(const resultado_experimento_t* resultado);

/**
 * @brief Imprime el análisis estadístico de múltiples experimentos
 * @param analisis Puntero al análisis estadístico
 */
void imprimir_analisis_estadistico(const analisis_estadistico_t* analisis);

/**
 * @brief Imprime la comparación entre versiones con y sin sincronización
 * @param comparacion Puntero a la comparación
 */
void imprimir_comparacion_sincronizacion(const comparacion_sincronizacion_t* comparacion);

/**
 * @brief Obtiene tiempo actual en microsegundos
 * @return Tiempo en microsegundos
 */
uint64_t obtener_tiempo_microsegundos(void);

/**
 * @brief Calcula estadísticas básicas de un array de enteros
 * @param valores Array de valores
 * @param n Número de elementos
 * @param min Puntero donde almacenar el mínimo
 * @param max Puntero donde almacenar el máximo
 * @param promedio Puntero donde almacenar el promedio
 * @param desviacion Puntero donde almacenar la desviación estándar
 */
void calcular_estadisticas(const int* valores, int n, int* min, int* max, 
                          double* promedio, double* desviacion);

/**
 * @brief Calcula estadísticas de tiempos en microsegundos
 * @param tiempos Array de tiempos
 * @param n Número de elementos
 * @param min Puntero donde almacenar el mínimo
 * @param max Puntero donde almacenar el máximo
 * @param promedio Puntero donde almacenar el promedio
 */
void calcular_estadisticas_tiempo(const uint64_t* tiempos, int n, 
                                 uint64_t* min, uint64_t* max, double* promedio);

/**
 * @brief Ejecuta el ejemplo básico del enunciado
 * @return true si la ejecución fue exitosa
 */
bool ejecutar_ejemplo_basico_enunciado(void);

/**
 * @brief Ejecuta una demostración completa de race conditions
 * @return true si la demostración fue exitosa
 */
bool ejecutar_demo_completa_race_conditions(void);

/**
 * @brief Demuestra cómo diferentes factores afectan las race conditions
 * @return true si la demostración fue exitosa
 */
bool demostrar_factores_race_conditions(void);

/**
 * @brief Genera un reporte detallado de race conditions para análisis
 * @param config Configuración del experimento
 * @param nombre_archivo Archivo donde guardar el reporte (NULL para stdout)
 * @return true si el reporte fue generado exitosamente
 */
bool generar_reporte_race_conditions(const configuracion_experimento_t* config, 
                                    const char* nombre_archivo);

/**
 * @brief Estima la probabilidad de race condition basada en parámetros
 * @param num_hilos Número de hilos
 * @param incrementos_por_hilo Incrementos por hilo
 * @param delay_microsegundos Delay entre operaciones
 * @return Probabilidad estimada (0.0 a 1.0)
 */
double estimar_probabilidad_race_condition(int num_hilos, int incrementos_por_hilo, 
                                          int delay_microsegundos);

/**
 * @brief Simula diferentes escenarios de race conditions
 * @return true si la simulación fue exitosa
 */
bool simular_escenarios_race_conditions(void);

// Variable global compartida para demostraciones (extern)
extern int contador_global;
extern pthread_mutex_t mutex_contador;

#endif // RACE_CONDITIONS_H
