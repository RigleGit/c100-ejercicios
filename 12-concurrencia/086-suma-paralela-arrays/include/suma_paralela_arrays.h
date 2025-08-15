#ifndef SUMA_PARALELA_ARRAYS_H
#define SUMA_PARALELA_ARRAYS_H

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @file suma_paralela_arrays.h
 * @brief Ejercicio 086: Suma Paralela de Arrays con Hilos
 * 
 * Este ejercicio demuestra cómo dividir tareas computacionales entre
 * múltiples hilos para aprovechar sistemas con múltiples núcleos.
 * 
 * Conceptos cubiertos:
 * - División de trabajo entre hilos
 * - Computación paralela básica
 * - Agregación de resultados parciales
 * - Medición de rendimiento paralelo vs secuencial
 * - Load balancing entre hilos
 * - Escalabilidad horizontal
 */

// Estructura para pasar parámetros a un hilo de suma
typedef struct {
    const int* array;           // Array a procesar
    size_t inicio;              // Índice de inicio (inclusivo)
    size_t fin;                 // Índice de fin (exclusivo)
    int64_t resultado;          // Resultado de la suma parcial
    int id_hilo;                // Identificador del hilo
    uint64_t tiempo_inicio;     // Tiempo de inicio en microsegundos
    uint64_t tiempo_fin;        // Tiempo de fin en microsegundos
} parametros_suma_t;

// Estructura para configuración de suma paralela
typedef struct {
    const int* array;           // Array a procesar
    size_t tamano;              // Tamaño del array
    int num_hilos;              // Número de hilos a usar
    bool mostrar_detalles;      // Mostrar información detallada
} configuracion_suma_t;

// Estructura para resultados de suma paralela
typedef struct {
    int64_t suma_total;         // Suma total calculada
    uint64_t tiempo_total_us;   // Tiempo total en microsegundos
    int num_hilos_usados;       // Número de hilos realmente usados
    parametros_suma_t* resultados_hilos; // Resultados de cada hilo
    double speedup;             // Aceleración vs versión secuencial
    double eficiencia;          // Eficiencia del paralelismo
} resultado_suma_paralela_t;

// Estructura para benchmark comparativo
typedef struct {
    int64_t suma_secuencial;
    int64_t suma_paralela;
    uint64_t tiempo_secuencial_us;
    uint64_t tiempo_paralelo_us;
    double speedup;
    double eficiencia;
    int num_hilos;
    size_t tamano_array;
} benchmark_suma_t;

/**
 * @brief Función que ejecuta un hilo para calcular suma parcial
 * @param arg Puntero a parametros_suma_t
 * @return NULL
 */
void* hilo_suma_parcial(void* arg);

/**
 * @brief Calcula la suma de un array usando múltiples hilos
 * @param config Configuración de la suma paralela
 * @param resultado Estructura donde almacenar los resultados
 * @return true si la operación fue exitosa
 */
bool suma_paralela(const configuracion_suma_t* config, resultado_suma_paralela_t* resultado);

/**
 * @brief Calcula la suma de un array de forma secuencial
 * @param array Array a sumar
 * @param tamano Tamaño del array
 * @param tiempo_us Puntero donde almacenar el tiempo (puede ser NULL)
 * @return Suma total del array
 */
int64_t suma_secuencial(const int* array, size_t tamano, uint64_t* tiempo_us);

/**
 * @brief Inicializa una estructura de resultados
 * @param resultado Puntero a la estructura a inicializar
 * @param num_hilos Número de hilos que se usarán
 * @return true si la inicialización fue exitosa
 */
bool inicializar_resultado_suma(resultado_suma_paralela_t* resultado, int num_hilos);

/**
 * @brief Libera recursos de una estructura de resultados
 * @param resultado Puntero a la estructura a limpiar
 */
void limpiar_resultado_suma(resultado_suma_paralela_t* resultado);

/**
 * @brief Crea un array con valores aleatorios para pruebas
 * @param tamano Tamaño del array a crear
 * @param valor_min Valor mínimo de los elementos
 * @param valor_max Valor máximo de los elementos
 * @return Puntero al array creado (debe liberarse con free)
 */
int* crear_array_aleatorio(size_t tamano, int valor_min, int valor_max);

/**
 * @brief Crea un array con secuencia aritmética
 * @param tamano Tamaño del array
 * @param inicio Valor inicial
 * @param incremento Incremento entre elementos
 * @return Puntero al array creado (debe liberarse con free)
 */
int* crear_array_secuencia(size_t tamano, int inicio, int incremento);

/**
 * @brief Verifica que dos sumas sean iguales
 * @param suma1 Primera suma
 * @param suma2 Segunda suma
 * @return true si las sumas son iguales
 */
bool verificar_sumas_iguales(int64_t suma1, int64_t suma2);

/**
 * @brief Ejecuta benchmark comparando suma secuencial vs paralela
 * @param array Array a usar en el benchmark
 * @param tamano Tamaño del array
 * @param num_hilos Número de hilos para la versión paralela
 * @param benchmark Estructura donde almacenar los resultados
 * @return true si el benchmark fue exitoso
 */
bool ejecutar_benchmark_suma(const int* array, size_t tamano, int num_hilos, benchmark_suma_t* benchmark);

/**
 * @brief Imprime los resultados de una suma paralela
 * @param resultado Puntero a los resultados
 */
void imprimir_resultados_suma_paralela(const resultado_suma_paralela_t* resultado);

/**
 * @brief Imprime los resultados de un benchmark
 * @param benchmark Puntero a los resultados del benchmark
 */
void imprimir_benchmark(const benchmark_suma_t* benchmark);

/**
 * @brief Calcula el número óptimo de hilos para un tamaño de array dado
 * @param tamano_array Tamaño del array
 * @return Número recomendado de hilos
 */
int calcular_num_hilos_optimo(size_t tamano_array);

/**
 * @brief Divide un rango en chunks para distribución entre hilos
 * @param inicio Inicio del rango
 * @param fin Fin del rango
 * @param num_hilos Número de hilos
 * @param chunks Array donde almacenar los chunks (debe tener espacio para num_hilos+1)
 */
void dividir_rango_en_chunks(size_t inicio, size_t fin, int num_hilos, size_t* chunks);

/**
 * @brief Obtiene tiempo actual en microsegundos
 * @return Tiempo en microsegundos
 */
uint64_t obtener_tiempo_microsegundos(void);

/**
 * @brief Obtiene número de CPUs disponibles en el sistema
 * @return Número de CPUs o -1 si no se puede determinar
 */
int obtener_num_cpus(void);

/**
 * @brief Ejecuta múltiples pruebas de escalabilidad
 * @param array Array para las pruebas
 * @param tamano Tamaño del array
 * @param max_hilos Número máximo de hilos a probar
 * @return true si las pruebas fueron exitosas
 */
bool ejecutar_pruebas_escalabilidad(const int* array, size_t tamano, int max_hilos);

/**
 * @brief Demostración completa de suma paralela con diferentes configuraciones
 * @return true si la demostración fue exitosa
 */
bool ejecutar_demo_completa_suma_paralela(void);

/**
 * @brief Calcula estadísticas de un array (min, max, promedio)
 * @param array Array a analizar
 * @param tamano Tamaño del array
 * @param min Puntero donde almacenar el mínimo
 * @param max Puntero donde almacenar el máximo
 * @param promedio Puntero donde almacenar el promedio
 */
void calcular_estadisticas_array(const int* array, size_t tamano, int* min, int* max, double* promedio);

/**
 * @brief Valida que un array no contenga valores que puedan causar overflow
 * @param array Array a validar
 * @param tamano Tamaño del array
 * @return true si el array es seguro para sumar
 */
bool validar_array_para_suma(const int* array, size_t tamano);

#endif // SUMA_PARALELA_ARRAYS_H
