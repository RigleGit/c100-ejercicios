/**
 * @file cliente_tcp.c
 * @brief Implementación completa del cliente TCP educativo
 * @description Implementación robusta y educativa de un cliente TCP que demuestra
 *              los fundamentos de sockets BSD, manejo de errores, timeouts,
 *              reconexión automática y métricas de rendimiento.
 * @version 1.0
 * @date 2024
 * @author Estudiante de C
 */

#include "../include/cliente_tcp.h"
#include <sys/wait.h>

// Variables globales para manejo de señales
static volatile int señal_interrupcion = 0;

/**
 * @brief Manejador de señales para interrupción
 */
static void manejador_señal(int sig) {
    (void)sig; // Evitar warning de parámetro no usado
    señal_interrupcion = 1;
}

/**
 * @brief Obtener tiempo actual en milisegundos
 */
static double obtener_tiempo_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

/**
 * @brief Configurar timeout para socket
 */
static cliente_estado_t configurar_timeout_socket(int socket_fd, int timeout_segundos) {
    struct timeval timeout;
    timeout.tv_sec = timeout_segundos;
    timeout.tv_usec = 0;
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        return CLIENTE_ERROR_SOCKET;
    }
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        return CLIENTE_ERROR_SOCKET;
    }
    
    return CLIENTE_OK;
}

/**
 * @brief Resolver hostname a dirección IP
 */
static cliente_estado_t resolver_hostname(const char *hostname, char *ip_str, size_t ip_len) {
    struct hostent *host_entry;
    struct in_addr addr;
    
    // Verificar si ya es una IP
    if (inet_pton(AF_INET, hostname, &addr) == 1) {
        strncpy(ip_str, hostname, ip_len - 1);
        ip_str[ip_len - 1] = '\0';
        return CLIENTE_OK;
    }
    
    // Resolver hostname
    host_entry = gethostbyname(hostname);
    if (host_entry == NULL) {
        return CLIENTE_ERROR_CONEXION;
    }
    
    // Convertir a string
    char *ip = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    strncpy(ip_str, ip, ip_len - 1);
    ip_str[ip_len - 1] = '\0';
    
    return CLIENTE_OK;
}

// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES PRINCIPALES
// =============================================================================

cliente_tcp_t *cliente_crear(const char *servidor, int puerto) {
    if (!servidor || puerto <= 0 || puerto > 65535) {
        CLIENTE_ERROR_LOG("Parámetros inválidos: servidor=%s, puerto=%d", 
                         servidor ? servidor : "NULL", puerto);
        return NULL;
    }
    
    cliente_tcp_t *cliente = malloc(sizeof(cliente_tcp_t));
    if (!cliente) {
        CLIENTE_ERROR_LOG("No se pudo asignar memoria para el cliente");
        return NULL;
    }
    
    // Inicializar estructura
    memset(cliente, 0, sizeof(cliente_tcp_t));
    cliente->socket_fd = -1;
    cliente->conectado = 0;
    
    // Configuración por defecto
    strncpy(cliente->config.servidor, servidor, sizeof(cliente->config.servidor) - 1);
    cliente->config.servidor[sizeof(cliente->config.servidor) - 1] = '\0';
    cliente->config.puerto = puerto;
    cliente->config.timeout_conexion = TIMEOUT_CONEXION;
    cliente->config.timeout_recv = TIMEOUT_RECV;
    cliente->config.intentos_reconexion = INTENTOS_RECONEXION;
    cliente->config.retraso_reconexion = RETRASO_RECONEXION;
    cliente->config.modo_verbose = 0;
    cliente->config.keepalive = 1;
    
    // Configurar dirección del servidor
    memset(&cliente->servidor_addr, 0, sizeof(cliente->servidor_addr));
    cliente->servidor_addr.sin_family = AF_INET;
    cliente->servidor_addr.sin_port = htons(puerto);
    
    // Resolver dirección
    char ip_resuelto[INET_ADDRSTRLEN];
    if (resolver_hostname(servidor, ip_resuelto, sizeof(ip_resuelto)) != CLIENTE_OK) {
        CLIENTE_ERROR_LOG("No se pudo resolver el hostname: %s", servidor);
        free(cliente);
        return NULL;
    }
    
    if (inet_pton(AF_INET, ip_resuelto, &cliente->servidor_addr.sin_addr) <= 0) {
        CLIENTE_ERROR_LOG("Dirección IP inválida: %s", ip_resuelto);
        free(cliente);
        return NULL;
    }
    
    // Configurar manejador de señales
    signal(SIGPIPE, SIG_IGN); // Ignorar SIGPIPE
    signal(SIGINT, manejador_señal);
    
    return cliente;
}

void cliente_destruir(cliente_tcp_t *cliente) {
    if (!cliente) {
        return;
    }
    
    if (cliente->conectado) {
        cliente_desconectar(cliente);
    }
    
    free(cliente);
}

cliente_estado_t cliente_configurar(cliente_tcp_t *cliente, const cliente_config_t *config) {
    CLIENTE_VERIFICAR_PARAMETROS(cliente);
    
    if (!config) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    // Copiar configuración
    cliente->config = *config;
    
    // Validar parámetros
    if (cliente->config.timeout_conexion <= 0) {
        cliente->config.timeout_conexion = TIMEOUT_CONEXION;
    }
    
    if (cliente->config.timeout_recv <= 0) {
        cliente->config.timeout_recv = TIMEOUT_RECV;
    }
    
    if (cliente->config.intentos_reconexion < 0) {
        cliente->config.intentos_reconexion = 0;
    }
    
    if (cliente->config.retraso_reconexion <= 0) {
        cliente->config.retraso_reconexion = RETRASO_RECONEXION;
    }
    
    CLIENTE_LOG(cliente, "Configuración actualizada");
    return CLIENTE_OK;
}

cliente_estado_t cliente_conectar(cliente_tcp_t *cliente) {
    CLIENTE_VERIFICAR_PARAMETROS(cliente);
    
    if (cliente->conectado) {
        CLIENTE_LOG(cliente, "Ya está conectado");
        return CLIENTE_OK;
    }
    
    // Crear socket
    cliente->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (cliente->socket_fd < 0) {
        CLIENTE_ERROR_LOG("Error creando socket: %s", strerror(errno));
        return CLIENTE_ERROR_SOCKET;
    }
    
    CLIENTE_LOG(cliente, "Socket creado exitosamente (fd=%d)", cliente->socket_fd);
    
    // Configurar timeouts
    if (configurar_timeout_socket(cliente->socket_fd, cliente->config.timeout_conexion) != CLIENTE_OK) {
        CLIENTE_ERROR_LOG("Error configurando timeouts del socket");
        close(cliente->socket_fd);
        cliente->socket_fd = -1;
        return CLIENTE_ERROR_SOCKET;
    }
    
    // Configurar keep-alive si está habilitado
    if (cliente->config.keepalive) {
        int optval = 1;
        if (setsockopt(cliente->socket_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
            CLIENTE_LOG(cliente, "Advertencia: No se pudo habilitar keep-alive");
        }
    }
    
    // Conectar al servidor
    CLIENTE_LOG(cliente, "Conectando a %s:%d...", 
               cliente->config.servidor, cliente->config.puerto);
    
    double tiempo_inicio = obtener_tiempo_ms();
    
    if (connect(cliente->socket_fd, (struct sockaddr*)&cliente->servidor_addr, 
                sizeof(cliente->servidor_addr)) < 0) {
        CLIENTE_ERROR_LOG("Error conectando al servidor %s:%d: %s", 
                         cliente->config.servidor, cliente->config.puerto, strerror(errno));
        close(cliente->socket_fd);
        cliente->socket_fd = -1;
        return CLIENTE_ERROR_CONEXION;
    }
    
    double tiempo_conexion = obtener_tiempo_ms() - tiempo_inicio;
    
    cliente->conectado = 1;
    cliente->tiempo_conexion = time(NULL);
    
    CLIENTE_LOG(cliente, "Conectado exitosamente en %.2f ms", tiempo_conexion);
    
    return CLIENTE_OK;
}

cliente_estado_t cliente_desconectar(cliente_tcp_t *cliente) {
    CLIENTE_VERIFICAR_PARAMETROS(cliente);
    
    if (!cliente->conectado) {
        return CLIENTE_OK;
    }
    
    if (cliente->socket_fd >= 0) {
        // Enviar señal de cierre
        shutdown(cliente->socket_fd, SHUT_RDWR);
        close(cliente->socket_fd);
        cliente->socket_fd = -1;
    }
    
    cliente->conectado = 0;
    
    CLIENTE_LOG(cliente, "Desconectado del servidor");
    
    return CLIENTE_OK;
}

int cliente_enviar(cliente_tcp_t *cliente, const char *mensaje, size_t longitud) {
    if (!cliente || !mensaje || longitud == 0) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    if (!cliente->conectado) {
        CLIENTE_ERROR_LOG("Cliente no está conectado");
        return CLIENTE_ERROR_CONEXION;
    }
    
    CLIENTE_LOG(cliente, "Enviando %zu bytes...", longitud);
    
    ssize_t bytes_enviados = send(cliente->socket_fd, mensaje, longitud, 0);
    if (bytes_enviados < 0) {
        CLIENTE_ERROR_LOG("Error enviando datos: %s", strerror(errno));
        return CLIENTE_ERROR_ENVIO;
    }
    
    cliente->bytes_enviados += bytes_enviados;
    cliente->mensajes_enviados++;
    
    CLIENTE_LOG(cliente, "Enviados %zd bytes exitosamente", bytes_enviados);
    
    return (int)bytes_enviados;
}

int cliente_recibir(cliente_tcp_t *cliente, char *buffer, size_t tamaño_buffer) {
    if (!cliente || !buffer || tamaño_buffer == 0) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    if (!cliente->conectado) {
        CLIENTE_ERROR_LOG("Cliente no está conectado");
        return CLIENTE_ERROR_CONEXION;
    }
    
    CLIENTE_LOG(cliente, "Esperando respuesta...");
    
    ssize_t bytes_recibidos = recv(cliente->socket_fd, buffer, tamaño_buffer - 1, 0);
    if (bytes_recibidos < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            CLIENTE_ERROR_LOG("Timeout recibiendo datos");
            return CLIENTE_ERROR_TIMEOUT;
        }
        CLIENTE_ERROR_LOG("Error recibiendo datos: %s", strerror(errno));
        return CLIENTE_ERROR_RECEPCION;
    }
    
    if (bytes_recibidos == 0) {
        CLIENTE_LOG(cliente, "Servidor cerró la conexión");
        cliente->conectado = 0;
        return 0;
    }
    
    buffer[bytes_recibidos] = '\0'; // Asegurar terminación
    cliente->bytes_recibidos += bytes_recibidos;
    cliente->mensajes_recibidos++;
    
    CLIENTE_LOG(cliente, "Recibidos %zd bytes: \"%.100s%s\"", 
               bytes_recibidos, buffer, bytes_recibidos > 100 ? "..." : "");
    
    return (int)bytes_recibidos;
}

cliente_estado_t cliente_transaccion(cliente_tcp_t *cliente, 
                                    const char *mensaje,
                                    char *respuesta, 
                                    size_t tamaño_respuesta,
                                    transaccion_t *transaccion) {
    CLIENTE_VERIFICAR_PARAMETROS(cliente);
    
    if (!mensaje || !respuesta || tamaño_respuesta == 0) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    // Inicializar transacción si se proporciona
    if (transaccion) {
        memset(transaccion, 0, sizeof(transaccion_t));
        strncpy(transaccion->mensaje_enviado, mensaje, sizeof(transaccion->mensaje_enviado) - 1);
        transaccion->mensaje_enviado[sizeof(transaccion->mensaje_enviado) - 1] = '\0';
    }
    
    double tiempo_inicio = obtener_tiempo_ms();
    
    // Enviar mensaje
    int bytes_enviados = cliente_enviar(cliente, mensaje, strlen(mensaje));
    if (bytes_enviados < 0) {
        if (transaccion) {
            transaccion->estado = (cliente_estado_t)bytes_enviados;
        }
        return (cliente_estado_t)bytes_enviados;
    }
    
    // Recibir respuesta
    int bytes_recibidos = cliente_recibir(cliente, respuesta, tamaño_respuesta);
    if (bytes_recibidos < 0) {
        if (transaccion) {
            transaccion->estado = (cliente_estado_t)bytes_recibidos;
        }
        return (cliente_estado_t)bytes_recibidos;
    }
    
    double tiempo_fin = obtener_tiempo_ms();
    
    // Completar información de transacción
    if (transaccion) {
        transaccion->bytes_enviados = bytes_enviados;
        transaccion->bytes_recibidos = bytes_recibidos;
        transaccion->tiempo_respuesta = tiempo_fin - tiempo_inicio;
        transaccion->estado = CLIENTE_OK;
        strncpy(transaccion->respuesta_recibida, respuesta, sizeof(transaccion->respuesta_recibida) - 1);
        transaccion->respuesta_recibida[sizeof(transaccion->respuesta_recibida) - 1] = '\0';
    }
    
    return CLIENTE_OK;
}

// =============================================================================
// FUNCIONES DE UTILIDAD Y DIAGNÓSTICO
// =============================================================================

int cliente_esta_conectado(const cliente_tcp_t *cliente) {
    if (!cliente) {
        return 0;
    }
    
    return cliente->conectado;
}

cliente_estado_t cliente_obtener_estadisticas(const cliente_tcp_t *cliente, 
                                             estadisticas_cliente_t *estadisticas) {
    CLIENTE_VERIFICAR_PARAMETROS(cliente);
    
    if (!estadisticas) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    memset(estadisticas, 0, sizeof(estadisticas_cliente_t));
    
    // Estadísticas básicas (simplificado para este ejemplo)
    estadisticas->conexiones_exitosas = cliente->conectado ? 1 : 0;
    estadisticas->conexiones_fallidas = 0; // Se podría implementar contador
    estadisticas->transacciones_exitosas = cliente->mensajes_recibidos;
    estadisticas->transacciones_fallidas = cliente->mensajes_enviados - cliente->mensajes_recibidos;
    estadisticas->total_bytes_enviados = cliente->bytes_enviados;
    estadisticas->total_bytes_recibidos = cliente->bytes_recibidos;
    
    // Calcular promedios (simplificado)
    if (cliente->mensajes_recibidos > 0) {
        estadisticas->tiempo_respuesta_promedio = 100.0; // Placeholder
    }
    
    return CLIENTE_OK;
}

void cliente_mostrar_estadisticas(const cliente_tcp_t *cliente) {
    if (!cliente) {
        printf("Cliente es NULL\n");
        return;
    }
    
    printf("\n=== ESTADÍSTICAS DEL CLIENTE TCP ===\n");
    printf("Servidor: %s:%d\n", cliente->config.servidor, cliente->config.puerto);
    printf("Estado: %s\n", cliente->conectado ? "Conectado" : "Desconectado");
    
    if (cliente->conectado) {
        time_t tiempo_actual = time(NULL);
        double duracion = difftime(tiempo_actual, cliente->tiempo_conexion);
        printf("Tiempo conectado: %.0f segundos\n", duracion);
    }
    
    printf("Mensajes enviados: %u\n", cliente->mensajes_enviados);
    printf("Mensajes recibidos: %u\n", cliente->mensajes_recibidos);
    printf("Bytes enviados: %lu\n", cliente->bytes_enviados);
    printf("Bytes recibidos: %lu\n", cliente->bytes_recibidos);
    
    if (cliente->mensajes_enviados > 0) {
        double tasa_exito = (double)cliente->mensajes_recibidos / cliente->mensajes_enviados * 100.0;
        printf("Tasa de éxito: %.1f%%\n", tasa_exito);
    }
    
    printf("=====================================\n\n");
}

const char *cliente_obtener_error(cliente_estado_t estado) {
    switch (estado) {
        case CLIENTE_OK:
            return "Operación exitosa";
        case CLIENTE_ERROR_SOCKET:
            return "Error creando o configurando socket";
        case CLIENTE_ERROR_CONEXION:
            return "Error estableciendo conexión";
        case CLIENTE_ERROR_ENVIO:
            return "Error enviando datos";
        case CLIENTE_ERROR_RECEPCION:
            return "Error recibiendo datos";
        case CLIENTE_ERROR_TIMEOUT:
            return "Timeout en operación";
        case CLIENTE_ERROR_PARAMETROS:
            return "Parámetros inválidos";
        case CLIENTE_ERROR_MEMORIA:
            return "Error de memoria";
        case CLIENTE_ERROR_PROTOCOLO:
            return "Error de protocolo";
        default:
            return "Error desconocido";
    }
}

void cliente_set_verbose(cliente_tcp_t *cliente, int verbose) {
    if (cliente) {
        cliente->config.modo_verbose = verbose;
    }
}

cliente_estado_t cliente_set_timeouts(cliente_tcp_t *cliente, 
                                     int timeout_conexion, 
                                     int timeout_recv) {
    CLIENTE_VERIFICAR_PARAMETROS(cliente);
    
    if (timeout_conexion <= 0 || timeout_recv <= 0) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    cliente->config.timeout_conexion = timeout_conexion;
    cliente->config.timeout_recv = timeout_recv;
    
    // Si ya está conectado, aplicar nuevos timeouts
    if (cliente->conectado && cliente->socket_fd >= 0) {
        return configurar_timeout_socket(cliente->socket_fd, timeout_recv);
    }
    
    return CLIENTE_OK;
}

// =============================================================================
// FUNCIONES DE TESTING Y DEMOSTRACIONES
// =============================================================================

cliente_estado_t cliente_test_conectividad(const char *servidor, int puerto) {
    printf("=== TEST DE CONECTIVIDAD ===\n");
    printf("Servidor: %s:%d\n", servidor, puerto);
    
    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
    if (!cliente) {
        printf("ERROR: No se pudo crear el cliente\n");
        return CLIENTE_ERROR_MEMORIA;
    }
    
    cliente_set_verbose(cliente, 1);
    
    cliente_estado_t resultado = cliente_conectar(cliente);
    if (resultado == CLIENTE_OK) {
        printf("✓ Conectividad exitosa\n");
        cliente_desconectar(cliente);
    } else {
        printf("✗ Fallo en conectividad: %s\n", cliente_obtener_error(resultado));
    }
    
    cliente_destruir(cliente);
    printf("=============================\n\n");
    
    return resultado;
}

cliente_estado_t cliente_test_eco(cliente_tcp_t *cliente, const char *mensaje) {
    if (!cliente || !mensaje) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    printf("=== TEST DE ECO ===\n");
    printf("Mensaje: \"%s\"\n", mensaje);
    
    char respuesta[BUFFER_MAXIMO];
    transaccion_t transaccion;
    
    cliente_estado_t resultado = cliente_transaccion(cliente, mensaje, respuesta, 
                                                   sizeof(respuesta), &transaccion);
    
    if (resultado == CLIENTE_OK) {
        printf("✓ Eco exitoso\n");
        printf("Enviado: \"%s\" (%d bytes)\n", transaccion.mensaje_enviado, transaccion.bytes_enviados);
        printf("Recibido: \"%s\" (%d bytes)\n", transaccion.respuesta_recibida, transaccion.bytes_recibidos);
        printf("Tiempo de respuesta: %.2f ms\n", transaccion.tiempo_respuesta);
    } else {
        printf("✗ Fallo en eco: %s\n", cliente_obtener_error(resultado));
    }
    
    printf("===================\n\n");
    
    return resultado;
}

cliente_estado_t cliente_test_stress(cliente_tcp_t *cliente, 
                                    int num_transacciones, 
                                    const char *mensaje_base) {
    if (!cliente || num_transacciones <= 0 || !mensaje_base) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    printf("=== TEST DE STRESS ===\n");
    printf("Transacciones: %d\n", num_transacciones);
    printf("Mensaje base: \"%s\"\n", mensaje_base);
    
    int exitosas = 0;
    int fallidas = 0;
    double tiempo_total_inicio = obtener_tiempo_ms();
    double tiempo_respuesta_acumulado = 0.0;
    
    for (int i = 0; i < num_transacciones && !señal_interrupcion; i++) {
        char mensaje[BUFFER_MAXIMO];
        char respuesta[BUFFER_MAXIMO];
        transaccion_t transaccion;
        
        snprintf(mensaje, sizeof(mensaje), "%s #%d", mensaje_base, i + 1);
        
        cliente_estado_t resultado = cliente_transaccion(cliente, mensaje, respuesta, 
                                                       sizeof(respuesta), &transaccion);
        
        if (resultado == CLIENTE_OK) {
            exitosas++;
            tiempo_respuesta_acumulado += transaccion.tiempo_respuesta;
        } else {
            fallidas++;
            printf("Transacción %d falló: %s\n", i + 1, cliente_obtener_error(resultado));
        }
        
        // Mostrar progreso cada 100 transacciones
        if ((i + 1) % 100 == 0) {
            printf("Progreso: %d/%d transacciones\n", i + 1, num_transacciones);
        }
    }
    
    double tiempo_total = obtener_tiempo_ms() - tiempo_total_inicio;
    
    printf("\n--- RESULTADOS ---\n");
    printf("Transacciones exitosas: %d\n", exitosas);
    printf("Transacciones fallidas: %d\n", fallidas);
    printf("Tiempo total: %.2f ms\n", tiempo_total);
    printf("Throughput: %.2f transacciones/segundo\n", 
           exitosas * 1000.0 / tiempo_total);
    
    if (exitosas > 0) {
        printf("Tiempo de respuesta promedio: %.2f ms\n", 
               tiempo_respuesta_acumulado / exitosas);
    }
    
    double tasa_exito = (double)exitosas / (exitosas + fallidas) * 100.0;
    printf("Tasa de éxito: %.1f%%\n", tasa_exito);
    printf("======================\n\n");
    
    return exitosas > 0 ? CLIENTE_OK : CLIENTE_ERROR_CONEXION;
}

cliente_estado_t cliente_benchmark(cliente_tcp_t *cliente, 
                                  int num_mensajes, 
                                  size_t tamaño_mensaje) {
    if (!cliente || num_mensajes <= 0 || tamaño_mensaje == 0) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    printf("=== BENCHMARK DE RENDIMIENTO ===\n");
    printf("Mensajes: %d\n", num_mensajes);
    printf("Tamaño por mensaje: %zu bytes\n", tamaño_mensaje);
    printf("Datos totales: %.2f KB\n", (num_mensajes * tamaño_mensaje) / 1024.0);
    
    // Crear mensaje de prueba
    char *mensaje = malloc(tamaño_mensaje + 1);
    if (!mensaje) {
        return CLIENTE_ERROR_MEMORIA;
    }
    
    // Llenar con datos de prueba
    for (size_t i = 0; i < tamaño_mensaje; i++) {
        mensaje[i] = 'A' + (i % 26);
    }
    mensaje[tamaño_mensaje] = '\0';
    
    char respuesta[BUFFER_MAXIMO];
    int exitosos = 0;
    double tiempo_inicio = obtener_tiempo_ms();
    
    for (int i = 0; i < num_mensajes && !señal_interrupcion; i++) {
        cliente_estado_t resultado = cliente_transaccion(cliente, mensaje, respuesta, 
                                                       sizeof(respuesta), NULL);
        if (resultado == CLIENTE_OK) {
            exitosos++;
        }
        
        if ((i + 1) % 100 == 0) {
            printf("Progreso: %d/%d mensajes\n", i + 1, num_mensajes);
        }
    }
    
    double tiempo_total = obtener_tiempo_ms() - tiempo_inicio;
    
    printf("\n--- RESULTADOS DEL BENCHMARK ---\n");
    printf("Mensajes exitosos: %d/%d\n", exitosos, num_mensajes);
    printf("Tiempo total: %.2f ms\n", tiempo_total);
    printf("Throughput: %.2f mensajes/segundo\n", exitosos * 1000.0 / tiempo_total);
    printf("Bandwidth: %.2f KB/segundo\n", 
           (exitosos * tamaño_mensaje * 1000.0) / (tiempo_total * 1024.0));
    printf("Latencia promedio: %.2f ms\n", tiempo_total / exitosos);
    printf("================================\n\n");
    
    free(mensaje);
    
    return exitosos > 0 ? CLIENTE_OK : CLIENTE_ERROR_CONEXION;
}

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN Y EXPERIENCIAS EDUCATIVAS
// =============================================================================

cliente_estado_t demo_cliente_basico(const char *servidor, int puerto) {
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("\n%s\n", LINEA_SEP);
    }
    printf("      DEMOSTRACIÓN BÁSICA - CLIENTE TCP\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("Esta demostración muestra el uso básico de un cliente TCP:\n");
    printf("1. Crear cliente\n");
    printf("2. Conectar al servidor\n");
    printf("3. Enviar mensaje\n");
    printf("4. Recibir respuesta\n");
    printf("5. Desconectar\n\n");
    
    // Crear cliente
    printf("🔧 Creando cliente para %s:%d...\n", servidor, puerto);
    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
    if (!cliente) {
        printf("❌ Error: No se pudo crear el cliente\n");
        return CLIENTE_ERROR_MEMORIA;
    }
    printf("✅ Cliente creado exitosamente\n\n");
    
    // Habilitar modo verbose para la demostración
    cliente_set_verbose(cliente, 1);
    
    // Conectar
    printf("🌐 Conectando al servidor...\n");
    cliente_estado_t resultado = cliente_conectar(cliente);
    if (resultado != CLIENTE_OK) {
        printf("❌ Error conectando: %s\n", cliente_obtener_error(resultado));
        cliente_destruir(cliente);
        return resultado;
    }
    printf("✅ Conexión establecida\n\n");
    
    // Enviar mensaje de saludo
    printf("📤 Enviando mensaje de saludo...\n");
    char respuesta[BUFFER_MAXIMO];
    transaccion_t transaccion;
    
    resultado = cliente_transaccion(cliente, MENSAJE_SALUDO, respuesta, 
                                   sizeof(respuesta), &transaccion);
    
    if (resultado == CLIENTE_OK) {
        printf("✅ Transacción exitosa\n");
        printf("   📝 Enviado: \"%s\" (%d bytes)\n", 
               transaccion.mensaje_enviado, transaccion.bytes_enviados);
        printf("   📥 Recibido: \"%s\" (%d bytes)\n", 
               transaccion.respuesta_recibida, transaccion.bytes_recibidos);
        printf("   ⏱️  Tiempo de respuesta: %.2f ms\n", transaccion.tiempo_respuesta);
    } else {
        printf("❌ Error en transacción: %s\n", cliente_obtener_error(resultado));
    }
    
    printf("\n");
    
    // Mostrar estadísticas
    cliente_mostrar_estadisticas(cliente);
    
    // Desconectar
    printf("🔌 Desconectando...\n");
    cliente_desconectar(cliente);
    printf("✅ Desconectado exitosamente\n");
    
    // Limpiar
    cliente_destruir(cliente);
    
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("      FIN DE DEMOSTRACIÓN BÁSICA\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n\n", LINEA_SEP);
    }
    
    return resultado;
}

cliente_estado_t demo_cliente_interactivo(const char *servidor, int puerto) {
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("\n%s\n", LINEA_SEP);
    }
    printf("    DEMOSTRACIÓN INTERACTIVA - CLIENTE TCP\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("En esta demostración puedes enviar mensajes personalizados\n");
    printf("al servidor y ver las respuestas en tiempo real.\n");
    printf("Escribe 'quit' para salir.\n\n");
    
    // Crear y conectar cliente
    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
    if (!cliente) {
        printf("❌ Error: No se pudo crear el cliente\n");
        return CLIENTE_ERROR_MEMORIA;
    }
    
    cliente_set_verbose(cliente, 1);
    
    cliente_estado_t resultado = cliente_conectar(cliente);
    if (resultado != CLIENTE_OK) {
        printf("❌ Error conectando: %s\n", cliente_obtener_error(resultado));
        cliente_destruir(cliente);
        return resultado;
    }
    
    printf("✅ Conectado al servidor %s:%d\n", servidor, puerto);
    printf("Puedes empezar a enviar mensajes:\n\n");
    
    char mensaje[BUFFER_MAXIMO];
    char respuesta[BUFFER_MAXIMO];
    int numero_mensaje = 1;
    
    while (!señal_interrupcion) {
        printf("Mensaje #%d> ", numero_mensaje);
        fflush(stdout);
        
        // Leer mensaje del usuario
        if (!fgets(mensaje, sizeof(mensaje), stdin)) {
            break;
        }
        
        // Remover salto de línea
        size_t len = strlen(mensaje);
        if (len > 0 && mensaje[len - 1] == '\n') {
            mensaje[len - 1] = '\0';
        }
        
        // Verificar comando de salida
        if (strcmp(mensaje, "quit") == 0 || strcmp(mensaje, "exit") == 0) {
            break;
        }
        
        // Verificar mensaje vacío
        if (strlen(mensaje) == 0) {
            continue;
        }
        
        // Enviar mensaje y recibir respuesta
        transaccion_t transaccion;
        resultado = cliente_transaccion(cliente, mensaje, respuesta, 
                                       sizeof(respuesta), &transaccion);
        
        if (resultado == CLIENTE_OK) {
            printf("📥 Respuesta: \"%s\"\n", transaccion.respuesta_recibida);
            printf("   ⏱️  Tiempo: %.2f ms | Bytes: %d → %d\n", 
                   transaccion.tiempo_respuesta, 
                   transaccion.bytes_enviados, 
                   transaccion.bytes_recibidos);
        } else {
            printf("❌ Error: %s\n", cliente_obtener_error(resultado));
            break;
        }
        
        numero_mensaje++;
        printf("\n");
    }
    
    printf("\n");
    cliente_mostrar_estadisticas(cliente);
    
    cliente_desconectar(cliente);
    cliente_destruir(cliente);
    
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("    FIN DE DEMOSTRACIÓN INTERACTIVA\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n\n", LINEA_SEP);
    }
    
    return CLIENTE_OK;
}

cliente_estado_t demo_reconexion_automatica(const char *servidor, int puerto) {
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("\n%s\n", LINEA_SEP);
    }
    printf("   DEMOSTRACIÓN DE RECONEXIÓN AUTOMÁTICA\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("Esta demostración muestra cómo manejar desconexiones\n");
    printf("y reconectarse automáticamente al servidor.\n\n");
    
    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
    if (!cliente) {
        printf("❌ Error: No se pudo crear el cliente\n");
        return CLIENTE_ERROR_MEMORIA;
    }
    
    cliente_set_verbose(cliente, 1);
    
    // Configurar reconexión automática
    cliente_config_t config = cliente->config;
    config.intentos_reconexion = 3;
    config.retraso_reconexion = 2;
    cliente_configurar(cliente, &config);
    
    printf("📋 Configuración de reconexión:\n");
    printf("   - Intentos máximos: %d\n", config.intentos_reconexion);
    printf("   - Retraso entre intentos: %d segundos\n\n", config.retraso_reconexion);
    
    // Intentar conectar con reconexión automática
    cliente_estado_t resultado = CLIENTE_ERROR_CONEXION;
    
    for (int intento = 1; intento <= config.intentos_reconexion && !señal_interrupcion; intento++) {
        printf("🔄 Intento de conexión #%d...\n", intento);
        
        resultado = cliente_conectar(cliente);
        if (resultado == CLIENTE_OK) {
            printf("✅ Conectado exitosamente en el intento #%d\n", intento);
            break;
        }
        
        printf("❌ Fallo en intento #%d: %s\n", intento, cliente_obtener_error(resultado));
        
        if (intento < config.intentos_reconexion) {
            printf("⏳ Esperando %d segundos antes del siguiente intento...\n", 
                   config.retraso_reconexion);
            sleep(config.retraso_reconexion);
        }
    }
    
    if (resultado == CLIENTE_OK) {
        printf("\n🎉 Conexión establecida exitosamente\n");
        
        // Realizar algunas transacciones de prueba
        printf("🧪 Realizando transacciones de prueba...\n");
        
        const char *mensajes_prueba[] = {
            "Test de reconexión #1",
            "Test de reconexión #2",
            "Test de reconexión #3"
        };
        
        for (size_t i = 0; i < sizeof(mensajes_prueba) / sizeof(mensajes_prueba[0]); i++) {
            char respuesta[BUFFER_MAXIMO];
            transaccion_t transaccion;
            
            cliente_estado_t estado_tx = cliente_transaccion(cliente, mensajes_prueba[i], 
                                                           respuesta, sizeof(respuesta), 
                                                           &transaccion);
            
            if (estado_tx == CLIENTE_OK) {
                printf("✅ Transacción %zu exitosa (%.2f ms)\n", 
                       i + 1, transaccion.tiempo_respuesta);
            } else {
                printf("❌ Transacción %zu falló: %s\n", 
                       i + 1, cliente_obtener_error(estado_tx));
            }
            
            sleep(1); // Pausa entre transacciones
        }
        
        cliente_mostrar_estadisticas(cliente);
        cliente_desconectar(cliente);
    } else {
        printf("\n❌ No se pudo establecer conexión después de %d intentos\n", 
               config.intentos_reconexion);
    }
    
    cliente_destruir(cliente);
    
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n\n", LINEA_SEP);
    }
    printf("  FIN DE DEMOSTRACIÓN DE RECONEXIÓN\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n\n", LINEA_SEP);
    }
    
    return resultado;
}

cliente_estado_t demo_manejo_errores(const char *servidor, int puerto) {
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("\n%s\n", LINEA_SEP);
    }
    printf("    DEMOSTRACIÓN DE MANEJO DE ERRORES\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("Esta demostración muestra diferentes tipos de errores\n");
    printf("y cómo el cliente los maneja apropiadamente.\n\n");
    
    // Test 1: Crear cliente con parámetros inválidos
    printf("🧪 Test 1: Parámetros inválidos\n");
    cliente_tcp_t *cliente_invalido = cliente_crear(NULL, 0);
    if (!cliente_invalido) {
        printf("✅ Correctamente rechazó parámetros inválidos\n");
    } else {
        printf("❌ Error: Aceptó parámetros inválidos\n");
        cliente_destruir(cliente_invalido);
    }
    printf("\n");
    
    // Test 2: Conectar a puerto inexistente
    printf("🧪 Test 2: Puerto inexistente (puerto 9999)\n");
    cliente_tcp_t *cliente_puerto_malo = cliente_crear("127.0.0.1", 9999);
    if (cliente_puerto_malo) {
        cliente_set_verbose(cliente_puerto_malo, 1);
        cliente_estado_t resultado = cliente_conectar(cliente_puerto_malo);
        if (resultado != CLIENTE_OK) {
            printf("✅ Correctamente detectó puerto inexistente: %s\n", 
                   cliente_obtener_error(resultado));
        } else {
            printf("❌ Error: Se conectó a puerto que no debería existir\n");
            cliente_desconectar(cliente_puerto_malo);
        }
        cliente_destruir(cliente_puerto_malo);
    }
    printf("\n");
    
    // Test 3: Hostname inexistente
    printf("🧪 Test 3: Hostname inexistente\n");
    cliente_tcp_t *cliente_host_malo = cliente_crear("servidor.inexistente.local", 8080);
    if (!cliente_host_malo) {
        printf("✅ Correctamente rechazó hostname inexistente\n");
    } else {
        printf("❌ Error: Aceptó hostname inexistente\n");
        cliente_destruir(cliente_host_malo);
    }
    printf("\n");
    
    // Test 4: Cliente válido con timeouts
    printf("🧪 Test 4: Timeouts y configuración\n");
    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
    if (cliente) {
        cliente_set_verbose(cliente, 1);
        
        // Configurar timeouts muy cortos
        cliente_estado_t resultado = cliente_set_timeouts(cliente, 1, 1);
        if (resultado == CLIENTE_OK) {
            printf("✅ Timeouts configurados correctamente\n");
        } else {
            printf("❌ Error configurando timeouts: %s\n", cliente_obtener_error(resultado));
        }
        
        // Intentar conectar con timeout corto
        printf("🔗 Intentando conectar con timeout de 1 segundo...\n");
        resultado = cliente_conectar(cliente);
        if (resultado == CLIENTE_OK) {
            printf("✅ Conexión exitosa incluso con timeout corto\n");
            
            // Test de operación sin conexión después de desconectar
            printf("🔌 Desconectando y probando operación sin conexión...\n");
            cliente_desconectar(cliente);
            
            char respuesta[BUFFER_MAXIMO];
            int bytes = cliente_recibir(cliente, respuesta, sizeof(respuesta));
            if (bytes < 0) {
                printf("✅ Correctamente detectó que no hay conexión: %s\n", 
                       cliente_obtener_error((cliente_estado_t)bytes));
            } else {
                printf("❌ Error: Permitió recibir datos sin conexión\n");
            }
            
        } else {
            printf("ℹ️  Timeout o error de conexión (esperado): %s\n", 
                   cliente_obtener_error(resultado));
        }
        
        cliente_destruir(cliente);
    }
    printf("\n");
    
    // Test 5: Mostrar todos los códigos de error
    printf("🧪 Test 5: Códigos de error disponibles\n");
    printf("Tipos de error que puede manejar el cliente:\n");
    for (int i = CLIENTE_OK; i >= CLIENTE_ERROR_PROTOCOLO; i--) {
        printf("   %2d: %s\n", i, cliente_obtener_error((cliente_estado_t)i));
    }
    
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n", LINEA_SEP);
    }
    printf("   FIN DE DEMOSTRACIÓN DE ERRORES\n");
    {
        static const char LINEA_SEP[] = "==================================================";
        printf("%s\n\n", LINEA_SEP);
    }
    
    return CLIENTE_OK;
}

// =============================================================================
// UTILIDADES PARA SERVIDOR DE PRUEBA
// =============================================================================

pid_t crear_servidor_prueba(int puerto, const char *mensaje_respuesta) {
    if (!mensaje_respuesta) {
        mensaje_respuesta = "Echo desde servidor de prueba";
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo - servidor
        int servidor_fd, cliente_fd;
        struct sockaddr_in direccion;
        int opt = 1;
        socklen_t addrlen = sizeof(direccion);
        char buffer[BUFFER_MAXIMO];
        
        // Crear socket del servidor
        servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (servidor_fd < 0) {
            exit(1);
        }
        
        // Configurar reutilización de dirección
        setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // Configurar dirección
        direccion.sin_family = AF_INET;
        direccion.sin_addr.s_addr = INADDR_ANY;
        direccion.sin_port = htons(puerto);
        
        // Bind y listen
        if (bind(servidor_fd, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
            close(servidor_fd);
            exit(1);
        }
        
        if (listen(servidor_fd, 3) < 0) {
            close(servidor_fd);
            exit(1);
        }
        
        // Loop del servidor
        while (1) {
            cliente_fd = accept(servidor_fd, (struct sockaddr*)&direccion, &addrlen);
            if (cliente_fd < 0) {
                continue;
            }
            
            // Recibir mensaje
            ssize_t bytes_recibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_recibidos > 0) {
                buffer[bytes_recibidos] = '\0';
                
                // Enviar respuesta (eco del mensaje recibido + respuesta)
                char respuesta_completa[BUFFER_MAXIMO];
                snprintf(respuesta_completa, sizeof(respuesta_completa), 
                        "%s: %s", mensaje_respuesta, buffer);
                send(cliente_fd, respuesta_completa, strlen(respuesta_completa), 0);
            }
            
            close(cliente_fd);
        }
        
        close(servidor_fd);
        exit(0);
    }
    
    // Proceso padre
    if (pid > 0) {
        // Dar tiempo al servidor para inicializar
        sleep(1);
    }
    
    return pid;
}

cliente_estado_t detener_servidor_prueba(pid_t pid) {
    if (pid <= 0) {
        return CLIENTE_ERROR_PARAMETROS;
    }
    
    if (kill(pid, SIGTERM) == 0) {
        // Esperar que el proceso termine
        int status;
        waitpid(pid, &status, 0);
        return CLIENTE_OK;
    }
    
    return CLIENTE_ERROR_CONEXION;
}
