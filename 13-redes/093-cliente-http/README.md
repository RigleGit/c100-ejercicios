# Ejercicio 093: Cliente HTTP

## Descripción

Este ejercicio implementa un cliente HTTP básico usando sockets TCP de bajo nivel. El cliente puede realizar peticiones GET a servidores web, parsear respuestas HTTP y mostrar estadísticas de rendimiento. Es una excelente introducción a la programación de redes y el protocolo HTTP.

## Objetivos de Aprendizaje

- **Protocolo HTTP**: Comprensión del protocolo HTTP/1.0 y HTTP/1.1
- **Sockets TCP**: Uso de getaddrinfo, connect, send, recv
- **Parsing de URLs**: Descomposición de URLs en sus componentes
- **Gestión de buffers**: Manejo eficiente de datos de red
- **Análisis de respuestas**: Parsing de headers y contenido HTTP
- **Estadísticas de red**: Medición de tiempos y throughput

## Conceptos Clave

### 1. Petición HTTP Básica
```c
// Estructura de una petición HTTP GET
GET /path HTTP/1.0\r\n
Host: example.com\r\n
User-Agent: ClienteHTTP/1.0\r\n
Connection: close\r\n
\r\n
```

### 2. Parsing de URL
```c
// URL: https://api.example.com:8080/data?param=value#section
url_parseada_t parseada;
cliente_http_parsear_url(url, &parseada);
// parseada.esquema = "https"
// parseada.host = "api.example.com"
// parseada.puerto = "8080"
// parseada.path = "/data"
// parseada.query = "param=value"
```

### 3. Respuesta HTTP
```c
// Estructura típica de respuesta
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 1234\r\n
Server: Apache/2.4\r\n
\r\n
<html>...</html>
```

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make
```

### Compilación Manual
```bash
gcc -std=c11 -Wall -Wextra -O2 \
    src/cliente_http.c src/main.c \
    -I include -o cliente_http

# Herramientas
gcc -std=c11 -Wall -Wextra -O2 \
    src/cliente_http.c tools/simple_http_client.c \
    -I include -o simple_http_client

gcc -std=c11 -Wall -Wextra -O2 \
    src/cliente_http.c tools/http_benchmark.c \
    -I include -o http_benchmark
```

### Tests con Criterion
```bash
gcc -std=c11 -Wall -Wextra \
    src/cliente_http.c tests/test_cliente_http.c \
    -I include -lcriterion -pthread -o test_cliente_http

./test_cliente_http
```

## Uso

### Cliente Principal (Interactivo)
```bash
./cliente_http

# Menú interactivo:
# 1. Petición GET básica
# 2. Petición con estadísticas
# 3. Solo headers (HEAD)
# 4. Múltiples peticiones
# 5. Benchmark de rendimiento
# 6. URLs predefinidas
# 7. Analizar URL
# 8. Configuración personalizada
```

### Cliente Simple (Línea de Comandos)
```bash
./simple_http_client http://example.com
./simple_http_client http://httpbin.org/get
./simple_http_client https://api.github.com
```

### Benchmark HTTP
```bash
./http_benchmark http://example.com -n 50
./http_benchmark http://httpbin.org/get -n 100 -t 10 -v

# Opciones:
# -n <número>   Número de peticiones (default: 10)
# -t <segundos> Timeout por petición (default: 30)
# -v            Modo verbose
# -h            Ayuda
```

## Ejemplos de Uso

### 1. Petición Básica
```bash
# Con el cliente interactivo
./cliente_http
# Seleccionar opción 1
# Introducir: http://example.com
```

### 2. Análisis de URL
```bash
# Con el cliente interactivo
./cliente_http
# Seleccionar opción 7
# Introducir: https://api.example.com:8080/v1/data?limit=10#results
```

### 3. Benchmark de Rendimiento
```bash
# Con herramienta de benchmark
./http_benchmark http://httpbin.org/get -n 50

# Resultados:
# - Total de peticiones: 50
# - Peticiones exitosas: 50 (100.0%)
# - Tiempo total: 2.543 segundos
# - Peticiones por segundo: 19.66 req/s
# - Tiempo promedio: 50.86 ms
```

### 4. Petición con Headers Personalizados
```bash
# Con configuración personalizada
./cliente_http
# Seleccionar opción 8
# URL: http://httpbin.org/headers
# User-Agent: MiCliente/2.0
# Headers: Accept: application/json\r\n
```

## API Principales

### Funciones de Alto Nivel
```c
// Petición GET simple
int cliente_http_get_simple(const char *url, respuesta_http_t *respuesta);

// Petición con configuración completa
int cliente_http_realizar_peticion(const peticion_http_t *peticion, 
                                   respuesta_http_t *respuesta,
                                   estadisticas_http_t *estadisticas);
```

### Funciones de Parsing
```c
// Parsear URL en componentes
int cliente_http_parsear_url(const char *url, url_parseada_t *parseada);

// Construir petición HTTP
ssize_t cliente_http_construir_peticion(const peticion_http_t *peticion, 
                                        char *buffer, size_t tamaño_buffer);

// Parsear headers de respuesta
int cliente_http_parsear_headers(const char *headers_raw, respuesta_http_t *respuesta);
```

### Funciones de Red
```c
// Conectar a servidor
int cliente_http_conectar(const char *host, const char *puerto, 
                          int timeout, char *ip_servidor);

// Enviar petición
ssize_t cliente_http_enviar_peticion(int socket, const peticion_http_t *peticion);

// Recibir respuesta
int cliente_http_recibir_respuesta(int socket, respuesta_http_t *respuesta, int timeout);
```

## Estructuras de Datos

### Petición HTTP
```c
typedef struct {
    metodo_http_t metodo;           // GET, POST, HEAD, etc.
    version_http_t version;         // HTTP/1.0, HTTP/1.1
    char url[MAX_URL_LENGTH];       // URL completa
    char host[MAX_HOST_LENGTH];     // Hostname
    char puerto[8];                 // Puerto
    char path[MAX_PATH_LENGTH];     // Path y query
    char user_agent[256];           // User-Agent header
    char headers_extra[MAX_HEADERS_LENGTH]; // Headers adicionales
    int timeout;                    // Timeout en segundos
} peticion_http_t;
```

### Respuesta HTTP
```c
typedef struct {
    int codigo_estado;              // 200, 404, 500, etc.
    char razon_estado[256];         // "OK", "Not Found", etc.
    version_http_t version;         // Versión HTTP
    char headers[MAX_HEADERS_LENGTH]; // Headers completos
    char *contenido;                // Contenido de la respuesta
    size_t tamaño_contenido;        // Tamaño del contenido
    double tiempo_respuesta;        // Tiempo de respuesta
    size_t bytes_recibidos;         // Bytes totales recibidos
} respuesta_http_t;
```

### Estadísticas
```c
typedef struct {
    double tiempo_dns;              // Tiempo resolución DNS
    double tiempo_conexion;         // Tiempo de conexión
    double tiempo_envio;            // Tiempo de envío
    double tiempo_recepcion;        // Tiempo de recepción
    double tiempo_total;            // Tiempo total
    size_t bytes_enviados;          // Bytes enviados
    size_t bytes_recibidos;         // Bytes recibidos
    char ip_servidor[INET6_ADDRSTRLEN]; // IP del servidor
} estadisticas_http_t;
```

## Estructura del Proyecto

```
093-cliente-http/
├── include/
│   ├── cliente_http.h                  # API principal
│   └── .gitkeep
├── src/
│   ├── cliente_http.c                  # Implementación
│   └── main.c                          # Programa principal
├── tests/
│   └── test_cliente_http.c             # Tests con Criterion
├── tools/
│   ├── simple_http_client.c            # Cliente simple CLI
│   └── http_benchmark.c                # Herramienta de benchmark
├── CMakeLists.txt                      # Configuración build
├── README.md                           # Esta documentación
└── .gitignore                          # Archivos ignorados
```

## Casos de Prueba

### Tests Unitarios
- ✅ Parsing correcto de URLs (básicas, con puerto, HTTPS, con query)
- ✅ Construcción de peticiones HTTP válidas
- ✅ Parsing de headers de respuesta
- ✅ Validación de URLs
- ✅ Funciones de utilidad (formateo, conversiones)

### Tests de Integración
- ✅ Peticiones GET contra servidor mock
- ✅ Medición de estadísticas
- ✅ Manejo de errores de red
- ✅ Timeouts y conexiones fallidas

### Tests de Rendimiento
- ✅ Múltiples peticiones secuenciales
- ✅ Benchmark de tiempo de respuesta
- ✅ Gestión de memoria sin leaks

## URLs de Prueba Recomendadas

### Servicios de Testing
```bash
# httpbin.org - Excelente para pruebas
http://httpbin.org/get                  # Información de la petición
http://httpbin.org/headers              # Headers enviados
http://httpbin.org/user-agent           # User-Agent específico
http://httpbin.org/status/200           # Código de estado específico
http://httpbin.org/delay/2              # Respuesta con delay

# example.com - Simple y estable
http://example.com                      # Página HTML simple

# Otros servicios útiles
http://httpforever.com                  # Siempre devuelve 200
http://httpstat.us/200                  # Códigos de estado específicos
```

### APIs Públicas (Solo GET)
```bash
# GitHub API
https://api.github.com/users/octocat

# JSONPlaceholder
https://jsonplaceholder.typicode.com/posts/1

# Cat Facts (requiere HTTPS - no soportado en esta versión)
# https://catfact.ninja/fact
```

## Características del Protocolo HTTP

### Métodos Soportados
- **GET**: Obtener recurso (implementado completamente)
- **HEAD**: Solo headers (implementado)
- **POST**: Enviar datos (preparado, no completamente implementado)
- **PUT**: Actualizar recurso (preparado)
- **DELETE**: Eliminar recurso (preparado)

### Versiones HTTP
- **HTTP/1.0**: Conexión simple, se cierra después de cada petición
- **HTTP/1.1**: Soporte básico (sin keep-alive en esta implementación)

### Headers Importantes
- **Host**: Requerido, especifica el servidor
- **User-Agent**: Identifica el cliente
- **Accept**: Tipos de contenido aceptados
- **Content-Length**: Tamaño del contenido
- **Connection**: Manejo de la conexión

## Limitaciones Actuales

1. **No HTTPS**: Solo HTTP (puerto 80)
2. **No Keep-Alive**: Cada petición abre nueva conexión
3. **No Compression**: No soporta gzip/deflate
4. **No Redirecciones**: No sigue redirects automáticamente
5. **No Autenticación**: No soporta Basic/Digest auth
6. **No Proxy**: No soporta proxies HTTP

## Mejoras Futuras

1. **Soporte HTTPS**: Integración con OpenSSL/TLS
2. **HTTP/2**: Soporte para protocolo más moderno
3. **Keep-Alive**: Reutilización de conexiones
4. **Redirecciones**: Seguimiento automático de redirects
5. **Compression**: Soporte para content-encoding
6. **Async I/O**: Operaciones no bloqueantes
7. **WebSockets**: Upgrade de protocolo

## Códigos de Error

```c
typedef enum {
    CLIENTE_HTTP_OK = 0,                    // Éxito
    CLIENTE_HTTP_ERROR_MEMORIA = -1,        // Error de memoria
    CLIENTE_HTTP_ERROR_RED = -2,            // Error de red genérico
    CLIENTE_HTTP_ERROR_DNS = -3,            // Error resolución DNS
    CLIENTE_HTTP_ERROR_CONEXION = -4,       // Error de conexión
    CLIENTE_HTTP_ERROR_ENVIO = -5,          // Error al enviar
    CLIENTE_HTTP_ERROR_RECEPCION = -6,      // Error al recibir
    CLIENTE_HTTP_ERROR_TIMEOUT = -7,        // Timeout
    CLIENTE_HTTP_ERROR_URL_INVALIDA = -8,   // URL mal formada
    CLIENTE_HTTP_ERROR_RESPUESTA_INVALIDA = -9, // Respuesta inválida
    CLIENTE_HTTP_ERROR_PARAMETRO = -10      // Parámetro inválido
} codigo_error_http_t;
```

## Debugging y Troubleshooting

### Compilación con Debug
```bash
gcc -DDEBUG -g -O0 -Wall -Wextra \
    src/cliente_http.c src/main.c \
    -I include -o cliente_http_debug
```

### Herramientas Útiles
```bash
# Verificar conectividad
ping example.com
telnet example.com 80

# Monitorear tráfico de red
sudo tcpdump -i any port 80

# Verificar con curl
curl -v http://example.com

# Servidor local para pruebas
python3 -m http.server 8000
```

### Errores Comunes
- **DNS Error**: Verificar conectividad a internet
- **Connection Error**: Puerto cerrado o firewall
- **Timeout**: Servidor lento o sobrecargado
- **Invalid Response**: Servidor no HTTP o respuesta malformada

## Referencias

- **RFC 7230**: HTTP/1.1 Message Syntax and Routing
- **RFC 7231**: HTTP/1.1 Semantics and Content
- **Stevens, W.R.**: "TCP/IP Illustrated, Volume 1"
- **MDN HTTP**: https://developer.mozilla.org/en-US/docs/Web/HTTP
- **httpbin.org**: https://httpbin.org/ (servicio de testing)
