/**
 * @file servidor_prueba.c
 * @brief Servidor TCP simple para testing del cliente
 * @description Servidor minimalista que acepta conexiones TCP y responde
 *              con eco de los mensajes recibidos. Útil para probar el cliente
 *              sin necesidad de servidores externos.
 * @version 1.0
 * @date 2024
 * @author Estudiante de C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define PUERTO_DEFECTO 8080
#define BUFFER_MAXIMO 4096
#define MAX_CLIENTES 10

static volatile int servidor_activo = 1;

/**
 * @brief Manejador de señales para cierre limpio
 */
static void manejador_señal(int sig) {
    (void)sig;
    printf("\n🔴 Señal recibida, cerrando servidor...\n");
    servidor_activo = 0;
}

/**
 * @brief Obtener timestamp formateado
 */
static void obtener_timestamp(char *buffer, size_t tamaño) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, tamaño, "%H:%M:%S", tm_info);
}

/**
 * @brief Mostrar ayuda de uso
 */
static void mostrar_ayuda(const char *programa) {
    printf("Uso: %s [OPCIONES]\n", programa);
    printf("\nOpciones:\n");
    printf("  -p, --puerto PUERTO    Puerto donde ejecutar el servidor (defecto: %d)\n", PUERTO_DEFECTO);
    printf("  -h, --help            Mostrar esta ayuda\n");
    printf("  -v, --verbose         Modo verbose (mostrar más información)\n");
    printf("  -e, --eco             Modo eco simple (devuelve exactamente lo recibido)\n");
    printf("  -r, --respuesta TEXTO Respuesta personalizada para todos los mensajes\n");
    printf("\nEjemplos:\n");
    printf("  %s                    # Servidor en puerto 8080\n", programa);
    printf("  %s -p 9000           # Servidor en puerto 9000\n", programa);
    printf("  %s -v                # Servidor con logging detallado\n", programa);
    printf("  %s -r \"Hola cliente\" # Respuesta personalizada\n", programa);
    printf("\nPara probar el servidor:\n");
    printf("  telnet localhost 8080\n");
    printf("  nc localhost 8080\n");
    printf("  ./cliente_tcp_main\n");
}

/**
 * @brief Procesar conexión de cliente
 */
static void procesar_cliente(int cliente_fd, struct sockaddr_in *cliente_addr, 
                           int verbose, int modo_eco, const char *respuesta_personalizada) {
    char buffer[BUFFER_MAXIMO];
    char timestamp[20];
    char cliente_ip[INET_ADDRSTRLEN];
    
    // Obtener IP del cliente
    inet_ntop(AF_INET, &(cliente_addr->sin_addr), cliente_ip, INET_ADDRSTRLEN);
    
    obtener_timestamp(timestamp, sizeof(timestamp));
    
    if (verbose) {
        printf("[%s] 🔗 Cliente conectado desde %s:%d\n", 
               timestamp, cliente_ip, ntohs(cliente_addr->sin_port));
    }
    
    // Configurar timeout para recv
    struct timeval timeout;
    timeout.tv_sec = 30; // 30 segundos
    timeout.tv_usec = 0;
    setsockopt(cliente_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    while (servidor_activo) {
        ssize_t bytes_recibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_recibidos <= 0) {
            if (bytes_recibidos == 0) {
                if (verbose) {
                    obtener_timestamp(timestamp, sizeof(timestamp));
                    printf("[%s] 📴 Cliente %s desconectado\n", timestamp, cliente_ip);
                }
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                if (verbose) {
                    obtener_timestamp(timestamp, sizeof(timestamp));
                    printf("[%s] ❌ Error recibiendo de %s: %s\n", 
                           timestamp, cliente_ip, strerror(errno));
                }
            }
            break;
        }
        
        buffer[bytes_recibidos] = '\0';
        
        obtener_timestamp(timestamp, sizeof(timestamp));
        
        if (verbose) {
            printf("[%s] 📥 Recibido de %s (%zd bytes): \"%.*s%s\"\n", 
                   timestamp, cliente_ip, bytes_recibidos,
                   (int)(bytes_recibidos > 100 ? 100 : bytes_recibidos), buffer,
                   bytes_recibidos > 100 ? "..." : "");
        }
        
        // Preparar respuesta
        char respuesta[BUFFER_MAXIMO];
        
        if (respuesta_personalizada) {
            // Respuesta personalizada
            snprintf(respuesta, sizeof(respuesta), "%s", respuesta_personalizada);
        } else if (modo_eco) {
            // Eco simple
            snprintf(respuesta, sizeof(respuesta), "%s", buffer);
        } else {
            // Respuesta con formato
            snprintf(respuesta, sizeof(respuesta), 
                    "Servidor TCP: Recibido \"%s\" (%zd bytes)", buffer, bytes_recibidos);
        }
        
        // Enviar respuesta
        ssize_t bytes_enviados = send(cliente_fd, respuesta, strlen(respuesta), 0);
        
        if (bytes_enviados < 0) {
            if (verbose) {
                obtener_timestamp(timestamp, sizeof(timestamp));
                printf("[%s] ❌ Error enviando a %s: %s\n", 
                       timestamp, cliente_ip, strerror(errno));
            }
            break;
        }
        
        if (verbose) {
            printf("[%s] 📤 Enviado a %s (%zd bytes): \"%.*s%s\"\n", 
                   timestamp, cliente_ip, bytes_enviados,
                   (int)(strlen(respuesta) > 100 ? 100 : strlen(respuesta)), respuesta,
                   strlen(respuesta) > 100 ? "..." : "");
        }

        // Continuar procesando mensajes del mismo cliente hasta que cierre o haya error
    }
    
    close(cliente_fd);
}

/**
 * @brief Función principal del servidor
 */
int main(int argc, char *argv[]) {
    int puerto = PUERTO_DEFECTO;
    int verbose = 0;
    int modo_eco = 0;
    char *respuesta_personalizada = NULL;
    
    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            mostrar_ayuda(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--eco") == 0) {
            modo_eco = 1;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--puerto") == 0) {
            if (i + 1 < argc) {
                puerto = atoi(argv[++i]);
                if (puerto <= 0 || puerto > 65535) {
                    fprintf(stderr, "❌ Error: Puerto inválido %d\n", puerto);
                    return 1;
                }
            } else {
                fprintf(stderr, "❌ Error: Opción %s requiere un valor\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--respuesta") == 0) {
            if (i + 1 < argc) {
                respuesta_personalizada = argv[++i];
            } else {
                fprintf(stderr, "❌ Error: Opción %s requiere un valor\n", argv[i]);
                return 1;
            }
        } else {
            fprintf(stderr, "❌ Error: Opción desconocida %s\n", argv[i]);
            fprintf(stderr, "Use %s --help para ver las opciones disponibles.\n", argv[0]);
            return 1;
        }
    }
    
    // Configurar manejadores de señales
    signal(SIGINT, manejador_señal);
    signal(SIGTERM, manejador_señal);
    signal(SIGPIPE, SIG_IGN);
    
    printf("🚀 Iniciando Servidor TCP de Prueba\n");
    printf("=====================================\n");
    printf("Puerto: %d\n", puerto);
    printf("Modo verbose: %s\n", verbose ? "Activado" : "Desactivado");
    printf("Modo eco: %s\n", modo_eco ? "Activado" : "Desactivado");
    if (respuesta_personalizada) {
        printf("Respuesta personalizada: \"%s\"\n", respuesta_personalizada);
    }
    printf("=====================================\n\n");
    
    // Crear socket del servidor
    int servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_fd < 0) {
        perror("❌ Error creando socket");
        return 1;
    }
    
    // Configurar reutilización de dirección
    int opt = 1;
    if (setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("❌ Error configurando SO_REUSEADDR");
        close(servidor_fd);
        return 1;
    }
    
    // Configurar dirección del servidor
    struct sockaddr_in direccion_servidor;
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(puerto);
    
    // Bind
    if (bind(servidor_fd, (struct sockaddr*)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        perror("❌ Error en bind");
        close(servidor_fd);
        return 1;
    }
    
    // Listen
    if (listen(servidor_fd, MAX_CLIENTES) < 0) {
        perror("❌ Error en listen");
        close(servidor_fd);
        return 1;
    }
    
    printf("✅ Servidor TCP escuchando en puerto %d\n", puerto);
    printf("🔗 Esperando conexiones de clientes...\n");
    printf("💡 Presiona Ctrl+C para detener el servidor\n\n");
    
    if (verbose) {
        printf("📋 Modo verbose activado - se mostrarán todos los mensajes\n\n");
    }
    
    // Loop principal del servidor
    while (servidor_activo) {
        struct sockaddr_in direccion_cliente;
        socklen_t tamaño_direccion = sizeof(direccion_cliente);
        
        // Configurar timeout para accept
        fd_set read_fds;
        struct timeval timeout;
        FD_ZERO(&read_fds);
        FD_SET(servidor_fd, &read_fds);
        timeout.tv_sec = 1; // 1 segundo
        timeout.tv_usec = 0;
        
        int resultado = select(servidor_fd + 1, &read_fds, NULL, NULL, &timeout);
        
        if (resultado < 0) {
            if (errno != EINTR) {
                perror("❌ Error en select");
            }
            break;
        } else if (resultado == 0) {
            // Timeout - continuar loop para verificar servidor_activo
            continue;
        }
        
        if (!FD_ISSET(servidor_fd, &read_fds)) {
            continue;
        }
        
        int cliente_fd = accept(servidor_fd, (struct sockaddr*)&direccion_cliente, &tamaño_direccion);
        
        if (cliente_fd < 0) {
            if (errno != EINTR && servidor_activo) {
                perror("❌ Error en accept");
            }
            continue;
        }
        
        // Procesar cliente (versión simple sin fork/threads)
        procesar_cliente(cliente_fd, &direccion_cliente, verbose, modo_eco, respuesta_personalizada);
    }
    
    // Limpieza
    close(servidor_fd);
    
    printf("\n🔴 Servidor TCP detenido\n");
    printf("👋 ¡Gracias por usar el servidor de prueba!\n");
    
    return 0;
}
