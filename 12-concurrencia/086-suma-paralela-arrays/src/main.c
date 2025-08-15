#include "../include/suma_paralela_arrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void imprimir_menu(void) {
    printf("\n=== Ejercicio 086: Suma Paralela de Arrays ===\n");
    printf("1. Ejemplo básico (como en el enunciado)\n");
    printf("2. Benchmark secuencial vs paralelo\n");
    printf("3. Pruebas de escalabilidad\n");
    printf("4. Array aleatorio personalizado\n");
    printf("5. Análisis de rendimiento detallado\n");
    printf("6. Demostración completa\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

static void ejemplo_basico(void) {
    printf("\n=== Ejemplo Básico (Enunciado Original) ===\n");
    
    // Array exacto del enunciado
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t tamano = 10;
    
    printf("Array: [");
    for (size_t i = 0; i < tamano; i++) {
        printf("%d%s", array[i], (i < tamano - 1) ? ", " : "");
    }
    printf("]\n");
    printf("Suma esperada: 55\n\n");
    
    // Configuración para 2 hilos como en el enunciado
    configuracion_suma_t config = {
        .array = array,
        .tamano = tamano,
        .num_hilos = 2,
        .mostrar_detalles = true
    };
    
    resultado_suma_paralela_t resultado;
    if (suma_paralela(&config, &resultado)) {
        printf("\nResultado: %ld\n", resultado.suma_total);
        printf("Tiempo de ejecución: %lu microsegundos\n", resultado.tiempo_total_us);
        
        if (resultado.suma_total == 55) {
            printf("✓ CORRECTO: La suma coincide con el valor esperado\n");
        } else {
            printf("✗ ERROR: La suma no coincide\n");
        }
        
        limpiar_resultado_suma(&resultado);
    } else {
        printf("Error ejecutando suma paralela\n");
    }
}

static void demo_benchmark(void) {
    printf("\n=== Benchmark Secuencial vs Paralelo ===\n");
    
    size_t tamano;
    printf("Ingrese el tamaño del array (recomendado: 100000-1000000): ");
    if (scanf("%zu", &tamano) != 1 || tamano == 0) {
        printf("Tamaño inválido\n");
        return;
    }
    
    int num_hilos;
    printf("Ingrese el número de hilos (recomendado: 2-8): ");
    if (scanf("%d", &num_hilos) != 1 || num_hilos <= 0) {
        printf("Número de hilos inválido\n");
        return;
    }
    
    printf("\nCreando array de %zu elementos...\n", tamano);
    int* array = crear_array_secuencia(tamano, 1, 1);
    if (array == NULL) {
        printf("Error creando array\n");
        return;
    }
    
    printf("Ejecutando benchmark...\n");
    benchmark_suma_t benchmark;
    if (ejecutar_benchmark_suma(array, tamano, num_hilos, &benchmark)) {
        imprimir_benchmark(&benchmark);
        
        // Análisis adicional
        printf("\n=== Análisis ===\n");
        if (benchmark.speedup > 1.0) {
            printf("🚀 El paralelismo MEJORÓ el rendimiento\n");
            printf("   Ganancia: %.1f%% más rápido\n", (benchmark.speedup - 1.0) * 100.0);
        } else if (benchmark.speedup < 1.0) {
            printf("⚠️  El paralelismo EMPEORÓ el rendimiento\n");
            printf("   Pérdida: %.1f%% más lento\n", (1.0 - benchmark.speedup) * 100.0);
            printf("   Esto puede deberse al overhead de crear hilos\n");
        } else {
            printf("⚖️  El rendimiento fue similar\n");
        }
        
        if (benchmark.eficiencia > 0.8) {
            printf("✅ Excelente eficiencia de paralelización\n");
        } else if (benchmark.eficiencia > 0.5) {
            printf("✔️  Buena eficiencia de paralelización\n");
        } else {
            printf("⚠️  Baja eficiencia de paralelización\n");
        }
    }
    
    free(array);
}

static void demo_escalabilidad(void) {
    printf("\n=== Pruebas de Escalabilidad ===\n");
    
    size_t tamano = 1000000; // 1 millón de elementos
    printf("Usando array de %zu elementos\n", tamano);
    
    int* array = crear_array_secuencia(tamano, 1, 1);
    if (array == NULL) {
        printf("Error creando array\n");
        return;
    }
    
    int max_hilos = obtener_num_cpus();
    if (max_hilos <= 0) {
        max_hilos = 8; // Valor por defecto
    }
    
    printf("Número de CPUs detectadas: %d\n", max_hilos);
    printf("Probando escalabilidad hasta %d hilos...\n\n", max_hilos);
    
    ejecutar_pruebas_escalabilidad(array, tamano, max_hilos);
    
    printf("\n=== Observaciones ===\n");
    printf("• Speedup ideal: lineal hasta el número de CPUs\n");
    printf("• Eficiencia ideal: 100%% con pocos hilos\n");
    printf("• Degradación esperada: más hilos que CPUs\n");
    printf("• Overhead: visible en arrays pequeños\n");
    
    free(array);
}

static void demo_array_personalizado(void) {
    printf("\n=== Array Aleatorio Personalizado ===\n");
    
    size_t tamano;
    int valor_min, valor_max, num_hilos;
    
    printf("Configuración del array:\n");
    printf("Tamaño: ");
    if (scanf("%zu", &tamano) != 1 || tamano == 0) {
        printf("Tamaño inválido\n");
        return;
    }
    
    printf("Valor mínimo: ");
    if (scanf("%d", &valor_min) != 1) {
        printf("Valor inválido\n");
        return;
    }
    
    printf("Valor máximo: ");
    if (scanf("%d", &valor_max) != 1 || valor_max < valor_min) {
        printf("Valor inválido\n");
        return;
    }
    
    printf("Número de hilos: ");
    if (scanf("%d", &num_hilos) != 1 || num_hilos <= 0) {
        printf("Número de hilos inválido\n");
        return;
    }
    
    printf("\nCreando array aleatorio...\n");
    int* array = crear_array_aleatorio(tamano, valor_min, valor_max);
    if (array == NULL) {
        printf("Error creando array\n");
        return;
    }
    
    // Mostrar estadísticas del array
    int min, max;
    double promedio;
    calcular_estadisticas_array(array, tamano, &min, &max, &promedio);
    
    printf("Estadísticas del array:\n");
    printf("  Tamaño: %zu elementos\n", tamano);
    printf("  Mínimo: %d\n", min);
    printf("  Máximo: %d\n", max);
    printf("  Promedio: %.2f\n", promedio);
    
    // Validar array
    if (!validar_array_para_suma(array, tamano)) {
        printf("⚠️  ADVERTENCIA: El array puede causar overflow\n");
    }
    
    // Mostrar algunos elementos si el array es pequeño
    if (tamano <= 20) {
        printf("  Elementos: [");
        for (size_t i = 0; i < tamano; i++) {
            printf("%d%s", array[i], (i < tamano - 1) ? ", " : "");
        }
        printf("]\n");
    }
    
    printf("\nEjecutando suma paralela...\n");
    benchmark_suma_t benchmark;
    if (ejecutar_benchmark_suma(array, tamano, num_hilos, &benchmark)) {
        imprimir_benchmark(&benchmark);
    }
    
    free(array);
}

static void demo_analisis_detallado(void) {
    printf("\n=== Análisis de Rendimiento Detallado ===\n");
    
    // Usar un array de tamaño medio para ver detalles
    size_t tamano = 50000;
    printf("Usando array de %zu elementos (secuencia 1, 2, 3, ...)\n", tamano);
    
    int* array = crear_array_secuencia(tamano, 1, 1);
    if (array == NULL) {
        printf("Error creando array\n");
        return;
    }
    
    // Probar con diferentes números de hilos
    int hilos_a_probar[] = {1, 2, 4, 8};
    int num_pruebas = sizeof(hilos_a_probar) / sizeof(hilos_a_probar[0]);
    
    printf("\nComparando diferentes configuraciones:\n");
    
    for (int i = 0; i < num_pruebas; i++) {
        int num_hilos = hilos_a_probar[i];
        printf("\n--- Configuración: %d hilo(s) ---\n", num_hilos);
        
        configuracion_suma_t config = {
            .array = array,
            .tamano = tamano,
            .num_hilos = num_hilos,
            .mostrar_detalles = (num_hilos <= 4) // Solo mostrar detalles para pocos hilos
        };
        
        resultado_suma_paralela_t resultado;
        if (suma_paralela(&config, &resultado)) {
            imprimir_resultados_suma_paralela(&resultado);
            limpiar_resultado_suma(&resultado);
        }
    }
    
    // Análisis final
    printf("\n=== Conclusiones ===\n");
    printf("• 1 hilo: Referencia base (equivale a secuencial)\n");
    printf("• 2 hilos: Debería mostrar mejora significativa\n");
    printf("• 4 hilos: Máximo beneficio en CPUs de 4 núcleos\n");
    printf("• 8 hilos: Puede mostrar degradación por overhead\n");
    
    free(array);
}

#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    
    printf("Este programa demuestra la suma paralela de arrays usando hilos.\n");
    printf("Permite dividir el trabajo computacional entre múltiples hilos\n");
    printf("para aprovechar sistemas con múltiples núcleos.\n");
    
    do {
        imprimir_menu();
        
        if (scanf("%d", &opcion) != 1) {
            printf("Error leyendo opción. Intente nuevamente.\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        
        switch (opcion) {
            case 1:
                ejemplo_basico();
                break;
                
            case 2:
                demo_benchmark();
                break;
                
            case 3:
                demo_escalabilidad();
                break;
                
            case 4:
                demo_array_personalizado();
                break;
                
            case 5:
                demo_analisis_detallado();
                break;
                
            case 6:
                ejecutar_demo_completa_suma_paralela();
                break;
                
            case 0:
                printf("Saliendo del programa...\n");
                break;
                
            default:
                printf("Opción inválida. Intente nuevamente.\n");
                break;
        }
        
        if (opcion != 0) {
            printf("\nPresione Enter para continuar...");
            while (getchar() != '\n'); // Limpiar buffer
            getchar(); // Esperar Enter
        }
        
    } while (opcion != 0);
    
    return 0;
}
#endif
