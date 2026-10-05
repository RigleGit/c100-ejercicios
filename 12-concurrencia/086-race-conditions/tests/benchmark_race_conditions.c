/**
 * @file benchmark_race_conditions.c
 * @brief Benchmarks de rendimiento para race conditions
 */

#include "../include/race_conditions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void benchmark_configuracion(int num_hilos, int incrementos, const char* nombre) {
    printf("\n=== BENCHMARK: %s ===\n", nombre);
    printf("Configuración: %d hilos, %d incrementos\n", num_hilos, incrementos);
    
    configuracion_experimento_t config = {
        .num_hilos = num_hilos,
        .incrementos_por_hilo = incrementos,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    // Benchmark de race condition
    uint64_t inicio_race = obtener_tiempo_microsegundos();
    resultado_experimento_t resultado_race;
    bool race_ok = ejecutar_experimento_race_condition(&config, &resultado_race);
    uint64_t tiempo_race = obtener_tiempo_microsegundos() - inicio_race;
    
    // Benchmark sincronizado
    uint64_t inicio_sync = obtener_tiempo_microsegundos();
    resultado_experimento_t resultado_sync;
    bool sync_ok = ejecutar_experimento_sincronizado(&config, &resultado_sync);
    uint64_t tiempo_sync = obtener_tiempo_microsegundos() - inicio_sync;
    
    if (race_ok && sync_ok) {
        printf("Race condition:\n");
        printf("  Tiempo: %lu μs (%.3f ms)\n", tiempo_race, tiempo_race / 1000.0);
        printf("  Resultado: %d/%d (%.1f%% correcto)\n", 
               resultado_race.valor_obtenido, 
               resultado_race.valor_esperado,
               (double)resultado_race.valor_obtenido * 100.0 / resultado_race.valor_esperado);
        
        printf("Sincronizado:\n");
        printf("  Tiempo: %lu μs (%.3f ms)\n", tiempo_sync, tiempo_sync / 1000.0);
        printf("  Resultado: %d/%d (%.1f%% correcto)\n", 
               resultado_sync.valor_obtenido, 
               resultado_sync.valor_esperado,
               (double)resultado_sync.valor_obtenido * 100.0 / resultado_sync.valor_esperado);
        
        double overhead = (double)tiempo_sync / tiempo_race;
        printf("Overhead: %.2fx más lento\n", overhead);
        
        // Calcular throughput
        long long ops_total = (long long)num_hilos * incrementos;
        double throughput_race = (double)ops_total / (tiempo_race / 1000000.0);
        double throughput_sync = (double)ops_total / (tiempo_sync / 1000000.0);
        
        printf("Throughput race: %.0f ops/sec\n", throughput_race);
        printf("Throughput sync: %.0f ops/sec\n", throughput_sync);
        
        limpiar_resultado_experimento(&resultado_race);
        limpiar_resultado_experimento(&resultado_sync);
    } else {
        printf("❌ Error en benchmark\n");
    }
}

int main(void) {
    printf("=== BENCHMARKS DE RACE CONDITIONS ===\n");
    printf("Midiendo rendimiento de diferentes configuraciones...\n");
    
    // Diferentes cargas de trabajo
    benchmark_configuracion(2, 10000, "Carga Ligera");
    benchmark_configuracion(2, 100000, "Carga Moderada");
    benchmark_configuracion(2, 500000, "Carga Pesada");
    
    // Diferentes números de hilos
    benchmark_configuracion(2, 50000, "2 Hilos");
    benchmark_configuracion(4, 25000, "4 Hilos");
    benchmark_configuracion(8, 12500, "8 Hilos");
    
    printf("\n=== ANÁLISIS DE ESCALABILIDAD ===\n");
    
    int incrementos_base = 100000;
    for (int hilos = 1; hilos <= 8; hilos *= 2) {
        int incrementos_por_hilo = incrementos_base / hilos;
        
        configuracion_experimento_t config = {
            .num_hilos = hilos,
            .incrementos_por_hilo = incrementos_por_hilo,
            .usar_delay = false,
            .delay_microsegundos = 0,
            .mostrar_detalles = false
        };
        
        resultado_experimento_t resultado;
        if (ejecutar_experimento_sincronizado(&config, &resultado)) {
            double tiempo_sec = resultado.tiempo_total_us / 1000000.0;
            double throughput = incrementos_base / tiempo_sec;
            
            printf("%d hilos: %.3f sec, %.0f ops/sec\n", 
                   hilos, tiempo_sec, throughput);
            
            limpiar_resultado_experimento(&resultado);
        }
    }
    
    printf("\n=== RESUMEN ===\n");
    printf("• Race conditions son más rápidas pero incorrectas\n");
    printf("• Sincronización añade overhead pero garantiza correctitud\n");
    printf("• El overhead varía según la contención\n");
    printf("• Más hilos → mayor overhead relativo\n");
    printf("• La escalabilidad depende del diseño del algoritmo\n");
    
    return EXIT_SUCCESS;
}
