/**
 * @file main.c
 * @brief Programa principal para demostrar hilos POSIX básicos
 * @author Ejercicios C
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hilos_basicos.h"

/* ====================================================================
 * FUNCIONES DE PRESENTACIÓN
 * ==================================================================== */

void mostrar_titulo(void) {
    printf("🧵 DEMOSTRACIÓN DE HILOS POSIX (pthread)\n");
    printf("Introducción a la programación concurrente en C\n");
    printf("=============================================\n\n");
}

void mostrar_menu(void) {
    printf("Selecciona una demostración:\n");
    printf("1. Hilo básico (ejemplo del enunciado)\n");
    printf("2. Hilos con parámetros personalizados\n");
    printf("3. Múltiples hilos trabajadores\n");
    printf("4. Cálculo paralelo\n");
    printf("5. Información del sistema de hilos\n");
    printf("6. Ejecutar ejemplo simple\n");
    printf("7. Demostración completa (todos los casos)\n");
    printf("0. Salir\n");
    printf("\nOpción: ");
}

/* ====================================================================
 * EJEMPLO SIMPLE DEL ENUNCIADO
 * ==================================================================== */

// Función que ejecutará el hilo (del enunciado original)
void* funcionDelHilo(void* arg) {
    (void)arg; // Suprimir warning
    printf("Hola desde el hilo hijo.\n");
    return NULL;
}

int ejecutar_ejemplo_simple(void) {
    printf("\n=== EJEMPLO SIMPLE DEL ENUNCIADO ===\n");
    
    pthread_t hilo;

    // Crear el hilo
    if (pthread_create(&hilo, NULL, funcionDelHilo, NULL) != 0) {
        perror("Error al crear el hilo");
        return 1;
    }

    printf("Hilo principal esperando al hilo hijo...\n");

    // Esperar a que el hilo termine
    if (pthread_join(hilo, NULL) != 0) {
        perror("Error al esperar el hilo");
        return 1;
    }

    printf("Hilo hijo ha terminado. Fin del programa.\n");
    
    return 0;
}

/* ====================================================================
 * FUNCIÓN DE DEMOSTRACIÓN COMPLETA
 * ==================================================================== */

int ejecutar_demostracion_completa(void) {
    printf("\n🎭 EJECUTANDO DEMOSTRACIÓN COMPLETA\n");
    printf("===================================\n");
    
    int resultado = 0;
    
    // 1. Mostrar información del sistema
    mostrar_info_hilos();
    
    // 2. Hilo básico
    resultado = demostrar_hilo_basico();
    if (resultado != 0) {
        printf("❌ Error en demostración básica: %d\n", resultado);
        return resultado;
    }
    
    printf("\n⏳ Pausa entre demostraciones...\n");
    sleep(2);
    
    // 3. Hilos con parámetros
    resultado = demostrar_hilos_con_parametros();
    if (resultado != 0) {
        printf("❌ Error en hilos con parámetros: %d\n", resultado);
        return resultado;
    }
    
    printf("\n⏳ Pausa entre demostraciones...\n");
    sleep(2);
    
    // 4. Múltiples hilos
    resultado = demostrar_multiples_hilos(5);
    if (resultado != 0) {
        printf("❌ Error en múltiples hilos: %d\n", resultado);
        return resultado;
    }
    
    printf("\n⏳ Pausa entre demostraciones...\n");
    sleep(2);
    
    // 5. Cálculo paralelo
    resultado = demostrar_calculo_paralelo();
    if (resultado != 0) {
        printf("❌ Error en cálculo paralelo: %d\n", resultado);
        return resultado;
    }
    
    printf("\n🎉 DEMOSTRACIÓN COMPLETA FINALIZADA\n");
    printf("===================================\n");
    printf("✅ Todas las demostraciones ejecutadas exitosamente\n");
    printf("🎓 Has aprendido los conceptos básicos de hilos POSIX:\n");
    printf("   • Creación de hilos con pthread_create()\n");
    printf("   • Sincronización con pthread_join()\n");
    printf("   • Paso de parámetros a hilos\n");
    printf("   • Ejecución paralela y concurrencia\n");
    printf("   • Manejo de resultados de hilos\n");
    
    return 0;
}

/* ====================================================================
 * FUNCIÓN PRINCIPAL INTERACTIVA
 * ==================================================================== */

#ifndef UNIT_TESTING
int main(void) {
    mostrar_titulo();
    
    // Verificar soporte de pthread
    if (!verificar_soporte_pthread()) {
        printf("❌ Error: La biblioteca pthread no está disponible\n");
        printf("💡 En algunos sistemas, puede necesitar:\n");
        printf("   - Compilar con: gcc -pthread programa.c\n");
        printf("   - O instalar: sudo apt-get install libpthread-stubs0-dev\n");
        return 1;
    }
    
    mostrar_info_hilos();
    
    int opcion;
    bool continuar = true;
    
    while (continuar) {
        mostrar_menu();
        
        if (scanf("%d", &opcion) != 1) {
            // Limpiar buffer en caso de entrada inválida
            while (getchar() != '\n');
            printf("❌ Opción inválida. Intenta de nuevo.\n\n");
            continue;
        }
        
        printf("\n");
        int resultado = 0;
        
        switch (opcion) {
            case 1:
                resultado = demostrar_hilo_basico();
                break;
                
            case 2:
                resultado = demostrar_hilos_con_parametros();
                break;
                
            case 3: {
                printf("¿Cuántos hilos crear? (1-%d): ", MAX_HILOS);
                int num_hilos;
                if (scanf("%d", &num_hilos) == 1) {
                    resultado = demostrar_multiples_hilos(num_hilos);
                } else {
                    printf("❌ Número inválido, usando 3 hilos por defecto\n");
                    resultado = demostrar_multiples_hilos(3);
                }
                break;
            }
            
            case 4:
                resultado = demostrar_calculo_paralelo();
                break;
                
            case 5:
                mostrar_info_hilos();
                break;
                
            case 6:
                resultado = ejecutar_ejemplo_simple();
                break;
                
            case 7:
                resultado = ejecutar_demostracion_completa();
                break;
                
            case 0:
                continuar = false;
                printf("👋 ¡Hasta luego!\n");
                break;
                
            default:
                printf("❌ Opción inválida\n");
                break;
        }
        
        if (resultado != 0) {
            printf("❌ Error en la operación: código %d\n", resultado);
        }
        
        if (continuar && opcion != 5) {
            printf("\nPresiona Enter para continuar...");
            while (getchar() != '\n'); // Limpiar buffer
            getchar(); // Esperar Enter
            printf("\n");
        }
    }
    
    printf("\n🎓 RESUMEN EDUCATIVO:\n");
    printf("===================\n");
    printf("• pthread_create() - Crea un nuevo hilo de ejecución\n");
    printf("• pthread_join() - Espera a que termine un hilo específico\n");
    printf("• Los hilos comparten el espacio de memoria del proceso\n");
    printf("• Cada hilo tiene su propia pila de ejecución\n");
    printf("• La concurrencia permite ejecutar tareas en paralelo\n");
    printf("• Es importante manejar la sincronización entre hilos\n");
    printf("• pthread es el estándar POSIX para hilos en Unix/Linux\n");
    
    printf("\n🔧 PRÓXIMOS PASOS:\n");
    printf("================\n");
    printf("• Aprende sobre mutex y sincronización\n");
    printf("• Estudia condiciones de carrera (race conditions)\n");
    printf("• Explora variables de condición (condition variables)\n");
    printf("• Investiga pools de hilos y patrones avanzados\n");
    
    return 0;
}
#endif
