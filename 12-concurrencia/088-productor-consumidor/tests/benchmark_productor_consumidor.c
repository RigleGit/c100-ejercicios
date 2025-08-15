/**
 * @file benchmark_productor_consumidor.c
 * @brief Benchmark y análisis de rendimiento para patrón productor-consumidor
 * @description Tests de rendimiento, throughput, latencia y escalabilidad
 *              para evaluar la eficiencia del patrón productor-consumidor.
 * @version 1.0
 * @date 2024
 * @author Estudiante de C
 */

#include <criterion/criterion.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "../include/productor_consumidor.h"

/**
 * @brief Configuración para benchmarks
 */
TestSuite(benchmark_productor_consumidor, 
          .description = "Benchmarks de rendimiento para productor-consumidor");

/**
 * @brief Estructura para datos de benchmark
 */
typedef struct {
    buffer_circular_t *buffer;
    int num_items;
    int items_procesados;
    double tiempo_inicio;
    double tiempo_fin;
    pthread_mutex_t *mutex_stats;
    int *estadisticas;
} benchmark_data_t;

/**
 * @brief Obtener tiempo actual en segundos
 */
static double obtener_tiempo_actual(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

/**
 * @brief Productor para benchmark de throughput
 */
static void *productor_benchmark(void *arg) {
    benchmark_data_t *data = (benchmark_data_t *)arg;
    
    data->tiempo_inicio = obtener_tiempo_actual();
    
    for (int i = 0; i < data->num_items; i++) {
        producir_item(data->buffer, i);
        
        pthread_mutex_lock(data->mutex_stats);
        data->estadisticas[0]++; // Items producidos
        pthread_mutex_unlock(data->mutex_stats);
    }
    
    data->tiempo_fin = obtener_tiempo_actual();
    return NULL;
}

/**
 * @brief Consumidor para benchmark de throughput
 */
static void *consumidor_benchmark(void *arg) {
    benchmark_data_t *data = (benchmark_data_t *)arg;
    int item;
    int consumidos = 0;
    
    double inicio = obtener_tiempo_actual();
    
    while (consumidos < data->num_items) {
        if (consumir_item(data->buffer, &item) == 0) {
            consumidos++;
            
            pthread_mutex_lock(data->mutex_stats);
            data->estadisticas[1]++; // Items consumidos
            pthread_mutex_unlock(data->mutex_stats);
        }
    }
    
    data->tiempo_fin = obtener_tiempo_actual();
    return NULL;
}

/**
 * @brief Benchmark de throughput básico
 */
Test(benchmark_productor_consumidor, benchmark_throughput_basico, .timeout = 15.0) {
    const int NUM_ITEMS = 50000;
    buffer_circular_t *buffer = crear_buffer(1000);
    
    pthread_t productor_thread, consumidor_thread;
    pthread_mutex_t mutex_stats = PTHREAD_MUTEX_INITIALIZER;
    int estadisticas[2] = {0, 0}; // [producidos, consumidos]
    
    benchmark_data_t data_productor = {
        .buffer = buffer,
        .num_items = NUM_ITEMS,
        .items_procesados = 0,
        .tiempo_inicio = 0,
        .tiempo_fin = 0,
        .mutex_stats = &mutex_stats,
        .estadisticas = estadisticas
    };
    
    benchmark_data_t data_consumidor = {
        .buffer = buffer,
        .num_items = NUM_ITEMS,
        .items_procesados = 0,
        .tiempo_inicio = 0,
        .tiempo_fin = 0,
        .mutex_stats = &mutex_stats,
        .estadisticas = estadisticas
    };
    
    printf("\n=== Benchmark de Throughput Básico ===\n");
    printf("Items a procesar: %d\n", NUM_ITEMS);
    printf("Tamaño del buffer: %d\n", buffer->tamaño);
    
    double inicio_total = obtener_tiempo_actual();
    
    // Crear hilos
    pthread_create(&productor_thread, NULL, productor_benchmark, &data_productor);
    pthread_create(&consumidor_thread, NULL, consumidor_benchmark, &data_consumidor);
    
    // Esperar finalización
    pthread_join(productor_thread, NULL);
    pthread_join(consumidor_thread, NULL);
    
    double fin_total = obtener_tiempo_actual();
    double tiempo_total = fin_total - inicio_total;
    
    // Calcular métricas
    double throughput = NUM_ITEMS / tiempo_total;
    double tiempo_productor = data_productor.tiempo_fin - data_productor.tiempo_inicio;
    double tiempo_consumidor = data_consumidor.tiempo_fin - data_consumidor.tiempo_inicio;
    
    printf("Tiempo total: %.3f segundos\n", tiempo_total);
    printf("Tiempo productor: %.3f segundos\n", tiempo_productor);
    printf("Tiempo consumidor: %.3f segundos\n", tiempo_consumidor);
    printf("Throughput: %.2f items/segundo\n", throughput);
    printf("Items producidos: %d\n", estadisticas[0]);
    printf("Items consumidos: %d\n", estadisticas[1]);
    
    // Verificaciones
    cr_assert_eq(estadisticas[0], NUM_ITEMS, "Todos los items deben haberse producido");
    cr_assert_eq(estadisticas[1], NUM_ITEMS, "Todos los items deben haberse consumido");
    cr_assert_gt(throughput, 1000.0, "Throughput debe ser al menos 1000 items/segundo");
    
    pthread_mutex_destroy(&mutex_stats);
    destruir_buffer(buffer);
    printf("=== Fin del Benchmark ===\n\n");
}

/**
 * @brief Estructura para benchmark de latencia
 */
typedef struct {
    buffer_circular_t *buffer;
    double *latencias;
    int num_mediciones;
    int indice_actual;
    pthread_mutex_t *mutex_latencia;
} latencia_data_t;

/**
 * @brief Productor para benchmark de latencia
 */
static void *productor_latencia(void *arg) {
    latencia_data_t *data = (latencia_data_t *)arg;
    
    for (int i = 0; i < data->num_mediciones; i++) {
        double inicio = obtener_tiempo_actual();
        
        // Usar valor con timestamp para medir latencia
        int valor_con_tiempo = (int)(inicio * 1e6) % 1000000; // Microsegundos
        producir_item(data->buffer, valor_con_tiempo);
        
        double fin = obtener_tiempo_actual();
        
        pthread_mutex_lock(data->mutex_latencia);
        data->latencias[i] = (fin - inicio) * 1e6; // Convertir a microsegundos
        pthread_mutex_unlock(data->mutex_latencia);
        
        usleep(100); // Pausa pequeña entre producciones
    }
    
    return NULL;
}

/**
 * @brief Benchmark de latencia
 */
Test(benchmark_productor_consumidor, benchmark_latencia, .timeout = 10.0) {
    const int NUM_MEDICIONES = 1000;
    buffer_circular_t *buffer = crear_buffer(10);
    
    pthread_t productor_thread;
    pthread_mutex_t mutex_latencia = PTHREAD_MUTEX_INITIALIZER;
    double latencias[NUM_MEDICIONES];
    
    latencia_data_t data = {
        .buffer = buffer,
        .latencias = latencias,
        .num_mediciones = NUM_MEDICIONES,
        .indice_actual = 0,
        .mutex_latencia = &mutex_latencia
    };
    
    printf("\n=== Benchmark de Latencia ===\n");
    printf("Mediciones: %d\n", NUM_MEDICIONES);
    
    // Ejecutar benchmark
    pthread_create(&productor_thread, NULL, productor_latencia, &data);
    
    // Consumir items para mantener el buffer operativo
    int item;
    for (int i = 0; i < NUM_MEDICIONES; i++) {
        consumir_item(buffer, &item);
        usleep(150); // Consumidor ligeramente más lento
    }
    
    pthread_join(productor_thread, NULL);
    
    // Calcular estadísticas de latencia
    double suma = 0, minima = latencias[0], maxima = latencias[0];
    for (int i = 0; i < NUM_MEDICIONES; i++) {
        suma += latencias[i];
        if (latencias[i] < minima) minima = latencias[i];
        if (latencias[i] > maxima) maxima = latencias[i];
    }
    
    double promedio = suma / NUM_MEDICIONES;
    
    // Calcular percentiles (aproximado)
    // Ordenar latencias para percentiles
    for (int i = 0; i < NUM_MEDICIONES - 1; i++) {
        for (int j = i + 1; j < NUM_MEDICIONES; j++) {
            if (latencias[i] > latencias[j]) {
                double temp = latencias[i];
                latencias[i] = latencias[j];
                latencias[j] = temp;
            }
        }
    }
    
    double p50 = latencias[NUM_MEDICIONES / 2];
    double p95 = latencias[(int)(NUM_MEDICIONES * 0.95)];
    double p99 = latencias[(int)(NUM_MEDICIONES * 0.99)];
    
    printf("Latencia promedio: %.2f μs\n", promedio);
    printf("Latencia mínima: %.2f μs\n", minima);
    printf("Latencia máxima: %.2f μs\n", maxima);
    printf("Percentil 50: %.2f μs\n", p50);
    printf("Percentil 95: %.2f μs\n", p95);
    printf("Percentil 99: %.2f μs\n", p99);
    
    // Verificaciones de latencia razonable
    cr_assert_lt(promedio, 1000.0, "Latencia promedio debe ser menor a 1ms");
    cr_assert_lt(p95, 5000.0, "P95 debe ser menor a 5ms");
    
    pthread_mutex_destroy(&mutex_latencia);
    destruir_buffer(buffer);
    printf("=== Fin del Benchmark de Latencia ===\n\n");
}

/**
 * @brief Benchmark de escalabilidad con múltiples hilos
 */
Test(benchmark_productor_consumidor, benchmark_escalabilidad, .timeout = 20.0) {
    const int ITEMS_POR_HILO = 5000;
    const int MAX_HILOS = 8;
    
    printf("\n=== Benchmark de Escalabilidad ===\n");
    printf("Items por hilo: %d\n", ITEMS_POR_HILO);
    
    for (int num_hilos = 1; num_hilos <= MAX_HILOS; num_hilos *= 2) {
        buffer_circular_t *buffer = crear_buffer(100);
        
        pthread_t productores[num_hilos];
        pthread_t consumidores[num_hilos];
        pthread_mutex_t mutex_stats = PTHREAD_MUTEX_INITIALIZER;
        int estadisticas[2] = {0, 0};
        
        benchmark_data_t datos[num_hilos * 2]; // Para productores y consumidores
        
        printf("\n--- Configuración: %d hilos ---\n", num_hilos);
        
        double inicio = obtener_tiempo_actual();
        
        // Crear productores
        for (int i = 0; i < num_hilos; i++) {
            datos[i] = (benchmark_data_t){
                .buffer = buffer,
                .num_items = ITEMS_POR_HILO,
                .items_procesados = 0,
                .tiempo_inicio = 0,
                .tiempo_fin = 0,
                .mutex_stats = &mutex_stats,
                .estadisticas = estadisticas
            };
            pthread_create(&productores[i], NULL, productor_benchmark, &datos[i]);
        }
        
        // Crear consumidores
        for (int i = 0; i < num_hilos; i++) {
            datos[num_hilos + i] = (benchmark_data_t){
                .buffer = buffer,
                .num_items = ITEMS_POR_HILO,
                .items_procesados = 0,
                .tiempo_inicio = 0,
                .tiempo_fin = 0,
                .mutex_stats = &mutex_stats,
                .estadisticas = estadisticas
            };
            pthread_create(&consumidores[i], NULL, consumidor_benchmark, &datos[num_hilos + i]);
        }
        
        // Esperar finalización
        for (int i = 0; i < num_hilos; i++) {
            pthread_join(productores[i], NULL);
            pthread_join(consumidores[i], NULL);
        }
        
        double fin = obtener_tiempo_actual();
        double tiempo_total = fin - inicio;
        int total_items = num_hilos * ITEMS_POR_HILO;
        double throughput = total_items / tiempo_total;
        double throughput_por_hilo = throughput / num_hilos;
        
        printf("Tiempo total: %.3f segundos\n", tiempo_total);
        printf("Total items: %d\n", total_items);
        printf("Throughput total: %.2f items/segundo\n", throughput);
        printf("Throughput por hilo: %.2f items/segundo\n", throughput_por_hilo);
        printf("Items producidos: %d\n", estadisticas[0]);
        printf("Items consumidos: %d\n", estadisticas[1]);
        
        // Verificaciones
        cr_assert_eq(estadisticas[0], total_items, "Producción debe ser completa");
        cr_assert_eq(estadisticas[1], total_items, "Consumo debe ser completo");
        
        pthread_mutex_destroy(&mutex_stats);
        destruir_buffer(buffer);
    }
    
    printf("=== Fin del Benchmark de Escalabilidad ===\n\n");
}

/**
 * @brief Benchmark de diferentes tamaños de buffer
 */
Test(benchmark_productor_consumidor, benchmark_tamaños_buffer, .timeout = 15.0) {
    const int NUM_ITEMS = 10000;
    int tamaños[] = {1, 5, 10, 50, 100, 500, 1000};
    int num_tamaños = sizeof(tamaños) / sizeof(tamaños[0]);
    
    printf("\n=== Benchmark de Tamaños de Buffer ===\n");
    printf("Items fijos: %d\n", NUM_ITEMS);
    
    for (int i = 0; i < num_tamaños; i++) {
        int tamaño = tamaños[i];
        buffer_circular_t *buffer = crear_buffer(tamaño);
        
        pthread_t productor_thread, consumidor_thread;
        pthread_mutex_t mutex_stats = PTHREAD_MUTEX_INITIALIZER;
        int estadisticas[2] = {0, 0};
        
        benchmark_data_t data_productor = {
            .buffer = buffer,
            .num_items = NUM_ITEMS,
            .items_procesados = 0,
            .tiempo_inicio = 0,
            .tiempo_fin = 0,
            .mutex_stats = &mutex_stats,
            .estadisticas = estadisticas
        };
        
        benchmark_data_t data_consumidor = {
            .buffer = buffer,
            .num_items = NUM_ITEMS,
            .items_procesados = 0,
            .tiempo_inicio = 0,
            .tiempo_fin = 0,
            .mutex_stats = &mutex_stats,
            .estadisticas = estadisticas
        };
        
        printf("\n--- Buffer tamaño: %d ---\n", tamaño);
        
        double inicio = obtener_tiempo_actual();
        
        pthread_create(&productor_thread, NULL, productor_benchmark, &data_productor);
        pthread_create(&consumidor_thread, NULL, consumidor_benchmark, &data_consumidor);
        
        pthread_join(productor_thread, NULL);
        pthread_join(consumidor_thread, NULL);
        
        double fin = obtener_tiempo_actual();
        double tiempo_total = fin - inicio;
        double throughput = NUM_ITEMS / tiempo_total;
        
        printf("Tiempo: %.3f segundos\n", tiempo_total);
        printf("Throughput: %.2f items/segundo\n", throughput);
        printf("Eficiencia: %.2f items/segundo/tamaño\n", throughput / tamaño);
        
        // Verificación básica
        cr_assert_eq(estadisticas[0], NUM_ITEMS, "Producción completa");
        cr_assert_eq(estadisticas[1], NUM_ITEMS, "Consumo completo");
        
        pthread_mutex_destroy(&mutex_stats);
        destruir_buffer(buffer);
    }
    
    printf("=== Fin del Benchmark de Tamaños ===\n\n");
}

/**
 * @brief Benchmark de stress con alta concurrencia
 */
Test(benchmark_productor_consumidor, benchmark_stress_concurrencia, .timeout = 25.0) {
    const int NUM_PRODUCTORES = 16;
    const int NUM_CONSUMIDORES = 16;
    const int ITEMS_POR_PRODUCTOR = 1000;
    const int TOTAL_ITEMS = NUM_PRODUCTORES * ITEMS_POR_PRODUCTOR;
    
    buffer_circular_t *buffer = crear_buffer(50);
    
    pthread_t productores[NUM_PRODUCTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];
    pthread_mutex_t mutex_stats = PTHREAD_MUTEX_INITIALIZER;
    int estadisticas[2] = {0, 0};
    
    benchmark_data_t datos_productores[NUM_PRODUCTORES];
    benchmark_data_t datos_consumidores[NUM_CONSUMIDORES];
    
    printf("\n=== Benchmark de Stress - Alta Concurrencia ===\n");
    printf("Productores: %d\n", NUM_PRODUCTORES);
    printf("Consumidores: %d\n", NUM_CONSUMIDORES);
    printf("Items por productor: %d\n", ITEMS_POR_PRODUCTOR);
    printf("Total items: %d\n", TOTAL_ITEMS);
    
    double inicio = obtener_tiempo_actual();
    
    // Configurar y crear productores
    for (int i = 0; i < NUM_PRODUCTORES; i++) {
        datos_productores[i] = (benchmark_data_t){
            .buffer = buffer,
            .num_items = ITEMS_POR_PRODUCTOR,
            .items_procesados = 0,
            .tiempo_inicio = 0,
            .tiempo_fin = 0,
            .mutex_stats = &mutex_stats,
            .estadisticas = estadisticas
        };
        pthread_create(&productores[i], NULL, productor_benchmark, &datos_productores[i]);
    }
    
    // Configurar y crear consumidores
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        datos_consumidores[i] = (benchmark_data_t){
            .buffer = buffer,
            .num_items = TOTAL_ITEMS / NUM_CONSUMIDORES,
            .items_procesados = 0,
            .tiempo_inicio = 0,
            .tiempo_fin = 0,
            .mutex_stats = &mutex_stats,
            .estadisticas = estadisticas
        };
        pthread_create(&consumidores[i], NULL, consumidor_benchmark, &datos_consumidores[i]);
    }
    
    // Esperar que terminen todos los hilos
    for (int i = 0; i < NUM_PRODUCTORES; i++) {
        pthread_join(productores[i], NULL);
    }
    
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }
    
    double fin = obtener_tiempo_actual();
    double tiempo_total = fin - inicio;
    double throughput = TOTAL_ITEMS / tiempo_total;
    
    printf("Tiempo total: %.3f segundos\n", tiempo_total);
    printf("Throughput: %.2f items/segundo\n", throughput);
    printf("Items producidos: %d\n", estadisticas[0]);
    printf("Items consumidos: %d\n", estadisticas[1]);
    printf("Hilos totales: %d\n", NUM_PRODUCTORES + NUM_CONSUMIDORES);
    printf("Contención del buffer: %.2f%%\n", 
           (double)buffer->tamaño / TOTAL_ITEMS * 100);
    
    // Verificaciones de integridad bajo stress
    cr_assert_eq(estadisticas[0], TOTAL_ITEMS, 
                "Todos los items deben haberse producido bajo stress");
    cr_assert_eq(estadisticas[1], TOTAL_ITEMS, 
                "Todos los items deben haberse consumido bajo stress");
    cr_assert(esta_vacio(buffer), "Buffer debe estar vacío al final");
    
    // Verificar que el rendimiento se mantiene bajo alta concurrencia
    cr_assert_gt(throughput, 500.0, 
                "Throughput debe mantenerse sobre 500 items/segundo");
    
    pthread_mutex_destroy(&mutex_stats);
    destruir_buffer(buffer);
    printf("=== Fin del Benchmark de Stress ===\n\n");
}
