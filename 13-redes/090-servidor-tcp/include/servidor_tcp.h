/**
 * @file servidor_tcp.h
 * @brief API para servidor TCP de eco - Ejercicio 090
 * @author Ejercicios de C
 * @date 2025
 * 
 * Este archivo define la interfaz para un servidor TCP de eco que escucha
 * conexiones en un puerto, acepta clientes, lee mensajes y los devuelve
 * (eco) usando la API de sockets BSD.
 * 
 * Conceptos cubiertos:
 * - Sockets de servidor (socket, bind, listen, accept)
 * - Manejo de múltiples clientes
 * - Protocolo TCP servidor-cliente
 * - Gestión de conexiones y recursos
 * - Señales y shutdown graceful
 */

#ifndef SERVIDOR_TCP_H
#define SERVIDOR_TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CONSTANTES Y CONFIGURACIÓN
// ============================================================================

#define PUERTO_DEFECTO 8080
#define BUFFER_MAXIMO 4096
#define BACKLOG_DEFECTO 10
#define MAX_CLIENTES 100
#define TIMEOUT_CLIENTE_SEG 30
#define MAX_MENSAJE 1024

// Versión del servidor
#define SERVIDOR_VERSION_MAJOR 1
#define SERVIDOR_VERSION_MINOR 0
#define SERVIDOR_VERSION_PATCH 0

// ============================================================================
// TIPOS DE DATOS
// ============================================================================

/**
 * @brief Códigos de resultado para operaciones del servidor
 */
typedef enum {
    SERVIDOR_EXITO = 0,
    SERVIDOR_ERROR_SOCKET = -1,
    SERVIDOR_ERROR_BIND = -2,
    SERVIDOR_ERROR_LISTEN = -3,
    SERVIDOR_ERROR_ACCEPT = -4,
    SERVIDOR_ERROR_RECV = -5,
    SERVIDOR_ERROR_SEND = -6,
    SERVIDOR_ERROR_MEMORIA = -7,
    SERVIDOR_ERROR_TIMEOUT = -8,
    SERVIDOR_ERROR_PARAMETRO = -9,
    SERVIDOR_ERROR_SISTEMA = -10,
    SERVIDOR_SHUTDOWN = -11
} resultado_servidor_t;

/**
 * @brief Modos de operación del servidor
 */
typedef enum {
    MODO_SECUENCIAL,    // Un cliente a la vez
    MODO_FORK,          // Fork por cliente
    MODO_THREAD,        // Thread por cliente
    MODO_SELECT,        // I/O multiplexing con select
    MODO_POLL           // I/O multiplexing con poll
} modo_servidor_t;

/**
 * @brief Configuración del servidor
 */
typedef struct {
    char host[256];                 // Dirección IP para bind (INADDR_ANY = "0.0.0.0")
    int puerto;                     // Puerto de escucha
    int backlog;                    // Tamaño de cola de conexiones pendientes
    int max_clientes;               // Máximo número de clientes simultáneos
    int timeout_cliente_seg;        // Timeout por cliente en segundos
    int buffer_size;                // Tamaño del buffer de recepción
    modo_servidor_t modo;           // Modo de operación
    int reusar_puerto;              // SO_REUSEADDR
    int keep_alive;                 // SO_KEEPALIVE
    int no_delay;                   // TCP_NODELAY
    int verbose;                    // Logging detallado
    int daemonizar;                 // Ejecutar como daemon
} config_servidor_t;

/**
 * @brief Información de un cliente conectado
 */
typedef struct {
    int socket_fd;                  // Descriptor del socket
    struct sockaddr_in direccion;  // Dirección del cliente
    time_t tiempo_conexion;         // Timestamp de conexión
    int mensajes_procesados;        // Número de mensajes procesados
    size_t bytes_recibidos;         // Total de bytes recibidos
    size_t bytes_enviados;          // Total de bytes enviados
    pid_t pid_proceso;              // PID del proceso hijo (si MODO_FORK)
    pthread_t thread_id;            // ID del thread (si MODO_THREAD)
    int activo;                     // Cliente activo (1) o desconectado (0)
} info_cliente_t;

/**
 * @brief Estadísticas del servidor
 */
typedef struct {
    int conexiones_totales;         // Total de conexiones aceptadas
    int conexiones_activas;         // Conexiones actualmente activas
    int conexiones_rechazadas;      // Conexiones rechazadas
    int mensajes_procesados;        // Total de mensajes de eco procesados
    size_t bytes_transferidos;      // Total de bytes transferidos
    time_t tiempo_inicio;           // Timestamp de inicio del servidor
    double tiempo_promedio_ms;      // Tiempo promedio de procesamiento por mensaje
    int errores_red;                // Errores de red encontrados
    int timeouts;                   // Timeouts de cliente
} estadisticas_servidor_t;

/**
 * @brief Estructura principal del servidor
 */
typedef struct servidor_tcp {
    int socket_servidor;            // Socket del servidor
    config_servidor_t config;       // Configuración
    estadisticas_servidor_t stats;  // Estadísticas
    info_cliente_t* clientes;       // Array de clientes activos
    int ejecutandose;               // Flag de ejecución
    int shutdown_solicitado;        // Flag de shutdown graceful
    pthread_mutex_t mutex_stats;    // Mutex para estadísticas thread-safe
} servidor_tcp_t;

// ============================================================================
// CONFIGURACIÓN POR DEFECTO
// ============================================================================

#define CONFIG_SERVIDOR_DEFECTO { \
    .host = "0.0.0.0", \
    .puerto = PUERTO_DEFECTO, \
    .backlog = BACKLOG_DEFECTO, \
    .max_clientes = MAX_CLIENTES, \
    .timeout_cliente_seg = TIMEOUT_CLIENTE_SEG, \
    .buffer_size = BUFFER_MAXIMO, \
    .modo = MODO_SECUENCIAL, \
    .reusar_puerto = 1, \
    .keep_alive = 1, \
    .no_delay = 0, \
    .verbose = 0, \
    .daemonizar = 0 \
}

// ============================================================================
// API PRINCIPAL DEL SERVIDOR
// ============================================================================

/**
 * @brief Crear un nuevo servidor TCP
 * @param config Configuración del servidor (NULL = configuración por defecto)
 * @return Puntero al servidor creado o NULL en caso de error
 */
servidor_tcp_t* crear_servidor(const config_servidor_t* config);

/**
 * @brief Destruir un servidor y liberar recursos
 * @param servidor Puntero al servidor a destruir
 */
void destruir_servidor(servidor_tcp_t* servidor);

/**
 * @brief Iniciar el servidor (bind + listen)
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t iniciar_servidor(servidor_tcp_t* servidor);

/**
 * @brief Ejecutar el bucle principal del servidor
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t ejecutar_servidor(servidor_tcp_t* servidor);

/**
 * @brief Detener el servidor de forma graceful
 * @param servidor Puntero al servidor
 */
void detener_servidor(servidor_tcp_t* servidor);

// ============================================================================
// FUNCIONES DE GESTIÓN DE CLIENTES
// ============================================================================

/**
 * @brief Aceptar una nueva conexión de cliente
 * @param servidor Puntero al servidor
 * @param info_cliente Estructura para almacenar información del cliente
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t aceptar_cliente(servidor_tcp_t* servidor, info_cliente_t* info_cliente);

/**
 * @brief Procesar un mensaje de eco de un cliente
 * @param servidor Puntero al servidor
 * @param cliente Información del cliente
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t procesar_cliente_eco(servidor_tcp_t* servidor, info_cliente_t* cliente);

/**
 * @brief Desconectar un cliente
 * @param servidor Puntero al servidor
 * @param cliente Información del cliente
 */
void desconectar_cliente(servidor_tcp_t* servidor, info_cliente_t* cliente);

// ============================================================================
// MODOS DE OPERACIÓN
// ============================================================================

/**
 * @brief Ejecutar servidor en modo secuencial (un cliente a la vez)
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t servidor_modo_secuencial(servidor_tcp_t* servidor);

/**
 * @brief Ejecutar servidor en modo fork (proceso por cliente)
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t servidor_modo_fork(servidor_tcp_t* servidor);

/**
 * @brief Ejecutar servidor en modo thread (hilo por cliente)
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t servidor_modo_thread(servidor_tcp_t* servidor);

/**
 * @brief Ejecutar servidor con I/O multiplexing (select)
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t servidor_modo_select(servidor_tcp_t* servidor);

// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

/**
 * @brief Obtener estadísticas del servidor
 * @param servidor Puntero al servidor
 * @param stats Estructura para almacenar las estadísticas
 */
void obtener_estadisticas_servidor(const servidor_tcp_t* servidor, estadisticas_servidor_t* stats);

/**
 * @brief Resetear estadísticas del servidor
 * @param servidor Puntero al servidor
 */
void resetear_estadisticas_servidor(servidor_tcp_t* servidor);

/**
 * @brief Configurar opciones avanzadas del socket
 * @param servidor Puntero al servidor
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t configurar_socket_servidor(servidor_tcp_t* servidor);

/**
 * @brief Instalar manejadores de señales para shutdown graceful
 * @param servidor Puntero al servidor
 */
void instalar_manejadores_senales(servidor_tcp_t* servidor);

/**
 * @brief Convertir código de error a string descriptivo
 * @param codigo Código de error
 * @return String descriptivo del error
 */
const char* servidor_strerror(resultado_servidor_t codigo);

/**
 * @brief Obtener información de un cliente conectado
 * @param cliente_fd Descriptor del socket del cliente
 * @param info Estructura para almacenar la información
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t obtener_info_cliente(int cliente_fd, info_cliente_t* info);

/**
 * @brief Imprimir estadísticas del servidor
 * @param servidor Puntero al servidor
 * @param archivo Archivo donde imprimir (NULL = stdout)
 */
void imprimir_estadisticas_servidor(const servidor_tcp_t* servidor, FILE* archivo);

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// ============================================================================

/**
 * @brief Demo básico: servidor de eco simple
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t demo_servidor_basico(void);

/**
 * @brief Demo avanzado: servidor con múltiples modos
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t demo_servidor_modos(void);

/**
 * @brief Demo de rendimiento: servidor con métricas
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t demo_servidor_rendimiento(void);

/**
 * @brief Demo de configuración: servidor personalizable
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t demo_servidor_configuracion(void);

// ============================================================================
// HERRAMIENTAS AUXILIARES
// ============================================================================

/**
 * @brief Crear un cliente de prueba simple
 * @param host Dirección del servidor
 * @param puerto Puerto del servidor
 * @param mensaje Mensaje a enviar
 * @param respuesta Buffer para la respuesta
 * @param tam_respuesta Tamaño del buffer de respuesta
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t cliente_prueba_simple(const char* host, int puerto, 
                                          const char* mensaje, char* respuesta, 
                                          size_t tam_respuesta);

/**
 * @brief Generar múltiples clientes de prueba concurrentes
 * @param host Dirección del servidor
 * @param puerto Puerto del servidor
 * @param num_clientes Número de clientes a crear
 * @param mensajes_por_cliente Mensajes por cliente
 * @return SERVIDOR_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_servidor_t stress_test_clientes(const char* host, int puerto, 
                                         int num_clientes, int mensajes_por_cliente);

#ifdef __cplusplus
}
#endif

#endif // SERVIDOR_TCP_H
