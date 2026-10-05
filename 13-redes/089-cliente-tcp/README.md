# Ejercicio 089: Cliente TCP

## Descripción

Este ejercicio implementa un cliente TCP en C que se conecta a un servidor en localhost (127.0.0.1) utilizando un puerto conocido (8080). El cliente envía un mensaje de texto al servidor ("Hola, servidor") y muestra la respuesta recibida.

Este ejercicio introduce los fundamentos de la programación de sockets TCP en C, mostrando cómo establecer una conexión cliente-servidor, enviar datos y recibir una respuesta a través de la red local.

## Funcionalidades

### Cliente TCP Básico
- Conexión a servidor TCP en localhost:8080
- Envío de mensajes de texto
- Recepción y visualización de respuestas
- Manejo robusto de errores de red

### Características Avanzadas
- **Timeout configurable**: Evita bloqueos indefinidos
- **Reconexión automática**: Reintenta conexiones fallidas
- **Métricas de rendimiento**: Tiempo de respuesta, throughput
- **Modo interactivo**: Envío de múltiples mensajes
- **Configuración flexible**: Puerto, host, timeouts personalizables

### Herramientas Incluidas
- **Servidor de prueba**: Para testing local sin dependencias externas
- **Demos educativos**: Ejemplos paso a paso
- **Benchmarks**: Medición de rendimiento

## Uso

### 1. Ejecutar el Cliente Básico
```bash
./cliente_tcp
```

### 2. Usar el Servidor de Prueba
En una terminal:
```bash
./servidor_prueba
```

En otra terminal:
```bash
./cliente_tcp
```

### 3. Servidor de Prueba con Opciones
```bash
# Puerto personalizado
./servidor_prueba -p 9090

# Modo verbose
./servidor_prueba -v

# Respuesta personalizada
./servidor_prueba -r "¡Hola, cliente!"

# Ver ayuda
./servidor_prueba -h
```

### 4. Cliente Interactivo
El programa principal ofrece un menú interactivo:
```
=== CLIENTE TCP - EJERCICIO 089 ===
1. Demo básico
2. Demo interactivo
3. Demo reconexión automática
4. Demo manejo de errores
5. Configurar cliente
6. Benchmark de rendimiento
7. Test de estrés
8. Ayuda
9. Salir
```

### 5. Ejecutar Tests
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/cliente_tcp_main
```

## API Principal

### Tipos de Datos
```c
typedef struct {
    char host[256];
    int puerto;
    int timeout_segundos;
    int max_reintentos;
    int intervalo_reintento_ms;
} config_cliente_t;

typedef struct {
    int conexiones_exitosas;
    int conexiones_fallidas;
    int mensajes_enviados;
    int mensajes_recibidos;
    int bytes_enviados;
    int bytes_recibidos;
    double tiempo_total_ms;
} estadisticas_cliente_t;
```

### Funciones Principales
```c
// Gestión de clientes
cliente_tcp_t* crear_cliente(const config_cliente_t* config);
void destruir_cliente(cliente_tcp_t* cliente);

// Operaciones de red
resultado_t conectar_cliente(cliente_tcp_t* cliente);
resultado_t enviar_mensaje(cliente_tcp_t* cliente, const char* mensaje);
resultado_t recibir_respuesta(cliente_tcp_t* cliente, char* buffer, size_t tam_buffer);
void desconectar_cliente(cliente_tcp_t* cliente);

// Operaciones de alto nivel
resultado_t transaccion_completa(cliente_tcp_t* cliente, const char* mensaje, char* respuesta, size_t tam_respuesta);

// Utilidades
void obtener_estadisticas(const cliente_tcp_t* cliente, estadisticas_cliente_t* stats);
void resetear_estadisticas(cliente_tcp_t* cliente);
```

## Conceptos de Red Cubiertos

### Sockets BSD
- `socket()`: Creación de endpoint de comunicación
- `connect()`: Establecimiento de conexión TCP
- `send()`: Envío de datos
- `recv()`: Recepción de datos
- `close()`: Cierre de socket

### Protocolo TCP
- Protocolo orientado a conexión
- Garantía de entrega y orden
- Control de flujo y congestión
- Handshake de tres vías

### Direccionamiento de Red
- IPv4: 127.0.0.1 (localhost)
- Puertos: 8080 (HTTP alternativo)
- Estructura `sockaddr_in`
- Conversión de endianness (`htons`, `inet_addr`)

### Manejo de Errores
- Verificación de valores de retorno
- Uso de `perror()` para diagnósticos
- Limpieza de recursos en errores
- Timeouts para evitar bloqueos

## Ejemplos de Uso

### Cliente Básico
```c
// Configuración simple
config_cliente_t config = CONFIG_CLIENTE_DEFECTO;
cliente_tcp_t* cliente = crear_cliente(&config);

// Conexión y envío
if (conectar_cliente(cliente) == EXITO) {
    char respuesta[1024];
    if (transaccion_completa(cliente, "Hola, servidor", 
                           respuesta, sizeof(respuesta)) == EXITO) {
        printf("Respuesta: %s\n", respuesta);
    }
}

destruir_cliente(cliente);
```

### Cliente con Reconexión
```c
config_cliente_t config = {
    .host = "127.0.0.1",
    .puerto = 8080,
    .timeout_segundos = 5,
    .max_reintentos = 3,
    .intervalo_reintento_ms = 1000
};

cliente_tcp_t* cliente = crear_cliente(&config);
// El cliente reintentará automáticamente en caso de fallo
```

## Extensiones Sugeridas

1. **Soporte IPv6**: Usar `AF_INET6`
2. **Cifrado TLS**: Integrar OpenSSL
3. **Multiplexing**: Usar `select()` o `epoll()`
4. **Protocolo personalizado**: Headers, checksums
5. **Cliente asíncrono**: Threading o async I/O
6. **Pool de conexiones**: Reutilización de sockets
7. **Heartbeat**: Keep-alive automático
8. **Compression**: Comprimir datos antes de envío

## Referencias

### Documentación
- [POSIX Sockets API](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [TCP/IP Illustrated, Volume 1](https://www.amazon.com/TCP-Illustrated-Protocols-Addison-Wesley-Professional/dp/0321336313)
- [Unix Network Programming](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551)

### RFCs Relevantes
- [RFC 793: TCP](https://tools.ietf.org/html/rfc793)
- [RFC 1122: Internet Host Requirements](https://tools.ietf.org/html/rfc1122)
- [RFC 5681: TCP Congestion Control](https://tools.ietf.org/html/rfc5681)

### Herramientas
- `netstat`: Estado de conexiones de red
- `tcpdump`/`wireshark`: Análisis de tráfico
- `nc` (netcat): Testing de sockets
- `telnet`: Cliente TCP simple

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/cliente_tcp_main
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
