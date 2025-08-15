/**
 * @file http_benchmark.c
 * @brief Herramienta de benchmark para el cliente HTTP
 */

#include "../include/cliente_http.h"
#include <signal.h>
#include <sys/time.h>

static volatile bool benchmark_activo = true;

/**
 * @brief Manejador de señales para interrumpir el benchmark
 */
void signal_handler(int sig) {
    (void)sig;
    benchmark_activo = false;
    printf("\n\n⚠️  Benchmark interrumpido por el usuario\n");
}

/**
 * @brief Estructura para estadísticas del benchmark
 */
typedef struct {
    int total_peticiones;
    int peticiones_exitosas;
    int peticiones_fallidas;
    double tiempo_total;
    double tiempo_min;
    double tiempo_max;
    double tiempo_promedio;
    size_t bytes_totales;
    int codigos_estado[6]; // 1xx, 2xx, 3xx, 4xx, 5xx, otros
} stats_benchmark_t;

/**
 * @brief Actualiza estadísticas con una nueva medición
 */
void actualizar_stats(stats_benchmark_t *stats, double tiempo, int codigo_estado, size_t bytes) {
    stats->total_peticiones++;
    
    if (codigo_estado > 0) {
        stats->peticiones_exitosas++;
        stats->bytes_totales += bytes;
        
        // Actualizar tiempos
        if (stats->peticiones_exitosas == 1) {
            stats->tiempo_min = tiempo;
            stats->tiempo_max = tiempo;
        } else {
            if (tiempo < stats->tiempo_min) stats->tiempo_min = tiempo;
            if (tiempo > stats->tiempo_max) stats->tiempo_max = tiempo;
        }
        
        // Actualizar promedio
        stats->tiempo_promedio = ((stats->tiempo_promedio * (stats->peticiones_exitosas - 1)) + tiempo) / stats->peticiones_exitosas;
        
        // Contar códigos de estado
        int indice = (codigo_estado / 100) - 1;
        if (indice >= 0 && indice < 5) {
            stats->codigos_estado[indice]++;
        } else {
            stats->codigos_estado[5]++;
        }
    } else {
        stats->peticiones_fallidas++;
    }
}

/**
 * @brief Imprime estadísticas del benchmark
 */
void imprimir_stats_benchmark(const stats_benchmark_t *stats, double tiempo_total) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    RESULTADOS DEL BENCHMARK                 ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("📊 ESTADÍSTICAS GENERALES:\n");
    printf("   Total de peticiones:     %d\n", stats->total_peticiones);
    printf("   Peticiones exitosas:     %d (%.1f%%)\n", 
           stats->peticiones_exitosas, 
           (float)stats->peticiones_exitosas / stats->total_peticiones * 100);
    printf("   Peticiones fallidas:     %d (%.1f%%)\n", 
           stats->peticiones_fallidas,
           (float)stats->peticiones_fallidas / stats->total_peticiones * 100);
    printf("   Tiempo total:            %.3f segundos\n", tiempo_total);
    printf("\n");
    
    if (stats->peticiones_exitosas > 0) {
        printf("⚡ RENDIMIENTO:\n");
        printf("   Peticiones por segundo:  %.2f req/s\n", stats->total_peticiones / tiempo_total);
        printf("   Tiempo de respuesta:\n");
        printf("     • Mínimo:              %.3f ms\n", stats->tiempo_min * 1000);
        printf("     • Máximo:              %.3f ms\n", stats->tiempo_max * 1000);
        printf("     • Promedio:            %.3f ms\n", stats->tiempo_promedio * 1000);
        printf("\n");
        
        char bytes_str[32];
        cliente_http_formatear_bytes(stats->bytes_totales, bytes_str, sizeof(bytes_str));
        printf("📊 TRANSFERENCIA:\n");
        printf("   Bytes transferidos:      %s\n", bytes_str);
        printf("   Throughput:              %.2f KB/s\n", 
               (stats->bytes_totales / 1024.0) / tiempo_total);
        printf("\n");
        
        printf("📋 CÓDIGOS DE ESTADO:\n");
        const char* nombres_codigos[] = {"1xx", "2xx", "3xx", "4xx", "5xx", "Otros"};
        for (int i = 0; i < 6; i++) {
            if (stats->codigos_estado[i] > 0) {
                printf("   %s:                      %d\n", nombres_codigos[i], stats->codigos_estado[i]);
            }
        }
    }
    printf("\n");
}

/**
 * @brief Ejecuta benchmark secuencial
 */
int ejecutar_benchmark_secuencial(const char *url, int num_peticiones) {
    stats_benchmark_t stats = {0};
    double tiempo_inicio = cliente_http_timestamp();
    
    printf("Ejecutando %d peticiones secuenciales...\n", num_peticiones);
    printf("Presiona Ctrl+C para interrumpir\n\n");
    
    for (int i = 0; i < num_peticiones && benchmark_activo; i++) {
        respuesta_http_t respuesta;
        double tiempo_peticion_inicio = cliente_http_timestamp();
        
        int resultado = cliente_http_get_simple(url, &respuesta);
        
        double tiempo_peticion = cliente_http_timestamp() - tiempo_peticion_inicio;
        
        if (resultado == CLIENTE_HTTP_OK) {
            actualizar_stats(&stats, tiempo_peticion, respuesta.codigo_estado, respuesta.tamaño_contenido);
            cliente_http_liberar_respuesta(&respuesta);
        } else {
            actualizar_stats(&stats, 0, 0, 0);
        }
        
        // Mostrar progreso cada 10 peticiones
        if ((i + 1) % 10 == 0 || i == num_peticiones - 1) {
            printf("\rProgreso: [%d/%d] %.1f%% - Exitosas: %d, Fallidas: %d", 
                   i + 1, num_peticiones, 
                   (float)(i + 1) / num_peticiones * 100,
                   stats.peticiones_exitosas, stats.peticiones_fallidas);
            fflush(stdout);
        }
    }
    
    double tiempo_total = cliente_http_timestamp() - tiempo_inicio;
    imprimir_stats_benchmark(&stats, tiempo_total);
    
    return 0;
}

/**
 * @brief Muestra ayuda del programa
 */
void mostrar_ayuda_benchmark(const char *nombre_programa) {
    printf("Uso: %s <URL> [opciones]\n\n", nombre_programa);
    printf("Opciones:\n");
    printf("  -n <número>     Número de peticiones (por defecto: 10)\n");
    printf("  -c <número>     Concurrencia - NO IMPLEMENTADO (futuro)\n");
    printf("  -t <segundos>   Timeout por petición (por defecto: 30)\n");
    printf("  -v              Modo verbose\n");
    printf("  -h              Mostrar esta ayuda\n\n");
    printf("Ejemplos:\n");
    printf("  %s http://example.com\n", nombre_programa);
    printf("  %s http://httpbin.org/get -n 50\n", nombre_programa);
    printf("  %s http://example.com -n 100 -t 10 -v\n", nombre_programa);
    printf("\n");
}

int main(int argc, char *argv[]) {
    const char *url = NULL;
    int num_peticiones = 10;
    int timeout = 30;
    bool verbose = false;
    
    // Configurar manejador de señales
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Parsear argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            mostrar_ayuda_benchmark(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            num_peticiones = atoi(argv[++i]);
            if (num_peticiones <= 0) {
                printf("Error: Número de peticiones debe ser positivo\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeout = atoi(argv[++i]);
            if (timeout <= 0) {
                printf("Error: Timeout debe ser positivo\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (argv[i][0] != '-') {
            url = argv[i];
        }
    }
    
    if (!url) {
        printf("Error: Debes especificar una URL\n\n");
        mostrar_ayuda_benchmark(argv[0]);
        return 1;
    }
    
    // Validar parámetros
    if (num_peticiones > 1000) {
        printf("⚠️  Advertencia: %d peticiones es mucho. ¿Continuar? (s/N): ", num_peticiones);
        char respuesta;
        scanf("%c", &respuesta);
        if (respuesta != 's' && respuesta != 'S') {
            printf("Benchmark cancelado\n");
            return 0;
        }
    }
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    BENCHMARK HTTP CLIENT                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("🎯 CONFIGURACIÓN:\n");
    printf("   URL:                     %s\n", url);
    printf("   Peticiones:              %d\n", num_peticiones);
    printf("   Timeout:                 %d segundos\n", timeout);
    printf("   Modo verbose:            %s\n", verbose ? "Sí" : "No");
    printf("\n");
    
    // Validar URL
    if (!cliente_http_validar_url(url)) {
        printf("❌ Error: URL inválida\n");
        return 1;
    }
    
    // Ejecutar benchmark
    printf("🚀 Iniciando benchmark...\n\n");
    
    int resultado = ejecutar_benchmark_secuencial(url, num_peticiones);
    
    if (resultado == 0) {
        printf("✅ Benchmark completado exitosamente\n");
    } else {
        printf("❌ Benchmark completado con errores\n");
    }
    
    return resultado;
}
