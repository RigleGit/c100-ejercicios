/**
 * @file servidor_tcp_multicliente.c
 * @brief Implementación completa de servidor TCP multicliente con hilos POSIX
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Implementación robusta de un servidor TCP que maneja múltiples clientes
 * simultáneamente usando pthread_create para crear un hilo por conexión.
 * Incluye manejo avanzado de errores, estadísticas, logging y funciones
 * de demostración educativas.
 */

#include "../include/servidor_tcp_multicliente.h"
#include <stdarg.h>
#include <sys/resource.h>

// Variable global para el contexto del servidor (para manejadores de señales)
static ContextoServidor* g_servidor_ctx = NULL;

// =============================================================================
// FUNCIONES DE CONFIGURACIÓN
// =============================================================================

void configurar_servidor_por_defecto(ConfigServidor* config) {
    if (!config) return;
    
    memset(config, 0, sizeof(ConfigServidor));
    config->puerto = PUERTO_DEFAULT;
    config->max_conexiones = MAX_CONEXIONES_DEFAULT;
    config->max_hilos = MAX_HILOS_DEFAULT;
    config->buffer_size = BUFFER_SIZE_DEFAULT;
    config->timeout_cliente = TIMEOUT_CLIENTE_DEFAULT;
    config->backlog = BACKLOG_DEFAULT;
    config->tipo_servidor = TIPO_ECO;
    config->log_detallado = 1;
    config->reutilizar_puerto = 1;
    config->keepalive = 1;
    strcpy(config->bind_ip, "0.0.0.0");
}

int validar_configuracion_servidor(const ConfigServidor* config) {
    if (!config) return SERVER_ERROR_CONFIGURACION;
    
    if (config->puerto <= 0 || config->puerto > 65535) {
        return SERVER_ERROR_CONFIGURACION;
    }
    
    if (config->max_conexiones <= 0 || config->max_conexiones > 1000) {
        return SERVER_ERROR_CONFIGURACION;
    }
    
    if (config->max_hilos <= 0 || config->max_hilos > 200) {
        return SERVER_ERROR_CONFIGURACION;
    }
    
    if (config->buffer_size <= 0 || config->buffer_size > 1024*1024) {
        return SERVER_ERROR_CONFIGURACION;
    }
    
    if (config->timeout_cliente < 0) {
        return SERVER_ERROR_CONFIGURACION;
    }
    
    if (config->backlog <= 0 || config->backlog > 128) {
        return SERVER_ERROR_CONFIGURACION;
    }
    
    return SERVER_EXITO;
}

int aplicar_configuracion_socket(int socket_fd, const ConfigServidor* config) {
    if (socket_fd < 0 || !config) return -1;
    
    // SO_REUSEADDR para reutilizar direcciones
    if (config->reutilizar_puerto) {
        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("Error configurando SO_REUSEADDR");
            return -1;
        }
    }
    
    // SO_KEEPALIVE para detectar conexiones muertas
    if (config->keepalive) {
        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) {
            perror("Error configurando SO_KEEPALIVE");
            return -1;
        }
    }
    
    // Configurar buffer de recepción
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, 
                   &config->buffer_size, sizeof(config->buffer_size)) < 0) {
        perror("Warning: Error configurando SO_RCVBUF");
        // No es crítico, continuamos
    }
    
    // Configurar buffer de envío
    if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, 
                   &config->buffer_size, sizeof(config->buffer_size)) < 0) {
        perror("Warning: Error configurando SO_SNDBUF");
        // No es crítico, continuamos
    }
    
    return 0;
}

// =============================================================================
// FUNCIONES PRINCIPALES DEL SERVIDOR
// =============================================================================

int inicializar_servidor(ContextoServidor* contexto, const ConfigServidor* config) {
    if (!contexto || !config) return SERVER_ERROR_CONFIGURACION;
    
    // Validar configuración
    int resultado = validar_configuracion_servidor(config);
    if (resultado != SERVER_EXITO) {
        return resultado;
    }
    
    // Inicializar contexto
    memset(contexto, 0, sizeof(ContextoServidor));
    contexto->config = *config;
    contexto->socket_servidor = -1;
    contexto->ejecutando = 0;
    
    // Reservar memoria para clientes
    contexto->clientes = calloc(config->max_conexiones, sizeof(InfoCliente));
    if (!contexto->clientes) {
        return SERVER_ERROR_MEMORIA;
    }
    
    // Inicializar mutexes
    if (pthread_mutex_init(&contexto->mutex_clientes, NULL) != 0) {
        free(contexto->clientes);
        return SERVER_ERROR_SISTEMA;
    }
    
    if (pthread_mutex_init(&contexto->mutex_logs, NULL) != 0) {
        pthread_mutex_destroy(&contexto->mutex_clientes);
        free(contexto->clientes);
        return SERVER_ERROR_SISTEMA;
    }
    
    // Inicializar estadísticas
    inicializar_estadisticas(&contexto->stats);
    
    return SERVER_EXITO;
}

int crear_socket_servidor(ContextoServidor* contexto) {
    if (!contexto) return -1;
    
    // Crear socket TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        LOG_ERROR(contexto, "Error creando socket: %s", strerror(errno));
        return -1;
    }
    
    // Aplicar configuración de socket
    if (aplicar_configuracion_socket(socket_fd, &contexto->config) < 0) {
        close(socket_fd);
        return -1;
    }
    
    // Configurar dirección del servidor
    struct sockaddr_in direccion_servidor;
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(contexto->config.puerto);
    
    if (strcmp(contexto->config.bind_ip, "0.0.0.0") == 0) {
        direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    } else {
        if (inet_pton(AF_INET, contexto->config.bind_ip, 
                      &direccion_servidor.sin_addr) <= 0) {
            LOG_ERROR(contexto, "IP de bind inválida: %s", contexto->config.bind_ip);
            close(socket_fd);
            return -1;
        }
    }
    
    // Bind del socket
    if (bind(socket_fd, (struct sockaddr*)&direccion_servidor, 
             sizeof(direccion_servidor)) < 0) {
        LOG_ERROR(contexto, "Error en bind: %s", strerror(errno));
        close(socket_fd);
        return -1;
    }
    
    // Listen para conexiones entrantes
    if (listen(socket_fd, contexto->config.backlog) < 0) {
        LOG_ERROR(contexto, "Error en listen: %s", strerror(errno));
        close(socket_fd);
        return -1;
    }
    
    contexto->socket_servidor = socket_fd;
    
    LOG_INFO(contexto, "Servidor creado exitosamente en %s:%d", 
             contexto->config.bind_ip, contexto->config.puerto);
    
    return socket_fd;
}

// =============================================================================
// FUNCIONES DE MANEJO DE SEÑALES
// =============================================================================

void manejador_senales(int signum) {
    if (g_servidor_ctx) {
        LOG_INFO(g_servidor_ctx, "Señal %d recibida, iniciando cierre ordenado", signum);
        g_servidor_ctx->ejecutando = 0;
    }
}

void instalar_manejadores_senales(ContextoServidor* contexto) {
    g_servidor_ctx = contexto;
    signal(SIGINT, manejador_senales);
    signal(SIGTERM, manejador_senales);
    signal(SIGPIPE, SIG_IGN); // Ignorar SIGPIPE para conexiones cerradas
}

// =============================================================================
// FUNCIONES DE MANEJO DE CLIENTES
// =============================================================================

InfoCliente* registrar_cliente(ContextoServidor* contexto, int cliente_fd, 
                              const struct sockaddr_in* direccion) {
    if (!contexto || cliente_fd < 0 || !direccion) return NULL;
    
    LOCK_CLIENTES(contexto);
    
    // Buscar slot libre para el cliente
    InfoCliente* cliente = NULL;
    for (int i = 0; i < contexto->config.max_conexiones; i++) {
        if (!contexto->clientes[i].activo) {
            cliente = &contexto->clientes[i];
            break;
        }
    }
    
    if (!cliente) {
        UNLOCK_CLIENTES(contexto);
        LOG_WARN(contexto, "Máximo de conexiones alcanzado, rechazando cliente");
        close(cliente_fd);
        
        LOCK_STATS(&contexto->stats);
        contexto->stats.conexiones_rechazadas++;
        UNLOCK_STATS(&contexto->stats);
        
        return NULL;
    }
    
    // Inicializar información del cliente
    memset(cliente, 0, sizeof(InfoCliente));
    cliente->socket_fd = cliente_fd;
    cliente->direccion = *direccion;
    cliente->tiempo_conexion = obtener_timestamp_actual();
    cliente->ultima_actividad = cliente->tiempo_conexion;
    cliente->activo = 1;
    
    // Generar identificador único
    generar_id_cliente(direccion, cliente->identificador, sizeof(cliente->identificador));
    
    // Actualizar estadísticas
    actualizar_estadisticas_conexion(&contexto->stats, 1, 0);
    
    UNLOCK_CLIENTES(contexto);
    
    char direccion_str[64];
    formatear_direccion_cliente(direccion, direccion_str, sizeof(direccion_str));
    LOG_INFO(contexto, "Cliente registrado: %s [%s]", direccion_str, cliente->identificador);
    
    return cliente;
}

void desregistrar_cliente(ContextoServidor* contexto, InfoCliente* cliente) {
    if (!contexto || !cliente || !cliente->activo) return;
    
    LOCK_CLIENTES(contexto);
    
    // Marcar cliente como inactivo
    cliente->activo = 0;
    
    // Cerrar socket si está abierto
    if (cliente->socket_fd >= 0) {
        close(cliente->socket_fd);
        cliente->socket_fd = -1;
    }
    
    // Actualizar estadísticas
    actualizar_estadisticas_conexion(&contexto->stats, 0, 1);
    
    UNLOCK_CLIENTES(contexto);
    
    char direccion_str[64];
    formatear_direccion_cliente(&cliente->direccion, direccion_str, sizeof(direccion_str));
    LOG_INFO(contexto, "Cliente desregistrado: %s [%s] - Bytes: ↑%zu ↓%zu Mensajes: ↑%zu ↓%zu", 
             direccion_str, cliente->identificador, 
             cliente->bytes_enviados, cliente->bytes_recibidos,
             cliente->mensajes_enviados, cliente->mensajes_recibidos);
}

InfoCliente* buscar_cliente_por_socket(ContextoServidor* contexto, int socket_fd) {
    if (!contexto || socket_fd < 0) return NULL;
    
    LOCK_CLIENTES(contexto);
    
    for (int i = 0; i < contexto->config.max_conexiones; i++) {
        if (contexto->clientes[i].activo && 
            contexto->clientes[i].socket_fd == socket_fd) {
            UNLOCK_CLIENTES(contexto);
            return &contexto->clientes[i];
        }
    }
    
    UNLOCK_CLIENTES(contexto);
    return NULL;
}

InfoCliente* aceptar_cliente(ContextoServidor* contexto) {
    if (!contexto || contexto->socket_servidor < 0) return NULL;
    
    struct sockaddr_in direccion_cliente;
    socklen_t tamaño_direccion = sizeof(direccion_cliente);
    
    int cliente_fd = accept(contexto->socket_servidor, 
                           (struct sockaddr*)&direccion_cliente, 
                           &tamaño_direccion);
    
    if (cliente_fd < 0) {
        if (errno != EINTR && contexto->ejecutando) {
            LOG_ERROR(contexto, "Error en accept: %s", strerror(errno));
        }
        return NULL;
    }
    
    // Configurar socket del cliente
    ConfigServidor config_cliente = contexto->config;
    if (aplicar_configuracion_socket(cliente_fd, &config_cliente) < 0) {
        close(cliente_fd);
        return NULL;
    }
    
    return registrar_cliente(contexto, cliente_fd, &direccion_cliente);
}

// =============================================================================
// FUNCIONES DE COMUNICACIÓN
// =============================================================================

ssize_t enviar_a_cliente(InfoCliente* cliente, const char* mensaje, size_t tamaño) {
    if (!CLIENTE_ACTIVO(cliente) || !mensaje || tamaño == 0) return -1;
    
    ssize_t bytes_enviados = send(cliente->socket_fd, mensaje, tamaño, MSG_NOSIGNAL);
    
    if (bytes_enviados > 0) {
        cliente->bytes_enviados += bytes_enviados;
        cliente->mensajes_enviados++;
        cliente->ultima_actividad = obtener_timestamp_actual();
    }
    
    return bytes_enviados;
}

ssize_t recibir_de_cliente(InfoCliente* cliente, char* buffer, size_t tamaño_buffer) {
    if (!CLIENTE_ACTIVO(cliente) || !buffer || tamaño_buffer == 0) return -1;
    
    ssize_t bytes_recibidos = recv(cliente->socket_fd, buffer, tamaño_buffer, 0);
    
    if (bytes_recibidos > 0) {
        cliente->bytes_recibidos += bytes_recibidos;
        cliente->mensajes_recibidos++;
        cliente->ultima_actividad = obtener_timestamp_actual();
    }
    
    return bytes_recibidos;
}

int enviar_broadcast(ContextoServidor* contexto, const char* mensaje, 
                    size_t tamaño, InfoCliente* excluir) {
    if (!contexto || !mensaje || tamaño == 0) return 0;
    
    int clientes_enviados = 0;
    
    LOCK_CLIENTES(contexto);
    
    for (int i = 0; i < contexto->config.max_conexiones; i++) {
        InfoCliente* cliente = &contexto->clientes[i];
        
        if (CLIENTE_ACTIVO(cliente) && cliente != excluir) {
            if (enviar_a_cliente(cliente, mensaje, tamaño) > 0) {
                clientes_enviados++;
            }
        }
    }
    
    UNLOCK_CLIENTES(contexto);
    
    return clientes_enviados;
}

// =============================================================================
// FUNCIÓN PRINCIPAL DEL HILO DE CLIENTE
// =============================================================================

void* atender_cliente(void* arg) {
    ParametrosHilo* params = (ParametrosHilo*)arg;
    if (!params || !params->cliente || !params->config) {
        free(params);
        return NULL;
    }
    
    InfoCliente* cliente = params->cliente;
    ContextoServidor* contexto = (ContextoServidor*)params->servidor_ctx;
    char* buffer = malloc(params->config->buffer_size);
    
    if (!buffer) {
        LOG_ERROR(contexto, "Error asignando buffer para cliente %s", cliente->identificador);
        desregistrar_cliente(contexto, cliente);
        free(params);
        return NULL;
    }
    
    // Actualizar estadísticas de hilos
    LOCK_STATS(&contexto->stats);
    contexto->stats.hilos_activos++;
    UNLOCK_STATS(&contexto->stats);
    
    char direccion_str[64];
    formatear_direccion_cliente(&cliente->direccion, direccion_str, sizeof(direccion_str));
    LOG_INFO(contexto, "Hilo iniciado para cliente %s", direccion_str);
    
    // Enviar mensaje de bienvenida
    if (enviar_a_cliente(cliente, MENSAJE_BIENVENIDA, strlen(MENSAJE_BIENVENIDA)) < 0) {
        LOG_WARN(contexto, "Error enviando bienvenida a %s", direccion_str);
    }
    
    // Bucle principal de comunicación
    while (CLIENTE_ACTIVO(cliente) && contexto->ejecutando) {
        ssize_t bytes_recibidos = recibir_de_cliente(cliente, buffer, 
                                                    params->config->buffer_size - 1);
        
        if (bytes_recibidos > 0) {
            buffer[bytes_recibidos] = '\0';
            
            // Procesar comandos especiales
            if (strncmp(buffer, COMANDO_QUIT, strlen(COMANDO_QUIT)) == 0) {
                LOG_INFO(contexto, "Cliente %s solicitó desconexión", direccion_str);
                break;
            } else if (strncmp(buffer, COMANDO_STATS, strlen(COMANDO_STATS)) == 0) {
                // Enviar estadísticas del cliente
                char stats_msg[512];
                snprintf(stats_msg, sizeof(stats_msg),
                        "=== ESTADÍSTICAS ===\n"
                        "Tiempo conectado: %ld segundos\n"
                        "Bytes enviados: %zu\n"
                        "Bytes recibidos: %zu\n"
                        "Mensajes enviados: %zu\n"
                        "Mensajes recibidos: %zu\n"
                        "==================\n",
                        obtener_timestamp_actual() - cliente->tiempo_conexion,
                        cliente->bytes_enviados, cliente->bytes_recibidos,
                        cliente->mensajes_enviados, cliente->mensajes_recibidos);
                
                enviar_a_cliente(cliente, stats_msg, strlen(stats_msg));
                continue;
            } else if (strncmp(buffer, COMANDO_HELP, strlen(COMANDO_HELP)) == 0) {
                const char* help_msg = 
                    "=== COMANDOS DISPONIBLES ===\n"
                    "quit  - Desconectar\n"
                    "stats - Ver estadísticas\n"
                    "help  - Esta ayuda\n"
                    "===========================\n";
                
                enviar_a_cliente(cliente, help_msg, strlen(help_msg));
                continue;
            }
            
            // Eco del mensaje (comportamiento por defecto)
            if (params->config->tipo_servidor == TIPO_ECO) {
                if (enviar_a_cliente(cliente, buffer, bytes_recibidos) < 0) {
                    LOG_WARN(contexto, "Error enviando eco a %s", direccion_str);
                    break;
                }
            } else if (params->config->tipo_servidor == TIPO_CHAT) {
                // Broadcast del mensaje a todos los clientes
                char mensaje_chat[params->config->buffer_size + 100];
                snprintf(mensaje_chat, sizeof(mensaje_chat), 
                        "[%s]: %s", cliente->identificador, buffer);
                
                int clientes_enviados = enviar_broadcast(contexto, mensaje_chat, 
                                                       strlen(mensaje_chat), cliente);
                
                LOG_INFO(contexto, "Mensaje de %s enviado a %d clientes", 
                        direccion_str, clientes_enviados);
            }
            
            // Actualizar estadísticas globales
            actualizar_estadisticas_comunicacion(&contexto->stats, 
                                                bytes_recibidos, bytes_recibidos);
            
        } else if (bytes_recibidos == 0) {
            // Cliente desconectado normalmente
            LOG_INFO(contexto, "Cliente %s desconectado", direccion_str);
            break;
        } else {
            // Error en recepción
            if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
                LOG_WARN(contexto, "Error recibiendo de %s: %s", direccion_str, strerror(errno));
                break;
            }
        }
        
        // Verificar timeout del cliente
        if (params->config->timeout_cliente > 0 && 
            CLIENTE_TIMEOUT(cliente, params->config->timeout_cliente)) {
            LOG_INFO(contexto, "Timeout para cliente %s", direccion_str);
            break;
        }
    }
    
    // Enviar mensaje de despedida
    enviar_a_cliente(cliente, MENSAJE_DESPEDIDA, strlen(MENSAJE_DESPEDIDA));
    
    // Limpiar recursos
    desregistrar_cliente(contexto, cliente);
    
    // Actualizar estadísticas de hilos
    LOCK_STATS(&contexto->stats);
    contexto->stats.hilos_activos--;
    UNLOCK_STATS(&contexto->stats);
    
    free(buffer);
    free(params);
    
    LOG_INFO(contexto, "Hilo terminado para cliente %s", direccion_str);
    
    return NULL;
}

// =============================================================================
// FUNCIÓN PRINCIPAL DEL SERVIDOR
// =============================================================================

int ejecutar_servidor(ContextoServidor* contexto) {
    if (!contexto) return SERVER_ERROR_CONFIGURACION;
    
    // Crear socket del servidor
    if (crear_socket_servidor(contexto) < 0) {
        return SERVER_ERROR_SOCKET;
    }
    
    // Instalar manejadores de señales
    instalar_manejadores_senales(contexto);
    
    // Marcar servidor como ejecutándose
    contexto->ejecutando = 1;
    contexto->stats.tiempo_inicio = obtener_timestamp_actual();
    
    LOG_INFO(contexto, "Servidor iniciado - Puerto: %d, Max conexiones: %d, Max hilos: %d", 
             contexto->config.puerto, contexto->config.max_conexiones, contexto->config.max_hilos);
    
    // Bucle principal de aceptación de conexiones
    while (contexto->ejecutando) {
        InfoCliente* cliente = aceptar_cliente(contexto);
        
        if (!cliente) {
            if (contexto->ejecutando) {
                // Solo es error si el servidor sigue ejecutándose
                usleep(10000); // Pausa corta para evitar busy loop
            }
            continue;
        }
        
        // Verificar límite de hilos activos
        LOCK_STATS(&contexto->stats);
        int hilos_activos = contexto->stats.hilos_activos;
        UNLOCK_STATS(&contexto->stats);
        
        if (hilos_activos >= contexto->config.max_hilos) {
            LOG_WARN(contexto, "Máximo de hilos alcanzado, rechazando cliente");
            desregistrar_cliente(contexto, cliente);
            continue;
        }
        
        // Crear parámetros para el hilo
        ParametrosHilo* params = malloc(sizeof(ParametrosHilo));
        if (!params) {
            LOG_ERROR(contexto, "Error asignando memoria para parámetros de hilo");
            desregistrar_cliente(contexto, cliente);
            continue;
        }
        
        params->cliente = cliente;
        params->config = &contexto->config;
        params->servidor_ctx = contexto;
        
        // Crear hilo para atender al cliente
        if (pthread_create(&cliente->thread_id, NULL, atender_cliente, params) != 0) {
            LOG_ERROR(contexto, "Error creando hilo para cliente: %s", strerror(errno));
            desregistrar_cliente(contexto, cliente);
            free(params);
            
            LOCK_STATS(&contexto->stats);
            contexto->stats.errores_hilos++;
            UNLOCK_STATS(&contexto->stats);
            
            continue;
        }
        
        // Detach del hilo para liberación automática de recursos
        if (pthread_detach(cliente->thread_id) != 0) {
            LOG_WARN(contexto, "Error haciendo detach del hilo");
        }
    }
    
    LOG_INFO(contexto, "Servidor detenido");
    return SERVER_EXITO;
}

void detener_servidor(ContextoServidor* contexto) {
    if (!contexto) return;
    
    LOG_INFO(contexto, "Iniciando detención del servidor...");
    contexto->ejecutando = 0;
    
    // Cerrar socket principal para interrumpir accept()
    if (contexto->socket_servidor >= 0) {
        close(contexto->socket_servidor);
        contexto->socket_servidor = -1;
    }
    
    // Desconectar todos los clientes activos
    LOCK_CLIENTES(contexto);
    for (int i = 0; i < contexto->config.max_conexiones; i++) {
        if (contexto->clientes[i].activo) {
            close(contexto->clientes[i].socket_fd);
            contexto->clientes[i].activo = 0;
        }
    }
    UNLOCK_CLIENTES(contexto);
    
    // Esperar un poco para que los hilos terminen
    sleep(1);
    
    LOG_INFO(contexto, "Servidor detenido completamente");
}

void limpiar_servidor(ContextoServidor* contexto) {
    if (!contexto) return;
    
    // Liberar memoria de clientes
    if (contexto->clientes) {
        free(contexto->clientes);
        contexto->clientes = NULL;
    }
    
    // Destruir mutexes
    pthread_mutex_destroy(&contexto->mutex_clientes);
    pthread_mutex_destroy(&contexto->mutex_logs);
    pthread_mutex_destroy(&contexto->stats.mutex);
    
    LOG_INFO(contexto, "Recursos del servidor liberados");
}

// =============================================================================
// FUNCIONES DE ESTADÍSTICAS Y LOGGING
// =============================================================================

void inicializar_estadisticas(EstadisticasServidor* stats) {
    if (!stats) return;
    
    memset(stats, 0, sizeof(EstadisticasServidor));
    pthread_mutex_init(&stats->mutex, NULL);
    stats->tiempo_inicio = obtener_timestamp_actual();
}

void actualizar_estadisticas_conexion(EstadisticasServidor* stats, 
                                     int nueva_conexion, int desconexion) {
    if (!stats) return;
    
    LOCK_STATS(stats);
    
    if (nueva_conexion) {
        stats->conexiones_totales++;
        stats->conexiones_activas++;
    }
    
    if (desconexion && stats->conexiones_activas > 0) {
        stats->conexiones_activas--;
    }
    
    stats->tiempo_actividad = obtener_timestamp_actual();
    
    UNLOCK_STATS(stats);
}

void actualizar_estadisticas_comunicacion(EstadisticasServidor* stats,
                                         size_t bytes_enviados, size_t bytes_recibidos) {
    if (!stats) return;
    
    LOCK_STATS(stats);
    
    stats->bytes_totales_enviados += bytes_enviados;
    stats->bytes_totales_recibidos += bytes_recibidos;
    stats->mensajes_totales++;
    stats->tiempo_actividad = obtener_timestamp_actual();
    
    UNLOCK_STATS(stats);
}

void mostrar_estadisticas_servidor(const ContextoServidor* contexto) {
    if (!contexto) return;
    
    LOCK_STATS(&contexto->stats);
    
    time_t tiempo_actual = obtener_timestamp_actual();
    long tiempo_ejecucion = tiempo_actual - contexto->stats.tiempo_inicio;
    
    printf("\n=== ESTADÍSTICAS DEL SERVIDOR ===\n");
    printf("Tiempo de ejecución: %ld segundos\n", tiempo_ejecucion);
    printf("Conexiones totales: %zu\n", contexto->stats.conexiones_totales);
    printf("Conexiones activas: %zu\n", contexto->stats.conexiones_activas);
    printf("Conexiones rechazadas: %zu\n", contexto->stats.conexiones_rechazadas);
    printf("Hilos activos: %zu\n", contexto->stats.hilos_activos);
    printf("Bytes enviados: %zu\n", contexto->stats.bytes_totales_enviados);
    printf("Bytes recibidos: %zu\n", contexto->stats.bytes_totales_recibidos);
    printf("Mensajes totales: %zu\n", contexto->stats.mensajes_totales);
    printf("Errores de red: %zu\n", contexto->stats.errores_red);
    printf("Errores de hilos: %zu\n", contexto->stats.errores_hilos);
    
    if (tiempo_ejecucion > 0) {
        printf("Promedio conexiones/hora: %.2f\n", 
               (double)contexto->stats.conexiones_totales * 3600.0 / tiempo_ejecucion);
        printf("Throughput promedio: %.2f bytes/s\n", 
               (double)(contexto->stats.bytes_totales_enviados + contexto->stats.bytes_totales_recibidos) / tiempo_ejecucion);
    }
    
    printf("===============================\n\n");
    
    UNLOCK_STATS(&contexto->stats);
}

void log_servidor(ContextoServidor* contexto, const char* nivel, 
                 const char* formato, ...) {
    if (!contexto || !contexto->config.log_detallado) return;
    
    va_list args;
    va_start(args, formato);
    
    pthread_mutex_lock(&contexto->mutex_logs);
    
    // Timestamp
    time_t ahora;
    time(&ahora);
    struct tm* tm_info = localtime(&ahora);
    
    printf("[%04d-%02d-%02d %02d:%02d:%02d] [%s] ", 
           tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
           tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, nivel);
    
    vprintf(formato, args);
    printf("\n");
    fflush(stdout);
    
    pthread_mutex_unlock(&contexto->mutex_logs);
    
    va_end(args);
}

// =============================================================================
// FUNCIONES AUXILIARES
// =============================================================================

time_t obtener_timestamp_actual(void) {
    return time(NULL);
}

char* formatear_direccion_cliente(const struct sockaddr_in* direccion, 
                                 char* buffer, size_t tamaño_buffer) {
    if (!direccion || !buffer || tamaño_buffer == 0) return NULL;
    
    snprintf(buffer, tamaño_buffer, "%s:%d", 
             inet_ntoa(direccion->sin_addr), ntohs(direccion->sin_port));
    
    return buffer;
}

char* generar_id_cliente(const struct sockaddr_in* direccion, 
                        char* buffer, size_t tamaño_buffer) {
    if (!direccion || !buffer || tamaño_buffer == 0) return NULL;
    
    // Generar ID basado en IP, puerto y timestamp
    uint32_t ip = ntohl(direccion->sin_addr.s_addr);
    uint16_t puerto = ntohs(direccion->sin_port);
    uint32_t timestamp = (uint32_t)time(NULL);
    static volatile unsigned long contador_global = 0;
    unsigned long c = __sync_add_and_fetch(&contador_global, 1);
    snprintf(buffer, tamaño_buffer, "C%08X%04X%08X%08lX", ip, puerto, timestamp, c);
    
    return buffer;
}

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// =============================================================================

int demo_servidor_eco(int puerto) {
    printf("=== DEMO SERVIDOR ECO MULTICLIENTE ===\n");
    
    ContextoServidor contexto;
    ConfigServidor config;
    
    configurar_servidor_por_defecto(&config);
    config.puerto = (puerto > 0) ? puerto : PUERTO_DEMO_ECO;
    config.tipo_servidor = TIPO_ECO;
    config.max_conexiones = 10;
    config.max_hilos = 5;
    
    if (inicializar_servidor(&contexto, &config) != SERVER_EXITO) {
        printf("Error inicializando servidor demo\n");
        return -1;
    }
    
    printf("Servidor eco iniciado en puerto %d\n", config.puerto);
    printf("Conecta clientes con: telnet localhost %d\n", config.puerto);
    printf("Presiona Ctrl+C para detener\n\n");
    
    int resultado = ejecutar_servidor(&contexto);
    
    mostrar_estadisticas_servidor(&contexto);
    limpiar_servidor(&contexto);
    
    return resultado;
}

int demo_servidor_chat(int puerto) {
    printf("=== DEMO SERVIDOR CHAT MULTICLIENTE ===\n");
    
    ContextoServidor contexto;
    ConfigServidor config;
    
    configurar_servidor_por_defecto(&config);
    config.puerto = (puerto > 0) ? puerto : PUERTO_DEMO_CHAT;
    config.tipo_servidor = TIPO_CHAT;
    config.max_conexiones = 20;
    config.max_hilos = 10;
    
    if (inicializar_servidor(&contexto, &config) != SERVER_EXITO) {
        printf("Error inicializando servidor chat\n");
        return -1;
    }
    
    printf("Servidor chat iniciado en puerto %d\n", config.puerto);
    printf("Conecta múltiples clientes con: telnet localhost %d\n", config.puerto);
    printf("Los mensajes se retransmiten a todos los clientes conectados\n");
    printf("Presiona Ctrl+C para detener\n\n");
    
    int resultado = ejecutar_servidor(&contexto);
    
    mostrar_estadisticas_servidor(&contexto);
    limpiar_servidor(&contexto);
    
    return resultado;
}

void diagnosticar_sistema_multithread(void) {
    printf("=== DIAGNÓSTICO DEL SISTEMA MULTITHREAD ===\n");
    
    // Límites del sistema
    struct rlimit rlim;
    
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("Límite de file descriptors: %ld (soft) / %ld (hard)\n", 
               (long)rlim.rlim_cur, (long)rlim.rlim_max);
    }
    
    // Información de threads
    printf("Concurrencia de hardware: %d cores\n", (int)sysconf(_SC_NPROCESSORS_ONLN));
    
    // Tamaños de stack por defecto
    pthread_attr_t attr;
    size_t stack_size;
    if (pthread_attr_init(&attr) == 0) {
        if (pthread_attr_getstacksize(&attr, &stack_size) == 0) {
            printf("Tamaño de stack por thread: %zu bytes\n", stack_size);
        }
        pthread_attr_destroy(&attr);
    }
    
    printf("==========================================\n");
}
