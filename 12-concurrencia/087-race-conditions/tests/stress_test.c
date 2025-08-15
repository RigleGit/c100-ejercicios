/**
 * @file stress_test.c
 * @brief Tests de estrés para race conditions
 */

#include "../include/race_conditions.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static volatile bool continuar_test = true;

void manejador_señal(int señal) {
    printf("\nRecibida señal %d, deteniendo test...\n", señal);
    continuar_test = false;
}

void test_estres_basico(void) {
    printf("\n=== TEST DE ESTRÉS BÁSICO ===\n");
    printf("Ejecutando experimentos hasta detectar race condition...\n");
    
    configuracion_experimento_t config = {
        .num_hilos = 4,
        .incrementos_por_hilo = 10000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    int experimentos = 0;
    int race_conditions_detectadas = 0;
    
    while (continuar_test && experimentos < 1000) {
        experimentos++;
        
        resultado_experimento_t resultado;
        if (ejecutar_experimento_race_condition(&config, &resultado)) {
            if (resultado.diferencia > 0) {
                race_conditions_detectadas++;
                if (race_conditions_detectadas == 1) {
                    printf("Primera race condition detectada en experimento %d\n", experimentos);
                    printf("  Valor esperado: %d\n", resultado.valor_esperado);
                    printf("  Valor obtenido: %d\n", resultado.valor_obtenido);
                    printf("  Incrementos perdidos: %d\n", resultado.diferencia);
                }
            }
            limpiar_resultado_experimento(&resultado);
        }
        
        if (experimentos % 100 == 0) {
            printf("Experimentos: %d, Race conditions: %d (%.1f%%)\n", 
                   experimentos, race_conditions_detectadas,
                   (double)race_conditions_detectadas * 100.0 / experimentos);
        }
    }
    
    printf("\nResultados finales:\n");
    printf("Experimentos realizados: %d\n", experimentos);
    printf("Race conditions detectadas: %d\n", race_conditions_detectadas);
    printf("Tasa de race conditions: %.2f%%\n", 
           (double)race_conditions_detectadas * 100.0 / experimentos);
}

void test_estres_carga_variable(void) {
    printf("\n=== TEST DE ESTRÉS CON CARGA VARIABLE ===\n");
    
    struct {
        int hilos;
        int incrementos;
        const char* nombre;
    } configuraciones[] = {
        {2, 50000, "Configuración Moderada"},
        {4, 25000, "Múltiples Hilos"},
        {8, 12500, "Alta Concurrencia"},
        {2, 200000, "Carga Pesada"}
    };
    
    int num_configs = sizeof(configuraciones) / sizeof(configuraciones[0]);
    
    for (int i = 0; i < num_configs && continuar_test; i++) {
        printf("\n--- %s ---\n", configuraciones[i].nombre);
        printf("Hilos: %d, Incrementos: %d\n", 
               configuraciones[i].hilos, configuraciones[i].incrementos);
        
        configuracion_experimento_t config = {
            .num_hilos = configuraciones[i].hilos,
            .incrementos_por_hilo = configuraciones[i].incrementos,
            .usar_delay = false,
            .delay_microsegundos = 0,
            .mostrar_detalles = false
        };
        
        // Ejecutar análisis estadístico rápido
        analisis_estadistico_t analisis;
        if (ejecutar_analisis_estadistico(&config, 20, &analisis)) {
            printf("Tasa de inconsistencia: %.1f%%\n", analisis.tasa_inconsistencia);
            printf("Error promedio: %.2f%%\n", analisis.error_promedio);
            printf("Incrementos perdidos (promedio): %.0f\n", analisis.diferencia_promedio);
            
            if (analisis.tasa_inconsistencia > 80.0) {
                printf("Estado: 🚨 CRÍTICO - Race conditions muy frecuentes\n");
            } else if (analisis.tasa_inconsistencia > 50.0) {
                printf("Estado: ⚠️  ALTO - Race conditions frecuentes\n");
            } else if (analisis.tasa_inconsistencia > 20.0) {
                printf("Estado: ⚠️  MODERADO - Race conditions ocasionales\n");
            } else if (analisis.tasa_inconsistencia > 0.0) {
                printf("Estado: ✅ BAJO - Race conditions raras\n");
            } else {
                printf("Estado: ✅ SEGURO - No se detectaron race conditions\n");
            }
            
            limpiar_analisis_estadistico(&analisis);
        }
    }
}

void test_estres_prolongado(void) {
    printf("\n=== TEST DE ESTRÉS PROLONGADO ===\n");
    printf("Ejecutando experimentos continuos (Ctrl+C para detener)...\n");
    
    configuracion_experimento_t config = {
        .num_hilos = 2,
        .incrementos_por_hilo = 25000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    int experimentos_totales = 0;
    int race_conditions_totales = 0;
    int diferencia_acumulada = 0;
    uint64_t tiempo_acumulado = 0;
    
    time_t inicio = time(NULL);
    
    while (continuar_test) {
        resultado_experimento_t resultado;
        if (ejecutar_experimento_race_condition(&config, &resultado)) {
            experimentos_totales++;
            tiempo_acumulado += resultado.tiempo_total_us;
            
            if (resultado.diferencia > 0) {
                race_conditions_totales++;
                diferencia_acumulada += resultado.diferencia;
            }
            
            // Reportar cada 50 experimentos
            if (experimentos_totales % 50 == 0) {
                time_t ahora = time(NULL);
                int duracion = (int)(ahora - inicio);
                
                printf("Tiempo: %02d:%02d | ", duracion / 60, duracion % 60);
                printf("Experimentos: %d | ", experimentos_totales);
                printf("Race conditions: %d (%.1f%%) | ", 
                       race_conditions_totales,
                       (double)race_conditions_totales * 100.0 / experimentos_totales);
                printf("Incrementos perdidos: %d\n", diferencia_acumulada);
            }
            
            limpiar_resultado_experimento(&resultado);
        }
        
        // Pequeña pausa para no saturar el sistema
        usleep(1000);  // 1ms
    }
    
    printf("\n=== RESULTADOS FINALES DEL TEST PROLONGADO ===\n");
    printf("Duración total: %ld segundos\n", time(NULL) - inicio);
    printf("Experimentos realizados: %d\n", experimentos_totales);
    printf("Race conditions detectadas: %d\n", race_conditions_totales);
    printf("Tasa de race conditions: %.2f%%\n", 
           (double)race_conditions_totales * 100.0 / experimentos_totales);
    printf("Incrementos perdidos total: %d\n", diferencia_acumulada);
    
    if (experimentos_totales > 0) {
        double tiempo_promedio = (double)tiempo_acumulado / experimentos_totales;
        printf("Tiempo promedio por experimento: %.1f μs\n", tiempo_promedio);
        
        if (race_conditions_totales > 0) {
            double incrementos_perdidos_promedio = (double)diferencia_acumulada / race_conditions_totales;
            printf("Incrementos perdidos promedio: %.1f\n", incrementos_perdidos_promedio);
        }
    }
}

void test_deteccion_temprana(void) {
    printf("\n=== TEST DE DETECCIÓN TEMPRANA ===\n");
    printf("Buscando la configuración mínima que produce race conditions...\n");
    
    bool race_encontrada = false;
    
    for (int hilos = 2; hilos <= 8 && !race_encontrada && continuar_test; hilos++) {
        for (int incrementos = 100; incrementos <= 10000 && !race_encontrada; incrementos *= 2) {
            printf("Probando: %d hilos, %d incrementos... ", hilos, incrementos);
            
            configuracion_experimento_t config = {
                .num_hilos = hilos,
                .incrementos_por_hilo = incrementos,
                .usar_delay = false,
                .delay_microsegundos = 0,
                .mostrar_detalles = false
            };
            
            // Probar con varios experimentos
            int races_detectadas = 0;
            for (int exp = 0; exp < 10; exp++) {
                resultado_experimento_t resultado;
                if (ejecutar_experimento_race_condition(&config, &resultado)) {
                    if (resultado.diferencia > 0) {
                        races_detectadas++;
                    }
                    limpiar_resultado_experimento(&resultado);
                }
            }
            
            if (races_detectadas > 0) {
                printf("✗ %d/10 race conditions detectadas\n", races_detectadas);
                race_encontrada = true;
                
                printf("\nConfiguración mínima encontrada:\n");
                printf("  Hilos: %d\n", hilos);
                printf("  Incrementos por hilo: %d\n", incrementos);
                printf("  Tasa de detección: %d/10 (%.0f%%)\n", 
                       races_detectadas, races_detectadas * 10.0);
            } else {
                printf("✓ Sin race conditions\n");
            }
        }
    }
    
    if (!race_encontrada) {
        printf("No se encontraron race conditions en las configuraciones probadas\n");
        printf("Esto puede indicar un sistema muy rápido o con alta coherencia de caché\n");
    }
}

int main(void) {
    printf("=== TESTS DE ESTRÉS PARA RACE CONDITIONS ===\n");
    printf("Presione Ctrl+C para detener cualquier test\n");
    
    // Configurar manejador de señales
    signal(SIGINT, manejador_señal);
    signal(SIGTERM, manejador_señal);
    
    test_deteccion_temprana();
    
    if (continuar_test) {
        test_estres_basico();
    }
    
    if (continuar_test) {
        test_estres_carga_variable();
    }
    
    if (continuar_test) {
        printf("\nIniciando test prolongado en 3 segundos...\n");
        printf("(Ctrl+C para saltar al resumen)\n");
        sleep(3);
        
        if (continuar_test) {
            test_estres_prolongado();
        }
    }
    
    printf("\n=== TESTS DE ESTRÉS COMPLETADOS ===\n");
    printf("Los tests de estrés ayudan a:\n");
    printf("• Encontrar las condiciones mínimas para race conditions\n");
    printf("• Medir la frecuencia real de race conditions\n");
    printf("• Evaluar la estabilidad bajo carga sostenida\n");
    printf("• Validar la efectividad de las medidas de sincronización\n");
    
    return EXIT_SUCCESS;
}
