/**
 * @file emisor_udp.c
 * @brief Programa emisor UDP - Cliente que envía datagramas
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Este programa implementa un cliente UDP que envía mensajes a un servidor.
 * Demuestra el uso de sendto() para comunicación sin conexión.
 * 
 * Compilación:
 *   gcc -o emisor_udp emisor_udp.c
 * 
 * Uso:
 *   ./emisor_udp [dirección_ip] [puerto] [mensaje]
 *   ./emisor_udp 127.0.0.1 9090 "Hola UDP"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>

#define PUERTO_DEFAULT 9090
#define IP_DEFAULT "127.0.0.1"
#define MENSAJE_DEFAULT "Hola desde el cliente UDP"
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 5

/**
 * @brief Muestra la ayuda del programa
 * @param programa_nombre Nombre del programa
 */
void mostrar_ayuda(const char* programa_nombre) {
    printf("Uso: %s [opciones] [dirección_ip] [puerto] [mensaje]\n", programa_nombre);
    printf("\nOpciones:\n");
    printf("  -h, --help     Mostrar esta ayuda\n");
    printf("  -v, --verbose  Modo verboso\n");
    printf("  -t <segundos>  Timeout para envío (default: %d)\n", TIMEOUT_SEC);
    printf("  -n <número>    Número de mensajes a enviar (default: 1)\n");
    printf("  -i <intervalo> Intervalo entre mensajes en ms (default: 0)\n");
    printf("\nEjemplos:\n");
    printf("  %s                              # Usar valores por defecto\n", programa_nombre);
    printf("  %s 192.168.1.100 8080 \"Hola\"    # IP, puerto y mensaje específicos\n", programa_nombre);
    printf("  %s -n 5 -i 1000                 # Enviar 5 mensajes con intervalo de 1s\n", programa_nombre);
    printf("  %s -v -t 10                     # Modo verboso con timeout de 10s\n", programa_nombre);
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
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error configurando timeout de envío");
        return -1;
    }
    
    return 0;
}

/**
 * @brief Envía un mensaje UDP
 * @param sockfd Descriptor del socket
 * @param servidor Estructura de dirección del servidor
 * @param mensaje Mensaje a enviar
 * @param verbose Modo verboso
 * @return Número de bytes enviados, -1 si error
 */
ssize_t enviar_mensaje(int sockfd, struct sockaddr_in* servidor, const char* mensaje, int verbose) {
    ssize_t bytes_enviados;
    struct timeval inicio, fin;
    
    if (verbose) {
        gettimeofday(&inicio, NULL);
        printf("Enviando mensaje: \"%s\" (%zu bytes)\n", mensaje, strlen(mensaje));
    }
    
    bytes_enviados = sendto(sockfd, mensaje, strlen(mensaje), 0, 
                           (struct sockaddr*)servidor, sizeof(*servidor));
    
    if (bytes_enviados < 0) {
        perror("Error en sendto");
        return -1;
    }
    
    if (verbose) {
        gettimeofday(&fin, NULL);
        long tiempo_us = (fin.tv_sec - inicio.tv_sec) * 1000000 + (fin.tv_usec - inicio.tv_usec);
        printf("Mensaje enviado exitosamente: %zd bytes en %ld μs\n", bytes_enviados, tiempo_us);
    }
    
    return bytes_enviados;
}

/**
 * @brief Función principal del emisor UDP
 */
int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servidor;
    const char* ip = IP_DEFAULT;
    int puerto = PUERTO_DEFAULT;
    const char* mensaje = MENSAJE_DEFAULT;
    int verbose = 0;
    int timeout_sec = TIMEOUT_SEC;
    int num_mensajes = 1;
    int intervalo_ms = 0;
    
    // Procesar argumentos de línea de comandos
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            mostrar_ayuda(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
            i++;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeout_sec = atoi(argv[i + 1]);
            if (timeout_sec <= 0) {
                fprintf(stderr, "Error: Timeout debe ser positivo\n");
                return 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            num_mensajes = atoi(argv[i + 1]);
            if (num_mensajes <= 0) {
                fprintf(stderr, "Error: Número de mensajes debe ser positivo\n");
                return 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            intervalo_ms = atoi(argv[i + 1]);
            if (intervalo_ms < 0) {
                fprintf(stderr, "Error: Intervalo debe ser no negativo\n");
                return 1;
            }
            i += 2;
        } else {
            // Argumentos posicionales: IP, puerto, mensaje
            if (i < argc) ip = argv[i++];
            if (i < argc) puerto = atoi(argv[i++]);
            if (i < argc) mensaje = argv[i++];
            break;
        }
    }
    
    if (verbose) {
        printf("=== Cliente UDP ===\n");
        printf("Servidor: %s:%d\n", ip, puerto);
        printf("Mensaje: \"%s\"\n", mensaje);
        printf("Número de mensajes: %d\n", num_mensajes);
        printf("Intervalo: %d ms\n", intervalo_ms);
        printf("Timeout: %d s\n", timeout_sec);
        printf("==================\n\n");
    }
    
    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear socket UDP");
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
    servidor.sin_port = htons(puerto);
    
    if (inet_pton(AF_INET, ip, &servidor.sin_addr) <= 0) {
        fprintf(stderr, "Error: Dirección IP inválida: %s\n", ip);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    // Estadísticas
    int mensajes_enviados = 0;
    ssize_t total_bytes = 0;
    struct timeval inicio_total, fin_total;
    
    gettimeofday(&inicio_total, NULL);
    
    // Enviar mensajes
    for (int j = 0; j < num_mensajes; j++) {
        char mensaje_numerado[BUFFER_SIZE];
        
        if (num_mensajes > 1) {
            snprintf(mensaje_numerado, sizeof(mensaje_numerado), "%s (mensaje %d/%d)", 
                    mensaje, j + 1, num_mensajes);
        } else {
            strncpy(mensaje_numerado, mensaje, sizeof(mensaje_numerado) - 1);
            mensaje_numerado[sizeof(mensaje_numerado) - 1] = '\0';
        }
        
        ssize_t bytes_enviados = enviar_mensaje(sockfd, &servidor, mensaje_numerado, verbose);
        
        if (bytes_enviados > 0) {
            mensajes_enviados++;
            total_bytes += bytes_enviados;
        } else {
            fprintf(stderr, "Error enviando mensaje %d/%d\n", j + 1, num_mensajes);
            break;
        }
        
        // Intervalo entre mensajes (excepto el último)
        if (j < num_mensajes - 1 && intervalo_ms > 0) {
            usleep(intervalo_ms * 1000);
        }
    }
    
    gettimeofday(&fin_total, NULL);
    
    // Mostrar estadísticas finales
    if (verbose || num_mensajes > 1) {
        long tiempo_total_ms = (fin_total.tv_sec - inicio_total.tv_sec) * 1000 + 
                              (fin_total.tv_usec - inicio_total.tv_usec) / 1000;
        
        printf("\n=== Estadísticas ===\n");
        printf("Mensajes enviados: %d/%d\n", mensajes_enviados, num_mensajes);
        printf("Total de bytes: %zd\n", total_bytes);
        printf("Tiempo total: %ld ms\n", tiempo_total_ms);
        
        if (mensajes_enviados > 0 && tiempo_total_ms > 0) {
            printf("Promedio: %.2f mensajes/s\n", (double)mensajes_enviados * 1000.0 / tiempo_total_ms);
            printf("Throughput: %.2f bytes/s\n", (double)total_bytes * 1000.0 / tiempo_total_ms);
        }
        printf("==================\n");
    } else {
        printf("Mensaje enviado al servidor UDP %s:%d\n", ip, puerto);
    }
    
    close(sockfd);
    return (mensajes_enviados == num_mensajes) ? 0 : 1;
}
