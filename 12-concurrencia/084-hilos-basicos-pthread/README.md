# Ejercicio 084: Hilos básicos (pthread)

## Descripción

Introducción práctica al uso de hilos POSIX (`pthread`). El ejercicio cubre la creación de hilos, paso de argumentos, sincronización básica (join, detach) y ejemplos simples para ilustrar condiciones de carrera y cómo evitarlas.

## Objetivos de aprendizaje

- Comprender la creación y finalización de hilos con `pthread_create` y `pthread_join`.
- Diferenciar `pthread_join` y `pthread_detach` y sus efectos en la gestión de recursos.
- Identificar condiciones de carrera y aplicar técnicas básicas para evitarlas.
- Ejecutar tests automáticos que validan comportamiento concurrente.

## Conceptos clave

- `pthread_create`, `pthread_join`, `pthread_detach`.
- Paso de argumentos a hilos mediante `void*` y estructuras.
- Condición de carrera (race condition) y ejemplos simples.
- Sincronización mínima (join) y consideraciones sobre mutex/semaforos (introducción).

## Estructura del proyecto

```
084-hilos-basicos-pthread/
├── include/
│   └── hilos_basicos.h         # API pública del ejercicio
├── src/
│   ├── hilos_basicos.c         # Implementación de utilidades con pthread
│   └── main.c                  # Programa demostrativo / menú
├── tests/
│   └── test_hilos_basicos.c    # Tests (Criterion o CTest)
├── CMakeLists.txt
└── README.md
```

## Compilación

Requisitos mínimos: compilador C (gcc o clang) con soporte pthread.

### Usando CMake (recomendado)

```bash
mkdir -p build && cd build
cmake ..
make -j
```

### Compilación manual

```bash
gcc -std=c11 -Wall -Wextra -O2 -pthread src/hilos_basicos.c src/main.c -I include -o hilos_basicos_main
```

## Ejecución

Ejecutables disponibles tras build:

```bash
# Ejecutable principal (desde la raíz si usaste build/)
./build/hilos_basicos_main

# Ejecutable compilado manualmente
./hilos_basicos_main
```

El programa principal incluye un menú con demostraciones (crear hilos, sumar con varios hilos, ejemplos de race condition, etc.).

## Tests

Los tests están en `tests/` y pueden ser ejecutados mediante CTest o ejecutables generados por CMake.

```bash
cd build
ctest --output-on-failure
# o
make test
```

Si prefieres ejecutar el test directamente (Criterion):

```bash
gcc -std=c11 -Wall -Wextra -pthread src/hilos_basicos.c tests/test_hilos_basicos.c -I include -lcriterion -o test_hilos_basicos
./test_hilos_basicos
```

## Ejemplos de uso

1. Ejecutar demostración básica de creación de hilos:

```bash
./build/hilos_basicos_main
# Seleccionar la opción "Ejemplo básico" en el menú
```

2. Ejecutar varios hilos que incrementan un contador (ejemplo de race condition):

```bash
./build/hilos_basicos_main --demo race
```

3. Ejecutar pruebas unitarias:

```bash
cd build && ctest --output-on-failure
```

## Casos de prueba importantes

- Inicialización y terminación correcta de hilos.
- Comportamiento determinista con `pthread_join`.
- Demostración de race condition cuando múltiples hilos modifican un contador sin sincronización.
- Verificación de que los recursos se liberan (no leaks básicos).

## Buenas prácticas y notas

- Usar `pthread_join` cuando necesites resultado o sincronización con el hilo hijo.
- Usar `pthread_detach` para hilos que no requieren join y liberar recursos automáticamente.
- Para sincronización real use mutexes (`pthread_mutex_t`) o mecanismos más avanzados.

## Mejoras futuras

1. Añadir ejemplos con `pthread_mutex_t` para corregir race conditions.
2. Añadir tests de rendimiento que midan escalado con número de hilos.
3. Incluir ejemplos de thread pools para evitar overhead de crear hilos frecuentemente.

## Referencias

- POSIX Threads Programming: https://computing.llnl.gov/tutorials/pthreads/
- The Linux Programming Interface (Kerrisk)
- Stevens, Unix Network Programming (capítulos relevantes sobre threads)

## API pública

A continuación se muestran las estructuras y funciones públicas definidas en `include/hilos_basicos.h` (firmas exactas):

Estructuras:

```c
typedef struct {
    int id;
    char mensaje[BUFFER_SIZE];
    int iteraciones;
    int delay_ms;
} hilo_params_t;

typedef struct {
    int hilo_id;
    int operaciones_realizadas;
    long tiempo_ejecucion_ms;
    bool exito;
} hilo_resultado_t;

typedef struct {
    int contador_compartido;
    pthread_mutex_t mutex;
    int num_hilos;
    int incrementos_por_hilo;
} contador_compartido_t;
```

Funciones principales:

```c
void* funcion_hilo_basico(void* arg);
void* funcion_hilo_con_parametros(void* arg);
void* funcion_hilo_calculo(void* arg);
void* funcion_hilo_trabajo(void* arg);

int demostrar_hilo_basico(void);
int demostrar_hilos_con_parametros(void);
int demostrar_multiples_hilos(int num_hilos);
int demostrar_calculo_paralelo(void);
int demostrar_sincronizacion_basica(void);

void* incrementar_sin_mutex(void* arg);
void* incrementar_con_mutex(void* arg);
int demostrar_condicion_carrera(void);
int demostrar_mutex_solucion(void);

long obtener_id_hilo_actual(void);
void simular_trabajo(int segundos, const char* mensaje);
long obtener_tiempo_ms(void);
hilo_params_t crear_parametros_hilo(int id, const char* mensaje);
void liberar_resultado_hilo(hilo_resultado_t* resultado);

void mostrar_info_hilos(void);
void mostrar_estadisticas_hilos(hilo_resultado_t* resultados[], int num_resultados);
bool verificar_soporte_pthread(void);
```

Se recomienda usar estas funciones desde un programa de demostración o tests: las funciones `demostrar_*` realizan ejemplos completos y liberan la memoria que devuelven los hilos cuando corresponde.

---

Para comprobaciones rápidas usa `./tools/quick_test.sh 12 084` desde la raíz del repositorio.
