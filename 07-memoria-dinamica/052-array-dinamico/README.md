# Ejercicio 052: Array Dinámico

## 📋 Descripción

Este ejercicio implementa un **array dinámico** completo en C, enseñando los conceptos fundamentales de gestión de memoria dinámica, redimensionamiento automático y operaciones sobre estructuras de datos dinámicas.

## 🎯 Objetivos de Aprendizaje

- **Memoria dinámica**: Uso correcto de `malloc()`, `realloc()` y `free()`
- **Estructuras de datos dinámicas**: Implementación de arrays que crecen automáticamente
- **Gestión de capacidad**: Diferencia entre tamaño y capacidad en estructuras dinámicas
- **Operaciones sobre arrays**: Inserción, eliminación, búsqueda y manipulación
- **Validación de entrada**: Verificación de parámetros y manejo de errores
- **Algoritmos básicos**: Ordenamiento, búsqueda y estadísticas

## 🚀 Funcionalidades

### Funciones Básicas
- `crearArrayDinamico()`: Crear array con capacidad inicial
- `liberarArrayDinamico()`: Liberar memoria correctamente
- `inicializarConValor()`: Inicializar todos los elementos
- `mostrarArray()`: Visualizar contenido del array

### Operaciones de Modificación
- `agregarAlFinal()`: Agregar elemento con redimensionamiento automático
- `insertarElemento()`: Insertar en posición específica
- `eliminarElemento()`: Eliminar elemento desplazando el resto
- `redimensionarArray()`: Cambiar capacidad manualmente

### Operaciones de Consulta
- `buscarElemento()`: Búsqueda lineal con índice de retorno
- `obtenerMaximo()`: Elemento máximo del array
- `obtenerMinimo()`: Elemento mínimo del array
- `calcularPromedio()`: Promedio aritmético de los elementos
- `contarElementos()`: Contar ocurrencias de un valor

### Operaciones Avanzadas
- `invertirArray()`: Invertir orden de elementos in-place
- `ordenarArray()`: Ordenamiento burbuja ascendente
- `copiarArray()`: Crear copia independiente
- `concatenarArrays()`: Unir dos arrays en uno nuevo

### Utilidades
- `mostrarEstadisticas()`: Información completa del array
- `validarArray()`: Verificar integridad de la estructura

## 🔧 Estructura de Datos

```c
typedef struct {
    int *datos;        // Puntero a los datos del array
    size_t tamaño;     // Número actual de elementos
    size_t capacidad;  // Espacio total asignado
} ArrayDinamico;
```

### Conceptos Clave

- **Tamaño vs Capacidad**: El tamaño es el número de elementos válidos, la capacidad es el espacio total asignado
- **Redimensionamiento**: Cuando se agota la capacidad, se duplica automáticamente
- **Factor de Carga**: Relación entre tamaño y capacidad para optimizar memoria

## 📊 Ejemplo de Uso

```c
#include "array_dinamico.h"

int main() {
    // Crear array con capacidad inicial de 5
    ArrayDinamico *array = crearArrayDinamico(5);
    
    // Agregar elementos (redimensiona automáticamente si es necesario)
    agregarAlFinal(array, 10);
    agregarAlFinal(array, 20);
    agregarAlFinal(array, 30);
    
    // Insertar en posición específica
    insertarElemento(array, 1, 15);  // [10, 15, 20, 30]
    
    // Operaciones de consulta
    int maximo = obtenerMaximo(array);           // 30
    int indice = buscarElemento(array, 15);      // 1
    double promedio = calcularPromedio(array);   // 18.75
    
    // Operaciones avanzadas
    invertirArray(array);  // [30, 20, 15, 10]
    ordenarArray(array);   // [10, 15, 20, 30]
    
    // Mostrar estadísticas
    mostrarEstadisticas(array);
    
    // Liberar memoria
    liberarArrayDinamico(array);
    
    return 0;
}
```

## 🔍 Conceptos Técnicos

### 1. Gestión de Memoria Dinámica

```c
// Asignación inicial
array->datos = malloc(capacidad * sizeof(int));

// Redimensionamiento
int *nuevos_datos = realloc(array->datos, nueva_capacidad * sizeof(int));

// Liberación
free(array->datos);
free(array);
```

### 2. Redimensionamiento Automático

- **Estrategia**: Duplicar capacidad cuando se llena
- **Ventaja**: Amortiza el costo de redimensionamiento
- **Factor**: Cada redimensionamiento dobla la capacidad

### 3. Desplazamiento de Elementos

```c
// Inserción: mover elementos hacia la derecha
for (size_t i = array->tamaño; i > indice; i--) {
    array->datos[i] = array->datos[i - 1];
}

// Eliminación: mover elementos hacia la izquierda
for (size_t i = indice; i < array->tamaño - 1; i++) {
    array->datos[i] = array->datos[i + 1];
}
```

## 📈 Complejidad Algorítmica

| Operación | Mejor Caso | Caso Promedio | Peor Caso |
|-----------|------------|---------------|-----------|
| Agregar al final | O(1) | O(1) | O(n)* |
| Insertar | O(1)** | O(n) | O(n) |
| Eliminar | O(1)** | O(n) | O(n) |
| Buscar | O(1)** | O(n) | O(n) |
| Acceso por índice | O(1) | O(1) | O(1) |

*Cuando requiere redimensionamiento  
**En la última posición

## 🚨 Buenas Prácticas

### Gestión de Memoria
- ✅ Siempre verificar retorno de `malloc()` y `realloc()`
- ✅ Liberar memoria con `free()` cuando ya no se necesite
- ✅ Establecer punteros a `NULL` después de liberar
- ✅ Usar `size_t` para tamaños y índices

### Validación de Entrada
- ✅ Verificar parámetros antes de usar
- ✅ Manejar casos límite (arrays vacíos, índices inválidos)
- ✅ Retornar códigos de error apropiados

### Eficiencia
- ✅ Redimensionar de forma inteligente (duplicar capacidad)
- ✅ Minimizar movimientos de memoria innecesarios
- ✅ Usar `memcpy()` para copiar bloques grandes

## 🎯 Casos de Uso Reales

1. **Buffers de datos**: Almacenar datos de tamaño variable
2. **Listas de elementos**: Interfaces de usuario dinámicas
3. **Cachés**: Almacenamiento temporal con crecimiento automático
4. **Parsers**: Acumular tokens o elementos durante análisis
5. **Algoritmos de ordenamiento**: Trabajar con conjuntos dinámicos

## 🔄 Extensiones Posibles

- **Arrays genéricos**: Usar `void*` para cualquier tipo de dato
- **Arrays 2D dinámicos**: Matrices que crecen en ambas dimensiones
- **Iteradores**: Interfaz estándar para recorrer elementos
- **Callbacks**: Funciones personalizadas para comparación y procesamiento
- **Serialización**: Guardar y cargar arrays desde archivos

## 📚 Recursos Adicionales

- [Dynamic Arrays in C](https://en.wikipedia.org/wiki/Dynamic_array)
- [Memory Management in C](https://www.cprogramming.com/tutorial/c/lesson6.html)
- [C Dynamic Data Structures](https://www.geeksforgeeks.org/dynamic-data-structures/)

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/array_dinamico
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
