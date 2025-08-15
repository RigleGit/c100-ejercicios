/**
 * @file servidor_tcp.c
 * @brief Implementación del servidor TCP de eco - Ejercicio 090
 * @author Ejercicios de C
 * @date 2025
 */

#include "../include/servidor_tcp.h"
#include <netinet/tcp.h>

// ============================================================================
// VARIABLES GLOBALES PARA MANEJO DE SEÑALES
// ============================================================================

static servidor_tcp_t* servidor_global = NULL;

// ============================================================================
// MANEJADORES DE SEÑALES
// ============================================================================

/**
 * @brief Manejador de señales para shutdown graceful
 */
static void manejador_shutdown(int signal) {
    printf("\n[SERVIDOR] Señal %d recibida, iniciando shutdown graceful...\n", signal);
    if (servidor_global) {
        servidor_global->shutdown_solicitado = 1;
        servidor_global->ejecutandose = 0;
    }
}

/**
 * @brief Manejador para SIGCHLD (procesos hijos terminados)
 */
static void manejador_sigchld(int signal) {
    // Recoger procesos hijos zombi
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// ============================================================================
// FUNCIONES DE UTILIDAD INTERNAS
// ============================================================================

/**
 * @brief Configurar un socket con opciones estándar
 */
static resultado_servidor_t configurar_socket_opciones(int socket_fd, const config_servidor_t* config) {
    int opcion = 1;
    
    // SO_REUSEADDR: reutilizar dirección inmediatamente
    if (config->reusar_puerto) {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion)) < 0) {
            perror("setsockopt SO_REUSEADDR");
            return SERVIDOR_ERROR_SOCKET;
        }
    }
    
    // SO_KEEPALIVE: mantener conexión viva
    if (config->keep_alive) {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opcion, sizeof(opcion)) < 0) {
            perror("setsockopt SO_KEEPALIVE");
            return SERVIDOR_ERROR_SOCKET;
        }
    }
    
    // TCP_NODELAY: deshabilitar algoritmo de Nagle
    if (config->no_delay) {
        if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &opcion, sizeof(opcion)) < 0) {
            perror("setsockopt TCP_NODELAY");
            return SERVIDOR_ERROR_SOCKET;
        }
    }
    
    return SERVIDOR_EXITO;
}

/**
 * @brief Configurar timeout para un socket
 */
static resultado_servidor_t configurar_timeout_socket(int socket_fd, int timeout_seg) {
    struct timeval timeout;
    timeout.tv_sec = timeout_seg;
    timeout.tv_usec = 0;
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt SO_RCVTIMEO");
        return SERVIDOR_ERROR_SOCKET;
    }
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt SO_SNDTIMEO");
        return SERVIDOR_ERROR_SOCKET;
    }
    
    return SERVIDOR_EXITO;
}

/**
 * @brief Actualizar estadísticas del servidor de forma thread-safe
 */
static void actualizar_estadisticas(servidor_tcp_t* servidor, const char* operacion, 
                                   size_t bytes, double tiempo_ms) {
    if (!servidor) return;
    
    pthread_mutex_lock(&servidor->mutex_stats);
    
    if (strcmp(operacion, "conexion") == 0) {
        servidor->stats.conexiones_totales++;
        servidor->stats.conexiones_activas++;
    } else if (strcmp(operacion, "desconexion") == 0) {
        servidor->stats.conexiones_activas--;
    } else if (strcmp(operacion, "mensaje") == 0) {
        servidor->stats.mensajes_procesados++;
        servidor->stats.bytes_transferidos += bytes;
        if (tiempo_ms > 0) {
            double total_tiempo = servidor->stats.tiempo_promedio_ms * (servidor->stats.mensajes_procesados - 1);
            servidor->stats.tiempo_promedio_ms = (total_tiempo + tiempo_ms) / servidor->stats.mensajes_procesados;
        }
    } else if (strcmp(operacion, "error") == 0) {
        servidor->stats.errores_red++;
    } else if (strcmp(operacion, "timeout") == 0) {
        servidor->stats.timeouts++;
    }
    
    pthread_mutex_unlock(&servidor->mutex_stats);
}

// ============================================================================
// IMPLEMENTACIÓN DE LA API PRINCIPAL
// ============================================================================

servidor_tcp_t* crear_servidor(const config_servidor_t* config) {
    servidor_tcp_t* servidor = malloc(sizeof(servidor_tcp_t));
    if (!servidor) {
        perror("malloc servidor");
        return NULL;
    }
    
    // Inicializar estructura
    memset(servidor, 0, sizeof(servidor_tcp_t));
    
    // Configuración
    if (config) {
        servidor->config = *config;
    } else {
        config_servidor_t config_defecto = CONFIG_SERVIDOR_DEFECTO;
        servidor->config = config_defecto;
    }
    
    // Asignar memoria para clientes
    servidor->clientes = malloc(sizeof(info_cliente_t) * servidor->config.max_clientes);
    if (!servidor->clientes) {
        perror("malloc clientes");
        free(servidor);
        return NULL;
    }
    memset(servidor->clientes, 0, sizeof(info_cliente_t) * servidor->config.max_clientes);
    
    // Inicializar mutex para estadísticas
    if (pthread_mutex_init(&servidor->mutex_stats, NULL) != 0) {
        perror("pthread_mutex_init");
        free(servidor->clientes);
        free(servidor);
        return NULL;
    }
    
    // Inicializar estadísticas
    servidor->stats.tiempo_inicio = time(NULL);
    servidor->socket_servidor = -1;
    servidor->ejecutandose = 0;
    servidor->shutdown_solicitado = 0;
    
    if (servidor->config.verbose) {
        printf("[SERVIDOR] Creado en modo %s, puerto %d\n",
               servidor->config.modo == MODO_SECUENCIAL ? "SECUENCIAL" :
               servidor->config.modo == MODO_FORK ? "FORK" :
               servidor->config.modo == MODO_THREAD ? "THREAD" :
               servidor->config.modo == MODO_SELECT ? "SELECT" : "POLL",
               servidor->config.puerto);
    }
    
    return servidor;
}

void destruir_servidor(servidor_tcp_t* servidor) {
    if (!servidor) return;
    
    if (servidor->config.verbose) {
        printf("[SERVIDOR] Destruyendo servidor...\n");
    }
    
    // Cerrar socket si está abierto
    if (servidor->socket_servidor >= 0) {
        close(servidor->socket_servidor);
    }
    
    // Desconectar todos los clientes activos
    for (int i = 0; i < servidor->config.max_clientes; i++) {
        if (servidor->clientes[i].activo) {
            desconectar_cliente(servidor, &servidor->clientes[i]);
        }
    }
    
    // Destruir mutex
    pthread_mutex_destroy(&servidor->mutex_stats);
    
    // Liberar memoria
    free(servidor->clientes);
    free(servidor);
}

resultado_servidor_t iniciar_servidor(servidor_tcp_t* servidor) {
    if (!servidor) return SERVIDOR_ERROR_PARAMETRO;
    
    // Crear socket
    servidor->socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor->socket_servidor < 0) {
        perror("socket");
        return SERVIDOR_ERROR_SOCKET;
    }
    
    // Configurar opciones del socket
    resultado_servidor_t resultado = configurar_socket_opciones(servidor->socket_servidor, &servidor->config);
    if (resultado != SERVIDOR_EXITO) {
        close(servidor->socket_servidor);
        return resultado;
    }
    
    // Configurar dirección del servidor
    struct sockaddr_in direccion_servidor;
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(servidor->config.puerto);
    
    if (strcmp(servidor->config.host, "0.0.0.0") == 0) {
        direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    } else {
        if (inet_aton(servidor->config.host, &direccion_servidor.sin_addr) == 0) {
            fprintf(stderr, "Dirección IP inválida: %s\n", servidor->config.host);
            close(servidor->socket_servidor);
            return SERVIDOR_ERROR_PARAMETRO;
        }
    }
    
    // Bind
    if (bind(servidor->socket_servidor, (struct sockaddr*)&direccion_servidor, 
             sizeof(direccion_servidor)) < 0) {
        perror("bind");
        close(servidor->socket_servidor);
        return SERVIDOR_ERROR_BIND;
    }
    
    // Listen
    if (listen(servidor->socket_servidor, servidor->config.backlog) < 0) {
        perror("listen");
        close(servidor->socket_servidor);
        return SERVIDOR_ERROR_LISTEN;
    }
    
    // Instalar manejadores de señales
    instalar_manejadores_senales(servidor);
    
    printf("[SERVIDOR] Escuchando en %s:%d (backlog: %d, modo: %s)\n",
           servidor->config.host, servidor->config.puerto, servidor->config.backlog,
           servidor->config.modo == MODO_SECUENCIAL ? "SECUENCIAL" :
           servidor->config.modo == MODO_FORK ? "FORK" :
           servidor->config.modo == MODO_THREAD ? "THREAD" :
           servidor->config.modo == MODO_SELECT ? "SELECT" : "POLL");
    
    servidor->ejecutandose = 1;
    return SERVIDOR_EXITO;
}

resultado_servidor_t ejecutar_servidor(servidor_tcp_t* servidor) {
    if (!servidor || !servidor->ejecutandose) {
        return SERVIDOR_ERROR_PARAMETRO;
    }
    
    // Ejecutar según el modo configurado
    switch (servidor->config.modo) {
        case MODO_SECUENCIAL:
            return servidor_modo_secuencial(servidor);
        case MODO_FORK:
            return servidor_modo_fork(servidor);
        case MODO_THREAD:
            return servidor_modo_thread(servidor);
        case MODO_SELECT:
            return servidor_modo_select(servidor);
        case MODO_POLL:
            // TODO: Implementar modo poll
            fprintf(stderr, "Modo POLL no implementado aún\n");
            return SERVIDOR_ERROR_SISTEMA;
        default:
            fprintf(stderr, "Modo de servidor desconocido: %d\n", servidor->config.modo);
            return SERVIDOR_ERROR_PARAMETRO;
    }
}

void detener_servidor(servidor_tcp_t* servidor) {
    if (!servidor) return;
    
    servidor->shutdown_solicitado = 1;
    servidor->ejecutandose = 0;
    
    if (servidor->config.verbose) {
        printf("[SERVIDOR] Shutdown solicitado\n");
    }
}

// ============================================================================
// GESTIÓN DE CLIENTES
// ============================================================================

resultado_servidor_t aceptar_cliente(servidor_tcp_t* servidor, info_cliente_t* info_cliente) {
    if (!servidor || !info_cliente) return SERVIDOR_ERROR_PARAMETRO;
    
    socklen_t tam_direccion = sizeof(info_cliente->direccion);
    info_cliente->socket_fd = accept(servidor->socket_servidor, 
                                   (struct sockaddr*)&info_cliente->direccion, 
                                   &tam_direccion);
    
    if (info_cliente->socket_fd < 0) {
        if (errno == EINTR) {
            return SERVIDOR_SHUTDOWN; // Interrupción por señal
        }
        perror("accept");
        actualizar_estadisticas(servidor, "error", 0, 0);
        return SERVIDOR_ERROR_ACCEPT;
    }
    
    // Configurar timeout para el cliente
    if (servidor->config.timeout_cliente_seg > 0) {
        configurar_timeout_socket(info_cliente->socket_fd, servidor->config.timeout_cliente_seg);
    }
    
    // Evitar SIGPIPE en envíos hacia este cliente (macOS)
    #ifdef SO_NOSIGPIPE
    int opt = 1;
    setsockopt(info_cliente->socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt));
    #endif
    
    // Inicializar información del cliente
    info_cliente->tiempo_conexion = time(NULL);
    info_cliente->mensajes_procesados = 0;
    info_cliente->bytes_recibidos = 0;
    info_cliente->bytes_enviados = 0;
    info_cliente->activo = 1;
    
    // Actualizar estadísticas
    actualizar_estadisticas(servidor, "conexion", 0, 0);
    
    if (servidor->config.verbose) {
        char ip_cliente[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &info_cliente->direccion.sin_addr, ip_cliente, INET_ADDRSTRLEN);
        printf("[SERVIDOR] Cliente conectado desde %s:%d (fd: %d)\n",
               ip_cliente, ntohs(info_cliente->direccion.sin_port), info_cliente->socket_fd);
    }
    
    return SERVIDOR_EXITO;
}

resultado_servidor_t procesar_cliente_eco(servidor_tcp_t* servidor, info_cliente_t* cliente) {
    if (!servidor || !cliente) return SERVIDOR_ERROR_PARAMETRO;
    
    char buffer[servidor->config.buffer_size];
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    // Recibir mensaje del cliente (manejar EINTR)
    ssize_t bytes_leidos;
    do {
        bytes_leidos = recv(cliente->socket_fd, buffer, sizeof(buffer) - 1, 0);
    } while (bytes_leidos < 0 && errno == EINTR);
    
    if (bytes_leidos < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            actualizar_estadisticas(servidor, "timeout", 0, 0);
            return SERVIDOR_ERROR_TIMEOUT;
        }
        perror("recv");
        actualizar_estadisticas(servidor, "error", 0, 0);
        return SERVIDOR_ERROR_RECV;
    }
    
    if (bytes_leidos == 0) {
        // Cierre limpio del lado del cliente
        return SERVIDOR_SHUTDOWN;
    }
    
    buffer[bytes_leidos] = '\0';
    cliente->bytes_recibidos += bytes_leidos;
    
    if (servidor->config.verbose) {
        printf("[SERVIDOR] Recibido de fd:%d (%zd bytes): \"%s\"\n", 
               cliente->socket_fd, bytes_leidos, buffer);
    }
    
    // Enviar eco del mensaje (evitar señales y manejar EINTR)
    ssize_t bytes_enviados;
    do {
        #ifdef MSG_NOSIGNAL
        bytes_enviados = send(cliente->socket_fd, buffer, (size_t)bytes_leidos, MSG_NOSIGNAL);
        #else
        bytes_enviados = send(cliente->socket_fd, buffer, (size_t)bytes_leidos, 0);
        #endif
    } while (bytes_enviados < 0 && errno == EINTR);
    
    if (bytes_enviados < 0) {
        if (errno == EPIPE) {
            // Cliente cerró; no tratar como crash: desconectar educadamente
            return SERVIDOR_SHUTDOWN;
        }
        perror("send");
        actualizar_estadisticas(servidor, "error", 0, 0);
        return SERVIDOR_ERROR_SEND;
    }
    
    if (bytes_enviados != bytes_leidos) {
        // Intentar enviar el remanente una vez
        size_t restante = (size_t)bytes_leidos - (size_t)bytes_enviados;
        const char* ptr = buffer + bytes_enviados;
        ssize_t extra = 0;
        if (restante > 0) {
            #ifdef MSG_NOSIGNAL
            extra = send(cliente->socket_fd, ptr, restante, MSG_NOSIGNAL);
            #else
            extra = send(cliente->socket_fd, ptr, restante, 0);
            #endif
            if (extra > 0) {
                bytes_enviados += extra;
            }
        }
    }
    
    cliente->bytes_enviados += bytes_enviados;
    cliente->mensajes_procesados++;
    
    // Calcular tiempo de procesamiento
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_ms = (fin.tv_sec - inicio.tv_sec) * 1000.0 + 
                       (fin.tv_nsec - inicio.tv_nsec) / 1000000.0;
    
    // Actualizar estadísticas
    actualizar_estadisticas(servidor, "mensaje", bytes_leidos + bytes_enviados, tiempo_ms);
    
    if (servidor->config.verbose) {
        printf("[SERVIDOR] Eco enviado a fd:%d (%zd bytes, %.2f ms)\n", 
               cliente->socket_fd, bytes_enviados, tiempo_ms);
    }
    
    return SERVIDOR_EXITO;
}

void desconectar_cliente(servidor_tcp_t* servidor, info_cliente_t* cliente) {
    if (!cliente || cliente->socket_fd < 0) return;
    
    if (servidor && servidor->config.verbose) {
        printf("[SERVIDOR] Desconectando cliente fd:%d (%d mensajes, %zu bytes R/%zu bytes S)\n",
               cliente->socket_fd, cliente->mensajes_procesados, 
               cliente->bytes_recibidos, cliente->bytes_enviados);
    }
    
    close(cliente->socket_fd);
    cliente->socket_fd = -1;
    cliente->activo = 0;
    
    if (servidor) {
        actualizar_estadisticas(servidor, "desconexion", 0, 0);
    }
}

// ============================================================================
// MODOS DE OPERACIÓN
// ============================================================================

resultado_servidor_t servidor_modo_secuencial(servidor_tcp_t* servidor) {
    if (!servidor) return SERVIDOR_ERROR_PARAMETRO;
    
    printf("[SERVIDOR] Ejecutando en modo SECUENCIAL\n");
    
    while (servidor->ejecutandose && !servidor->shutdown_solicitado) {
        info_cliente_t cliente;
        memset(&cliente, 0, sizeof(cliente));
        
        // Aceptar cliente
        resultado_servidor_t resultado = aceptar_cliente(servidor, &cliente);
        if (resultado == SERVIDOR_SHUTDOWN) {
            break; // Shutdown graceful
        }
        if (resultado != SERVIDOR_EXITO) {
            continue; // Intentar con el siguiente cliente
        }
        
        // Procesar múltiples mensajes del mismo cliente
        while (servidor->ejecutandose && !servidor->shutdown_solicitado) {
            resultado = procesar_cliente_eco(servidor, &cliente);
            if (resultado == SERVIDOR_SHUTDOWN || resultado == SERVIDOR_ERROR_TIMEOUT) {
                break; // Cliente se desconectó o timeout
            }
            if (resultado != SERVIDOR_EXITO) {
                fprintf(stderr, "Error procesando cliente: %s\n", servidor_strerror(resultado));
                break;
            }
        }
        
        // Desconectar cliente
        desconectar_cliente(servidor, &cliente);
    }
    
    return SERVIDOR_EXITO;
}

resultado_servidor_t servidor_modo_fork(servidor_tcp_t* servidor) {
    if (!servidor) return SERVIDOR_ERROR_PARAMETRO;
    
    printf("[SERVIDOR] Ejecutando en modo FORK\n");
    
    while (servidor->ejecutandose && !servidor->shutdown_solicitado) {
        info_cliente_t cliente;
        memset(&cliente, 0, sizeof(cliente));
        
        // Aceptar cliente
        resultado_servidor_t resultado = aceptar_cliente(servidor, &cliente);
        if (resultado == SERVIDOR_SHUTDOWN) {
            break;
        }
        if (resultado != SERVIDOR_EXITO) {
            continue;
        }
        
        // Fork para manejar el cliente
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            desconectar_cliente(servidor, &cliente);
            continue;
        }
        
        if (pid == 0) {
            // Proceso hijo - manejar cliente
            close(servidor->socket_servidor); // No necesario en el hijo
            
            while (1) {
                resultado_servidor_t res = procesar_cliente_eco(servidor, &cliente);
                if (res == SERVIDOR_SHUTDOWN || res == SERVIDOR_ERROR_TIMEOUT) {
                    break;
                }
                if (res != SERVIDOR_EXITO) {
                    fprintf(stderr, "Error en proceso hijo: %s\n", servidor_strerror(res));
                    break;
                }
            }
            
            desconectar_cliente(servidor, &cliente);
            exit(0);
        } else {
            // Proceso padre
            cliente.pid_proceso = pid;
            close(cliente.socket_fd); // El padre no necesita el socket del cliente
        }
    }
    
    return SERVIDOR_EXITO;
}

/**
 * @brief Función ejecutada por cada thread para manejar un cliente
 */
static void* thread_cliente(void* arg) {
    info_cliente_t* cliente = (info_cliente_t*)arg;
    servidor_tcp_t* servidor = cliente->activo ? servidor_global : NULL; // Acceso global (no ideal, pero funcional)
    
    if (!servidor || !cliente) {
        pthread_exit(NULL);
    }
    
    while (1) {
        resultado_servidor_t resultado = procesar_cliente_eco(servidor, cliente);
        if (resultado == SERVIDOR_SHUTDOWN || resultado == SERVIDOR_ERROR_TIMEOUT) {
            break;
        }
        if (resultado != SERVIDOR_EXITO) {
            fprintf(stderr, "Error en thread: %s\n", servidor_strerror(resultado));
            break;
        }
    }
    
    desconectar_cliente(servidor, cliente);
    pthread_exit(NULL);
}

resultado_servidor_t servidor_modo_thread(servidor_tcp_t* servidor) {
    if (!servidor) return SERVIDOR_ERROR_PARAMETRO;
    
    printf("[SERVIDOR] Ejecutando en modo THREAD\n");
    servidor_global = servidor; // Para acceso desde threads (solución simple)
    
    while (servidor->ejecutandose && !servidor->shutdown_solicitado) {
        // Buscar slot libre para cliente
        int slot_libre = -1;
        for (int i = 0; i < servidor->config.max_clientes; i++) {
            if (!servidor->clientes[i].activo) {
                slot_libre = i;
                break;
            }
        }
        
        if (slot_libre == -1) {
            // No hay slots libres, esperar un poco
            usleep(10000); // 10ms
            continue;
        }
        
        // Aceptar cliente
        resultado_servidor_t resultado = aceptar_cliente(servidor, &servidor->clientes[slot_libre]);
        if (resultado == SERVIDOR_SHUTDOWN) {
            break;
        }
        if (resultado != SERVIDOR_EXITO) {
            continue;
        }
        
        // Crear thread para manejar el cliente
        if (pthread_create(&servidor->clientes[slot_libre].thread_id, NULL, 
                          thread_cliente, &servidor->clientes[slot_libre]) != 0) {
            perror("pthread_create");
            desconectar_cliente(servidor, &servidor->clientes[slot_libre]);
            continue;
        }
        
        // Detach del thread para limpieza automática
        pthread_detach(servidor->clientes[slot_libre].thread_id);
    }
    
    return SERVIDOR_EXITO;
}

resultado_servidor_t servidor_modo_select(servidor_tcp_t* servidor) {
    if (!servidor) return SERVIDOR_ERROR_PARAMETRO;
    
    printf("[SERVIDOR] Ejecutando en modo SELECT\n");
    
    fd_set master_set, read_set;
    int max_fd = servidor->socket_servidor;
    struct timeval timeout;
    
    FD_ZERO(&master_set);
    FD_SET(servidor->socket_servidor, &master_set);
    
    while (servidor->ejecutandose && !servidor->shutdown_solicitado) {
        read_set = master_set;
        timeout.tv_sec = 1;  // Timeout de 1 segundo
        timeout.tv_usec = 0;
        
        int actividad = select(max_fd + 1, &read_set, NULL, NULL, &timeout);
        
        if (actividad < 0) {
            if (errno == EINTR) {
                continue; // Señal recibida
            }
            perror("select");
            break;
        }
        
        if (actividad == 0) {
            continue; // Timeout
        }
        
        // Verificar socket del servidor para nuevas conexiones
        if (FD_ISSET(servidor->socket_servidor, &read_set)) {
            info_cliente_t nuevo_cliente;
            memset(&nuevo_cliente, 0, sizeof(nuevo_cliente));
            
            resultado_servidor_t resultado = aceptar_cliente(servidor, &nuevo_cliente);
            if (resultado == SERVIDOR_EXITO) {
                // Buscar slot libre
                int slot_libre = -1;
                for (int i = 0; i < servidor->config.max_clientes; i++) {
                    if (!servidor->clientes[i].activo) {
                        slot_libre = i;
                        break;
                    }
                }
                
                if (slot_libre != -1) {
                    servidor->clientes[slot_libre] = nuevo_cliente;
                    FD_SET(nuevo_cliente.socket_fd, &master_set);
                    if (nuevo_cliente.socket_fd > max_fd) {
                        max_fd = nuevo_cliente.socket_fd;
                    }
                } else {
                    printf("[SERVIDOR] Máximo de clientes alcanzado, rechazando conexión\n");
                    desconectar_cliente(servidor, &nuevo_cliente);
                }
            }
        }
        
        // Verificar actividad en sockets de clientes
        for (int i = 0; i < servidor->config.max_clientes; i++) {
            if (servidor->clientes[i].activo && 
                FD_ISSET(servidor->clientes[i].socket_fd, &read_set)) {
                
                resultado_servidor_t resultado = procesar_cliente_eco(servidor, &servidor->clientes[i]);
                if (resultado == SERVIDOR_SHUTDOWN || resultado == SERVIDOR_ERROR_TIMEOUT) {
                    FD_CLR(servidor->clientes[i].socket_fd, &master_set);
                    desconectar_cliente(servidor, &servidor->clientes[i]);
                } else if (resultado != SERVIDOR_EXITO) {
                    fprintf(stderr, "Error procesando cliente: %s\n", servidor_strerror(resultado));
                    FD_CLR(servidor->clientes[i].socket_fd, &master_set);
                    desconectar_cliente(servidor, &servidor->clientes[i]);
                }
            }
        }
    }
    
    return SERVIDOR_EXITO;
}

// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

void obtener_estadisticas_servidor(const servidor_tcp_t* servidor, estadisticas_servidor_t* stats) {
    if (!servidor || !stats) return;
    
    pthread_mutex_lock((pthread_mutex_t*)&servidor->mutex_stats);
    *stats = servidor->stats;
    pthread_mutex_unlock((pthread_mutex_t*)&servidor->mutex_stats);
}

void resetear_estadisticas_servidor(servidor_tcp_t* servidor) {
    if (!servidor) return;
    
    pthread_mutex_lock(&servidor->mutex_stats);
    memset(&servidor->stats, 0, sizeof(estadisticas_servidor_t));
    servidor->stats.tiempo_inicio = time(NULL);
    pthread_mutex_unlock(&servidor->mutex_stats);
}

resultado_servidor_t configurar_socket_servidor(servidor_tcp_t* servidor) {
    return configurar_socket_opciones(servidor->socket_servidor, &servidor->config);
}

void instalar_manejadores_senales(servidor_tcp_t* servidor) {
    servidor_global = servidor;
    
    signal(SIGINT, manejador_shutdown);   // Ctrl+C
    signal(SIGTERM, manejador_shutdown);  // Terminación
    signal(SIGCHLD, manejador_sigchld);   // Procesos hijos terminados
    signal(SIGPIPE, SIG_IGN);             // Ignorar SIGPIPE
}

const char* servidor_strerror(resultado_servidor_t codigo) {
    switch (codigo) {
        case SERVIDOR_EXITO: return "Operación exitosa";
        case SERVIDOR_ERROR_SOCKET: return "Error creando socket";
        case SERVIDOR_ERROR_BIND: return "Error en bind";
        case SERVIDOR_ERROR_LISTEN: return "Error en listen";
        case SERVIDOR_ERROR_ACCEPT: return "Error en accept";
        case SERVIDOR_ERROR_RECV: return "Error recibiendo datos";
        case SERVIDOR_ERROR_SEND: return "Error enviando datos";
        case SERVIDOR_ERROR_MEMORIA: return "Error de memoria";
        case SERVIDOR_ERROR_TIMEOUT: return "Timeout";
        case SERVIDOR_ERROR_PARAMETRO: return "Parámetro inválido";
        case SERVIDOR_ERROR_SISTEMA: return "Error del sistema";
        case SERVIDOR_SHUTDOWN: return "Shutdown del servidor";
        default: return "Error desconocido";
    }
}

resultado_servidor_t obtener_info_cliente(int cliente_fd, info_cliente_t* info) {
    if (!info || cliente_fd < 0) return SERVIDOR_ERROR_PARAMETRO;
    
    socklen_t tam = sizeof(info->direccion);
    if (getpeername(cliente_fd, (struct sockaddr*)&info->direccion, &tam) < 0) {
        perror("getpeername");
        return SERVIDOR_ERROR_SISTEMA;
    }
    
    info->socket_fd = cliente_fd;
    info->tiempo_conexion = time(NULL);
    info->activo = 1;
    
    return SERVIDOR_EXITO;
}

void imprimir_estadisticas_servidor(const servidor_tcp_t* servidor, FILE* archivo) {
    if (!servidor) return;
    if (!archivo) archivo = stdout;
    
    estadisticas_servidor_t stats;
    obtener_estadisticas_servidor(servidor, &stats);
    
    time_t tiempo_actual = time(NULL);
    double tiempo_ejecucion = difftime(tiempo_actual, stats.tiempo_inicio);
    
    fprintf(archivo, "\n=== ESTADÍSTICAS DEL SERVIDOR ===\n");
    fprintf(archivo, "Tiempo de ejecución: %.0f segundos\n", tiempo_ejecucion);
    fprintf(archivo, "Conexiones totales: %d\n", stats.conexiones_totales);
    fprintf(archivo, "Conexiones activas: %d\n", stats.conexiones_activas);
    fprintf(archivo, "Conexiones rechazadas: %d\n", stats.conexiones_rechazadas);
    fprintf(archivo, "Mensajes procesados: %d\n", stats.mensajes_procesados);
    fprintf(archivo, "Bytes transferidos: %zu\n", stats.bytes_transferidos);
    fprintf(archivo, "Tiempo promedio por mensaje: %.2f ms\n", stats.tiempo_promedio_ms);
    fprintf(archivo, "Errores de red: %d\n", stats.errores_red);
    fprintf(archivo, "Timeouts: %d\n", stats.timeouts);
    
    if (stats.mensajes_procesados > 0 && tiempo_ejecucion > 0) {
        fprintf(archivo, "Mensajes por segundo: %.2f\n", stats.mensajes_procesados / tiempo_ejecucion);
        fprintf(archivo, "Throughput: %.2f KB/s\n", (stats.bytes_transferidos / 1024.0) / tiempo_ejecucion);
    }
    fprintf(archivo, "================================\n\n");
}

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// ============================================================================

resultado_servidor_t demo_servidor_basico(void) {
    printf("\n=== DEMO: Servidor TCP Básico ===\n");
    
    servidor_tcp_t* servidor = crear_servidor(NULL);
    if (!servidor) {
        fprintf(stderr, "Error creando servidor\n");
        return SERVIDOR_ERROR_MEMORIA;
    }
    
    resultado_servidor_t resultado = iniciar_servidor(servidor);
    if (resultado != SERVIDOR_EXITO) {
        fprintf(stderr, "Error iniciando servidor: %s\n", servidor_strerror(resultado));
        destruir_servidor(servidor);
        return resultado;
    }
    
    printf("Servidor de eco ejecutándose. Presiona Ctrl+C para detener.\n");
    printf("Prueba con: telnet localhost %d\n", servidor->config.puerto);
    
    resultado = ejecutar_servidor(servidor);
    
    imprimir_estadisticas_servidor(servidor, NULL);
    destruir_servidor(servidor);
    
    return resultado;
}

resultado_servidor_t demo_servidor_modos(void) {
    printf("\n=== DEMO: Modos del Servidor ===\n");
    
    config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
    config.modo = MODO_THREAD;
    config.max_clientes = 5;
    config.verbose = 1;
    
    servidor_tcp_t* servidor = crear_servidor(&config);
    if (!servidor) return SERVIDOR_ERROR_MEMORIA;
    
    resultado_servidor_t resultado = iniciar_servidor(servidor);
    if (resultado != SERVIDOR_EXITO) {
        destruir_servidor(servidor);
        return resultado;
    }
    
    printf("Servidor ejecutándose en modo THREAD. Presiona Ctrl+C para detener.\n");
    resultado = ejecutar_servidor(servidor);
    
    imprimir_estadisticas_servidor(servidor, NULL);
    destruir_servidor(servidor);
    
    return resultado;
}

resultado_servidor_t demo_servidor_rendimiento(void) {
    printf("\n=== DEMO: Rendimiento del Servidor ===\n");
    
    config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
    config.modo = MODO_SELECT;
    config.max_clientes = 20;
    config.buffer_size = 8192;
    config.verbose = 0; // Menos verbose para mejor rendimiento
    
    servidor_tcp_t* servidor = crear_servidor(&config);
    if (!servidor) return SERVIDOR_ERROR_MEMORIA;
    
    resultado_servidor_t resultado = iniciar_servidor(servidor);
    if (resultado != SERVIDOR_EXITO) {
        destruir_servidor(servidor);
        return resultado;
    }
    
    printf("Servidor optimizado para rendimiento. Presiona Ctrl+C para detener.\n");
    printf("Métricas se actualizarán automáticamente.\n");
    
    resultado = ejecutar_servidor(servidor);
    
    imprimir_estadisticas_servidor(servidor, NULL);
    destruir_servidor(servidor);
    
    return resultado;
}

resultado_servidor_t demo_servidor_configuracion(void) {
    printf("\n=== DEMO: Configuración Personalizada ===\n");
    
    config_servidor_t config = {
        .host = "127.0.0.1",
        .puerto = 9090,
        .backlog = 5,
        .max_clientes = 10,
        .timeout_cliente_seg = 15,
        .buffer_size = 2048,
        .modo = MODO_FORK,
        .reusar_puerto = 1,
        .keep_alive = 1,
        .no_delay = 1,
        .verbose = 1,
        .daemonizar = 0
    };
    
    servidor_tcp_t* servidor = crear_servidor(&config);
    if (!servidor) return SERVIDOR_ERROR_MEMORIA;
    
    resultado_servidor_t resultado = iniciar_servidor(servidor);
    if (resultado != SERVIDOR_EXITO) {
        destruir_servidor(servidor);
        return resultado;
    }
    
    printf("Servidor con configuración personalizada. Presiona Ctrl+C para detener.\n");
    resultado = ejecutar_servidor(servidor);
    
    imprimir_estadisticas_servidor(servidor, NULL);
    destruir_servidor(servidor);
    
    return resultado;
}

// ============================================================================
// HERRAMIENTAS AUXILIARES
// ============================================================================

resultado_servidor_t cliente_prueba_simple(const char* host, int puerto, 
                                          const char* mensaje, char* respuesta, 
                                          size_t tam_respuesta) {
    if (!host || !mensaje || !respuesta) return SERVIDOR_ERROR_PARAMETRO;
    
    // Evitar SIGPIPE en el cliente de prueba
    signal(SIGPIPE, SIG_IGN);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket cliente");
        return SERVIDOR_ERROR_SOCKET;
    }
    
    struct sockaddr_in servidor_addr;
    memset(&servidor_addr, 0, sizeof(servidor_addr));
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);
    
    if (inet_aton(host, &servidor_addr.sin_addr) == 0) {
        fprintf(stderr, "Dirección IP inválida: %s\n", host);
        close(socket_fd);
        return SERVIDOR_ERROR_PARAMETRO;
    }
    
    if (connect(socket_fd, (struct sockaddr*)&servidor_addr, sizeof(servidor_addr)) < 0) {
        perror("connect cliente");
        close(socket_fd);
        return SERVIDOR_ERROR_SISTEMA;
    }
    
    // Si el mensaje es vacío, considerar éxito de conexión sin intercambio
    if (mensaje[0] == '\0') {
        if (respuesta && tam_respuesta > 0) respuesta[0] = '\0';
        close(socket_fd);
        return SERVIDOR_EXITO;
    }

    // Enviar mensaje (evitar SIGPIPE en macOS)
    #ifdef SO_NOSIGPIPE
        int optval = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
    #endif

    // Configurar un timeout de recepción para el cliente de prueba
    struct timeval tv; tv.tv_sec = 2; tv.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Enviar mensaje
    size_t longitud_mensaje = strlen(mensaje);
    ssize_t enviados;
    #ifdef MSG_NOSIGNAL
    enviados = send(socket_fd, mensaje, longitud_mensaje, MSG_NOSIGNAL);
    #else
    enviados = send(socket_fd, mensaje, longitud_mensaje, 0);
    #endif
    if (enviados < 0) {
        perror("send cliente");
        close(socket_fd);
        return SERVIDOR_ERROR_SEND;
    }
    
    // Recibir respuesta (acumular hasta longitud esperada o timeout)
    size_t esperado = longitud_mensaje < (tam_respuesta - 1) ? longitud_mensaje : (tam_respuesta - 1);
    size_t total = 0;
    while (total < esperado) {
        ssize_t r = recv(socket_fd, respuesta + total, (tam_respuesta - 1) - total, 0);
        if (r < 0) {
            if (errno == EINTR) continue;
            perror("recv cliente");
            close(socket_fd);
            return SERVIDOR_ERROR_RECV;
        }
        if (r == 0) {
            break; // conexión cerrada
        }
        total += (size_t)r;
    }
    respuesta[total] = '\0';
    close(socket_fd);
    
    return SERVIDOR_EXITO;
}

resultado_servidor_t stress_test_clientes(const char* host, int puerto, 
                                         int num_clientes, int mensajes_por_cliente) {
    printf("Iniciando stress test: %d clientes, %d mensajes cada uno\n", 
           num_clientes, mensajes_por_cliente);
    
    for (int cliente = 0; cliente < num_clientes; cliente++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Proceso hijo - cliente
            for (int msg = 0; msg < mensajes_por_cliente; msg++) {
                char mensaje[256];
                char respuesta[256];
                
                snprintf(mensaje, sizeof(mensaje), "Cliente %d, mensaje %d", cliente, msg);
                
                resultado_servidor_t resultado = cliente_prueba_simple(host, puerto, mensaje, 
                                                                     respuesta, sizeof(respuesta));
                if (resultado != SERVIDOR_EXITO) {
                    fprintf(stderr, "Cliente %d error en mensaje %d: %s\n", 
                           cliente, msg, servidor_strerror(resultado));
                    break;
                }
                
                usleep(10000); // 10ms entre mensajes
            }
            exit(0);
        } else if (pid < 0) {
            perror("fork cliente");
            return SERVIDOR_ERROR_SISTEMA;
        }
    }
    
    // Esperar a todos los clientes
    for (int i = 0; i < num_clientes; i++) {
        wait(NULL);
    }
    
    printf("Stress test completado\n");
    return SERVIDOR_EXITO;
}
