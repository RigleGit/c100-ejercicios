/**
 * @file cliente_tcp.h
 * @brief Cliente TCP educativo con API completa para comunicación en red
 * @description Header principal que define estructuras, constantes y funciones
 *              para un cliente TCP robusto y educativo que demuestra los fundamentos
 *              de la programación de sockets BSD en C.
 * @version 1.0
 * @date 2024
 * @author Estudiante de C
 */

#ifndef CLIENTE_TCP_H
#define CLIENTE_TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>

/**
 * @brief Constantes de configuración del cliente
 */
#define CLIENTE_TCP_VERSION "1.0"
#define PUERTO_DEFECTO 8080
#define SERVIDOR_DEFECTO "127.0.0.1"
#define BUFFER_MAXIMO 4096
#define TIMEOUT_CONEXION 10
#define TIMEOUT_RECV 5
#define INTENTOS_RECONEXION 3
#define RETRASO_RECONEXION 1

/**
 * @brief Mensajes predefinidos para testing
 */
#define MENSAJE_SALUDO "Hola, servidor"
#define MENSAJE_PING "PING"
#define MENSAJE_ECHO "ECHO: Mensaje de prueba"
#define MENSAJE_DESPEDIDA "BYE"

/**
 * @brief Códigos de estado para operaciones del cliente
 */
typedef enum {
    CLIENTE_OK = 0,              /**< Operación exitosa */
    CLIENTE_ERROR_SOCKET = -1,   /**< Error creando socket */
    CLIENTE_ERROR_CONEXION = -2, /**< Error en conexión */
    CLIENTE_ERROR_ENVIO = -3,    /**< Error enviando datos */
    CLIENTE_ERROR_RECEPCION = -4,/**< Error recibiendo datos */
    CLIENTE_ERROR_TIMEOUT = -5,  /**< Timeout en operación */
    CLIENTE_ERROR_PARAMETROS = -6,/**< Parámetros inválidos */
    CLIENTE_ERROR_MEMORIA = -7,  /**< Error de memoria */
    CLIENTE_ERROR_PROTOCOLO = -8 /**< Error de protocolo */
} cliente_estado_t;

/**
 * @brief Configuración del cliente TCP
 */
typedef struct {
    char servidor[256];          /**< Dirección del servidor */
    int puerto;                  /**< Puerto del servidor */
    int timeout_conexion;        /**< Timeout para conexión (segundos) */
    int timeout_recv;            /**< Timeout para recepción (segundos) */
    int intentos_reconexion;     /**< Número de intentos de reconexión */
    int retraso_reconexion;      /**< Retraso entre reconexiones (segundos) */
    int modo_verbose;            /**< Modo verbose para debug */
    int keepalive;               /**< Habilitar keep-alive */
} cliente_config_t;

/**
 * @brief Estructura principal del cliente TCP
 */
typedef struct {
    int socket_fd;               /**< File descriptor del socket */
    cliente_config_t config;     /**< Configuración del cliente */
    struct sockaddr_in servidor_addr; /**< Dirección del servidor */
    int conectado;               /**< Estado de conexión */
    time_t tiempo_conexion;      /**< Timestamp de conexión */
    unsigned long bytes_enviados; /**< Contador de bytes enviados */
    unsigned long bytes_recibidos; /**< Contador de bytes recibidos */
    unsigned int mensajes_enviados; /**< Contador de mensajes enviados */
    unsigned int mensajes_recibidos; /**< Contador de mensajes recibidos */
} cliente_tcp_t;

/**
 * @brief Información de una transacción de comunicación
 */
typedef struct {
    char mensaje_enviado[BUFFER_MAXIMO]; /**< Mensaje que se envió */
    char respuesta_recibida[BUFFER_MAXIMO]; /**< Respuesta recibida */
    int bytes_enviados;          /**< Bytes enviados en esta transacción */
    int bytes_recibidos;         /**< Bytes recibidos en esta transacción */
    double tiempo_respuesta;     /**< Tiempo de respuesta en milisegundos */
    cliente_estado_t estado;     /**< Estado de la transacción */
} transaccion_t;

/**
 * @brief Estadísticas del cliente
 */
typedef struct {
    unsigned int conexiones_exitosas;   /**< Conexiones exitosas */
    unsigned int conexiones_fallidas;   /**< Conexiones fallidas */
    unsigned int transacciones_exitosas; /**< Transacciones exitosas */
    unsigned int transacciones_fallidas; /**< Transacciones fallidas */
    double tiempo_conexion_promedio;    /**< Tiempo promedio de conexión */
    double tiempo_respuesta_promedio;   /**< Tiempo promedio de respuesta */
    unsigned long total_bytes_enviados; /**< Total de bytes enviados */
    unsigned long total_bytes_recibidos; /**< Total de bytes recibidos */
} estadisticas_cliente_t;

// =============================================================================
// FUNCIONES PRINCIPALES DE LA API
// =============================================================================

/**
 * @brief Crear y configurar un nuevo cliente TCP
 * @param servidor Dirección del servidor (puede ser IP o hostname)
 * @param puerto Puerto del servidor
 * @return Puntero al cliente creado o NULL en caso de error
 */
cliente_tcp_t *cliente_crear(const char *servidor, int puerto);

/**
 * @brief Destruir cliente y liberar recursos
 * @param cliente Puntero al cliente a destruir
 */
void cliente_destruir(cliente_tcp_t *cliente);

/**
 * @brief Configurar opciones del cliente
 * @param cliente Puntero al cliente
 * @param config Configuración a aplicar
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_configurar(cliente_tcp_t *cliente, const cliente_config_t *config);

/**
 * @brief Conectar al servidor
 * @param cliente Puntero al cliente
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_conectar(cliente_tcp_t *cliente);

/**
 * @brief Desconectar del servidor
 * @param cliente Puntero al cliente
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_desconectar(cliente_tcp_t *cliente);

/**
 * @brief Enviar mensaje al servidor
 * @param cliente Puntero al cliente
 * @param mensaje Mensaje a enviar
 * @param longitud Longitud del mensaje
 * @return Número de bytes enviados o código de error
 */
int cliente_enviar(cliente_tcp_t *cliente, const char *mensaje, size_t longitud);

/**
 * @brief Recibir mensaje del servidor
 * @param cliente Puntero al cliente
 * @param buffer Buffer para almacenar la respuesta
 * @param tamaño_buffer Tamaño del buffer
 * @return Número de bytes recibidos o código de error
 */
int cliente_recibir(cliente_tcp_t *cliente, char *buffer, size_t tamaño_buffer);

/**
 * @brief Enviar mensaje y recibir respuesta (transacción completa)
 * @param cliente Puntero al cliente
 * @param mensaje Mensaje a enviar
 * @param respuesta Buffer para la respuesta
 * @param tamaño_respuesta Tamaño del buffer de respuesta
 * @param transaccion Información de la transacción (opcional)
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_transaccion(cliente_tcp_t *cliente, 
                                    const char *mensaje,
                                    char *respuesta, 
                                    size_t tamaño_respuesta,
                                    transaccion_t *transaccion);

// =============================================================================
// FUNCIONES DE UTILIDAD Y DIAGNÓSTICO
// =============================================================================

/**
 * @brief Verificar si el cliente está conectado
 * @param cliente Puntero al cliente
 * @return 1 si está conectado, 0 en caso contrario
 */
int cliente_esta_conectado(const cliente_tcp_t *cliente);

/**
 * @brief Obtener estadísticas del cliente
 * @param cliente Puntero al cliente
 * @param estadisticas Estructura para almacenar las estadísticas
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_obtener_estadisticas(const cliente_tcp_t *cliente, 
                                             estadisticas_cliente_t *estadisticas);

/**
 * @brief Mostrar estadísticas del cliente
 * @param cliente Puntero al cliente
 */
void cliente_mostrar_estadisticas(const cliente_tcp_t *cliente);

/**
 * @brief Obtener descripción de un código de error
 * @param estado Código de estado
 * @return Cadena descriptiva del error
 */
const char *cliente_obtener_error(cliente_estado_t estado);

/**
 * @brief Configurar modo verbose para debug
 * @param cliente Puntero al cliente
 * @param verbose 1 para habilitar, 0 para deshabilitar
 */
void cliente_set_verbose(cliente_tcp_t *cliente, int verbose);

/**
 * @brief Establecer timeout para operaciones
 * @param cliente Puntero al cliente
 * @param timeout_conexion Timeout para conexión (segundos)
 * @param timeout_recv Timeout para recepción (segundos)
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_set_timeouts(cliente_tcp_t *cliente, 
                                     int timeout_conexion, 
                                     int timeout_recv);

// =============================================================================
// FUNCIONES DE TESTING Y DEMOSTRACIONES
// =============================================================================

/**
 * @brief Realizar test básico de conectividad
 * @param servidor Dirección del servidor
 * @param puerto Puerto del servidor
 * @return CLIENTE_OK si el test es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_test_conectividad(const char *servidor, int puerto);

/**
 * @brief Realizar test de eco (enviar mensaje y verificar respuesta)
 * @param cliente Puntero al cliente
 * @param mensaje Mensaje a enviar
 * @return CLIENTE_OK si el test es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_test_eco(cliente_tcp_t *cliente, const char *mensaje);

/**
 * @brief Realizar múltiples transacciones para test de stress
 * @param cliente Puntero al cliente
 * @param num_transacciones Número de transacciones a realizar
 * @param mensaje_base Mensaje base para las transacciones
 * @return CLIENTE_OK si el test es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_test_stress(cliente_tcp_t *cliente, 
                                    int num_transacciones, 
                                    const char *mensaje_base);

/**
 * @brief Benchmark de latencia y throughput
 * @param cliente Puntero al cliente
 * @param num_mensajes Número de mensajes para el benchmark
 * @param tamaño_mensaje Tamaño de cada mensaje
 * @return CLIENTE_OK si el benchmark es exitoso, código de error en caso contrario
 */
cliente_estado_t cliente_benchmark(cliente_tcp_t *cliente, 
                                  int num_mensajes, 
                                  size_t tamaño_mensaje);

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN Y EXPERIENCIAS EDUCATIVAS
// =============================================================================

/**
 * @brief Demostración básica de cliente TCP
 * @param servidor Dirección del servidor
 * @param puerto Puerto del servidor
 * @return CLIENTE_OK si la demostración es exitosa
 */
cliente_estado_t demo_cliente_basico(const char *servidor, int puerto);

/**
 * @brief Demostración interactiva donde el usuario puede enviar mensajes
 * @param servidor Dirección del servidor
 * @param puerto Puerto del servidor
 * @return CLIENTE_OK si la demostración es exitosa
 */
cliente_estado_t demo_cliente_interactivo(const char *servidor, int puerto);

/**
 * @brief Demostración de reconexión automática
 * @param servidor Dirección del servidor
 * @param puerto Puerto del servidor
 * @return CLIENTE_OK si la demostración es exitosa
 */
cliente_estado_t demo_reconexion_automatica(const char *servidor, int puerto);

/**
 * @brief Demostración de manejo de errores
 * @param servidor Dirección del servidor
 * @param puerto Puerto del servidor
 * @return CLIENTE_OK si la demostración es exitosa
 */
cliente_estado_t demo_manejo_errores(const char *servidor, int puerto);

// =============================================================================
// UTILIDADES PARA SERVIDOR DE PRUEBA
// =============================================================================

/**
 * @brief Crear servidor simple para testing (single-threaded)
 * @param puerto Puerto donde ejecutar el servidor
 * @param mensaje_respuesta Mensaje que enviará como respuesta
 * @return PID del proceso servidor o -1 en caso de error
 */
pid_t crear_servidor_prueba(int puerto, const char *mensaje_respuesta);

/**
 * @brief Detener servidor de prueba
 * @param pid PID del servidor a detener
 * @return CLIENTE_OK si es exitoso, código de error en caso contrario
 */
cliente_estado_t detener_servidor_prueba(pid_t pid);

// =============================================================================
// MACROS DE UTILIDAD
// =============================================================================

/**
 * @brief Macro para logging con timestamp
 */
#define CLIENTE_LOG(cliente, formato, ...) \
    do { \
        if ((cliente) && (cliente)->config.modo_verbose) { \
            time_t now = time(NULL); \
            char *timestr = ctime(&now); \
            timestr[strlen(timestr) - 1] = '\0'; \
            printf("[%s] CLIENTE: " formato "\n", timestr, ##__VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief Macro para logging de errores
 */
#define CLIENTE_ERROR_LOG(formato, ...) \
    do { \
        fprintf(stderr, "ERROR CLIENTE: %s\n", formato); \
    } while(0)

/**
 * @brief Macro para verificar parámetros
 */
#define CLIENTE_VERIFICAR_PARAMETROS(cliente) \
    do { \
        if (!(cliente)) { \
            CLIENTE_ERROR_LOG("Cliente es NULL"); \
            return CLIENTE_ERROR_PARAMETROS; \
        } \
    } while(0)

#endif /* CLIENTE_TCP_H */
