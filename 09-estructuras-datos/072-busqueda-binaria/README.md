# Ejercicio 072: Algoritmo de Búsqueda Binaria

## Descripción

Este ejercicio implementa una versión completa y educativa del algoritmo de **Búsqueda Binaria**, uno de los algoritmos de búsqueda más fundamentales y eficientes para arrays ordenados. Incluye múltiples variantes, casos especiales y análisis de complejidad.

## Conceptos Clave

### Búsqueda Binaria
- **Algoritmo divide y vencerás**: Divide el espacio de búsqueda por la mitad en cada iteración
- **Prerequisito**: El array debe estar ordenado
- **Complejidad temporal**: O(log n)
- **Complejidad espacial**: O(1) iterativa, O(log n) recursiva
- **Principio**: Compara el elemento objetivo con el elemento medio y descarta la mitad irrelevante

### Ventajas
- ✅ Muy eficiente para arrays grandes
- ✅ Complejidad logarítmica garantizada
- ✅ Fácil de implementar y entender
- ✅ Funciona en arrays ordenados de cualquier tipo

### Limitaciones
- ❌ Requiere array ordenado
- ❌ No eficiente para arrays pequeños vs búsqueda lineal
- ❌ Acceso aleatorio necesario (no funciona en listas enlazadas)

## Funcionalidades Implementadas

### Implementaciones Básicas
- `busqueda_binaria_recursiva()`: Versión recursiva clásica
- `busqueda_binaria_iterativa()`: Versión iterativa (más eficiente en memoria)
- `busqueda_binaria_detallada()`: Con información de estadísticas

### Variantes para Duplicados
- `busqueda_primera_ocurrencia()`: Encuentra la primera aparición de un elemento
- `busqueda_ultima_ocurrencia()`: Encuentra la última aparición de un elemento
- `busqueda_todas_ocurrencias()`: Información completa sobre todas las ocurrencias

### Búsquedas Relacionadas
- `busqueda_ceiling()`: Encuentra el menor elemento ≥ objetivo
- `busqueda_floor()`: Encuentra el mayor elemento ≤ objetivo
- `encontrar_valor_mas_cercano()`: Encuentra el valor más próximo al objetivo

### Arrays Especiales
- `busqueda_array_rotado()`: Búsqueda en arrays ordenados que han sido rotados
- `encontrar_punto_rotacion()`: Encuentra el índice de rotación
- `busqueda_matriz_ordenada()`: Búsqueda en matrices ordenadas por filas y columnas
- `busqueda_tamanio_desconocido()`: Para arrays de tamaño indefinido

### Problemas Especiales
- `encontrar_pico()`: Encuentra elementos mayores que sus vecinos
- `encontrar_elemento_unico()`: En array con duplicados pares excepto uno
- `raiz_cuadrada_binaria()`: Calcula raíz cuadrada usando búsqueda binaria
- `raiz_k_binaria()`: Calcula k-ésima raíz usando búsqueda binaria

### Aplicaciones Prácticas
- `buscar_en_diccionario()`: Búsqueda en diccionarios ordenados
- `busqueda_en_rango()`: Encuentra elementos en un rango específico
- Análisis comparativo con búsqueda lineal

## Estructura del Proyecto

```
072-busqueda-binaria/
├── include/
│   └── busqueda_binaria.h    # Declaraciones y estructuras
├── src/
│   └── busqueda_binaria.c    # Implementación completa
├── tests/
│   └── test_busqueda_binaria.c # Tests exhaustivos con Criterion
├── CMakeLists.txt            # Configuración de compilación
├── README.md                # Esta documentación
└── .gitignore               # Archivos a ignorar por git
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
./busqueda_binaria

# Ejecutar tests
make run_tests

# Ejecutar tests por categoría
make run_tests_basicos
make run_tests_variantes
make run_tests_rendimiento
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/busqueda_binaria.c -lm -o busqueda_binaria

# Compilar y ejecutar tests
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/busqueda_binaria.c tests/test_busqueda_binaria.c -lcriterion -lm -o test_busqueda_binaria
./test_busqueda_binaria
```

## Uso

### Ejemplo Básico

```c
#include "busqueda_binaria.h"

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int tamanio = sizeof(arr) / sizeof(arr[0]);
    int objetivo = 7;
    
    // Búsqueda básica
    int posicion = busqueda_binaria_iterativa(arr, tamanio, objetivo);
    
    if (posicion != -1) {
        printf("Elemento %d encontrado en posición %d\n", objetivo, posicion);
    } else {
        printf("Elemento %d no encontrado\n", objetivo);
    }
    
    return 0;
}
```

### Ejemplo con Información Detallada

```c
int arr[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
int tamanio = sizeof(arr) / sizeof(arr[0]);
int objetivo = 12;

ResultadoBusqueda resultado = busqueda_binaria_detallada(arr, tamanio, objetivo);

if (resultado.encontrado) {
    printf("✓ Encontrado en posición %d\n", resultado.posicion);
} else {
    printf("✗ No encontrado\n");
}
printf("Comparaciones realizadas: %d\n", resultado.comparaciones);
```

### Ejemplo con Duplicados

```c
int arr[] = {1, 2, 2, 2, 3, 4, 4, 5};
int tamanio = sizeof(arr) / sizeof(arr[0]);
int objetivo = 2;

ResultadoBusquedaMultiple resultado = busqueda_todas_ocurrencias(arr, tamanio, objetivo);

if (resultado.primera_posicion != -1) {
    printf("Primera ocurrencia: posición %d\n", resultado.primera_posicion);
    printf("Última ocurrencia: posición %d\n", resultado.ultima_posicion);
    printf("Total de ocurrencias: %d\n", resultado.total_ocurrencias);
}
```

## Ejemplo Básico del Ejercicio

El ejercicio 072 incluye una implementación simple y directa de búsqueda binaria, perfecta para entender los conceptos fundamentales:

```c
#include "busqueda_binaria.h"

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    int n = sizeof(arr) / sizeof(arr[0]);
    int clave = 7;

    printf("Array:\n");
    imprimirArray_simple(arr, n);

    int indice = busquedaBinaria(arr, n, clave);
    if (indice != -1)
        printf("Elemento %d encontrado en la posición %d.\n", clave, indice);
    else
        printf("Elemento %d no encontrado.\n", clave);

    return 0;
}
```

### Función Simple de Búsqueda Binaria

```c
int busquedaBinaria(int arr[], int n, int clave) {
    int inicio = 0;
    int fin = n - 1;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;

        if (arr[medio] == clave)
            return medio;
        else if (arr[medio] < clave)
            inicio = medio + 1;
        else
            fin = medio - 1;
    }
    return -1;
}
```

## Algoritmos Implementados

### 0. Búsqueda Binaria Simple (Versión Básica del Ejercicio)
**Características**: Implementación directa y fácil de entender
```c
int busquedaBinaria(int arr[], int n, int clave);                    // Versión iterativa simple
int busquedaBinariaRecursiva_simple(int arr[], int inicio, int fin, int clave);  // Versión recursiva simple
```

### 1. Búsqueda Binaria Básica (Versión Avanzada)
**Complejidad**: O(log n)
```c
int busqueda_binaria_iterativa(int arr[], int tamanio, int objetivo);
int busqueda_binaria_recursiva(int arr[], int izquierda, int derecha, int objetivo);
```

### 2. Variantes para Duplicados
**Uso**: Arrays con elementos repetidos
```c
int busqueda_primera_ocurrencia(int arr[], int tamanio, int objetivo);
int busqueda_ultima_ocurrencia(int arr[], int tamanio, int objetivo);
```

### 3. Búsquedas Ceiling y Floor
**Aplicación**: Rangos de valores, aproximaciones
```c
int busqueda_ceiling(int arr[], int tamanio, int objetivo);  // Menor elemento ≥ objetivo
int busqueda_floor(int arr[], int tamanio, int objetivo);    // Mayor elemento ≤ objetivo
```

### 4. Array Rotado
**Caso especial**: Arrays ordenados que han sido rotados
```c
int busqueda_array_rotado(int arr[], int tamanio, int objetivo);
int encontrar_punto_rotacion(int arr[], int tamanio);
```

### 5. Problemas Matemáticos
**Aplicación**: Cálculos numéricos usando búsqueda binaria
```c
int raiz_cuadrada_binaria(int numero);
int raiz_k_binaria(int numero, int k);
```

## Tests

El proyecto incluye una suite completa de tests usando Criterion:

### Categorías de Tests
- **Funciones simples**: Versión básica del ejercicio
- **Búsqueda básica**: Funcionalidad fundamental avanzada
- **Variantes**: Primera/última ocurrencia, ceiling/floor
- **Arrays especiales**: Rotados, matrices, tamaño desconocido
- **Elementos especiales**: Picos, elementos únicos, raíces
- **Utilidades**: Funciones auxiliares
- **Comparación**: Búsqueda binaria vs lineal
- **Aplicaciones**: Casos de uso prácticos
- **Rendimiento**: Tests con arrays grandes
- **Casos límite**: Arrays vacíos, nulos, índices inválidos

### Ejecutar Tests

```bash
# Todos los tests
make run_tests

# Tests por categoría
make run_tests_basicos
make run_tests_variantes
make run_tests_rendimiento

# Test específico
./test_busqueda_binaria --filter busqueda_basica
```

## Análisis de Complejidad

### Complejidad Temporal
| Operación | Mejor Caso | Caso Promedio | Peor Caso |
|-----------|------------|---------------|-----------|
| Búsqueda básica | O(1) | O(log n) | O(log n) |
| Primera/última ocurrencia | O(log n) | O(log n) | O(log n) |
| Ceiling/Floor | O(log n) | O(log n) | O(log n) |
| Array rotado | O(log n) | O(log n) | O(log n) |

### Complejidad Espacial
- **Versión iterativa**: O(1) - espacio constante
- **Versión recursiva**: O(log n) - pila de recursión

### Comparación con Búsqueda Lineal
- **Búsqueda binaria**: O(log n) - máximo log₂(n) comparaciones
- **Búsqueda lineal**: O(n) - máximo n comparaciones
- **Mejora**: ~n/log(n) veces más rápida para arrays grandes

## Casos de Uso

### Cuándo Usar Búsqueda Binaria
- ✅ Arrays grandes y ordenados
- ✅ Búsquedas frecuentes en el mismo dataset
- ✅ Cuando el costo de ordenamiento se amortiza
- ✅ Problemas de optimización numérica

### Aplicaciones Reales
- **Bases de datos**: Índices B-tree
- **Sistemas operativos**: Búsqueda en tablas ordenadas
- **Bibliotecas estándar**: `bsearch()` en C, `binary_search()` en C++
- **Algoritmos numéricos**: Cálculo de raíces, aproximaciones
- **Compresión**: Búsqueda en tablas de codificación

## Variantes Avanzadas

### Búsqueda Ternaria
- Divide en tres partes en lugar de dos
- Útil para funciones unimodales
- Complejidad: O(log₃ n)

### Búsqueda Exponencial
- Para arrays de tamaño desconocido
- Encuentra límites exponencialmente
- Complejidad: O(log n)

### Búsqueda Interpolación
- Estima la posición basada en valor
- Mejor que binaria para datos uniformemente distribuidos
- Complejidad promedio: O(log log n)

## Optimizaciones Implementadas

### 1. Prevención de Overflow
```c
int medio = izquierda + (derecha - izquierda) / 2;  // En lugar de (izquierda + derecha) / 2
```

### 2. Versión Iterativa
- Evita overhead de recursión
- Más eficiente en memoria
- Robusta para arrays grandes

### 3. Búsquedas Especializadas
- Optimizaciones específicas para cada tipo de búsqueda
- Manejo eficiente de duplicados
- Casos especiales optimizados

## Recursos de Aprendizaje

### Conceptos Relacionados
- **Algoritmos divide y vencerás**
- **Análisis de complejidad**
- **Estructuras de datos ordenadas**
- **Algoritmos de ordenamiento**

### Referencias
- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein
- "Programming Pearls" - Jon Bentley
- "Algorithms" - Robert Sedgewick, Kevin Wayne

### Problemas Relacionados
- Búsqueda en arrays 2D ordenados
- K-ésimo elemento más pequeño
- Mediana de dos arrays ordenados
- Búsqueda en rangos infinitos

## Notas de Implementación

### Consideraciones de Rendimiento
- La versión iterativa es generalmente más rápida
- El manejo de duplicados añade complejidad constante
- Las búsquedas especializadas pueden ser más eficientes que múltiples búsquedas básicas

### Consideraciones de Memoria
- Versión recursiva: cuidado con stack overflow en arrays muy grandes
- Versión iterativa: uso mínimo de memoria
- Búsquedas múltiples: reutilizar resultados cuando sea posible

### Robustez
- Validación de parámetros de entrada
- Manejo de casos límite (arrays vacíos, nulos)
- Prevención de overflow en cálculos

