/**
 * @file receptor_udp.c
 * @brief Programa receptor UDP - Servidor que recibe datagramas
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Este programa implementa un servidor UDP que recibe mensajes de clientes.
 * Demuestra el uso de bind() y recvfrom() para comunicación sin conexión.
 * 
 * Compilación:
 *   gcc -o receptor_udp receptor_udp.c
 * 
 * Uso:
 *   ./receptor_udp [puerto]
 *   ./receptor_udp 9090
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>

#define PUERTO_DEFAULT 9090
#define BUFFER_SIZE 4096
#define TIMEOUT_SEC 300  // 5 minutos

// Variable global para manejo de señales
static volatile sig_atomic_t ejecutando = 1;

/**
 * @brief Manejador de señales para cierre ordenado
 * @param signum Número de señal
 */
void manejador_senales(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        printf("\nSeñal recibida (%d). Cerrando servidor...\n", signum);
        ejecutando = 0;
    }
}

/**
 * @brief Muestra la ayuda del programa
 * @param programa_nombre Nombre del programa
 */
void mostrar_ayuda(const char* programa_nombre) {
    printf("Uso: %s [opciones] [puerto]\n", programa_nombre);
    printf("\nOpciones:\n");
    printf("  -h, --help     Mostrar esta ayuda\n");
    printf("  -v, --verbose  Modo verboso\n");
    printf("  -t <segundos>  Timeout para recepción (default: %d)\n", TIMEOUT_SEC);
    printf("  -b <tamaño>    Tamaño del buffer (default: %d)\n", BUFFER_SIZE);
    printf("  -e, --echo     Modo eco (responder a cada mensaje)\n");
    printf("  -s, --stats    Mostrar estadísticas cada 10 mensajes\n");
    printf("\nEjemplos:\n");
    printf("  %s                    # Escuchar en puerto %d\n", programa_nombre, PUERTO_DEFAULT);
    printf("  %s 8080               # Escuchar en puerto 8080\n", programa_nombre);
    printf("  %s -v -e 9090         # Modo verboso y eco en puerto 9090\n", programa_nombre);
    printf("  %s -s -t 60           # Estadísticas cada 10 mensajes, timeout 60s\n", programa_nombre);
    printf("\nPresiona Ctrl+C para detener el servidor\n");
}

/**
 * @brief Configura timeout para el socket
 * @param sockfd Descriptor del socket
 * @param timeout_sec Timeout en segundos
 * @return 0 si éxito, -1 si error
 */
int configurar_timeout(int sockfd, int timeout_sec) {
    struct timeval tv;
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error configurando timeout de recepción");
        return -1;
    }
    
    return 0;
}

/**
 * @brief Formatea dirección IP y puerto como string
 * @param addr Estructura sockaddr_in
 * @param buffer Buffer para el resultado
 * @param tam_buffer Tamaño del buffer
 * @return Puntero al buffer
 */
char* formatear_direccion(struct sockaddr_in* addr, char* buffer, size_t tam_buffer) {
    snprintf(buffer, tam_buffer, "%s:%d", 
             inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    return buffer;
}

/**
 * @brief Procesa un mensaje recibido
 * @param buffer Buffer con el mensaje
 * @param bytes_recibidos Número de bytes recibidos
 * @param cliente Dirección del cliente
 * @param verbose Modo verboso
 * @param mensaje_num Número de mensaje
 */
void procesar_mensaje(const char* buffer, ssize_t bytes_recibidos, 
                     struct sockaddr_in* cliente, int verbose, int mensaje_num) {
    char dir_cliente[64];
    formatear_direccion(cliente, dir_cliente, sizeof(dir_cliente));
    
    if (verbose) {
        printf("\n--- Mensaje #%d ---\n", mensaje_num);
        printf("De: %s\n", dir_cliente);
        printf("Tamaño: %zd bytes\n", bytes_recibidos);
        printf("Contenido: \"%.*s\"\n", (int)bytes_recibidos, buffer);
        
        // Mostrar bytes en hexadecimal si contiene datos binarios
        int es_texto = 1;
        for (ssize_t i = 0; i < bytes_recibidos; i++) {
            if (buffer[i] < 32 && buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\t') {
                es_texto = 0;
                break;
            }
        }
        
        if (!es_texto) {
            printf("Hex: ");
            for (ssize_t i = 0; i < bytes_recibidos && i < 32; i++) {
                printf("%02x ", (unsigned char)buffer[i]);
            }
            if (bytes_recibidos > 32) printf("...");
            printf("\n");
        }
        printf("------------------\n");
    } else {
        printf("Recibido de %s: \"%.*s\" (%zd bytes)\n", 
               dir_cliente, (int)bytes_recibidos, buffer, bytes_recibidos);
    }
}

/**
 * @brief Envía respuesta eco al cliente
 * @param sockfd Descriptor del socket
 * @param cliente Dirección del cliente
 * @param buffer Buffer con el mensaje original
 * @param bytes_recibidos Número de bytes del mensaje original
 * @param verbose Modo verboso
 * @return 0 si éxito, -1 si error
 */
int enviar_eco(int sockfd, struct sockaddr_in* cliente, const char* buffer, 
               ssize_t bytes_recibidos, int verbose) {
    char respuesta[BUFFER_SIZE + 100];
    snprintf(respuesta, sizeof(respuesta), "ECO: %.*s", (int)bytes_recibidos, buffer);
    
    ssize_t bytes_enviados = sendto(sockfd, respuesta, strlen(respuesta), 0,
                                   (struct sockaddr*)cliente, sizeof(*cliente));
    
    if (bytes_enviados < 0) {
        perror("Error enviando eco");
        return -1;
    }
    
    if (verbose) {
        char dir_cliente[64];
        formatear_direccion(cliente, dir_cliente, sizeof(dir_cliente));
        printf("Eco enviado a %s: %zd bytes\n", dir_cliente, bytes_enviados);
    }
    
    return 0;
}

/**
 * @brief Muestra estadísticas del servidor
 * @param mensajes_recibidos Número de mensajes recibidos
 * @param total_bytes Total de bytes recibidos
 * @param tiempo_inicio Tiempo de inicio del servidor
 */
void mostrar_estadisticas(int mensajes_recibidos, size_t total_bytes, struct timeval* tiempo_inicio) {
    struct timeval ahora;
    gettimeofday(&ahora, NULL);
    
    long tiempo_transcurrido = (ahora.tv_sec - tiempo_inicio->tv_sec) * 1000 + 
                              (ahora.tv_usec - tiempo_inicio->tv_usec) / 1000;
    
    printf("\n=== Estadísticas ===\n");
    printf("Mensajes recibidos: %d\n", mensajes_recibidos);
    printf("Total de bytes: %zu\n", total_bytes);
    printf("Tiempo activo: %ld ms\n", tiempo_transcurrido);
    
    if (tiempo_transcurrido > 0) {
        printf("Promedio: %.2f mensajes/s\n", (double)mensajes_recibidos * 1000.0 / tiempo_transcurrido);
        printf("Throughput: %.2f bytes/s\n", (double)total_bytes * 1000.0 / tiempo_transcurrido);
    }
    
    if (mensajes_recibidos > 0) {
        printf("Tamaño promedio: %.2f bytes/mensaje\n", (double)total_bytes / mensajes_recibidos);
    }
    printf("===================\n\n");
}

/**
 * @brief Función principal del receptor UDP
 */
int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servidor, cliente;
    socklen_t len_cliente = sizeof(cliente);
    char buffer[BUFFER_SIZE];
    int puerto = PUERTO_DEFAULT;
    int verbose = 0;
    int timeout_sec = TIMEOUT_SEC;
    int buffer_size = BUFFER_SIZE;
    int modo_eco = 0;
    int mostrar_stats = 0;
    
    // Procesar argumentos de línea de comandos
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            mostrar_ayuda(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
            i++;
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--echo") == 0) {
            modo_eco = 1;
            i++;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0) {
            mostrar_stats = 1;
            i++;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeout_sec = atoi(argv[i + 1]);
            if (timeout_sec <= 0) {
                fprintf(stderr, "Error: Timeout debe ser positivo\n");
                return 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            buffer_size = atoi(argv[i + 1]);
            if (buffer_size <= 0 || buffer_size > BUFFER_SIZE) {
                fprintf(stderr, "Error: Tamaño de buffer inválido (1-%d)\n", BUFFER_SIZE);
                return 1;
            }
            i += 2;
        } else {
            puerto = atoi(argv[i]);
            if (puerto <= 0 || puerto > 65535) {
                fprintf(stderr, "Error: Puerto inválido (1-65535)\n");
                return 1;
            }
            i++;
        }
    }
    
    // Configurar manejadores de señales
    signal(SIGINT, manejador_senales);
    signal(SIGTERM, manejador_senales);
    
    if (verbose) {
        printf("=== Servidor UDP ===\n");
        printf("Puerto: %d\n", puerto);
        printf("Buffer: %d bytes\n", buffer_size);
        printf("Timeout: %d s\n", timeout_sec);
        printf("Modo eco: %s\n", modo_eco ? "Sí" : "No");
        printf("Estadísticas: %s\n", mostrar_stats ? "Sí" : "No");
        printf("====================\n\n");
    }
    
    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear socket UDP");
        exit(EXIT_FAILURE);
    }
    
    // Permitir reutilización de dirección
    int reuseaddr = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0) {
        perror("Error configurando SO_REUSEADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    // Configurar timeout
    if (configurar_timeout(sockfd, timeout_sec) < 0) {
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    // Configurar dirección del servidor
    memset(&servidor, 0, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = htons(puerto);
    
    // Bind del socket
    if (bind(sockfd, (struct sockaddr*)&servidor, sizeof(servidor)) < 0) {
        perror("Error en bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Servidor UDP escuchando en puerto %d...\n", puerto);
    if (!verbose) {
        printf("Usa Ctrl+C para detener el servidor\n");
    }
    
    // Variables para estadísticas
    int mensajes_recibidos = 0;
    size_t total_bytes = 0;
    struct timeval tiempo_inicio;
    gettimeofday(&tiempo_inicio, NULL);
    
    // Bucle principal de recepción
    while (ejecutando) {
        ssize_t bytes_recibidos = recvfrom(sockfd, buffer, buffer_size - 1, 0,
                                          (struct sockaddr*)&cliente, &len_cliente);
        
        if (bytes_recibidos < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                if (verbose) printf("Timeout en recepción\n");
                continue;
            } else if (errno == EINTR) {
                // Señal recibida, continuar
                continue;
            } else {
                perror("Error en recvfrom");
                break;
            }
        }
        
        if (bytes_recibidos == 0) {
            if (verbose) printf("Recibido datagrama vacío\n");
            continue;
        }
        
        // Null-terminar para tratamiento como string
        buffer[bytes_recibidos] = '\0';
        
        mensajes_recibidos++;
        total_bytes += bytes_recibidos;
        
        // Procesar mensaje
        procesar_mensaje(buffer, bytes_recibidos, &cliente, verbose, mensajes_recibidos);
        
        // Enviar eco si está habilitado
        if (modo_eco) {
            enviar_eco(sockfd, &cliente, buffer, bytes_recibidos, verbose);
        }
        
        // Mostrar estadísticas periódicas
        if (mostrar_stats && mensajes_recibidos % 10 == 0) {
            mostrar_estadisticas(mensajes_recibidos, total_bytes, &tiempo_inicio);
        }
    }
    
    // Estadísticas finales
    printf("\n=== Estadísticas Finales ===\n");
    mostrar_estadisticas(mensajes_recibidos, total_bytes, &tiempo_inicio);
    
    close(sockfd);
    printf("Servidor UDP cerrado correctamente\n");
    return 0;
}
