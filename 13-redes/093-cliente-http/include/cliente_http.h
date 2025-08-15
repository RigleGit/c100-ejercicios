/**
 * @file cliente_http.h
 * @brief Cliente HTTP básico para peticiones GET usando sockets TCP
 * @version 1.0
 * @date 2025-08-05
 * 
 * Este ejercicio implementa un cliente HTTP simple que puede realizar
 * peticiones GET a servidores web usando sockets TCP de bajo nivel.
 * Demuestra el uso de getaddrinfo, connect, send, recv y gestión de buffers.
 */

#ifndef CLIENTE_HTTP_H
#define CLIENTE_HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>

/**
 * @brief Configuración por defecto
 */
#define CLIENTE_HTTP_VERSION "1.0"
#define PUERTO_HTTP_DEFAULT "80"
#define PUERTO_HTTPS_DEFAULT "443"
#define BUFFER_SIZE_DEFAULT 4096
#define TIMEOUT_DEFAULT 30
#define MAX_URL_LENGTH 2048
#define MAX_HOST_LENGTH 256
#define MAX_PATH_LENGTH 1024
#define MAX_HEADERS_LENGTH 4096
#define USER_AGENT_DEFAULT "ClienteHTTP/1.0"

/**
 * @brief Códigos de error
 */
typedef enum {
    CLIENTE_HTTP_OK = 0,
    CLIENTE_HTTP_ERROR_MEMORIA = -1,
    CLIENTE_HTTP_ERROR_RED = -2,
    CLIENTE_HTTP_ERROR_DNS = -3,
    CLIENTE_HTTP_ERROR_CONEXION = -4,
    CLIENTE_HTTP_ERROR_ENVIO = -5,
    CLIENTE_HTTP_ERROR_RECEPCION = -6,
    CLIENTE_HTTP_ERROR_TIMEOUT = -7,
    CLIENTE_HTTP_ERROR_URL_INVALIDA = -8,
    CLIENTE_HTTP_ERROR_RESPUESTA_INVALIDA = -9,
    CLIENTE_HTTP_ERROR_PARAMETRO = -10
} codigo_error_http_t;

/**
 * @brief Métodos HTTP soportados
 */
typedef enum {
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_HEAD,
    HTTP_PUT,
    HTTP_DELETE
} metodo_http_t;

/**
 * @brief Versiones HTTP soportadas
 */
typedef enum {
    HTTP_VERSION_1_0 = 0,
    HTTP_VERSION_1_1
} version_http_t;

/**
 * @brief Estructura para parsear URL
 */
typedef struct {
    char esquema[16];        // http, https
    char host[MAX_HOST_LENGTH];
    char puerto[8];
    char path[MAX_PATH_LENGTH];
    char query[MAX_PATH_LENGTH];
    char fragment[MAX_PATH_LENGTH];
} url_parseada_t;

/**
 * @brief Estructura para petición HTTP
 */
typedef struct {
    metodo_http_t metodo;
    version_http_t version;
    char url[MAX_URL_LENGTH];
    char host[MAX_HOST_LENGTH];
    char puerto[8];
    char path[MAX_PATH_LENGTH];
    char user_agent[256];
    char headers_extra[MAX_HEADERS_LENGTH];
    int timeout;
    bool seguir_redirecciones;
    int max_redirecciones;
} peticion_http_t;

/**
 * @brief Estructura para respuesta HTTP
 */
typedef struct {
    int codigo_estado;
    char razon_estado[256];
    version_http_t version;
    char headers[MAX_HEADERS_LENGTH];
    char *contenido;
    size_t tamaño_contenido;
    double tiempo_respuesta;
    size_t bytes_recibidos;
    size_t bytes_enviados;
} respuesta_http_t;

/**
 * @brief Estructura para estadísticas de conexión
 */
typedef struct {
    double tiempo_dns;
    double tiempo_conexion;
    double tiempo_envio;
    double tiempo_recepcion;
    double tiempo_total;
    size_t bytes_enviados;
    size_t bytes_recibidos;
    int intentos_conexion;
    char ip_servidor[INET6_ADDRSTRLEN];
} estadisticas_http_t;

/**
 * @brief Configuración del cliente HTTP
 */
typedef struct {
    int timeout_conexion;
    int timeout_lectura;
    int buffer_size;
    bool verbose;
    bool mostrar_headers;
    bool solo_headers;
    bool seguir_redirecciones;
    int max_redirecciones;
    char user_agent[256];
    char proxy_host[MAX_HOST_LENGTH];
    char proxy_puerto[8];
} config_cliente_http_t;

/* ================================
 * FUNCIONES PRINCIPALES DE API
 * ================================ */

/**
 * @brief Realiza una petición HTTP GET simple
 * @param url URL completa del recurso
 * @param respuesta Estructura donde almacenar la respuesta
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_get_simple(const char *url, respuesta_http_t *respuesta);

/**
 * @brief Realiza una petición HTTP con configuración personalizada
 * @param peticion Configuración de la petición
 * @param respuesta Estructura donde almacenar la respuesta
 * @param estadisticas Estadísticas de la conexión (puede ser NULL)
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_realizar_peticion(const peticion_http_t *peticion, 
                                   respuesta_http_t *respuesta,
                                   estadisticas_http_t *estadisticas);

/**
 * @brief Conecta a un servidor HTTP
 * @param host Nombre del host o IP
 * @param puerto Puerto del servidor
 * @param timeout Timeout en segundos
 * @param ip_servidor Buffer para almacenar la IP del servidor
 * @return Socket conectado o -1 en caso de error
 */
int cliente_http_conectar(const char *host, const char *puerto, 
                          int timeout, char *ip_servidor);

/**
 * @brief Envía una petición HTTP
 * @param socket Socket conectado
 * @param peticion Datos de la petición
 * @return Bytes enviados o -1 en caso de error
 */
ssize_t cliente_http_enviar_peticion(int socket, const peticion_http_t *peticion);

/**
 * @brief Recibe una respuesta HTTP completa
 * @param socket Socket conectado
 * @param respuesta Estructura donde almacenar la respuesta
 * @param timeout Timeout en segundos
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_recibir_respuesta(int socket, respuesta_http_t *respuesta, int timeout);

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

/**
 * @brief Parsea una URL en sus componentes
 * @param url URL a parsear
 * @param parseada Estructura donde almacenar los componentes
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_parsear_url(const char *url, url_parseada_t *parseada);

/**
 * @brief Construye una petición HTTP desde sus componentes
 * @param peticion Configuración de la petición
 * @param buffer Buffer donde escribir la petición
 * @param tamaño_buffer Tamaño del buffer
 * @return Longitud de la petición o -1 en caso de error
 */
ssize_t cliente_http_construir_peticion(const peticion_http_t *peticion, 
                                        char *buffer, size_t tamaño_buffer);

/**
 * @brief Parsea headers HTTP de una respuesta
 * @param headers_raw Headers en formato raw
 * @param respuesta Estructura donde almacenar los datos parseados
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_parsear_headers(const char *headers_raw, respuesta_http_t *respuesta);

/**
 * @brief Extrae el contenido del cuerpo de la respuesta
 * @param respuesta_completa Respuesta HTTP completa
 * @param respuesta Estructura donde almacenar el contenido
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_extraer_contenido(const char *respuesta_completa, respuesta_http_t *respuesta);

/* ================================
 * FUNCIONES DE CONFIGURACIÓN
 * ================================ */

/**
 * @brief Inicializa configuración por defecto
 * @param config Estructura de configuración
 */
void cliente_http_init_config(config_cliente_http_t *config);

/**
 * @brief Inicializa estructura de petición
 * @param peticion Estructura de petición
 */
void cliente_http_init_peticion(peticion_http_t *peticion);

/**
 * @brief Inicializa estructura de respuesta
 * @param respuesta Estructura de respuesta
 */
void cliente_http_init_respuesta(respuesta_http_t *respuesta);

/**
 * @brief Libera memoria de una respuesta
 * @param respuesta Estructura de respuesta
 */
void cliente_http_liberar_respuesta(respuesta_http_t *respuesta);

/* ================================
 * FUNCIONES DE DEMO Y PRUEBA
 * ================================ */

/**
 * @brief Demo básico de petición GET
 * @param url URL a consultar
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_demo_get_basico(const char *url);

/**
 * @brief Demo con headers y estadísticas
 * @param url URL a consultar
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_demo_con_estadisticas(const char *url);

/**
 * @brief Demo de múltiples peticiones
 * @param urls Array de URLs
 * @param num_urls Número de URLs
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_demo_multiples_peticiones(const char **urls, int num_urls);

/**
 * @brief Benchmark de rendimiento
 * @param url URL para el benchmark
 * @param num_peticiones Número de peticiones a realizar
 * @return Código de error o CLIENTE_HTTP_OK
 */
int cliente_http_benchmark(const char *url, int num_peticiones);

/* ================================
 * FUNCIONES DE UTILIDAD GENERAL
 * ================================ */

/**
 * @brief Convierte código de error a cadena descriptiva
 * @param codigo Código de error
 * @return Descripción del error
 */
const char* cliente_http_error_string(codigo_error_http_t codigo);

/**
 * @brief Convierte método HTTP a cadena
 * @param metodo Método HTTP
 * @return Cadena del método
 */
const char* cliente_http_metodo_string(metodo_http_t metodo);

/**
 * @brief Convierte versión HTTP a cadena
 * @param version Versión HTTP
 * @return Cadena de la versión
 */
const char* cliente_http_version_string(version_http_t version);

/**
 * @brief Valida una URL
 * @param url URL a validar
 * @return true si es válida, false en caso contrario
 */
bool cliente_http_validar_url(const char *url);

/**
 * @brief Obtiene timestamp actual en segundos
 * @return Timestamp actual
 */
double cliente_http_timestamp(void);

/**
 * @brief Formatea tamaño en bytes de forma legible
 * @param bytes Número de bytes
 * @param buffer Buffer donde escribir el resultado
 * @param tamaño_buffer Tamaño del buffer
 */
void cliente_http_formatear_bytes(size_t bytes, char *buffer, size_t tamaño_buffer);

/**
 * @brief Imprime estadísticas de una petición
 * @param estadisticas Estadísticas a imprimir
 */
void cliente_http_imprimir_estadisticas(const estadisticas_http_t *estadisticas);

/**
 * @brief Imprime headers de respuesta de forma legible
 * @param respuesta Respuesta HTTP
 */
void cliente_http_imprimir_headers(const respuesta_http_t *respuesta);

/* ================================
 * CONFIGURACIÓN DE COMPILACIÓN
 * ================================ */

#ifdef DEBUG
    #define CLIENTE_HTTP_DEBUG(fmt, ...) \
        fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define CLIENTE_HTTP_DEBUG(fmt, ...)
#endif

#define CLIENTE_HTTP_LOG(fmt, ...) \
    fprintf(stdout, "[CLIENTE_HTTP] " fmt "\n", ##__VA_ARGS__)

#define CLIENTE_HTTP_ERROR(fmt, ...) \
    fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

/* ================================
 * CONSTANTES Y MACROS
 * ================================ */

#define CLIENTE_HTTP_MAX_REDIRECTS 10
#define CLIENTE_HTTP_CHUNK_SIZE 8192
#define CLIENTE_HTTP_MAX_HEADER_SIZE 8192
#define CLIENTE_HTTP_MAX_CONTENT_SIZE (10 * 1024 * 1024) // 10MB

// Helper macros
#define CLIENTE_HTTP_SAFE_FREE(ptr) do { \
    if (ptr) { \
        free(ptr); \
        ptr = NULL; \
    } \
} while(0)

#define CLIENTE_HTTP_SAFE_CLOSE(fd) do { \
    if (fd >= 0) { \
        close(fd); \
        fd = -1; \
    } \
} while(0)

#endif // CLIENTE_HTTP_H
