# Ejercicio 055: Matriz Dinámica 2D

## Descripción

Este ejercicio demuestra la creación y manipulación de matrices bidimensionales usando memoria dinámica en C. Aprenderás a trabajar con punteros dobles para crear estructuras de datos matriciales flexibles en tiempo de ejecución.

## Conceptos Clave

### Matrices Dinámicas 2D
- **Puntero doble**: `int**` para apuntar a un array de punteros
- **Reserva por niveles**: Primero el array de punteros, luego cada fila
- **Liberación ordenada**: Liberar filas antes que el array de punteros

### Estrategias de Implementación
1. **Array de punteros**: Cada puntero apunta a una fila
2. **Bloque contiguo**: Toda la matriz en un solo bloque (avanzado)
3. **Matriz jagged**: Filas de diferentes tamaños

## Algoritmos Implementados

### Creación de Matriz
```c
int** crear_matriz_2d(int filas, int columnas) {
    // 1. Reservar array de punteros a filas
    int **matriz = malloc(filas * sizeof(int*));
    
    // 2. Reservar cada fila individualmente
    for (int i = 0; i < filas; i++) {
        matriz[i] = malloc(columnas * sizeof(int));
    }
    
    return matriz;
}
```

### Liberación Segura
```c
void liberar_matriz_2d(int** matriz, int filas) {
    // 1. Liberar cada fila
    for (int i = 0; i < filas; i++) {
        free(matriz[i]);
    }
    
    // 2. Liberar array de punteros
    free(matriz);
}
```

## Funcionalidades

### Básicas
- ✅ Crear matriz dinámica con dimensiones específicas
- ✅ Liberar memoria correctamente
- ✅ Acceso seguro a elementos
- ✅ Inicialización con valores

### Avanzadas
- ✅ Rellenado manual interactivo
- ✅ Rellenado automático (i*j)
- ✅ Rellenado con valores aleatorios
- ✅ Visualización formateada
- ✅ Estadísticas (suma, promedio, min/max)
- ✅ Búsqueda de valores
- ✅ Copia de matrices

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./matriz_dinamica_2d

# Ejecutar tests
make run_tests
```

### Usando GCC directamente
```bash
gcc -std=c99 -Wall -Wextra -Iinclude src/*.c -o matriz_dinamica_2d
./matriz_dinamica_2d
```

## Uso del Programa

### Programa Principal
El programa ofrece un menú interactivo:

```
=== Ejercicio 055: Matriz Dinámica 2D ===

Introduce el número de filas: 3
Introduce el número de columnas: 4

--- Opciones ---
1. Rellenar con valores manuales
2. Rellenar con valores automáticos (i*j)
3. Rellenar con valores aleatorios
4. Mostrar matriz
5. Obtener estadísticas
6. Buscar valor
0. Salir
```

### Ejemplos de Uso

#### Matriz 3x3 automática
```
Matriz [3x3]:
  0   0   0 
  0   1   2 
  0   2   4 

--- Estadísticas de la Matriz ---
Dimensiones: 3x3 (9 elementos)
Suma total: 9
Promedio: 1.00
Valor mínimo: 0 en posición [0][0]
Valor máximo: 4 en posición [2][2]
```

#### Búsqueda de valores
```
Buscando el valor 0 en la matriz:
Encontrado en posición [0][0]
Encontrado en posición [0][1]
Encontrado en posición [0][2]
Encontrado en posición [1][0]
Encontrado en posición [2][0]
Se encontraron 5 ocurrencias del valor 0.
```

## Tests

El proyecto incluye tests exhaustivos con Criterion:

```bash
# Ejecutar todos los tests
make run_tests

# Tests con salida detallada
make run_tests_verbose
```

### Tests Implementados
- ✅ Creación de matrices válidas
- ✅ Manejo de dimensiones inválidas
- ✅ Liberación segura de memoria
- ✅ Acceso y modificación de elementos
- ✅ Copia de matrices
- ✅ Tests de estrés con operaciones múltiples
- ✅ Tests parametrizados para diferentes tamaños

## Análisis de Complejidad

### Temporal
- **Creación**: O(filas + filas×columnas) = O(n×m)
- **Liberación**: O(filas)
- **Acceso a elemento**: O(1)
- **Rellenado**: O(filas×columnas)
- **Búsqueda**: O(filas×columnas)

### Espacial
- **Memoria total**: O(filas×columnas + filas) = O(n×m)
- **Overhead de punteros**: filas × sizeof(int*)
- **Fragmentación**: Posible debido a allocaciones separadas

## Consideraciones de Memoria

### Ventajas del Array de Punteros
- ✅ Implementación intuitiva
- ✅ Filas pueden tener tamaños diferentes (jagged arrays)
- ✅ Fácil de entender y debuggear

### Desventajas
- ❌ Mayor overhead de memoria (punteros extra)
- ❌ Posible fragmentación
- ❌ Peor localidad de referencia

### Alternativa: Bloque Contiguo
```c
// Reservar todo en un bloque
int *data = malloc(filas * columnas * sizeof(int));
int **matriz = malloc(filas * sizeof(int*));

// Configurar punteros a filas
for (int i = 0; i < filas; i++) {
    matriz[i] = data + i * columnas;
}
```

## Aplicaciones Prácticas

### Matemáticas y Ciencia
- Representación de matrices matemáticas
- Procesamiento de imágenes (píxeles como matriz)
- Grillas en simulaciones físicas
- Algoritmos de álgebra lineal

### Programación de Videojuegos
- Mapas de tiles en juegos 2D
- Tableros de juegos (ajedrez, damas)
- Grillas de pathfinding
- Heightmaps para terrenos

### Procesamiento de Datos
- Tablas de datos bidimensionales
- Matrices de distancias
- Tablas de contingencia
- Representación de grafos (matriz de adyacencia)

## Patrones Avanzados

### Matriz de Funciones
```c
typedef int (*operacion_t)(int, int);
operacion_t **matriz_ops = crear_matriz_funciones(filas, cols);
```

### Matriz de Estructuras
```c
typedef struct {
    int valor;
    char etiqueta[20];
} celda_t;

celda_t **matriz_celdas = crear_matriz_estructuras(filas, cols);
```

## Errores Comunes y Soluciones

### 1. Memory Leaks
```c
// ❌ Mal - no liberar filas
free(matriz);

// ✅ Bien - liberar todo
for (int i = 0; i < filas; i++) {
    free(matriz[i]);
}
free(matriz);
```

### 2. Acceso Fuera de Límites
```c
// ❌ Mal - no verificar límites
matriz[i][j] = valor;

// ✅ Bien - verificar límites
if (i >= 0 && i < filas && j >= 0 && j < columnas) {
    matriz[i][j] = valor;
}
```

### 3. Liberación en Orden Incorrecto
```c
// ❌ Mal - liberar matriz antes que filas
free(matriz);
for (int i = 0; i < filas; i++) {
    free(matriz[i]); // matriz ya liberada!
}
```

## Optimizaciones

### 1. Verificación de Alineación
```c
// Asegurar alineación para mejor performance
int **matriz = aligned_alloc(64, filas * sizeof(int*));
```

### 2. Pool de Memoria
```c
// Pre-asignar bloque grande y subdividir
static char memory_pool[1024*1024];
static size_t pool_offset = 0;
```

### 3. Cache-Friendly Access
```c
// Acceso por filas (mejor localidad)
for (int i = 0; i < filas; i++) {
    for (int j = 0; j < columnas; j++) {
        procesar(matriz[i][j]);
    }
}
```

## Recursos Adicionales

- [Dynamic Memory Allocation](https://en.cppreference.com/w/c/memory)
- [Two-dimensional arrays in C](https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/)
- [Memory Layout and Performance](https://stackoverflow.com/questions/2128728/what-is-the-difference-between-char-array-and-char-array)

