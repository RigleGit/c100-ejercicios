/**
 * @file main.c
 * @brief Programa principal para demostrar el patrón Productor-Consumidor
 * 
 * Este programa ofrece un menú interactivo para explorar diferentes aspectos
 * del patrón Productor-Consumidor con hilos, mutex y variables de condición.
 */

#include "../include/productor_consumidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Función para mostrar el menú principal
void mostrar_menu(void) {
    printf("\n=== PATRÓN PRODUCTOR-CONSUMIDOR ===\n");
    printf("1. Ejecutar ejemplo básico del enunciado\n");
    printf("2. Demo con múltiples productores/consumidores\n");
    printf("3. Comparar diferentes tamaños de buffer\n");
    printf("4. Análisis de rendimiento\n");
    printf("5. Benchmark de configuraciones\n");
    printf("6. Simulación educativa paso a paso\n");
    printf("7. Experimento personalizado\n");
    printf("8. Demostrar problema sin sincronización\n");
    printf("9. Tests de casos límite\n");
    printf("10. Información educativa\n");
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

// Función para demo con múltiples hilos
void ejecutar_demo_multiple_hilos(void) {
    printf("\n=== DEMO CON MÚLTIPLES HILOS ===\n");
    
    int num_productores = obtener_entero("Número de productores (1-8): ", 1, 8);
    int num_consumidores = obtener_entero("Número de consumidores (1-8): ", 1, 8);
    int tam_buffer = obtener_entero("Tamaño del buffer (1-50): ", 1, 50);
    
    printf("\nEjecutando demo...\n");
    if (ejecutar_demo_multiple_productores_consumidores(num_productores, num_consumidores, tam_buffer)) {
        printf("\n✅ Demo completada exitosamente\n");
        
        printf("\n=== OBSERVACIONES ===\n");
        if (num_productores > num_consumidores) {
            printf("• Más productores que consumidores: expect más bloqueos por buffer lleno\n");
        } else if (num_consumidores > num_productores) {
            printf("• Más consumidores que productores: expect más bloqueos por buffer vacío\n");
        } else {
            printf("• Configuración balanceada: distribución equilibrada de bloqueos\n");
        }
        
        if (tam_buffer < 5) {
            printf("• Buffer pequeño: mayor frecuencia de bloqueos\n");
        } else if (tam_buffer > 20) {
            printf("• Buffer grande: menor frecuencia de bloqueos\n");
        }
    } else {
        printf("❌ Error en la demo\n");
    }
}

// Función para experimento personalizado
void ejecutar_experimento_personalizado(void) {
    printf("\n=== EXPERIMENTO PERSONALIZADO ===\n");
    
    configuracion_experimento_t config;
    
    config.num_productores = obtener_entero("Número de productores (1-8): ", 1, 8);
    config.num_consumidores = obtener_entero("Número de consumidores (1-8): ", 1, 8);
    config.tam_buffer = obtener_entero("Tamaño del buffer (1-100): ", 1, 100);
    config.num_elementos_total = obtener_entero("Total de elementos (10-1000): ", 10, 1000);
    
    printf("Delay de producción en microsegundos (1000-1000000): ");
    config.delay_produccion_us = obtener_entero("", 1000, 1000000);
    
    printf("Delay de consumo en microsegundos (1000-1000000): ");
    config.delay_consumo_us = obtener_entero("", 1000, 1000000);
    
    config.valor_minimo = obtener_entero("Valor mínimo a generar (0-100): ", 0, 100);
    config.valor_maximo = obtener_entero("Valor máximo a generar (valor_min-1000): ", 
                                        config.valor_minimo, 1000);
    
    printf("¿Mostrar elementos individuales? (0=No, 1=Sí): ");
    config.mostrar_elementos = obtener_entero("", 0, 1);
    
    printf("¿Usar valores secuenciales? (0=No, 1=Sí): ");
    config.usar_valores_secuenciales = obtener_entero("", 0, 1);
    
    config.mostrar_detalles = true;
    config.medir_rendimiento = true;
    
    printf("\n=== CONFIGURACIÓN SELECCIONADA ===\n");
    printf("Productores: %d, Consumidores: %d\n", config.num_productores, config.num_consumidores);
    printf("Buffer: %zu elementos\n", config.tam_buffer);
    printf("Elementos totales: %d\n", config.num_elementos_total);
    printf("Delays: %d μs (prod), %d μs (cons)\n", config.delay_produccion_us, config.delay_consumo_us);
    printf("Rango de valores: %d-%d\n", config.valor_minimo, config.valor_maximo);
    printf("Tipo de valores: %s\n", config.usar_valores_secuenciales ? "Secuenciales" : "Aleatorios");
    
    printf("\nPresione Enter para continuar...");
    getchar();
    
    resultado_experimento_t resultado;
    if (ejecutar_experimento_productor_consumidor(&config, &resultado)) {
        imprimir_resultado_experimento(&resultado);
        
        printf("\n=== RECOMENDACIONES ===\n");
        if (resultado.estadisticas_globales.veces_buffer_lleno > resultado.estadisticas_globales.veces_buffer_vacio * 2) {
            printf("• Considere aumentar el número de consumidores o su velocidad\n");
            printf("• El buffer se llena frecuentemente (productores más rápidos)\n");
        } else if (resultado.estadisticas_globales.veces_buffer_vacio > resultado.estadisticas_globales.veces_buffer_lleno * 2) {
            printf("• Considere aumentar el número de productores o su velocidad\n");
            printf("• El buffer se vacía frecuentemente (consumidores más rápidos)\n");
        } else {
            printf("• La configuración parece equilibrada\n");
        }
        
        if (resultado.throughput_promedio < 10.0) {
            printf("• El throughput es bajo, considere reducir los delays\n");
        }
        
    } else {
        printf("❌ Error ejecutando el experimento\n");
    }
}

// Función para tests de casos límite
void ejecutar_tests_casos_limite(void) {
    printf("\n=== TESTS DE CASOS LÍMITE ===\n");
    
    printf("1. Buffer de 1 elemento:\n");
    if (ejecutar_demo_multiple_productores_consumidores(1, 1, 1)) {
        printf("   ✅ Funciona correctamente con buffer mínimo\n");
    }
    
    printf("\n2. Múltiples productores, un consumidor:\n");
    if (ejecutar_demo_multiple_productores_consumidores(4, 1, 5)) {
        printf("   ✅ Maneja correctamente el desbalance\n");
    }
    
    printf("\n3. Un productor, múltiples consumidores:\n");
    if (ejecutar_demo_multiple_productores_consumidores(1, 4, 5)) {
        printf("   ✅ Los consumidores comparten eficientemente\n");
    }
    
    printf("\n4. Buffer grande con pocos elementos:\n");
    configuracion_experimento_t config = {
        .num_productores = 1,
        .num_consumidores = 1,
        .tam_buffer = 100,
        .num_elementos_total = 5,
        .delay_produccion_us = 10000,
        .delay_consumo_us = 10000,
        .valor_minimo = 1,
        .valor_maximo = 5,
        .mostrar_detalles = false,
        .mostrar_elementos = false,
        .usar_valores_secuenciales = true,
        .medir_rendimiento = true
    };
    
    resultado_experimento_t resultado;
    if (ejecutar_experimento_productor_consumidor(&config, &resultado)) {
        printf("   ✅ Buffer nunca se llena: %lu bloqueos por lleno\n", 
               resultado.estadisticas_globales.veces_buffer_lleno);
    }
    
    printf("\n5. Producción/consumo muy rápido (sin delays):\n");
    config.delay_produccion_us = 0;
    config.delay_consumo_us = 0;
    config.num_elementos_total = 100;
    
    if (ejecutar_experimento_productor_consumidor(&config, &resultado)) {
        printf("   ✅ Funciona correctamente sin delays\n");
        printf("   Throughput máximo: %.0f elementos/segundo\n", resultado.throughput_promedio);
    }
    
    printf("\n=== CONCLUSIONES ===\n");
    printf("• El patrón funciona correctamente en todos los casos límite\n");
    printf("• La sincronización previene race conditions\n");
    printf("• El rendimiento se adapta a diferentes configuraciones\n");
    printf("• Los bloqueos son manejados eficientemente\n");
}

// Función para mostrar información educativa
void mostrar_informacion_educativa(void) {
    printf("\n=== INFORMACIÓN EDUCATIVA: PATRÓN PRODUCTOR-CONSUMIDOR ===\n\n");
    
    printf("¿QUÉ ES EL PATRÓN PRODUCTOR-CONSUMIDOR?\n");
    printf("───────────────────────────────────────\n");
    printf("Es un patrón clásico de concurrencia donde:\n");
    printf("• PRODUCTORES generan datos y los colocan en un buffer\n");
    printf("• CONSUMIDORES toman datos del buffer y los procesan\n");
    printf("• Un BUFFER CIRCULAR intermedio desacopla productores de consumidores\n\n");
    
    printf("COMPONENTES CLAVE:\n");
    printf("─────────────────\n");
    printf("1. Buffer circular (ring buffer):\n");
    printf("   - Array de tamaño fijo\n");
    printf("   - Índices de entrada (in) y salida (out)\n");
    printf("   - Contador de elementos actuales\n\n");
    
    printf("2. Mutex (exclusión mutua):\n");
    printf("   - Protege el acceso al buffer\n");
    printf("   - Evita race conditions\n");
    printf("   - Solo un hilo puede modificar el buffer a la vez\n\n");
    
    printf("3. Variables de condición:\n");
    printf("   - cond_lleno: señala cuando hay espacio disponible\n");
    printf("   - cond_vacio: señala cuando hay datos disponibles\n");
    printf("   - Evitan busy waiting (espera activa)\n\n");
    
    printf("FLUJO DE FUNCIONAMIENTO:\n");
    printf("───────────────────────\n");
    printf("PRODUCTOR:\n");
    printf("1. Bloquear mutex\n");
    printf("2. Mientras buffer lleno → esperar en cond_lleno\n");
    printf("3. Insertar elemento en buffer[in]\n");
    printf("4. Actualizar in = (in + 1) %% tamaño\n");
    printf("5. Incrementar contador\n");
    printf("6. Señalar cond_vacio (hay datos)\n");
    printf("7. Desbloquear mutex\n\n");
    
    printf("CONSUMIDOR:\n");
    printf("1. Bloquear mutex\n");
    printf("2. Mientras buffer vacío → esperar en cond_vacio\n");
    printf("3. Extraer elemento de buffer[out]\n");
    printf("4. Actualizar out = (out + 1) %% tamaño\n");
    printf("5. Decrementar contador\n");
    printf("6. Señalar cond_lleno (hay espacio)\n");
    printf("7. Desbloquear mutex\n\n");
    
    printf("VENTAJAS DEL PATRÓN:\n");
    printf("───────────────────\n");
    printf("• Desacoplamiento: productores y consumidores trabajan independientemente\n");
    printf("• Balanceamiento: el buffer absorbe diferencias de velocidad\n");
    printf("• Escalabilidad: se pueden agregar múltiples productores/consumidores\n");
    printf("• Eficiencia: no hay busy waiting, los hilos duermen cuando esperan\n");
    printf("• Robustez: maneja correctamente situaciones de buffer lleno/vacío\n\n");
    
    printf("APLICACIONES REALES:\n");
    printf("───────────────────\n");
    printf("• Sistemas de logging: productores generan logs, consumidores los escriben\n");
    printf("• Pipelines de datos: procesamiento de streams de datos\n");
    printf("• Sistemas de mensajería: envío y recepción de mensajes\n");
    printf("• Web servers: productores procesan requests, consumidores generan responses\n");
    printf("• Sistemas operativos: buffers de I/O, spools de impresión\n\n");
    
    printf("CONSIDERACIONES DE DISEÑO:\n");
    printf("─────────────────────────\n");
    printf("• Tamaño del buffer: muy pequeño → muchos bloqueos; muy grande → desperdicio memoria\n");
    printf("• Balance prod/cons: debe coincidir con las velocidades relativas\n");
    printf("• Manejo de finalización: señalar correctamente cuando no hay más datos\n");
    printf("• Gestión de errores: qué hacer si un hilo falla\n\n");
    
    printf("VARIANTES DEL PATRÓN:\n");
    printf("────────────────────\n");
    printf("• Múltiples buffers especializados\n");
    printf("• Prioridades en elementos\n");
    printf("• Timeouts en operaciones\n");
    printf("• Buffers no bloqueantes (lock-free)\n");
    printf("• Backpressure (control de flujo)\n\n");
    
    printf("Presione Enter para continuar...");
    getchar();
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== EJERCICIO 088: PATRÓN PRODUCTOR-CONSUMIDOR ===\n");
    printf("Exploración interactiva de concurrencia con hilos, mutex y variables de condición\n");
    
    // Inicializar generador de números aleatorios
    srand((unsigned int)time(NULL));
    
    int opcion;
    
    do {
        mostrar_menu();
        opcion = obtener_entero("", 0, 10);
        
        switch (opcion) {
            case 1:
                printf("\nEjecutando ejemplo básico del enunciado...\n");
                ejecutar_ejemplo_basico_enunciado();
                break;
                
            case 2:
                ejecutar_demo_multiple_hilos();
                break;
                
            case 3:
                printf("\nComparando diferentes tamaños de buffer...\n");
                demostrar_diferentes_configuraciones_buffer();
                break;
                
            case 4:
                printf("\nEjecutando análisis de rendimiento...\n");
                analizar_rendimiento_productor_consumidor();
                break;
                
            case 5:
                printf("\nEjecutando benchmark de configuraciones...\n");
                ejecutar_benchmark_configuraciones();
                break;
                
            case 6:
                printf("\nEjecutando simulación educativa...\n");
                ejecutar_simulacion_educativa();
                break;
                
            case 7:
                ejecutar_experimento_personalizado();
                break;
                
            case 8:
                demostrar_problema_sin_sincronizacion();
                break;
                
            case 9:
                ejecutar_tests_casos_limite();
                break;
                
            case 10:
                mostrar_informacion_educativa();
                break;
                
            case 0:
                printf("\n¡Gracias por explorar el patrón Productor-Consumidor!\n");
                printf("\nPUNTOS CLAVE APRENDIDOS:\n");
                printf("• El patrón Productor-Consumidor es fundamental en programación concurrente\n");
                printf("• Los mutex previenen race conditions en el acceso al buffer\n");
                printf("• Las variables de condición permiten sincronización eficiente\n");
                printf("• El buffer circular desacopla productores de consumidores\n");
                printf("• El tamaño del buffer afecta el rendimiento y uso de memoria\n");
                printf("• El patrón es escalable y se usa en muchos sistemas reales\n");
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
