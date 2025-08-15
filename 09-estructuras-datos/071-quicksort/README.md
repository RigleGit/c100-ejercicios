# Ejercicio 071: Algoritmo Quicksort

## Descripción

Este ejercicio implementa una versión completa y educativa del algoritmo **Quicksort**, uno de los algoritmos de ordenamiento más importantes y eficientes. Incluye múltiples variantes, optimizaciones y análisis de rendimiento.

## Conceptos Clave

### Quicksort
- **Algoritmo divide y vencerás**: Divide el problema en sub-problemas más pequeños
- **Partición**: Reorganiza el array alrededor de un elemento pivot
- **Recursión**: Se aplica recursivamente a las sub-listas
- **Complejidad promedio**: O(n log n)
- **Complejidad peor caso**: O(n²)
- **Complejidad espacial**: O(log n) por la pila de recursión

### Estrategias de Pivot
1. **Primer elemento**: Simple pero puede ser ineficiente
2. **Último elemento**: Implementación estándar
3. **Elemento medio**: Mejora para arrays parcialmente ordenados
4. **Mediana de tres**: Selecciona la mediana entre primer, medio y último elemento
5. **Aleatorio**: Garantiza buen rendimiento promedio

## Funcionalidades Implementadas

### Variantes Básicas
- `quicksort_basico()`: Implementación estándar con último elemento como pivot
- `particion_basica()`: Función de partición clásica
- `intercambiar()`: Intercambio eficiente de elementos

### Variantes Básicas
- `quicksort_basico()`: Implementación estándar con último elemento como pivot
- `quicksort_simple()`: Versión simple y directa (ejemplo educativo)
- `particion_basica()`: Función de partición clásica
- `particion_simple()`: Partición básica con intercambio de punteros
- `intercambiar()` / `intercambiar_simple()`: Intercambio eficiente de elementos

### Variantes Optimizadas
- `quicksort_optimizado()`: Soporte para diferentes estrategias de pivot
- `quicksort_hibrido()`: Combina Quicksort con Insertion Sort para arrays pequeños
- `quicksort_iterativo()`: Versión iterativa usando pila explícita
- `quicksort_tres_vias()`: Manejo eficiente de elementos duplicados

### Funciones de Partición
- `particion_primer_elemento()`: Usa el primer elemento como pivot
- `particion_elemento_medio()`: Usa el elemento medio como pivot
- `particion_mediana_tres()`: Usa mediana de tres elementos como pivot
- `particion_aleatoria()`: Selección aleatoria de pivot
- `particion_hoare()`: Implementación de Hoare con dos punteros

### Análisis y Utilidades
- `EstadisticasOrdenamiento`: Estructura para medir rendimiento
- `esta_ordenado()`: Verificación de ordenamiento correcto
- `comparar_variantes_quicksort()`: Análisis comparativo de rendimiento
- Generadores de diferentes tipos de arrays (aleatorios, ordenados, duplicados)

### Demostración Educativa
- `demo_quicksort_paso_a_paso()`: Visualización del proceso de ordenamiento
- `quicksort_visualizado()`: Muestra cada paso de la recursión
- `demo_completa_quicksort()`: Demostración integral de todas las variantes

## Estructura del Proyecto

```
071-quicksort/
├── include/
│   └── quicksort.h          # Declaraciones de funciones y estructuras
├── src/
│   └── quicksort.c          # Implementación completa
├── tests/
│   └── test_quicksort.c     # Tests exhaustivos con Criterion
├── CMakeLists.txt           # Configuración de compilación
├── README.md               # Esta documentación
└── .gitignore              # Archivos a ignorar por git
```

## Compilación

### Usando CMake (Recomendado)

```bash
# Crear directorio de compilación
mkdir build && cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar el programa principal
./quicksort

# Ejecutar tests (requiere Criterion)
make run_tests
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/quicksort.c -o quicksort

# Compilar y ejecutar tests
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/quicksort.c tests/test_quicksort.c -lcriterion -o test_quicksort
./test_quicksort
```

## Uso

### Ejemplo Básico

```c
#include "quicksort.h"

int main() {
    int arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array original:\n");
    imprimirArray_simple(arr, n);

    quicksort_simple(arr, 0, n - 1);

    printf("Array ordenado:\n");
    imprimirArray_simple(arr, n);
    
    return 0;
}
```

### Ejemplo Avanzado

```c
#include "quicksort.h"

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int tamanio = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array original: ");
    imprimir_array(arr, tamanio, "");
    
    quicksort_basico(arr, 0, tamanio - 1);
    
    printf("Array ordenado: ");
    imprimir_array(arr, tamanio, "");
    
    return 0;
}
```

### Ejemplo con Estadísticas

```c
int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
int tamanio = sizeof(arr) / sizeof(arr[0]);

EstadisticasOrdenamiento stats;
estadisticas_inicializar(&stats);

quicksort_optimizado(arr, 0, tamanio - 1, PIVOT_MEDIANA_DE_TRES, &stats);
stats.tiempo_fin = clock();

estadisticas_imprimir(&stats, tamanio);
```

### Ejemplo de Comparación de Variantes

```c
int arr[] = {64, 34, 25, 12, 22, 11, 90, 50, 30, 70};
int tamanio = sizeof(arr) / sizeof(arr[0]);

comparar_variantes_quicksort(arr, tamanio);
```

## Algoritmos Implementados

### 0. Quicksort Simple (Versión Educativa)
**Propósito**: Implementación directa y fácil de entender
**Características**: Usa punteros para intercambio, partición simple
```c
void quicksort_simple(int arr[], int bajo, int alto);
void intercambiar_simple(int* a, int* b);
int particionar_simple(int arr[], int bajo, int alto);
```

### 1. Quicksort Básico
**Complejidad**: O(n log n) promedio, O(n²) peor caso
```c
void quicksort_basico(int arr[], int bajo, int alto);
```

### 2. Quicksort Optimizado
**Características**: Múltiples estrategias de pivot, recolección de estadísticas
```c
void quicksort_optimizado(int arr[], int bajo, int alto, TipoPivot tipo_pivot, EstadisticasOrdenamiento* stats);
```

### 3. Quicksort Híbrido
**Optimización**: Cambia a Insertion Sort para arrays pequeños
```c
void quicksort_hibrido(int arr[], int bajo, int alto, int umbral_insertion);
```

### 4. Quicksort Iterativo
**Ventaja**: No utiliza recursión, evita stack overflow
```c
void quicksort_iterativo(int arr[], int tamanio);
```

### 5. Quicksort de Tres Vías
**Especialidad**: Eficiente con muchos elementos duplicados
```c
void quicksort_tres_vias(int arr[], int bajo, int alto);
```

## Tests

El proyecto incluye una suite completa de tests usando Criterion:

### Categorías de Tests
- **Funcionalidad básica**: Arrays pequeños, casos límite
- **Variantes de partición**: Verificación de todas las estrategias de pivot
- **Optimizaciones**: Tests de versiones híbrida e iterativa
- **Manejo de duplicados**: Quicksort de tres vías
- **Rendimiento**: Tests con arrays grandes
- **Casos límite**: Arrays vacíos, nulos, índices inválidos
- **Utilidades**: Funciones auxiliares y de análisis

### Ejecutar Tests

```bash
# Tests básicos
make run_tests

# Tests con output detallado
make run_tests_detailed

# Test específico
./test_quicksort --filter quicksort_basico
```

## Optimizaciones Implementadas

### 1. Selección Inteligente de Pivot
- **Mediana de tres**: Mejora significativa en arrays parcialmente ordenados
- **Pivot aleatorio**: Evita casos patológicos

### 2. Quicksort Híbrido
- Cambia a Insertion Sort para arrays pequeños (típicamente < 10 elementos)
- Mejora la constante del algoritmo

### 3. Quicksort de Tres Vías
- Maneja eficientemente arrays con muchos duplicados
- Complejidad O(n) en el mejor caso (todos los elementos iguales)

### 4. Implementación Iterativa
- Evita overhead de recursión
- Usa pila explícita para simular llamadas recursivas

## Análisis de Complejidad

### Complejidad Temporal
- **Mejor caso**: O(n log n) - pivot siempre divide el array por la mitad
- **Caso promedio**: O(n log n) - con buenas estrategias de pivot
- **Peor caso**: O(n²) - array ya ordenado con primer/último elemento como pivot

### Complejidad Espacial
- **Versión recursiva**: O(log n) - profundidad de la pila de recursión
- **Versión iterativa**: O(log n) - pila explícita
- **In-place**: No requiere memoria adicional proporcional al tamaño del array

## Casos de Uso

### Cuándo Usar Quicksort
- ✅ Arrays grandes con datos aleatorios
- ✅ Cuando se necesita ordenamiento in-place
- ✅ Implementaciones de bibliotecas estándar
- ✅ Cuando el rendimiento promedio es más importante que el peor caso

### Cuándo NO Usar Quicksort
- ❌ Cuando se requiere estabilidad en el ordenamiento
- ❌ Arrays pequeños (mejor usar Insertion Sort)
- ❌ Cuando se debe garantizar O(n log n) en el peor caso (mejor usar Heapsort o Mergesort)

## Variantes Avanzadas

### Quicksort Dual-Pivot
- Usa dos pivots en lugar de uno
- Implementado en Java 7+ para arrays de primitivas

### Quicksort Paralelo
- Divide el trabajo entre múltiples threads
- Eficiente en sistemas multi-core

### Quickselect
- Encuentra el k-ésimo elemento más pequeño
- Complejidad promedio O(n)

## Recursos de Aprendizaje

### Conceptos Relacionados
- **Algoritmos divide y vencerás**
- **Análisis de algoritmos**
- **Teoría de la complejidad**
- **Estructuras de datos (pilas)**

### Referencias
- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein
- "The Algorithm Design Manual" - Steven Skiena
- "Algorithms" - Robert Sedgewick, Kevin Wayne

## Aplicaciones Prácticas

### En la Vida Real
- **Bibliotecas estándar**: Base de `qsort()` en C y `sort()` en C++
- **Bases de datos**: Ordenamiento de índices y consultas
- **Sistemas operativos**: Ordenamiento de procesos y recursos
- **Gráficos por computadora**: Ordenamiento de primitivas por profundidad

### Proyectos Relacionados
- Implementación de algoritmos de búsqueda
- Sistemas de ordenamiento híbridos
- Análisis comparativo de algoritmos
- Optimización de consultas en bases de datos

## Notas de Implementación

### Consideraciones de Rendimiento
- La elección del pivot es crucial para el rendimiento
- La versión híbrida es típicamente la más rápida en la práctica
- El manejo de duplicados puede mejorar significativamente casos específicos

### Consideraciones de Memoria
- El algoritmo opera in-place
- La versión recursiva puede causar stack overflow en arrays muy grandes
- La versión iterativa es más robusta para arrays grandes

