/**
 * @file hilos_basicos.c
 * @brief Implementación de funciones para demostrar hilos POSIX básicos
 * @author Ejercicios C
 * @date 2025
 */

#include "hilos_basicos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

/* ====================================================================
 * FUNCIONES BÁSICAS DE HILOS
 * ==================================================================== */

void* funcion_hilo_basico(void* arg) {
    (void)arg; // Suprimir warning de parámetro no usado
    
    printf("🧵 Hola desde el hilo hijo (ID: %ld)\n", obtener_id_hilo_actual());
    printf("🧵 El hilo está ejecutándose correctamente\n");
    printf("🧵 Hilo hijo terminando...\n");
    
    return NULL;
}

void* funcion_hilo_con_parametros(void* arg) {
    hilo_params_t* params = (hilo_params_t*)arg;
    
    if (params == NULL) {
        printf("❌ Error: parámetros nulos en hilo\n");
        return NULL;
    }
    
    // Crear estructura de resultado
    hilo_resultado_t* resultado = malloc(sizeof(hilo_resultado_t));
    if (resultado == NULL) {
        printf("❌ Error: no se pudo asignar memoria para resultado\n");
        return NULL;
    }
    
    long tiempo_inicio = obtener_tiempo_ms();
    
    printf("🧵 Hilo %d iniciado: %s\n", params->id, params->mensaje);
    
    // Realizar iteraciones con delay
    for (int i = 0; i < params->iteraciones; i++) {
        printf("🧵 Hilo %d - iteración %d/%d\n", 
               params->id, i + 1, params->iteraciones);
        
        if (params->delay_ms > 0) {
            usleep(params->delay_ms * 1000); // usleep usa microsegundos
        }
    }
    
    long tiempo_fin = obtener_tiempo_ms();
    
    // Llenar resultado
    resultado->hilo_id = params->id;
    resultado->operaciones_realizadas = params->iteraciones;
    resultado->tiempo_ejecucion_ms = tiempo_fin - tiempo_inicio;
    resultado->exito = true;
    
    printf("🧵 Hilo %d completado en %ld ms\n", 
           params->id, resultado->tiempo_ejecucion_ms);
    
    return resultado;
}

void* funcion_hilo_calculo(void* arg) {
    int* limite_ptr = (int*)arg;
    if (limite_ptr == NULL) {
        return NULL;
    }
    
    int limite = *limite_ptr;
    long suma = 0;
    
    printf("🧵 Hilo calculando suma de 1 a %d\n", limite);
    
    for (int i = 1; i <= limite; i++) {
        suma += i;
        
        // Simular trabajo más intensivo cada 1000 iteraciones
        if (i % 1000 == 0) {
            printf("🧵 Progreso: %d/%d (suma parcial: %ld)\n", i, limite, suma);
        }
    }
    
    // Retornar resultado en memoria dinámica
    long* resultado = malloc(sizeof(long));
    if (resultado != NULL) {
        *resultado = suma;
        printf("🧵 Cálculo completado. Suma total: %ld\n", suma);
    }
    
    return resultado;
}

void* funcion_hilo_trabajo(void* arg) {
    hilo_params_t* params = (hilo_params_t*)arg;
    
    if (params == NULL) {
        return NULL;
    }
    
    hilo_resultado_t* resultado = malloc(sizeof(hilo_resultado_t));
    if (resultado == NULL) {
        return NULL;
    }
    
    long tiempo_inicio = obtener_tiempo_ms();
    
    printf("🧵 Trabajador %d iniciando: %s\n", params->id, params->mensaje);
    
    // Simular trabajo variable
    for (int i = 0; i < params->iteraciones; i++) {
        printf("🧵 Trabajador %d procesando tarea %d\n", params->id, i + 1);
        
        // Simular trabajo con tiempo variable
        int tiempo_trabajo = (i % 3 + 1) * params->delay_ms;
        usleep(tiempo_trabajo * 1000);
        
        printf("🧵 Trabajador %d completó tarea %d (tardó %d ms)\n", 
               params->id, i + 1, tiempo_trabajo);
    }
    
    long tiempo_fin = obtener_tiempo_ms();
    
    resultado->hilo_id = params->id;
    resultado->operaciones_realizadas = params->iteraciones;
    resultado->tiempo_ejecucion_ms = tiempo_fin - tiempo_inicio;
    resultado->exito = true;
    
    printf("🧵 Trabajador %d terminado (tiempo total: %ld ms)\n", 
           params->id, resultado->tiempo_ejecucion_ms);
    
    return resultado;
}

/* ====================================================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ==================================================================== */

int demostrar_hilo_basico(void) {
    printf("\n=== DEMOSTRACIÓN: HILO BÁSICO ===\n");
    
    pthread_t hilo;
    int resultado_creacion, resultado_join;
    
    printf("📋 Hilo principal (ID: %ld) creando hilo hijo...\n", 
           obtener_id_hilo_actual());
    
    // Crear hilo
    resultado_creacion = pthread_create(&hilo, NULL, funcion_hilo_basico, NULL);
    if (resultado_creacion != 0) {
        printf("❌ Error al crear hilo: %s\n", strerror(resultado_creacion));
        return resultado_creacion;
    }
    
    printf("✅ Hilo creado exitosamente\n");
    printf("📋 Hilo principal esperando a que termine el hilo hijo...\n");
    
    // Esperar a que termine el hilo
    resultado_join = pthread_join(hilo, NULL);
    if (resultado_join != 0) {
        printf("❌ Error al esperar hilo: %s\n", strerror(resultado_join));
        return resultado_join;
    }
    
    printf("✅ Hilo hijo terminado exitosamente\n");
    printf("📋 Hilo principal continuando...\n");
    
    return 0;
}

int demostrar_hilos_con_parametros(void) {
    printf("\n=== DEMOSTRACIÓN: HILOS CON PARÁMETROS ===\n");
    
    const int num_hilos = 3;
    pthread_t hilos[num_hilos];
    hilo_params_t params[num_hilos];
    hilo_resultado_t* resultados[num_hilos];
    
    // Configurar parámetros para cada hilo
    params[0] = crear_parametros_hilo(1, "Procesando datos de usuario");
    params[0].iteraciones = 3;
    params[0].delay_ms = 500;
    
    params[1] = crear_parametros_hilo(2, "Calculando estadísticas");
    params[1].iteraciones = 5;
    params[1].delay_ms = 300;
    
    params[2] = crear_parametros_hilo(3, "Generando reportes");
    params[2].iteraciones = 4;
    params[2].delay_ms = 400;
    
    // Crear hilos
    printf("📋 Creando %d hilos con parámetros...\n", num_hilos);
    for (int i = 0; i < num_hilos; i++) {
        int resultado = pthread_create(&hilos[i], NULL, 
                                     funcion_hilo_con_parametros, &params[i]);
        if (resultado != 0) {
            printf("❌ Error al crear hilo %d: %s\n", i, strerror(resultado));
            return resultado;
        }
        printf("✅ Hilo %d creado\n", i + 1);
    }
    
    // Esperar y recoger resultados
    printf("📋 Esperando resultados...\n");
    for (int i = 0; i < num_hilos; i++) {
        void* resultado_ptr;
        int resultado_join = pthread_join(hilos[i], &resultado_ptr);
        
        if (resultado_join != 0) {
            printf("❌ Error al esperar hilo %d: %s\n", i, strerror(resultado_join));
            resultados[i] = NULL;
        } else {
            resultados[i] = (hilo_resultado_t*)resultado_ptr;
        }
    }
    
    // Mostrar estadísticas
    mostrar_estadisticas_hilos(resultados, num_hilos);
    
    // Liberar memoria
    for (int i = 0; i < num_hilos; i++) {
        liberar_resultado_hilo(resultados[i]);
    }
    
    return 0;
}

int demostrar_multiples_hilos(int num_hilos) {
    printf("\n=== DEMOSTRACIÓN: MÚLTIPLES HILOS (%d) ===\n", num_hilos);
    
    if (num_hilos <= 0 || num_hilos > MAX_HILOS) {
        printf("❌ Número de hilos inválido (1-%d permitidos)\n", MAX_HILOS);
        return -1;
    }
    
    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));
    hilo_params_t* params = malloc(num_hilos * sizeof(hilo_params_t));
    
    if (hilos == NULL || params == NULL) {
        printf("❌ Error al asignar memoria\n");
        free(hilos);
        free(params);
        return -1;
    }
    
    long tiempo_inicio = obtener_tiempo_ms();
    
    // Crear hilos
    printf("📋 Lanzando %d hilos trabajadores...\n", num_hilos);
    for (int i = 0; i < num_hilos; i++) {
        char mensaje[BUFFER_SIZE];
        snprintf(mensaje, sizeof(mensaje), "Trabajador #%d procesando", i + 1);
        
        params[i] = crear_parametros_hilo(i + 1, mensaje);
        params[i].iteraciones = 3 + (i % 3); // Varíar entre 3-5 iteraciones
        params[i].delay_ms = 200 + (i * 50);  // Varíar delay
        
        int resultado = pthread_create(&hilos[i], NULL, 
                                     funcion_hilo_trabajo, &params[i]);
        if (resultado != 0) {
            printf("❌ Error al crear hilo %d: %s\n", i + 1, strerror(resultado));
            // Continuar con los otros hilos
        }
    }
    
    // Esperar a todos los hilos
    printf("📋 Esperando a que terminen todos los hilos...\n");
    int hilos_exitosos = 0;
    
    for (int i = 0; i < num_hilos; i++) {
        void* resultado_ptr;
        int resultado_join = pthread_join(hilos[i], &resultado_ptr);
        
        if (resultado_join == 0) {
            hilos_exitosos++;
            if (resultado_ptr != NULL) {
                liberar_resultado_hilo((hilo_resultado_t*)resultado_ptr);
            }
        } else {
            printf("❌ Error al esperar hilo %d: %s\n", i + 1, strerror(resultado_join));
        }
    }
    
    long tiempo_total = obtener_tiempo_ms() - tiempo_inicio;
    
    printf("✅ Ejecución completada:\n");
    printf("   - Hilos lanzados: %d\n", num_hilos);
    printf("   - Hilos exitosos: %d\n", hilos_exitosos);
    printf("   - Tiempo total: %ld ms\n", tiempo_total);
    
    free(hilos);
    free(params);
    
    return 0;
}

int demostrar_calculo_paralelo(void) {
    printf("\n=== DEMOSTRACIÓN: CÁLCULO PARALELO ===\n");
    
    const int num_hilos = 4;
    const int limite_base = 10000;
    
    pthread_t hilos[num_hilos];
    int limites[num_hilos];
    long* resultados[num_hilos];
    
    // Configurar límites para cada hilo
    for (int i = 0; i < num_hilos; i++) {
        limites[i] = limite_base * (i + 1);
    }
    
    long tiempo_inicio = obtener_tiempo_ms();
    
    // Crear hilos de cálculo
    printf("📋 Iniciando cálculos paralelos...\n");
    for (int i = 0; i < num_hilos; i++) {
        int resultado = pthread_create(&hilos[i], NULL, 
                                     funcion_hilo_calculo, &limites[i]);
        if (resultado != 0) {
            printf("❌ Error al crear hilo de cálculo %d: %s\n", 
                   i + 1, strerror(resultado));
            return resultado;
        }
        printf("✅ Hilo de cálculo %d iniciado (límite: %d)\n", i + 1, limites[i]);
    }
    
    // Recoger resultados
    printf("📋 Recogiendo resultados...\n");
    for (int i = 0; i < num_hilos; i++) {
        void* resultado_ptr;
        int resultado_join = pthread_join(hilos[i], &resultado_ptr);
        
        if (resultado_join == 0 && resultado_ptr != NULL) {
            resultados[i] = (long*)resultado_ptr;
            printf("✅ Hilo %d: suma(1..%d) = %ld\n", 
                   i + 1, limites[i], *resultados[i]);
        } else {
            printf("❌ Error en hilo %d\n", i + 1);
            resultados[i] = NULL;
        }
    }
    
    long tiempo_total = obtener_tiempo_ms() - tiempo_inicio;
    
    // Calcular suma total
    long suma_total = 0;
    for (int i = 0; i < num_hilos; i++) {
        if (resultados[i] != NULL) {
            suma_total += *resultados[i];
            free(resultados[i]);
        }
    }
    
    printf("📊 RESULTADOS:\n");
    printf("   - Tiempo total de cálculo paralelo: %ld ms\n", tiempo_total);
    printf("   - Suma combinada de todos los cálculos: %ld\n", suma_total);
    
    return 0;
}

/* ====================================================================
 * FUNCIONES DE SINCRONIZACIÓN
 * ==================================================================== */

void* incrementar_sin_mutex(void* arg) {
    contador_compartido_t* contador = (contador_compartido_t*)arg;
    
    for (int i = 0; i < contador->incrementos_por_hilo; i++) {
        // RACE CONDITION: acceso sin sincronización
        int valor_actual = contador->contador_compartido;
        usleep(1); // Simular algo de trabajo
        contador->contador_compartido = valor_actual + 1;
    }
    
    return NULL;
}

void* incrementar_con_mutex(void* arg) {
    contador_compartido_t* contador = (contador_compartido_t*)arg;
    
    for (int i = 0; i < contador->incrementos_por_hilo; i++) {
        // Sección crítica protegida por mutex
        pthread_mutex_lock(&contador->mutex);
        
        int valor_actual = contador->contador_compartido;
        usleep(1); // Simular algo de trabajo
        contador->contador_compartido = valor_actual + 1;
        
        pthread_mutex_unlock(&contador->mutex);
    }
    
    return NULL;
}

/* ====================================================================
 * UTILIDADES Y HELPERS
 * ==================================================================== */

long obtener_id_hilo_actual(void) {
    return (long)pthread_self();
}

void simular_trabajo(int segundos, const char* mensaje) {
    if (mensaje != NULL) {
        printf("💼 %s (duración: %d segundos)\n", mensaje, segundos);
    }
    sleep(segundos);
}

long obtener_tiempo_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

hilo_params_t crear_parametros_hilo(int id, const char* mensaje) {
    hilo_params_t params;
    params.id = id;
    params.iteraciones = 1;
    params.delay_ms = TIEMPO_ESPERA_DEFAULT * 1000;
    
    if (mensaje != NULL) {
        strncpy(params.mensaje, mensaje, BUFFER_SIZE - 1);
        params.mensaje[BUFFER_SIZE - 1] = '\0';
    } else {
        snprintf(params.mensaje, BUFFER_SIZE, "Hilo %d", id);
    }
    
    return params;
}

void liberar_resultado_hilo(hilo_resultado_t* resultado) {
    if (resultado != NULL) {
        free(resultado);
    }
}

void mostrar_info_hilos(void) {
    printf("\n=== INFORMACIÓN DEL SISTEMA DE HILOS ===\n");
    printf("📋 Biblioteca: POSIX Threads (pthread)\n");
    printf("📋 Soporte pthread: %s\n", 
           verificar_soporte_pthread() ? "✅ Disponible" : "❌ No disponible");
    printf("📋 ID del hilo principal: %ld\n", obtener_id_hilo_actual());
    printf("📋 Máximo de hilos configurado: %d\n", MAX_HILOS);
}

void mostrar_estadisticas_hilos(hilo_resultado_t* resultados[], int num_resultados) {
    printf("\n📊 ESTADÍSTICAS DE EJECUCIÓN:\n");
    
    int hilos_exitosos = 0;
    long tiempo_total = 0;
    int operaciones_totales = 0;
    
    for (int i = 0; i < num_resultados; i++) {
        if (resultados[i] != NULL && resultados[i]->exito) {
            hilos_exitosos++;
            tiempo_total += resultados[i]->tiempo_ejecucion_ms;
            operaciones_totales += resultados[i]->operaciones_realizadas;
            
            printf("   Hilo %d: %d ops en %ld ms\n",
                   resultados[i]->hilo_id,
                   resultados[i]->operaciones_realizadas,
                   resultados[i]->tiempo_ejecucion_ms);
        }
    }
    
    if (hilos_exitosos > 0) {
        printf("   ────────────────────────────\n");
        printf("   Total: %d hilos exitosos\n", hilos_exitosos);
        printf("   Operaciones totales: %d\n", operaciones_totales);
        printf("   Tiempo promedio: %.2f ms\n", (double)tiempo_total / hilos_exitosos);
    }
}

bool verificar_soporte_pthread(void) {
    // Intentar crear un mutex como test básico
    pthread_mutex_t test_mutex;
    int resultado = pthread_mutex_init(&test_mutex, NULL);
    
    if (resultado == 0) {
        pthread_mutex_destroy(&test_mutex);
        return true;
    }
    
    return false;
}
