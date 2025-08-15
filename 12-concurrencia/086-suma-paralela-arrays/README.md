# Ejercicio 086: Suma Paralela de Arrays con Hilos

## Descripción

Este ejercicio demuestra cómo **dividir tareas computacionales entre múltiples hilos** para aprovechar sistemas con múltiples núcleos. Se implementa la suma de elementos de un array dividiéndolo en segmentos que son procesados concurrentemente por diferentes hilos.

## Conceptos Clave

### 1. **Paralelización de Tareas Computacionales**
División de un problema computacional en subproblemas que pueden ejecutarse simultáneamente:
- **Descomposición de datos**: Dividir el array en chunks
- **Computación paralela**: Cada hilo procesa su chunk
- **Agregación de resultados**: Combinar resultados parciales

### 2. **Speedup y Eficiencia**
Métricas para evaluar el rendimiento del paralelismo:
- **Speedup**: `T_secuencial / T_paralelo`
- **Eficiencia**: `Speedup / Número_de_hilos`
- **Ley de Amdahl**: Límite teórico del speedup

### 3. **Load Balancing (Balanceo de Carga)**
Distribución equitativa del trabajo entre hilos:
- **División uniforme**: Chunks de tamaño similar
- **Manejo de residuos**: Distribución de elementos extra
- **Granularidad óptima**: Balance entre overhead y paralelismo

### 4. **Overhead de Paralelización**
Costos asociados con la creación y sincronización de hilos:
- **Creación de hilos**: `pthread_create()`
- **Sincronización**: `pthread_join()`
- **Gestión de memoria**: Estructuras de datos compartidas

## Funcionalidades Implementadas

### Suma Paralela Básica
```c
// Configuración de suma paralela
configuracion_suma_t config = {
    .array = mi_array,
    .tamano = 10000,
    .num_hilos = 4,
    .mostrar_detalles = true
};

resultado_suma_paralela_t resultado;
suma_paralela(&config, &resultado);
```

### Función de Hilo
```c
void* hilo_suma_parcial(void* arg) {
    parametros_suma_t* params = (parametros_suma_t*)arg;
    
    params->tiempo_inicio = obtener_tiempo_microsegundos();
    
    params->resultado = 0;
    for (size_t i = params->inicio; i < params->fin; i++) {
        params->resultado += params->array[i];
    }
    
    params->tiempo_fin = obtener_tiempo_microsegundos();
    return NULL;
}
```

### Benchmark y Análisis
```c
// Comparación secuencial vs paralelo
benchmark_suma_t benchmark;
ejecutar_benchmark_suma(array, tamano, num_hilos, &benchmark);

printf("Speedup: %.2fx\n", benchmark.speedup);
printf("Eficiencia: %.2f%%\n", benchmark.eficiencia * 100.0);
```

### Pruebas de Escalabilidad
```c
// Probar con diferentes números de hilos
ejecutar_pruebas_escalabilidad(array, tamano, max_hilos);
```

## Algoritmos y Técnicas

### 1. **División de Rango en Chunks**
```c
void dividir_rango_en_chunks(size_t inicio, size_t fin, int num_hilos, size_t* chunks) {
    size_t tamano_total = fin - inicio;
    size_t chunk_base = tamano_total / num_hilos;
    size_t elementos_extra = tamano_total % num_hilos;
    
    chunks[0] = inicio;
    for (int i = 1; i <= num_hilos; i++) {
        size_t tamano_chunk = chunk_base + (i <= elementos_extra ? 1 : 0);
        chunks[i] = chunks[i-1] + tamano_chunk;
    }
}
```

### 2. **Patrón Fork-Join**
```c
// Fork: Crear hilos
for (int i = 0; i < num_hilos; i++) {
    pthread_create(&hilos[i], NULL, hilo_suma_parcial, &params[i]);
}

// Join: Esperar resultados
for (int i = 0; i < num_hilos; i++) {
    pthread_join(hilos[i], NULL);
}

// Reducción: Combinar resultados
int64_t suma_total = 0;
for (int i = 0; i < num_hilos; i++) {
    suma_total += params[i].resultado;
}
```

### 3. **Cálculo de Hilos Óptimo**
```c
int calcular_num_hilos_optimo(size_t tamano_array) {
    int num_cpus = obtener_num_cpus();
    
    if (tamano_array < 1000) {
        return 1;  // Array pequeño: no vale la pena paralelizar
    } else if (tamano_array < 10000) {
        return min(2, num_cpus);  // Paralelismo limitado
    } else {
        return num_cpus;  // Usar todos los CPUs disponibles
    }
}
```

## Estructura del Proyecto

```
086-suma-paralela-arrays/
├── include/
│   └── suma_paralela_arrays.h     # Declaraciones y estructuras
├── src/
│   ├── suma_paralela_arrays.c     # Implementación principal
│   └── main.c                     # Programa interactivo
├── tests/
│   └── test_suma_paralela_arrays.c # Tests exhaustivos
├── CMakeLists.txt                 # Configuración de build
├── README.md                      # Esta documentación
└── .gitignore                     # Exclusiones de Git
```

## Compilación

### Requisitos
- **Compilador**: GCC 7+ o Clang 6+
- **Estándar**: C99 o superior
- **Bibliotecas**: pthread, libm (matemáticas)
- **Tests**: Criterion framework (opcional)

### Comandos de Compilación

```bash
# Configuración y compilación
mkdir build && cd build
cmake ..
make

# Compilación optimizada para rendimiento
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Solo biblioteca
make suma_paralela_arrays_lib
```

### Ejecución de Tests

```bash
# Todos los tests
make test

# Tests con salida detallada
make test_verbose

# Solo tests de rendimiento
make test_performance

# Tests de escalabilidad
make escalabilidad

# Detección de race conditions
make test_helgrind
```

## Uso del Programa

### Programa Interactivo

```bash
./bin/suma_paralela_arrays
```

El programa ofrece varias demostraciones:

1. **Ejemplo básico**: Implementación del enunciado original
2. **Benchmark comparativo**: Secuencial vs paralelo personalizable
3. **Pruebas de escalabilidad**: Análisis con diferentes números de hilos
4. **Array personalizado**: Configuración custom de arrays
5. **Análisis detallado**: Información pormenorizada de rendimiento
6. **Demostración completa**: Todas las funcionalidades

### Ejemplos de Uso

#### Ejemplo Básico (del Enunciado)
```c
#include "suma_paralela_arrays.h"

int main() {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    configuracion_suma_t config = {
        .array = array,
        .tamano = 10,
        .num_hilos = 2,
        .mostrar_detalles = true
    };
    
    resultado_suma_paralela_t resultado;
    if (suma_paralela(&config, &resultado)) {
        printf("Suma total: %ld\n", resultado.suma_total);
        // Output: Suma total: 55
        limpiar_resultado_suma(&resultado);
    }
    
    return 0;
}
```

#### Benchmark Avanzado
```c
// Array grande para ver beneficios del paralelismo
int* array = crear_array_secuencia(1000000, 1, 1);

benchmark_suma_t benchmark;
if (ejecutar_benchmark_suma(array, 1000000, 4, &benchmark)) {
    printf("Speedup: %.2fx\n", benchmark.speedup);
    printf("Eficiencia: %.2f%%\n", benchmark.eficiencia * 100.0);
}

free(array);
```

## Casos de Prueba

### Tests Básicos
- ✅ Suma secuencial con diferentes tamaños
- ✅ Suma paralela con 1, 2, 4, 8 hilos
- ✅ Validación de parámetros y manejo de errores
- ✅ Inicialización y limpieza de estructuras

### Tests de Arrays
- ✅ Creación de arrays secuenciales y aleatorios
- ✅ Arrays con valores positivos, negativos y mixtos
- ✅ Validación de overflow y underflow
- ✅ Estadísticas de arrays (min, max, promedio)

### Tests de Concurrencia
- ✅ Corrección con múltiples hilos
- ✅ Comparación secuencial vs paralelo
- ✅ Arrays grandes (100,000+ elementos)
- ✅ División correcta de trabajo entre hilos

### Tests de Rendimiento
- ✅ Medición de tiempos de ejecución
- ✅ Cálculo de speedup y eficiencia
- ✅ Pruebas de escalabilidad
- ✅ Detección de degradación por overhead

### Tests de Utilidades
- ✅ División de rangos en chunks
- ✅ Cálculo de hilos óptimo
- ✅ Funciones de tiempo y estadísticas
- ✅ Detección de número de CPUs

## Análisis de Rendimiento

### Factores que Afectan el Speedup

#### 1. **Tamaño del Array**
```
Array pequeño  (< 1,000):    Overhead > Beneficio
Array mediano  (1K - 100K):  Speedup moderado
Array grande   (> 100K):     Speedup óptimo
```

#### 2. **Número de Hilos**
```
1 hilo:     Referencia base
2 hilos:    Speedup ~1.8x (ideal: 2.0x)
4 hilos:    Speedup ~3.2x (ideal: 4.0x)
8+ hilos:   Posible degradación por contención
```

#### 3. **Características del Sistema**
- **CPUs físicos**: Determina el máximo speedup práctico
- **Cache coherency**: Afecta el rendimiento con muchos hilos
- **Memory bandwidth**: Puede ser el cuello de botella

### Métricas de Evaluación

#### Speedup Ideal vs Real
```c
double speedup_ideal = (double)num_hilos;
double speedup_real = tiempo_secuencial / tiempo_paralelo;
double eficiencia = speedup_real / speedup_ideal;

printf("Speedup ideal: %.2fx\n", speedup_ideal);
printf("Speedup real:  %.2fx\n", speedup_real);
printf("Eficiencia:    %.1f%%\n", eficiencia * 100.0);
```

#### Análisis de Overhead
```c
uint64_t overhead = tiempo_paralelo - (tiempo_secuencial / num_hilos);
printf("Overhead estimado: %lu microsegundos\n", overhead);
```

## Optimizaciones Implementadas

### 1. **Ajuste Dinámico de Hilos**
```c
// No usar más hilos que elementos
int num_hilos_efectivo = min(config->num_hilos, config->tamano);
```

### 2. **División Equilibrada de Trabajo**
```c
// Distribuir elementos extra uniformemente
size_t elementos_extra = tamano_total % num_hilos;
for (int i = 1; i <= num_hilos; i++) {
    size_t tamano_chunk = chunk_base + (i <= elementos_extra ? 1 : 0);
    chunks[i] = chunks[i-1] + tamano_chunk;
}
```

### 3. **Medición Precisa de Tiempo**
```c
uint64_t obtener_tiempo_microsegundos(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}
```

### 4. **Validación de Overflow**
```c
bool validar_array_para_suma(const int* array, size_t tamano) {
    int64_t suma_positiva = 0, suma_negativa = 0;
    
    for (size_t i = 0; i < tamano; i++) {
        if (array[i] > 0) {
            if (suma_positiva > INT64_MAX - array[i]) return false;
            suma_positiva += array[i];
        } else if (array[i] < 0) {
            if (suma_negativa < INT64_MIN - array[i]) return false;
            suma_negativa += array[i];
        }
    }
    return true;
}
```

## Aplicaciones Prácticas

### 1. **Procesamiento de Datos Masivos**
```c
// Suma de ventas por regiones
int* ventas_regionales = cargar_datos_ventas();
suma_paralela_ventas(ventas_regionales, num_regiones, num_hilos);
```

### 2. **Análisis Numérico**
```c
// Integración numérica paralela
double integral = integracion_paralela(funcion, a, b, n_intervalos, num_hilos);
```

### 3. **Procesamiento de Imágenes**
```c
// Suma de píxeles por canales de color
suma_paralela_pixeles(imagen_rgb, ancho, alto, num_hilos);
```

### 4. **Simulaciones Científicas**
```c
// Reducción de resultados de simulación
double energia_total = suma_paralela_energia(particulas, n_particulas, num_hilos);
```

## Patrones de Diseño Aplicados

### 1. **Map-Reduce**
- **Map**: Cada hilo procesa un chunk del array
- **Reduce**: Agregación de resultados parciales

### 2. **Fork-Join**
- **Fork**: División del trabajo en tareas paralelas
- **Join**: Sincronización y combinación de resultados

### 3. **SPMD (Single Program Multiple Data)**
- Mismo código ejecutado en diferentes datos
- Parametrización por rango de índices

## Herramientas de Debugging y Profiling

### 1. **Detección de Race Conditions**
```bash
# Valgrind Helgrind
make test_helgrind

# Thread Sanitizer
make test_with_tsan
```

### 2. **Análisis de Memoria**
```bash
# Detección de memory leaks
make test_memory

# Profiling de memoria
valgrind --tool=massif ./bin/suma_paralela_arrays
```

### 3. **Profiling de CPU**
```bash
# gprof
gcc -pg programa.c -o programa
./programa
gprof programa gmon.out

# perf (Linux)
perf record ./bin/suma_paralela_arrays
perf report
```

## Limitaciones y Consideraciones

### 1. **Ley de Amdahl**
El speedup está limitado por la fracción secuencial del algoritmo:
```
Speedup_max = 1 / (f_secuencial + (1-f_secuencial)/N)
```

### 2. **Overhead de Hilos**
Para arrays pequeños, el overhead puede superar los beneficios:
- Creación de hilos: ~10-100 μs por hilo
- Sincronización: ~1-10 μs por operación
- Context switching: Variable según el sistema

### 3. **Memory Bandwidth**
Con muchos hilos, el ancho de banda de memoria puede convertirse en cuello de botella.

### 4. **Cache Effects**
- **Cache line sharing**: False sharing entre hilos
- **Cache miss rate**: Aumenta con más hilos
- **Memory locality**: Importante para el rendimiento

## Referencias y Recursos

### Documentación
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Parallel Computing Concepts](https://hpc.llnl.gov/training/tutorials)
- [Amdahl's Law](https://en.wikipedia.org/wiki/Amdahl%27s_law)

### Libros Recomendados
- "Introduction to Parallel Computing" - Ananth Grama
- "Parallel Programming in C with MPI and OpenMP" - Michael Quinn
- "The Art of Multiprocessor Programming" - Herlihy & Shavit

### Papers y Artículos
- "Evaluating the Performance of Parallel Algorithms"
- "Load Balancing Techniques for Parallel Applications"
- "Cache-Efficient Parallel Algorithms"