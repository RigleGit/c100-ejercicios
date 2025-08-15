/**
 * @file servidor_tcp_multicliente.h
 * @brief API completa para servidor TCP multicliente con hilos POSIX
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Este header define la API completa para implementar un servidor TCP
 * que maneja múltiples clientes simultáneamente usando pthread para
 * crear un hilo por cada conexión cliente.
 * 
 * Características principales:
 * - Servidor TCP multihilo con pthread
 * - Pool de hilos configurable
 * - Manejo robusto de conexiones y errores
 * - Estadísticas de conexiones en tiempo real
 * - Configuración avanzada de servidor
 * - Logging y diagnósticos
 * - Demos educativos y herramientas de test
 */

#ifndef SERVIDOR_TCP_MULTICLIENTE_H
#define SERVIDOR_TCP_MULTICLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// =============================================================================
// CONSTANTES Y CONFIGURACIÓN
// =============================================================================

#define SERVIDOR_VERSION "1.0.0"
#define PUERTO_DEFAULT 9090
#define BUFFER_SIZE_DEFAULT 4096
#define MAX_CONEXIONES_DEFAULT 100
#define MAX_HILOS_DEFAULT 50
#define TIMEOUT_CLIENTE_DEFAULT 300  // 5 minutos
#define BACKLOG_DEFAULT 10

// Constantes para tipos de servidor
#define TIPO_ECO 1
#define TIPO_CHAT 2
#define TIPO_CUSTOM 3

// =============================================================================
// CÓDIGOS DE ERROR
// =============================================================================

typedef enum {
    SERVER_EXITO = 0,
    SERVER_ERROR_SOCKET = -1,
    SERVER_ERROR_BIND = -2,
    SERVER_ERROR_LISTEN = -3,
    SERVER_ERROR_ACCEPT = -4,
    SERVER_ERROR_THREAD = -5,
    SERVER_ERROR_MEMORIA = -6,
    SERVER_ERROR_CONFIGURACION = -7,
    SERVER_ERROR_TIMEOUT = -8,
    SERVER_ERROR_CLIENTE = -9,
    SERVER_ERROR_SISTEMA = -10
} CodigoErrorServidor;

// =============================================================================
// ESTRUCTURAS DE DATOS
// =============================================================================

/**
 * @brief Configuración del servidor TCP multicliente
 */
typedef struct {
    int puerto;                    ///< Puerto de escucha
    int max_conexiones;           ///< Máximo número de conexiones simultáneas
    int max_hilos;               ///< Máximo número de hilos worker
    int buffer_size;             ///< Tamaño del buffer por cliente
    int timeout_cliente;         ///< Timeout para clientes inactivos (segundos)
    int backlog;                 ///< Tamaño de la cola de listen()
    int tipo_servidor;           ///< Tipo de servidor (eco, chat, etc.)
    int log_detallado;           ///< Habilitar logging detallado
    int reutilizar_puerto;       ///< SO_REUSEADDR
    int keepalive;               ///< SO_KEEPALIVE
    char bind_ip[64];            ///< IP específica para bind (INADDR_ANY si vacío)
} ConfigServidor;

/**
 * @brief Información de un cliente conectado
 */
typedef struct {
    int socket_fd;               ///< Descriptor del socket del cliente
    pthread_t thread_id;         ///< ID del hilo que atiende al cliente
    struct sockaddr_in direccion; ///< Dirección IP y puerto del cliente
    time_t tiempo_conexion;      ///< Timestamp de conexión
    time_t ultima_actividad;     ///< Timestamp de última actividad
    size_t bytes_enviados;       ///< Total de bytes enviados al cliente
    size_t bytes_recibidos;      ///< Total de bytes recibidos del cliente
    size_t mensajes_enviados;    ///< Número de mensajes enviados
    size_t mensajes_recibidos;   ///< Número de mensajes recibidos
    int activo;                  ///< Flag de cliente activo
    char identificador[32];      ///< Identificador único del cliente
} InfoCliente;

/**
 * @brief Parámetros para el hilo de atención a cliente
 */
typedef struct {
    InfoCliente* cliente;        ///< Información del cliente
    ConfigServidor* config;      ///< Configuración del servidor
    void* servidor_ctx;          ///< Contexto del servidor principal
} ParametrosHilo;

/**
 * @brief Estadísticas globales del servidor
 */
typedef struct {
    size_t conexiones_totales;       ///< Total de conexiones recibidas
    size_t conexiones_activas;       ///< Conexiones actualmente activas
    size_t conexiones_rechazadas;    ///< Conexiones rechazadas por límites
    size_t hilos_activos;           ///< Hilos actualmente activos
    size_t bytes_totales_enviados;   ///< Total de bytes enviados
    size_t bytes_totales_recibidos;  ///< Total de bytes recibidos
    size_t mensajes_totales;         ///< Total de mensajes procesados
    size_t errores_red;             ///< Errores de red ocurridos
    size_t errores_hilos;           ///< Errores de hilos ocurridos
    time_t tiempo_inicio;           ///< Timestamp de inicio del servidor
    time_t tiempo_actividad;        ///< Timestamp de última actividad
    pthread_mutex_t mutex;          ///< Mutex para acceso thread-safe
} EstadisticasServidor;

/**
 * @brief Contexto principal del servidor
 */
typedef struct {
    ConfigServidor config;              ///< Configuración del servidor
    EstadisticasServidor stats;         ///< Estadísticas del servidor
    InfoCliente* clientes;              ///< Array de información de clientes
    int socket_servidor;                ///< Socket principal del servidor
    int ejecutando;                     ///< Flag de ejecución
    pthread_mutex_t mutex_clientes;     ///< Mutex para lista de clientes
    pthread_mutex_t mutex_logs;         ///< Mutex para logging thread-safe
} ContextoServidor;

// =============================================================================
// TIPOS DE FUNCIONES CALLBACK
// =============================================================================

/**
 * @brief Función callback para procesar mensajes de cliente
 * @param cliente Información del cliente
 * @param buffer Buffer con el mensaje
 * @param tamaño Tamaño del mensaje
 * @param contexto Contexto del servidor
 * @return Número de bytes a enviar de respuesta, 0 para no responder, -1 para error
 */
typedef ssize_t (*ProcesadorMensaje)(InfoCliente* cliente, const char* buffer, 
                                     size_t tamaño, void* contexto);

/**
 * @brief Función callback para manejar conexión de cliente
 * @param cliente Información del cliente
 * @param contexto Contexto del servidor
 * @return 0 si éxito, -1 si error
 */
typedef int (*ManejadorConexion)(InfoCliente* cliente, void* contexto);

/**
 * @brief Función callback para manejar desconexión de cliente
 * @param cliente Información del cliente
 * @param contexto Contexto del servidor
 */
typedef void (*ManejadorDesconexion)(InfoCliente* cliente, void* contexto);

// =============================================================================
// FUNCIONES DE CONFIGURACIÓN
// =============================================================================

/**
 * @brief Inicializa configuración por defecto del servidor
 * @param config Puntero a estructura de configuración
 */
void configurar_servidor_por_defecto(ConfigServidor* config);

/**
 * @brief Valida la configuración del servidor
 * @param config Configuración a validar
 * @return SERVER_EXITO si válida, código de error caso contrario
 */
int validar_configuracion_servidor(const ConfigServidor* config);

/**
 * @brief Aplica configuración avanzada al socket del servidor
 * @param socket_fd Descriptor del socket
 * @param config Configuración del servidor
 * @return 0 si éxito, -1 si error
 */
int aplicar_configuracion_socket(int socket_fd, const ConfigServidor* config);

// =============================================================================
// FUNCIONES PRINCIPALES DEL SERVIDOR
// =============================================================================

/**
 * @brief Inicializa el contexto del servidor
 * @param contexto Contexto del servidor a inicializar
 * @param config Configuración del servidor
 * @return SERVER_EXITO si éxito, código de error caso contrario
 */
int inicializar_servidor(ContextoServidor* contexto, const ConfigServidor* config);

/**
 * @brief Crea y configura el socket del servidor
 * @param contexto Contexto del servidor
 * @return Socket del servidor si éxito, -1 si error
 */
int crear_socket_servidor(ContextoServidor* contexto);

/**
 * @brief Ejecuta el bucle principal del servidor
 * @param contexto Contexto del servidor
 * @return SERVER_EXITO si éxito, código de error caso contrario
 */
int ejecutar_servidor(ContextoServidor* contexto);

/**
 * @brief Detiene el servidor de forma ordenada
 * @param contexto Contexto del servidor
 */
void detener_servidor(ContextoServidor* contexto);

/**
 * @brief Limpia recursos del servidor
 * @param contexto Contexto del servidor
 */
void limpiar_servidor(ContextoServidor* contexto);

// =============================================================================
// FUNCIONES DE MANEJO DE CLIENTES
// =============================================================================

/**
 * @brief Acepta una nueva conexión de cliente
 * @param contexto Contexto del servidor
 * @return Puntero a InfoCliente si éxito, NULL si error
 */
InfoCliente* aceptar_cliente(ContextoServidor* contexto);

/**
 * @brief Función principal del hilo de atención a cliente
 * @param arg Puntero a ParametrosHilo
 * @return NULL
 */
void* atender_cliente(void* arg);

/**
 * @brief Registra un nuevo cliente en el servidor
 * @param contexto Contexto del servidor
 * @param cliente_fd Socket del cliente
 * @param direccion Dirección del cliente
 * @return Puntero a InfoCliente si éxito, NULL si error
 */
InfoCliente* registrar_cliente(ContextoServidor* contexto, int cliente_fd, 
                              const struct sockaddr_in* direccion);

/**
 * @brief Desregistra un cliente del servidor
 * @param contexto Contexto del servidor
 * @param cliente Información del cliente
 */
void desregistrar_cliente(ContextoServidor* contexto, InfoCliente* cliente);

/**
 * @brief Busca cliente por socket file descriptor
 * @param contexto Contexto del servidor
 * @param socket_fd Descriptor del socket a buscar
 * @return Puntero a InfoCliente si encontrado, NULL caso contrario
 */
InfoCliente* buscar_cliente_por_socket(ContextoServidor* contexto, int socket_fd);

/**
 * @brief Desconecta un cliente específico
 * @param contexto Contexto del servidor
 * @param cliente Cliente a desconectar
 */
void desconectar_cliente(ContextoServidor* contexto, InfoCliente* cliente);

/**
 * @brief Desconecta clientes inactivos por timeout
 * @param contexto Contexto del servidor
 * @return Número de clientes desconectados
 */
int limpiar_clientes_inactivos(ContextoServidor* contexto);

// =============================================================================
// FUNCIONES DE COMUNICACIÓN
// =============================================================================

/**
 * @brief Envía mensaje a un cliente específico
 * @param cliente Cliente destinatario
 * @param mensaje Mensaje a enviar
 * @param tamaño Tamaño del mensaje
 * @return Número de bytes enviados, -1 si error
 */
ssize_t enviar_a_cliente(InfoCliente* cliente, const char* mensaje, size_t tamaño);

/**
 * @brief Recibe mensaje de un cliente
 * @param cliente Cliente emisor
 * @param buffer Buffer para recibir mensaje
 * @param tamaño_buffer Tamaño del buffer
 * @return Número de bytes recibidos, 0 si desconexión, -1 si error
 */
ssize_t recibir_de_cliente(InfoCliente* cliente, char* buffer, size_t tamaño_buffer);

/**
 * @brief Envía mensaje a todos los clientes conectados (broadcast)
 * @param contexto Contexto del servidor
 * @param mensaje Mensaje a enviar
 * @param tamaño Tamaño del mensaje
 * @param excluir Cliente a excluir del broadcast (opcional)
 * @return Número de clientes que recibieron el mensaje
 */
int enviar_broadcast(ContextoServidor* contexto, const char* mensaje, 
                    size_t tamaño, InfoCliente* excluir);

// =============================================================================
// FUNCIONES DE ESTADÍSTICAS Y LOGGING
// =============================================================================

/**
 * @brief Inicializa estadísticas del servidor
 * @param stats Estadísticas a inicializar
 */
void inicializar_estadisticas(EstadisticasServidor* stats);

/**
 * @brief Actualiza estadísticas de conexión
 * @param stats Estadísticas del servidor
 * @param nueva_conexion Si es una nueva conexión
 * @param desconexion Si es una desconexión
 */
void actualizar_estadisticas_conexion(EstadisticasServidor* stats, 
                                     int nueva_conexion, int desconexion);

/**
 * @brief Actualiza estadísticas de comunicación
 * @param stats Estadísticas del servidor
 * @param bytes_enviados Bytes enviados
 * @param bytes_recibidos Bytes recibidos
 */
void actualizar_estadisticas_comunicacion(EstadisticasServidor* stats,
                                         size_t bytes_enviados, size_t bytes_recibidos);

/**
 * @brief Muestra estadísticas del servidor
 * @param contexto Contexto del servidor
 */
void mostrar_estadisticas_servidor(const ContextoServidor* contexto);

/**
 * @brief Registra evento en log de forma thread-safe
 * @param contexto Contexto del servidor
 * @param nivel Nivel de log (INFO, WARN, ERROR)
 * @param formato Formato del mensaje (printf-style)
 * @param ... Argumentos del formato
 */
void log_servidor(ContextoServidor* contexto, const char* nivel, 
                 const char* formato, ...);

// =============================================================================
// FUNCIONES AUXILIARES
// =============================================================================

/**
 * @brief Obtiene timestamp actual
 * @return Timestamp actual en time_t
 */
time_t obtener_timestamp_actual(void);

/**
 * @brief Formatea dirección IP y puerto como string
 * @param direccion Dirección a formatear
 * @param buffer Buffer para el resultado
 * @param tamaño_buffer Tamaño del buffer
 * @return Puntero al buffer formateado
 */
char* formatear_direccion_cliente(const struct sockaddr_in* direccion, 
                                 char* buffer, size_t tamaño_buffer);

/**
 * @brief Genera identificador único para cliente
 * @param direccion Dirección del cliente
 * @param buffer Buffer para el identificador
 * @param tamaño_buffer Tamaño del buffer
 * @return Puntero al buffer con identificador
 */
char* generar_id_cliente(const struct sockaddr_in* direccion, 
                        char* buffer, size_t tamaño_buffer);

/**
 * @brief Instala manejadores de señales para cierre ordenado
 * @param contexto Contexto del servidor
 */
void instalar_manejadores_senales(ContextoServidor* contexto);

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN Y TESTING
// =============================================================================

/**
 * @brief Ejecuta demo de servidor eco multicliente
 * @param puerto Puerto para el demo (0 para usar default)
 * @return 0 si éxito, -1 si error
 */
int demo_servidor_eco(int puerto);

/**
 * @brief Ejecuta demo de servidor chat multicliente
 * @param puerto Puerto para el demo (0 para usar default)
 * @return 0 si éxito, -1 si error
 */
int demo_servidor_chat(int puerto);

/**
 * @brief Ejecuta prueba de stress con múltiples clientes
 * @param puerto Puerto del servidor
 * @param num_clientes Número de clientes simulados
 * @param duracion_segundos Duración de la prueba
 * @return 0 si éxito, -1 si error
 */
int prueba_stress_multicliente(int puerto, int num_clientes, int duracion_segundos);

/**
 * @brief Prueba rendimiento del servidor con métricas
 * @param puerto Puerto del servidor
 * @param num_mensajes Número de mensajes por cliente
 * @param num_clientes Número de clientes concurrentes
 * @return 0 si éxito, -1 si error
 */
int probar_rendimiento_servidor(int puerto, int num_mensajes, int num_clientes);

/**
 * @brief Ejecuta diagnósticos del sistema para servidores multithread
 */
void diagnosticar_sistema_multithread(void);

// =============================================================================
// MACROS DE UTILIDAD
// =============================================================================

#define LOG_INFO(ctx, ...) log_servidor(ctx, "INFO", __VA_ARGS__)
#define LOG_WARN(ctx, ...) log_servidor(ctx, "WARN", __VA_ARGS__)
#define LOG_ERROR(ctx, ...) log_servidor(ctx, "ERROR", __VA_ARGS__)

#define LOCK_CLIENTES(ctx) pthread_mutex_lock(&(ctx)->mutex_clientes)
#define UNLOCK_CLIENTES(ctx) pthread_mutex_unlock(&(ctx)->mutex_clientes)

#define LOCK_STATS(stats) pthread_mutex_lock(&(stats)->mutex)
#define UNLOCK_STATS(stats) pthread_mutex_unlock(&(stats)->mutex)

#define CLIENTE_ACTIVO(cliente) ((cliente) && (cliente)->activo)
#define CLIENTE_TIMEOUT(cliente, timeout) \
    (time(NULL) - (cliente)->ultima_actividad > (timeout))

// =============================================================================
// CONSTANTES PARA DEMOS Y TESTS
// =============================================================================

#define PUERTO_DEMO_ECO 9090
#define PUERTO_DEMO_CHAT 9091
#define PUERTO_TEST_STRESS 9092
#define PUERTO_TEST_RENDIMIENTO 9093

#define MENSAJE_BIENVENIDA "Bienvenido al servidor TCP multicliente\n"
#define MENSAJE_DESPEDIDA "Desconectando cliente...\n"
#define COMANDO_QUIT "quit"
#define COMANDO_STATS "stats"
#define COMANDO_HELP "help"

#endif // SERVIDOR_TCP_MULTICLIENTE_H
