/**
 * @file main.c
 * @brief Programa principal para demostrar race conditions
 * 
 * Este programa demuestra las condiciones de carrera que ocurren cuando
 * múltiples hilos acceden concurrentemente a recursos compartidos sin
 * sincronización adecuada.
 */

#include "../include/race_conditions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Función para mostrar el menú
void mostrar_menu(void) {
    printf("\n=== DEMOSTRACIÓN DE RACE CONDITIONS ===\n");
    printf("1. Ejecutar ejemplo básico del enunciado\n");
    printf("2. Ejecutar demostración completa\n");
    printf("3. Demostrar factores que afectan race conditions\n");
    printf("4. Simular diferentes escenarios\n");
    printf("5. Experimento personalizado\n");
    printf("6. Comparación con sincronización\n");
    printf("7. Análisis estadístico detallado\n");
    printf("8. Información educativa\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

// Función para obtener entrada del usuario
int obtener_entero(const char* prompt, int min_val, int max_val) {
    int valor;
    char linea[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(linea, sizeof(linea), stdin) != NULL) {
            if (sscanf(linea, "%d", &valor) == 1) {
                if (valor >= min_val && valor <= max_val) {
                    return valor;
                }
            }
        }
        printf("Por favor, ingrese un valor entre %d y %d.\n", min_val, max_val);
    }
}

// Función para experimento personalizado
void ejecutar_experimento_personalizado(void) {
    printf("\n=== EXPERIMENTO PERSONALIZADO ===\n");
    
    configuracion_experimento_t config;
    
    config.num_hilos = obtener_entero("Número de hilos (1-16): ", 1, 16);
    config.incrementos_por_hilo = obtener_entero("Incrementos por hilo (100-1000000): ", 100, 1000000);
    
    printf("¿Usar delay entre incrementos? (0=No, 1=Sí): ");
    int usar_delay = obtener_entero("", 0, 1);
    config.usar_delay = (usar_delay == 1);
    
    if (config.usar_delay) {
        config.delay_microsegundos = obtener_entero("Delay en microsegundos (1-100): ", 1, 100);
    } else {
        config.delay_microsegundos = 0;
    }
    
    printf("¿Mostrar detalles de hilos? (0=No, 1=Sí): ");
    int mostrar_detalles = obtener_entero("", 0, 1);
    config.mostrar_detalles = (mostrar_detalles == 1);
    
    printf("\nEjecutando experimento personalizado...\n");
    resultado_experimento_t resultado;
    if (ejecutar_experimento_race_condition(&config, &resultado)) {
        imprimir_resultado_experimento(&resultado);
        limpiar_resultado_experimento(&resultado);
    } else {
        printf("Error ejecutando el experimento.\n");
    }
}

// Función para comparación detallada
void ejecutar_comparacion_detallada(void) {
    printf("\n=== COMPARACIÓN DETALLADA ===\n");
    
    configuracion_experimento_t config = {
        .num_hilos = 2,
        .incrementos_por_hilo = 100000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = true
    };
    
    // Permitir personalizar la configuración
    printf("Configuración actual: %d hilos, %d incrementos/hilo\n", 
           config.num_hilos, config.incrementos_por_hilo);
    
    printf("¿Usar configuración personalizada? (0=No, 1=Sí): ");
    int personalizar = obtener_entero("", 0, 1);
    
    if (personalizar) {
        config.num_hilos = obtener_entero("Número de hilos (2-8): ", 2, 8);
        config.incrementos_por_hilo = obtener_entero("Incrementos por hilo (1000-500000): ", 1000, 500000);
    }
    
    comparacion_sincronizacion_t comparacion;
    if (ejecutar_comparacion_sincronizacion(&config, &comparacion)) {
        imprimir_comparacion_sincronizacion(&comparacion);
        
        // Análisis adicional
        printf("\n=== ANÁLISIS ADICIONAL ===\n");
        if (comparacion.experimento_race.diferencia > 0) {
            double porcentaje_perdido = (double)comparacion.experimento_race.diferencia * 100.0 / 
                                       comparacion.experimento_race.valor_esperado;
            printf("Porcentaje de incrementos perdidos: %.3f%%\n", porcentaje_perdido);
            
            if (porcentaje_perdido > 1.0) {
                printf("⚠ ALTA tasa de pérdida - race condition muy evidente\n");
            } else if (porcentaje_perdido > 0.1) {
                printf("⚠ Pérdida moderada - race condition detectable\n");
            } else {
                printf("• Pérdida baja - race condition sutil\n");
            }
        }
        
        if (comparacion.factor_slowdown < 1.2) {
            printf("✓ Overhead de sincronización muy bajo\n");
        } else if (comparacion.factor_slowdown < 2.0) {
            printf("• Overhead de sincronización moderado\n");
        } else {
            printf("⚠ Overhead de sincronización alto\n");
        }
        
        limpiar_resultado_experimento(&comparacion.experimento_race);
        limpiar_resultado_experimento(&comparacion.experimento_seguro);
    } else {
        printf("Error ejecutando la comparación.\n");
    }
}

// Función para análisis estadístico personalizado
void ejecutar_analisis_personalizado(void) {
    printf("\n=== ANÁLISIS ESTADÍSTICO DETALLADO ===\n");
    
    configuracion_experimento_t config = {
        .num_hilos = 2,
        .incrementos_por_hilo = 50000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    config.num_hilos = obtener_entero("Número de hilos (2-8): ", 2, 8);
    config.incrementos_por_hilo = obtener_entero("Incrementos por hilo (1000-200000): ", 1000, 200000);
    int num_experimentos = obtener_entero("Número de experimentos (5-100): ", 5, 100);
    
    printf("\nEjecutando análisis estadístico con %d experimentos...\n", num_experimentos);
    printf("Esto puede tomar unos momentos...\n");
    
    analisis_estadistico_t analisis;
    if (ejecutar_analisis_estadistico(&config, num_experimentos, &analisis)) {
        imprimir_analisis_estadistico(&analisis);
        
        // Análisis adicional
        printf("\n=== INTERPRETACIÓN ===\n");
        if (analisis.tasa_inconsistencia == 0.0) {
            printf("• No se detectaron race conditions.\n");
            printf("• Esto puede indicar que la contención es muy baja\n");
            printf("  o que se necesitan más experimentos.\n");
        } else if (analisis.tasa_inconsistencia < 10.0) {
            printf("• Race conditions raras pero detectables.\n");
            printf("• El sistema es mayormente estable pero no determinístico.\n");
        } else if (analisis.tasa_inconsistencia < 50.0) {
            printf("• Race conditions frecuentes.\n");
            printf("• El sistema muestra inconsistencia notable.\n");
        } else if (analisis.tasa_inconsistencia < 90.0) {
            printf("• Race conditions muy frecuentes.\n");
            printf("• El sistema es altamente inconsistente.\n");
        } else {
            printf("• Race conditions casi garantizadas.\n");
            printf("• El sistema es extremadamente inconsistente.\n");
        }
        
        if (analisis.desviacion_estandar > analisis.valor_promedio * 0.1) {
            printf("• Alta variabilidad en los resultados.\n");
        } else {
            printf("• Variabilidad moderada en los resultados.\n");
        }
        
        limpiar_analisis_estadistico(&analisis);
    } else {
        printf("Error ejecutando el análisis estadístico.\n");
    }
}

// Función para mostrar información educativa
void mostrar_informacion_educativa(void) {
    printf("\n=== INFORMACIÓN EDUCATIVA: RACE CONDITIONS ===\n\n");
    
    printf("¿QUÉ SON LAS RACE CONDITIONS?\n");
    printf("─────────────────────────────\n");
    printf("Las race conditions (condiciones de carrera) ocurren cuando múltiples\n");
    printf("hilos acceden concurrentemente a un recurso compartido sin sincronización\n");
    printf("adecuada, y el resultado final depende del orden de ejecución.\n\n");
    
    printf("¿POR QUÉ OCURREN?\n");
    printf("─────────────────\n");
    printf("1. Operaciones no atómicas: counter++ no es una sola instrucción\n");
    printf("   - Lee el valor actual\n");
    printf("   - Incrementa el valor\n");
    printf("   - Escribe el nuevo valor\n\n");
    printf("2. Intercalado de hilos: El scheduler puede cambiar entre hilos\n");
    printf("   en cualquier momento, incluso entre estas operaciones.\n\n");
    
    printf("EJEMPLO DE RACE CONDITION:\n");
    printf("─────────────────────────\n");
    printf("Hilo 1:                 Hilo 2:\n");
    printf("lee counter (100)       lee counter (100)\n");
    printf("incrementa (101)        incrementa (101)\n");
    printf("escribe counter (101)   escribe counter (101)\n");
    printf("Resultado: 101 (debería ser 102)\n\n");
    
    printf("CONSECUENCIAS:\n");
    printf("─────────────\n");
    printf("• Resultados no determinísticos\n");
    printf("• Pérdida de datos\n");
    printf("• Estados inconsistentes\n");
    printf("• Bugs difíciles de reproducir\n\n");
    
    printf("SOLUCIONES:\n");
    printf("──────────\n");
    printf("1. Mutex: Bloqueo exclusivo\n");
    printf("2. Semáforos: Control de acceso\n");
    printf("3. Operaciones atómicas: Hardware level\n");
    printf("4. Variables de condición: Sincronización compleja\n");
    printf("5. Lock-free programming: Algoritmos sin bloqueos\n\n");
    
    printf("DETECCIÓN:\n");
    printf("─────────\n");
    printf("• Herramientas como ThreadSanitizer\n");
    printf("• Testing exhaustivo\n");
    printf("• Análisis estático de código\n");
    printf("• Revisión manual del código\n\n");
    
    printf("BUENAS PRÁCTICAS:\n");
    printf("────────────────\n");
    printf("• Minimizar el estado compartido\n");
    printf("• Usar primitivas de sincronización apropiadas\n");
    printf("• Diseñar para inmutabilidad cuando sea posible\n");
    printf("• Documentar claramente los requisitos de sincronización\n");
    printf("• Testing con diferentes cargas y configuraciones\n\n");
    
    printf("Presione Enter para continuar...");
    getchar();
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== EJERCICIO 087: RACE CONDITIONS ===\n");
    printf("Demostración de condiciones de carrera en programación concurrente\n");
    
    int opcion;
    
    do {
        mostrar_menu();
        opcion = obtener_entero("", 0, 8);
        
        switch (opcion) {
            case 1:
                printf("\nEjecutando ejemplo básico del enunciado...\n");
                ejecutar_ejemplo_basico_enunciado();
                break;
                
            case 2:
                printf("\nEjecutando demostración completa...\n");
                ejecutar_demo_completa_race_conditions();
                break;
                
            case 3:
                printf("\nDemostrando factores que afectan race conditions...\n");
                demostrar_factores_race_conditions();
                break;
                
            case 4:
                printf("\nSimulando diferentes escenarios...\n");
                simular_escenarios_race_conditions();
                break;
                
            case 5:
                ejecutar_experimento_personalizado();
                break;
                
            case 6:
                ejecutar_comparacion_detallada();
                break;
                
            case 7:
                ejecutar_analisis_personalizado();
                break;
                
            case 8:
                mostrar_informacion_educativa();
                break;
                
            case 0:
                printf("\n¡Gracias por usar el programa!\n");
                printf("\nRECUERDE:\n");
                printf("• Las race conditions son peligrosas en código de producción\n");
                printf("• Siempre use sincronización apropiada para recursos compartidos\n");
                printf("• Teste su código concurrente exhaustivamente\n");
                break;
                
            default:
                printf("Opción no válida.\n");
                break;
        }
        
        if (opcion != 0) {
            printf("\nPresione Enter para continuar...");
            while (getchar() != '\n');  // Limpiar buffer
        }
        
    } while (opcion != 0);
    
    return EXIT_SUCCESS;
}
#endif
