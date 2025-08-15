#include "../include/race_conditions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

// Variables globales para demostraciones
int contador_global = 0;
pthread_mutex_t mutex_contador = PTHREAD_MUTEX_INITIALIZER;

// Implementación de funciones básicas

void* hilo_incrementar_race_condition(void* arg) {
    parametros_incremento_t* params = (parametros_incremento_t*)arg;
    if (params == NULL) {
        return NULL;
    }
    
    params->tiempo_inicio = obtener_tiempo_microsegundos();
    
    for (int i = 0; i < params->incrementos; i++) {
        // RACE CONDITION: Acceso no protegido a variable compartida
        (*params->contador)++;
        
        // Opcional: añadir delay para aumentar probabilidad de race condition
        if (params->usar_delay && params->delay_microsegundos > 0) {
            usleep(params->delay_microsegundos);
        }
    }
    
    params->tiempo_fin = obtener_tiempo_microsegundos();
    return NULL;
}

void* hilo_incrementar_sincronizado(void* arg) {
    parametros_incremento_t* params = (parametros_incremento_t*)arg;
    if (params == NULL) {
        return NULL;
    }
    
    params->tiempo_inicio = obtener_tiempo_microsegundos();
    
    for (int i = 0; i < params->incrementos; i++) {
        // Acceso protegido con mutex
        pthread_mutex_lock(&mutex_contador);
        (*params->contador)++;
        pthread_mutex_unlock(&mutex_contador);
        
        // Mismo delay que la versión race condition para comparación justa
        if (params->usar_delay && params->delay_microsegundos > 0) {
            usleep(params->delay_microsegundos);
        }
    }
    
    params->tiempo_fin = obtener_tiempo_microsegundos();
    return NULL;
}

bool ejecutar_experimento_race_condition(const configuracion_experimento_t* config, 
                                        resultado_experimento_t* resultado) {
    if (config == NULL || resultado == NULL) {
        return false;
    }
    
    // Inicializar resultado
    if (!inicializar_resultado_experimento(resultado, config->num_hilos)) {
        return false;
    }
    
    // Variable compartida local para este experimento
    int contador_local = 0;
    
    uint64_t tiempo_inicio = obtener_tiempo_microsegundos();
    
    // Crear hilos
    pthread_t* hilos = malloc(config->num_hilos * sizeof(pthread_t));
    if (hilos == NULL) {
        limpiar_resultado_experimento(resultado);
        return false;
    }
    
    // Configurar parámetros para cada hilo
    for (int i = 0; i < config->num_hilos; i++) {
        resultado->resultados_hilos[i].contador = &contador_local;
        resultado->resultados_hilos[i].incrementos = config->incrementos_por_hilo;
        resultado->resultados_hilos[i].id_hilo = i;
        resultado->resultados_hilos[i].usar_delay = config->usar_delay;
        resultado->resultados_hilos[i].delay_microsegundos = config->delay_microsegundos;
    }
    
    // Crear hilos
    for (int i = 0; i < config->num_hilos; i++) {
        int ret = pthread_create(&hilos[i], NULL, hilo_incrementar_race_condition, 
                                &resultado->resultados_hilos[i]);
        if (ret != 0) {
            fprintf(stderr, "Error creando hilo %d: %s\n", i, strerror(ret));
            // Limpiar hilos ya creados
            for (int j = 0; j < i; j++) {
                pthread_join(hilos[j], NULL);
            }
            free(hilos);
            limpiar_resultado_experimento(resultado);
            return false;
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < config->num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    uint64_t tiempo_fin = obtener_tiempo_microsegundos();
    
    // Llenar resultados
    resultado->valor_esperado = config->num_hilos * config->incrementos_por_hilo;
    resultado->valor_obtenido = contador_local;
    resultado->diferencia = resultado->valor_esperado - resultado->valor_obtenido;
    resultado->porcentaje_error = (double)abs(resultado->diferencia) * 100.0 / resultado->valor_esperado;
    resultado->tiempo_total_us = tiempo_fin - tiempo_inicio;
    resultado->num_hilos = config->num_hilos;
    resultado->incrementos_por_hilo = config->incrementos_por_hilo;
    
    if (config->mostrar_detalles) {
        printf("\n=== Detalles del Experimento (Race Condition) ===\n");
        printf("Hilos utilizados: %d\n", config->num_hilos);
        printf("Incrementos por hilo: %d\n", config->incrementos_por_hilo);
        printf("Delay entre incrementos: %s\n", 
               config->usar_delay ? "Sí" : "No");
        
        for (int i = 0; i < config->num_hilos; i++) {
            printf("Hilo %d: tiempo=%lu us\n",
                   resultado->resultados_hilos[i].id_hilo,
                   resultado->resultados_hilos[i].tiempo_fin - 
                   resultado->resultados_hilos[i].tiempo_inicio);
        }
    }
    
    free(hilos);
    return true;
}

bool ejecutar_experimento_sincronizado(const configuracion_experimento_t* config, 
                                      resultado_experimento_t* resultado) {
    if (config == NULL || resultado == NULL) {
        return false;
    }
    
    // Inicializar resultado
    if (!inicializar_resultado_experimento(resultado, config->num_hilos)) {
        return false;
    }
    
    // Variable compartida local para este experimento
    int contador_local = 0;
    
    uint64_t tiempo_inicio = obtener_tiempo_microsegundos();
    
    // Crear hilos
    pthread_t* hilos = malloc(config->num_hilos * sizeof(pthread_t));
    if (hilos == NULL) {
        limpiar_resultado_experimento(resultado);
        return false;
    }
    
    // Configurar parámetros para cada hilo
    for (int i = 0; i < config->num_hilos; i++) {
        resultado->resultados_hilos[i].contador = &contador_local;
        resultado->resultados_hilos[i].incrementos = config->incrementos_por_hilo;
        resultado->resultados_hilos[i].id_hilo = i;
        resultado->resultados_hilos[i].usar_delay = config->usar_delay;
        resultado->resultados_hilos[i].delay_microsegundos = config->delay_microsegundos;
    }
    
    // Crear hilos
    for (int i = 0; i < config->num_hilos; i++) {
        int ret = pthread_create(&hilos[i], NULL, hilo_incrementar_sincronizado, 
                                &resultado->resultados_hilos[i]);
        if (ret != 0) {
            fprintf(stderr, "Error creando hilo %d: %s\n", i, strerror(ret));
            // Limpiar hilos ya creados
            for (int j = 0; j < i; j++) {
                pthread_join(hilos[j], NULL);
            }
            free(hilos);
            limpiar_resultado_experimento(resultado);
            return false;
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < config->num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    uint64_t tiempo_fin = obtener_tiempo_microsegundos();
    
    // Llenar resultados
    resultado->valor_esperado = config->num_hilos * config->incrementos_por_hilo;
    resultado->valor_obtenido = contador_local;
    resultado->diferencia = resultado->valor_esperado - resultado->valor_obtenido;
    resultado->porcentaje_error = (double)abs(resultado->diferencia) * 100.0 / resultado->valor_esperado;
    resultado->tiempo_total_us = tiempo_fin - tiempo_inicio;
    resultado->num_hilos = config->num_hilos;
    resultado->incrementos_por_hilo = config->incrementos_por_hilo;
    
    free(hilos);
    return true;
}

// Funciones de inicialización y limpieza

bool inicializar_resultado_experimento(resultado_experimento_t* resultado, int num_hilos) {
    if (resultado == NULL || num_hilos <= 0) {
        return false;
    }
    
    memset(resultado, 0, sizeof(resultado_experimento_t));
    
    resultado->resultados_hilos = malloc(num_hilos * sizeof(parametros_incremento_t));
    if (resultado->resultados_hilos == NULL) {
        return false;
    }
    
    memset(resultado->resultados_hilos, 0, num_hilos * sizeof(parametros_incremento_t));
    return true;
}

void limpiar_resultado_experimento(resultado_experimento_t* resultado) {
    if (resultado == NULL) {
        return;
    }
    
    if (resultado->resultados_hilos != NULL) {
        free(resultado->resultados_hilos);
        resultado->resultados_hilos = NULL;
    }
    
    memset(resultado, 0, sizeof(resultado_experimento_t));
}

bool inicializar_analisis_estadistico(analisis_estadistico_t* analisis, int num_experimentos) {
    if (analisis == NULL || num_experimentos <= 0) {
        return false;
    }
    
    memset(analisis, 0, sizeof(analisis_estadistico_t));
    
    analisis->experimentos = malloc(num_experimentos * sizeof(resultado_experimento_t));
    if (analisis->experimentos == NULL) {
        return false;
    }
    
    memset(analisis->experimentos, 0, num_experimentos * sizeof(resultado_experimento_t));
    analisis->num_experimentos = num_experimentos;
    return true;
}

void limpiar_analisis_estadistico(analisis_estadistico_t* analisis) {
    if (analisis == NULL) {
        return;
    }
    
    if (analisis->experimentos != NULL) {
        for (int i = 0; i < analisis->num_experimentos; i++) {
            limpiar_resultado_experimento(&analisis->experimentos[i]);
        }
        free(analisis->experimentos);
        analisis->experimentos = NULL;
    }
    
    memset(analisis, 0, sizeof(analisis_estadistico_t));
}

// Funciones de análisis estadístico

bool ejecutar_analisis_estadistico(const configuracion_experimento_t* config, 
                                   int num_experimentos, 
                                   analisis_estadistico_t* analisis) {
    if (config == NULL || num_experimentos <= 0 || analisis == NULL) {
        return false;
    }
    
    if (!inicializar_analisis_estadistico(analisis, num_experimentos)) {
        return false;
    }
    
    printf("Ejecutando %d experimentos de race conditions...\n", num_experimentos);
    
    // Ejecutar todos los experimentos
    int experimentos_exitosos = 0;
    for (int i = 0; i < num_experimentos; i++) {
        printf("Experimento %d/%d...\r", i + 1, num_experimentos);
        fflush(stdout);
        
        if (ejecutar_experimento_race_condition(config, &analisis->experimentos[i])) {
            experimentos_exitosos++;
        }
    }
    printf("\n");
    
    if (experimentos_exitosos == 0) {
        limpiar_analisis_estadistico(analisis);
        return false;
    }
    
    // Calcular estadísticas
    int* valores = malloc(experimentos_exitosos * sizeof(int));
    int* diferencias = malloc(experimentos_exitosos * sizeof(int));
    uint64_t* tiempos = malloc(experimentos_exitosos * sizeof(uint64_t));
    double* errores = malloc(experimentos_exitosos * sizeof(double));
    
    if (valores == NULL || diferencias == NULL || tiempos == NULL || errores == NULL) {
        free(valores);
        free(diferencias);
        free(tiempos);
        free(errores);
        limpiar_analisis_estadistico(analisis);
        return false;
    }
    
    // Recopilar datos válidos
    int idx = 0;
    analisis->valor_esperado = config->num_hilos * config->incrementos_por_hilo;
    analisis->experimentos_correctos = 0;
    
    for (int i = 0; i < num_experimentos; i++) {
        if (analisis->experimentos[i].valor_esperado > 0) { // Experimento válido
            valores[idx] = analisis->experimentos[i].valor_obtenido;
            diferencias[idx] = analisis->experimentos[i].diferencia;
            tiempos[idx] = analisis->experimentos[i].tiempo_total_us;
            errores[idx] = analisis->experimentos[i].porcentaje_error;
            
            if (analisis->experimentos[i].valor_obtenido == analisis->experimentos[i].valor_esperado) {
                analisis->experimentos_correctos++;
            }
            
            idx++;
        }
    }
    
    // Calcular estadísticas de valores obtenidos
    calcular_estadisticas(valores, experimentos_exitosos, 
                         &analisis->valor_min, &analisis->valor_max, 
                         &analisis->valor_promedio, &analisis->desviacion_estandar);
    
    // Calcular estadísticas de diferencias
    calcular_estadisticas(diferencias, experimentos_exitosos, 
                         &analisis->diferencia_min, &analisis->diferencia_max, 
                         &analisis->diferencia_promedio, NULL);
    
    // Calcular estadísticas de tiempos
    calcular_estadisticas_tiempo(tiempos, experimentos_exitosos, 
                                &analisis->tiempo_min_us, &analisis->tiempo_max_us, 
                                &analisis->tiempo_promedio_us);
    
    // Calcular estadísticas de errores
    analisis->error_min = errores[0];
    analisis->error_max = errores[0];
    double suma_errores = 0;
    
    for (int i = 0; i < experimentos_exitosos; i++) {
        if (errores[i] < analisis->error_min) analisis->error_min = errores[i];
        if (errores[i] > analisis->error_max) analisis->error_max = errores[i];
        suma_errores += errores[i];
    }
    analisis->error_promedio = suma_errores / experimentos_exitosos;
    
    // Calcular tasa de inconsistencia
    analisis->tasa_inconsistencia = 
        (double)(experimentos_exitosos - analisis->experimentos_correctos) * 100.0 / experimentos_exitosos;
    
    free(valores);
    free(diferencias);
    free(tiempos);
    free(errores);
    
    return true;
}

bool ejecutar_comparacion_sincronizacion(const configuracion_experimento_t* config, 
                                        comparacion_sincronizacion_t* comparacion) {
    if (config == NULL || comparacion == NULL) {
        return false;
    }
    
    memset(comparacion, 0, sizeof(comparacion_sincronizacion_t));
    
    printf("Ejecutando experimento con race condition...\n");
    configuracion_experimento_t cfg_race = *config;
    cfg_race.usar_delay = false; // asegurar sin delay
    if (!ejecutar_experimento_race_condition(&cfg_race, &comparacion->experimento_race)) {
        return false;
    }
    
    printf("Ejecutando experimento sincronizado...\n");
    configuracion_experimento_t cfg_sync = *config;
    cfg_sync.usar_delay = true;
    cfg_sync.delay_microsegundos = (cfg_sync.delay_microsegundos > 0) ? cfg_sync.delay_microsegundos : 1;
    if (!ejecutar_experimento_sincronizado(&cfg_sync, &comparacion->experimento_seguro)) {
        limpiar_resultado_experimento(&comparacion->experimento_race);
        return false;
    }
    
    // Calcular métricas de comparación
    comparacion->overhead_sincronizacion_us = 
        comparacion->experimento_seguro.tiempo_total_us - comparacion->experimento_race.tiempo_total_us;
    
    if (comparacion->experimento_race.tiempo_total_us > 0) {
        comparacion->factor_slowdown = 
            (double)comparacion->experimento_seguro.tiempo_total_us / comparacion->experimento_race.tiempo_total_us;
    } else {
        comparacion->factor_slowdown = 1.0;
    }
    
    return true;
}

// Funciones de impresión

void imprimir_resultado_experimento(const resultado_experimento_t* resultado) {
    if (resultado == NULL) {
        return;
    }
    
    printf("\n=== Resultado del Experimento ===\n");
    printf("Configuración:\n");
    printf("  Hilos: %d\n", resultado->num_hilos);
    printf("  Incrementos por hilo: %d\n", resultado->incrementos_por_hilo);
    printf("  Total de incrementos: %d\n", resultado->num_hilos * resultado->incrementos_por_hilo);
    
    printf("\nResultados:\n");
    printf("  Valor esperado: %d\n", resultado->valor_esperado);
    printf("  Valor obtenido: %d\n", resultado->valor_obtenido);
    printf("  Diferencia: %d\n", resultado->diferencia);
    printf("  Error: %.2f%%\n", resultado->porcentaje_error);
    printf("  Tiempo total: %lu μs (%.3f ms)\n", 
           resultado->tiempo_total_us, resultado->tiempo_total_us / 1000.0);
    
    if (resultado->diferencia == 0) {
        printf("  Estado: ✓ CORRECTO\n");
    } else {
        printf("  Estado: ✗ RACE CONDITION DETECTADA\n");
        printf("  Incrementos perdidos: %d\n", resultado->diferencia);
    }
}

void imprimir_analisis_estadistico(const analisis_estadistico_t* analisis) {
    if (analisis == NULL) {
        return;
    }
    
    printf("\n=== Análisis Estadístico de Race Conditions ===\n");
    printf("Número de experimentos: %d\n", analisis->num_experimentos);
    printf("Valor esperado (constante): %d\n", analisis->valor_esperado);
    
    printf("\nEstadísticas de valores obtenidos:\n");
    printf("  Mínimo: %d\n", analisis->valor_min);
    printf("  Máximo: %d\n", analisis->valor_max);
    printf("  Promedio: %.2f\n", analisis->valor_promedio);
    printf("  Desviación estándar: %.2f\n", analisis->desviacion_estandar);
    
    printf("\nEstadísticas de diferencias (incrementos perdidos):\n");
    printf("  Mínima: %d\n", analisis->diferencia_min);
    printf("  Máxima: %d\n", analisis->diferencia_max);
    printf("  Promedio: %.2f\n", analisis->diferencia_promedio);
    
    printf("\nEstadísticas de error:\n");
    printf("  Error mínimo: %.2f%%\n", analisis->error_min);
    printf("  Error máximo: %.2f%%\n", analisis->error_max);
    printf("  Error promedio: %.2f%%\n", analisis->error_promedio);
    
    printf("\nEstadísticas de tiempo:\n");
    printf("  Tiempo mínimo: %lu μs\n", analisis->tiempo_min_us);
    printf("  Tiempo máximo: %lu μs\n", analisis->tiempo_max_us);
    printf("  Tiempo promedio: %.2f μs\n", analisis->tiempo_promedio_us);
    
    printf("\nAnálisis de consistencia:\n");
    printf("  Experimentos correctos: %d/%d\n", 
           analisis->experimentos_correctos, analisis->num_experimentos);
    printf("  Tasa de inconsistencia: %.2f%%\n", analisis->tasa_inconsistencia);
    
    if (analisis->tasa_inconsistencia > 90.0) {
        printf("  Evaluación: Race conditions MUY FRECUENTES\n");
    } else if (analisis->tasa_inconsistencia > 50.0) {
        printf("  Evaluación: Race conditions FRECUENTES\n");
    } else if (analisis->tasa_inconsistencia > 10.0) {
        printf("  Evaluación: Race conditions OCASIONALES\n");
    } else if (analisis->tasa_inconsistencia > 0.0) {
        printf("  Evaluación: Race conditions RARAS\n");
    } else {
        printf("  Evaluación: NO se detectaron race conditions\n");
    }
}

void imprimir_comparacion_sincronizacion(const comparacion_sincronizacion_t* comparacion) {
    if (comparacion == NULL) {
        return;
    }
    
    printf("\n=== Comparación: Race Condition vs Sincronizado ===\n");
    
    printf("\nExperimento con Race Condition:\n");
    printf("  Valor obtenido: %d\n", comparacion->experimento_race.valor_obtenido);
    printf("  Diferencia: %d\n", comparacion->experimento_race.diferencia);
    printf("  Error: %.2f%%\n", comparacion->experimento_race.porcentaje_error);
    printf("  Tiempo: %lu μs\n", comparacion->experimento_race.tiempo_total_us);
    
    printf("\nExperimento Sincronizado:\n");
    printf("  Valor obtenido: %d\n", comparacion->experimento_seguro.valor_obtenido);
    printf("  Diferencia: %d\n", comparacion->experimento_seguro.diferencia);
    printf("  Error: %.2f%%\n", comparacion->experimento_seguro.porcentaje_error);
    printf("  Tiempo: %lu μs\n", comparacion->experimento_seguro.tiempo_total_us);
    
    printf("\nAnálisis de rendimiento:\n");
    printf("  Overhead de sincronización: %lu μs\n", comparacion->overhead_sincronizacion_us);
    printf("  Factor de ralentización: %.2fx\n", comparacion->factor_slowdown);
    
    if (comparacion->experimento_race.diferencia == 0) {
        printf("  Observación: No se detectó race condition en esta ejecución\n");
    } else {
        printf("  Observación: Race condition detectada - %d incrementos perdidos\n", 
               comparacion->experimento_race.diferencia);
    }
    
    printf("\n✓ La sincronización GARANTIZA correctitud\n");
    if (comparacion->factor_slowdown > 1.5) {
        printf("⚠ La sincronización tiene overhead significativo (%.1fx más lento)\n", 
               comparacion->factor_slowdown);
    } else {
        printf("✓ El overhead de sincronización es razonable (%.1fx más lento)\n", 
               comparacion->factor_slowdown);
    }
}

// Funciones utilitarias

uint64_t obtener_tiempo_microsegundos(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void calcular_estadisticas(const int* valores, int n, int* min, int* max, 
                          double* promedio, double* desviacion) {
    if (valores == NULL || n <= 0) {
        if (min) *min = 0;
        if (max) *max = 0;
        if (promedio) *promedio = 0.0;
        if (desviacion) *desviacion = 0.0;
        return;
    }
    
    int min_val = valores[0];
    int max_val = valores[0];
    long long suma = 0;
    
    for (int i = 0; i < n; i++) {
        if (valores[i] < min_val) min_val = valores[i];
        if (valores[i] > max_val) max_val = valores[i];
        suma += valores[i];
    }
    
    if (min) *min = min_val;
    if (max) *max = max_val;
    
    double media = (double)suma / n;
    if (promedio) *promedio = media;
    
    if (desviacion && n > 1) {
        double suma_cuadrados = 0.0;
        for (int i = 0; i < n; i++) {
            double diff = valores[i] - media;
            suma_cuadrados += diff * diff;
        }
        *desviacion = sqrt(suma_cuadrados / (n - 1));
    } else if (desviacion) {
        *desviacion = 0.0;
    }
}

void calcular_estadisticas_tiempo(const uint64_t* tiempos, int n, 
                                 uint64_t* min, uint64_t* max, double* promedio) {
    if (tiempos == NULL || n <= 0) {
        if (min) *min = 0;
        if (max) *max = 0;
        if (promedio) *promedio = 0.0;
        return;
    }
    
    uint64_t min_val = tiempos[0];
    uint64_t max_val = tiempos[0];
    uint64_t suma = 0;
    
    for (int i = 0; i < n; i++) {
        if (tiempos[i] < min_val) min_val = tiempos[i];
        if (tiempos[i] > max_val) max_val = tiempos[i];
        suma += tiempos[i];
    }
    
    if (min) *min = min_val;
    if (max) *max = max_val;
    if (promedio) *promedio = (double)suma / n;
}

// Funciones de demostración

bool ejecutar_ejemplo_basico_enunciado(void) {
    printf("=== Ejemplo Básico del Enunciado ===\n");
    printf("Dos hilos incrementando un contador 100,000 veces cada uno\n");
    printf("SIN sincronización (race condition esperada)\n\n");
    
    configuracion_experimento_t config = {
        .num_hilos = 2,
        .incrementos_por_hilo = NUM_INCREMENTOS_DEFAULT,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = true
    };
    
    resultado_experimento_t resultado;
    if (ejecutar_experimento_race_condition(&config, &resultado)) {
        imprimir_resultado_experimento(&resultado);
        
        printf("\n=== Explicación ===\n");
        if (resultado.diferencia == 0) {
            printf("En esta ejecución NO se detectó race condition.\n");
            printf("Esto puede ocurrir ocasionalmente por casualidad.\n");
            printf("Ejecute el programa varias veces para observar la inconsistencia.\n");
        } else {
            printf("Se detectó una race condition típica.\n");
            printf("Los %d incrementos perdidos se deben a que múltiples hilos\n", resultado.diferencia);
            printf("leyeron el mismo valor del contador antes de incrementarlo,\n");
            printf("causando que algunos incrementos se \"pierdan\".\n");
        }
        
        limpiar_resultado_experimento(&resultado);
        return true;
    }
    
    return false;
}

bool ejecutar_demo_completa_race_conditions(void) {
    printf("=== Demostración Completa de Race Conditions ===\n\n");
    
    // 1. Ejemplo básico
    printf("1. EJEMPLO BÁSICO:\n");
    ejecutar_ejemplo_basico_enunciado();
    
    // 2. Análisis estadístico
    printf("\n\n2. ANÁLISIS ESTADÍSTICO:\n");
    printf("Ejecutando múltiples experimentos para observar la variabilidad...\n");
    
    configuracion_experimento_t config = {
        .num_hilos = 2,
        .incrementos_por_hilo = 50000,  // Reducido para rapidez
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    analisis_estadistico_t analisis;
    if (ejecutar_analisis_estadistico(&config, 20, &analisis)) {
        imprimir_analisis_estadistico(&analisis);
        limpiar_analisis_estadistico(&analisis);
    }
    
    // 3. Comparación con versión sincronizada
    printf("\n\n3. COMPARACIÓN CON SINCRONIZACIÓN:\n");
    
    config.incrementos_por_hilo = 100000;  // Restaurar valor original
    
    comparacion_sincronizacion_t comparacion;
    if (ejecutar_comparacion_sincronizacion(&config, &comparacion)) {
        imprimir_comparacion_sincronizacion(&comparacion);
        limpiar_resultado_experimento(&comparacion.experimento_race);
        limpiar_resultado_experimento(&comparacion.experimento_seguro);
    }
    
    printf("\n=== Demostración Completada ===\n");
    return true;
}

bool demostrar_factores_race_conditions(void) {
    printf("=== Factores que Afectan las Race Conditions ===\n\n");
    
    // Factor 1: Número de hilos
    printf("1. EFECTO DEL NÚMERO DE HILOS:\n");
    configuracion_experimento_t config = {
        .incrementos_por_hilo = 10000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    int hilos_prueba[] = {2, 4, 8};
    int num_pruebas = sizeof(hilos_prueba) / sizeof(hilos_prueba[0]);
    
    for (int i = 0; i < num_pruebas; i++) {
        config.num_hilos = hilos_prueba[i];
        
        printf("\nCon %d hilos:\n", config.num_hilos);
        
        analisis_estadistico_t analisis;
        if (ejecutar_analisis_estadistico(&config, 10, &analisis)) {
            printf("  Tasa de inconsistencia: %.1f%%\n", analisis.tasa_inconsistencia);
            printf("  Error promedio: %.2f%%\n", analisis.error_promedio);
            printf("  Incrementos perdidos (promedio): %.0f\n", analisis.diferencia_promedio);
            limpiar_analisis_estadistico(&analisis);
        }
    }
    
    // Factor 2: Cantidad de trabajo
    printf("\n\n2. EFECTO DE LA CANTIDAD DE TRABAJO:\n");
    config.num_hilos = 2;
    
    int incrementos_prueba[] = {1000, 10000, 100000};
    num_pruebas = sizeof(incrementos_prueba) / sizeof(incrementos_prueba[0]);
    
    for (int i = 0; i < num_pruebas; i++) {
        config.incrementos_por_hilo = incrementos_prueba[i];
        
        printf("\nCon %d incrementos por hilo:\n", config.incrementos_por_hilo);
        
        analisis_estadistico_t analisis;
        if (ejecutar_analisis_estadistico(&config, 5, &analisis)) {
            printf("  Tasa de inconsistencia: %.1f%%\n", analisis.tasa_inconsistencia);
            printf("  Error promedio: %.2f%%\n", analisis.error_promedio);
            limpiar_analisis_estadistico(&analisis);
        }
    }
    
    printf("\n=== Conclusiones ===\n");
    printf("• Más hilos → Mayor probabilidad de race conditions\n");
    printf("• Más trabajo → Race conditions más evidentes en porcentaje\n");
    printf("• Los resultados son no determinísticos por naturaleza\n");
    printf("• La sincronización es ESENCIAL para correctitud\n");
    
    return true;
}

double estimar_probabilidad_race_condition(int num_hilos, int incrementos_por_hilo, 
                                          int delay_microsegundos) {
    // Modelo simple y heurístico - no es matemáticamente preciso
    double factor_hilos = (num_hilos - 1) * 0.3;  // Más hilos, más probabilidad
    double factor_trabajo = 1.0 - exp(-incrementos_por_hilo / 50000.0);  // Saturación asintótica
    double factor_delay = delay_microsegundos > 0 ? 0.8 : 1.0;  // Delay reduce contención
    
    double probabilidad = factor_hilos * factor_trabajo * factor_delay;
    
    // Limitar entre 0 y 1
    if (probabilidad > 1.0) probabilidad = 1.0;
    if (probabilidad < 0.0) probabilidad = 0.0;
    
    return probabilidad;
}

bool simular_escenarios_race_conditions(void) {
    printf("=== Simulación de Diferentes Escenarios ===\n\n");
    
    struct {
        const char* nombre;
        int num_hilos;
        int incrementos;
        bool usar_delay;
        int delay_us;
    } escenarios[] = {
        {"Carga ligera", 2, 1000, false, 0},
        {"Carga moderada", 2, 50000, false, 0},
        {"Carga pesada", 2, 200000, false, 0},
        {"Muchos hilos", 8, 10000, false, 0},
        {"Con delays (menos contención)", 2, 10000, true, 1}
    };
    
    int num_escenarios = sizeof(escenarios) / sizeof(escenarios[0]);
    
    for (int i = 0; i < num_escenarios; i++) {
        printf("%d. %s:\n", i + 1, escenarios[i].nombre);
        printf("   Configuración: %d hilos, %d incrementos/hilo", 
               escenarios[i].num_hilos, escenarios[i].incrementos);
        if (escenarios[i].usar_delay) {
            printf(", delay %d μs", escenarios[i].delay_us);
        }
        printf("\n");
        
        configuracion_experimento_t config = {
            .num_hilos = escenarios[i].num_hilos,
            .incrementos_por_hilo = escenarios[i].incrementos,
            .usar_delay = escenarios[i].usar_delay,
            .delay_microsegundos = escenarios[i].delay_us,
            .mostrar_detalles = false
        };
        
        double prob_estimada = estimar_probabilidad_race_condition(
            config.num_hilos, config.incrementos_por_hilo, config.delay_microsegundos);
        printf("   Probabilidad estimada de race condition: %.1f%%\n", prob_estimada * 100.0);
        
        analisis_estadistico_t analisis;
        if (ejecutar_analisis_estadistico(&config, 5, &analisis)) {
            printf("   Resultado real: %.1f%% de inconsistencia\n", analisis.tasa_inconsistencia);
            printf("   Error promedio: %.2f%%\n", analisis.error_promedio);
            limpiar_analisis_estadistico(&analisis);
        }
        printf("\n");
    }
    
    return true;
}
