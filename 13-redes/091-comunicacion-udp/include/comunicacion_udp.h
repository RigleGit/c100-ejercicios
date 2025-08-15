/**
 * @file comunicacion_udp.h
 * @brief API para comunicación UDP - Ejercicio 091
 * @author Ejercicios de C
 * @date 2025
 * 
 * Este archivo define la interfaz para comunicación UDP (User Datagram Protocol)
 * usando sockets BSD. Incluye tanto funcionalidad de emisor como receptor
 * para implementar comunicación basada en datagramas sin conexión.
 * 
 * Conceptos cubiertos:
 * - Protocolo UDP sin conexión
 * - Sockets de datagrama (SOCK_DGRAM)
 * - sendto() y recvfrom()
 * - Diferencias entre TCP y UDP
 * - Manejo de direcciones de red
 */

#ifndef COMUNICACION_UDP_H
#define COMUNICACION_UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CONSTANTES Y CONFIGURACIÓN
// ============================================================================

#define PUERTO_UDP_DEFECTO 9090
#define BUFFER_UDP_MAXIMO 65536  // Máximo tamaño de datagrama UDP
#define BUFFER_UDP_ESTANDAR 1024
#define TIMEOUT_UDP_DEFECTO 5    // Segundos
#define MAX_INTENTOS_DEFECTO 3

// Tamaños típicos de mensaje UDP
#define UDP_MENSAJE_CORTO 256
#define UDP_MENSAJE_MEDIO 1024
#define UDP_MENSAJE_LARGO 8192

// Versión de la biblioteca
#define UDP_VERSION_MAJOR 1
#define UDP_VERSION_MINOR 0
#define UDP_VERSION_PATCH 0

// ============================================================================
// TIPOS DE DATOS
// ============================================================================

/**
 * @brief Códigos de resultado para operaciones UDP
 */
typedef enum {
    UDP_EXITO = 0,
    UDP_ERROR_SOCKET = -1,
    UDP_ERROR_BIND = -2,
    UDP_ERROR_SENDTO = -3,
    UDP_ERROR_RECVFROM = -4,
    UDP_ERROR_TIMEOUT = -5,
    UDP_ERROR_DIRECCION = -6,
    UDP_ERROR_PARAMETRO = -7,
    UDP_ERROR_MEMORIA = -8,
    UDP_ERROR_SISTEMA = -9,
    UDP_MENSAJE_TRUNCADO = -10,
    UDP_ERROR_RESOLUCION = -11
} resultado_udp_t;

/**
 * @brief Tipos de operación UDP
 */
typedef enum {
    UDP_EMISOR,         // Solo envía mensajes
    UDP_RECEPTOR,       // Solo recibe mensajes
    UDP_BIDIRECCIONAL   // Envía y recibe mensajes
} tipo_udp_t;

/**
 * @brief Configuración para operaciones UDP
 */
typedef struct {
    char host[256];             // Dirección IP o hostname
    int puerto;                 // Puerto UDP
    int timeout_segundos;       // Timeout para operaciones
    int max_intentos;           // Máximo intentos de reenvío
    int buffer_size;            // Tamaño del buffer
    int reusar_puerto;          // SO_REUSEADDR
    int broadcast;              // SO_BROADCAST
    int verbose;                // Logging detallado
    tipo_udp_t tipo;            // Tipo de operación
} config_udp_t;

/**
 * @brief Información de un mensaje UDP
 */
typedef struct {
    struct sockaddr_in origen;  // Dirección de origen
    struct sockaddr_in destino; // Dirección de destino
    char* datos;                // Contenido del mensaje
    size_t tamaño;              // Tamaño del mensaje
    time_t timestamp;           // Timestamp del mensaje
    int intentos;               // Número de intentos de envío
    double rtt_ms;              // Round-trip time (si aplicable)
} mensaje_udp_t;

/**
 * @brief Estadísticas de comunicación UDP
 */
typedef struct {
    int mensajes_enviados;      // Total de mensajes enviados
    int mensajes_recibidos;     // Total de mensajes recibidos
    int bytes_enviados;         // Total de bytes enviados
    int bytes_recibidos;        // Total de bytes recibidos
    int errores_envio;          // Errores de envío
    int errores_recepcion;      // Errores de recepción
    int timeouts;               // Timeouts ocurridos
    int mensajes_truncados;     // Mensajes truncados
    double rtt_promedio_ms;     // RTT promedio
    double throughput_kbps;     // Throughput en KB/s
    time_t tiempo_inicio;       // Timestamp de inicio
} estadisticas_udp_t;

/**
 * @brief Estructura principal para comunicación UDP
 */
typedef struct {
    int socket_fd;              // Descriptor del socket
    config_udp_t config;        // Configuración
    estadisticas_udp_t stats;   // Estadísticas
    struct sockaddr_in direccion_local;  // Dirección local vinculada
    char* buffer_interno;       // Buffer interno para operaciones
    int inicializado;           // Flag de inicialización
} udp_context_t;

// ============================================================================
// CONFIGURACIÓN POR DEFECTO
// ============================================================================

#define CONFIG_UDP_DEFECTO { \
    .host = "127.0.0.1", \
    .puerto = PUERTO_UDP_DEFECTO, \
    .timeout_segundos = TIMEOUT_UDP_DEFECTO, \
    .max_intentos = MAX_INTENTOS_DEFECTO, \
    .buffer_size = BUFFER_UDP_ESTANDAR, \
    .reusar_puerto = 1, \
    .broadcast = 0, \
    .verbose = 0, \
    .tipo = UDP_BIDIRECCIONAL \
}

#define CONFIG_UDP_RECEPTOR(puerto) { \
    .host = "0.0.0.0", \
    .puerto = (puerto), \
    .timeout_segundos = 0, \
    .max_intentos = 1, \
    .buffer_size = BUFFER_UDP_ESTANDAR, \
    .reusar_puerto = 1, \
    .broadcast = 0, \
    .verbose = 1, \
    .tipo = UDP_RECEPTOR \
}

static inline config_udp_t config_udp_emisor(const char* host, int puerto) {
    config_udp_t cfg = CONFIG_UDP_DEFECTO;
    strncpy(cfg.host, host ? host : "127.0.0.1", sizeof(cfg.host)-1);
    cfg.host[sizeof(cfg.host)-1] = '\0';
    cfg.puerto = puerto;
    cfg.timeout_segundos = TIMEOUT_UDP_DEFECTO;
    cfg.max_intentos = MAX_INTENTOS_DEFECTO;
    cfg.buffer_size = BUFFER_UDP_ESTANDAR;
    cfg.reusar_puerto = 0;
    cfg.broadcast = 0;
    cfg.verbose = 1;
    cfg.tipo = UDP_EMISOR;
    return cfg;
}

// ============================================================================
// API PRINCIPAL
// ============================================================================

/**
 * @brief Crear un contexto UDP
 * @param config Configuración (NULL = configuración por defecto)
 * @return Puntero al contexto creado o NULL en caso de error
 */
udp_context_t* crear_contexto_udp(const config_udp_t* config);

/**
 * @brief Destruir un contexto UDP y liberar recursos
 * @param contexto Puntero al contexto a destruir
 */
void destruir_contexto_udp(udp_context_t* contexto);

/**
 * @brief Inicializar el contexto UDP (crear socket y bind si es receptor)
 * @param contexto Puntero al contexto
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t inicializar_udp(udp_context_t* contexto);

// ============================================================================
// FUNCIONES DE EMISIÓN
// ============================================================================

/**
 * @brief Enviar un mensaje UDP a una dirección específica
 * @param contexto Puntero al contexto UDP
 * @param mensaje Mensaje a enviar
 * @param host Dirección de destino (IP o hostname)
 * @param puerto Puerto de destino
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t enviar_mensaje_udp(udp_context_t* contexto, const char* mensaje, 
                                   const char* host, int puerto);

/**
 * @brief Enviar mensaje UDP con información completa
 * @param contexto Puntero al contexto UDP
 * @param msg_info Información completa del mensaje
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t enviar_mensaje_udp_completo(udp_context_t* contexto, mensaje_udp_t* msg_info);

/**
 * @brief Enviar mensaje UDP con reintentos automáticos
 * @param contexto Puntero al contexto UDP
 * @param mensaje Mensaje a enviar
 * @param host Dirección de destino
 * @param puerto Puerto de destino
 * @param esperar_respuesta Si esperar respuesta (para calcular RTT)
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t enviar_mensaje_udp_confiable(udp_context_t* contexto, const char* mensaje,
                                            const char* host, int puerto, int esperar_respuesta);

/**
 * @brief Enviar broadcast UDP a toda la red local
 * @param contexto Puntero al contexto UDP
 * @param mensaje Mensaje a enviar
 * @param puerto Puerto de destino
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t enviar_broadcast_udp(udp_context_t* contexto, const char* mensaje, int puerto);

// ============================================================================
// FUNCIONES DE RECEPCIÓN
// ============================================================================

/**
 * @brief Recibir un mensaje UDP
 * @param contexto Puntero al contexto UDP
 * @param buffer Buffer para almacenar el mensaje
 * @param tam_buffer Tamaño del buffer
 * @param origen Estructura para almacenar dirección de origen (puede ser NULL)
 * @return Número de bytes recibidos (>= 0) o código de error (< 0)
 */
int recibir_mensaje_udp(udp_context_t* contexto, char* buffer, size_t tam_buffer,
                       struct sockaddr_in* origen);

/**
 * @brief Recibir mensaje UDP con información completa
 * @param contexto Puntero al contexto UDP
 * @param msg_info Estructura para almacenar información completa del mensaje
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t recibir_mensaje_udp_completo(udp_context_t* contexto, mensaje_udp_t* msg_info);

/**
 * @brief Recibir mensaje UDP con timeout
 * @param contexto Puntero al contexto UDP
 * @param buffer Buffer para almacenar el mensaje
 * @param tam_buffer Tamaño del buffer
 * @param origen Estructura para almacenar dirección de origen
 * @param timeout_ms Timeout en milisegundos
 * @return Número de bytes recibidos (>= 0) o código de error (< 0)
 */
int recibir_mensaje_udp_timeout(udp_context_t* contexto, char* buffer, size_t tam_buffer,
                               struct sockaddr_in* origen, int timeout_ms);

/**
 * @brief Escuchar mensajes UDP en bucle
 * @param contexto Puntero al contexto UDP
 * @param callback Función llamada por cada mensaje recibido
 * @param user_data Datos de usuario para el callback
 * @return UDP_EXITO cuando se detiene por señal, código de error en caso contrario
 */
resultado_udp_t escuchar_mensajes_udp(udp_context_t* contexto,
                                     void (*callback)(const mensaje_udp_t*, void*),
                                     void* user_data);

// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

/**
 * @brief Obtener estadísticas del contexto UDP
 * @param contexto Puntero al contexto UDP
 * @param stats Estructura para almacenar las estadísticas
 */
void obtener_estadisticas_udp(const udp_context_t* contexto, estadisticas_udp_t* stats);

/**
 * @brief Resetear estadísticas del contexto UDP
 * @param contexto Puntero al contexto UDP
 */
void resetear_estadisticas_udp(udp_context_t* contexto);

/**
 * @brief Convertir código de error UDP a string descriptivo
 * @param codigo Código de error
 * @return String descriptivo del error
 */
const char* udp_strerror(resultado_udp_t codigo);

/**
 * @brief Convertir sockaddr_in a string legible
 * @param addr Dirección a convertir
 * @param buffer Buffer para almacenar el resultado
 * @param tam_buffer Tamaño del buffer
 * @return Puntero al buffer o NULL en caso de error
 */
char* direccion_a_string(const struct sockaddr_in* addr, char* buffer, size_t tam_buffer);

/**
 * @brief Resolver hostname a dirección IP
 * @param hostname Nombre del host a resolver
 * @param ip_buffer Buffer para almacenar la IP resultante
 * @param tam_buffer Tamaño del buffer
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t resolver_hostname(const char* hostname, char* ip_buffer, size_t tam_buffer);

/**
 * @brief Validar dirección IP
 * @param ip_string String con la dirección IP
 * @return 1 si es válida, 0 si no es válida
 */
int validar_ip(const char* ip_string);

/**
 * @brief Imprimir estadísticas UDP
 * @param contexto Puntero al contexto UDP
 * @param archivo Archivo donde imprimir (NULL = stdout)
 */
void imprimir_estadisticas_udp(const udp_context_t* contexto, FILE* archivo);

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// ============================================================================

/**
 * @brief Demo básico: emisor UDP simple
 * @param host Host de destino
 * @param puerto Puerto de destino
 * @param mensaje Mensaje a enviar
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t demo_emisor_udp(const char* host, int puerto, const char* mensaje);

/**
 * @brief Demo básico: receptor UDP simple
 * @param puerto Puerto de escucha
 * @param num_mensajes Número de mensajes a recibir (0 = infinito)
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t demo_receptor_udp(int puerto, int num_mensajes);

/**
 * @brief Demo avanzado: comunicación bidireccional
 * @param host Host para comunicación
 * @param puerto Puerto para comunicación
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t demo_bidireccional_udp(const char* host, int puerto);

/**
 * @brief Demo de broadcast UDP
 * @param puerto Puerto para broadcast
 * @param mensaje Mensaje a enviar
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t demo_broadcast_udp(int puerto, const char* mensaje);

/**
 * @brief Demo de performance: medir throughput UDP
 * @param host Host de destino
 * @param puerto Puerto de destino
 * @param num_mensajes Número de mensajes para el test
 * @param tamaño_mensaje Tamaño de cada mensaje
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t demo_performance_udp(const char* host, int puerto, 
                                    int num_mensajes, int tamaño_mensaje);

// ============================================================================
// HERRAMIENTAS DE COMPARACIÓN TCP vs UDP
// ============================================================================

/**
 * @brief Estructura para comparar TCP vs UDP
 */
typedef struct {
    double tiempo_tcp_ms;
    double tiempo_udp_ms;
    int perdidas_udp;
    int reordenamientos_udp;
    double throughput_tcp_kbps;
    double throughput_udp_kbps;
} comparacion_tcp_udp_t;

/**
 * @brief Ejecutar test comparativo entre TCP y UDP
 * @param host Host de destino
 * @param puerto_tcp Puerto para test TCP
 * @param puerto_udp Puerto para test UDP
 * @param num_mensajes Número de mensajes para el test
 * @param tamaño_mensaje Tamaño de cada mensaje
 * @param resultado Estructura para almacenar los resultados
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t comparar_tcp_vs_udp(const char* host, int puerto_tcp, int puerto_udp,
                                   int num_mensajes, int tamaño_mensaje,
                                   comparacion_tcp_udp_t* resultado);

/**
 * @brief Imprimir comparación TCP vs UDP
 * @param comparacion Estructura con los resultados de la comparación
 * @param archivo Archivo donde imprimir (NULL = stdout)
 */
void imprimir_comparacion_tcp_udp(const comparacion_tcp_udp_t* comparacion, FILE* archivo);

// ============================================================================
// FUNCIONES AUXILIARES PARA TESTING
// ============================================================================

/**
 * @brief Crear receptor UDP simple para testing
 * @param puerto Puerto de escucha
 * @param timeout_segundos Timeout para recepción
 * @param respuesta_automatica Si enviar respuesta automática
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t crear_receptor_testing(int puerto, int timeout_segundos, int respuesta_automatica);

/**
 * @brief Crear emisor UDP simple para testing
 * @param host Host de destino
 * @param puerto Puerto de destino
 * @param mensajes Array de mensajes a enviar
 * @param num_mensajes Número de mensajes
 * @param intervalo_ms Intervalo entre mensajes
 * @return UDP_EXITO en caso de éxito, código de error en caso contrario
 */
resultado_udp_t crear_emisor_testing(const char* host, int puerto, 
                                    const char** mensajes, int num_mensajes, 
                                    int intervalo_ms);

#ifdef __cplusplus
}
#endif

#endif // COMUNICACION_UDP_H
