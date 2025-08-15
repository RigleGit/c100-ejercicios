# Ejercicio 091: Comunicación UDP

## Descripción

Implementación completa de comunicación UDP en C, incluyendo emisor y receptor con funcionalidades avanzadas. Este ejercicio demuestra el uso de sockets UDP para comunicación sin conexión, manejo de errores robusto, configuración avanzada y herramientas de análisis de rendimiento.

## Características

### 🚀 Funcionalidades Principales
- **Emisor UDP**: Cliente que envía datagramas a servidores remotos
- **Receptor UDP**: Servidor que recibe y procesa datagramas
- **Configuración avanzada**: Timeouts, buffers, opciones de socket
- **Estadísticas detalladas**: Métricas de rendimiento y diagnósticos
- **Demos interactivos**: Ejemplos de eco, broadcast y multicast

### 🛠️ Características Técnicas
- Sockets UDP con `SOCK_DGRAM`
- Manejo robusto de errores y timeouts
- Configuración flexible de buffers y opciones
- Estadísticas en tiempo real
- Soporte para IPv4
- Validación de entrada y configuración

### 📊 Herramientas Incluidas
- `emisor_udp`: Cliente UDP con opciones avanzadas
- `receptor_udp`: Servidor UDP con modo eco y estadísticas
- Programa de demostración interactivo
- Suite de tests exhaustiva con Criterion

## Estructura del Proyecto

```
091-comunicacion-udp/
├── include/
│   └── comunicacion_udp.h      # API completa para UDP
├── src/
│   ├── comunicacion_udp.c      # Implementación principal
│   └── main.c                  # Programa de demostración
├── tests/
│   └── test_comunicacion_udp.c # Suite de tests con Criterion
├── tools/
│   ├── emisor_udp.c           # Cliente UDP standalone
│   └── receptor_udp.c         # Servidor UDP standalone
├── CMakeLists.txt             # Build system avanzado
├── README.md                  # Esta documentación
└── .gitignore                # Archivos a ignorar en git
```

## Compilación

### Requisitos Previos
- **Compilador C99** (GCC, Clang)
- **CMake 3.10+**
- **Criterion** (para tests): `brew install criterion` (macOS) o `apt install libcriterion-dev` (Ubuntu)

### Build Rápido
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Opciones de Build
```bash
# Build con todas las características
cmake -DBUILD_TESTS=ON -DBUILD_TOOLS=ON -DBUILD_DOCS=ON ..

# Build optimizado para producción
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ..

# Build con sanitizers para debugging
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON ..

# Build con cobertura de código
cmake -DENABLE_COVERAGE=ON ..
```

## Uso

### 1. Programa Principal (Demostración Interactiva)
```bash
./main_comunicacion_udp
```
Presenta un menú interactivo con todas las funcionalidades disponibles.

### 2. Emisor UDP
```bash
# Uso básico
./emisor_udp 127.0.0.1 9090 "Hola UDP"

# Múltiples mensajes con intervalo
./emisor_udp -n 5 -i 1000 127.0.0.1 9090 "Mensaje"

# Modo verboso con timeout personalizado
./emisor_udp -v -t 10 192.168.1.100 8080 "Datos importantes"

# Ayuda completa
./emisor_udp -h
```

### 3. Receptor UDP
```bash
# Receptor básico
./receptor_udp 9090

# Receptor con eco y modo verboso
./receptor_udp -e -v 9090

# Receptor con estadísticas cada 10 mensajes
./receptor_udp -s -t 60 9090

# Ayuda completa
./receptor_udp -h
```

### 4. Demo Eco Completo
```bash
# Terminal 1: Iniciar receptor con eco
./receptor_udp -e -v 9090

# Terminal 2: Enviar mensajes
./emisor_udp -n 3 -i 2000 127.0.0.1 9090 "Mensaje de prueba"
```

## Ejemplos de Código

### Configuración Básica
```c
#include "comunicacion_udp.h"

// Configurar emisor
ConfigUDP config = {0};
configurar_udp_por_defecto(&config);
strcpy(config.direccion_ip, "192.168.1.100");
config.puerto = 8080;

int sockfd = crear_socket_udp(&config);
if (sockfd < 0) {
    perror("Error creando socket");
    return -1;
}
```

### Envío de Datagrama
```c
const char* mensaje = "Hola servidor UDP";
ssize_t bytes_enviados = enviar_datagrama(sockfd, &config, 
                                         mensaje, strlen(mensaje));
if (bytes_enviados > 0) {
    printf("Enviado: %zd bytes\n", bytes_enviados);
}
```

### Configuración de Receptor
```c
ConfigUDP config = {0};
config.puerto = 9090;
config.timeout_segundos = 30;

int sockfd = crear_socket_udp(&config);
if (configurar_receptor(sockfd, &config) == 0) {
    printf("Receptor activo en puerto %d\n", config.puerto);
}
```

### Recepción con Estadísticas
```c
char buffer[1024];
EstadisticasUDP stats = {0};
inicializar_estadisticas(&stats);

while (activo) {
    ssize_t bytes = recibir_datagrama(sockfd, &config, buffer, 
                                     sizeof(buffer) - 1, &stats);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("Recibido: %s\n", buffer);
    }
}

mostrar_estadisticas(&stats);
```

## Tests

### Ejecutar Tests
```bash
# Todos los tests
make test

# Tests específicos con output detallado
./test_comunicacion_udp --verbose

# Tests en modo single-thread
./test_comunicacion_udp --jobs=1
```

### Cobertura de Tests
```bash
# Build con cobertura
cmake -DENABLE_COVERAGE=ON ..
make
make test

# Generar reporte de cobertura
gcov comunicacion_udp.c
```

### Categorías de Tests
- **Configuración**: Validación de parámetros y configuración
- **Socket**: Creación y configuración de sockets UDP
- **Comunicación**: Envío y recepción de datagramas
- **Estadísticas**: Métricas y análisis de rendimiento
- **Edge Cases**: Casos límite y manejo de errores
- **Rendimiento**: Tests de carga y velocidad

## Análisis de Rendimiento

### Benchmark Básico
```bash
# Terminal 1: Receptor con estadísticas
./receptor_udp -s 9090

# Terminal 2: Emisor con múltiples mensajes
./emisor_udp -n 1000 -i 1 127.0.0.1 9090 "Benchmark"
```

### Métricas Disponibles
- **Throughput**: Bytes/segundo y mensajes/segundo
- **Latencia**: Tiempo de envío y recepción
- **Pérdida**: Porcentaje de mensajes perdidos
- **Errores**: Conteo de errores de red

### Configuración de Rendimiento
```c
// Optimizar buffer para alta velocidad
config.buffer_size = 8192;

// Reducir timeout para detección rápida
config.timeout_segundos = 1;

// Habilitar reutilización de dirección
config.reutilizar_direccion = 1;
```

## Características Avanzadas

### 1. Broadcast UDP
```c
// Habilitar broadcast
config.habilitar_broadcast = 1;
strcpy(config.direccion_ip, "255.255.255.255");

// Enviar a todos los hosts de la red
enviar_datagrama(sockfd, &config, mensaje, strlen(mensaje));
```

### 2. Configuración de Timeouts
```c
// Timeout de socket
config.timeout_segundos = 10;

// Timeout personalizado con select()
struct timeval timeout = {.tv_sec = 5, .tv_usec = 0};
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
```

### 3. Estadísticas Detalladas
```c
EstadisticasUDP stats = {0};
// ... operaciones de red ...

printf("Mensajes enviados: %zu\n", stats.mensajes_enviados);
printf("Bytes totales: %zu\n", stats.bytes_enviados);
printf("Errores: %zu\n", stats.errores_envio);
```

## Troubleshooting

### Problemas Comunes

1. **Puerto en uso**
```bash
# Error: Address already in use
# Solución: Usar puerto diferente o SO_REUSEADDR
./receptor_udp 9091  # Puerto alternativo
```

2. **Timeout en recepción**
```bash
# Error: Timeout
# Solución: Verificar que el emisor esté enviando al puerto correcto
./emisor_udp -v 127.0.0.1 9090 "test"  # Modo verboso para debug
```

3. **Permisos de bind**
```bash
# Error: Permission denied (puertos < 1024)
# Solución: Usar puertos > 1024 o ejecutar como root
./receptor_udp 8080  # Puerto > 1024
```

### Debug y Logging
```bash
# Ejecutar con strace para ver system calls
strace -e network ./emisor_udp 127.0.0.1 9090 "debug"

# Usar netstat para verificar puertos
netstat -unl | grep 9090

# Wireshark para capturar tráfico UDP
tcpdump -i lo udp port 9090
```

## Recursos Adicionales

### Referencias
- [RFC 768 - User Datagram Protocol](https://tools.ietf.org/html/rfc768)
- [Linux Socket Programming](https://man7.org/linux/man-pages/man7/socket.7.html)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)

### Ejercicios Relacionados
- [089-cliente-tcp](../089-cliente-tcp/) - Cliente TCP
- [090-servidor-tcp](../090-servidor-tcp/) - Servidor TCP  
- [092-servidor-http](../092-servidor-http/) - Servidor HTTP básico
