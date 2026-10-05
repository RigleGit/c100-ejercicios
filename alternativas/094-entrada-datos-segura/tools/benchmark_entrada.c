/**
 * @file benchmark_entrada.c
 * @brief Benchmark de rendimiento para funciones de entrada segura
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "entrada_segura.h"

#define ITERACIONES_DEFAULT 10000
#define HILOS_DEFAULT 4
#define BUFFER_SIZE_BENCH 256

/**
 * @brief Estructura para resultados de benchmark
 */
typedef struct {
    double tiempo_promedio;
    double tiempo_minimo;
    double tiempo_maximo;
    double desviacion_estandar;
    size_t iteraciones;
    size_t errores;
} resultado_benchmark_t;

/**
 * @brief Estructura para datos de hilo
 */
typedef struct {
    int id_hilo;
    size_t iteraciones;
    resultado_benchmark_t resultado;
    const char* datos_prueba;
} datos_hilo_t;

/**
 * @brief Obtiene tiempo actual en microsegundos
 */
static double obtener_tiempo_microsegundos(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

/**
 * @brief Benchmark de fgets() estándar
 */
static resultado_benchmark_t benchmark_fgets(size_t iteraciones) {
    resultado_benchmark_t resultado = {0};
    double *tiempos = malloc(iteraciones * sizeof(double));
    if (!tiempos) return resultado;
    
    char buffer[BUFFER_SIZE_BENCH];
    FILE *fp;
    
    for (size_t i = 0; i < iteraciones; i++) {
        // Crear entrada temporal
        fp = tmpfile();
        if (!fp) {
            resultado.errores++;
            continue;
        }
        
        fprintf(fp, "Texto de prueba para benchmark %zu\n", i);
        rewind(fp);
        
        double inicio = obtener_tiempo_microsegundos();
        
        char *ret = fgets(buffer, sizeof(buffer), fp);
        
        double fin = obtener_tiempo_microsegundos();
        
        if (ret == NULL) {
            resultado.errores++;
        }
        
        tiempos[i] = fin - inicio;
        fclose(fp);
    }
    
    // Calcular estadísticas
    resultado.iteraciones = iteraciones;
    resultado.tiempo_minimo = tiempos[0];
    resultado.tiempo_maximo = tiempos[0];
    double suma = 0.0;
    
    for (size_t i = 0; i < iteraciones; i++) {
        suma += tiempos[i];
        if (tiempos[i] < resultado.tiempo_minimo) {
            resultado.tiempo_minimo = tiempos[i];
        }
        if (tiempos[i] > resultado.tiempo_maximo) {
            resultado.tiempo_maximo = tiempos[i];
        }
    }
    
    resultado.tiempo_promedio = suma / iteraciones;
    
    // Calcular desviación estándar
    double suma_cuadrados = 0.0;
    for (size_t i = 0; i < iteraciones; i++) {
        double diff = tiempos[i] - resultado.tiempo_promedio;
        suma_cuadrados += diff * diff;
    }
    resultado.desviacion_estandar = sqrt(suma_cuadrados / iteraciones);
    
    free(tiempos);
    return resultado;
}

/**
 * @brief Benchmark de leer_cadena_segura()
 */
static resultado_benchmark_t benchmark_entrada_segura(size_t iteraciones) {
    resultado_benchmark_t resultado = {0};
    double *tiempos = malloc(iteraciones * sizeof(double));
    if (!tiempos) return resultado;
    
    char buffer[BUFFER_SIZE_BENCH];
    FILE *fp_original = stdin;
    
    for (size_t i = 0; i < iteraciones; i++) {
        // Crear entrada simulada
        FILE *fp = tmpfile();
        if (!fp) {
            resultado.errores++;
            continue;
        }
        
        fprintf(fp, "Datos de prueba para entrada segura %zu\n", i);
        rewind(fp);
        stdin = fp;
        
        double inicio = obtener_tiempo_microsegundos();
        
        resultado_entrada_t ret = leer_cadena_segura(buffer, sizeof(buffer));
        
        double fin = obtener_tiempo_microsegundos();
        
        if (ret != ENTRADA_OK) {
            resultado.errores++;
        }
        
        tiempos[i] = fin - inicio;
        fclose(fp);
    }
    
    stdin = fp_original;
    
    // Calcular estadísticas (mismo código que arriba)
    resultado.iteraciones = iteraciones;
    resultado.tiempo_minimo = tiempos[0];
    resultado.tiempo_maximo = tiempos[0];
    double suma = 0.0;
    
    for (size_t i = 0; i < iteraciones; i++) {
        suma += tiempos[i];
        if (tiempos[i] < resultado.tiempo_minimo) {
            resultado.tiempo_minimo = tiempos[i];
        }
        if (tiempos[i] > resultado.tiempo_maximo) {
            resultado.tiempo_maximo = tiempos[i];
        }
    }
    
    resultado.tiempo_promedio = suma / iteraciones;
    
    double suma_cuadrados = 0.0;
    for (size_t i = 0; i < iteraciones; i++) {
        double diff = tiempos[i] - resultado.tiempo_promedio;
        suma_cuadrados += diff * diff;
    }
    resultado.desviacion_estandar = sqrt(suma_cuadrados / iteraciones);
    
    free(tiempos);
    return resultado;
}

/**
 * @brief Benchmark de entrada de enteros
 */
static resultado_benchmark_t benchmark_entero_seguro(size_t iteraciones) {
    resultado_benchmark_t resultado = {0};
    double *tiempos = malloc(iteraciones * sizeof(double));
    if (!tiempos) return resultado;
    
    int valor;
    FILE *fp_original = stdin;
    
    for (size_t i = 0; i < iteraciones; i++) {
        FILE *fp = tmpfile();
        if (!fp) {
            resultado.errores++;
            continue;
        }
        
        fprintf(fp, "%zu\n", i % 1000);
        rewind(fp);
        stdin = fp;
        
        double inicio = obtener_tiempo_microsegundos();
        
        resultado_entrada_t ret = leer_entero_seguro(&valor, 0, 999);
        
        double fin = obtener_tiempo_microsegundos();
        
        if (ret != ENTRADA_OK) {
            resultado.errores++;
        }
        
        tiempos[i] = fin - inicio;
        fclose(fp);
    }
    
    stdin = fp_original;
    
    // Calcular estadísticas
    resultado.iteraciones = iteraciones;
    if (iteraciones > 0) {
        resultado.tiempo_minimo = tiempos[0];
        resultado.tiempo_maximo = tiempos[0];
        double suma = 0.0;
        
        for (size_t i = 0; i < iteraciones; i++) {
            suma += tiempos[i];
            if (tiempos[i] < resultado.tiempo_minimo) {
                resultado.tiempo_minimo = tiempos[i];
            }
            if (tiempos[i] > resultado.tiempo_maximo) {
                resultado.tiempo_maximo = tiempos[i];
            }
        }
        
        resultado.tiempo_promedio = suma / iteraciones;
        
        double suma_cuadrados = 0.0;
        for (size_t i = 0; i < iteraciones; i++) {
            double diff = tiempos[i] - resultado.tiempo_promedio;
            suma_cuadrados += diff * diff;
        }
        resultado.desviacion_estandar = sqrt(suma_cuadrados / iteraciones);
    }
    
    free(tiempos);
    return resultado;
}

/**
 * @brief Función de hilo para benchmark concurrente
 */
static void* hilo_benchmark(void* arg) {
    datos_hilo_t* datos = (datos_hilo_t*)arg;
    
    datos->resultado = benchmark_entrada_segura(datos->iteraciones);
    
    return NULL;
}

/**
 * @brief Benchmark concurrente con múltiples hilos
 */
static void benchmark_concurrente(size_t iteraciones, int num_hilos) {
    printf("\n" COLOR_AZUL "=== BENCHMARK CONCURRENTE ===" COLOR_RESET "\n");
    printf("Hilos: %d, Iteraciones por hilo: %zu\n\n", num_hilos, iteraciones);
    
    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));
    datos_hilo_t* datos_hilos = malloc(num_hilos * sizeof(datos_hilo_t));
    
    if (!hilos || !datos_hilos) {
        printf("Error al asignar memoria para hilos\n");
        free(hilos);
        free(datos_hilos);
        return;
    }
    
    double inicio_total = obtener_tiempo_microsegundos();
    
    // Crear hilos
    for (int i = 0; i < num_hilos; i++) {
        datos_hilos[i].id_hilo = i;
        datos_hilos[i].iteraciones = iteraciones;
        
        if (pthread_create(&hilos[i], NULL, hilo_benchmark, &datos_hilos[i]) != 0) {
            printf("Error al crear hilo %d\n", i);
            num_hilos = i;
            break;
        }
    }
    
    // Esperar hilos
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    double fin_total = obtener_tiempo_microsegundos();
    
    // Mostrar resultados
    printf("Resultados por hilo:\n");
    printf("┌──────┬─────────────┬─────────────┬─────────────┬─────────┐\n");
    printf("│ Hilo │ Promedio μs │ Mínimo μs   │ Máximo μs   │ Errores │\n");
    printf("├──────┼─────────────┼─────────────┼─────────────┼─────────┤\n");
    
    double tiempo_total_promedio = 0.0;
    size_t errores_totales = 0;
    
    for (int i = 0; i < num_hilos; i++) {
        printf("│ %4d │ %11.2f │ %11.2f │ %11.2f │ %7zu │\n",
               datos_hilos[i].id_hilo,
               datos_hilos[i].resultado.tiempo_promedio,
               datos_hilos[i].resultado.tiempo_minimo,
               datos_hilos[i].resultado.tiempo_maximo,
               datos_hilos[i].resultado.errores);
        
        tiempo_total_promedio += datos_hilos[i].resultado.tiempo_promedio;
        errores_totales += datos_hilos[i].resultado.errores;
    }
    
    printf("└──────┴─────────────┴─────────────┴─────────────┴─────────┘\n");
    
    printf("\nResumen concurrente:\n");
    printf("Tiempo total:        %.2f ms\n", (fin_total - inicio_total) / 1000.0);
    printf("Promedio por hilo:   %.2f μs\n", tiempo_total_promedio / num_hilos);
    printf("Throughput:          %.2f ops/s\n", 
           (iteraciones * num_hilos) / ((fin_total - inicio_total) / 1000000.0));
    printf("Errores totales:     %zu\n", errores_totales);
    
    free(hilos);
    free(datos_hilos);
}

/**
 * @brief Imprime resultados de benchmark
 */
static void imprimir_resultado(const char* nombre, resultado_benchmark_t resultado) {
    printf("=== %s ===\n", nombre);
    printf("Iteraciones:         %zu\n", resultado.iteraciones);
    printf("Tiempo promedio:     %.2f μs\n", resultado.tiempo_promedio);
    printf("Tiempo mínimo:       %.2f μs\n", resultado.tiempo_minimo);
    printf("Tiempo máximo:       %.2f μs\n", resultado.tiempo_maximo);
    printf("Desviación estándar: %.2f μs\n", resultado.desviacion_estandar);
    printf("Errores:             %zu\n", resultado.errores);
    printf("Throughput:          %.2f ops/s\n", 
           1000000.0 / resultado.tiempo_promedio);
    printf("\n");
}

/**
 * @brief Ejecuta benchmark completo
 */
static void ejecutar_benchmark_completo(size_t iteraciones) {
    printf(COLOR_AZUL "=== BENCHMARK DE RENDIMIENTO ===" COLOR_RESET "\n");
    printf("Iteraciones: %zu\n\n", iteraciones);
    
    printf("Ejecutando benchmarks...\n");
    
    resultado_benchmark_t resultado_fgets = benchmark_fgets(iteraciones);
    imprimir_resultado("fgets() estándar", resultado_fgets);
    
    resultado_benchmark_t resultado_segura = benchmark_entrada_segura(iteraciones);
    imprimir_resultado("leer_cadena_segura()", resultado_segura);
    
    resultado_benchmark_t resultado_entero = benchmark_entero_seguro(iteraciones);
    imprimir_resultado("leer_entero_seguro()", resultado_entero);
    
    // Comparación
    printf(COLOR_VERDE "=== COMPARACIÓN ===" COLOR_RESET "\n");
    printf("┌─────────────────────────┬─────────────┬─────────────┬──────────┐\n");
    printf("│ Función                 │ Promedio μs │ Throughput  │ Overhead │\n");
    printf("├─────────────────────────┼─────────────┼─────────────┼──────────┤\n");
    printf("│ fgets()                 │ %11.2f │ %9.0f   │   0.0%%   │\n",
           resultado_fgets.tiempo_promedio,
           1000000.0 / resultado_fgets.tiempo_promedio);
    
    double overhead = ((resultado_segura.tiempo_promedio - resultado_fgets.tiempo_promedio) 
                      / resultado_fgets.tiempo_promedio) * 100.0;
    
    printf("│ leer_cadena_segura()    │ %11.2f │ %9.0f   │ %7.1f%% │\n",
           resultado_segura.tiempo_promedio,
           1000000.0 / resultado_segura.tiempo_promedio,
           overhead);
    
    printf("│ leer_entero_seguro()    │ %11.2f │ %9.0f   │    N/A   │\n",
           resultado_entero.tiempo_promedio,
           1000000.0 / resultado_entero.tiempo_promedio);
    
    printf("└─────────────────────────┴─────────────┴─────────────┴──────────┘\n");
}

/**
 * @brief Benchmark de stress test
 */
static void stress_test(void) {
    printf("\n" COLOR_AMARILLO "=== STRESS TEST ===" COLOR_RESET "\n");
    printf("Probando con entradas extremas...\n\n");
    
    // Test con buffer muy grande
    char buffer_grande[10000];
    FILE *fp_original = stdin;
    FILE *fp = tmpfile();
    
    if (fp) {
        // Crear entrada muy larga
        for (int i = 0; i < 5000; i++) {
            fprintf(fp, "A");
        }
        fprintf(fp, "\n");
        rewind(fp);
        stdin = fp;
        
        double inicio = obtener_tiempo_microsegundos();
        resultado_entrada_t resultado = leer_cadena_segura(buffer_grande, sizeof(buffer_grande));
        double fin = obtener_tiempo_microsegundos();
        
        printf("Entrada muy larga (5000 chars):\n");
        printf("Resultado: %s\n", resultado == ENTRADA_OK ? "OK" : "ERROR");
        printf("Tiempo: %.2f μs\n", fin - inicio);
        printf("Caracteres leídos: %zu\n\n", strlen(buffer_grande));
        
        fclose(fp);
    }
    
    stdin = fp_original;
    
    // Test con muchas validaciones
    printf("Test de validación intensiva:\n");
    int valor;
    fp = tmpfile();
    if (fp) {
        fprintf(fp, "999999999\n");  // Valor fuera de rango
        rewind(fp);
        stdin = fp;
        
        double inicio = obtener_tiempo_microsegundos();
        resultado_entrada_t resultado = leer_entero_seguro(&valor, 0, 100);
        double fin = obtener_tiempo_microsegundos();
        
        printf("Validación de rango (fuera de límites):\n");
        printf("Resultado: %s\n", resultado == ENTRADA_ERROR_RANGO ? "RECHAZADO (correcto)" : "ERROR");
        printf("Tiempo: %.2f μs\n", fin - inicio);
        
        fclose(fp);
    }
    
    stdin = fp_original;
}

/**
 * @brief Menú principal
 */
static void mostrar_menu(void) {
    printf("\n" COLOR_AZUL "=== BENCHMARK DE ENTRADA SEGURA ===" COLOR_RESET "\n");
    printf("1. Benchmark rápido (1,000 iteraciones)\n");
    printf("2. Benchmark medio (10,000 iteraciones)\n");
    printf("3. Benchmark intensivo (100,000 iteraciones)\n");
    printf("4. Benchmark concurrente\n");
    printf("5. Stress test\n");
    printf("6. Benchmark personalizado\n");
    printf("0. Salir\n");
    printf("\nSelecciona una opción: ");
}

/**
 * @brief Función principal
 */
int main(int argc, char *argv[]) {
    size_t iteraciones = ITERACIONES_DEFAULT;
    int num_hilos = HILOS_DEFAULT;
    
    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--iteraciones") == 0 && i + 1 < argc) {
            iteraciones = (size_t)atol(argv[++i]);
        } else if (strcmp(argv[i], "--hilos") == 0 && i + 1 < argc) {
            num_hilos = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--auto") == 0) {
            ejecutar_benchmark_completo(iteraciones);
            return 0;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Uso: %s [opciones]\n", argv[0]);
            printf("Opciones:\n");
            printf("  --iteraciones N    Número de iteraciones (default: %d)\n", ITERACIONES_DEFAULT);
            printf("  --hilos N          Número de hilos (default: %d)\n", HILOS_DEFAULT);
            printf("  --auto             Ejecutar benchmark automático\n");
            printf("  --help             Mostrar esta ayuda\n");
            return 0;
        }
    }
    
    int opcion;
    
    do {
        mostrar_menu();
        
        resultado_entrada_t resultado = leer_entero_seguro(&opcion, 0, 6);
        
        if (resultado != ENTRADA_OK) {
            printf(COLOR_ROJO "Opción inválida\n" COLOR_RESET);
            continue;
        }
        
        switch (opcion) {
            case 1:
                ejecutar_benchmark_completo(1000);
                break;
            case 2:
                ejecutar_benchmark_completo(10000);
                break;
            case 3:
                ejecutar_benchmark_completo(100000);
                break;
            case 4:
                benchmark_concurrente(iteraciones / num_hilos, num_hilos);
                break;
            case 5:
                stress_test();
                break;
            case 6: {
                size_t iter_custom;
                printf("Ingresa número de iteraciones: ");
                if (leer_entero_seguro((int*)&iter_custom, 1, 1000000) == ENTRADA_OK) {
                    ejecutar_benchmark_completo(iter_custom);
                }
                break;
            }
            case 0:
                printf(COLOR_VERDE "¡Benchmark completado!\n" COLOR_RESET);
                break;
            default:
                printf(COLOR_ROJO "Opción no válida\n" COLOR_RESET);
                break;
        }
        
        if (opcion != 0) {
            printf("\nPresiona Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 0);
    
    return 0;
}
