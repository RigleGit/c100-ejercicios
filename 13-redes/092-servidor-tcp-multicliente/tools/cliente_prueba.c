/**
 * @file cliente_prueba.c
 * @brief Cliente TCP para testing del servidor multicliente
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Herramienta para probar el servidor TCP multicliente con opciones
 * avanzadas de configuración, stress testing y análisis de rendimiento.
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

#define BUFFER_SIZE 4096
#define SERVIDOR_DEFAULT "127.0.0.1"
#define PUERTO_DEFAULT 9090
#define MENSAJE_DEFAULT "Mensaje de prueba"

// Variables globales para control
static volatile sig_atomic_t ejecutando = 1;

/**
 * @brief Configuración del cliente de prueba
 */
typedef struct {
    char servidor[64];
    int puerto;
    int num_mensajes;
    int intervalo_ms;
    int timeout_segundos;
    int modo_interactivo;
    int modo_stress;
    int num_hilos;
    int mostrar_estadisticas;
    char mensaje[BUFFER_SIZE];
} ConfigCliente;

/**
 * @brief Estadísticas del cliente
 */
typedef struct {
    size_t mensajes_enviados;
    size_t mensajes_recibidos;
    size_t bytes_enviados;
    size_t bytes_recibidos;
    size_t errores_conexion;
    size_t errores_envio;
    size_t errores_recepcion;
    struct timeval tiempo_inicio;
    struct timeval tiempo_fin;
    pthread_mutex_t mutex;
} EstadisticasCliente;

/**
 * @brief Parámetros para hilos de cliente
 */
typedef struct {
    ConfigCliente* config;
    EstadisticasCliente* stats;
    int id_hilo;
} ParametrosHilo;

/**
 * @brief Manejador de señales
 */
void manejador_senales(int signum) {
    printf("\nSeñal %d recibida. Cerrando...\n", signum);
    ejecutando = 0;
}

/**
 * @brief Muestra ayuda del programa
 */
void mostrar_ayuda(const char* programa) {
    printf("Uso: %s [opciones]\n", programa);
    printf("\nOpciones:\n");
    printf("  -h, --help                Mostrar esta ayuda\n");
    printf("  -s, --servidor IP         IP del servidor (default: %s)\n", SERVIDOR_DEFAULT);
    printf("  -p, --puerto PUERTO       Puerto del servidor (default: %d)\n", PUERTO_DEFAULT);
    printf("  -n, --mensajes NUM        Número de mensajes a enviar (default: 1)\n");
    printf("  -i, --intervalo MS        Intervalo entre mensajes en ms (default: 0)\n");
    printf("  -t, --timeout SEG         Timeout de conexión en segundos (default: 10)\n");
    printf("  -m, --mensaje TEXTO       Mensaje a enviar (default: \"%s\")\n", MENSAJE_DEFAULT);
    printf("  -I, --interactivo         Modo interactivo\n");
    printf("  -S, --stress HILOS        Modo stress con N hilos concurrentes\n");
    printf("  -v, --verbose             Mostrar estadísticas detalladas\n");
    printf("\nEjemplos:\n");
    printf("  %s                                    # Conexión básica\n", programa);
    printf("  %s -n 10 -i 1000                     # 10 mensajes con intervalo de 1s\n", programa);
    printf("  %s -I                                # Modo interactivo\n", programa);
    printf("  %s -S 5 -n 100                       # Stress con 5 hilos, 100 msg c/u\n", programa);
    printf("  %s -s 192.168.1.100 -p 8080         # Servidor remoto\n", programa);
}

/**
 * @brief Inicializa configuración por defecto
 */
void configurar_cliente_por_defecto(ConfigCliente* config) {
    strcpy(config->servidor, SERVIDOR_DEFAULT);
    config->puerto = PUERTO_DEFAULT;
    config->num_mensajes = 1;
    config->intervalo_ms = 0;
    config->timeout_segundos = 10;
    config->modo_interactivo = 0;
    config->modo_stress = 0;
    config->num_hilos = 1;
    config->mostrar_estadisticas = 0;
    strcpy(config->mensaje, MENSAJE_DEFAULT);
}

/**
 * @brief Inicializa estadísticas
 */
void inicializar_estadisticas(EstadisticasCliente* stats) {
    memset(stats, 0, sizeof(EstadisticasCliente));
    pthread_mutex_init(&stats->mutex, NULL);
    gettimeofday(&stats->tiempo_inicio, NULL);
}

/**
 * @brief Actualiza estadísticas de forma thread-safe
 */
void actualizar_estadisticas(EstadisticasCliente* stats, size_t bytes_env, 
                            size_t bytes_rec, int error_env, int error_rec) {
    pthread_mutex_lock(&stats->mutex);
    
    if (bytes_env > 0) {
        stats->mensajes_enviados++;
        stats->bytes_enviados += bytes_env;
    }
    if (bytes_rec > 0) {
        stats->mensajes_recibidos++;
        stats->bytes_recibidos += bytes_rec;
    }
    if (error_env) stats->errores_envio++;
    if (error_rec) stats->errores_recepcion++;
    
    pthread_mutex_unlock(&stats->mutex);
}

/**
 * @brief Muestra estadísticas finales
 */
void mostrar_estadisticas_finales(EstadisticasCliente* stats) {
    gettimeofday(&stats->tiempo_fin, NULL);
    
    long tiempo_total_ms = (stats->tiempo_fin.tv_sec - stats->tiempo_inicio.tv_sec) * 1000 +
                          (stats->tiempo_fin.tv_usec - stats->tiempo_inicio.tv_usec) / 1000;
    
    pthread_mutex_lock(&stats->mutex);
    
    printf("\n=== ESTADÍSTICAS FINALES ===\n");
    printf("Tiempo total: %ld ms\n", tiempo_total_ms);
    printf("Mensajes enviados: %zu\n", stats->mensajes_enviados);
    printf("Mensajes recibidos: %zu\n", stats->mensajes_recibidos);
    printf("Bytes enviados: %zu\n", stats->bytes_enviados);
    printf("Bytes recibidos: %zu\n", stats->bytes_recibidos);
    printf("Errores de conexión: %zu\n", stats->errores_conexion);
    printf("Errores de envío: %zu\n", stats->errores_envio);
    printf("Errores de recepción: %zu\n", stats->errores_recepcion);
    
    if (tiempo_total_ms > 0) {
        printf("Promedio: %.2f mensajes/s\n", 
               (double)stats->mensajes_enviados * 1000.0 / tiempo_total_ms);
        printf("Throughput: %.2f bytes/s\n", 
               (double)(stats->bytes_enviados + stats->bytes_recibidos) * 1000.0 / tiempo_total_ms);
    }
    
    if (stats->mensajes_enviados > 0) {
        printf("Tasa de éxito: %.2f%%\n", 
               (double)stats->mensajes_recibidos * 100.0 / stats->mensajes_enviados);
    }
    
    printf("==========================\n");
    
    pthread_mutex_unlock(&stats->mutex);
}

/**
 * @brief Conecta al servidor TCP
 */
int conectar_servidor(const ConfigCliente* config) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creando socket");
        return -1;
    }
    
    // Configurar timeout
    struct timeval timeout;
    timeout.tv_sec = config->timeout_segundos;
    timeout.tv_usec = 0;
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Error configurando timeout de recepción");
    }
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Error configurando timeout de envío");
    }
    
    // Configurar dirección del servidor
    struct sockaddr_in direccion_servidor;
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(config->puerto);
    
    if (inet_pton(AF_INET, config->servidor, &direccion_servidor.sin_addr) <= 0) {
        fprintf(stderr, "Error: Dirección IP inválida: %s\n", config->servidor);
        close(sockfd);
        return -1;
    }
    
    // Conectar
    if (connect(sockfd, (struct sockaddr*)&direccion_servidor, 
                sizeof(direccion_servidor)) < 0) {
        perror("Error conectando al servidor");
        close(sockfd);
        return -1;
    }
    
    return sockfd;
}

/**
 * @brief Cliente básico (envía mensajes y recibe respuestas)
 */
void* cliente_basico(void* arg) {
    ParametrosHilo* params = (ParametrosHilo*)arg;
    ConfigCliente* config = params->config;
    EstadisticasCliente* stats = params->stats;
    int id = params->id_hilo;
    
    int sockfd = conectar_servidor(config);
    if (sockfd < 0) {
        pthread_mutex_lock(&stats->mutex);
        stats->errores_conexion++;
        pthread_mutex_unlock(&stats->mutex);
        return NULL;
    }
    
    printf("Hilo %d: Conectado al servidor %s:%d\n", id, config->servidor, config->puerto);
    
    char buffer[BUFFER_SIZE];
    
    // Recibir mensaje de bienvenida
    ssize_t bytes_bienvenida = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_bienvenida > 0) {
        buffer[bytes_bienvenida] = '\0';
        printf("Hilo %d: Bienvenida: %s", id, buffer);
    }
    
    // Enviar mensajes
    for (int i = 0; i < config->num_mensajes && ejecutando; i++) {
        char mensaje_numerado[BUFFER_SIZE];
        
        if (config->num_mensajes > 1 || config->num_hilos > 1) {
            snprintf(mensaje_numerado, sizeof(mensaje_numerado), 
                    "%s (hilo:%d msg:%d/%d)", config->mensaje, id, i + 1, config->num_mensajes);
        } else {
            strcpy(mensaje_numerado, config->mensaje);
        }
        
        // Enviar mensaje
        ssize_t bytes_enviados = send(sockfd, mensaje_numerado, strlen(mensaje_numerado), 0);
        if (bytes_enviados < 0) {
            perror("Error enviando mensaje");
            actualizar_estadisticas(stats, 0, 0, 1, 0);
            break;
        }
        
        // Recibir respuesta
        ssize_t bytes_recibidos = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_recibidos > 0) {
            buffer[bytes_recibidos] = '\0';
            if (config->mostrar_estadisticas) {
                printf("Hilo %d: Enviado: %zd bytes, Recibido: %zd bytes\n", 
                       id, bytes_enviados, bytes_recibidos);
            }
            actualizar_estadisticas(stats, bytes_enviados, bytes_recibidos, 0, 0);
        } else if (bytes_recibidos < 0) {
            perror("Error recibiendo respuesta");
            actualizar_estadisticas(stats, bytes_enviados, 0, 0, 1);
        } else {
            printf("Hilo %d: Servidor cerró la conexión\n", id);
            break;
        }
        
        // Intervalo entre mensajes
        if (config->intervalo_ms > 0 && i < config->num_mensajes - 1) {
            usleep(config->intervalo_ms * 1000);
        }
    }
    
    // Enviar comando quit para desconexión ordenada
    send(sockfd, "quit", 4, 0);
    
    close(sockfd);
    printf("Hilo %d: Desconectado\n", id);
    
    return NULL;
}

/**
 * @brief Modo interactivo
 */
void modo_interactivo(const ConfigCliente* config) {
    printf("=== MODO INTERACTIVO ===\n");
    printf("Conectando a %s:%d...\n", config->servidor, config->puerto);
    
    int sockfd = conectar_servidor(config);
    if (sockfd < 0) {
        return;
    }
    
    printf("Conectado. Escribe mensajes (o 'quit' para salir):\n");
    
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];
    
    // Recibir mensaje de bienvenida
    ssize_t bytes_bienvenida = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_bienvenida > 0) {
        buffer[bytes_bienvenida] = '\0';
        printf("Servidor: %s", buffer);
    }
    
    while (ejecutando) {
        printf("> ");
        fflush(stdout);
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        // Remover newline
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        // Enviar mensaje
        if (send(sockfd, input, strlen(input), 0) < 0) {
            perror("Error enviando mensaje");
            break;
        }
        
        // Recibir respuesta
        ssize_t bytes_recibidos = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_recibidos > 0) {
            buffer[bytes_recibidos] = '\0';
            printf("Servidor: %s\n", buffer);
        } else if (bytes_recibidos == 0) {
            printf("Servidor cerró la conexión\n");
            break;
        } else {
            perror("Error recibiendo respuesta");
            break;
        }
    }
    
    close(sockfd);
    printf("Desconectado del servidor\n");
}

/**
 * @brief Modo stress con múltiples hilos
 */
void modo_stress(const ConfigCliente* config, EstadisticasCliente* stats) {
    printf("=== MODO STRESS ===\n");
    printf("Iniciando %d hilos concurrentes...\n", config->num_hilos);
    printf("Cada hilo enviará %d mensajes\n", config->num_mensajes);
    printf("Total de mensajes: %d\n", config->num_hilos * config->num_mensajes);
    
    pthread_t* hilos = malloc(config->num_hilos * sizeof(pthread_t));
    ParametrosHilo* parametros = malloc(config->num_hilos * sizeof(ParametrosHilo));
    
    if (!hilos || !parametros) {
        fprintf(stderr, "Error asignando memoria para hilos\n");
        free(hilos);
        free(parametros);
        return;
    }
    
    // Crear hilos
    for (int i = 0; i < config->num_hilos; i++) {
        parametros[i].config = (ConfigCliente*)config;
        parametros[i].stats = stats;
        parametros[i].id_hilo = i + 1;
        
        if (pthread_create(&hilos[i], NULL, cliente_basico, &parametros[i]) != 0) {
            perror("Error creando hilo");
            break;
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < config->num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    free(hilos);
    free(parametros);
    
    printf("Todos los hilos han terminado\n");
}

/**
 * @brief Procesa argumentos de línea de comandos
 */
int procesar_argumentos(int argc, char* argv[], ConfigCliente* config) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            mostrar_ayuda(argv[0]);
            return 0;
        } else if ((strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--servidor") == 0) && i + 1 < argc) {
            strcpy(config->servidor, argv[++i]);
        } else if ((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--puerto") == 0) && i + 1 < argc) {
            config->puerto = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--mensajes") == 0) && i + 1 < argc) {
            config->num_mensajes = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--intervalo") == 0) && i + 1 < argc) {
            config->intervalo_ms = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--timeout") == 0) && i + 1 < argc) {
            config->timeout_segundos = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mensaje") == 0) && i + 1 < argc) {
            strcpy(config->mensaje, argv[++i]);
        } else if (strcmp(argv[i], "-I") == 0 || strcmp(argv[i], "--interactivo") == 0) {
            config->modo_interactivo = 1;
        } else if ((strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--stress") == 0) && i + 1 < argc) {
            config->modo_stress = 1;
            config->num_hilos = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            config->mostrar_estadisticas = 1;
        } else {
            fprintf(stderr, "Argumento desconocido: %s\n", argv[i]);
            return -1;
        }
    }
    
    return 1;
}

/**
 * @brief Función principal
 */
int main(int argc, char* argv[]) {
    ConfigCliente config;
    EstadisticasCliente stats;
    
    // Configurar manejadores de señales
    signal(SIGINT, manejador_senales);
    signal(SIGTERM, manejador_senales);
    signal(SIGPIPE, SIG_IGN);
    
    // Configuración por defecto
    configurar_cliente_por_defecto(&config);
    
    // Procesar argumentos
    int resultado_args = procesar_argumentos(argc, argv, &config);
    if (resultado_args <= 0) {
        return (resultado_args == 0) ? 0 : 1;
    }
    
    // Validar configuración
    if (config.puerto <= 0 || config.puerto > 65535) {
        fprintf(stderr, "Error: Puerto inválido\n");
        return 1;
    }
    
    if (config.num_mensajes <= 0) {
        fprintf(stderr, "Error: Número de mensajes debe ser positivo\n");
        return 1;
    }
    
    if (config.modo_stress && config.num_hilos <= 0) {
        fprintf(stderr, "Error: Número de hilos debe ser positivo\n");
        return 1;
    }
    
    // Inicializar estadísticas
    inicializar_estadisticas(&stats);
    
    printf("Cliente TCP de prueba\n");
    printf("Servidor: %s:%d\n", config.servidor, config.puerto);
    
    // Ejecutar según el modo
    if (config.modo_interactivo) {
        modo_interactivo(&config);
    } else if (config.modo_stress) {
        modo_stress(&config, &stats);
        mostrar_estadisticas_finales(&stats);
    } else {
        // Modo básico
        ParametrosHilo params = {&config, &stats, 1};
        cliente_basico(&params);
        if (config.mostrar_estadisticas) {
            mostrar_estadisticas_finales(&stats);
        }
    }
    
    // Limpiar recursos
    pthread_mutex_destroy(&stats.mutex);
    
    return 0;
}
