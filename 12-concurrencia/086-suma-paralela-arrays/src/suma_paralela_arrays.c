#include "../include/suma_paralela_arrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

// Implementación de funciones básicas

void* hilo_suma_parcial(void* arg) {
    parametros_suma_t* params = (parametros_suma_t*)arg;
    if (params == NULL) {
        return NULL;
    }
    
    // Registrar tiempo de inicio
    params->tiempo_inicio = obtener_tiempo_microsegundos();
    
    // Calcular suma parcial
    params->resultado = 0;
    for (size_t i = params->inicio; i < params->fin; i++) {
        params->resultado += params->array[i];
    }
    
    // Registrar tiempo de fin
    params->tiempo_fin = obtener_tiempo_microsegundos();
    if (params->tiempo_fin <= params->tiempo_inicio) {
        params->tiempo_fin = params->tiempo_inicio + 1; // Garantizar tiempo positivo
    }
    
    return NULL;
}

bool suma_paralela(const configuracion_suma_t* config, resultado_suma_paralela_t* resultado) {
    if (config == NULL || resultado == NULL || config->array == NULL || 
        config->tamano == 0 || config->num_hilos <= 0) {
        return false;
    }
    
    // Ajustar número de hilos si es mayor que el tamaño del array
    int num_hilos_efectivo = (config->num_hilos > (int)config->tamano) ? 
                             (int)config->tamano : config->num_hilos;
    
    // Inicializar resultado
    if (!inicializar_resultado_suma(resultado, num_hilos_efectivo)) {
        return false;
    }
    
    uint64_t tiempo_inicio = obtener_tiempo_microsegundos();
    
    // Crear hilos
    pthread_t* hilos = malloc(num_hilos_efectivo * sizeof(pthread_t));
    if (hilos == NULL) {
        limpiar_resultado_suma(resultado);
        return false;
    }
    
    // Dividir trabajo entre hilos
    size_t* chunks = malloc((num_hilos_efectivo + 1) * sizeof(size_t));
    if (chunks == NULL) {
        free(hilos);
        limpiar_resultado_suma(resultado);
        return false;
    }
    
    dividir_rango_en_chunks(0, config->tamano, num_hilos_efectivo, chunks);
    
    // Configurar parámetros para cada hilo
    for (int i = 0; i < num_hilos_efectivo; i++) {
        resultado->resultados_hilos[i].array = config->array;
        resultado->resultados_hilos[i].inicio = chunks[i];
        resultado->resultados_hilos[i].fin = chunks[i + 1];
        resultado->resultados_hilos[i].id_hilo = i;
        resultado->resultados_hilos[i].resultado = 0;
    }
    
    // Crear hilos
    for (int i = 0; i < num_hilos_efectivo; i++) {
        int resultado_create = pthread_create(&hilos[i], NULL, hilo_suma_parcial, 
                                            &resultado->resultados_hilos[i]);
        if (resultado_create != 0) {
            fprintf(stderr, "Error creando hilo %d: %s\n", i, strerror(resultado_create));
            // Esperar hilos ya creados
            for (int j = 0; j < i; j++) {
                pthread_join(hilos[j], NULL);
            }
            free(hilos);
            free(chunks);
            limpiar_resultado_suma(resultado);
            return false;
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < num_hilos_efectivo; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    uint64_t tiempo_fin = obtener_tiempo_microsegundos();
    
    // Agregar resultados parciales
    resultado->suma_total = 0;
    for (int i = 0; i < num_hilos_efectivo; i++) {
        resultado->suma_total += resultado->resultados_hilos[i].resultado;
    }
    
    resultado->tiempo_total_us = tiempo_fin - tiempo_inicio;
    resultado->num_hilos_usados = num_hilos_efectivo;
    
    // Mostrar detalles si se solicita
    if (config->mostrar_detalles) {
        printf("\n=== Detalles de Suma Paralela ===\n");
        printf("Hilos utilizados: %d\n", num_hilos_efectivo);
        printf("Tamaño del array: %zu\n", config->tamano);
        
        for (int i = 0; i < num_hilos_efectivo; i++) {
            printf("Hilo %d: rango [%zu, %zu), suma=%ld, tiempo=%lu us\n",
                   resultado->resultados_hilos[i].id_hilo,
                   resultado->resultados_hilos[i].inicio,
                   resultado->resultados_hilos[i].fin,
                   resultado->resultados_hilos[i].resultado,
                   resultado->resultados_hilos[i].tiempo_fin - 
                   resultado->resultados_hilos[i].tiempo_inicio);
        }
    }
    
    free(hilos);
    free(chunks);
    return true;
}

int64_t suma_secuencial(const int* array, size_t tamano, uint64_t* tiempo_us) {
    if (array == NULL || tamano == 0) {
        if (tiempo_us) *tiempo_us = 0;
        return 0;
    }
    
    uint64_t inicio = obtener_tiempo_microsegundos();
    
    int64_t suma = 0;
    for (size_t i = 0; i < tamano; i++) {
        suma += array[i];
    }
    
    uint64_t fin = obtener_tiempo_microsegundos();
    
    if (tiempo_us) {
        uint64_t dur = fin - inicio;
        *tiempo_us = (dur == 0) ? 1 : dur; // Evitar tiempo cero en arrays pequeños
    }
    
    return suma;
}

bool inicializar_resultado_suma(resultado_suma_paralela_t* resultado, int num_hilos) {
    if (resultado == NULL || num_hilos <= 0) {
        return false;
    }
    
    memset(resultado, 0, sizeof(resultado_suma_paralela_t));
    
    resultado->resultados_hilos = malloc(num_hilos * sizeof(parametros_suma_t));
    if (resultado->resultados_hilos == NULL) {
        return false;
    }
    
    memset(resultado->resultados_hilos, 0, num_hilos * sizeof(parametros_suma_t));
    return true;
}

void limpiar_resultado_suma(resultado_suma_paralela_t* resultado) {
    if (resultado == NULL) {
        return;
    }
    
    if (resultado->resultados_hilos != NULL) {
        free(resultado->resultados_hilos);
        resultado->resultados_hilos = NULL;
    }
    
    memset(resultado, 0, sizeof(resultado_suma_paralela_t));
}

// Funciones de creación de arrays

int* crear_array_aleatorio(size_t tamano, int valor_min, int valor_max) {
    if (tamano == 0 || valor_min > valor_max) {
        return NULL;
    }
    
    int* array = malloc(tamano * sizeof(int));
    if (array == NULL) {
        return NULL;
    }
    
    // Inicializar generador de números aleatorios
    static bool inicializado = false;
    if (!inicializado) {
        srand((unsigned int)time(NULL));
        inicializado = true;
    }
    
    int rango = valor_max - valor_min + 1;
    for (size_t i = 0; i < tamano; i++) {
        array[i] = valor_min + (rand() % rango);
    }
    
    return array;
}

int* crear_array_secuencia(size_t tamano, int inicio, int incremento) {
    if (tamano == 0) {
        return NULL;
    }
    
    int* array = malloc(tamano * sizeof(int));
    if (array == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < tamano; i++) {
        array[i] = inicio + (int)(i * incremento);
    }
    
    return array;
}

bool verificar_sumas_iguales(int64_t suma1, int64_t suma2) {
    return suma1 == suma2;
}

// Funciones de benchmark

bool ejecutar_benchmark_suma(const int* array, size_t tamano, int num_hilos, 
                            benchmark_suma_t* benchmark) {
    if (array == NULL || tamano == 0 || num_hilos <= 0 || benchmark == NULL) {
        return false;
    }
    
    memset(benchmark, 0, sizeof(benchmark_suma_t));
    benchmark->num_hilos = num_hilos;
    benchmark->tamano_array = tamano;
    
    // Suma secuencial
    benchmark->suma_secuencial = suma_secuencial(array, tamano, 
                                                &benchmark->tiempo_secuencial_us);
    
    // Suma paralela
    configuracion_suma_t config = {
        .array = array,
        .tamano = tamano,
        .num_hilos = num_hilos,
        .mostrar_detalles = false
    };
    
    resultado_suma_paralela_t resultado;
    if (!suma_paralela(&config, &resultado)) {
        return false;
    }
    
    benchmark->suma_paralela = resultado.suma_total;
    benchmark->tiempo_paralelo_us = resultado.tiempo_total_us;
    
    // Calcular métricas
    if (benchmark->tiempo_secuencial_us > 0) {
        benchmark->speedup = (double)benchmark->tiempo_secuencial_us / 
                           benchmark->tiempo_paralelo_us;
        benchmark->eficiencia = benchmark->speedup / num_hilos;
    }
    
    limpiar_resultado_suma(&resultado);
    
    return verificar_sumas_iguales(benchmark->suma_secuencial, benchmark->suma_paralela);
}

// Funciones de impresión

void imprimir_resultados_suma_paralela(const resultado_suma_paralela_t* resultado) {
    if (resultado == NULL) {
        return;
    }
    
    printf("\n=== Resultados de Suma Paralela ===\n");
    printf("Suma total: %ld\n", resultado->suma_total);
    printf("Tiempo total: %lu microsegundos (%.3f ms)\n", 
           resultado->tiempo_total_us, resultado->tiempo_total_us / 1000.0);
    printf("Hilos utilizados: %d\n", resultado->num_hilos_usados);
    
    if (resultado->speedup > 0) {
        printf("Speedup: %.2fx\n", resultado->speedup);
        printf("Eficiencia: %.2f%%\n", resultado->eficiencia * 100.0);
    }
}

void imprimir_benchmark(const benchmark_suma_t* benchmark) {
    if (benchmark == NULL) {
        return;
    }
    
    printf("\n=== Benchmark de Suma ===\n");
    printf("Tamaño del array: %zu elementos\n", benchmark->tamano_array);
    printf("Número de hilos: %d\n", benchmark->num_hilos);
    printf("\nResultados:\n");
    printf("  Suma secuencial: %ld (%lu us = %.3f ms)\n", 
           benchmark->suma_secuencial, benchmark->tiempo_secuencial_us,
           benchmark->tiempo_secuencial_us / 1000.0);
    printf("  Suma paralela:   %ld (%lu us = %.3f ms)\n", 
           benchmark->suma_paralela, benchmark->tiempo_paralelo_us,
           benchmark->tiempo_paralelo_us / 1000.0);
    
    printf("\nMétricas de rendimiento:\n");
    printf("  Speedup:     %.2fx\n", benchmark->speedup);
    printf("  Eficiencia:  %.2f%%\n", benchmark->eficiencia * 100.0);
    
    bool correcto = verificar_sumas_iguales(benchmark->suma_secuencial, 
                                          benchmark->suma_paralela);
    printf("  Correctitud: %s\n", correcto ? "✓ CORRECTO" : "✗ ERROR");
    
    if (benchmark->speedup > 1.0) {
        printf("  Ganancia:    %.1fx más rápido\n", benchmark->speedup);
    } else if (benchmark->speedup < 1.0) {
        printf("  Pérdida:     %.1fx más lento\n", 1.0 / benchmark->speedup);
    }
}

// Funciones utilitarias

int calcular_num_hilos_optimo(size_t tamano_array) {
    int num_cpus = obtener_num_cpus();
    if (num_cpus <= 0) {
        num_cpus = 4; // Valor por defecto
    }
    
    // Para arrays pequeños, usar menos hilos
    if (tamano_array < 1000) {
        return 1;
    } else if (tamano_array < 10000) {
        return (num_cpus > 2) ? 2 : num_cpus;
    } else {
        return num_cpus;
    }
}

void dividir_rango_en_chunks(size_t inicio, size_t fin, int num_hilos, size_t* chunks) {
    if (chunks == NULL || num_hilos <= 0 || fin <= inicio) {
        return;
    }
    
    size_t tamano_total = fin - inicio;
    size_t chunk_base = tamano_total / num_hilos;
    size_t elementos_extra = tamano_total % num_hilos;
    
    chunks[0] = inicio;
    
    for (int i = 1; i <= num_hilos; i++) {
        size_t tamano_chunk = chunk_base + (i <= (int)elementos_extra ? 1 : 0);
        chunks[i] = chunks[i-1] + tamano_chunk;
    }
}

uint64_t obtener_tiempo_microsegundos(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

int obtener_num_cpus(void) {
#ifdef _SC_NPROCESSORS_ONLN
    long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    return (num_cpus > 0) ? (int)num_cpus : -1;
#else
    return -1; // No se puede determinar
#endif
}

// Funciones de pruebas avanzadas

bool ejecutar_pruebas_escalabilidad(const int* array, size_t tamano, int max_hilos) {
    if (array == NULL || tamano == 0 || max_hilos <= 0) {
        return false;
    }
    
    printf("\n=== Pruebas de Escalabilidad ===\n");
    printf("Tamaño del array: %zu elementos\n", tamano);
    printf("Probando de 1 a %d hilos\n\n", max_hilos);
    
    // Benchmark secuencial como referencia
    uint64_t tiempo_secuencial;
    int64_t suma_referencia = suma_secuencial(array, tamano, &tiempo_secuencial);
    
    printf("%-6s %-12s %-12s %-8s %-10s %-8s\n", 
           "Hilos", "Tiempo(ms)", "Speedup", "Efic(%)", "Suma", "Estado");
    printf("%-6s %-12s %-12s %-8s %-10s %-8s\n", 
           "-----", "---------", "-------", "-----", "----", "------");
    
    // Imprimir referencia secuencial
    printf("%-6d %-12.3f %-12.2f %-8.1f %-10ld %-8s\n",
           1, tiempo_secuencial / 1000.0, 1.0, 100.0, suma_referencia, "REF");
    
    for (int num_hilos = 2; num_hilos <= max_hilos; num_hilos++) {
        benchmark_suma_t benchmark;
        bool exito = ejecutar_benchmark_suma(array, tamano, num_hilos, &benchmark);
        
        if (exito) {
            bool correcto = verificar_sumas_iguales(suma_referencia, benchmark.suma_paralela);
            printf("%-6d %-12.3f %-12.2f %-8.1f %-10ld %-8s\n",
                   num_hilos, 
                   benchmark.tiempo_paralelo_us / 1000.0,
                   benchmark.speedup,
                   benchmark.eficiencia * 100.0,
                   benchmark.suma_paralela,
                   correcto ? "OK" : "ERROR");
        } else {
            printf("%-6d %-12s %-12s %-8s %-10s %-8s\n",
                   num_hilos, "ERROR", "N/A", "N/A", "N/A", "FAIL");
        }
    }
    
    return true;
}

bool ejecutar_demo_completa_suma_paralela(void) {
    printf("=== Demostración Completa de Suma Paralela ===\n\n");
    
    // 1. Ejemplo básico como en el enunciado
    printf("1. Ejemplo Básico (10 elementos):\n");
    int array_basico[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t tamano_basico = sizeof(array_basico) / sizeof(array_basico[0]);
    
    configuracion_suma_t config_basico = {
        .array = array_basico,
        .tamano = tamano_basico,
        .num_hilos = 2,
        .mostrar_detalles = true
    };
    
    resultado_suma_paralela_t resultado_basico;
    if (suma_paralela(&config_basico, &resultado_basico)) {
        printf("Suma total: %ld\n", resultado_basico.suma_total);
        printf("Tiempo: %lu microsegundos\n", resultado_basico.tiempo_total_us);
        limpiar_resultado_suma(&resultado_basico);
    }
    
    // 2. Array más grande para ver beneficios del paralelismo
    printf("\n2. Array Grande (1,000,000 elementos):\n");
    size_t tamano_grande = 1000000;
    int* array_grande = crear_array_secuencia(tamano_grande, 1, 1);
    
    if (array_grande != NULL) {
        int num_hilos_optimo = calcular_num_hilos_optimo(tamano_grande);
        printf("Número de hilos óptimo calculado: %d\n", num_hilos_optimo);
        
        benchmark_suma_t benchmark_grande;
        if (ejecutar_benchmark_suma(array_grande, tamano_grande, num_hilos_optimo, 
                                  &benchmark_grande)) {
            imprimir_benchmark(&benchmark_grande);
        }
        
        // 3. Pruebas de escalabilidad
        printf("\n3. Análisis de Escalabilidad:\n");
        int max_hilos = obtener_num_cpus();
        if (max_hilos <= 0) max_hilos = 8;
        ejecutar_pruebas_escalabilidad(array_grande, tamano_grande, max_hilos);
        
        free(array_grande);
    }
    
    // 4. Diferentes tipos de arrays
    printf("\n4. Pruebas con Diferentes Tipos de Arrays:\n");
    
    // Array aleatorio
    printf("\nArray aleatorio:\n");
    int* array_aleatorio = crear_array_aleatorio(100000, -100, 100);
    if (array_aleatorio != NULL) {
        benchmark_suma_t bench_aleatorio;
        if (ejecutar_benchmark_suma(array_aleatorio, 100000, 4, &bench_aleatorio)) {
            printf("Suma: %ld, Speedup: %.2fx\n", 
                   bench_aleatorio.suma_paralela, bench_aleatorio.speedup);
        }
        free(array_aleatorio);
    }
    
    printf("\n=== Demostración Completada ===\n");
    return true;
}

// Funciones de validación y estadísticas

void calcular_estadisticas_array(const int* array, size_t tamano, int* min, int* max, double* promedio) {
    if (array == NULL || tamano == 0) {
        if (min) *min = 0;
        if (max) *max = 0;
        if (promedio) *promedio = 0.0;
        return;
    }
    
    int min_val = array[0];
    int max_val = array[0];
    int64_t suma = 0;
    
    for (size_t i = 0; i < tamano; i++) {
        if (array[i] < min_val) min_val = array[i];
        if (array[i] > max_val) max_val = array[i];
        suma += array[i];
    }
    
    if (min) *min = min_val;
    if (max) *max = max_val;
    if (promedio) *promedio = (double)suma / tamano;
}

bool validar_array_para_suma(const int* array, size_t tamano) {
    if (array == NULL || tamano == 0) {
        return false;
    }
    
    // Verificar que la suma no cause overflow
    int64_t suma_positiva = 0;
    int64_t suma_negativa = 0;
    
    for (size_t i = 0; i < tamano; i++) {
        if (array[i] > 0) {
            if (suma_positiva > INT64_MAX - array[i]) {
                return false; // Overflow positivo
            }
            suma_positiva += array[i];
        } else if (array[i] < 0) {
            if (suma_negativa < INT64_MIN - array[i]) {
                return false; // Overflow negativo
            }
            suma_negativa += array[i];
        }
    }
    
    // Verificar que la suma total no cause overflow
    if (suma_positiva > 0 && suma_negativa < 0) {
        if (suma_positiva > INT64_MAX + suma_negativa) {
            return false;
        }
    }
    
    return true;
}
