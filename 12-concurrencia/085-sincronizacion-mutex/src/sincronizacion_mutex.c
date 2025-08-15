#include "../include/sincronizacion_mutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
// Helper para demostrar race condition sin protección (parámetros por hilo)
typedef struct {
    volatile int64_t* contador_compartido;
    int operaciones;
} args_incremento_sin_proteccion_t;

static void* incrementar_sin_proteccion_thread(void* arg) {
    args_incremento_sin_proteccion_t* args = (args_incremento_sin_proteccion_t*)arg;
    if (args == NULL || args->contador_compartido == NULL) {
        return NULL;
    }
    for (int i = 0; i < args->operaciones; i++) {
        // Acceso no protegido provoca race condition
        int64_t valor = *(args->contador_compartido);
        usleep(1);
        *(args->contador_compartido) = valor + 1;
    }
    return NULL;
}


// Implementación de funciones básicas de recurso compartido

bool inicializar_recurso_compartido(recurso_compartido_t* recurso, int64_t valor_inicial) {
    if (recurso == NULL) {
        return false;
    }
    
    // Inicializar mutex con atributos por defecto
    int resultado = pthread_mutex_init(&recurso->mutex, NULL);
    if (resultado != 0) {
        fprintf(stderr, "Error inicializando mutex: %s\n", strerror(resultado));
        return false;
    }
    
    recurso->valor = valor_inicial;
    recurso->inicializado = true;
    
    return true;
}

bool destruir_recurso_compartido(recurso_compartido_t* recurso) {
    if (recurso == NULL || !recurso->inicializado) {
        return false;
    }
    
    int resultado = pthread_mutex_destroy(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error destruyendo mutex: %s\n", strerror(resultado));
        return false;
    }
    
    recurso->inicializado = false;
    return true;
}

bool incrementar_recurso_seguro(recurso_compartido_t* recurso, int64_t incremento) {
    if (recurso == NULL || !recurso->inicializado) {
        return false;
    }
    
    // Entrar en sección crítica
    int resultado = pthread_mutex_lock(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error bloqueando mutex: %s\n", strerror(resultado));
        return false;
    }
    
    // Operación crítica
    recurso->valor += incremento;
    
    // Salir de sección crítica
    resultado = pthread_mutex_unlock(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error desbloqueando mutex: %s\n", strerror(resultado));
        return false;
    }
    
    return true;
}

bool leer_recurso_seguro(recurso_compartido_t* recurso, int64_t* valor) {
    if (recurso == NULL || !recurso->inicializado || valor == NULL) {
        return false;
    }
    
    int resultado = pthread_mutex_lock(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error bloqueando mutex para lectura: %s\n", strerror(resultado));
        return false;
    }
    
    *valor = recurso->valor;
    
    resultado = pthread_mutex_unlock(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error desbloqueando mutex después de lectura: %s\n", strerror(resultado));
        return false;
    }
    
    return true;
}

bool establecer_recurso_seguro(recurso_compartido_t* recurso, int64_t nuevo_valor) {
    if (recurso == NULL || !recurso->inicializado) {
        return false;
    }
    
    int resultado = pthread_mutex_lock(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error bloqueando mutex para escritura: %s\n", strerror(resultado));
        return false;
    }
    
    recurso->valor = nuevo_valor;
    
    resultado = pthread_mutex_unlock(&recurso->mutex);
    if (resultado != 0) {
        fprintf(stderr, "Error desbloqueando mutex después de escritura: %s\n", strerror(resultado));
        return false;
    }
    
    return true;
}

// Implementación de estadísticas

bool inicializar_estadisticas_mutex(estadisticas_mutex_t* stats) {
    if (stats == NULL) {
        return false;
    }
    
    memset(stats, 0, sizeof(estadisticas_mutex_t));
    
    int resultado = pthread_mutex_init(&stats->mutex_stats, NULL);
    if (resultado != 0) {
        fprintf(stderr, "Error inicializando mutex de estadísticas: %s\n", strerror(resultado));
        return false;
    }
    
    return true;
}

bool destruir_estadisticas_mutex(estadisticas_mutex_t* stats) {
    if (stats == NULL) {
        return false;
    }
    
    int resultado = pthread_mutex_destroy(&stats->mutex_stats);
    if (resultado != 0) {
        fprintf(stderr, "Error destruyendo mutex de estadísticas: %s\n", strerror(resultado));
        return false;
    }
    
    return true;
}

void registrar_lectura(estadisticas_mutex_t* stats, uint64_t tiempo_espera_ms) {
    if (stats == NULL) return;
    
    pthread_mutex_lock(&stats->mutex_stats);
    stats->operaciones_lectura++;
    stats->tiempo_espera_ms += tiempo_espera_ms;
    pthread_mutex_unlock(&stats->mutex_stats);
}

void registrar_escritura(estadisticas_mutex_t* stats, uint64_t tiempo_espera_ms) {
    if (stats == NULL) return;
    
    pthread_mutex_lock(&stats->mutex_stats);
    stats->operaciones_escritura++;
    stats->tiempo_espera_ms += tiempo_espera_ms;
    pthread_mutex_unlock(&stats->mutex_stats);
}

bool obtener_estadisticas(estadisticas_mutex_t* stats, estadisticas_mutex_t* copia) {
    if (stats == NULL || copia == NULL) {
        return false;
    }
    
    pthread_mutex_lock(&stats->mutex_stats);
    
    copia->operaciones_lectura = stats->operaciones_lectura;
    copia->operaciones_escritura = stats->operaciones_escritura;
    copia->colisiones_detectadas = stats->colisiones_detectadas;
    copia->tiempo_espera_ms = stats->tiempo_espera_ms;
    
    pthread_mutex_unlock(&stats->mutex_stats);
    
    return true;
}

// Implementación de funciones de prueba

void* hilo_prueba_concurrencia(void* arg) {
    parametros_prueba_t* params = (parametros_prueba_t*)arg;
    if (params == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < params->operaciones_por_hilo; i++) {
        uint64_t inicio = obtener_tiempo_microsegundos();
        
        if (params->usar_mutex) {
            // Operación con mutex (thread-safe)
            if (i % 3 == 0) {
                // Lectura
                int64_t valor;
                leer_recurso_seguro(params->recurso, &valor);
                registrar_lectura(params->stats, (obtener_tiempo_microsegundos() - inicio) / 1000);
            } else {
                // Escritura
                incrementar_recurso_seguro(params->recurso, 1);
                registrar_escritura(params->stats, (obtener_tiempo_microsegundos() - inicio) / 1000);
            }
        } else {
            // Operación sin mutex (race condition)
            if (i % 3 == 0) {
                // Lectura directa (no thread-safe)
                volatile int64_t valor = params->recurso->valor;
                (void)valor; // Evitar warning de variable no usada
                registrar_lectura(params->stats, (obtener_tiempo_microsegundos() - inicio) / 1000);
            } else {
                // Escritura directa (no thread-safe)
                params->recurso->valor++;
                registrar_escritura(params->stats, (obtener_tiempo_microsegundos() - inicio) / 1000);
            }
        }
        
        // Simular algo de trabajo
        usleep(100); // 100 microsegundos
    }
    
    return NULL;
}

bool ejecutar_prueba_rendimiento(parametros_prueba_t* params) {
    if (params == NULL || params->recurso == NULL || params->stats == NULL) {
        return false;
    }
    
    pthread_t* hilos = malloc(params->num_hilos * sizeof(pthread_t));
    if (hilos == NULL) {
        return false;
    }
    
    printf("Ejecutando prueba con %d hilos, %d operaciones por hilo (%s mutex)\n",
           params->num_hilos, params->operaciones_por_hilo,
           params->usar_mutex ? "con" : "sin");
    
    uint64_t inicio_prueba = obtener_tiempo_microsegundos();
    
    // Crear hilos
    for (int i = 0; i < params->num_hilos; i++) {
        int resultado = pthread_create(&hilos[i], NULL, hilo_prueba_concurrencia, params);
        if (resultado != 0) {
            fprintf(stderr, "Error creando hilo %d: %s\n", i, strerror(resultado));
            free(hilos);
            return false;
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < params->num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    uint64_t tiempo_total = (obtener_tiempo_microsegundos() - inicio_prueba) / 1000;
    
    printf("Prueba completada en %lu ms\n", tiempo_total);
    
    int64_t valor_final;
    if (params->usar_mutex) {
        leer_recurso_seguro(params->recurso, &valor_final);
    } else {
        valor_final = params->recurso->valor;
    }
    
    printf("Valor final del recurso: %ld\n", valor_final);
    printf("Valor esperado: %ld\n", 
           (int64_t)(params->num_hilos * params->operaciones_por_hilo * 2 / 3)); // 2/3 son escrituras
    
    imprimir_estadisticas_rendimiento(params->stats, tiempo_total);
    
    free(hilos);
    return true;
}

int64_t demostrar_race_condition(int num_hilos, int operaciones) {
    printf("\n=== Demostración de Race Condition ===\n");
    
    // Recurso sin protección de mutex adecuada
    volatile int64_t contador_global = 0;
    
    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));
    if (hilos == NULL) {
        return -1;
    }
    
    printf("Incrementando contador %d veces con %d hilos (sin mutex)\n", 
           operaciones, num_hilos);

    args_incremento_sin_proteccion_t* args = malloc((size_t)num_hilos * sizeof(*args));
    if (args == NULL) {
        free(hilos);
        return -1;
    }

    for (int i = 0; i < num_hilos; i++) {
        args[i].contador_compartido = &contador_global;
        args[i].operaciones = operaciones;
        pthread_create(&hilos[i], NULL, incrementar_sin_proteccion_thread, &args[i]);
    }
    
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    printf("Valor final: %lld\n", (long long)contador_global);
    printf("Valor esperado: %d\n", num_hilos * operaciones);
    printf("Diferencia: %lld (evidencia de race condition)\n", 
           (long long)((int64_t)(num_hilos * operaciones) - contador_global));
    
    free(hilos);
    free(args);
    return contador_global;
}

// Implementación de tipos de mutex

bool inicializar_demo_tipos_mutex(demo_tipos_mutex_t* demo) {
    if (demo == NULL) {
        return false;
    }
    
    memset(demo, 0, sizeof(demo_tipos_mutex_t));
    
    // Mutex normal
    if (pthread_mutex_init(&demo->mutex_normal, NULL) != 0) {
        return false;
    }
    
    // Mutex recursivo
    pthread_mutexattr_t attr_recursivo;
    pthread_mutexattr_init(&attr_recursivo);
    pthread_mutexattr_settype(&attr_recursivo, PTHREAD_MUTEX_RECURSIVE);
    
    if (pthread_mutex_init(&demo->mutex_recursivo, &attr_recursivo) != 0) {
        pthread_mutexattr_destroy(&attr_recursivo);
        pthread_mutex_destroy(&demo->mutex_normal);
        return false;
    }
    pthread_mutexattr_destroy(&attr_recursivo);
    
    // Mutex errorcheck
    pthread_mutexattr_t attr_errorcheck;
    pthread_mutexattr_init(&attr_errorcheck);
    pthread_mutexattr_settype(&attr_errorcheck, PTHREAD_MUTEX_ERRORCHECK);
    
    if (pthread_mutex_init(&demo->mutex_errorcheck, &attr_errorcheck) != 0) {
        pthread_mutexattr_destroy(&attr_errorcheck);
        pthread_mutex_destroy(&demo->mutex_normal);
        pthread_mutex_destroy(&demo->mutex_recursivo);
        return false;
    }
    pthread_mutexattr_destroy(&attr_errorcheck);
    
    demo->inicializado = true;
    return true;
}

bool destruir_demo_tipos_mutex(demo_tipos_mutex_t* demo) {
    if (demo == NULL || !demo->inicializado) {
        return false;
    }
    
    pthread_mutex_destroy(&demo->mutex_normal);
    pthread_mutex_destroy(&demo->mutex_recursivo);
    pthread_mutex_destroy(&demo->mutex_errorcheck);
    
    demo->inicializado = false;
    return true;
}

bool demostrar_mutex_recursivo(demo_tipos_mutex_t* demo, int nivel) {
    if (demo == NULL || !demo->inicializado || nivel < 0) {
        return false;
    }
    
    printf("Nivel de recursión: %d\n", nivel);
    
    // Adquirir mutex recursivo
    int resultado = pthread_mutex_lock(&demo->mutex_recursivo);
    if (resultado != 0) {
        printf("Error adquiriendo mutex recursivo en nivel %d: %s\n", 
               nivel, strerror(resultado));
        return false;
    }
    
    demo->contador_recursivo++;
    printf("Contador incrementado a: %d\n", demo->contador_recursivo);
    
    // Llamada recursiva
    if (nivel > 0) {
        demostrar_mutex_recursivo(demo, nivel - 1);
    }
    
    // Liberar mutex recursivo
    resultado = pthread_mutex_unlock(&demo->mutex_recursivo);
    if (resultado != 0) {
        printf("Error liberando mutex recursivo en nivel %d: %s\n", 
               nivel, strerror(resultado));
        return false;
    }
    
    return true;
}

bool demostrar_mutex_errorcheck(demo_tipos_mutex_t* demo) {
    if (demo == NULL || !demo->inicializado) {
        return false;
    }
    
    printf("\n=== Demostración Mutex ErrorCheck ===\n");
    
    // Adquirir mutex
    int resultado = pthread_mutex_lock(&demo->mutex_errorcheck);
    if (resultado == 0) {
        printf("Mutex errorcheck adquirido correctamente\n");
    } else {
        printf("Error adquiriendo mutex errorcheck: %s\n", strerror(resultado));
        return false;
    }
    
    // Intentar adquirir nuevamente (debería fallar)
    resultado = pthread_mutex_lock(&demo->mutex_errorcheck);
    if (resultado == EDEADLK) {
        printf("Detección correcta de deadlock potencial\n");
    } else if (resultado == 0) {
        printf("ADVERTENCIA: Mutex errorcheck permitió doble lock\n");
    } else {
        printf("Error inesperado en segundo lock: %s\n", strerror(resultado));
    }
    
    // Liberar mutex
    resultado = pthread_mutex_unlock(&demo->mutex_errorcheck);
    if (resultado == 0) {
        printf("Mutex errorcheck liberado correctamente\n");
    } else {
        printf("Error liberando mutex errorcheck: %s\n", strerror(resultado));
    }
    
    // Intentar liberar nuevamente (debería fallar)
    resultado = pthread_mutex_unlock(&demo->mutex_errorcheck);
    if (resultado == EPERM) {
        printf("Detección correcta de unlock sin lock previo\n");
    } else if (resultado == 0) {
        printf("ADVERTENCIA: Mutex errorcheck permitió doble unlock\n");
    } else {
        printf("Error inesperado en segundo unlock: %s\n", strerror(resultado));
    }
    
    return true;
}

bool mutex_trylock_timeout(pthread_mutex_t* mutex, int timeout_ms) {
    if (mutex == NULL || timeout_ms < 0) {
        return false;
    }

#if !defined(__APPLE__)
    // Uso directo de pthread_mutex_timedlock cuando está disponible
    struct timespec abs_timeout;
    clock_gettime(CLOCK_REALTIME, &abs_timeout);
    abs_timeout.tv_sec += timeout_ms / 1000;
    abs_timeout.tv_nsec += (long)(timeout_ms % 1000) * 1000000L;
    if (abs_timeout.tv_nsec >= 1000000000L) {
        abs_timeout.tv_sec += 1;
        abs_timeout.tv_nsec -= 1000000000L;
    }

    int resultado = pthread_mutex_timedlock(mutex, &abs_timeout);
    if (resultado == 0) return true;
    if (resultado == ETIMEDOUT) return false;
    return false;
#else
    // macOS: emular timeout con trylock + nanosleep
    const int sleep_step_us = 1000; // 1ms
    int elapsed_ms = 0;
    while (elapsed_ms <= timeout_ms) {
        if (pthread_mutex_trylock(mutex) == 0) {
            return true;
        }
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = (long)sleep_step_us * 1000L;
        nanosleep(&ts, NULL);
        elapsed_ms += sleep_step_us / 1000;
    }
    return false;
#endif
}

// Funciones utilitarias

uint64_t obtener_tiempo_microsegundos(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void imprimir_estadisticas_rendimiento(estadisticas_mutex_t* stats, uint64_t tiempo_total_ms) {
    if (stats == NULL) return;
    
    estadisticas_mutex_t copia;
    if (!obtener_estadisticas(stats, &copia)) {
        printf("Error obteniendo estadísticas\n");
        return;
    }
    
    printf("\n=== Estadísticas de Rendimiento ===\n");
    printf("Operaciones de lectura: %lu\n", copia.operaciones_lectura);
    printf("Operaciones de escritura: %lu\n", copia.operaciones_escritura);
    printf("Total de operaciones: %lu\n", 
           copia.operaciones_lectura + copia.operaciones_escritura);
    printf("Tiempo total de espera: %lu ms\n", copia.tiempo_espera_ms);
    printf("Tiempo total de prueba: %lu ms\n", tiempo_total_ms);
    
    if (tiempo_total_ms > 0) {
        uint64_t total_ops = copia.operaciones_lectura + copia.operaciones_escritura;
        printf("Operaciones por segundo: %.2f\n", 
               (double)total_ops * 1000.0 / tiempo_total_ms);
    }
    
    if (copia.operaciones_lectura + copia.operaciones_escritura > 0) {
        printf("Tiempo promedio de espera por operación: %.2f ms\n",
               (double)copia.tiempo_espera_ms / 
               (copia.operaciones_lectura + copia.operaciones_escritura));
    }
}

bool ejecutar_demo_completa_mutex(void) {
    printf("=== Demostración Completa de Sincronización con Mutex ===\n\n");
    
    // 1. Demostrar race condition
    demostrar_race_condition(4, 1000);
    
    // 2. Prueba con mutex
    printf("\n=== Prueba con Mutex ===\n");
    
    recurso_compartido_t recurso;
    estadisticas_mutex_t stats;
    
    if (!inicializar_recurso_compartido(&recurso, 0)) {
        printf("Error inicializando recurso compartido\n");
        return false;
    }
    
    if (!inicializar_estadisticas_mutex(&stats)) {
        printf("Error inicializando estadísticas\n");
        destruir_recurso_compartido(&recurso);
        return false;
    }
    
    parametros_prueba_t params = {
        .num_hilos = 4,
        .operaciones_por_hilo = 1000,
        .recurso = &recurso,
        .stats = &stats,
        .usar_mutex = true
    };
    
    ejecutar_prueba_rendimiento(&params);
    
    // 3. Demostrar tipos de mutex
    printf("\n=== Demostración de Tipos de Mutex ===\n");
    
    demo_tipos_mutex_t demo;
    if (inicializar_demo_tipos_mutex(&demo)) {
        printf("\nMutex Recursivo:\n");
        demostrar_mutex_recursivo(&demo, 3);
        
        demostrar_mutex_errorcheck(&demo);
        
        destruir_demo_tipos_mutex(&demo);
    }
    
    // Cleanup
    destruir_estadisticas_mutex(&stats);
    destruir_recurso_compartido(&recurso);
    
    printf("\n=== Demostración Completa Finalizada ===\n");
    return true;
}
