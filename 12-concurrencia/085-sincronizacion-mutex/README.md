# Ejercicio 085: Sincronización con Mutex

## Descripción

Este ejercicio demuestra el uso de **mutex (mutual exclusion)** para sincronizar el acceso a recursos compartidos entre hilos. Los mutex son uno de los mecanismos fundamentales de sincronización en programación concurrente, permitiendo que solo un hilo acceda a un recurso crítico a la vez.

## Conceptos Clave

### 1. **Mutex (Mutual Exclusion)**
Un mutex es un objeto de sincronización que permite el acceso exclusivo a un recurso compartido:
- Solo un hilo puede "poseer" el mutex a la vez
- Otros hilos deben esperar hasta que se libere
- Previene condiciones de carrera (race conditions)

### 2. **Sección Crítica**
Región del código que accede a recursos compartidos y debe ejecutarse de forma atómica:
- Protegida por mutex
- Solo un hilo puede estar en la sección crítica
- Debe ser lo más corta posible para maximizar concurrencia

### 3. **Condiciones de Carrera (Race Conditions)**
Situación donde el resultado depende del orden de ejecución de los hilos:
- Ocurre cuando múltiples hilos acceden concurrentemente a datos compartidos
- Al menos una operación es de escritura
- Produce resultados impredecibles e incorrectos

### 4. **Tipos de Mutex**
- **Normal**: Básico, sin verificaciones adicionales
- **Recursivo**: Permite re-adquisición por el mismo hilo
- **ErrorCheck**: Detecta errores como deadlocks

## Funcionalidades Implementadas

### Operaciones Básicas
```c
// Inicialización y destrucción
bool inicializar_recurso_compartido(recurso_compartido_t* recurso, int64_t valor_inicial);
bool destruir_recurso_compartido(recurso_compartido_t* recurso);

// Operaciones thread-safe
bool incrementar_recurso_seguro(recurso_compartido_t* recurso, int64_t incremento);
bool leer_recurso_seguro(recurso_compartido_t* recurso, int64_t* valor);
bool establecer_recurso_seguro(recurso_compartido_t* recurso, int64_t nuevo_valor);
```

### Estadísticas de Rendimiento
```c
// Monitoreo de operaciones concurrentes
void registrar_lectura(estadisticas_mutex_t* stats, uint64_t tiempo_espera_ms);
void registrar_escritura(estadisticas_mutex_t* stats, uint64_t tiempo_espera_ms);
bool obtener_estadisticas(estadisticas_mutex_t* stats, estadisticas_mutex_t* copia);
```

### Demostraciones Educativas
```c
// Comparación con y sin mutex
bool ejecutar_prueba_rendimiento(parametros_prueba_t* params);
int64_t demostrar_race_condition(int num_hilos, int operaciones);

// Tipos especiales de mutex
bool demostrar_mutex_recursivo(demo_tipos_mutex_t* demo, int nivel);
bool demostrar_mutex_errorcheck(demo_tipos_mutex_t* demo);
```

## Algoritmos y Técnicas

### 1. **Patrón Lock-Execute-Unlock**
```c
int resultado = pthread_mutex_lock(&mutex);
// Sección crítica - acceso exclusivo al recurso
recurso_compartido = nuevo_valor;
pthread_mutex_unlock(&mutex);
```

### 2. **RAII para Mutex (Resource Acquisition Is Initialization)**
Aunque C no tiene destructores automáticos, se implementa gestión cuidadosa:
```c
if (pthread_mutex_lock(&mutex) == 0) {
    // Operación crítica
    pthread_mutex_unlock(&mutex);
}
```

### 3. **Timeout en Mutex**
```c
struct timespec timeout;
clock_gettime(CLOCK_REALTIME, &timeout);
timeout.tv_sec += segundos;
int resultado = pthread_mutex_timedlock(&mutex, &timeout);
```

## Estructura del Proyecto

```
085-sincronizacion-mutex/
├── include/
│   └── sincronizacion_mutex.h     # Declaraciones y estructuras
├── src/
│   ├── sincronizacion_mutex.c     # Implementación principal
│   └── main.c                     # Programa interactivo
├── tests/
│   └── test_sincronizacion_mutex.c # Tests exhaustivos
├── CMakeLists.txt                 # Configuración de build
├── README.md                      # Esta documentación
└── .gitignore                     # Exclusiones de Git
```

## Compilación

### Requisitos
- **Compilador**: GCC 7+ o Clang 6+
- **Estándar**: C99 o superior
- **Bibliotecas**: pthread (POSIX Threads)
- **Tests**: Criterion framework (opcional)

### Comandos de Compilación

```bash
# Configuración y compilación
mkdir build && cd build
cmake ..
make

# Compilación optimizada
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Solo biblioteca
make sincronizacion_mutex_lib
```

### Ejecución de Tests

```bash
# Todos los tests
make test

# Tests con salida detallada
make test_verbose

# Tests específicos
./tests/test_sincronizacion_mutex --filter="concurrencia*"

# Análisis de memoria (Linux)
make test_memory
```

## Uso del Programa

### Programa Interactivo

```bash
./bin/sincronizacion_mutex
```

El programa ofrece un menú con diferentes demostraciones:

1. **Demostrar race condition**: Muestra problemas sin sincronización
2. **Demostrar solución con mutex**: Solución correcta con mutex
3. **Comparar rendimiento**: Análisis de costo de sincronización
4. **Demostrar tipos de mutex**: Mutex recursivo y errorcheck
5. **Prueba de timeout**: Evitar bloqueos indefinidos
6. **Demostración completa**: Todas las funcionalidades

### Ejemplos de Uso

```c
#include "sincronizacion_mutex.h"

// Crear recurso protegido por mutex
recurso_compartido_t recurso;
inicializar_recurso_compartido(&recurso, 100);

// Operaciones thread-safe
incrementar_recurso_seguro(&recurso, 50);

int64_t valor;
leer_recurso_seguro(&recurso, &valor);
printf("Valor actual: %ld\n", valor); // 150

// Cleanup
destruir_recurso_compartido(&recurso);
```

## Casos de Prueba

### Tests Básicos
- ✅ Inicialización y destrucción de recursos
- ✅ Operaciones thread-safe básicas
- ✅ Manejo de errores y punteros nulos
- ✅ Estadísticas de operaciones

### Tests de Concurrencia
- ✅ Múltiples hilos incrementando contador
- ✅ Operaciones mixtas lectura/escritura
- ✅ Verificación de consistencia de datos
- ✅ Pruebas de rendimiento

### Tests de Tipos de Mutex
- ✅ Mutex recursivo con múltiples niveles
- ✅ Detección de errores con errorcheck
- ✅ Timeout en operaciones de lock

### Tests de Edge Cases
- ✅ Valores extremos (INT64_MAX, INT64_MIN)
- ✅ Destrucción múltiple
- ✅ Operaciones concurrentes en estadísticas

## Problemas Comunes y Soluciones

### 1. **Deadlock**
```c
// PROBLEMA: Orden inconsistente de mutex
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);

// SOLUCIÓN: Orden consistente
always_lock_mutex1_first();
always_lock_mutex2_second();
```

### 2. **Race Conditions**
```c
// PROBLEMA: Acceso sin protección
contador++; // No thread-safe

// SOLUCIÓN: Proteger con mutex
pthread_mutex_lock(&mutex);
contador++;
pthread_mutex_unlock(&mutex);
```

### 3. **Performance Degradation**
```c
// PROBLEMA: Sección crítica muy larga
pthread_mutex_lock(&mutex);
expensive_operation(); // Mal - bloquea otros hilos
calculate_result();
pthread_mutex_unlock(&mutex);

// SOLUCIÓN: Minimizar sección crítica
calculate_result(); // Fuera del mutex
pthread_mutex_lock(&mutex);
update_shared_data();
pthread_mutex_unlock(&mutex);
```

## Conceptos Avanzados

### 1. **Priority Inversion**
- Hilo de baja prioridad bloquea a uno de alta prioridad
- Solución: herencia de prioridad

### 2. **Lock-Free Programming**
- Uso de operaciones atómicas
- Compare-and-swap (CAS)
- Mejor rendimiento pero mayor complejidad

### 3. **Reader-Writer Locks**
- Múltiples lectores simultáneos
- Escritores exclusivos
- `pthread_rwlock_t` en POSIX

## Optimizaciones de Rendimiento

### 1. **Granularidad de Bloqueo**
```c
// Granularidad fina - mejor concurrencia
pthread_mutex_t mutex_per_element[N];

// Granularidad gruesa - menos overhead
pthread_mutex_t global_mutex;
```

### 2. **Spin Locks vs Mutex**
```c
// Spin lock para secciones críticas muy cortas
while (!__sync_bool_compare_and_swap(&lock, 0, 1)) {
    // Busy wait
}

// Mutex para secciones críticas largas
pthread_mutex_lock(&mutex);
```

### 3. **Cache Line Alignment**
```c
// Evitar false sharing
struct {
    pthread_mutex_t mutex;
    char padding[64 - sizeof(pthread_mutex_t)];
    int data;
} aligned_data;
```

## Aplicaciones Prácticas

### 1. **Pool de Conexiones**
```c
typedef struct {
    connection_t* connections[MAX_CONN];
    pthread_mutex_t mutex;
    int available_count;
} connection_pool_t;
```

### 2. **Cache Thread-Safe**
```c
typedef struct {
    hash_table_t cache;
    pthread_mutex_t cache_mutex;
    cache_stats_t stats;
} thread_safe_cache_t;
```

### 3. **Producer-Consumer**
```c
typedef struct {
    buffer_t buffer;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} bounded_buffer_t;
```

## Herramientas de Debugging

### 1. **Helgrind (Valgrind)**
```bash
valgrind --tool=helgrind ./programa
```

### 2. **Thread Sanitizer**
```bash
gcc -fsanitize=thread programa.c
```

### 3. **GDB con Threads**
```bash
(gdb) info threads
(gdb) thread 2
(gdb) bt
```

## Referencias y Recursos

### Documentación
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Linux Thread Synchronization](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Mutex Documentation](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)

### Libros Recomendados
- "Programming with POSIX Threads" - David R. Butenhof
- "The Art of Multiprocessor Programming" - Herlihy & Shavit
- "C Concurrency in Action" - Anthony Williams

### Artículos y Papers
- "The Problem with Threads" - Edward A. Lee
- "Lock-Free Data Structures" - Maurice Herlihy
- "Scalable Synchronization" - Mellor-Crummey & Scott
