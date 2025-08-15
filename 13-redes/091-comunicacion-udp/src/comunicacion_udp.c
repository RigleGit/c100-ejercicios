/**
 * @file comunicacion_udp.c
 * @brief Implementación de comunicación UDP - Ejercicio 091
 * @author Ejercicios de C
 * @date 2025
 */

#include "../include/comunicacion_udp.h"

// ============================================================================
// FUNCIONES DE UTILIDAD INTERNAS
// ============================================================================

/**
 * @brief Configurar timeout en un socket
 */
static resultado_udp_t configurar_timeout_socket(int socket_fd, int timeout_segundos) {
    if (timeout_segundos <= 0) return UDP_EXITO;
    
    struct timeval timeout;
    timeout.tv_sec = timeout_segundos;
    timeout.tv_usec = 0;
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt SO_RCVTIMEO");
        return UDP_ERROR_SOCKET;
    }
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt SO_SNDTIMEO");
        return UDP_ERROR_SOCKET;
    }
    
    return UDP_EXITO;
}

/**
 * @brief Configurar opciones del socket UDP
 */
static resultado_udp_t configurar_socket_udp(int socket_fd, const config_udp_t* config) {
    int opcion = 1;
    
    // SO_REUSEADDR: reutilizar dirección
    if (config->reusar_puerto) {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion)) < 0) {
            perror("setsockopt SO_REUSEADDR");
            return UDP_ERROR_SOCKET;
        }
    }
    
    // SO_BROADCAST: habilitar broadcast
    if (config->broadcast) {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &opcion, sizeof(opcion)) < 0) {
            perror("setsockopt SO_BROADCAST");
            return UDP_ERROR_SOCKET;
        }
    }
    
    // Configurar timeout
    return configurar_timeout_socket(socket_fd, config->timeout_segundos);
}

/**
 * @brief Actualizar estadísticas de forma thread-safe
 */
static void actualizar_estadisticas(udp_context_t* contexto, const char* operacion, 
                                   size_t bytes, double tiempo_ms) {
    if (!contexto) return;
    
    if (strcmp(operacion, "envio") == 0) {
        contexto->stats.mensajes_enviados++;
        contexto->stats.bytes_enviados += bytes;
    } else if (strcmp(operacion, "recepcion") == 0) {
        contexto->stats.mensajes_recibidos++;
        contexto->stats.bytes_recibidos += bytes;
    } else if (strcmp(operacion, "error_envio") == 0) {
        contexto->stats.errores_envio++;
    } else if (strcmp(operacion, "error_recepcion") == 0) {
        contexto->stats.errores_recepcion++;
    } else if (strcmp(operacion, "timeout") == 0) {
        contexto->stats.timeouts++;
    } else if (strcmp(operacion, "truncado") == 0) {
        contexto->stats.mensajes_truncados++;
    }
    
    // Actualizar RTT promedio si se proporciona tiempo
    if (tiempo_ms > 0 && strcmp(operacion, "envio") == 0) {
        if (contexto->stats.mensajes_enviados == 1) {
            contexto->stats.rtt_promedio_ms = tiempo_ms;
        } else {
            double total_anterior = contexto->stats.rtt_promedio_ms * (contexto->stats.mensajes_enviados - 1);
            contexto->stats.rtt_promedio_ms = (total_anterior + tiempo_ms) / contexto->stats.mensajes_enviados;
        }
    }
    
    // Calcular throughput
    time_t tiempo_actual = time(NULL);
    double tiempo_transcurrido = difftime(tiempo_actual, contexto->stats.tiempo_inicio);
    if (tiempo_transcurrido > 0) {
        double total_bytes = contexto->stats.bytes_enviados + contexto->stats.bytes_recibidos;
        contexto->stats.throughput_kbps = (total_bytes / 1024.0) / tiempo_transcurrido;
    }
}

// ============================================================================
// IMPLEMENTACIÓN DE LA API PRINCIPAL
// ============================================================================

udp_context_t* crear_contexto_udp(const config_udp_t* config) {
    udp_context_t* contexto = malloc(sizeof(udp_context_t));
    if (!contexto) {
        perror("malloc contexto UDP");
        return NULL;
    }
    
    // Inicializar estructura
    memset(contexto, 0, sizeof(udp_context_t));
    
    // Configuración
    if (config) {
        contexto->config = *config;
    } else {
        config_udp_t config_defecto = CONFIG_UDP_DEFECTO;
        contexto->config = config_defecto;
    }
    
    // Asignar buffer interno
    contexto->buffer_interno = malloc(contexto->config.buffer_size);
    if (!contexto->buffer_interno) {
        perror("malloc buffer interno");
        free(contexto);
        return NULL;
    }
    
    // Inicializar estadísticas
    contexto->stats.tiempo_inicio = time(NULL);
    contexto->socket_fd = -1;
    contexto->inicializado = 0;
    
    if (contexto->config.verbose) {
        printf("[UDP] Contexto creado: %s:%d, tipo=%s, buffer=%d bytes\n",
               contexto->config.host, contexto->config.puerto,
               contexto->config.tipo == UDP_EMISOR ? "EMISOR" :
               contexto->config.tipo == UDP_RECEPTOR ? "RECEPTOR" : "BIDIRECCIONAL",
               contexto->config.buffer_size);
    }
    
    return contexto;
}

void destruir_contexto_udp(udp_context_t* contexto) {
    if (!contexto) return;
    
    if (contexto->config.verbose) {
        printf("[UDP] Destruyendo contexto...\n");
    }
    
    // Cerrar socket si está abierto
    if (contexto->socket_fd >= 0) {
        close(contexto->socket_fd);
    }
    
    // Liberar buffer interno
    if (contexto->buffer_interno) {
        free(contexto->buffer_interno);
    }
    
    free(contexto);
}

resultado_udp_t inicializar_udp(udp_context_t* contexto) {
    if (!contexto) return UDP_ERROR_PARAMETRO;
    
    // Crear socket UDP
    contexto->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (contexto->socket_fd < 0) {
        perror("socket UDP");
        return UDP_ERROR_SOCKET;
    }
    
    // Configurar opciones del socket
    resultado_udp_t resultado = configurar_socket_udp(contexto->socket_fd, &contexto->config);
    if (resultado != UDP_EXITO) {
        close(contexto->socket_fd);
        contexto->socket_fd = -1;
        return resultado;
    }
    
    // Si es receptor o bidireccional, hacer bind
    if (contexto->config.tipo == UDP_RECEPTOR || contexto->config.tipo == UDP_BIDIRECCIONAL) {
        memset(&contexto->direccion_local, 0, sizeof(contexto->direccion_local));
        contexto->direccion_local.sin_family = AF_INET;
        contexto->direccion_local.sin_port = htons(contexto->config.puerto);
        
        if (strcmp(contexto->config.host, "0.0.0.0") == 0) {
            contexto->direccion_local.sin_addr.s_addr = INADDR_ANY;
        } else {
            if (inet_aton(contexto->config.host, &contexto->direccion_local.sin_addr) == 0) {
                fprintf(stderr, "Dirección IP inválida: %s\n", contexto->config.host);
                close(contexto->socket_fd);
                contexto->socket_fd = -1;
                return UDP_ERROR_DIRECCION;
            }
        }
        
        if (bind(contexto->socket_fd, (struct sockaddr*)&contexto->direccion_local,
                 sizeof(contexto->direccion_local)) < 0) {
            perror("bind UDP");
            close(contexto->socket_fd);
            contexto->socket_fd = -1;
            return UDP_ERROR_BIND;
        }
        
        if (contexto->config.verbose) {
            printf("[UDP] Socket vinculado a %s:%d\n", 
                   contexto->config.host, contexto->config.puerto);
        }
    }
    
    contexto->inicializado = 1;
    
    if (contexto->config.verbose) {
        printf("[UDP] Contexto inicializado correctamente\n");
    }
    
    return UDP_EXITO;
}

// ============================================================================
// FUNCIONES DE EMISIÓN
// ============================================================================

resultado_udp_t enviar_mensaje_udp(udp_context_t* contexto, const char* mensaje,
                                   const char* host, int puerto) {
    if (!contexto || !mensaje || !host) return UDP_ERROR_PARAMETRO;
    
    if (!contexto->inicializado) {
        resultado_udp_t resultado = inicializar_udp(contexto);
        if (resultado != UDP_EXITO) return resultado;
    }
    
    // Configurar dirección de destino
    struct sockaddr_in destino;
    memset(&destino, 0, sizeof(destino));
    destino.sin_family = AF_INET;
    destino.sin_port = htons(puerto);
    
    if (inet_aton(host, &destino.sin_addr) == 0) {
        // Intentar resolver hostname
        struct hostent* he = gethostbyname(host);
        if (!he) {
            fprintf(stderr, "No se pudo resolver hostname: %s\n", host);
            actualizar_estadisticas(contexto, "error_envio", 0, 0);
            return UDP_ERROR_RESOLUCION;
        }
        memcpy(&destino.sin_addr, he->h_addr_list[0], he->h_length);
    }
    
    // Enviar mensaje
    size_t tamaño_mensaje = strlen(mensaje);
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    ssize_t bytes_enviados = sendto(contexto->socket_fd, mensaje, tamaño_mensaje, 0,
                                   (struct sockaddr*)&destino, sizeof(destino));
    
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_ms = (fin.tv_sec - inicio.tv_sec) * 1000.0 + 
                       (fin.tv_nsec - inicio.tv_nsec) / 1000000.0;
    
    if (bytes_enviados < 0) {
        perror("sendto");
        actualizar_estadisticas(contexto, "error_envio", 0, 0);
        return UDP_ERROR_SENDTO;
    }
    
    if ((size_t)bytes_enviados != tamaño_mensaje) {
        fprintf(stderr, "Advertencia: no se enviaron todos los bytes (%zd de %zu)\n",
                bytes_enviados, tamaño_mensaje);
    }
    
    // Actualizar estadísticas
    actualizar_estadisticas(contexto, "envio", bytes_enviados, tiempo_ms);
    
    if (contexto->config.verbose) {
        printf("[UDP] Enviado a %s:%d (%zd bytes, %.2f ms): \"%s\"\n",
               host, puerto, bytes_enviados, tiempo_ms, mensaje);
    }
    
    return UDP_EXITO;
}

resultado_udp_t enviar_mensaje_udp_completo(udp_context_t* contexto, mensaje_udp_t* msg_info) {
    if (!contexto || !msg_info || !msg_info->datos) return UDP_ERROR_PARAMETRO;
    
    char host_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &msg_info->destino.sin_addr, host_str, INET_ADDRSTRLEN);
    int puerto = ntohs(msg_info->destino.sin_port);
    
    return enviar_mensaje_udp(contexto, msg_info->datos, host_str, puerto);
}

resultado_udp_t enviar_mensaje_udp_confiable(udp_context_t* contexto, const char* mensaje,
                                            const char* host, int puerto, int esperar_respuesta) {
    if (!contexto || !mensaje || !host) return UDP_ERROR_PARAMETRO;
    
    resultado_udp_t resultado = UDP_ERROR_SISTEMA;
    
    for (int intento = 1; intento <= contexto->config.max_intentos; intento++) {
        if (contexto->config.verbose && intento > 1) {
            printf("[UDP] Reintento %d/%d\n", intento, contexto->config.max_intentos);
        }
        
        resultado = enviar_mensaje_udp(contexto, mensaje, host, puerto);
        
        if (resultado == UDP_EXITO) {
            if (esperar_respuesta) {
                // Esperar confirmación (implementación básica)
                char respuesta[256];
                struct sockaddr_in origen;
                
                int bytes_recibidos = recibir_mensaje_udp_timeout(contexto, respuesta, 
                                                                sizeof(respuesta), &origen, 
                                                                contexto->config.timeout_segundos * 1000);
                
                if (bytes_recibidos > 0) {
                    if (contexto->config.verbose) {
                        printf("[UDP] Confirmación recibida: \"%s\"\n", respuesta);
                    }
                    break;
                } else {
                    if (contexto->config.verbose) {
                        printf("[UDP] No se recibió confirmación, reintentando...\n");
                    }
                    resultado = UDP_ERROR_TIMEOUT;
                }
            } else {
                break; // Envío exitoso sin esperar respuesta
            }
        }
        
        if (intento < contexto->config.max_intentos) {
            usleep(500000); // 500ms entre reintentos
        }
    }
    
    return resultado;
}

resultado_udp_t enviar_broadcast_udp(udp_context_t* contexto, const char* mensaje, int puerto) {
    if (!contexto || !mensaje) return UDP_ERROR_PARAMETRO;
    
    // Habilitar broadcast si no está habilitado
    if (!contexto->config.broadcast) {
        contexto->config.broadcast = 1;
        if (contexto->inicializado) {
            int opcion = 1;
            if (setsockopt(contexto->socket_fd, SOL_SOCKET, SO_BROADCAST, 
                          &opcion, sizeof(opcion)) < 0) {
                perror("setsockopt SO_BROADCAST");
                return UDP_ERROR_SOCKET;
            }
        }
    }
    
    return enviar_mensaje_udp(contexto, mensaje, "255.255.255.255", puerto);
}

// ============================================================================
// FUNCIONES DE RECEPCIÓN
// ============================================================================

int recibir_mensaje_udp(udp_context_t* contexto, char* buffer, size_t tam_buffer,
                       struct sockaddr_in* origen) {
    if (!contexto || !buffer) return UDP_ERROR_PARAMETRO;
    
    if (!contexto->inicializado) {
        resultado_udp_t resultado = inicializar_udp(contexto);
        if (resultado != UDP_EXITO) return resultado;
    }
    
    struct sockaddr_in direccion_origen;
    socklen_t tam_origen = sizeof(direccion_origen);
    
    ssize_t bytes_recibidos = recvfrom(contexto->socket_fd, buffer, tam_buffer - 1, 0,
                                      (struct sockaddr*)&direccion_origen, &tam_origen);
    
    if (bytes_recibidos < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            actualizar_estadisticas(contexto, "timeout", 0, 0);
            return UDP_ERROR_TIMEOUT;
        }
        perror("recvfrom");
        actualizar_estadisticas(contexto, "error_recepcion", 0, 0);
        return UDP_ERROR_RECVFROM;
    }
    
    buffer[bytes_recibidos] = '\0';
    
    // Verificar si el mensaje fue truncado
    if ((size_t)bytes_recibidos == tam_buffer - 1) {
        actualizar_estadisticas(contexto, "truncado", 0, 0);
        if (contexto->config.verbose) {
            printf("[UDP] Advertencia: mensaje posiblemente truncado\n");
        }
    }
    
    // Actualizar estadísticas
    actualizar_estadisticas(contexto, "recepcion", bytes_recibidos, 0);
    
    // Copiar información de origen si se solicita
    if (origen) {
        *origen = direccion_origen;
    }
    
    if (contexto->config.verbose) {
        char ip_origen[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &direccion_origen.sin_addr, ip_origen, INET_ADDRSTRLEN);
        printf("[UDP] Recibido de %s:%d (%zd bytes): \"%s\"\n",
               ip_origen, ntohs(direccion_origen.sin_port), bytes_recibidos, buffer);
    }
    
    return (int)bytes_recibidos;
}

resultado_udp_t recibir_mensaje_udp_completo(udp_context_t* contexto, mensaje_udp_t* msg_info) {
    if (!contexto || !msg_info) return UDP_ERROR_PARAMETRO;
    
    // Usar buffer interno si no se proporciona
    if (!msg_info->datos) {
        msg_info->datos = contexto->buffer_interno;
        msg_info->tamaño = contexto->config.buffer_size;
    }
    
    int bytes_recibidos = recibir_mensaje_udp(contexto, msg_info->datos, msg_info->tamaño,
                                             &msg_info->origen);
    
    if (bytes_recibidos < 0) {
        return (resultado_udp_t)bytes_recibidos;
    }
    
    msg_info->tamaño = bytes_recibidos;
    msg_info->timestamp = time(NULL);
    
    return UDP_EXITO;
}

int recibir_mensaje_udp_timeout(udp_context_t* contexto, char* buffer, size_t tam_buffer,
                               struct sockaddr_in* origen, int timeout_ms) {
    if (!contexto || !buffer) return UDP_ERROR_PARAMETRO;
    
    if (!contexto->inicializado) {
        resultado_udp_t resultado = inicializar_udp(contexto);
        if (resultado != UDP_EXITO) return resultado;
    }
    
    // Configurar timeout temporal
    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    
    if (setsockopt(contexto->socket_fd, SOL_SOCKET, SO_RCVTIMEO, 
                   &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt SO_RCVTIMEO");
        return UDP_ERROR_SOCKET;
    }
    
    int resultado = recibir_mensaje_udp(contexto, buffer, tam_buffer, origen);
    
    // Restaurar timeout original
    configurar_timeout_socket(contexto->socket_fd, contexto->config.timeout_segundos);
    
    return resultado;
}

resultado_udp_t escuchar_mensajes_udp(udp_context_t* contexto,
                                     void (*callback)(const mensaje_udp_t*, void*),
                                     void* user_data) {
    if (!contexto || !callback) return UDP_ERROR_PARAMETRO;
    
    if (contexto->config.verbose) {
        printf("[UDP] Iniciando escucha de mensajes...\n");
    }
    
    while (1) {
        mensaje_udp_t mensaje;
        memset(&mensaje, 0, sizeof(mensaje));
        mensaje.datos = contexto->buffer_interno;
        mensaje.tamaño = contexto->config.buffer_size;
        
        resultado_udp_t resultado = recibir_mensaje_udp_completo(contexto, &mensaje);
        
        if (resultado == UDP_EXITO) {
            callback(&mensaje, user_data);
        } else if (resultado == UDP_ERROR_TIMEOUT) {
            // Timeout normal, continuar
            continue;
        } else {
            // Error real
            return resultado;
        }
    }
    
    return UDP_EXITO;
}

// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

void obtener_estadisticas_udp(const udp_context_t* contexto, estadisticas_udp_t* stats) {
    if (!contexto || !stats) return;
    
    *stats = contexto->stats;
}

void resetear_estadisticas_udp(udp_context_t* contexto) {
    if (!contexto) return;
    
    memset(&contexto->stats, 0, sizeof(estadisticas_udp_t));
    contexto->stats.tiempo_inicio = time(NULL);
}

const char* udp_strerror(resultado_udp_t codigo) {
    switch (codigo) {
        case UDP_EXITO: return "Operación exitosa";
        case UDP_ERROR_SOCKET: return "Error creando socket";
        case UDP_ERROR_BIND: return "Error en bind";
        case UDP_ERROR_SENDTO: return "Error enviando datagrama";
        case UDP_ERROR_RECVFROM: return "Error recibiendo datagrama";
        case UDP_ERROR_TIMEOUT: return "Timeout de operación";
        case UDP_ERROR_DIRECCION: return "Dirección inválida";
        case UDP_ERROR_PARAMETRO: return "Parámetro inválido";
        case UDP_ERROR_MEMORIA: return "Error de memoria";
        case UDP_ERROR_SISTEMA: return "Error del sistema";
        case UDP_MENSAJE_TRUNCADO: return "Mensaje truncado";
        case UDP_ERROR_RESOLUCION: return "Error resolviendo hostname";
        default: return "Error desconocido";
    }
}

char* direccion_a_string(const struct sockaddr_in* addr, char* buffer, size_t tam_buffer) {
    if (!addr || !buffer || tam_buffer < INET_ADDRSTRLEN + 8) return NULL;
    
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr->sin_addr, ip_str, INET_ADDRSTRLEN);
    snprintf(buffer, tam_buffer, "%s:%d", ip_str, ntohs(addr->sin_port));
    
    return buffer;
}

resultado_udp_t resolver_hostname(const char* hostname, char* ip_buffer, size_t tam_buffer) {
    if (!hostname || !ip_buffer || tam_buffer < INET_ADDRSTRLEN) {
        return UDP_ERROR_PARAMETRO;
    }
    
    struct hostent* he = gethostbyname(hostname);
    if (!he) {
        return UDP_ERROR_RESOLUCION;
    }
    
    struct in_addr addr;
    memcpy(&addr, he->h_addr_list[0], he->h_length);
    
    if (!inet_ntop(AF_INET, &addr, ip_buffer, tam_buffer)) {
        return UDP_ERROR_SISTEMA;
    }
    
    return UDP_EXITO;
}

int validar_ip(const char* ip_string) {
    if (!ip_string) return 0;
    
    struct in_addr addr;
    return inet_aton(ip_string, &addr);
}

void imprimir_estadisticas_udp(const udp_context_t* contexto, FILE* archivo) {
    if (!contexto) return;
    if (!archivo) archivo = stdout;
    
    estadisticas_udp_t stats;
    obtener_estadisticas_udp(contexto, &stats);
    
    time_t tiempo_actual = time(NULL);
    double tiempo_ejecucion = difftime(tiempo_actual, stats.tiempo_inicio);
    
    fprintf(archivo, "\n=== ESTADÍSTICAS UDP ===\n");
    fprintf(archivo, "Tiempo de ejecución: %.0f segundos\n", tiempo_ejecucion);
    fprintf(archivo, "Mensajes enviados: %d\n", stats.mensajes_enviados);
    fprintf(archivo, "Mensajes recibidos: %d\n", stats.mensajes_recibidos);
    fprintf(archivo, "Bytes enviados: %d\n", stats.bytes_enviados);
    fprintf(archivo, "Bytes recibidos: %d\n", stats.bytes_recibidos);
    fprintf(archivo, "Errores de envío: %d\n", stats.errores_envio);
    fprintf(archivo, "Errores de recepción: %d\n", stats.errores_recepcion);
    fprintf(archivo, "Timeouts: %d\n", stats.timeouts);
    fprintf(archivo, "Mensajes truncados: %d\n", stats.mensajes_truncados);
    fprintf(archivo, "RTT promedio: %.2f ms\n", stats.rtt_promedio_ms);
    fprintf(archivo, "Throughput: %.2f KB/s\n", stats.throughput_kbps);
    
    if (stats.mensajes_enviados > 0 && tiempo_ejecucion > 0) {
        fprintf(archivo, "Mensajes por segundo: %.2f\n", 
               (stats.mensajes_enviados + stats.mensajes_recibidos) / tiempo_ejecucion);
    }
    fprintf(archivo, "=======================\n\n");
}

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// ============================================================================

resultado_udp_t demo_emisor_udp(const char* host, int puerto, const char* mensaje) {
    printf("\n=== DEMO: Emisor UDP ===\n");
    
    config_udp_t config = config_udp_emisor(host, puerto);
    udp_context_t* contexto = crear_contexto_udp(&config);
    
    if (!contexto) {
        fprintf(stderr, "Error creando contexto UDP\n");
        return UDP_ERROR_MEMORIA;
    }
    
    resultado_udp_t resultado = enviar_mensaje_udp(contexto, mensaje, host, puerto);
    
    if (resultado == UDP_EXITO) {
        printf("✅ Mensaje enviado exitosamente\n");
    } else {
        printf("❌ Error enviando mensaje: %s\n", udp_strerror(resultado));
    }
    
    imprimir_estadisticas_udp(contexto, NULL);
    destruir_contexto_udp(contexto);
    
    return resultado;
}

resultado_udp_t demo_receptor_udp(int puerto, int num_mensajes) {
    printf("\n=== DEMO: Receptor UDP ===\n");
    
    config_udp_t config = CONFIG_UDP_RECEPTOR(puerto);
    udp_context_t* contexto = crear_contexto_udp(&config);
    
    if (!contexto) {
        fprintf(stderr, "Error creando contexto UDP\n");
        return UDP_ERROR_MEMORIA;
    }
    
    resultado_udp_t resultado = inicializar_udp(contexto);
    if (resultado != UDP_EXITO) {
        printf("❌ Error inicializando receptor: %s\n", udp_strerror(resultado));
        destruir_contexto_udp(contexto);
        return resultado;
    }
    
    printf("📡 Escuchando en puerto %d...\n", puerto);
    if (num_mensajes > 0) {
        printf("Esperando %d mensajes\n", num_mensajes);
    } else {
        printf("Presiona Ctrl+C para detener\n");
    }
    
    char buffer[BUFFER_UDP_ESTANDAR];
    struct sockaddr_in origen;
    int mensajes_recibidos = 0;
    
    while (num_mensajes == 0 || mensajes_recibidos < num_mensajes) {
        int bytes = recibir_mensaje_udp(contexto, buffer, sizeof(buffer), &origen);
        
        if (bytes > 0) {
            mensajes_recibidos++;
            printf("[%d] Mensaje recibido\n", mensajes_recibidos);
        } else if (bytes == UDP_ERROR_TIMEOUT) {
            printf("⏰ Timeout, continuando...\n");
            continue;
        } else {
            printf("❌ Error recibiendo: %s\n", udp_strerror((resultado_udp_t)bytes));
            break;
        }
    }
    
    imprimir_estadisticas_udp(contexto, NULL);
    destruir_contexto_udp(contexto);
    
    return UDP_EXITO;
}

resultado_udp_t demo_bidireccional_udp(const char* host, int puerto) {
    printf("\n=== DEMO: Comunicación Bidireccional UDP ===\n");
    
    config_udp_t config = CONFIG_UDP_DEFECTO;
    strcpy(config.host, host);
    config.puerto = puerto;
    config.tipo = UDP_BIDIRECCIONAL;
    config.verbose = 1;
    
    udp_context_t* contexto = crear_contexto_udp(&config);
    if (!contexto) return UDP_ERROR_MEMORIA;
    
    resultado_udp_t resultado = inicializar_udp(contexto);
    if (resultado != UDP_EXITO) {
        destruir_contexto_udp(contexto);
        return resultado;
    }
    
    // Enviar mensaje
    const char* mensaje = "Hola desde UDP bidireccional";
    resultado = enviar_mensaje_udp(contexto, mensaje, host, puerto);
    
    if (resultado == UDP_EXITO) {
        printf("✅ Mensaje enviado\n");
        
        // Intentar recibir respuesta
        char respuesta[BUFFER_UDP_ESTANDAR];
        struct sockaddr_in origen;
        
        printf("Esperando respuesta...\n");
        int bytes = recibir_mensaje_udp_timeout(contexto, respuesta, sizeof(respuesta),
                                               &origen, 5000); // 5 segundos
        
        if (bytes > 0) {
            printf("✅ Respuesta recibida: \"%s\"\n", respuesta);
        } else {
            printf("⏰ No se recibió respuesta\n");
        }
    }
    
    imprimir_estadisticas_udp(contexto, NULL);
    destruir_contexto_udp(contexto);
    
    return resultado;
}

resultado_udp_t demo_broadcast_udp(int puerto, const char* mensaje) {
    printf("\n=== DEMO: Broadcast UDP ===\n");
    
    config_udp_t config = CONFIG_UDP_DEFECTO;
    config.puerto = puerto;
    config.broadcast = 1;
    config.verbose = 1;
    
    udp_context_t* contexto = crear_contexto_udp(&config);
    if (!contexto) return UDP_ERROR_MEMORIA;
    
    resultado_udp_t resultado = enviar_broadcast_udp(contexto, mensaje, puerto);
    
    if (resultado == UDP_EXITO) {
        printf("✅ Broadcast enviado a puerto %d\n", puerto);
    } else {
        printf("❌ Error en broadcast: %s\n", udp_strerror(resultado));
    }
    
    imprimir_estadisticas_udp(contexto, NULL);
    destruir_contexto_udp(contexto);
    
    return resultado;
}

resultado_udp_t demo_performance_udp(const char* host, int puerto, 
                                    int num_mensajes, int tamaño_mensaje) {
    printf("\n=== DEMO: Performance UDP ===\n");
    printf("Enviando %d mensajes de %d bytes a %s:%d\n", 
           num_mensajes, tamaño_mensaje, host, puerto);
    
    config_udp_t config = config_udp_emisor(host, puerto);
    config.verbose = 0; // Sin verbose para mejor performance
    
    udp_context_t* contexto = crear_contexto_udp(&config);
    if (!contexto) return UDP_ERROR_MEMORIA;
    
    // Crear mensaje de prueba
    char* mensaje = malloc(tamaño_mensaje + 1);
    if (!mensaje) {
        destruir_contexto_udp(contexto);
        return UDP_ERROR_MEMORIA;
    }
    
    memset(mensaje, 'A', tamaño_mensaje);
    mensaje[tamaño_mensaje] = '\0';
    
    // Medir tiempo total
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    int exitosos = 0;
    for (int i = 0; i < num_mensajes; i++) {
        if (enviar_mensaje_udp(contexto, mensaje, host, puerto) == UDP_EXITO) {
            exitosos++;
        }
        
        // Pequeña pausa para no saturar
        if (i % 100 == 0 && i > 0) {
            usleep(1000); // 1ms cada 100 mensajes
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_total = (fin.tv_sec - inicio.tv_sec) + 
                         (fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    
    printf("\n=== RESULTADOS DE PERFORMANCE ===\n");
    printf("Mensajes enviados: %d/%d (%.1f%% éxito)\n", 
           exitosos, num_mensajes, (exitosos * 100.0) / num_mensajes);
    printf("Tiempo total: %.3f segundos\n", tiempo_total);
    printf("Mensajes por segundo: %.2f\n", exitosos / tiempo_total);
    printf("Throughput: %.2f KB/s\n", (exitosos * tamaño_mensaje / 1024.0) / tiempo_total);
    printf("Latencia promedio: %.3f ms\n", (tiempo_total * 1000) / exitosos);
    
    free(mensaje);
    imprimir_estadisticas_udp(contexto, NULL);
    destruir_contexto_udp(contexto);
    
    return UDP_EXITO;
}

// ============================================================================
// HERRAMIENTAS DE COMPARACIÓN TCP vs UDP
// ============================================================================

resultado_udp_t comparar_tcp_vs_udp(const char* host, int puerto_tcp, int puerto_udp,
                                   int num_mensajes, int tamaño_mensaje,
                                   comparacion_tcp_udp_t* resultado) {
    if (!host || !resultado) return UDP_ERROR_PARAMETRO;
    
    printf("\n=== COMPARACIÓN TCP vs UDP ===\n");
    printf("Host: %s, Mensajes: %d, Tamaño: %d bytes\n", 
           host, num_mensajes, tamaño_mensaje);
    
    memset(resultado, 0, sizeof(comparacion_tcp_udp_t));
    
    // Crear mensaje de prueba
    char* mensaje = malloc(tamaño_mensaje + 1);
    if (!mensaje) return UDP_ERROR_MEMORIA;
    memset(mensaje, 'X', tamaño_mensaje);
    mensaje[tamaño_mensaje] = '\0';
    
    // Test UDP
    printf("\n--- Test UDP ---\n");
    config_udp_t config_udp = config_udp_emisor(host, puerto_udp);
    config_udp.verbose = 0;
    
    udp_context_t* contexto_udp = crear_contexto_udp(&config_udp);
    if (!contexto_udp) {
        free(mensaje);
        return UDP_ERROR_MEMORIA;
    }
    
    struct timespec inicio_udp, fin_udp;
    clock_gettime(CLOCK_MONOTONIC, &inicio_udp);
    
    int exitosos_udp = 0;
    for (int i = 0; i < num_mensajes; i++) {
        if (enviar_mensaje_udp(contexto_udp, mensaje, host, puerto_udp) == UDP_EXITO) {
            exitosos_udp++;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &fin_udp);
    resultado->tiempo_udp_ms = (fin_udp.tv_sec - inicio_udp.tv_sec) * 1000.0 + 
                               (fin_udp.tv_nsec - inicio_udp.tv_nsec) / 1000000.0;
    
    estadisticas_udp_t stats_udp;
    obtener_estadisticas_udp(contexto_udp, &stats_udp);
    resultado->throughput_udp_kbps = stats_udp.throughput_kbps;
    resultado->perdidas_udp = num_mensajes - exitosos_udp;
    
    destruir_contexto_udp(contexto_udp);
    
    printf("UDP completado: %d/%d mensajes en %.3f ms\n", 
           exitosos_udp, num_mensajes, resultado->tiempo_udp_ms);
    
    // Nota: Test TCP requeriría implementación del cliente TCP
    printf("Nota: Test TCP requiere servidor TCP ejecutándose\n");
    resultado->tiempo_tcp_ms = -1; // Indicar que no se ejecutó
    
    free(mensaje);
    return UDP_EXITO;
}

void imprimir_comparacion_tcp_udp(const comparacion_tcp_udp_t* comparacion, FILE* archivo) {
    if (!comparacion) return;
    if (!archivo) archivo = stdout;
    
    fprintf(archivo, "\n=== COMPARACIÓN TCP vs UDP ===\n");
    
    if (comparacion->tiempo_tcp_ms >= 0) {
        fprintf(archivo, "TCP - Tiempo: %.3f ms, Throughput: %.2f KB/s\n",
                comparacion->tiempo_tcp_ms, comparacion->throughput_tcp_kbps);
    } else {
        fprintf(archivo, "TCP - No ejecutado\n");
    }
    
    fprintf(archivo, "UDP - Tiempo: %.3f ms, Throughput: %.2f KB/s\n",
            comparacion->tiempo_udp_ms, comparacion->throughput_udp_kbps);
    fprintf(archivo, "UDP - Pérdidas: %d, Reordenamientos: %d\n",
            comparacion->perdidas_udp, comparacion->reordenamientos_udp);
    
    if (comparacion->tiempo_tcp_ms >= 0) {
        fprintf(archivo, "\nDiferencias:\n");
        fprintf(archivo, "- UDP es %.2fx más rápido que TCP\n",
                comparacion->tiempo_tcp_ms / comparacion->tiempo_udp_ms);
        fprintf(archivo, "- UDP tiene %.2fx más throughput que TCP\n",
                comparacion->throughput_udp_kbps / comparacion->throughput_tcp_kbps);
    }
    
    fprintf(archivo, "===============================\n\n");
}

// ============================================================================
// FUNCIONES AUXILIARES PARA TESTING
// ============================================================================

resultado_udp_t crear_receptor_testing(int puerto, int timeout_segundos, int respuesta_automatica) {
    config_udp_t config = CONFIG_UDP_RECEPTOR(puerto);
    config.timeout_segundos = timeout_segundos;
    config.verbose = 1;
    
    udp_context_t* contexto = crear_contexto_udp(&config);
    if (!contexto) return UDP_ERROR_MEMORIA;
    
    resultado_udp_t resultado = inicializar_udp(contexto);
    if (resultado != UDP_EXITO) {
        destruir_contexto_udp(contexto);
        return resultado;
    }
    
    printf("[TEST] Receptor UDP escuchando en puerto %d\n", puerto);
    
    char buffer[BUFFER_UDP_ESTANDAR];
    struct sockaddr_in origen;
    
    while (1) {
        int bytes = recibir_mensaje_udp(contexto, buffer, sizeof(buffer), &origen);
        
        if (bytes > 0) {
            printf("[TEST] Recibido: \"%s\"\n", buffer);
            
            if (respuesta_automatica) {
                char respuesta[256];
                snprintf(respuesta, sizeof(respuesta), "ACK: %s", buffer);
                
                char ip_origen[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &origen.sin_addr, ip_origen, INET_ADDRSTRLEN);
                
                enviar_mensaje_udp(contexto, respuesta, ip_origen, ntohs(origen.sin_port));
            }
        } else if (bytes == UDP_ERROR_TIMEOUT) {
            printf("[TEST] Timeout, terminando...\n");
            break;
        } else {
            printf("[TEST] Error: %s\n", udp_strerror((resultado_udp_t)bytes));
            break;
        }
    }
    
    destruir_contexto_udp(contexto);
    return UDP_EXITO;
}

resultado_udp_t crear_emisor_testing(const char* host, int puerto, 
                                    const char** mensajes, int num_mensajes, 
                                    int intervalo_ms) {
    config_udp_t config = config_udp_emisor(host, puerto);
    udp_context_t* contexto = crear_contexto_udp(&config);
    
    if (!contexto) return UDP_ERROR_MEMORIA;
    
    printf("[TEST] Emisor UDP enviando %d mensajes a %s:%d\n", 
           num_mensajes, host, puerto);
    
    for (int i = 0; i < num_mensajes; i++) {
        const char* mensaje = mensajes[i];
        resultado_udp_t resultado = enviar_mensaje_udp(contexto, mensaje, host, puerto);
        
        if (resultado == UDP_EXITO) {
            printf("[TEST] Enviado %d/%d: \"%s\"\n", i + 1, num_mensajes, mensaje);
        } else {
            printf("[TEST] Error enviando %d/%d: %s\n", 
                   i + 1, num_mensajes, udp_strerror(resultado));
        }
        
        if (i < num_mensajes - 1 && intervalo_ms > 0) {
            usleep(intervalo_ms * 1000);
        }
    }
    
    imprimir_estadisticas_udp(contexto, NULL);
    destruir_contexto_udp(contexto);
    
    return UDP_EXITO;
}
