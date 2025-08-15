/**
 * @file cliente_prueba.c
 * @brief Cliente simple para probar el servidor TCP de eco - Ejercicio 090
 * @author Ejercicios de C
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER_TAMAÑO 1024
#define PUERTO_DEFECTO 8080

/**
 * @brief Imprimir ayuda del programa
 */
static void imprimir_ayuda(const char* programa) {
    printf("Uso: %s [opciones]\n\n", programa);
    printf("Cliente TCP simple para probar servidores de eco.\n\n");
    printf("Opciones:\n");
    printf("  -h HOST        Dirección del servidor (por defecto: 127.0.0.1)\n");
    printf("  -p PUERTO      Puerto del servidor (por defecto: %d)\n", PUERTO_DEFECTO);
    printf("  -m MENSAJE     Mensaje a enviar (por defecto: interactivo)\n");
    printf("  -n NUM         Número de veces a enviar el mensaje (por defecto: 1)\n");
    printf("  -i INTERVALO   Intervalo entre mensajes en ms (por defecto: 1000)\n");
    printf("  -t TIMEOUT     Timeout de conexión en segundos (por defecto: 5)\n");
    printf("  -v             Modo verbose (mostrar detalles)\n");
    printf("  -b             Modo benchmark (medir rendimiento)\n");
    printf("  --help         Mostrar esta ayuda\n\n");
    printf("Ejemplos:\n");
    printf("  %s                                    # Modo interactivo\n", programa);
    printf("  %s -m \"Hola servidor\"                 # Enviar mensaje específico\n", programa);
    printf("  %s -p 9090 -m \"Test\" -n 10          # 10 mensajes al puerto 9090\n", programa);
    printf("  %s -b -n 1000                        # Benchmark con 1000 mensajes\n", programa);
}

/**
 * @brief Conectar al servidor con timeout
 */
static int conectar_servidor(const char* host, int puerto, int timeout_seg, int verbose) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return -1;
    }
    
    // Configurar timeout
    if (timeout_seg > 0) {
        struct timeval timeout;
        timeout.tv_sec = timeout_seg;
        timeout.tv_usec = 0;
        
        if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("setsockopt SO_RCVTIMEO");
        }
        if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("setsockopt SO_SNDTIMEO");
        }
    }
    
    struct sockaddr_in direccion_servidor;
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(puerto);
    
    if (inet_aton(host, &direccion_servidor.sin_addr) == 0) {
        fprintf(stderr, "Dirección IP inválida: %s\n", host);
        close(socket_fd);
        return -1;
    }
    
    if (verbose) {
        printf("Conectando a %s:%d...\n", host, puerto);
    }
    
    if (connect(socket_fd, (struct sockaddr*)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        perror("connect");
        close(socket_fd);
        return -1;
    }
    
    if (verbose) {
        printf("✅ Conectado exitosamente\n");
    }
    
    return socket_fd;
}

/**
 * @brief Enviar mensaje y recibir respuesta
 */
static int transaccion_eco(int socket_fd, const char* mensaje, char* respuesta, 
                          size_t tam_respuesta, int verbose) {
    // Enviar mensaje
    size_t tam_mensaje = strlen(mensaje);
    ssize_t enviados = send(socket_fd, mensaje, tam_mensaje, 0);
    
    if (enviados < 0) {
        perror("send");
        return -1;
    }
    
    if ((size_t)enviados != tam_mensaje) {
        fprintf(stderr, "Advertencia: no se enviaron todos los bytes (%zd de %zu)\n",
                enviados, tam_mensaje);
    }
    
    if (verbose) {
        printf("📤 Enviado (%zd bytes): \"%s\"\n", enviados, mensaje);
    }
    
    // Recibir respuesta
    ssize_t recibidos = recv(socket_fd, respuesta, tam_respuesta - 1, 0);
    
    if (recibidos < 0) {
        perror("recv");
        return -1;
    }
    
    if (recibidos == 0) {
        printf("⚠️ Servidor cerró la conexión\n");
        return 0;
    }
    
    respuesta[recibidos] = '\0';
    
    if (verbose) {
        printf("📥 Recibido (%zd bytes): \"%s\"\n", recibidos, respuesta);
    }
    
    // Verificar que el eco es correcto
    if (strcmp(mensaje, respuesta) == 0) {
        if (verbose) printf("✅ Eco correcto\n");
        return 1;
    } else {
        printf("❌ Error: eco incorrecto\n");
        printf("   Enviado:  \"%s\"\n", mensaje);
        printf("   Recibido: \"%s\"\n", respuesta);
        return -1;
    }
}

/**
 * @brief Modo interactivo
 */
static void modo_interactivo(const char* host, int puerto, int timeout_seg, int verbose) {
    printf("\n=== CLIENTE TCP INTERACTIVO ===\n");
    printf("Conectando a %s:%d...\n", host, puerto);
    
    int socket_fd = conectar_servidor(host, puerto, timeout_seg, verbose);
    if (socket_fd < 0) {
        return;
    }
    
    printf("✅ Conectado al servidor. Escribe mensajes (vacío para salir):\n\n");
    
    char mensaje[BUFFER_TAMAÑO];
    char respuesta[BUFFER_TAMAÑO];
    int contador = 0;
    
    while (1) {
        printf("Mensaje %d> ", ++contador);
        fflush(stdout);
        
        if (!fgets(mensaje, sizeof(mensaje), stdin)) {
            break;
        }
        
        // Quitar salto de línea
        size_t len = strlen(mensaje);
        if (len > 0 && mensaje[len - 1] == '\n') {
            mensaje[len - 1] = '\0';
        }
        
        // Salir si mensaje vacío
        if (strlen(mensaje) == 0) {
            break;
        }
        
        int resultado = transaccion_eco(socket_fd, mensaje, respuesta, 
                                       sizeof(respuesta), verbose);
        if (resultado <= 0) {
            break;
        }
        
        if (!verbose) {
            printf("← %s\n", respuesta);
        }
        printf("\n");
    }
    
    printf("👋 Cerrando conexión...\n");
    close(socket_fd);
}

/**
 * @brief Modo mensaje único
 */
static void modo_mensaje_unico(const char* host, int puerto, const char* mensaje,
                              int num_veces, int intervalo_ms, int timeout_seg,
                              int verbose, int benchmark) {
    int socket_fd = conectar_servidor(host, puerto, timeout_seg, verbose);
    if (socket_fd < 0) {
        return;
    }
    
    char respuesta[BUFFER_TAMAÑO];
    int exitosos = 0;
    int errores = 0;
    
    struct timespec inicio, fin;
    if (benchmark) {
        clock_gettime(CLOCK_MONOTONIC, &inicio);
    }
    
    for (int i = 1; i <= num_veces; i++) {
        if (!benchmark && verbose) {
            printf("\n--- Mensaje %d/%d ---\n", i, num_veces);
        }
        
        int resultado = transaccion_eco(socket_fd, mensaje, respuesta, 
                                       sizeof(respuesta), verbose && !benchmark);
        
        if (resultado > 0) {
            exitosos++;
            if (!benchmark && !verbose) {
                printf("%d: OK - %s\n", i, respuesta);
            }
        } else {
            errores++;
            if (!benchmark) {
                printf("%d: ERROR\n", i);
            }
        }
        
        // Pausa entre mensajes (excepto el último)
        if (i < num_veces && intervalo_ms > 0) {
            usleep(intervalo_ms * 1000);
        }
    }
    
    if (benchmark) {
        clock_gettime(CLOCK_MONOTONIC, &fin);
        
        double tiempo_total = (fin.tv_sec - inicio.tv_sec) + 
                             (fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;
        
        printf("\n=== RESULTADOS DEL BENCHMARK ===\n");
        printf("Mensajes enviados: %d\n", num_veces);
        printf("Exitosos: %d\n", exitosos);
        printf("Errores: %d\n", errores);
        printf("Tiempo total: %.3f segundos\n", tiempo_total);
        if (tiempo_total > 0) {
            printf("Mensajes por segundo: %.2f\n", num_veces / tiempo_total);
            printf("Tiempo promedio por mensaje: %.3f ms\n", 
                   (tiempo_total * 1000) / num_veces);
        }
        printf("Tamaño del mensaje: %zu bytes\n", strlen(mensaje));
        printf("Throughput: %.2f KB/s\n", 
               (exitosos * strlen(mensaje) * 2 / 1024.0) / tiempo_total);
    } else {
        printf("\n✅ Completado: %d exitosos, %d errores\n", exitosos, errores);
    }
    
    close(socket_fd);
}

int main(int argc, char* argv[]) {
    // Configuración por defecto
    char host[256] = "127.0.0.1";
    int puerto = PUERTO_DEFECTO;
    char mensaje[BUFFER_TAMAÑO] = "";
    int num_veces = 1;
    int intervalo_ms = 1000;
    int timeout_seg = 5;
    int verbose = 0;
    int benchmark = 0;
    int modo_mensaje = 0;
    
    // Parsear argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            strncpy(host, argv[++i], sizeof(host) - 1);
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            puerto = atoi(argv[++i]);
            if (puerto <= 0 || puerto > 65535) {
                fprintf(stderr, "Puerto inválido: %d\n", puerto);
                return 1;
            }
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            strncpy(mensaje, argv[++i], sizeof(mensaje) - 1);
            modo_mensaje = 1;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            num_veces = atoi(argv[++i]);
            if (num_veces <= 0) {
                fprintf(stderr, "Número de veces inválido: %d\n", num_veces);
                return 1;
            }
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            intervalo_ms = atoi(argv[++i]);
            if (intervalo_ms < 0) {
                fprintf(stderr, "Intervalo inválido: %d\n", intervalo_ms);
                return 1;
            }
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeout_seg = atoi(argv[++i]);
            if (timeout_seg < 0) {
                fprintf(stderr, "Timeout inválido: %d\n", timeout_seg);
                return 1;
            }
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            benchmark = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            imprimir_ayuda(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Opción desconocida: %s\n", argv[i]);
            imprimir_ayuda(argv[0]);
            return 1;
        }
    }
    
    printf("🚀 Cliente TCP de Prueba - Ejercicio 090\n");
    printf("Servidor: %s:%d\n", host, puerto);
    
    if (modo_mensaje) {
        if (strlen(mensaje) == 0) {
            strcpy(mensaje, "Hola, servidor TCP");
        }
        printf("Mensaje: \"%s\" (%d veces)\n", mensaje, num_veces);
        if (benchmark) {
            printf("Modo benchmark activado\n");
        }
        printf("\n");
        
        modo_mensaje_unico(host, puerto, mensaje, num_veces, intervalo_ms,
                          timeout_seg, verbose, benchmark);
    } else {
        modo_interactivo(host, puerto, timeout_seg, verbose);
    }
    
    return 0;
}
