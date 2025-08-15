# Ejercicio 090: Servidor TCP de Eco

## Descripción

Este ejercicio implementa un servidor TCP de eco en C que escucha conexiones en un puerto específico (8080 por defecto). El servidor acepta conexiones entrantes usando `accept()`, lee mensajes enviados por clientes con `recv()`, y devuelve exactamente el mismo mensaje usando `send()` (protocolo de eco).

Este ejercicio introduce los conceptos fundamentales de la programación de servidores TCP en C, usando la API de sockets BSD (`socket()`, `bind()`, `listen()`, `accept()`) y refuerza la lógica de manejo de conexiones múltiples.

## Estructura del Proyecto

```
090-servidor-tcp/
├── CMakeLists.txt              # Configuración avanzada de compilación
├── README.md                   # Este archivo
├── .gitignore                  # Archivos a ignorar en Git
├── include/
│   ├── .gitkeep               # Marcador de directorio
│   └── servidor_tcp.h         # API completa del servidor TCP
├── src/
│   ├── servidor_tcp.c         # Implementación del servidor
│   └── main.c                 # Programa principal interactivo
├── tests/
│   └── test_servidor_tcp.c    # Tests exhaustivos con Criterion
└── tools/
    └── cliente_prueba.c       # Cliente de prueba y benchmark
```

## Funcionalidades

### Servidor TCP de Eco Básico
- Escucha en puerto configurable (8080 por defecto)
- Acepta conexiones de múltiples clientes
- Recibe mensajes y los devuelve intactos (eco)
- Manejo robusto de errores de red y timeouts

### Arquitecturas de Servidor
- **SECUENCIAL**: Un cliente a la vez (simple, educativo)
- **FORK**: Proceso por cliente (robusto, Unix tradicional)
- **THREAD**: Hilo por cliente (eficiente, moderno)
- **SELECT**: I/O multiplexing (escalable, evento-driven)

### Características Avanzadas
- **Configuración flexible**: Puerto, host, timeouts, buffers
- **Estadísticas en tiempo real**: Conexiones, mensajes, throughput
- **Manejo graceful de señales**: Shutdown limpio con Ctrl+C
- **Opciones de socket**: SO_REUSEADDR, SO_KEEPALIVE, TCP_NODELAY
- **Thread-safety**: Estadísticas protegidas con mutex
- **Gestión de memoria**: Sin leaks, limpieza automática

### Herramientas Incluidas
- **Cliente de prueba**: Para testing manual y automatizado
- **Stress testing**: Múltiples clientes concurrentes
- **Benchmarks**: Medición de rendimiento y throughput
- **Tests exhaustivos**: Cobertura completa con Criterion

## Compilación

### Requisitos
- CMake 3.10+
- GCC o Clang con soporte C99
- POSIX threads (pthread)
- Criterion (para tests)
- Sockets BSD (disponible en Unix/Linux/macOS)

### Compilación Básica
```bash
mkdir build && cd build
cmake ..
make
```

### Opciones de Compilación
```bash
# Debug con símbolos
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Con sanitizers para debugging
cmake -DENABLE_SANITIZERS=ON ..

# Solo tests (para desarrollo de tests)
cmake -DBUILD_TESTS_ONLY=ON ..

# Con cobertura de código
cmake -DENABLE_COVERAGE=ON ..

# Con documentación Doxygen
cmake -DBUILD_DOCS=ON ..
```

### Targets Disponibles
```bash
make servidor_tcp       # Servidor principal
make cliente_prueba     # Cliente de prueba
make test_servidor_tcp  # Compilar tests
make test              # Ejecutar tests (CTest)
make run_tests         # Ejecutar tests (verbose)
make docs              # Generar documentación
make info              # Información del sistema
make install           # Instalar en el sistema
```

## Uso

### 1. Servidor Básico (Modo Interactivo)
```bash
./servidor_tcp
```

El programa mostrará un menú interactivo:
```
=== SERVIDOR TCP DE ECO - EJERCICIO 090 ===
1. Servidor básico (modo secuencial)
2. Servidor con fork (proceso por cliente)
3. Servidor con threads (hilo por cliente)
4. Servidor con select (I/O multiplexing)
5. Configurar servidor personalizado
6. Ejecutar stress test
7. Demo de rendimiento
8. Información sobre TCP y sockets
9. Salir
```

### 2. Probar el Servidor

En una terminal, ejecutar el servidor:
```bash
./servidor_tcp
# Seleccionar opción 1 (servidor básico)
```

En otra terminal, conectar con telnet o netcat:
```bash
# Con telnet
telnet localhost 8080

# Con netcat
nc localhost 8080

# Con nuestro cliente de prueba
./cliente_prueba
```

### 3. Cliente de Prueba Avanzado
```bash
# Modo interactivo
./cliente_prueba

# Mensaje específico
./cliente_prueba -m "Hola, servidor TCP"

# Múltiples mensajes
./cliente_prueba -m "Test" -n 10

# Benchmark de rendimiento
./cliente_prueba -b -n 1000

# Puerto personalizado
./cliente_prueba -p 9090 -m "Test puerto 9090"

# Con timeouts y verbose
./cliente_prueba -t 10 -v -m "Test con detalles"

# Ver ayuda completa
./cliente_prueba --help
```

### 4. Configuración Personalizada

El servidor permite configuración detallada:
```bash
# En el menú interactivo, seleccionar opción 5
# Configurar puerto, max clientes, timeouts, modo, etc.
```

### 5. Tests y Validación
```bash
# Ejecutar todos los tests
make run_tests

# Ver información del sistema
make info

# Benchmark integrado
./servidor_tcp
# Seleccionar opción 7 (Demo de rendimiento)
```

## API Principal

### Tipos de Datos
```c
typedef enum {
    SERVIDOR_EXITO = 0,
    SERVIDOR_ERROR_SOCKET = -1,
    SERVIDOR_ERROR_BIND = -2,
    SERVIDOR_ERROR_LISTEN = -3,
    SERVIDOR_ERROR_ACCEPT = -4,
    // ... más códigos de error
} resultado_servidor_t;

typedef enum {
    MODO_SECUENCIAL,    // Un cliente a la vez
    MODO_FORK,          // Fork por cliente
    MODO_THREAD,        // Thread por cliente
    MODO_SELECT,        // I/O multiplexing
    MODO_POLL           // Poll multiplexing
} modo_servidor_t;

typedef struct {
    char host[256];
    int puerto;
    int backlog;
    int max_clientes;
    int timeout_cliente_seg;
    int buffer_size;
    modo_servidor_t modo;
    int reusar_puerto;
    int keep_alive;
    int no_delay;
    int verbose;
    int daemonizar;
} config_servidor_t;
```

### Funciones Principales
```c
// Gestión del servidor
servidor_tcp_t* crear_servidor(const config_servidor_t* config);
void destruir_servidor(servidor_tcp_t* servidor);
resultado_servidor_t iniciar_servidor(servidor_tcp_t* servidor);
resultado_servidor_t ejecutar_servidor(servidor_tcp_t* servidor);
void detener_servidor(servidor_tcp_t* servidor);

// Gestión de clientes
resultado_servidor_t aceptar_cliente(servidor_tcp_t* servidor, info_cliente_t* info_cliente);
resultado_servidor_t procesar_cliente_eco(servidor_tcp_t* servidor, info_cliente_t* cliente);
void desconectar_cliente(servidor_tcp_t* servidor, info_cliente_t* cliente);

// Modos de operación
resultado_servidor_t servidor_modo_secuencial(servidor_tcp_t* servidor);
resultado_servidor_t servidor_modo_fork(servidor_tcp_t* servidor);
resultado_servidor_t servidor_modo_thread(servidor_tcp_t* servidor);
resultado_servidor_t servidor_modo_select(servidor_tcp_t* servidor);

// Utilidades
void obtener_estadisticas_servidor(const servidor_tcp_t* servidor, estadisticas_servidor_t* stats);
void resetear_estadisticas_servidor(servidor_tcp_t* servidor);
const char* servidor_strerror(resultado_servidor_t codigo);
```

## Conceptos de Red Cubiertos

### Sockets de Servidor BSD
- `socket()`: Crear endpoint de comunicación
- `bind()`: Asociar socket con dirección y puerto local
- `listen()`: Poner socket en modo escucha
- `accept()`: Aceptar conexión entrante (blocking)
- `recv()`: Recibir datos del cliente
- `send()`: Enviar datos al cliente
- `close()`: Cerrar socket

### Protocolo TCP
- Protocolo orientado a conexión
- Handshake de tres vías (SYN, SYN-ACK, ACK)
- Garantía de entrega y orden
- Control de flujo y congestión
- Estado de conexión mantenido

### Arquitecturas de Servidor
- **Iterativo (Secuencial)**: Simple pero no escalable
- **Concurrente con Fork**: Robusto, aislamiento de procesos
- **Concurrente con Threads**: Eficiente, memoria compartida
- **I/O Multiplexing**: Escalable, event-driven

### Configuración de Sockets
- `SO_REUSEADDR`: Reutilizar dirección inmediatamente
- `SO_KEEPALIVE`: Detectar conexiones muertas
- `TCP_NODELAY`: Deshabilitar algoritmo de Nagle
- `SO_RCVTIMEO/SO_SNDTIMEO`: Timeouts de E/S

### Manejo de Errores y Señales
- Verificación de valores de retorno
- Uso de `perror()` para diagnósticos
- Manejo de `SIGINT`, `SIGTERM`, `SIGCHLD`
- Limpieza de recursos en errores
- Shutdown graceful

## Ejemplos de Uso

### Servidor Básico
```c
// Configuración simple
config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
servidor_tcp_t* servidor = crear_servidor(&config);

// Iniciar y ejecutar
if (iniciar_servidor(servidor) == SERVIDOR_EXITO) {
    printf("Servidor escuchando en puerto %d\n", config.puerto);
    ejecutar_servidor(servidor);
}

destruir_servidor(servidor);
```

### Servidor con Threads
```c
config_servidor_t config = {
    .puerto = 8080,
    .modo = MODO_THREAD,
    .max_clientes = 20,
    .timeout_cliente_seg = 30,
    .verbose = 1
};

servidor_tcp_t* servidor = crear_servidor(&config);
// El servidor manejará múltiples clientes concurrentemente
```

### Cliente Simple
```c
int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in servidor_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(8080),
    .sin_addr.s_addr = inet_addr("127.0.0.1")
};

connect(socket_fd, (struct sockaddr*)&servidor_addr, sizeof(servidor_addr));
send(socket_fd, "Hola servidor", 13, 0);

char respuesta[256];
recv(socket_fd, respuesta, sizeof(respuesta), 0);
printf("Eco: %s\n", respuesta);

close(socket_fd);
```

## Troubleshooting

### Problemas Comunes

#### "Address already in use"
```bash
# El puerto está ocupado
netstat -ln | grep 8080
# Esperar o usar SO_REUSEADDR (ya incluido)
# O cambiar puerto en la configuración
```

#### "Connection refused"
```bash
# Verificar que el servidor esté ejecutándose
ps aux | grep servidor_tcp
# Verificar que el puerto sea correcto
netstat -ln | grep 8080
```

#### Tests fallan
```bash
# Verificar Criterion
pkg-config --modversion criterion

# En macOS
brew install criterion

# En Ubuntu/Debian
sudo apt-get install libcriterion-dev

# En otras distribuciones
# Compilar desde fuente: https://github.com/Snaipe/Criterion
```

#### Problemas de rendimiento
- Aumentar `max_clientes` en configuración
- Usar modo `MODO_SELECT` o `MODO_THREAD`
- Ajustar `buffer_size` según necesidades
- Habilitar `TCP_NODELAY` para baja latencia
- Verificar límites del sistema (`ulimit -n`)

### Debug Avanzado
```bash
# Compilar con debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Con sanitizers
cmake -DENABLE_SANITIZERS=ON ..
make

# Usar gdb/lldb
gdb ./servidor_tcp
lldb ./servidor_tcp

# Monitorear tráfico de red
# macOS
sudo tcpdump -i lo0 port 8080

# Linux
sudo tcpdump -i lo port 8080

# Analizar con Wireshark
wireshark
```

### Logs y Métricas
```bash
# Habilitar modo verbose en configuración
# Las estadísticas se muestran automáticamente al detener el servidor
# Usar option 6 en el menú para stress testing
```

## Testing

### Tests Unitarios (Criterion)
- ✅ Creación y destrucción de servidores
- ✅ Configuración y validación de parámetros
- ✅ Inicialización de sockets y bind
- ✅ Gestión de clientes y conexiones
- ✅ Procesamiento de mensajes de eco
- ✅ Manejo de errores de red
- ✅ Estadísticas y métricas
- ✅ Thread safety y concurrencia
- ✅ Diferentes modos de servidor
- ✅ Tests parametrizados (tamaños de mensaje)

### Tests de Integración
- ✅ Servidor completo con clientes reales
- ✅ Múltiples clientes concurrentes
- ✅ Diferentes arquitecturas (fork, thread, select)
- ✅ Stress testing con carga real
- ✅ Benchmarks de rendimiento

### Cobertura
```bash
# Habilitar cobertura
cmake -DENABLE_COVERAGE=ON ..
make run_tests

# Generar reporte
gcov *.gcda
# o usar lcov para reporte HTML
```

## Extensiones Sugeridas

1. **Protocolo personalizado**: Headers, checksums, compresión
2. **Servidor HTTP básico**: Implementar HTTP/1.1 sobre TCP
3. **SSL/TLS**: Cifrado con OpenSSL
4. **IPv6**: Soporte dual stack IPv4/IPv6
5. **Configuración por archivo**: JSON/YAML config
6. **Logging avanzado**: Rotación, niveles, syslog
7. **Métricas avanzadas**: Prometheus, Grafana
8. **Load balancing**: Múltiples servidores
9. **Connection pooling**: Reutilización de conexiones
10. **Modo reactor**: Async I/O con epoll/kqueue

## Comparación con Cliente TCP (Ejercicio 089)

| Aspecto | Cliente (089) | Servidor (090) |
|---------|---------------|----------------|
| **Función principal** | `connect()` | `bind()` + `listen()` + `accept()` |
| **Iniciativa** | Inicia conexión | Espera conexiones |
| **Complejidad** | Simple, lineal | Compleja, concurrente |
| **Recursos** | Un socket | Múltiples sockets |
| **Estado** | Temporal | Persistente |
| **Escalabilidad** | N/A | Crítica |

## Referencias

### Documentación Técnica
- [POSIX Sockets API](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [TCP/IP Illustrated, Volume 1](https://www.amazon.com/TCP-Illustrated-Protocols-Addison-Wesley-Professional/dp/0321336313)
- [Unix Network Programming, Volume 1](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551)
- [Advanced Programming in the UNIX Environment](https://www.amazon.com/Advanced-Programming-UNIX-Environment-3rd/dp/0321637739)

### RFCs Relevantes
- [RFC 793: Transmission Control Protocol](https://tools.ietf.org/html/rfc793)
- [RFC 1122: Requirements for Internet Hosts](https://tools.ietf.org/html/rfc1122)
- [RFC 5681: TCP Congestion Control](https://tools.ietf.org/html/rfc5681)
- [RFC 6335: Internet Assigned Numbers Authority](https://tools.ietf.org/html/rfc6335)

### Herramientas de Red
- `netstat`: Estado de conexiones y puertos
- `ss`: Estadísticas de sockets (moderno)
- `tcpdump`: Captura de tráfico de red
- `wireshark`: Análisis detallado de protocolos
- `nc` (netcat): Cliente/servidor TCP/UDP versátil
- `telnet`: Cliente TCP simple
- `nmap`: Escaneo de puertos y servicios

### Arquitecturas de Servidor
- [The C10K Problem](http://www.kegel.com/c10k.html)
- [Reactor Pattern](https://en.wikipedia.org/wiki/Reactor_pattern)
- [Proactor Pattern](https://en.wikipedia.org/wiki/Proactor_pattern)
- [Event-driven architecture](https://en.wikipedia.org/wiki/Event-driven_architecture)
