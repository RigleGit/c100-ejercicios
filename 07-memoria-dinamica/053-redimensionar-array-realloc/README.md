# Ejercicio 053: Redimensionar Array con Realloc

## 📋 Descripción

Este ejercicio enseña el **redimensionamiento controlado de arrays dinámicos** usando `realloc()`, implementando una estrategia de crecimiento en bloques fijos que optimiza el balance entre memoria y rendimiento.

## 🎯 Objetivos de Aprendizaje

- **Función realloc()**: Uso correcto para redimensionar memoria dinámica
- **Crecimiento por bloques**: Estrategia de redimensionamiento en incrementos fijos
- **Gestión eficiente**: Balance entre memoria usada y redimensionamientos
- **Validación robusta**: Manejo de errores en asignación de memoria
- **Patrón de entrada**: Lectura hasta valor centinela (-1)
- **Análisis de rendimiento**: Evaluación de diferentes estrategias de crecimiento

## 🚀 Funcionalidades

### Implementación Básica (Enunciado)
```c
// Redimensionamiento simple en bloques de 10
int *datos = malloc(10 * sizeof(int));
if (cantidad == capacidad) {
    capacidad += 10;
    int *nuevo = realloc(datos, capacidad * sizeof(int));
    // ... manejo de errores y uso
}
```

### Implementación Avanzada
- **Estructura de datos**: ArrayRedimensionable con metadatos
- **Redimensionamiento inteligente**: Bloques configurables
- **Estrategias múltiples**: Fijo, porcentaje, adaptativo
- **Análisis de memoria**: Estadísticas detalladas de uso
- **Operaciones completas**: Inserción, eliminación, copia

## 🔧 Estructura de Datos

```c
typedef struct {
    int *datos;              // Puntero a los datos
    size_t cantidad;         // Elementos actuales
    size_t capacidad;        // Capacidad total
    size_t bloque_tamaño;    // Tamaño del bloque de crecimiento
} ArrayRedimensionable;
```

### Conceptos Clave

- **Crecimiento por bloques**: Evita redimensionamientos frecuentes
- **Factor de uso**: Relación entre elementos usados y capacidad
- **Estrategia de memoria**: Balance entre desperdicio y eficiencia
- **Análisis de patrones**: Seguimiento de redimensionamientos

## 📊 Ejemplo de Uso

### Implementación Básica
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *datos = NULL;
    int capacidad = 10, cantidad = 0, entrada;
    
    datos = malloc(capacidad * sizeof(int));
    
    printf("Introduce números (termina con -1):\n");
    while (1) {
        scanf("%d", &entrada);
        if (entrada == -1) break;
        
        // Redimensionar si es necesario
        if (cantidad == capacidad) {
            capacidad += 10;  // Bloque fijo de 10
            int *nuevo = realloc(datos, capacidad * sizeof(int));
            if (!nuevo) {
                printf("Error al redimensionar\n");
                free(datos);
                return 1;
            }
            datos = nuevo;
        }
        
        datos[cantidad++] = entrada;
    }
    
    // Mostrar resultados
    printf("Números introducidos: %d\n", cantidad);
    for (int i = 0; i < cantidad; i++) {
        printf("%d ", datos[i]);
    }
    
    free(datos);
    return 0;
}
```

### Implementación Avanzada
```c
#include "redimensionar_array.h"

int main() {
    // Crear array con bloque de 5 elementos
    ArrayRedimensionable *array = crearArrayRedimensionable(5);
    
    // Agregar elementos (redimensiona automáticamente)
    for (int i = 1; i <= 18; i++) {
        agregarElemento(array, i * 10);
        
        if (i % 5 == 0) {
            mostrarEstadisticas(array);
        }
    }
    
    // Análisis final
    analizarPatronCrecimiento(array);
    
    liberarArrayRedimensionable(array);
    return 0;
}
```

## 🔍 Análisis de Rendimiento

### Estrategias de Crecimiento

| Estrategia | Ventajas | Desventajas | Uso Recomendado |
|------------|----------|-------------|-----------------|
| **Bloques fijos pequeños (3-5)** | Menos desperdicio | Más redimensionamientos | Datos con tamaño predecible |
| **Bloques fijos grandes (20-50)** | Menos redimensionamientos | Más desperdicio | Datos con crecimiento rápido |
| **Crecimiento porcentual (150%)** | Balance adaptativo | Complejidad adicional | Casos generales |

### Análisis de Complejidad

| Operación | Mejor Caso | Caso Promedio | Peor Caso |
|-----------|------------|---------------|-----------|
| Agregar elemento | O(1) | O(1) | O(n)* |
| Redimensionar | O(n) | O(n) | O(n) |
| Insertar en posición | O(1)** | O(n) | O(n) |
| Acceso por índice | O(1) | O(1) | O(1) |

*Cuando requiere redimensionamiento  
**En la última posición

## 📈 Ejemplo de Crecimiento

```
Capacidad inicial: 5 elementos
Bloque de crecimiento: 5 elementos

Elementos -> Capacidad -> Redimensionamientos
0-5       -> 5          -> 0
6-10      -> 10         -> 1  
11-15     -> 15         -> 2
16-20     -> 20         -> 3

Factor de uso promedio: ~75%
```

## 🔄 Funciones Principales

### Gestión Básica
```c
ArrayRedimensionable* crearArrayRedimensionable(size_t bloque_inicial);
bool agregarElemento(ArrayRedimensionable *array, int valor);
void liberarArrayRedimensionable(ArrayRedimensionable *array);
```

### Redimensionamiento
```c
bool redimensionarArray(ArrayRedimensionable *array, size_t nueva_capacidad);
bool necesitaRedimensionar(const ArrayRedimensionable *array);
size_t calcularNuevaCapacidad(const ArrayRedimensionable *array);
```

### Análisis y Estadísticas
```c
void mostrarEstadisticas(const ArrayRedimensionable *array);
double calcularFactorUso(const ArrayRedimensionable *array);
void analizarPatronCrecimiento(const ArrayRedimensionable *array);
```

### Operaciones Avanzadas
```c
bool insertarEnPosicion(ArrayRedimensionable *array, size_t pos, int valor);
bool eliminarEnPosicion(ArrayRedimensionable *array, size_t pos);
ArrayRedimensionable* copiarArray(const ArrayRedimensionable *array);
void compactarArray(ArrayRedimensionable *array);
```

## 🚨 Aspectos Importantes de realloc()

### Uso Correcto
```c
// ✅ CORRECTO
int *nuevo = realloc(datos, nueva_capacidad * sizeof(int));
if (nuevo == NULL) {
    // Manejar error - datos original sigue válido
    printf("Error en redimensionamiento\n");
    free(datos); // Liberar memoria original
    return -1;
} else {
    datos = nuevo; // Actualizar puntero
}
```

### Errores Comunes
```c
// ❌ INCORRECTO - perdemos referencia original
datos = realloc(datos, nueva_capacidad * sizeof(int));
if (datos == NULL) {
    // ¡datos original se perdió!
}

// ❌ INCORRECTO - no verificar NULL
datos = realloc(datos, nueva_capacidad * sizeof(int));
datos[0] = 42; // Puede causar crash si realloc falló
```

### Casos Especiales
- `realloc(NULL, size)` equivale a `malloc(size)`
- `realloc(ptr, 0)` equivale a `free(ptr)` (no portable)
- `realloc()` puede mover el bloque a nueva dirección
- Los datos existentes se preservan hasta `min(viejo_tamaño, nuevo_tamaño)`

## 🎯 Casos de Uso Reales

1. **Buffers de entrada**: Leer datos de tamaño desconocido
2. **Listas dinámicas**: Implementar vectores redimensionables
3. **Parsers**: Acumular tokens durante análisis sintáctico
4. **Cachés adaptativos**: Almacenamiento que crece con demanda
5. **Logs dinámicos**: Sistemas de logging con crecimiento automático

## 🔄 Optimizaciones Avanzadas

### Estrategia Híbrida
```c
size_t calcularNuevaCapacidadHibrida(const ArrayRedimensionable *array) {
    if (array->capacidad < 100) {
        return array->capacidad + 10;  // Bloques fijos para arrays pequeños
    } else {
        return array->capacidad * 1.5; // Crecimiento porcentual para arrays grandes
    }
}
```

### Pre-asignación Inteligente
```c
void preasignarCapacidad(ArrayRedimensionable *array, size_t elementos_estimados) {
    size_t capacidad_necesaria = ((elementos_estimados + array->bloque_tamaño - 1) 
                                  / array->bloque_tamaño) * array->bloque_tamaño;
    redimensionarArray(array, capacidad_necesaria);
}
```

## 📚 Conceptos Relacionados

- **Vector dinámico**: Estructura de datos fundamental en muchos lenguajes
- **Amortized analysis**: Análisis de complejidad promedio de operaciones
- **Memory fragmentation**: Fragmentación causada por redimensionamientos
- **Geometric growth**: Crecimiento geométrico vs aritmético
- **Cache locality**: Impacto del acceso secuencial en rendimiento

## 🔄 Extensiones Posibles

- **Arrays tipados genéricos**: Usar macros o void* para cualquier tipo
- **Estrategias de shrinking**: Reducir capacidad cuando hay mucho desperdicio
- **Memory pools**: Usar pools de memoria para evitar fragmentación
- **Thread safety**: Hacer la estructura thread-safe con mutexes
- **Estadísticas avanzadas**: Tracking detallado de patrones de uso

## 📚 Recursos Adicionales

- [realloc() Documentation](https://man7.org/linux/man-pages/man3/realloc.3.html)
- [Dynamic Arrays - Wikipedia](https://en.wikipedia.org/wiki/Dynamic_array)
- [Amortized Analysis](https://en.wikipedia.org/wiki/Amortized_analysis)

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/redimensionar_array_realloc
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
