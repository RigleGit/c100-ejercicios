/**
 * @file demo_simple.c
 * @brief Demostración simple de race conditions
 * 
 * Este programa muestra un ejemplo básico y directo de race conditions
 * sin toda la complejidad del programa principal completo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_HILOS 2
#define INCREMENTOS_POR_HILO 100000

// Variable global compartida - aquí está el problema
static int contador_global = 0;

// Función que ejecuta cada hilo
void* incrementar_contador(void* arg) {
    int id_hilo = *(int*)arg;
    
    printf("Hilo %d iniciando...\n", id_hilo);
    
    // Cada hilo incrementa el contador muchas veces
    for (int i = 0; i < INCREMENTOS_POR_HILO; i++) {
        // RACE CONDITION: Esta operación NO es atómica
        contador_global++;
        
        // Opcional: añadir pequeño delay para aumentar la probabilidad
        // de race condition en sistemas muy rápidos
        if (i % 10000 == 0) {
            usleep(1);  // 1 microsegundo cada 10k incrementos
        }
    }
    
    printf("Hilo %d terminado\n", id_hilo);
    return NULL;
}

int main(void) {
    printf("=== DEMOSTRACIÓN SIMPLE DE RACE CONDITIONS ===\n\n");
    
    printf("Configuración:\n");
    printf("- Número de hilos: %d\n", NUM_HILOS);
    printf("- Incrementos por hilo: %d\n", INCREMENTOS_POR_HILO);
    printf("- Total esperado: %d\n\n", NUM_HILOS * INCREMENTOS_POR_HILO);
    
    // Variables para los hilos
    pthread_t hilos[NUM_HILOS];
    int ids_hilos[NUM_HILOS];
    
    printf("Iniciando experimento...\n");
    
    // Crear los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids_hilos[i] = i;
        int ret = pthread_create(&hilos[i], NULL, incrementar_contador, &ids_hilos[i]);
        if (ret != 0) {
            perror("Error creando hilo");
            exit(EXIT_FAILURE);
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    printf("\n=== RESULTADOS ===\n");
    printf("Valor esperado: %d\n", NUM_HILOS * INCREMENTOS_POR_HILO);
    printf("Valor obtenido: %d\n", contador_global);
    
    int diferencia = (NUM_HILOS * INCREMENTOS_POR_HILO) - contador_global;
    printf("Diferencia: %d\n", diferencia);
    
    if (diferencia == 0) {
        printf("Estado: ✓ CORRECTO\n");
        printf("\nOBSERVACIÓN: En esta ejecución no se detectó race condition.\n");
        printf("Esto puede ocurrir ocasionalmente. Ejecute el programa varias\n");
        printf("veces para observar resultados inconsistentes.\n");
    } else {
        printf("Estado: ✗ RACE CONDITION DETECTADA\n");
        printf("Incrementos perdidos: %d\n", diferencia);
        
        double porcentaje_error = (double)diferencia * 100.0 / (NUM_HILOS * INCREMENTOS_POR_HILO);
        printf("Porcentaje de error: %.2f%%\n", porcentaje_error);
        
        printf("\nEXPLICACIÓN:\n");
        printf("Los incrementos perdidos ocurren porque la operación\n");
        printf("'contador_global++' no es atómica. Internamente realiza:\n");
        printf("1. Leer el valor actual de contador_global\n");
        printf("2. Incrementar el valor leído\n");
        printf("3. Escribir el nuevo valor a contador_global\n");
        printf("\nSi dos hilos ejecutan estos pasos al mismo tiempo,\n");
        printf("pueden leer el mismo valor inicial y ambos escribir\n");
        printf("el mismo valor incrementado, perdiendo un incremento.\n");
    }
    
    printf("\n=== RECOMENDACIONES ===\n");
    printf("• En código real, SIEMPRE use sincronización apropiada\n");
    printf("• Use mutex, semáforos u operaciones atómicas\n");
    printf("• Teste su código concurrente exhaustivamente\n");
    printf("• Use herramientas como ThreadSanitizer para detectar races\n");
    
    printf("\n=== EXPERIMENTO ADICIONAL ===\n");
    printf("Ejecute este programa múltiples veces para observar:\n");
    printf("• Resultados diferentes en cada ejecución\n");
    printf("• Variabilidad en la cantidad de incrementos perdidos\n");
    printf("• Ocasionalmente, resultados correctos por casualidad\n");
    
    return EXIT_SUCCESS;
}
