#include "../include/sincronizacion_mutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void imprimir_menu(void) {
    printf("\n=== Ejercicio 085: Sincronización con Mutex ===\n");
    printf("1. Demostrar race condition (sin mutex)\n");
    printf("2. Demostrar solución con mutex\n");
    printf("3. Comparar rendimiento (con vs sin mutex)\n");
    printf("4. Demostrar tipos de mutex\n");
    printf("5. Prueba de timeout en mutex\n");
    printf("6. Demostración completa\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

static void demo_race_condition(void) {
    printf("\n=== Demostración de Race Condition ===\n");
    printf("Ejecutando múltiples hilos que incrementan un contador sin protección...\n");
    
    int64_t resultado = demostrar_race_condition(5, 2000);
    printf("\nComo puede ver, el resultado final (%ld) probablemente no coincide\n", resultado);
    printf("con el valor esperado (10000). Esto demuestra el problema de las\n");
    printf("condiciones de carrera cuando múltiples hilos acceden concurrentemente\n");
    printf("a un recurso compartido sin sincronización.\n");
}

static void demo_solucion_mutex(void) {
    printf("\n=== Demostración de Solución con Mutex ===\n");
    
    recurso_compartido_t recurso;
    estadisticas_mutex_t stats;
    
    if (!inicializar_recurso_compartido(&recurso, 0)) {
        printf("Error inicializando recurso compartido\n");
        return;
    }
    
    if (!inicializar_estadisticas_mutex(&stats)) {
        printf("Error inicializando estadísticas\n");
        destruir_recurso_compartido(&recurso);
        return;
    }
    
    parametros_prueba_t params = {
        .num_hilos = 5,
        .operaciones_por_hilo = 2000,
        .recurso = &recurso,
        .stats = &stats,
        .usar_mutex = true
    };
    
    printf("Ejecutando la misma prueba pero con protección de mutex...\n");
    ejecutar_prueba_rendimiento(&params);
    
    printf("\nComo puede observar, ahora el resultado es consistente y correcto.\n");
    printf("El mutex garantiza que solo un hilo puede modificar el recurso a la vez.\n");
    
    destruir_estadisticas_mutex(&stats);
    destruir_recurso_compartido(&recurso);
}

static void demo_comparacion_rendimiento(void) {
    printf("\n=== Comparación de Rendimiento ===\n");
    
    recurso_compartido_t recurso;
    estadisticas_mutex_t stats_con_mutex, stats_sin_mutex;
    
    inicializar_recurso_compartido(&recurso, 0);
    inicializar_estadisticas_mutex(&stats_con_mutex);
    inicializar_estadisticas_mutex(&stats_sin_mutex);
    
    parametros_prueba_t params = {
        .num_hilos = 3,
        .operaciones_por_hilo = 1000,
        .recurso = &recurso,
        .stats = &stats_sin_mutex,
        .usar_mutex = false
    };
    
    printf("Ejecutando prueba SIN mutex (no thread-safe):\n");
    ejecutar_prueba_rendimiento(&params);
    
    // Reset del recurso
    establecer_recurso_seguro(&recurso, 0);
    
    params.stats = &stats_con_mutex;
    params.usar_mutex = true;
    
    printf("\nEjecutando prueba CON mutex (thread-safe):\n");
    ejecutar_prueba_rendimiento(&params);
    
    printf("\n=== Análisis de Rendimiento ===\n");
    printf("Sin mutex: más rápido pero INCORRECTOS resultados\n");
    printf("Con mutex: más lento pero CORRECTOS resultados\n");
    printf("La sincronización tiene un costo, pero garantiza la correctitud.\n");
    
    destruir_estadisticas_mutex(&stats_con_mutex);
    destruir_estadisticas_mutex(&stats_sin_mutex);
    destruir_recurso_compartido(&recurso);
}

static void demo_tipos_mutex(void) {
    printf("\n=== Demostración de Tipos de Mutex ===\n");
    
    demo_tipos_mutex_t demo;
    if (!inicializar_demo_tipos_mutex(&demo)) {
        printf("Error inicializando demostración de tipos de mutex\n");
        return;
    }
    
    printf("\n1. Mutex Recursivo:\n");
    printf("Permite que el mismo hilo adquiera el mutex múltiples veces.\n");
    demostrar_mutex_recursivo(&demo, 3);
    
    printf("\n2. Mutex ErrorCheck:\n");
    printf("Detecta errores comunes como deadlocks y double-unlocks.\n");
    demostrar_mutex_errorcheck(&demo);
    
    printf("\nCada tipo de mutex tiene sus ventajas:\n");
    printf("- Normal: Más rápido, pero sin verificaciones de error\n");
    printf("- Recursivo: Permite re-adquisición por el mismo hilo\n");
    printf("- ErrorCheck: Detecta errores de uso del mutex\n");
    
    destruir_demo_tipos_mutex(&demo);
}

static void demo_timeout_mutex(void) {
    printf("\n=== Demostración de Timeout en Mutex ===\n");
    
    pthread_mutex_t mutex_test;
    pthread_mutex_init(&mutex_test, NULL);
    
    printf("Adquiriendo mutex...\n");
    pthread_mutex_lock(&mutex_test);
    
    printf("Intentando adquirir el mismo mutex con timeout de 1000ms...\n");
    bool resultado = mutex_trylock_timeout(&mutex_test, 1000);
    
    if (!resultado) {
        printf("Como era esperado, el timeout evitó que el programa se bloquee.\n");
    } else {
        printf("Inesperado: el mutex fue adquirido.\n");
    }
    
    printf("Liberando mutex original...\n");
    pthread_mutex_unlock(&mutex_test);
    
    printf("Intentando adquirir mutex nuevamente con timeout...\n");
    resultado = mutex_trylock_timeout(&mutex_test, 100);
    
    if (resultado) {
        printf("Éxito: mutex adquirido dentro del timeout.\n");
        pthread_mutex_unlock(&mutex_test);
    } else {
        printf("Error: no se pudo adquirir mutex libre.\n");
    }
    
    pthread_mutex_destroy(&mutex_test);
    
    printf("\nLos timeouts son útiles para evitar bloqueos indefinidos\n");
    printf("y implementar lógica de fallback cuando un recurso no está disponible.\n");
}

#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    
    printf("Este programa demuestra el uso de mutex para sincronización de hilos.\n");
    printf("Los mutex son fundamentales para evitar condiciones de carrera\n");
    printf("cuando múltiples hilos acceden a recursos compartidos.\n");
    
    do {
        imprimir_menu();
        
        if (scanf("%d", &opcion) != 1) {
            printf("Error leyendo opción. Intente nuevamente.\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        
        switch (opcion) {
            case 1:
                demo_race_condition();
                break;
                
            case 2:
                demo_solucion_mutex();
                break;
                
            case 3:
                demo_comparacion_rendimiento();
                break;
                
            case 4:
                demo_tipos_mutex();
                break;
                
            case 5:
                demo_timeout_mutex();
                break;
                
            case 6:
                ejecutar_demo_completa_mutex();
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
