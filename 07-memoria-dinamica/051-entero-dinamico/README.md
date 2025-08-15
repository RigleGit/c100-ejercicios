# Ejercicio 051: Entero Dinámico

## Descripción

Este ejercicio introduce los conceptos fundamentales de gestión de memoria dinámica en C. Comienza con una implementación básica del enunciado original y extiende a técnicas avanzadas de manejo seguro de memoria.

**Enunciado Original**: Escribir un programa que reserve dinámicamente memoria para un entero usando `malloc`, le asigne un valor, imprima ese valor, libere la memoria con `free` y finalmente establezca el puntero a `NULL` para evitar referencias colgantes.

Este ejercicio proporciona una comprensión profunda de:

- Reserva de memoria dinámica con `malloc`
- Verificación de errores en allocación de memoria
- Uso seguro de memoria dinámica
- Liberación de memoria con `free`
- Prevención de memory leaks y referencias colgantes
- Buenas prácticas en gestión de memoria

## Implementación Básica

### Código del Enunciado Original

```c
void gestionBasica() {
    int *ptr = (int *)malloc(sizeof(int));

    if (ptr == NULL) {
        printf("Error al reservar memoria.\n");
        return;
    }

    *ptr = 2025;
    printf("Valor almacenado: %d\n", *ptr);

    free(ptr);
    ptr = NULL;
    
    printf("Memoria liberada y puntero establecido a NULL\n");
}
```

### Conceptos Clave de la Implementación Básica

1. **Reserva de Memoria**: `malloc(sizeof(int))` reserva memoria para un entero
2. **Verificación de Error**: Comprobar si `ptr == NULL` antes de usar
3. **Uso Normal**: Asignar y leer valores usando `*ptr`
4. **Liberación**: `free(ptr)` devuelve la memoria al sistema
5. **Anulación**: `ptr = NULL` previene uso accidental después de liberar

### Salida Esperada
```
Valor almacenado: 2025
Memoria liberada y puntero establecido a NULL
```

## Conceptos Fundamentales

### 1. Memoria Dinámica vs Estática

**Memoria Estática (stack)**:
- Variables locales, parámetros de función
- Gestionada automáticamente
- Tamaño fijo en tiempo de compilación
- Limitada en tamaño

**Memoria Dinámica (heap)**:
- Gestionada manualmente con `malloc`/`free`
- Tamaño determinado en tiempo de ejecución
- Mayor flexibilidad
- Requiere gestión explícita

### 2. Funciones de Gestión de Memoria

```c
void* malloc(size_t size)        // Reservar memoria
void free(void* ptr)             // Liberar memoria
void* calloc(size_t n, size_t s) // Reservar e inicializar a cero
void* realloc(void* ptr, size_t) // Redimensionar memoria
```

### 3. Errores Comunes

**Memory Leak**:
```c
int *ptr = malloc(sizeof(int));
ptr = NULL; // ¡ERROR! Memoria perdida sin liberar
```

**Use After Free**:
```c
int *ptr = malloc(sizeof(int));
free(ptr);
*ptr = 42; // ¡ERROR! Uso después de liberar
```

**Double Free**:
```c
int *ptr = malloc(sizeof(int));
free(ptr);
free(ptr); // ¡ERROR! Liberar dos veces
```

## Funciones Implementadas

### Función Básica

```c
void gestionBasica(void)
```
Implementación directa del enunciado original para gestión básica de memoria.

### Funciones Avanzadas

```c
int* crearEntero(int valor)
```
Crea un entero dinámico con verificaciones de error y información detallada.

```c
void mostrarEntero(int *ptr, const char *nombre)
```
Muestra información completa sobre un entero dinámico incluyendo dirección y representación.

```c
int modificarEntero(int *ptr, int nuevo_valor)
```
Modifica un entero dinámico con verificaciones de seguridad.

```c
void liberarEntero(int **ptr)
```
Libera memoria de forma segura estableciendo el puntero a NULL automáticamente.

### Funciones Educativas

```c
void demostrarErroresComunes(void)
```
Explica y demuestra errores típicos en gestión de memoria.

```c
void demostrarBuenasPracticas(void)
```
Muestra las mejores prácticas para gestión segura de memoria.

```c
void demostrarMultiplesEnteros(void)
```
Demuestra gestión de múltiples allocaciones dinámicas.

```c
void analizarUsoMemoria(void)
```
Proporciona análisis sobre uso y cálculo de memoria.

## Características Técnicas

### Verificación de Errores
```c
int *ptr = malloc(sizeof(int));
if (ptr == NULL) {
    fprintf(stderr, "Error: malloc falló\n");
    return NULL;
}
```

### Liberación Segura
```c
void liberarEntero(int **ptr) {
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}
```

### Inicialización
```c
// Siempre inicializar memoria allocada
*ptr = 0; // O valor apropiado
```

### Detección de Memory Leaks
Usar herramientas como `valgrind` para detectar leaks:
```bash
valgrind --leak-check=full ./entero_dinamico
```

## Buenas Prácticas

### 1. Verificación Inmediata
```c
int *ptr = malloc(sizeof(int));
if (ptr == NULL) {
    // Manejar error inmediatamente
    return ERROR_MEMORIA;
}
```

### 2. Inicialización
```c
*ptr = 0; // Inicializar memoria allocada
```

### 3. Liberación Inmediata
```c
// Liberar tan pronto como no se necesite
free(ptr);
ptr = NULL;
```

### 4. Función de Limpieza
```c
void cleanup(int **ptrs, int count) {
    for (int i = 0; i < count; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
}
```

### 5. Patrón RAII (Resource Acquisition Is Initialization)
```c
typedef struct {
    int *data;
} IntWrapper;

IntWrapper* crear_wrapper(int valor) {
    IntWrapper *w = malloc(sizeof(IntWrapper));
    if (w == NULL) return NULL;
    
    w->data = malloc(sizeof(int));
    if (w->data == NULL) {
        free(w);
        return NULL;
    }
    
    *(w->data) = valor;
    return w;
}

void destruir_wrapper(IntWrapper **w) {
    if (w != NULL && *w != NULL) {
        free((*w)->data);
        free(*w);
        *w = NULL;
    }
}
```

## Compilación y Ejecución

### Requisitos
- Compilador C (gcc, clang)
- CMake 3.16+
- Criterion (para tests)

### Instrucciones

```bash
# Crear directorio de construcción
mkdir build && cd build

# Configurar proyecto
cmake ..

# Compilar
make

# Ejecutar programa principal
./entero_dinamico

# Ejecutar tests
./test_entero_dinamico
```

## Ejemplo de Salida

```
=== IMPLEMENTACIÓN BÁSICA DEL ENUNCIADO ===
Valor almacenado: 2025
Memoria liberada y puntero establecido a NULL
============================================

=== Creando entero dinámico ===
Solicitando memoria para un entero...
Memoria reservada exitosamente
Dirección asignada: 0x7f8b4bc03710
Tamaño reservado: 4 bytes
Valor 42 asignado correctamente
==============================

=== Información de mi_entero ===
Dirección: 0x7f8b4bc03710
Valor: 42
Tamaño: 4 bytes
Valor en hexadecimal: 0x0000002A
Representación en bytes: 0x2A 0x00 0x00 0x00 
===============================
```

## Tests Incluidos

### Tests de Funcionalidad Básica
- ✅ Ejecución sin errores de la implementación básica
- ✅ Verificación manual del flujo malloc->uso->free->NULL
- ✅ Creación exitosa de enteros dinámicos
- ✅ Manejo de valores variados (positivos, negativos, extremos)

### Tests de Modificación
- ✅ Modificación exitosa de valores
- ✅ Manejo seguro de punteros nulos
- ✅ Verificación de valores después de modificación

### Tests de Liberación
- ✅ Liberación normal de memoria
- ✅ Doble liberación segura (no debe crashear)
- ✅ Liberación de punteros ya nulos

### Tests de Visualización
- ✅ Mostrar información de enteros válidos
- ✅ Manejo de punteros nulos en visualización

### Tests de Demostración
- ✅ Ejecución de demostraciones educativas
- ✅ Funciones de análisis de memoria
- ✅ Gestión de múltiples enteros dinámicos

### Tests de Integración
- ✅ Flujo completo: crear->modificar->mostrar->liberar
- ✅ Múltiples allocaciones y liberaciones (robustez)

## Herramientas de Debug

### Valgrind
```bash
# Detectar memory leaks
valgrind --leak-check=full --show-leak-kinds=all ./entero_dinamico

# Detectar errores de memoria
valgrind --tool=memcheck ./entero_dinamico
```

### AddressSanitizer
```bash
# Compilar con AddressSanitizer
gcc -fsanitize=address -g -o entero_dinamico src/main.c
./entero_dinamico
```

### Static Analysis
```bash
# Análisis estático con clang
clang --analyze src/main.c
```

## Consideraciones de Rendimiento

### 1. Overhead de malloc
- `malloc` tiene overhead por metadatos del heap
- Para allocaciones pequeñas frecuentes, considerar pool de memoria

### 2. Fragmentación
- Allocaciones/liberaciones frecuentes pueden fragmentar el heap
- Usar patrones de allocación consistentes

### 3. Alineación
- `malloc` garantiza alineación apropiada para cualquier tipo
- Para tipos específicos, la alineación puede ser más eficiente

Este ejercicio proporciona una base sólida para entender la gestión de memoria dinámica en C, fundamental para estructuras de datos avanzadas y programación de sistemas.
