# Ejercicio 050: Punteros Void

## Descripción

Este ejercicio explora el uso de punteros `void*` en C, comenzando con la implementación básica del enunciado y progresando hacia funciones genéricas avanzadas que pueden trabajar con cualquier tipo de dato. Los punteros void son fundamentales para la programación genérica y la creación de APIs flexibles.

## Implementación Básica del Enunciado

### Código Fundamental
```c
int main() {
    int a = 42;
    float b = 3.14f;
    char c = 'Z';
    void *ptr;

    // Apuntar a un int
    ptr = &a;
    printf("Valor de a: %d\n", *(int *)ptr);

    // Apuntar a un float
    ptr = &b;
    printf("Valor de b: %.2f\n", *(float *)ptr);

    // Apuntar a un char
    ptr = &c;
    printf("Valor de c: %c\n", *(char *)ptr);

    return 0;
}
```

Esta implementación básica demuestra:
- **Asignación**: Un puntero `void*` puede recibir la dirección de cualquier tipo
- **Casting explícito**: Se requiere `(tipo*)ptr` para desreferenciar
- **Flexibilidad**: El mismo puntero puede apuntar a diferentes tipos secuencialmente

## Conceptos Clave

### 1. Punteros Void (`void*`)
Un puntero void es un puntero genérico que puede apuntar a cualquier tipo de dato, pero no puede ser desreferenciado directamente sin casting.

```c
void *ptr;              // Declaración de puntero void
int valor = 42;
ptr = &valor;           // Puede apuntar a cualquier tipo
int resultado = *(int*)ptr;  // Requiere casting para acceder
```

### 2. Ventajas de los Punteros Void
- **Genericidad**: Una función puede trabajar con cualquier tipo
- **Flexibilidad**: APIs que no dependen de tipos específicos
- **Reutilización**: El mismo código para diferentes tipos de datos
- **Abstracción**: Oculta detalles de implementación

### 3. Limitaciones
- **No hay aritmética directa**: Requiere casting a tipo conocido
- **Sin información de tipo**: El programador debe saber el tipo real
- **Sin verificación de tipos**: Propenso a errores si se usa incorrectamente

## Funciones Implementadas

### Función Básica del Enunciado

```c
void ejemploBasicoVoid(void)
```
Implementación didáctica que demuestra el casting de `void*` a diferentes tipos básicos (`int`, `float`, `char`).

### Funciones de Inspección

```c
void mostrarMemoria(void *ptr, size_t size, const char *tipo_nombre)
```
Muestra el contenido de memoria en formato hexadecimal, decimal y como caracteres.

### Funciones de Manipulación

```c
void copiarMemoria(void *destino, const void *origen, size_t size)
```
Implementación genérica de `memcpy` usando punteros void.

```c
int compararMemoria(const void *ptr1, const void *ptr2, size_t size)
```
Implementación genérica de `memcmp` para comparar bloques de memoria.

```c
void intercambiarMemoria(void *ptr1, void *ptr2, size_t size)
```
Intercambia el contenido de dos bloques de memoria de cualquier tipo.

```c
void llenarMemoria(void *ptr, int valor, size_t size)
```
Implementación genérica de `memset` para llenar memoria.

### Funciones de Búsqueda

```c
void* buscarPatron(const void *memoria, size_t size_memoria, 
                   const void *patron, size_t size_patron)
```
Busca un patrón de bytes específico en un bloque de memoria.

### Sistema de Callbacks

```c
typedef void (*procesarCallback)(void *dato, size_t size, const char *nombre);

void procesarArray(void *array, size_t num_elementos, size_t size_elemento,
                   procesarCallback callback, const char *tipo_nombre)
```
Sistema genérico para procesar arrays usando callbacks.

## Características Técnicas

### Casting Seguro
```c
// Siempre verificar antes de hacer casting
if (ptr != NULL && size == sizeof(int)) {
    int *valor = (int*)ptr;
    // Usar valor
}
```

### Aritmética de Punteros
```c
// Los punteros void requieren casting para aritmética
void *ptr = array;
void *elemento = (char*)ptr + (index * sizeof(tipo));
```

### Manejo de Errores
- Verificación de punteros nulos
- Validación de tamaños
- Protección contra accesos fuera de límites

### Implementación de Memoria
```c
// Acceso byte por byte usando unsigned char*
unsigned char *byte_ptr = (unsigned char*)ptr;
for (size_t i = 0; i < size; i++) {
    // Procesar byte_ptr[i]
}
```

## Casos de Uso Prácticos

### 1. Funciones de Biblioteca
```c
// malloc/free usan void*
void *malloc(size_t size);
void free(void *ptr);

// Funciones de memoria estándar
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);
```

### 2. Estructuras de Datos Genéricas
```c
// Lista enlazada genérica
struct nodo {
    void *datos;
    size_t size;
    struct nodo *siguiente;
};
```

### 3. APIs de Callback
```c
// qsort usa void* para ser genérica
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
```

### 4. Serialización de Datos
```c
// Escribir cualquier tipo a archivo
void escribir_binario(FILE *archivo, void *datos, size_t size) {
    fwrite(datos, size, 1, archivo);
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
./puntero_void

# Ejecutar tests
./test_puntero_void
```

## Ejemplo de Salida

```
=== EJEMPLO BÁSICO DEL ENUNCIADO ===
Demostración de puntero void* con tipos básicos

Variables originales:
int a = 42
float b = 3.14
char c = 'Z'

Usando void* para acceder a int:
  ptr = &a; (dirección: 0x7fff5fbff6ac)
  Valor de a: 42
  Casting: *(int *)ptr

Usando void* para acceder a float:
  ptr = &b; (dirección: 0x7fff5fbff6a8)
  Valor de b: 3.14
  Casting: *(float *)ptr

Usando void* para acceder a char:
  ptr = &c; (dirección: 0x7fff5fbff6a7)
  Valor de c: 'Z'
  Casting: *(char *)ptr

Conceptos clave demostrados:
1. void* puede apuntar a cualquier tipo
2. Requiere casting explícito para desreferenciar
3. El programador debe recordar el tipo correcto
4. Base para funciones genéricas como malloc()
```

## Tests Incluidos

### Tests de Funcionalidad Básica
- ✅ **Casting básico**: Conversión de `void*` a tipos básicos (`int`, `float`, `char`)
- ✅ **Ejecución del ejemplo**: Implementación del enunciado sin errores
- ✅ Manejo de punteros nulos
- ✅ Inspección de diferentes tipos
- ✅ Copia de datos simples y arrays
- ✅ Comparación de memoria

### Tests de Manipulación
- ✅ Intercambio de valores
- ✅ Llenado de memoria con patrones
- ✅ Validación de integridad de datos

### Tests de Búsqueda
- ✅ Búsqueda de patrones encontrados
- ✅ Búsqueda de patrones no encontrados
- ✅ Múltiples ocurrencias
- ✅ Manejo de parámetros inválidos

### Tests de Callbacks
- ✅ Procesamiento con callbacks
- ✅ Diferentes tipos de datos
- ✅ Arrays genéricos

### Tests de Integración
- ✅ Flujo completo de operaciones
- ✅ Combinación de múltiples funciones

## Patrones de Diseño

### 1. Función Factory Genérica
```c
void* crear_objeto(size_t size, void (*inicializar)(void*)) {
    void *obj = malloc(size);
    if (obj && inicializar) {
        inicializar(obj);
    }
    return obj;
}
```

### 2. Iterador Genérico
```c
void iterar(void *array, size_t count, size_t element_size,
            void (*funcion)(void*, size_t)) {
    char *ptr = (char*)array;
    for (size_t i = 0; i < count; i++) {
        funcion(ptr + i * element_size, element_size);
    }
}
```

### 3. Comparador Genérico
```c
typedef int (*comparar_func)(const void *a, const void *b);

void ordenar_generico(void *array, size_t count, size_t size,
                      comparar_func comparar) {
    // Implementación de ordenamiento usando void*
}
```

## Consideraciones de Seguridad

### 1. Verificación de Tipos
```c
// Siempre validar el tamaño esperado
if (size != sizeof(tipo_esperado)) {
    return ERROR_TIPO_INCORRECTO;
}
```

### 2. Alineación de Memoria
```c
// Considerar alineación para tipos específicos
void *ptr_alineado = (void*)(((uintptr_t)ptr + alignment - 1) & ~(alignment - 1));
```

### 3. Detección de Corrupción
```c
// Usar patrones conocidos para detectar corrupción
#define MAGIC_PATTERN 0xDEADBEEF
```

## Aplicaciones Avanzadas

### 1. Pool de Memoria Genérico
```c
struct memory_pool {
    void *memoria;
    size_t element_size;
    size_t total_elements;
    // Gestión de elementos libres
};
```

### 2. Sistema de Serialización
```c
struct serializer {
    void (*serialize)(const void *obj, size_t size, FILE *file);
    void (*deserialize)(void *obj, size_t size, FILE *file);
};
```

### 3. Cache Genérico
```c
struct cache_entry {
    void *key;
    void *value;
    size_t key_size;
    size_t value_size;
};
```

