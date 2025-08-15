# Ejercicio 087: Race Conditions

## Descripción

Este ejercicio demuestra las **condiciones de carrera (race conditions)** que ocurren cuando múltiples hilos acceden concurrentemente a recursos compartidos sin sincronización adecuada. Las race conditions son uno de los bugs más peligrosos y difíciles de detectar en programación concurrente.

## Objetivos de Aprendizaje

- Entender qué son las race conditions y por qué ocurren
- Observar los efectos no determinísticos de las race conditions
- Aprender a detectar y medir race conditions
- Comparar código con y sin sincronización
- Analizar estadísticamente la inconsistencia en resultados
- Comprender la importancia de la sincronización apropiada

## Conceptos Clave

### ¿Qué es una Race Condition?

Una **race condition** ocurre cuando:
1. Múltiples hilos acceden al mismo recurso compartido
2. Al menos uno de los accesos es una escritura
3. Los accesos no están sincronizados
4. El resultado final depende del orden de ejecución (no determinístico)

### Ejemplo Clásico

```c
// Variable compartida
int contador = 0;

// Hilo 1                    Hilo 2
contador++;                 contador++;

// Problema: counter++ no es atómico
// 1. Leer valor actual
// 2. Incrementar valor
// 3. Escribir nuevo valor
```

Si ambos hilos leen el mismo valor inicial, ambos escribirán el mismo valor incrementado, causando la pérdida de un incremento.

### Factores que Influyen

1. **Número de hilos**: Más hilos → mayor probabilidad
2. **Frecuencia de acceso**: Más accesos → mayor contención
3. **Duración de operaciones**: Operaciones más largas → mayor ventana de race condition
4. **Arquitectura del hardware**: Núcleos, cachés, memory model

## Funcionalidades Implementadas

### 1. Demostración Básica
- Dos hilos incrementando un contador sin sincronización
- Comparación con versión sincronizada con mutex
- Medición de incrementos perdidos

### 2. Análisis Estadístico
- Ejecución de múltiples experimentos
- Cálculo de tasas de inconsistencia
- Análisis de variabilidad en resultados
- Estadísticas de tiempo de ejecución

### 3. Comparación de Rendimiento
- Medición del overhead de sincronización
- Factor de ralentización con mutex
- Análisis costo-beneficio de la sincronización

### 4. Simulación de Escenarios
- Diferentes configuraciones de hilos y carga
- Efectos de delays artificiales
- Predicción de probabilidades de race conditions

### 5. Herramientas de Análisis
- Generación de reportes detallados
- Visualización de inconsistencias
- Recomendaciones de sincronización

## Estructura del Proyecto

```
087-race-conditions/
├── include/
│   └── race_conditions.h          # Declaraciones y tipos
├── src/
│   ├── race_conditions.c          # Implementación principal
│   └── main.c                     # Programa interactivo
├── tests/
│   ├── test_race_conditions.c     # Tests exhaustivos con Criterion
│   ├── benchmark_race_conditions.c # Benchmarks de rendimiento
│   └── stress_test.c              # Tests de estrés
├── tools/
│   ├── reporter.c                 # Generador de reportes
│   └── analyzer.c                 # Analizador automatizado
├── demo/
│   └── demo_simple.c              # Demostración simple
├── docs/
│   └── Doxyfile.in               # Configuración de documentación
├── CMakeLists.txt                # Configuración de compilación
├── README.md                     # Este archivo
└── .gitignore                    # Exclusiones de git
```

## Compilación

### Requisitos

- Compilador C99 compatible (GCC, Clang)
- CMake 3.10 o superior
- Biblioteca pthread
- Criterion (opcional, para tests)

### Compilación Básica

```bash
mkdir build
cd build
cmake ..
make
```

### Opciones de Compilación

```bash
# Con tests (requiere Criterion)
cmake -DBUILD_TESTS=ON ..

# Con ThreadSanitizer (recomendado para detectar race conditions)
cmake -DENABLE_TSAN=ON ..

# Con AddressSanitizer
cmake -DENABLE_ASAN=ON ..

# Build de Debug con información detallada
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build optimizado para rendimiento
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Compilación con Sanitizers

Para detectar race conditions automáticamente:

```bash
# ThreadSanitizer (detección de race conditions)
cmake -DENABLE_TSAN=ON -DCMAKE_BUILD_TYPE=Debug ..
make
./bin/race_conditions

# AddressSanitizer (detección de memory errors)
cmake -DENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=Debug ..
```

## Uso

### Programa Principal

```bash
./bin/race_conditions
```

El programa ofrece un menú interactivo con las siguientes opciones:

1. **Ejemplo básico**: Demostración simple del enunciado
2. **Demostración completa**: Análisis exhaustivo con estadísticas
3. **Factores influyentes**: Cómo diferentes parámetros afectan las race conditions
4. **Simulación de escenarios**: Diferentes configuraciones y cargas
5. **Experimento personalizado**: Configuración manual de parámetros
6. **Comparación con sincronización**: Rendimiento vs correctitud
7. **Análisis estadístico**: Análisis detallado de múltiples ejecuciones
8. **Información educativa**: Explicación de conceptos

### Ejemplos de Uso

#### Experimento Básico
```bash
# Ejecutar demostración básica
./bin/race_conditions
# Seleccionar opción 1
```

#### Análisis con ThreadSanitizer
```bash
# Compilar con ThreadSanitizer
cmake -DENABLE_TSAN=ON ..
make

# Ejecutar (detectará race conditions automáticamente)
./bin/race_conditions
```

#### Análisis Estadístico
```bash
# Ejecutar múltiples experimentos
./bin/race_conditions
# Seleccionar opción 7
# Configurar: 4 hilos, 50000 incrementos/hilo, 20 experimentos
```

### Herramientas Adicionales

```bash
# Generador de reportes
./bin/race_conditions_reporter

# Analizador automatizado
./bin/race_conditions_analyzer

# Demostración simple
./bin/race_conditions_demo
```

## Ejecución de Tests

```bash
# Ejecutar todos los tests
make test

# Ejecutar tests específicos
./bin/test_race_conditions --pattern="race_conditions_basicos/*"
./bin/test_race_conditions --pattern="race_conditions_estadistico/*"

# Tests con verbose
make test_verbose

# Benchmark de rendimiento
./bin/benchmark_race_conditions

# Test de estrés
./bin/stress_test_race_conditions
```

## Resultados Esperados

### Sin Race Conditions (Ideal)
```
Valor esperado: 200000
Valor obtenido: 200000
Diferencia: 0
Error: 0.00%
Estado: ✓ CORRECTO
```

### Con Race Conditions (Común)
```
Valor esperado: 200000
Valor obtenido: 187342
Diferencia: 12658
Error: 6.33%
Estado: ✗ RACE CONDITION DETECTADA
```

### Análisis Estadístico
```
=== Análisis Estadístico de Race Conditions ===
Número de experimentos: 20
Valor esperado (constante): 200000

Estadísticas de valores obtenidos:
  Mínimo: 182456
  Máximo: 198234
  Promedio: 189345.50
  Desviación estándar: 4521.23

Tasa de inconsistencia: 85.0%
Evaluación: Race conditions FRECUENTES
```

## Aspectos Educativos

### 1. Observación de No Determinismo

Los resultados varían entre ejecuciones:
- A veces el resultado es correcto (por casualidad)
- La mayoría de las veces hay incrementos perdidos
- La cantidad perdida varía impredeciblemente

### 2. Factores de Influencia

**Número de hilos**:
- 1 hilo: No hay race conditions
- 2 hilos: Race conditions ocasionales
- 4+ hilos: Race conditions frecuentes

**Carga de trabajo**:
- Pocos incrementos: Poca contención
- Muchos incrementos: Mayor probabilidad de colisión

**Sincronización**:
- Sin mutex: Race conditions
- Con mutex: Correctitud garantizada + overhead

### 3. Detección y Medición

**Indicadores de Race Conditions**:
- Resultados inconsistentes entre ejecuciones
- Valores finales menores al esperado
- Alta variabilidad estadística

**Herramientas de Detección**:
- ThreadSanitizer: Detección automática
- Análisis estadístico: Medición de inconsistencia
- Tests de estrés: Exposición de conditions raras

### 4. Soluciones

**Mutex (Implementado)**:
```c
pthread_mutex_lock(&mutex);
contador++;
pthread_mutex_unlock(&mutex);
```

**Operaciones Atómicas**:
```c
__atomic_fetch_add(&contador, 1, __ATOMIC_SEQ_CST);
```

**Diseño Lock-Free**:
- Evitar estado compartido mutable
- Usar estructuras de datos concurrentes
- Message passing en lugar de shared memory

## Casos de Estudio

### 1. Contador Bancario
```c
// Peligroso: Race condition en balance
void transferir(cuenta_t* desde, cuenta_t* hacia, int monto) {
    desde->balance -= monto;  // Race condition!
    hacia->balance += monto;  // Race condition!
}
```

### 2. Lista Enlazada Concurrente
```c
// Peligroso: Race condition en estructura
void insertar(lista_t* lista, int valor) {
    nodo_t* nuevo = malloc(sizeof(nodo_t));
    nuevo->valor = valor;
    nuevo->siguiente = lista->cabeza;  // Race condition!
    lista->cabeza = nuevo;             // Race condition!
}
```

### 3. Cache Compartido
```c
// Peligroso: Race condition en cache
valor_t obtener_cache(cache_t* cache, clave_t clave) {
    if (!cache->valido[clave]) {
        cache->datos[clave] = calcular_valor(clave);  // Race condition!
        cache->valido[clave] = true;                  // Race condition!
    }
    return cache->datos[clave];
}
```

## Buenas Prácticas

### 1. Diseño
- **Minimizar estado compartido**: Menos compartición = menos race conditions
- **Inmutabilidad**: Datos inmutables no tienen race conditions
- **Aislamiento de hilos**: Cada hilo trabaja en su propio espacio

### 2. Sincronización
- **Usar primitivas apropiadas**: Mutex, semáforos, variables de condición
- **Granularidad balanceada**: Ni muy fina (overhead) ni muy gruesa (contención)
- **Orden consistente**: Evitar deadlocks con orden de bloqueo

### 3. Testing
- **Tests con múltiples configuraciones**: Diferentes números de hilos y cargas
- **Sanitizers**: ThreadSanitizer, AddressSanitizer
- **Tests de estrés**: Cargas altas y ejecutores prolongadas

### 4. Monitoreo
- **Métricas de concurrencia**: Contención, tiempo de bloqueo
- **Detección en producción**: Herramientas de profiling
- **Alertas**: Detección automática de anomalías

## Preguntas de Reflexión

1. **¿Por qué las race conditions son no determinísticas?**
   - Dependen del scheduler del SO
   - Timing entre hilos es impredecible
   - Arquitectura del hardware influye

2. **¿Cuándo es aceptable el riesgo de race conditions?**
   - Datos aproximados (estadísticas)
   - Sistemas con tolerancia a inconsistencia
   - Nunca en sistemas críticos

3. **¿Cómo balancear rendimiento vs correctitud?**
   - Medir overhead real de sincronización
   - Usar operaciones atómicas cuando sea posible
   - Diseñar para evitar necesidad de sincronización

4. **¿Qué alternativas existen a los mutex?**
   - Operaciones atómicas
   - Lock-free data structures
   - Message passing
   - Functional programming

## Recursos Adicionales

### Documentación
- `man pthread_mutex_init`
- `man pthread_create`
- ThreadSanitizer documentation

### Herramientas
- **Valgrind**: `valgrind --tool=helgrind ./programa`
- **ThreadSanitizer**: `gcc -fsanitize=thread`
- **Intel Inspector**: Detección comercial de race conditions

### Lecturas Recomendadas
- "The Art of Multiprocessor Programming" - Herlihy & Shavit
- "Programming with POSIX Threads" - Butenhof
- "Java Concurrency in Practice" - Goetz (conceptos aplicables a C)

---

**Nota de Seguridad**: Las race conditions son bugs extremadamente peligrosos en software de producción. Pueden causar corrupción de datos, crashes impredecibles y vulnerabilidades de seguridad. Siempre use sincronización apropiada en código real.
