# Ejercicio 092: Servidor TCP Multicliente

## Descripción

Este ejercicio implementa un servidor TCP que puede atender múltiples clientes simultáneamente usando POSIX threads (pthread). Cada cliente conectado es manejado por un hilo independiente, permitiendo conexiones concurrentes sin bloquear el hilo principal que acepta nuevas conexiones.

## Objetivos de Aprendizaje

- **Programación de redes**: Sockets TCP, bind, listen, accept
- **Concurrencia con threads**: pthread_create, pthread_detach
- **Arquitectura servidor multicliente**: Patrón thread-per-client
- **Gestión de recursos**: Prevención de memory leaks y resource leaks
- **Sincronización**: Manejo thread-safe de recursos compartidos
- **Manejo de errores**: Error handling robusto en entornos concurrentes

## Conceptos Clave

### 1. Servidor Multicliente
```c
// Bucle principal del servidor
while (1) {
    cliente_sock = accept(servidor_sock, ...);
    pthread_create(&hilo, NULL, atender_cliente, cliente_sock);
    pthread_detach(hilo);
}
```

### 2. Thread per Client
- Cada cliente obtiene su propio hilo de ejecución
- El hilo principal se dedica exclusivamente a aceptar conexiones
- Los hilos cliente manejan la comunicación independientemente

### 3. Gestión de Memoria Thread-Safe
- Cada hilo recibe su propio descriptor de socket
- Se usa malloc/free para evitar race conditions
- pthread_detach previene zombie threads

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make
```

### Compilación Manual
```bash
gcc -std=c11 -Wall -Wextra -O2 -pthread \
    src/servidor_tcp_multicliente.c src/main.c \
    -I include -o servidor_tcp_multicliente

# Herramientas de prueba
gcc -std=c11 -Wall -Wextra -O2 -pthread \
    tools/cliente_prueba.c -o cliente_prueba

gcc -std=c11 -Wall -Wextra -O2 -pthread \
    tools/benchmark_servidor.c -o benchmark_servidor
```

### Tests con Criterion
```bash
gcc -std=c11 -Wall -Wextra -pthread \
    src/servidor_tcp_multicliente.c tests/test_servidor_tcp_multicliente.c \
    -I include -lcriterion -o test_servidor

./test_servidor
```

## Uso

### Servidor Principal
```bash
./servidor_tcp_multicliente

# Menú interactivo:
# 1. Servidor básico (puerto 8080)
# 2. Servidor con configuración personalizada
# 3. Demo con múltiples clientes simulados
# 4. Benchmark de rendimiento
# 5. Prueba de estrés
```

### Cliente de Prueba
```bash
./cliente_prueba [servidor] [puerto]

# Ejemplo:
./cliente_prueba localhost 8080
```

### Benchmark
```bash
./benchmark_servidor [servidor] [puerto] [num_clientes] [mensajes_por_cliente]

# Ejemplo: 100 clientes, 50 mensajes cada uno
./benchmark_servidor localhost 8080 100 50
```

## Ejemplos de Uso

### 1. Servidor Básico
```bash
# Terminal 1 - Servidor
./servidor_tcp_multicliente
# Seleccionar opción 1

# Terminal 2 - Cliente 1
telnet localhost 8080
> Hola servidor
< Hola servidor

# Terminal 3 - Cliente 2
telnet localhost 8080
> Buenos días
< Buenos días
```

### 2. Prueba con Múltiples Clientes
```bash
# Usar el cliente de prueba en paralelo
for i in {1..10}; do
    ./cliente_prueba localhost 8080 &
done
wait
```

### 3. Benchmark de Rendimiento
```bash
# Probar con diferentes cargas
./benchmark_servidor localhost 8080 10 100    # 10 clientes, 100 msgs
./benchmark_servidor localhost 8080 50 50     # 50 clientes, 50 msgs
./benchmark_servidor localhost 8080 100 10    # 100 clientes, 10 msgs
```

## API Principales

### Funciones del Servidor
```c
// Crear y configurar servidor
int crear_servidor_tcp(int puerto);
int configurar_socket_servidor(int socket_fd);

// Atender clientes
void* atender_cliente(void* arg);
int manejar_cliente_eco(int cliente_sock);

// Gestión de conexiones
typedef struct {
    int socket_fd;
    struct sockaddr_in direccion;
    pthread_t hilo_id;
    time_t tiempo_conexion;
} info_cliente_t;

// Estadísticas y monitoreo
typedef struct {
    uint64_t total_conexiones;
    uint64_t conexiones_activas;
    uint64_t total_mensajes;
    uint64_t total_bytes;
    time_t tiempo_inicio;
} estadisticas_servidor_t;
```

### Configuración
```c
typedef struct {
    int puerto;
    int max_conexiones;
    int timeout_cliente;
    int buffer_size;
    bool log_activado;
    char directorio_log[256];
} config_servidor_t;
```

## Estructura del Proyecto

```
092-servidor-tcp-multicliente/
├── include/
│   ├── servidor_tcp_multicliente.h     # API principal
│   └── .gitkeep
├── src/
│   ├── servidor_tcp_multicliente.c     # Implementación
│   └── main.c                          # Programa principal
├── tests/
│   └── test_servidor_tcp_multicliente.c # Tests con Criterion
├── tools/
│   ├── cliente_prueba.c                # Cliente para pruebas
│   └── benchmark_servidor.c            # Herramienta de benchmark
├── CMakeLists.txt                      # Configuración build
├── README.md                           # Esta documentación
└── .gitignore                          # Archivos ignorados
```

## Casos de Prueba

### Tests Unitarios
- ✅ Creación y configuración de socket servidor
- ✅ Binding a puerto específico
- ✅ Manejo de múltiples conexiones concurrentes
- ✅ Eco de mensajes correcto
- ✅ Gestión de desconexiones
- ✅ Thread safety y ausencia de race conditions
- ✅ Prevención de memory/resource leaks

### Tests de Integración
- ✅ Servidor con múltiples clientes reales
- ✅ Pruebas de carga y estrés
- ✅ Reconexión de clientes
- ✅ Manejo de errores de red
- ✅ Estadísticas en tiempo real

### Tests de Rendimiento
- ✅ Throughput con diferentes cargas
- ✅ Latencia de respuesta
- ✅ Uso de memoria y threads
- ✅ Estabilidad bajo carga sostenida

## Arquitectura

### Patrón Thread-per-Client
```
Servidor Principal
    │
    ├─ Hilo Aceptador (main)
    │   ├─ accept() -> nuevo cliente
    │   ├─ pthread_create() -> nuevo hilo
    │   └─ pthread_detach()
    │
    ├─ Hilo Cliente 1
    │   ├─ recv() / send()
    │   └─ close() al desconectar
    │
    ├─ Hilo Cliente 2
    │   ├─ recv() / send()
    │   └─ close() al desconectar
    │
    └─ Hilo Cliente N...
```

### Ventajas
- ✅ Escalabilidad moderada (cientos de clientes)
- ✅ Aislamiento entre clientes
- ✅ Implementación simple y directa
- ✅ Manejo independiente de errores

### Limitaciones
- ⚠️ Un thread por cliente (overhead en memoria)
- ⚠️ Límite del SO en número de threads
- ⚠️ Context switching con muchos threads

## Optimizaciones Implementadas

1. **Gestión Eficiente de Memoria**
   - Malloc/free por cliente para evitar race conditions
   - pthread_detach para prevenir zombie threads

2. **Manejo Robusto de Errores**
   - Validación de todas las llamadas de sistema
   - Cleanup automático en caso de error

3. **Configuración Flexible**
   - Puerto, timeouts y buffers configurables
   - Logging opcional para debugging

4. **Monitoreo en Tiempo Real**
   - Estadísticas de conexiones y rendimiento
   - Información de threads activos

## Notas de Seguridad

- ⚠️ **Buffer Overflow**: Se valida el tamaño de mensajes
- ⚠️ **DoS Protection**: Límites en conexiones y timeouts
- ⚠️ **Resource Leaks**: Cleanup automático de sockets y memoria
- ⚠️ **Thread Safety**: Uso correcto de variables compartidas

## Mejoras Futuras

1. **Pool de Threads**: Usar thread pool en lugar de thread-per-client
2. **Multiplexing**: Implementar con select/poll/epoll
3. **Autenticación**: Agregar autenticación de clientes
4. **Protocolos**: Implementar protocolos más complejos que eco
5. **Load Balancing**: Distribución de carga entre workers

## Referencias

- Stevens, W. Richard. "Unix Network Programming, Volume 1"
- Kerrisk, Michael. "The Linux Programming Interface"
- POSIX Threads Programming: https://computing.llnl.gov/tutorials/pthreads/
- TCP/IP Sockets in C: Practical Guide for Programmers