/**
 * @file cliente_http.c
 * @brief Implementación del cliente HTTP
 */

#include "cliente_http.h"
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

/* ================================
 * VARIABLES GLOBALES
 * ================================ */

static config_cliente_http_t config_global = {0};
static bool config_inicializada = false;

/* ================================
 * FUNCIONES AUXILIARES INTERNAS
 * ================================ */

/**
 * @brief Configura timeout para socket
 */
static int configurar_timeout_socket(int socket, int timeout_sec) {
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        return -1;
    }
    
    if (setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        return -1;
    }
    
    return 0;
}

/**
 * @brief Convierte hostname a IP para logging
 */
static int obtener_ip_servidor(struct addrinfo *info, char *ip_buffer) {
    void *addr;
    
    if (info->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)info->ai_addr;
        addr = &(ipv4->sin_addr);
    } else if (info->ai_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)info->ai_addr;
        addr = &(ipv6->sin6_addr);
    } else {
        return -1;
    }
    
    if (!inet_ntop(info->ai_family, addr, ip_buffer, INET6_ADDRSTRLEN)) {
        return -1;
    }
    
    return 0;
}

/**
 * @brief Envía datos completos por socket
 */
static ssize_t enviar_completo(int socket, const char *buffer, size_t longitud) {
    size_t enviado = 0;
    ssize_t resultado;
    
    while (enviado < longitud) {
        resultado = send(socket, buffer + enviado, longitud - enviado, 0);
        if (resultado < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (resultado == 0) {
            return enviado; // Conexión cerrada
        }
        enviado += resultado;
    }
    
    return enviado;
}

/**
 * @brief Recibe datos hasta encontrar delimitador
 */
static ssize_t recibir_hasta_delimitador(int socket, char *buffer, size_t tamaño_buffer, 
                                        const char *delimitador) {
    size_t recibido = 0;
    ssize_t resultado;
    size_t len_delim = strlen(delimitador);
    
    while (recibido < tamaño_buffer - 1) {
        resultado = recv(socket, buffer + recibido, 1, 0);
        if (resultado < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (resultado == 0) {
            break; // Conexión cerrada
        }
        
        recibido++;
        buffer[recibido] = '\0';
        
        // Verificar si encontramos el delimitador
        if (recibido >= len_delim) {
            if (strstr(buffer + recibido - len_delim, delimitador)) {
                break;
            }
        }
    }
    
    return recibido;
}

/* ================================
 * IMPLEMENTACIÓN DE FUNCIONES PRINCIPALES
 * ================================ */

int cliente_http_conectar(const char *host, const char *puerto, 
                          int timeout, char *ip_servidor) {
    struct addrinfo hints, *resultado, *ptr;
    int socket_fd = -1;
    int error_code;
    
    // Configurar hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP;
    
    // Resolver hostname
    error_code = getaddrinfo(host, puerto, &hints, &resultado);
    if (error_code != 0) {
        CLIENTE_HTTP_ERROR("Error en getaddrinfo para %s:%s - %s", 
                          host, puerto, gai_strerror(error_code));
        return CLIENTE_HTTP_ERROR_DNS;
    }
    
    // Intentar conectar con cada dirección devuelta
    for (ptr = resultado; ptr != NULL; ptr = ptr->ai_next) {
        socket_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (socket_fd < 0) {
            continue;
        }
        
        // Configurar timeout
        if (timeout > 0) {
            configurar_timeout_socket(socket_fd, timeout);
        }
        
        // Intentar conectar
        if (connect(socket_fd, ptr->ai_addr, ptr->ai_addrlen) == 0) {
            // Conexión exitosa
            if (ip_servidor) {
                obtener_ip_servidor(ptr, ip_servidor);
            }
            break;
        }
        
        // Falló la conexión, cerrar socket y probar siguiente
        close(socket_fd);
        socket_fd = -1;
    }
    
    freeaddrinfo(resultado);
    
    if (socket_fd < 0) {
        CLIENTE_HTTP_ERROR("No se pudo conectar a %s:%s", host, puerto);
        return CLIENTE_HTTP_ERROR_CONEXION;
    }
    
    return socket_fd;
}

ssize_t cliente_http_enviar_peticion(int socket, const peticion_http_t *peticion) {
    char buffer_peticion[4096];
    ssize_t longitud_peticion;
    
    // Construir petición HTTP
    longitud_peticion = cliente_http_construir_peticion(peticion, 
                                                       buffer_peticion, 
                                                       sizeof(buffer_peticion));
    if (longitud_peticion < 0) {
        return CLIENTE_HTTP_ERROR_PARAMETRO;
    }
    
    // Enviar petición
    ssize_t enviado = enviar_completo(socket, buffer_peticion, longitud_peticion);
    if (enviado < 0) {
        CLIENTE_HTTP_ERROR("Error al enviar petición HTTP");
        return CLIENTE_HTTP_ERROR_ENVIO;
    }
    
    CLIENTE_HTTP_DEBUG("Enviada petición HTTP (%zd bytes)", enviado);
    return enviado;
}

int cliente_http_recibir_respuesta(int socket, respuesta_http_t *respuesta, int timeout) {
    char buffer[BUFFER_SIZE_DEFAULT];
    char *contenido_completo = NULL;
    size_t tamaño_total = 0;
    size_t capacidad = BUFFER_SIZE_DEFAULT;
    ssize_t recibido;
    bool headers_completos = false;
    char *pos_separador;
    
    // Configurar timeout si se especifica
    if (timeout > 0) {
        configurar_timeout_socket(socket, timeout);
    }
    
    // Allocar buffer inicial para el contenido
    contenido_completo = malloc(capacidad);
    if (!contenido_completo) {
        return CLIENTE_HTTP_ERROR_MEMORIA;
    }
    
    // Recibir datos en chunks
    while ((recibido = recv(socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[recibido] = '\0';
        
        // Verificar si necesitamos más espacio
        if (tamaño_total + recibido >= capacidad) {
            capacidad *= 2;
            char *nuevo_buffer = realloc(contenido_completo, capacidad);
            if (!nuevo_buffer) {
                free(contenido_completo);
                return CLIENTE_HTTP_ERROR_MEMORIA;
            }
            contenido_completo = nuevo_buffer;
        }
        
        // Agregar datos recibidos
        memcpy(contenido_completo + tamaño_total, buffer, recibido);
        tamaño_total += recibido;
        contenido_completo[tamaño_total] = '\0';
        
        // Si aún no hemos encontrado el final de headers, buscarlo
        if (!headers_completos) {
            pos_separador = strstr(contenido_completo, "\r\n\r\n");
            if (pos_separador) {
                headers_completos = true;
                
                // Parsear headers
                *pos_separador = '\0';
                int resultado = cliente_http_parsear_headers(contenido_completo, respuesta);
                *pos_separador = '\r'; // Restaurar
                
                if (resultado != CLIENTE_HTTP_OK) {
                    free(contenido_completo);
                    return resultado;
                }
                
                // Si es una respuesta HEAD o el código indica que no hay contenido,
                // podemos parar aquí
                if (respuesta->codigo_estado == 204 || respuesta->codigo_estado == 304) {
                    break;
                }
            }
        }
        
        // Verificar límite de tamaño
        if (tamaño_total > CLIENTE_HTTP_MAX_CONTENT_SIZE) {
            CLIENTE_HTTP_ERROR("Respuesta demasiado grande");
            free(contenido_completo);
            return CLIENTE_HTTP_ERROR_RESPUESTA_INVALIDA;
        }
    }
    
    if (recibido < 0) {
        CLIENTE_HTTP_ERROR("Error al recibir respuesta HTTP");
        free(contenido_completo);
        return CLIENTE_HTTP_ERROR_RECEPCION;
    }
    
    // Guardar contenido completo en la respuesta
    respuesta->contenido = contenido_completo;
    respuesta->tamaño_contenido = tamaño_total;
    respuesta->bytes_recibidos = tamaño_total;
    
    CLIENTE_HTTP_DEBUG("Recibida respuesta HTTP (%zu bytes)", tamaño_total);
    return CLIENTE_HTTP_OK;
}

int cliente_http_parsear_url(const char *url, url_parseada_t *parseada) {
    if (!url || !parseada) {
        return CLIENTE_HTTP_ERROR_PARAMETRO;
    }
    
    // Limpiar estructura
    memset(parseada, 0, sizeof(url_parseada_t));
    
    // Buscar esquema (http:// o https://)
    const char *pos = strstr(url, "://");
    if (pos) {
        size_t len_esquema = pos - url;
        if (len_esquema >= sizeof(parseada->esquema)) {
            return CLIENTE_HTTP_ERROR_URL_INVALIDA;
        }
        strncpy(parseada->esquema, url, len_esquema);
        url = pos + 3; // Saltar "://"
    } else {
        strcpy(parseada->esquema, "http"); // Por defecto HTTP
    }
    
    // Determinar puerto por defecto
    if (strcmp(parseada->esquema, "https") == 0) {
        strcpy(parseada->puerto, PUERTO_HTTPS_DEFAULT);
    } else {
        strcpy(parseada->puerto, PUERTO_HTTP_DEFAULT);
    }
    
    // Buscar el primer '/' para separar host de path
    const char *pos_path = strchr(url, '/');
    const char *pos_query = strchr(url, '?');
    const char *pos_fragment = strchr(url, '#');
    
    // Extraer host y puerto
    size_t len_host;
    if (pos_path) {
        len_host = pos_path - url;
    } else if (pos_query) {
        len_host = pos_query - url;
    } else if (pos_fragment) {
        len_host = pos_fragment - url;
    } else {
        len_host = strlen(url);
    }
    
    if (len_host >= sizeof(parseada->host)) {
        return CLIENTE_HTTP_ERROR_URL_INVALIDA;
    }
    
    // Buscar puerto en el host
    const char *pos_puerto = memchr(url, ':', len_host);
    if (pos_puerto) {
        size_t len_host_sin_puerto = pos_puerto - url;
        strncpy(parseada->host, url, len_host_sin_puerto);
        
        size_t len_puerto = len_host - len_host_sin_puerto - 1;
        if (len_puerto >= sizeof(parseada->puerto)) {
            return CLIENTE_HTTP_ERROR_URL_INVALIDA;
        }
        strncpy(parseada->puerto, pos_puerto + 1, len_puerto);
    } else {
        strncpy(parseada->host, url, len_host);
    }
    
    // Extraer path
    if (pos_path) {
        const char *fin_path;
        if (pos_query) {
            fin_path = pos_query;
        } else if (pos_fragment) {
            fin_path = pos_fragment;
        } else {
            fin_path = pos_path + strlen(pos_path);
        }
        
        size_t len_path = fin_path - pos_path;
        if (len_path >= sizeof(parseada->path)) {
            return CLIENTE_HTTP_ERROR_URL_INVALIDA;
        }
        strncpy(parseada->path, pos_path, len_path);
    } else {
        strcpy(parseada->path, "/");
    }
    
    // Extraer query
    if (pos_query) {
        const char *fin_query = pos_fragment ? pos_fragment : (pos_query + strlen(pos_query));
        size_t len_query = fin_query - pos_query - 1;
        if (len_query < sizeof(parseada->query)) {
            strncpy(parseada->query, pos_query + 1, len_query);
        }
    }
    
    // Extraer fragment
    if (pos_fragment) {
        strncpy(parseada->fragment, pos_fragment + 1, 
                sizeof(parseada->fragment) - 1);
    }
    
    return CLIENTE_HTTP_OK;
}

ssize_t cliente_http_construir_peticion(const peticion_http_t *peticion, 
                                        char *buffer, size_t tamaño_buffer) {
    if (!peticion || !buffer) {
        return -1;
    }
    
    int resultado = snprintf(buffer, tamaño_buffer,
        "%s %s HTTP/%s\r\n"
        "Host: %s\r\n"
        "User-Agent: %s\r\n"
        "Connection: close\r\n"
        "%s"
        "\r\n",
        cliente_http_metodo_string(peticion->metodo),
        peticion->path,
        cliente_http_version_string(peticion->version),
        peticion->host,
        peticion->user_agent,
        peticion->headers_extra
    );
    
    if (resultado < 0 || (size_t)resultado >= tamaño_buffer) {
        return -1;
    }
    
    return resultado;
}

int cliente_http_parsear_headers(const char *headers_raw, respuesta_http_t *respuesta) {
    if (!headers_raw || !respuesta) {
        return CLIENTE_HTTP_ERROR_PARAMETRO;
    }
    
    // Buscar la línea de estado (primera línea)
    const char *fin_linea = strchr(headers_raw, '\r');
    if (!fin_linea) {
        fin_linea = strchr(headers_raw, '\n');
    }
    if (!fin_linea) {
        return CLIENTE_HTTP_ERROR_RESPUESTA_INVALIDA;
    }
    
    // Parsear línea de estado: "HTTP/1.1 200 OK"
    char linea_estado[256];
    size_t len_linea = fin_linea - headers_raw;
    if (len_linea >= sizeof(linea_estado)) {
        return CLIENTE_HTTP_ERROR_RESPUESTA_INVALIDA;
    }
    
    strncpy(linea_estado, headers_raw, len_linea);
    linea_estado[len_linea] = '\0';
    
    // Parsear versión HTTP
    if (strncmp(linea_estado, "HTTP/1.0", 8) == 0) {
        respuesta->version = HTTP_VERSION_1_0;
    } else if (strncmp(linea_estado, "HTTP/1.1", 8) == 0) {
        respuesta->version = HTTP_VERSION_1_1;
    } else {
        respuesta->version = HTTP_VERSION_1_0; // Por defecto
    }
    
    // Extraer código de estado
    char *pos_codigo = strchr(linea_estado, ' ');
    if (pos_codigo) {
        respuesta->codigo_estado = atoi(pos_codigo + 1);
        
        // Extraer razón del estado
        char *pos_razon = strchr(pos_codigo + 1, ' ');
        if (pos_razon) {
            strncpy(respuesta->razon_estado, pos_razon + 1, 
                    sizeof(respuesta->razon_estado) - 1);
        }
    }
    
    // Copiar headers completos
    strncpy(respuesta->headers, headers_raw, 
            sizeof(respuesta->headers) - 1);
    
    return CLIENTE_HTTP_OK;
}

int cliente_http_extraer_contenido(const char *respuesta_completa, respuesta_http_t *respuesta) {
    if (!respuesta_completa || !respuesta) {
        return CLIENTE_HTTP_ERROR_PARAMETRO;
    }
    
    // Buscar separador entre headers y contenido
    const char *separador = strstr(respuesta_completa, "\r\n\r\n");
    if (!separador) {
        separador = strstr(respuesta_completa, "\n\n");
        if (separador) {
            separador += 2;
        }
    } else {
        separador += 4;
    }
    
    if (separador) {
        size_t tamaño_contenido = strlen(separador);
        respuesta->contenido = malloc(tamaño_contenido + 1);
        if (!respuesta->contenido) {
            return CLIENTE_HTTP_ERROR_MEMORIA;
        }
        
        strcpy(respuesta->contenido, separador);
        respuesta->tamaño_contenido = tamaño_contenido;
    }
    
    return CLIENTE_HTTP_OK;
}

/* ================================
 * FUNCIONES DE ALTO NIVEL
 * ================================ */

int cliente_http_get_simple(const char *url, respuesta_http_t *respuesta) {
    peticion_http_t peticion;
    cliente_http_init_peticion(&peticion);
    
    // Parsear URL
    url_parseada_t url_parseada;
    int resultado = cliente_http_parsear_url(url, &url_parseada);
    if (resultado != CLIENTE_HTTP_OK) {
        return resultado;
    }
    
    // Configurar petición
    strcpy(peticion.host, url_parseada.host);
    strcpy(peticion.puerto, url_parseada.puerto);
    strcpy(peticion.path, url_parseada.path);
    if (strlen(url_parseada.query) > 0) {
        strcat(peticion.path, "?");
        strcat(peticion.path, url_parseada.query);
    }
    
    return cliente_http_realizar_peticion(&peticion, respuesta, NULL);
}

int cliente_http_realizar_peticion(const peticion_http_t *peticion, 
                                   respuesta_http_t *respuesta,
                                   estadisticas_http_t *estadisticas) {
    int socket_fd = -1;
    int resultado = CLIENTE_HTTP_OK;
    double tiempo_inicio, tiempo_fin;
    char ip_servidor[INET6_ADDRSTRLEN] = {0};
    
    if (!peticion || !respuesta) {
        return CLIENTE_HTTP_ERROR_PARAMETRO;
    }
    
    cliente_http_init_respuesta(respuesta);
    
    if (estadisticas) {
        memset(estadisticas, 0, sizeof(estadisticas_http_t));
        // No existe campo tiempo_inicio en estadisticas_http_t; usar tiempo_total como acumulador final
    }
    
    tiempo_inicio = cliente_http_timestamp();
    
    // Conectar al servidor
    socket_fd = cliente_http_conectar(peticion->host, peticion->puerto, 
                                     peticion->timeout, ip_servidor);
    if (socket_fd < 0) {
        resultado = socket_fd; // El error ya está codificado
        goto cleanup;
    }
    
    if (estadisticas) {
        estadisticas->tiempo_conexion = cliente_http_timestamp() - tiempo_inicio;
        strcpy(estadisticas->ip_servidor, ip_servidor);
        estadisticas->intentos_conexion = 1;
    }
    
    // Enviar petición
    tiempo_inicio = cliente_http_timestamp();
    ssize_t bytes_enviados = cliente_http_enviar_peticion(socket_fd, peticion);
    if (bytes_enviados < 0) {
        resultado = CLIENTE_HTTP_ERROR_ENVIO;
        goto cleanup;
    }
    
    if (estadisticas) {
        estadisticas->tiempo_envio = cliente_http_timestamp() - tiempo_inicio;
        estadisticas->bytes_enviados = bytes_enviados;
    }
    
    // Recibir respuesta
    tiempo_inicio = cliente_http_timestamp();
    resultado = cliente_http_recibir_respuesta(socket_fd, respuesta, peticion->timeout);
    if (resultado != CLIENTE_HTTP_OK) {
        goto cleanup;
    }
    
    tiempo_fin = cliente_http_timestamp();
    respuesta->tiempo_respuesta = tiempo_fin - tiempo_inicio;
    respuesta->bytes_enviados = bytes_enviados;
    
    if (estadisticas) {
        estadisticas->tiempo_recepcion = tiempo_fin - tiempo_inicio;
        estadisticas->bytes_recibidos = respuesta->bytes_recibidos;
        estadisticas->tiempo_total = (estadisticas->tiempo_dns + estadisticas->tiempo_conexion + estadisticas->tiempo_envio + estadisticas->tiempo_recepcion);
    }
    
cleanup:
    CLIENTE_HTTP_SAFE_CLOSE(socket_fd);
    return resultado;
}

/* ================================
 * FUNCIONES DE INICIALIZACIÓN
 * ================================ */

void cliente_http_init_config(config_cliente_http_t *config) {
    if (!config) return;
    
    memset(config, 0, sizeof(config_cliente_http_t));
    config->timeout_conexion = TIMEOUT_DEFAULT;
    config->timeout_lectura = TIMEOUT_DEFAULT;
    config->buffer_size = BUFFER_SIZE_DEFAULT;
    config->verbose = false;
    config->mostrar_headers = false;
    config->solo_headers = false;
    config->seguir_redirecciones = false;
    config->max_redirecciones = CLIENTE_HTTP_MAX_REDIRECTS;
    strcpy(config->user_agent, USER_AGENT_DEFAULT);
}

void cliente_http_init_peticion(peticion_http_t *peticion) {
    if (!peticion) return;
    
    memset(peticion, 0, sizeof(peticion_http_t));
    peticion->metodo = HTTP_GET;
    peticion->version = HTTP_VERSION_1_0;
    peticion->timeout = TIMEOUT_DEFAULT;
    peticion->seguir_redirecciones = false;
    peticion->max_redirecciones = CLIENTE_HTTP_MAX_REDIRECTS;
    strcpy(peticion->user_agent, USER_AGENT_DEFAULT);
}

void cliente_http_init_respuesta(respuesta_http_t *respuesta) {
    if (!respuesta) return;
    
    memset(respuesta, 0, sizeof(respuesta_http_t));
    respuesta->codigo_estado = 0;
    respuesta->contenido = NULL;
    respuesta->tamaño_contenido = 0;
}

void cliente_http_liberar_respuesta(respuesta_http_t *respuesta) {
    if (!respuesta) return;
    
    CLIENTE_HTTP_SAFE_FREE(respuesta->contenido);
    respuesta->tamaño_contenido = 0;
    respuesta->bytes_recibidos = 0;
}

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

const char* cliente_http_error_string(codigo_error_http_t codigo) {
    switch (codigo) {
        case CLIENTE_HTTP_OK: return "Éxito";
        case CLIENTE_HTTP_ERROR_MEMORIA: return "Error de memoria";
        case CLIENTE_HTTP_ERROR_RED: return "Error de red";
        case CLIENTE_HTTP_ERROR_DNS: return "Error de resolución DNS";
        case CLIENTE_HTTP_ERROR_CONEXION: return "Error de conexión";
        case CLIENTE_HTTP_ERROR_ENVIO: return "Error al enviar datos";
        case CLIENTE_HTTP_ERROR_RECEPCION: return "Error al recibir datos";
        case CLIENTE_HTTP_ERROR_TIMEOUT: return "Timeout";
        case CLIENTE_HTTP_ERROR_URL_INVALIDA: return "URL inválida";
        case CLIENTE_HTTP_ERROR_RESPUESTA_INVALIDA: return "Respuesta inválida";
        case CLIENTE_HTTP_ERROR_PARAMETRO: return "Parámetro inválido";
        default: return "Error desconocido";
    }
}

const char* cliente_http_metodo_string(metodo_http_t metodo) {
    switch (metodo) {
        case HTTP_GET: return "GET";
        case HTTP_POST: return "POST";
        case HTTP_HEAD: return "HEAD";
        case HTTP_PUT: return "PUT";
        case HTTP_DELETE: return "DELETE";
        default: return "GET";
    }
}

const char* cliente_http_version_string(version_http_t version) {
    switch (version) {
        case HTTP_VERSION_1_0: return "1.0";
        case HTTP_VERSION_1_1: return "1.1";
        default: return "1.0";
    }
}

bool cliente_http_validar_url(const char *url) {
    if (!url || strlen(url) == 0) {
        return false;
    }
    
    url_parseada_t parseada;
    return cliente_http_parsear_url(url, &parseada) == CLIENTE_HTTP_OK;
}

double cliente_http_timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void cliente_http_formatear_bytes(size_t bytes, char *buffer, size_t tamaño_buffer) {
    const char *unidades[] = {"B", "KB", "MB", "GB"};
    int unidad = 0;
    double valor = bytes;
    
    while (valor >= 1024 && unidad < 3) {
        valor /= 1024;
        unidad++;
    }
    
    if (unidad == 0) {
        snprintf(buffer, tamaño_buffer, "%zu %s", bytes, unidades[unidad]);
    } else {
        snprintf(buffer, tamaño_buffer, "%.2f %s", valor, unidades[unidad]);
    }
}

void cliente_http_imprimir_estadisticas(const estadisticas_http_t *estadisticas) {
    if (!estadisticas) return;
    
    char bytes_env[32], bytes_rec[32];
    cliente_http_formatear_bytes(estadisticas->bytes_enviados, bytes_env, sizeof(bytes_env));
    cliente_http_formatear_bytes(estadisticas->bytes_recibidos, bytes_rec, sizeof(bytes_rec));
    
    printf("=== Estadísticas de Conexión ===\n");
    printf("Servidor IP: %s\n", estadisticas->ip_servidor);
    printf("Tiempo DNS: %.3f ms\n", estadisticas->tiempo_dns * 1000);
    printf("Tiempo conexión: %.3f ms\n", estadisticas->tiempo_conexion * 1000);
    printf("Tiempo envío: %.3f ms\n", estadisticas->tiempo_envio * 1000);
    printf("Tiempo recepción: %.3f ms\n", estadisticas->tiempo_recepcion * 1000);
    printf("Tiempo total: %.3f ms\n", estadisticas->tiempo_total * 1000);
    printf("Bytes enviados: %s\n", bytes_env);
    printf("Bytes recibidos: %s\n", bytes_rec);
    printf("Intentos de conexión: %d\n", estadisticas->intentos_conexion);
}

void cliente_http_imprimir_headers(const respuesta_http_t *respuesta) {
    if (!respuesta) return;
    
    printf("=== Headers de Respuesta ===\n");
    printf("Código: %d %s\n", respuesta->codigo_estado, respuesta->razon_estado);
    printf("Versión: HTTP/%s\n", cliente_http_version_string(respuesta->version));
    printf("\n%s\n", respuesta->headers);
}

/* ================================
 * FUNCIONES DE DEMO
 * ================================ */

int cliente_http_demo_get_basico(const char *url) {
    respuesta_http_t respuesta;
    int resultado;
    
    printf("=== Demo GET Básico ===\n");
    printf("URL: %s\n\n", url);
    
    resultado = cliente_http_get_simple(url, &respuesta);
    
    if (resultado == CLIENTE_HTTP_OK) {
        printf("Respuesta recibida (Código: %d)\n", respuesta.codigo_estado);
        printf("Tamaño: %zu bytes\n\n", respuesta.tamaño_contenido);
        
        if (respuesta.contenido) {
            // Mostrar solo los primeros 500 caracteres del contenido
            size_t mostrar = respuesta.tamaño_contenido > 500 ? 500 : respuesta.tamaño_contenido;
            printf("Contenido (primeros %zu caracteres):\n", mostrar);
            printf("---\n");
            fwrite(respuesta.contenido, 1, mostrar, stdout);
            if (respuesta.tamaño_contenido > 500) {
                printf("\n... [truncado] ...\n");
            }
            printf("\n---\n");
        }
        
        cliente_http_liberar_respuesta(&respuesta);
    } else {
        printf("Error: %s\n", cliente_http_error_string(resultado));
    }
    
    return resultado;
}

int cliente_http_demo_con_estadisticas(const char *url) {
    peticion_http_t peticion;
    respuesta_http_t respuesta;
    estadisticas_http_t estadisticas;
    int resultado;
    
    printf("=== Demo con Estadísticas ===\n");
    printf("URL: %s\n\n", url);
    
    cliente_http_init_peticion(&peticion);
    
    // Parsear URL
    url_parseada_t url_parseada;
    resultado = cliente_http_parsear_url(url, &url_parseada);
    if (resultado != CLIENTE_HTTP_OK) {
        printf("Error al parsear URL: %s\n", cliente_http_error_string(resultado));
        return resultado;
    }
    
    strcpy(peticion.host, url_parseada.host);
    strcpy(peticion.puerto, url_parseada.puerto);
    strcpy(peticion.path, url_parseada.path);
    
    resultado = cliente_http_realizar_peticion(&peticion, &respuesta, &estadisticas);
    
    if (resultado == CLIENTE_HTTP_OK) {
        cliente_http_imprimir_estadisticas(&estadisticas);
        printf("\n");
        cliente_http_imprimir_headers(&respuesta);
        printf("\nTamaño del contenido: %zu bytes\n", respuesta.tamaño_contenido);
        
        cliente_http_liberar_respuesta(&respuesta);
    } else {
        printf("Error: %s\n", cliente_http_error_string(resultado));
    }
    
    return resultado;
}

int cliente_http_demo_multiples_peticiones(const char **urls, int num_urls) {
    printf("=== Demo Múltiples Peticiones ===\n");
    
    for (int i = 0; i < num_urls; i++) {
        printf("\n[%d/%d] %s\n", i + 1, num_urls, urls[i]);
        
        respuesta_http_t respuesta;
        int resultado = cliente_http_get_simple(urls[i], &respuesta);
        
        if (resultado == CLIENTE_HTTP_OK) {
            printf("✓ Código: %d, Tamaño: %zu bytes\n", 
                   respuesta.codigo_estado, respuesta.tamaño_contenido);
            cliente_http_liberar_respuesta(&respuesta);
        } else {
            printf("✗ Error: %s\n", cliente_http_error_string(resultado));
        }
    }
    
    return CLIENTE_HTTP_OK;
}

int cliente_http_benchmark(const char *url, int num_peticiones) {
    printf("=== Benchmark HTTP ===\n");
    printf("URL: %s\n", url);
    printf("Peticiones: %d\n\n", num_peticiones);
    
    double tiempo_inicio = cliente_http_timestamp();
    int exitosas = 0;
    int errores = 0;
    size_t bytes_totales = 0;
    
    for (int i = 0; i < num_peticiones; i++) {
        respuesta_http_t respuesta;
        int resultado = cliente_http_get_simple(url, &respuesta);
        
        if (resultado == CLIENTE_HTTP_OK) {
            exitosas++;
            bytes_totales += respuesta.tamaño_contenido;
            cliente_http_liberar_respuesta(&respuesta);
        } else {
            errores++;
        }
        
        if ((i + 1) % 10 == 0 || i == num_peticiones - 1) {
            printf("\rProgreso: %d/%d (%.1f%%)", i + 1, num_peticiones, 
                   (float)(i + 1) / num_peticiones * 100);
            fflush(stdout);
        }
    }
    
    double tiempo_total = cliente_http_timestamp() - tiempo_inicio;
    
    printf("\n\n=== Resultados ===\n");
    printf("Peticiones exitosas: %d\n", exitosas);
    printf("Errores: %d\n", errores);
    printf("Tiempo total: %.3f segundos\n", tiempo_total);
    printf("Peticiones por segundo: %.2f\n", num_peticiones / tiempo_total);
    
    char bytes_str[32];
    cliente_http_formatear_bytes(bytes_totales, bytes_str, sizeof(bytes_str));
    printf("Bytes totales transferidos: %s\n", bytes_str);
    
    return CLIENTE_HTTP_OK;
}
