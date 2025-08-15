/**
 * @file benchmark_servidor.c
 * @brief Herramienta de benchmark para servidor TCP multicliente
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Herramienta especializada para evaluar el rendimiento del servidor
 * TCP multicliente con métricas detalladas y análisis de escalabilidad.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define SERVIDOR_DEFAULT "127.0.0.1"
#define PUERTO_DEFAULT 9090
#define MAX_HILOS 100

// Variables globales
static volatile sig_atomic_t ejecutando = 1;

/**
 * @brief Configuración del benchmark
 */
typedef struct {
    char servidor[64];
    int puerto;
    int num_clientes;
    int num_mensajes_por_cliente;
    int tamaño_mensaje;
    int duracion_segundos;
    int intervalo_conexion_ms;
    int mostrar_progreso;
    int guardar_latencias;
} ConfigBenchmark;

/**
 * @brief Métricas por cliente
 */
typedef struct {
    int id_cliente;
    size_t mensajes_enviados;
    size_t mensajes_recibidos;
    size_t bytes_enviados;
    size_t bytes_recibidos;
    double latencia_total_ms;
    double latencia_min_ms;
    double latencia_max_ms;
    struct timeval tiempo_inicio;
    struct timeval tiempo_fin;
    int errores;
} MetricasCliente;

/**
 * @brief Resultados globales del benchmark
 */
typedef struct {
    int clientes_completados;
    size_t mensajes_totales;
    size_t bytes_totales;
    double tiempo_total_segundos;
    double throughput_mensajes_por_segundo;
    double throughput_bytes_por_segundo;
    double latencia_promedio_ms;
    double latencia_mediana_ms;
    double latencia_p95_ms;
    double latencia_p99_ms;
    int errores_totales;
    pthread_mutex_t mutex;
} ResultadosBenchmark;

// Variables globales para estadísticas
static MetricasCliente* g_metricas_clientes = NULL;
static ResultadosBenchmark g_resultados;
static double* g_latencias = NULL;
static size_t g_num_latencias = 0;

/**
 * @brief Manejador de señales
 */
void manejador_senales(int signum) {
    printf("\nBenchmark interrumpido por señal %d\n", signum);
    ejecutando = 0;
}

/**
 * @brief Obtiene timestamp actual en microsegundos
 */
double obtener_timestamp_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

/**
 * @brief Calcula diferencia de tiempo en milisegundos
 */
double calcular_diferencia_ms(struct timeval* inicio, struct timeval* fin) {
    return (fin->tv_sec - inicio->tv_sec) * 1000.0 + 
           (fin->tv_usec - inicio->tv_usec) / 1000.0;
}

/**
 * @brief Conecta al servidor con timeout
 */
int conectar_servidor_con_timeout(const char* servidor, int puerto, int timeout_seg) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }
    
    // Configurar timeout
    struct timeval timeout;
    timeout.tv_sec = timeout_seg;
    timeout.tv_usec = 0;
    
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    
    // Configurar dirección
    struct sockaddr_in direccion;
    memset(&direccion, 0, sizeof(direccion));
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(puerto);
    
    if (inet_pton(AF_INET, servidor, &direccion.sin_addr) <= 0) {
        close(sockfd);
        return -1;
    }
    
    // Conectar
    if (connect(sockfd, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        close(sockfd);
        return -1;
    }
    
    return sockfd;
}

/**
 * @brief Función del hilo cliente para benchmark
 */
void* cliente_benchmark(void* arg) {
    MetricasCliente* metricas = (MetricasCliente*)arg;
    ConfigBenchmark* config = (ConfigBenchmark*)((char*)arg + sizeof(MetricasCliente));
    
    // Conectar al servidor
    int sockfd = conectar_servidor_con_timeout(config->servidor, config->puerto, 10);
    if (sockfd < 0) {
        metricas->errores++;
        return NULL;
    }
    
    gettimeofday(&metricas->tiempo_inicio, NULL);
    
    char buffer[BUFFER_SIZE];
    char mensaje[BUFFER_SIZE];
    
    // Preparar mensaje de tamaño específico
    memset(mensaje, 'A', config->tamaño_mensaje - 1);
    mensaje[config->tamaño_mensaje - 1] = '\0';
    
    // Recibir mensaje de bienvenida
    recv(sockfd, buffer, sizeof(buffer), 0);
    
    metricas->latencia_min_ms = INFINITY;
    metricas->latencia_max_ms = 0.0;
    
    // Enviar mensajes y medir latencias
    for (int i = 0; i < config->num_mensajes_por_cliente && ejecutando; i++) {
        struct timeval inicio_msg, fin_msg;
        
        // Timestamp antes del envío
        gettimeofday(&inicio_msg, NULL);
        
        // Enviar mensaje
        ssize_t bytes_enviados = send(sockfd, mensaje, strlen(mensaje), 0);
        if (bytes_enviados < 0) {
            metricas->errores++;
            break;
        }
        
        // Recibir respuesta
        ssize_t bytes_recibidos = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_recibidos <= 0) {
            metricas->errores++;
            break;
        }
        
        // Timestamp después de la recepción
        gettimeofday(&fin_msg, NULL);
        
        // Calcular latencia
        double latencia_ms = calcular_diferencia_ms(&inicio_msg, &fin_msg);
        
        // Actualizar métricas
        metricas->mensajes_enviados++;
        metricas->mensajes_recibidos++;
        metricas->bytes_enviados += bytes_enviados;
        metricas->bytes_recibidos += bytes_recibidos;
        metricas->latencia_total_ms += latencia_ms;
        
        if (latencia_ms < metricas->latencia_min_ms) {
            metricas->latencia_min_ms = latencia_ms;
        }
        if (latencia_ms > metricas->latencia_max_ms) {
            metricas->latencia_max_ms = latencia_ms;
        }
        
        // Guardar latencia individual si está habilitado
        if (config->guardar_latencias) {
            pthread_mutex_lock(&g_resultados.mutex);
            if (g_num_latencias < config->num_clientes * config->num_mensajes_por_cliente) {
                g_latencias[g_num_latencias++] = latencia_ms;
            }
            pthread_mutex_unlock(&g_resultados.mutex);
        }
    }
    
    gettimeofday(&metricas->tiempo_fin, NULL);
    
    // Cerrar conexión
    send(sockfd, "quit", 4, 0);
    close(sockfd);
    
    // Actualizar resultados globales
    pthread_mutex_lock(&g_resultados.mutex);
    g_resultados.clientes_completados++;
    g_resultados.mensajes_totales += metricas->mensajes_enviados;
    g_resultados.bytes_totales += metricas->bytes_enviados + metricas->bytes_recibidos;
    g_resultados.errores_totales += metricas->errores;
    pthread_mutex_unlock(&g_resultados.mutex);
    
    return NULL;
}

/**
 * @brief Función de comparación para qsort (latencias)
 */
int comparar_doubles(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

/**
 * @brief Calcula percentiles de latencia
 */
void calcular_percentiles(void) {
    if (g_num_latencias == 0 || !g_latencias) return;
    
    // Ordenar latencias
    qsort(g_latencias, g_num_latencias, sizeof(double), comparar_doubles);
    
    // Calcular percentiles
    size_t idx_50 = g_num_latencias * 50 / 100;
    size_t idx_95 = g_num_latencias * 95 / 100;
    size_t idx_99 = g_num_latencias * 99 / 100;
    
    g_resultados.latencia_mediana_ms = g_latencias[idx_50];
    g_resultados.latencia_p95_ms = g_latencias[idx_95];
    g_resultados.latencia_p99_ms = g_latencias[idx_99];
}

/**
 * @brief Ejecuta el benchmark
 */
int ejecutar_benchmark(const ConfigBenchmark* config) {
    printf("=== INICIANDO BENCHMARK ===\n");
    printf("Servidor: %s:%d\n", config->servidor, config->puerto);
    printf("Clientes: %d\n", config->num_clientes);
    printf("Mensajes por cliente: %d\n", config->num_mensajes_por_cliente);
    printf("Tamaño de mensaje: %d bytes\n", config->tamaño_mensaje);
    printf("Total de mensajes: %d\n", config->num_clientes * config->num_mensajes_por_cliente);
    printf("==========================\n\n");
    
    // Inicializar estructuras
    memset(&g_resultados, 0, sizeof(g_resultados));
    pthread_mutex_init(&g_resultados.mutex, NULL);
    
    g_metricas_clientes = calloc(config->num_clientes, sizeof(MetricasCliente));
    if (!g_metricas_clientes) {
        fprintf(stderr, "Error asignando memoria para métricas\n");
        return -1;
    }
    
    if (config->guardar_latencias) {
        g_latencias = calloc(config->num_clientes * config->num_mensajes_por_cliente, sizeof(double));
        if (!g_latencias) {
            fprintf(stderr, "Error asignando memoria para latencias\n");
            free(g_metricas_clientes);
            return -1;
        }
    }
    
    pthread_t* hilos = malloc(config->num_clientes * sizeof(pthread_t));
    if (!hilos) {
        fprintf(stderr, "Error asignando memoria para hilos\n");
        free(g_metricas_clientes);
        free(g_latencias);
        return -1;
    }
    
    struct timeval inicio_benchmark, fin_benchmark;
    gettimeofday(&inicio_benchmark, NULL);
    
    // Crear hilos cliente
    for (int i = 0; i < config->num_clientes; i++) {
        g_metricas_clientes[i].id_cliente = i + 1;
        
        // Crear estructura combinada para pasar al hilo
        char* args = malloc(sizeof(MetricasCliente) + sizeof(ConfigBenchmark));
        memcpy(args, &g_metricas_clientes[i], sizeof(MetricasCliente));
        memcpy(args + sizeof(MetricasCliente), config, sizeof(ConfigBenchmark));
        
        if (pthread_create(&hilos[i], NULL, cliente_benchmark, args) != 0) {
            fprintf(stderr, "Error creando hilo cliente %d\n", i);
            break;
        }
        
        // Intervalo entre conexiones
        if (config->intervalo_conexion_ms > 0) {
            usleep(config->intervalo_conexion_ms * 1000);
        }
    }
    
    // Mostrar progreso
    if (config->mostrar_progreso) {
        printf("Progreso: ");
        fflush(stdout);
        
        while (g_resultados.clientes_completados < config->num_clientes && ejecutando) {
            printf(".");
            fflush(stdout);
            sleep(1);
        }
        printf(" ¡Completado!\n\n");
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < config->num_clientes; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    gettimeofday(&fin_benchmark, NULL);
    g_resultados.tiempo_total_segundos = calcular_diferencia_ms(&inicio_benchmark, &fin_benchmark) / 1000.0;
    
    // Calcular métricas globales
    if (g_resultados.tiempo_total_segundos > 0) {
        g_resultados.throughput_mensajes_por_segundo = g_resultados.mensajes_totales / g_resultados.tiempo_total_segundos;
        g_resultados.throughput_bytes_por_segundo = g_resultados.bytes_totales / g_resultados.tiempo_total_segundos;
    }
    
    // Calcular latencia promedio
    double latencia_total = 0.0;
    int clientes_validos = 0;
    
    for (int i = 0; i < config->num_clientes; i++) {
        if (g_metricas_clientes[i].mensajes_enviados > 0) {
            latencia_total += g_metricas_clientes[i].latencia_total_ms / g_metricas_clientes[i].mensajes_enviados;
            clientes_validos++;
        }
    }
    
    if (clientes_validos > 0) {
        g_resultados.latencia_promedio_ms = latencia_total / clientes_validos;
    }
    
    // Calcular percentiles si están disponibles
    if (config->guardar_latencias) {
        calcular_percentiles();
    }
    
    // Limpiar recursos
    free(hilos);
    
    return 0;
}

/**
 * @brief Muestra resultados del benchmark
 */
void mostrar_resultados(const ConfigBenchmark* config) {
    printf("=== RESULTADOS DEL BENCHMARK ===\n");
    printf("Tiempo total: %.2f segundos\n", g_resultados.tiempo_total_segundos);
    printf("Clientes completados: %d/%d\n", g_resultados.clientes_completados, config->num_clientes);
    printf("Mensajes totales: %zu\n", g_resultados.mensajes_totales);
    printf("Bytes totales: %zu\n", g_resultados.bytes_totales);
    printf("Errores totales: %d\n", g_resultados.errores_totales);
    printf("\n--- THROUGHPUT ---\n");
    printf("Mensajes/segundo: %.2f\n", g_resultados.throughput_mensajes_por_segundo);
    printf("Bytes/segundo: %.2f (%.2f KB/s, %.2f MB/s)\n", 
           g_resultados.throughput_bytes_por_segundo,
           g_resultados.throughput_bytes_por_segundo / 1024.0,
           g_resultados.throughput_bytes_por_segundo / (1024.0 * 1024.0));
    
    printf("\n--- LATENCIA ---\n");
    printf("Latencia promedio: %.2f ms\n", g_resultados.latencia_promedio_ms);
    
    if (config->guardar_latencias && g_num_latencias > 0) {
        printf("Latencia mediana (P50): %.2f ms\n", g_resultados.latencia_mediana_ms);
        printf("Latencia P95: %.2f ms\n", g_resultados.latencia_p95_ms);
        printf("Latencia P99: %.2f ms\n", g_resultados.latencia_p99_ms);
    }
    
    // Mostrar estadísticas por cliente (solo primeros 10)
    printf("\n--- ESTADÍSTICAS POR CLIENTE (primeros 10) ---\n");
    printf("ID\tMsgs\tBytes\tLat.Avg\tLat.Min\tLat.Max\tErrores\n");
    
    int clientes_mostrados = (config->num_clientes > 10) ? 10 : config->num_clientes;
    for (int i = 0; i < clientes_mostrados; i++) {
        MetricasCliente* m = &g_metricas_clientes[i];
        double lat_avg = (m->mensajes_enviados > 0) ? m->latencia_total_ms / m->mensajes_enviados : 0.0;
        
        printf("%d\t%zu\t%zu\t%.2f\t%.2f\t%.2f\t%d\n",
               m->id_cliente, m->mensajes_enviados, m->bytes_enviados,
               lat_avg, m->latencia_min_ms, m->latencia_max_ms, m->errores);
    }
    
    if (config->num_clientes > 10) {
        printf("... y %d clientes más\n", config->num_clientes - 10);
    }
    
    printf("===============================\n");
}

/**
 * @brief Muestra ayuda del programa
 */
void mostrar_ayuda(const char* programa) {
    printf("Uso: %s [opciones]\n", programa);
    printf("\nOpciones:\n");
    printf("  -h, --help               Mostrar esta ayuda\n");
    printf("  -s, --servidor IP        IP del servidor (default: %s)\n", SERVIDOR_DEFAULT);
    printf("  -p, --puerto PUERTO      Puerto del servidor (default: %d)\n", PUERTO_DEFAULT);
    printf("  -c, --clientes NUM       Número de clientes concurrentes (default: 10)\n");
    printf("  -m, --mensajes NUM       Mensajes por cliente (default: 100)\n");
    printf("  -t, --tamaño BYTES       Tamaño del mensaje en bytes (default: 64)\n");
    printf("  -i, --intervalo MS       Intervalo entre conexiones en ms (default: 10)\n");
    printf("  -P, --progreso           Mostrar progreso durante el benchmark\n");
    printf("  -L, --latencias          Guardar todas las latencias para percentiles\n");
    printf("\nEjemplos:\n");
    printf("  %s                              # Benchmark básico\n", programa);
    printf("  %s -c 50 -m 200                 # 50 clientes, 200 mensajes cada uno\n", programa);
    printf("  %s -P -L                        # Con progreso y análisis de latencias\n", programa);
    printf("  %s -s 192.168.1.100 -p 8080    # Servidor remoto\n", programa);
}

/**
 * @brief Función principal
 */
int main(int argc, char* argv[]) {
    ConfigBenchmark config = {
        .puerto = PUERTO_DEFAULT,
        .num_clientes = 10,
        .num_mensajes_por_cliente = 100,
        .tamaño_mensaje = 64,
        .duracion_segundos = 0,
        .intervalo_conexion_ms = 10,
        .mostrar_progreso = 0,
        .guardar_latencias = 0
    };
    
    strcpy(config.servidor, SERVIDOR_DEFAULT);
    
    // Procesar argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            mostrar_ayuda(argv[0]);
            return 0;
        } else if ((strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--servidor") == 0) && i + 1 < argc) {
            strcpy(config.servidor, argv[++i]);
        } else if ((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--puerto") == 0) && i + 1 < argc) {
            config.puerto = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clientes") == 0) && i + 1 < argc) {
            config.num_clientes = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mensajes") == 0) && i + 1 < argc) {
            config.num_mensajes_por_cliente = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tamaño") == 0) && i + 1 < argc) {
            config.tamaño_mensaje = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--intervalo") == 0) && i + 1 < argc) {
            config.intervalo_conexion_ms = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-P") == 0 || strcmp(argv[i], "--progreso") == 0) {
            config.mostrar_progreso = 1;
        } else if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--latencias") == 0) {
            config.guardar_latencias = 1;
        }
    }
    
    // Validar configuración
    if (config.num_clientes <= 0 || config.num_clientes > MAX_HILOS) {
        fprintf(stderr, "Error: Número de clientes debe estar entre 1 y %d\n", MAX_HILOS);
        return 1;
    }
    
    if (config.tamaño_mensaje <= 0 || config.tamaño_mensaje >= BUFFER_SIZE) {
        fprintf(stderr, "Error: Tamaño de mensaje debe estar entre 1 y %d\n", BUFFER_SIZE - 1);
        return 1;
    }
    
    // Configurar manejadores de señales
    signal(SIGINT, manejador_senales);
    signal(SIGTERM, manejador_senales);
    signal(SIGPIPE, SIG_IGN);
    
    // Ejecutar benchmark
    if (ejecutar_benchmark(&config) == 0) {
        mostrar_resultados(&config);
    }
    
    // Limpiar recursos
    free(g_metricas_clientes);
    free(g_latencias);
    pthread_mutex_destroy(&g_resultados.mutex);
    
    return 0;
}
