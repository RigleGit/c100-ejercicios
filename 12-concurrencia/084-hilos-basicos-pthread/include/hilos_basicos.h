/**
 * @file hilos_basicos.h
 * @brief Biblioteca para demostrar el uso básico de hilos POSIX (pthread)
 * @author Ejercicios C
 * @date 2025
 */

#ifndef HILOS_BASICOS_H
#define HILOS_BASICOS_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

/* ====================================================================
 * CONFIGURACIÓN Y CONSTANTES
 * ==================================================================== */

#define MAX_HILOS 10
#define BUFFER_SIZE 256
#define TIEMPO_ESPERA_DEFAULT 1

/* ====================================================================
 * ESTRUCTURAS PARA PASAR DATOS A HILOS
 * ==================================================================== */

/**
 * @brief Estructura para pasar parámetros a un hilo
 */
typedef struct {
    int id;                    // ID del hilo
    char mensaje[BUFFER_SIZE]; // Mensaje personalizado
    int iteraciones;           // Número de iteraciones a realizar
    int delay_ms;              // Delay en milisegundos entre iteraciones
} hilo_params_t;

/**
 * @brief Estructura para retornar resultados de un hilo
 */
typedef struct {
    int hilo_id;              // ID del hilo que generó el resultado
    int operaciones_realizadas; // Número de operaciones completadas
    long tiempo_ejecucion_ms; // Tiempo de ejecución en milisegundos
    bool exito;               // Si la ejecución fue exitosa
} hilo_resultado_t;

/**
 * @brief Estructura para demostraciones de sincronización
 */
typedef struct {
    int contador_compartido;   // Contador compartido entre hilos
    pthread_mutex_t mutex;     // Mutex para proteger el contador
    int num_hilos;            // Número de hilos que accederán
    int incrementos_por_hilo; // Incrementos que hará cada hilo
} contador_compartido_t;

/* ====================================================================
 * FUNCIONES BÁSICAS DE HILOS
 * ==================================================================== */

/**
 * @brief Función básica que ejecuta un hilo simple
 * @param arg Puntero a argumentos (puede ser NULL)
 * @return Puntero a resultado (NULL en este caso)
 */
void* funcion_hilo_basico(void* arg);

/**
 * @brief Función de hilo que recibe parámetros personalizados
 * @param arg Puntero a estructura hilo_params_t
 * @return Puntero a estructura hilo_resultado_t
 */
void* funcion_hilo_con_parametros(void* arg);

/**
 * @brief Función de hilo que realiza cálculos intensivos
 * @param arg Puntero a número entero (límite de cálculo)
 * @return Puntero a resultado del cálculo
 */
void* funcion_hilo_calculo(void* arg);

/**
 * @brief Función de hilo que simula trabajo con delays
 * @param arg Puntero a hilo_params_t con configuración de trabajo
 * @return Puntero a hilo_resultado_t con estadísticas
 */
void* funcion_hilo_trabajo(void* arg);

/* ====================================================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ==================================================================== */

/**
 * @brief Demuestra la creación y ejecución de un hilo básico
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_hilo_basico(void);

/**
 * @brief Demuestra el paso de parámetros a hilos
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_hilos_con_parametros(void);

/**
 * @brief Demuestra la creación de múltiples hilos
 * @param num_hilos Número de hilos a crear
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_multiples_hilos(int num_hilos);

/**
 * @brief Demuestra hilos realizando cálculos paralelos
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_calculo_paralelo(void);

/**
 * @brief Demuestra la sincronización básica entre hilos
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_sincronizacion_basica(void);

/* ====================================================================
 * FUNCIONES DE SINCRONIZACIÓN (AVANZADAS)
 * ==================================================================== */

/**
 * @brief Función de hilo que incrementa un contador compartido sin sincronización
 * @param arg Puntero a contador_compartido_t
 * @return NULL
 */
void* incrementar_sin_mutex(void* arg);

/**
 * @brief Función de hilo que incrementa un contador compartido con mutex
 * @param arg Puntero a contador_compartido_t
 * @return NULL
 */
void* incrementar_con_mutex(void* arg);

/**
 * @brief Demuestra el problema de condición de carrera
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_condicion_carrera(void);

/**
 * @brief Demuestra la solución con mutex para condiciones de carrera
 * @return 0 si es exitoso, código de error en caso contrario
 */
int demostrar_mutex_solucion(void);

/* ====================================================================
 * UTILIDADES Y HELPERS
 * ==================================================================== */

/**
 * @brief Obtiene el ID del hilo actual
 * @return ID del hilo actual como entero
 */
long obtener_id_hilo_actual(void);

/**
 * @brief Función para simular trabajo (sleep con mensaje)
 * @param segundos Segundos a dormir
 * @param mensaje Mensaje a mostrar
 */
void simular_trabajo(int segundos, const char* mensaje);

/**
 * @brief Obtiene el tiempo actual en milisegundos
 * @return Tiempo actual en milisegundos
 */
long obtener_tiempo_ms(void);

/**
 * @brief Crea parámetros por defecto para un hilo
 * @param id ID del hilo
 * @param mensaje Mensaje personalizado
 * @return Estructura hilo_params_t inicializada
 */
hilo_params_t crear_parametros_hilo(int id, const char* mensaje);

/**
 * @brief Libera memoria de resultado de hilo
 * @param resultado Puntero a resultado a liberar
 */
void liberar_resultado_hilo(hilo_resultado_t* resultado);

/* ====================================================================
 * FUNCIONES DE INFORMACIÓN Y ESTADÍSTICAS
 * ==================================================================== */

/**
 * @brief Muestra información sobre el soporte de hilos
 */
void mostrar_info_hilos(void);

/**
 * @brief Muestra estadísticas de ejecución de hilos
 * @param resultados Array de resultados de hilos
 * @param num_resultados Número de resultados
 */
void mostrar_estadisticas_hilos(hilo_resultado_t* resultados[], int num_resultados);

/**
 * @brief Verifica si la biblioteca pthread está disponible
 * @return true si pthread está disponible, false en caso contrario
 */
bool verificar_soporte_pthread(void);

#endif /* HILOS_BASICOS_H */
