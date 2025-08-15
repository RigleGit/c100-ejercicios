# Ejercicio 088: Patrón Productor-Consumidor

## 📋 Descripción

Este ejercicio implementa el clásico **patrón Productor-Consumidor** utilizando hilos POSIX (pthread) en C. El patrón demuestra cómo coordinar múltiples hilos que producen y consumen datos de manera concurrente utilizando un buffer circular compartido con sincronización adecuada.

### 🎯 Objetivos de Aprendizaje

- **Programación Concurrente**: Entender los conceptos fundamentales de sincronización entre hilos
- **Mutex y Variables de Condición**: Usar `pthread_mutex_t` y `pthread_cond_t` para coordinación
- **Buffer Circular**: Implementar una estructura de datos eficiente para el intercambio de datos
- **Patrón Productor-Consumidor**: Comprender uno de los patrones más importantes en programación concurrente
- **Prevención de Race Conditions**: Evitar condiciones de carrera y problemas de sincronización
- **Gestión de Recursos**: Manejo adecuado de memoria y recursos del sistema

## 🏗️ Arquitectura del Proyecto

```
088-productor-consumidor/
├── include/
│   └── productor_consumidor.h     # Definiciones y API principal
├── src/
│   ├── productor_consumidor.c     # Implementación del patrón
│   └── main.c                     # Programa principal interactivo
├── tests/
│   ├── test_productor_consumidor.c          # Tests exhaustivos
│   └── benchmark_productor_consumidor.c     # Benchmarks de rendimiento
├── CMakeLists.txt                 # Sistema de compilación avanzado
├── README.md                      # Esta documentación
├── .gitignore                     # Archivos a ignorar en git
└── include/
    └── .gitkeep                   # Mantener directorio include/
```

## 🔧 Componentes Principales

### 1. Buffer Circular (`buffer_circular_t`)

Estructura de datos thread-safe que implementa un buffer circular con las siguientes características:

```c
typedef struct {
    int *items;                    // Array de items
    int tamaño;                    // Capacidad máxima del buffer
    int frente;                    // Índice del primer elemento
    int final;                     // Índice para la próxima inserción
    int ocupados;                  // Número de elementos actuales
    pthread_mutex_t mutex;         // Mutex para exclusión mutua
    pthread_cond_t no_lleno;       // Condición: buffer no está lleno
    pthread_cond_t no_vacio;       // Condición: buffer no está vacío
} buffer_circular_t;
```

### 2. Operaciones Fundamentales

#### Operaciones Básicas (No Bloqueantes)
- `insertar_item()`: Inserta un item si hay espacio
- `extraer_item()`: Extrae un item si existe
- `esta_vacio()`: Verifica si el buffer está vacío
- `esta_lleno()`: Verifica si el buffer está lleno

#### Operaciones de Hilos (Bloqueantes)
- `producir_item()`: Produce un item, espera si el buffer está lleno
- `consumir_item()`: Consume un item, espera si el buffer está vacío

### 3. Funciones de Demostración

- `demo_basica()`: Demostración simple con un productor y un consumidor
- `demo_multiples_hilos()`: Múltiples productores y consumidores
- `demo_rendimiento()`: Análisis de throughput y latencia
- `experimento_personalizado()`: Configuración interactiva de parámetros

## 💡 Conceptos Clave

### Sincronización con Mutex

```c
pthread_mutex_lock(&buffer->mutex);
// Sección crítica - acceso exclusivo al buffer
if (buffer->ocupados < buffer->tamaño) {
    buffer->items[buffer->final] = item;
    buffer->final = (buffer->final + 1) % buffer->tamaño;
    buffer->ocupados++;
}
pthread_mutex_unlock(&buffer->mutex);
```

### Variables de Condición

```c
// Esperar hasta que el buffer no esté lleno
while (buffer->ocupados == buffer->tamaño) {
    pthread_cond_wait(&buffer->no_lleno, &buffer->mutex);
}

// Señalar que el buffer ya no está vacío
pthread_cond_signal(&buffer->no_vacio);
```

### Buffer Circular

El buffer circular permite un uso eficiente del espacio:
- Los índices se "envuelven" usando el operador módulo (`%`)
- No hay necesidad de mover elementos
- Operaciones de inserción y extracción en O(1)

## 🚀 Compilación y Ejecución

### Requisitos del Sistema

- **Compilador**: GCC 7+ o Clang 10+
- **Estándar**: C11
- **Threading**: POSIX Threads (pthread)
- **Sistema**: Linux, macOS, o Windows con WSL/MinGW
- **Tests**: Criterion framework (opcional)
- **Build System**: CMake 3.16+

### Compilación Básica

```bash
# Crear directorio de compilación
mkdir build && cd build

# Configurar con CMake
cmake ..

# Compilar
make

# Ejecutar programa principal
./bin/productor_consumidor_main
```

### Opciones de Compilación Avanzadas

```bash
# Configuración con opciones específicas
cmake -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_TESTS=ON \
      -DBUILD_BENCHMARKS=ON \
      -DENABLE_SANITIZERS=ON \
      ..

# Compilar todos los targets
make all

# Ejecutar tests
make run-tests

# Ejecutar benchmarks
make run-benchmarks

# Ver información de configuración
make info
```

### Targets Disponibles

| Target | Descripción |
|--------|-------------|
| `productor_consumidor_main` | Programa principal |
| `run-main` | Ejecutar programa principal |
| `test_productor_consumidor` | Tests unitarios |
| `run-tests` | Ejecutar todos los tests |
| `benchmark_productor_consumidor` | Benchmarks de rendimiento |
| `run-benchmarks` | Ejecutar benchmarks |
| `info` | Información de configuración |
| `clean-all` | Limpiar archivos generados |

## 📊 Uso del Programa

### Programa Principal Interactivo

El programa principal ofrece un menú interactivo para explorar diferentes aspectos del patrón:

```
=== PATRÓN PRODUCTOR-CONSUMIDOR ===
Implementación educativa con pthread

Seleccione una opción:
1. Demostración básica
2. Múltiples productores y consumidores
3. Análisis de rendimiento
4. Experimento personalizado
5. Ayuda y conceptos
0. Salir

Opción: _
```

### Ejemplos de Uso

#### 1. Demostración Básica
- Un productor que genera números secuenciales
- Un consumidor que procesa los números
- Buffer pequeño para mostrar sincronización

#### 2. Múltiples Hilos
- Configuración con varios productores y consumidores
- Demostración de escalabilidad
- Análisis de contención y throughput

#### 3. Análisis de Rendimiento
- Medición de latencia y throughput
- Comparación con diferentes tamaños de buffer
- Evaluación bajo diferentes cargas de trabajo

#### 4. Experimento Personalizado
- Configuración interactiva de parámetros:
  - Número de productores y consumidores
  - Tamaño del buffer
  - Número de items a procesar
  - Velocidad de producción/consumo

## 🧪 Tests y Validación

### Suite de Tests Exhaustiva

Los tests cubren múltiples aspectos del sistema:

#### Tests Funcionales
- ✅ Creación e inicialización del buffer
- ✅ Operaciones básicas (insertar/extraer)
- ✅ Comportamiento cuando está lleno/vacío
- ✅ Funcionalidad circular del buffer
- ✅ Sincronización entre hilos

#### Tests de Concurrencia
- ✅ Un productor, un consumidor
- ✅ Múltiples productores y consumidores
- ✅ Integridad bajo alta concurrencia
- ✅ Ausencia de race conditions

#### Tests de Casos Límite
- ✅ Buffer de tamaño 1
- ✅ Manejo de parámetros NULL
- ✅ Limpieza de memoria
- ✅ Gestión de errores

#### Tests Parametrizados
- ✅ Diferentes tamaños de buffer (1-100)
- ✅ Variaciones en número de hilos
- ✅ Diferentes patrones de carga

### Ejecutar Tests

```bash
# Tests básicos
make run-tests

# Tests con sanitizers (detección de errores)
./bin/test_productor_consumidor_asan  # AddressSanitizer
./bin/test_productor_consumidor_tsan  # ThreadSanitizer

# Tests con salida detallada
./bin/test_productor_consumidor --verbose
```

## 📈 Benchmarks y Rendimiento

### Métricas Evaluadas

#### 1. Throughput
- Items procesados por segundo
- Escalabilidad con múltiples hilos
- Efecto del tamaño del buffer

#### 2. Latencia
- Tiempo de producción/consumo individual
- Distribución de latencias (P50, P95, P99)
- Variabilidad bajo diferentes cargas

#### 3. Escalabilidad
- Rendimiento con 1-16 hilos
- Puntos de saturación
- Eficiencia por hilo

#### 4. Stress Testing
- Comportamiento bajo alta concurrencia
- Integridad de datos a gran escala
- Detección de memory leaks

### Ejecutar Benchmarks

```bash
# Benchmarks completos
make run-benchmarks

# Benchmarks específicos
./bin/benchmark_productor_consumidor --filter="throughput"
./bin/benchmark_productor_consumidor --filter="latencia"
./bin/benchmark_productor_consumidor --filter="escalabilidad"
```

### Resultados Esperados

En un sistema moderno (ejemplo):
- **Throughput**: 10,000-100,000 items/segundo
- **Latencia promedio**: < 100 microsegundos
- **P95 latencia**: < 500 microsegundos
- **Escalabilidad**: Lineal hasta 4-8 hilos

## 🔒 Aspectos de Seguridad

### Thread Safety

El código implementa múltiples mecanismos de seguridad:

1. **Exclusión Mutua**: Todos los accesos al buffer están protegidos por mutex
2. **Variables de Condición**: Evitan busy waiting y polling ineficiente
3. **Atomicidad**: Las operaciones críticas son atómicas
4. **Gestión de Memoria**: Liberación correcta de recursos

### Prevención de Problemas Comunes

#### Race Conditions
```c
// ❌ Incorrecto (race condition)
if (!esta_lleno(buffer)) {
    insertar_item(buffer, item);  // Buffer puede llenarse entre verificación e inserción
}

// ✅ Correcto (operación atómica)
producir_item(buffer, item);  // Verifica y produce atomicamente
```

#### Deadlocks
- Orden consistente de adquisición de locks
- Timeouts en operaciones críticas
- Evitar anidación compleja de mutex

#### Starvation
- Variables de condición garantizan fairness
- Señalización apropiada para despertar hilos en espera

## 🔍 Herramientas de Depuración

### Sanitizers

```bash
# AddressSanitizer (detección de errores de memoria)
cmake -DENABLE_SANITIZERS=ON ..
make
./bin/productor_consumidor_asan

# ThreadSanitizer (detección de race conditions)
./bin/productor_consumidor_tsan
```

### Valgrind

```bash
# Detección de memory leaks
valgrind --leak-check=full ./bin/productor_consumidor_main

# Detección de race conditions
valgrind --tool=helgrind ./bin/productor_consumidor_main
```

### GDB (Depuración)

```bash
# Compilar con símbolos de depuración
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Depurar con GDB
gdb ./bin/productor_consumidor_main
(gdb) set follow-fork-mode child
(gdb) break producir_item
(gdb) run
```

## 📚 Conceptos Teóricos

### El Problema Productor-Consumidor

Es un problema clásico de sincronización que ilustra:

1. **Concurrencia**: Múltiples hilos ejecutándose simultáneamente
2. **Sincronización**: Coordinación entre hilos para acceso a recursos compartidos
3. **Comunicación**: Intercambio de datos entre hilos de manera segura

### Variantes del Patrón

#### 1. Productor-Consumidor Simple
- Un productor, un consumidor
- Buffer de tamaño fijo
- Sincronización básica

#### 2. Múltiples Productores/Consumidores
- Varios hilos de cada tipo
- Mayor complejidad de sincronización
- Problemas de fairness y starvation

#### 3. Productores/Consumidores con Prioridades
- Diferentes niveles de prioridad
- Queues prioritarias
- Scheduling complejo

### Aplicaciones Reales

- **Sistemas Operativos**: Buffers de I/O, pipes, message queues
- **Bases de Datos**: Connection pools, transaction queues
- **Servidores Web**: Request processing, thread pools
- **Multimedia**: Audio/video streaming, frame buffers
- **IoT**: Sensor data processing, event handling

## ⚡ Optimizaciones

### Técnicas Implementadas

1. **Lock-Free Operations**: Para verificaciones de estado simples
2. **Batch Processing**: Procesamiento en lotes para mejor throughput
3. **Cache Locality**: Estructura de datos optimizada para cache
4. **Minimal Locking**: Secciones críticas mínimas

### Posibles Mejoras

1. **Lock-Free Buffer**: Implementación completamente sin locks
2. **Adaptive Sizing**: Buffer que cambia de tamaño dinámicamente
3. **Priority Queues**: Soporte para prioridades en items
4. **NUMA Awareness**: Optimización para sistemas NUMA

## 🐛 Resolución de Problemas

### Problemas Comunes

#### 1. Deadlock
**Síntomas**: El programa se "cuelga" sin hacer progreso
**Solución**: Verificar orden de adquisición de locks, usar timeouts

#### 2. Race Conditions
**Síntomas**: Resultados inconsistentes, corrupción de datos
**Solución**: Usar ThreadSanitizer, revisar secciones críticas

#### 3. Memory Leaks
**Síntomas**: Uso creciente de memoria
**Solución**: Usar AddressSanitizer o Valgrind

#### 4. Performance Issues
**Síntomas**: Throughput bajo, alta latencia
**Solución**: Profiling, ajustar tamaño de buffer, revisar contención

### Debugging Tips

```c
// Habilitar debug logging
#ifdef DEBUG
    printf("Thread %lu: Produciendo item %d\n", pthread_self(), item);
#endif

// Verificar estado del buffer
void debug_buffer_state(buffer_circular_t *buffer) {
    printf("Buffer: ocupados=%d, frente=%d, final=%d\n", 
           buffer->ocupados, buffer->frente, buffer->final);
}
```

## 📖 Referencias y Enlaces

### Documentación POSIX
- [pthread_mutex_t](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
- [pthread_cond_t](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html)
- [pthread_create](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)

### Libros Recomendados
- "Programming with POSIX Threads" - David R. Butenhof
- "The Art of Multiprocessor Programming" - Maurice Herlihy
- "Concurrent Programming in C" - Stephen J. Hartley

### Tutoriales Online
- [POSIX Threads Tutorial](https://computing.llnl.gov/tutorials/pthreads/)
- [Producer-Consumer Problem](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem)

## 🤝 Contribuciones

Este ejercicio es parte de un proyecto educativo. Las contribuciones son bienvenidas:

1. **Mejoras de código**: Optimizaciones, mejor legibilidad
2. **Tests adicionales**: Casos límite, scenarios específicos
3. **Documentación**: Clarificaciones, ejemplos adicionales
4. **Benchmarks**: Nuevas métricas, comparaciones

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Ver archivo `LICENSE` para detalles.

---

## 🎓 Notas para Estudiantes

### Antes de Ejecutar
1. Asegúrate de entender los conceptos de mutex y variables de condición
2. Revisa la documentación de pthread
3. Compila primero en modo Debug para mejor experiencia de aprendizaje

### Durante la Ejecución
1. Observa el comportamiento con diferentes configuraciones
2. Experimenta con los parámetros del experimento personalizado
3. Analiza los resultados de los benchmarks

### Después de la Ejecución
1. Revisa el código fuente para entender la implementación
2. Modifica parámetros y observa los efectos
3. Intenta implementar variaciones del patrón

### Ejercicios Propuestos
1. Implementa un buffer con prioridades
2. Crea una versión lock-free del buffer
3. Añade métricas de monitoring en tiempo real
4. Implementa un patrón worker pool usando este buffer
