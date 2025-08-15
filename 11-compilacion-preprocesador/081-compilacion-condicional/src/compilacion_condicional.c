#include "compilacion_condicional.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Variables globales para estadísticas (si están habilitadas)
#ifdef FEATURE_STATS
int operaciones_realizadas = 0;
int memoria_total_asignada = 0;
#endif

// ============================================================================
// FUNCIÓN PRINCIPAL DEL ENUNCIADO
// ============================================================================

void procesar_datos(int x) {
    #ifdef DEBUG
        printf("[DEBUG] Entrando en procesarDatos con x = %d\n", x);
    #endif

    int resultado = x * 2 + 3;

    #ifdef DEBUG
        printf("[DEBUG] Resultado intermedio = %d\n", resultado);
    #endif

    printf("Resultado final: %d\n", resultado);
}

int main_usuario_basico(void) {
    printf("=== EJEMPLO BÁSICO DEL ENUNCIADO ===\n\n");
    
    int dato = 5;

    #ifdef DEBUG
        printf("[DEBUG] Iniciando programa\n");
    #endif

    procesar_datos(dato);

    #ifdef DEBUG
        printf("[DEBUG] Finalizando programa\n");
    #endif

    return 0;
}

// ============================================================================
// DEMOSTRACIONES EDUCATIVAS
// ============================================================================

void demo_niveles_debug(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    NIVELES DE DEBUG                             ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Demostrando diferentes niveles de información de debug:\n\n");

    // Debug básico
    DEBUG_PRINT("Este es un mensaje de debug básico");
    DEBUG_PRINTF("Variable de prueba: %d", 42);
    
    int variable_ejemplo = 100;
    DEBUG_VAR(variable_ejemplo);
    
    float valor_float = 3.14159f;
    DEBUG_FLOAT(valor_float);
    
    char* cadena_ejemplo = "Hola mundo";
    DEBUG_STRING(cadena_ejemplo);

    // Debug verbose
    VERBOSE_PRINT("Información detallada de debug");
    VERBOSE_PRINTF("Procesando elemento %d de %d", 3, 10);

    // Trazado de funciones
    TRACE_ENTER();
    printf("Ejecutando código de demostración...\n");
    TRACE_POINT();
    printf("Punto intermedio alcanzado\n");
    TRACE_EXIT();

    printf("\n");
}

void demo_caracteristicas_opcionales(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                CARACTERÍSTICAS OPCIONALES                       ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    // Logging
    LOG_INFO("Sistema inicializado correctamente");
    LOG_WARNING("Advertencia: valor fuera del rango recomendado");
    LOG_ERROR("Error simulado para demostración");

    // Estadísticas
    STATS_INC_OPS();
    STATS_ADD_MEM(1024);
    STATS_INC_OPS();
    STATS_ADD_MEM(512);
    STATS_PRINT();

    // Profiling
    PROFILE_FUNC();

    printf("\n");
}

void demo_plataforma(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║              INFORMACIÓN DE PLATAFORMA                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Plataforma detectada: %s\n", PLATFORM_NAME);
    printf("Separador de rutas: %s\n", PATH_SEPARATOR);
    printf("Compilador: %s (versión %d)\n", COMPILER_NAME, COMPILER_VERSION);

    // Código específico de plataforma
    #ifdef __linux__
        printf("Características específicas de Linux habilitadas\n");
    #elif defined(_WIN32) || defined(_WIN64)
        printf("Características específicas de Windows habilitadas\n");
    #elif defined(__APPLE__)
        printf("Características específicas de macOS habilitadas\n");
    #endif

    printf("\n");
}

void demo_compilador(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║               INFORMACIÓN DEL COMPILADOR                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    MOSTRAR_INFO_COMPILACION();
    printf("\n");
    MOSTRAR_CONFIGURACION_DEBUG();

    printf("\nCaracterísticas habilitadas:\n");
    #ifdef FEATURE_LOGGING
        printf("  ✓ Logging habilitado\n");
    #else
        printf("  ✗ Logging deshabilitado\n");
    #endif

    #ifdef FEATURE_STATS
        printf("  ✓ Estadísticas habilitadas\n");
    #else
        printf("  ✗ Estadísticas deshabilitadas\n");
    #endif

    #ifdef FEATURE_PROFILING
        printf("  ✓ Profiling habilitado\n");
    #else
        printf("  ✗ Profiling deshabilitado\n");
    #endif

    printf("\n");
}

void demo_aserciones(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    ASERCIONES CONDICIONALES                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Probando aserciones (solo activas en modo DEBUG):\n");

    // Aserción que pasa
    int valor_valido = 10;
    ASSERT(valor_valido > 0);
    printf("✓ Aserción 1 pasó: valor_valido > 0\n");

    // Aserción con puntero
    char* puntero_valido = "test";
    ASSERT_NOT_NULL(puntero_valido);
    printf("✓ Aserción 2 pasó: puntero no es NULL\n");

    // Aserción matemática
    int a = 5, b = 3;
    ASSERT(a > b);
    printf("✓ Aserción 3 pasó: a > b\n");

    printf("\nNota: Las aserciones solo se ejecutan si DEBUG está definido.\n");
    printf("Si DEBUG no está definido, estas verificaciones se omiten completamente.\n\n");
}

// ============================================================================
// FUNCIONES CON INSTRUMENTACIÓN AVANZADA
// ============================================================================

int operacion_que_puede_fallar(int entrada) {
    DEBUG_FUNC_ENTER("operacion_que_puede_fallar");
    
    VERBOSE_PRINTF("Procesando entrada: %d", entrada);
    
    if (entrada < 0) {
        ERROR_PRINT("Entrada negativa no permitida");
        DEBUG_FUNC_EXIT("operacion_que_puede_fallar");
        return -1;
    }
    
    if (entrada > 1000) {
        ERROR_PRINT("Entrada demasiado grande");
        DEBUG_FUNC_EXIT("operacion_que_puede_fallar");
        return -2;
    }
    
    int resultado = entrada * entrada + entrada;
    VERBOSE_PRINTF("Resultado calculado: %d", resultado);
    
    STATS_INC_OPS();
    
    DEBUG_FUNC_EXIT("operacion_que_puede_fallar");
    return resultado;
}

void funcion_con_logging(const char* mensaje) {
    PROFILE_FUNC();
    
    if (mensaje == NULL) {
        ERROR_PRINT("Mensaje es NULL");
        return;
    }
    
    LOG_INFO("Procesando mensaje");
    VERBOSE_PRINTF("Longitud del mensaje: %zu", strlen(mensaje));
    
    printf("Mensaje procesado: %s\n", mensaje);
    
    STATS_INC_OPS();
}

void funcion_con_timing(int iteraciones) {
    TIME_START(procesamiento);
    
    VERBOSE_PRINTF("Iniciando %d iteraciones", iteraciones);
    
    int suma = 0;
    for (int i = 0; i < iteraciones; i++) {
        suma += i * i;
        
        // Mostrar progreso cada 1000 iteraciones
        if (i % 1000 == 0 && i > 0) {
            VERBOSE_PRINTF("Progreso: %d/%d iteraciones", i, iteraciones);
        }
    }
    
    DEBUG_PRINTF("Suma total calculada: %d", suma);
    
    TIME_END(procesamiento);
    
    STATS_INC_OPS();
}

void* asignar_memoria_instrumentada(size_t size) {
    void* ptr = malloc(size);
    
    if (ptr == NULL) {
        ERROR_PRINT("Error al asignar memoria");
        ERROR_ERRNO();
        return NULL;
    }
    
    MEM_PRINT(ptr, size);
    STATS_ADD_MEM(size);
    
    return ptr;
}

void liberar_memoria_instrumentada(void* ptr) {
    if (ptr == NULL) {
        ERROR_PRINT("Intento de liberar puntero NULL");
        return;
    }
    
    MEM_FREE_PRINT(ptr);
    free(ptr);
}

void funcion_compleja_con_debug(int param1, const char* param2) {
    DEBUG_FUNC_ENTER("funcion_compleja_con_debug");
    
    DEBUG_VAR(param1);
    DEBUG_STRING(param2);
    
    ASSERT(param1 >= 0);
    ASSERT_NOT_NULL(param2);
    
    // Simular trabajo complejo
    VERBOSE_PRINT("Iniciando procesamiento complejo");
    
    TIME_START(trabajo_complejo);
    
    // Asignar memoria temporalmente
    size_t buffer_size = param1 * 10;
    char* buffer = (char*)asignar_memoria_instrumentada(buffer_size);
    
    if (buffer != NULL) {
        snprintf(buffer, buffer_size, "Procesado: %s (param1=%d)", param2, param1);
        VERBOSE_STRING(buffer);
        
        // Simular algún trabajo
        for (int i = 0; i < param1 * 100; i++) {
            buffer[i % (buffer_size - 1)] = 'X';
        }
        
        liberar_memoria_instrumentada(buffer);
    }
    
    TIME_END(trabajo_complejo);
    
    STATS_INC_OPS();
    
    DEBUG_FUNC_EXIT("funcion_compleja_con_debug");
}

int procesar_archivo_simulado(const char* nombre_archivo) {
    DEBUG_FUNC_ENTER("procesar_archivo_simulado");
    
    ASSERT_NOT_NULL(nombre_archivo);
    
    LOG_INFO("Iniciando procesamiento de archivo");
    DEBUG_STRING(nombre_archivo);
    
    // Simular apertura de archivo
    VERBOSE_PRINT("Abriendo archivo");
    
    if (strlen(nombre_archivo) == 0) {
        ERROR_PRINT("Nombre de archivo vacío");
        LOG_ERROR("Nombre de archivo inválido");
        DEBUG_FUNC_EXIT("procesar_archivo_simulado");
        return -1;
    }
    
    // Simular lectura de datos
    TIME_START(lectura_archivo);
    
    int lineas_procesadas = 0;
    int bytes_leidos = 0;
    
    for (int i = 0; i < 100; i++) {  // Simular 100 líneas
        lineas_procesadas++;
        bytes_leidos += 50;  // Simular 50 bytes por línea
        
        if (i % 20 == 0) {
            VERBOSE_PRINTF("Procesadas %d líneas, %d bytes", lineas_procesadas, bytes_leidos);
        }
    }
    
    TIME_END(lectura_archivo);
    
    DEBUG_PRINTF("Archivo procesado: %d líneas, %d bytes", lineas_procesadas, bytes_leidos);
    LOG_INFO("Procesamiento de archivo completado");
    
    STATS_INC_OPS();
    
    DEBUG_FUNC_EXIT("procesar_archivo_simulado");
    return lineas_procesadas;
}

// ============================================================================
// FUNCIONES DE INFORMACIÓN Y ESTADÍSTICAS
// ============================================================================

void mostrar_info_configuracion(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                 CONFIGURACIÓN DE COMPILACIÓN                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Configuración actual:\n");
    printf("  Modo de compilación: %s\n", MODO_COMPILACION);
    printf("  Optimizaciones: %s\n", OPTIMIZACIONES_HABILITADAS ? "Habilitadas" : "Deshabilitadas");
    printf("  Plataforma: %s\n", PLATFORM_NAME);
    printf("  Compilador: %s %d\n", COMPILER_NAME, COMPILER_VERSION);
    printf("  Versión del programa: %d.%d.%d\n", VERSION_MAYOR, VERSION_MENOR, VERSION_PATCH);

    printf("\nDirectivas de debug definidas:\n");
    
    #ifdef DEBUG
        printf("  ✓ DEBUG\n");
    #else
        printf("  ✗ DEBUG\n");
    #endif
    
    #ifdef DEBUG_VERBOSE
        printf("  ✓ DEBUG_VERBOSE\n");
    #else
        printf("  ✗ DEBUG_VERBOSE\n");
    #endif
    
    #ifdef DEBUG_TRACE
        printf("  ✓ DEBUG_TRACE\n");
    #else
        printf("  ✗ DEBUG_TRACE\n");
    #endif
    
    #ifdef DEBUG_MEMORY
        printf("  ✓ DEBUG_MEMORY\n");
    #else
        printf("  ✗ DEBUG_MEMORY\n");
    #endif
    
    #ifdef DEBUG_TIMING
        printf("  ✓ DEBUG_TIMING\n");
    #else
        printf("  ✗ DEBUG_TIMING\n");
    #endif

    printf("\nCaracterísticas opcionales:\n");
    
    #ifdef FEATURE_LOGGING
        printf("  ✓ FEATURE_LOGGING\n");
    #else
        printf("  ✗ FEATURE_LOGGING\n");
    #endif
    
    #ifdef FEATURE_STATS
        printf("  ✓ FEATURE_STATS\n");
    #else
        printf("  ✗ FEATURE_STATS\n");
    #endif
    
    #ifdef FEATURE_PROFILING
        printf("  ✓ FEATURE_PROFILING\n");
    #else
        printf("  ✗ FEATURE_PROFILING\n");
    #endif

    printf("\n");
}

void mostrar_estadisticas(void) {
    #ifdef FEATURE_STATS
        printf("╔══════════════════════════════════════════════════════════════════╗\n");
        printf("║                         ESTADÍSTICAS                            ║\n");
        printf("╚══════════════════════════════════════════════════════════════════╝\n\n");
        
        printf("Estadísticas de ejecución:\n");
        printf("  Operaciones realizadas: %d\n", operaciones_realizadas);
        printf("  Memoria total asignada: %d bytes\n", memoria_total_asignada);
        printf("  Memoria promedio por operación: %.2f bytes\n", 
               operaciones_realizadas > 0 ? (float)memoria_total_asignada / operaciones_realizadas : 0.0f);
    #else
        printf("Estadísticas no están habilitadas (FEATURE_STATS no definido)\n");
    #endif
    
    printf("\n");
}

void ejecutar_todas_demos(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║           COMPILACIÓN CONDICIONAL - DEMOSTRACIÓN COMPLETA       ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    // Mostrar configuración actual
    mostrar_info_configuracion();

    // Demostrar diferentes aspectos
    demo_niveles_debug();
    demo_caracteristicas_opcionales();
    demo_plataforma();
    demo_compilador();
    demo_aserciones();

    // Ejemplos prácticos
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      EJEMPLOS PRÁCTICOS                         ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("1. Operación que puede fallar:\n");
    int resultado1 = operacion_que_puede_fallar(25);
    printf("   Resultado: %d\n\n", resultado1);

    printf("2. Función con logging:\n");
    funcion_con_logging("Mensaje de prueba para logging");
    printf("\n");

    printf("3. Función con medición de tiempo:\n");
    funcion_con_timing(5000);
    printf("\n");

    printf("4. Función compleja con debug:\n");
    funcion_compleja_con_debug(10, "parámetro de prueba");
    printf("\n");

    printf("5. Procesamiento de archivo simulado:\n");
    int lineas = procesar_archivo_simulado("archivo_ejemplo.txt");
    printf("   Líneas procesadas: %d\n\n", lineas);

    // Mostrar estadísticas finales
    mostrar_estadisticas();

    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                           CONCLUSIÓN                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Esta demostración muestra cómo la compilación condicional permite:\n\n");
    printf("✓ Incluir/excluir código de debug según necesidades\n");
    printf("✓ Crear versiones optimizadas para producción\n");
    printf("✓ Adaptar el código a diferentes plataformas\n");
    printf("✓ Habilitar características opcionales\n");
    printf("✓ Instrumentar el código para análisis de rendimiento\n");
    printf("✓ Mantener código limpio sin impacto en producción\n\n");
    
    printf("Para probar diferentes configuraciones, compila con:\n");
    printf("  gcc -DDEBUG ...\n");
    printf("  gcc -DDEBUG -DDEBUG_VERBOSE ...\n");
    printf("  gcc -DFEATURE_LOGGING -DFEATURE_STATS ...\n\n");
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

#ifdef COMPILAR_MAIN_BASICO
int main(void) {
    return main_usuario_basico();
}
#elif defined(COMPILAR_DEMO_COMPLETO)
int main(void) {
    ejecutar_todas_demos();
    return 0;
}
#elif defined(COMPILAR_SIN_DEBUG)
int main(void) {
    printf("=== VERSIÓN SIN DEBUG ===\n\n");
    main_usuario_basico();
    return 0;
}
#elif defined(COMPILAR_CON_DEBUG)
int main(void) {
    printf("=== VERSIÓN CON DEBUG ===\n\n");
    #define DEBUG
    main_usuario_basico();
    return 0;
}
#else
// Por defecto, mostrar la demostración completa
int main(void) {
    ejecutar_todas_demos();
    return 0;
}
#endif
