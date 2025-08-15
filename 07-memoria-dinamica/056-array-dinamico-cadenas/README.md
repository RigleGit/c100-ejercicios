# Ejercicio 056: Array Dinámico de Cadenas

## Descripción

Este ejercicio implementa un array dinámico de cadenas de caracteres que demuestra la gestión avanzada de memoria con múltiples niveles de punteros. Aprenderás a manejar arrays de punteros a cadenas, donde cada cadena tiene exactamente la memoria necesaria.

## Conceptos Clave

### Arrays de Punteros a Cadenas
- **Doble indirección**: `char**` para apuntar a un array de punteros `char*`
- **Memoria optimizada**: Cada cadena usa solo `strlen(cadena) + 1` bytes
- **Gestión por niveles**: Array de punteros + cadenas individuales

### Gestión de Memoria Dinámica
- **Reserva exacta**: Solo la memoria necesaria para cada cadena
- **Redimensionamiento**: Capacidad automática usando `realloc`
- **Liberación ordenada**: Primero las cadenas, luego el array de punteros

## Algoritmos Implementados

### Estructura Principal
```c
typedef struct {
    char **cadenas;     // Array de punteros a cadenas
    int size;           // Número actual de cadenas
    int capacidad;      // Capacidad máxima del array
} ArrayCadenas;
```

### Agregar Cadena con Redimensionamiento
```c
bool agregar_nombre(ArrayCadenas* array, const char* nombre) {
    // 1. Verificar si necesita redimensionar
    if (array->size >= array->capacidad) {
        int nueva_capacidad = array->capacidad * 2;
        char **nuevo_array = realloc(array->cadenas, 
                                   nueva_capacidad * sizeof(char*));
        array->cadenas = nuevo_array;
        array->capacidad = nueva_capacidad;
    }
    
    // 2. Reservar memoria exacta para la cadena
    array->cadenas[array->size] = malloc(strlen(nombre) + 1);
    strcpy(array->cadenas[array->size], nombre);
    array->size++;
    
    return true;
}
```

### Liberación Segura
```c
void liberar_array_cadenas(ArrayCadenas* array) {
    // 1. Liberar cada cadena individual
    for (int i = 0; i < array->size; i++) {
        free(array->cadenas[i]);
    }
    
    // 2. Liberar array de punteros
    free(array->cadenas);
    
    // 3. Liberar estructura
    free(array);
}
```

## Funcionalidades

### Básicas
- ✅ Crear array con capacidad inicial
- ✅ Agregar nombres con memoria exacta
- ✅ Redimensionamiento automático
- ✅ Liberación completa de memoria
- ✅ Mostrar lista de nombres

### Intermedias
- ✅ Búsqueda lineal de nombres
- ✅ Eliminación por índice con compactación
- ✅ Ordenamiento alfabético (burbuja optimizado)
- ✅ Obtener nombre por índice

### Avanzadas
- ✅ Estadísticas detalladas (longitudes, memoria)
- ✅ Filtrado por longitud de cadenas
- ✅ Copia independiente del array
- ✅ Búsqueda insensible a mayúsculas/minúsculas
- ✅ Interfaz interactiva completa

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./array_dinamico_cadenas

# Ejecutar tests
make run_tests

# Tests específicos de memoria
make run_tests_memory
```

### Usando GCC directamente
```bash
gcc -std=c99 -Wall -Wextra -Iinclude src/*.c -o array_dinamico_cadenas
./array_dinamico_cadenas
```

## Uso del Programa

### Programa Principal
El programa ofrece un menú interactivo completo:

```
=== Ejercicio 056: Array Dinámico de Cadenas ===

¿Cuántos nombres vas a introducir?: 3

Introduce 3 nombre(s):
Nombre 1: Ana
Nombre 2: Pedro
Nombre 3: Carlos

--- Opciones ---
1. Mostrar nombres
2. Ordenar alfabéticamente
3. Buscar nombre
4. Agregar nombre
5. Eliminar nombre
6. Estadísticas
7. Filtrar por longitud
8. Crear copia
0. Salir
```

### Ejemplos de Uso

#### Gestión Básica
```
Nombres (3/4):
 1. Ana
 2. Pedro
 3. Carlos

Introduce el nuevo nombre: Miguel
Nombre 'Miguel' agregado correctamente.

Nombres (4/4):
 1. Ana
 2. Pedro
 3. Carlos
 4. Miguel
```

#### Estadísticas Detalladas
```
--- Estadísticas del Array ---
Cantidad de nombres: 4
Capacidad del array: 4
Longitud total de caracteres: 19
Longitud promedio: 4.75 caracteres
Nombre más corto: "Ana" (3 caracteres)
Nombre más largo: "Carlos" (6 caracteres)

Memoria utilizada:
  - Nombres: 23 bytes
  - Punteros: 32 bytes
  - Estructura: 16 bytes
  - Total: 71 bytes
```

#### Filtrado por Longitud
```
Longitud mínima: 4
Longitud máxima: 6

Nombres filtrados (4-6 caracteres):
 1. Pedro
 2. Carlos
 3. Miguel
```

## Tests

El proyecto incluye una suite exhaustiva de tests con Criterion:

```bash
# Ejecutar todos los tests
make run_tests

# Tests con salida detallada
make run_tests_verbose

# Solo tests de gestión de memoria
make run_tests_memory
```

### Tests Implementados
- ✅ Creación y liberación de arrays
- ✅ Agregar nombres con validación
- ✅ Redimensionamiento automático
- ✅ Búsqueda y eliminación
- ✅ Ordenamiento alfabético
- ✅ Copia de arrays
- ✅ Filtrado por criterios
- ✅ Tests parametrizados para diferentes capacidades
- ✅ Tests de integración completos

## Análisis de Complejidad

### Temporal
- **Agregar nombre**: O(1) amortizado, O(n) en redimensionamiento
- **Buscar nombre**: O(n) búsqueda lineal
- **Eliminar por índice**: O(n) por compactación
- **Ordenar**: O(n²) burbuja, O(n) si ya está ordenado
- **Filtrar**: O(n) para crear nuevo array

### Espacial
- **Memoria nombres**: Σ(len(cadena_i) + 1)
- **Memoria punteros**: capacidad × sizeof(char*)
- **Memoria estructura**: sizeof(ArrayCadenas)
- **Total**: O(memoria_cadenas + capacidad)

## Consideraciones de Memoria

### Ventajas del Diseño
- ✅ **Memoria exacta**: Cada cadena usa solo lo necesario
- ✅ **Crecimiento dinámico**: Se adapta al número de elementos
- ✅ **Gestión automática**: Redimensionamiento transparente
- ✅ **Fragmentación mínima**: Liberación ordenada

### Optimizaciones Implementadas
- ✅ **Redimensionamiento exponencial**: Duplica capacidad (menos reallocaciones)
- ✅ **Liberación defensiva**: Verifica punteros NULL
- ✅ **Compactación eficiente**: Minimiza movimientos en eliminación
- ✅ **Burbuja optimizada**: Para con bandera de intercambio

### Patrones de Uso de Memoria
```c
// Ejemplo de uso de memoria para ["Ana", "Pedro", "Carlos"]
// 
// ArrayCadenas (16 bytes)
// ├── cadenas: ptr → [ptr1, ptr2, ptr3, NULL] (32 bytes)
// ├── size: 3
// └── capacidad: 4
//
// Cadenas individuales:
// ptr1 → "Ana\0"    (4 bytes)
// ptr2 → "Pedro\0"  (6 bytes)  
// ptr3 → "Carlos\0" (7 bytes)
//
// Total: 16 + 32 + 4 + 6 + 7 = 65 bytes
```

## Aplicaciones Prácticas

### Procesamiento de Texto
- Listas de palabras dinámicas
- Diccionarios en memoria
- Procesamiento de archivos de configuración
- Parsers de líneas de comando

### Bases de Datos en Memoria
- Índices de cadenas
- Cachés de consultas
- Tablas de símbolos
- Directorios de archivos

### Interfaces de Usuario
- Listas de opciones dinámicas
- Historial de comandos
- Autocompletado
- Menús contextuales

## Patrones Avanzados

### Array de Estructuras con Cadenas
```c
typedef struct {
    char *nombre;
    char *apellido;
    int edad;
} Persona;

// Array dinámico de personas
Persona **personas = malloc(n * sizeof(Persona*));
```

### Pool de Memoria para Cadenas
```c
typedef struct {
    char *pool;
    size_t usado;
    size_t tamaño;
} PoolCadenas;

// Todas las cadenas en un bloque contiguo
char *nueva_cadena = pool->pool + pool->usado;
```

### Hash Table de Cadenas
```c
typedef struct {
    ArrayCadenas **buckets;
    int num_buckets;
} HashTableCadenas;

// Usar array dinámico en cada bucket
```

## Errores Comunes y Soluciones

### 1. Memory Leaks en Cadenas
```c
// ❌ Mal - no liberar cadenas individuales
free(array->cadenas);
free(array);

// ✅ Bien - liberar todo
for (int i = 0; i < array->size; i++) {
    free(array->cadenas[i]);
}
free(array->cadenas);
free(array);
```

### 2. Buffer Overflow en Lectura
```c
// ❌ Mal - buffer sin límite
char buffer[100];
gets(buffer);  // Peligroso

// ✅ Bien - lectura segura
char buffer[MAX_LONGITUD];
fgets(buffer, MAX_LONGITUD, stdin);
buffer[strcspn(buffer, "\n")] = '\0';  // Quitar \n
```

### 3. Redimensionamiento Incorrecto
```c
// ❌ Mal - perder referencia en caso de error
array->cadenas = realloc(array->cadenas, nuevo_tamaño);

// ✅ Bien - verificar antes de asignar
char **temp = realloc(array->cadenas, nuevo_tamaño);
if (temp == NULL) {
    return false;  // Mantener array original
}
array->cadenas = temp;
```

### 4. Índices Fuera de Rango
```c
// ❌ Mal - no verificar límites
return array->cadenas[indice];

// ✅ Bien - verificar límites
if (indice < 0 || indice >= array->size) {
    return NULL;
}
return array->cadenas[indice];
```

## Optimizaciones Avanzadas

### 1. Ordenamiento Más Eficiente
```c
// Usar qsort para mejor performance
int comparar_cadenas(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

qsort(array->cadenas, array->size, sizeof(char*), comparar_cadenas);
```

### 2. Búsqueda Binaria (Array Ordenado)
```c
// O(log n) vs O(n) lineal
int buscar_binario(ArrayCadenas *array, const char *nombre) {
    int izq = 0, der = array->size - 1;
    while (izq <= der) {
        int medio = (izq + der) / 2;
        int cmp = strcmp(array->cadenas[medio], nombre);
        if (cmp == 0) return medio;
        else if (cmp < 0) izq = medio + 1;
        else der = medio - 1;
    }
    return -1;
}
```

### 3. Pool de Memoria para Reducir Fragmentación
```c
typedef struct {
    char *pool;
    size_t offset;
    size_t tamaño;
} MemoryPool;

char* allocar_desde_pool(MemoryPool *pool, size_t tamaño) {
    if (pool->offset + tamaño > pool->tamaño) {
        return NULL;  // Pool lleno
    }
    char *ptr = pool->pool + pool->offset;
    pool->offset += tamaño;
    return ptr;
}
```

## Recursos Adicionales

- [Dynamic Arrays in C](https://en.wikipedia.org/wiki/Dynamic_array)
- [String Handling in C](https://en.cppreference.com/w/c/string)
- [Memory Management Best Practices](https://stackoverflow.com/questions/6441218/can-a-local-variables-memory-be-accessed-outside-its-scope)
- [Understanding malloc and free](https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/)
