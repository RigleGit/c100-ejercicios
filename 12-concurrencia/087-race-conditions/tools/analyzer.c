/**
 * @file analyzer.c
 * @brief Analizador automatizado de race conditions
 */

#include "../include/race_conditions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void analizar_configuracion(int num_hilos, int incrementos, const char* descripcion) {
    printf("\n=== %s ===\n", descripcion);
    printf("Configuración: %d hilos, %d incrementos/hilo\n", num_hilos, incrementos);
    
    configuracion_experimento_t config = {
        .num_hilos = num_hilos,
        .incrementos_por_hilo = incrementos,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    // Análisis rápido con pocos experimentos
    analisis_estadistico_t analisis;
    if (ejecutar_analisis_estadistico(&config, 10, &analisis)) {
        printf("Tasa de inconsistencia: %.1f%%\n", analisis.tasa_inconsistencia);
        
        if (analisis.tasa_inconsistencia == 0.0) {
            printf("Estado: ✅ Sin race conditions detectadas\n");
            printf("Recomendación: Configuración aparentemente segura\n");
        } else if (analisis.tasa_inconsistencia < 20.0) {
            printf("Estado: ⚠️  Race conditions ocasionales\n");
            printf("Recomendación: Use sincronización por seguridad\n");
        } else if (analisis.tasa_inconsistencia < 70.0) {
            printf("Estado: ⚠️  Race conditions frecuentes\n");
            printf("Recomendación: Sincronización REQUERIDA\n");
        } else {
            printf("Estado: 🚨 Race conditions críticas\n");
            printf("Recomendación: Sincronización OBLIGATORIA\n");
        }
        
        printf("Error promedio: %.2f%%\n", analisis.error_promedio);
        limpiar_analisis_estadistico(&analisis);
    } else {
        printf("❌ Error en el análisis\n");
    }
}

int main(void) {
    printf("=== ANALIZADOR AUTOMATIZADO DE RACE CONDITIONS ===\n");
    printf("Analizando diferentes configuraciones...\n");
    
    // Análisis de diferentes escenarios
    analizar_configuracion(2, 1000, "Carga Ligera");
    analizar_configuracion(2, 50000, "Carga Moderada");
    analizar_configuracion(2, 200000, "Carga Pesada");
    analizar_configuracion(4, 25000, "Múltiples Hilos");
    analizar_configuracion(8, 12500, "Muchos Hilos");
    
    printf("\n=== ANÁLISIS COMPARATIVO DE RENDIMIENTO ===\n");
    
    configuracion_experimento_t config_test = {
        .num_hilos = 2,
        .incrementos_por_hilo = 100000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    comparacion_sincronizacion_t comparacion;
    if (ejecutar_comparacion_sincronizacion(&config_test, &comparacion)) {
        printf("Experimento con race condition:\n");
        printf("  Tiempo: %lu μs\n", comparacion.experimento_race.tiempo_total_us);
        printf("  Valor obtenido: %d/%d\n", 
               comparacion.experimento_race.valor_obtenido,
               comparacion.experimento_race.valor_esperado);
        
        printf("Experimento sincronizado:\n");
        printf("  Tiempo: %lu μs\n", comparacion.experimento_seguro.tiempo_total_us);
        printf("  Valor obtenido: %d/%d\n", 
               comparacion.experimento_seguro.valor_obtenido,
               comparacion.experimento_seguro.valor_esperado);
        
        printf("Overhead de sincronización: %.1fx más lento\n", comparacion.factor_slowdown);
        
        if (comparacion.factor_slowdown < 1.5) {
            printf("✅ Overhead aceptable\n");
        } else if (comparacion.factor_slowdown < 3.0) {
            printf("⚠️  Overhead moderado\n");
        } else {
            printf("🚨 Overhead significativo\n");
        }
        
        limpiar_resultado_experimento(&comparacion.experimento_race);
        limpiar_resultado_experimento(&comparacion.experimento_seguro);
    }
    
    printf("\n=== RECOMENDACIONES GENERALES ===\n");
    printf("1. SIEMPRE use sincronización para recursos compartidos\n");
    printf("2. Use ThreadSanitizer durante desarrollo: gcc -fsanitize=thread\n");
    printf("3. Teste con diferentes configuraciones de hilos y cargas\n");
    printf("4. Considere alternativas lock-free cuando el rendimiento sea crítico\n");
    printf("5. Diseñe para minimizar el estado compartido mutable\n");
    
    printf("\n=== COMANDOS ÚTILES ===\n");
    printf("Compilar con ThreadSanitizer:\n");
    printf("  gcc -fsanitize=thread -g -o programa programa.c -lpthread\n\n");
    printf("Compilar con Helgrind:\n");
    printf("  gcc -g -o programa programa.c -lpthread\n");
    printf("  valgrind --tool=helgrind ./programa\n\n");
    printf("Análisis de rendimiento:\n");
    printf("  time ./programa\n");
    printf("  perf record ./programa && perf report\n");
    
    return EXIT_SUCCESS;
}
