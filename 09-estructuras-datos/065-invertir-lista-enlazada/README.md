# Ejercicio 065: Invertir Lista Enlazada

## Descripción

Este ejercicio implementa algoritmos para **invertir una lista enlazada simple** sin crear una nueva lista. El objetivo es reordenar los punteros de los nodos existentes para invertir su orden, utilizando punteros auxiliares (prev, actual, siguiente) que permiten recorrer la lista y modificar el enlace `next` de cada nodo de forma correcta.

La inversión de listas enlazadas es un **patrón algorítmico fundamental** que refuerza el manejo avanzado de punteros y estructuras dinámicas. Es útil en múltiples contextos, desde algoritmos de manipulación de listas hasta transformaciones en estructuras de datos más complejas.

## Conceptos Clave

### 1. Inversión Iterativa
- **Complejidad**: O(n) tiempo, O(1) espacio
- **Principio**: Usar tres punteros para invertir enlaces uno por uno
- **Ventajas**: Eficiente en memoria, fácil de entender

### 2. Inversión Recursiva
- **Complejidad**: O(n) tiempo, O(n) espacio (call stack)
- **Principio**: Resolver recursivamente e invertir enlaces en el retorno
- **Ventajas**: Elegante, conceptualmente clara

### 3. Inversión de Sublistas
- **Principio**: Invertir solo una porción específica de la lista
- **Aplicaciones**: Algoritmos de ordenamiento, rotaciones

### 4. Inversión por Grupos
- **Principio**: Invertir grupos de k nodos consecutivos
- **Aplicaciones**: Algoritmos especializados, procesamiento en bloques

## Implementación

### Estructura Principal

```c
typedef struct Nodo {
    int valor;
    struct Nodo* siguiente;
} Nodo;

typedef struct {
    Nodo* cabeza;
    Nodo* cola;
    size_t longitud;
} Lista;
```

### Algoritmo Básico (Iterativo)

```c
bool invertir_lista(Lista* lista) {
    if (!lista || !lista->cabeza) return true;
    
    Nodo* prev = NULL;
    Nodo* actual = lista->cabeza;
    Nodo* siguiente = NULL;
    
    // Guardar la cola original como nueva cabeza
    lista->cola = lista->cabeza;
    
    while (actual != NULL) {
        siguiente = actual->siguiente;  // Guardar siguiente nodo
        actual->siguiente = prev;       // Invertir enlace
        prev = actual;                  // Mover prev y actual
        actual = siguiente;
    }
    
    lista->cabeza = prev; // Nueva cabeza
    return true;
}
```

## Funcionalidades Implementadas

### Básicas
- ✅ **Inversión completa**: Invertir toda la lista
- ✅ **Versión simple**: Usando solo puntero a cabeza
- ✅ **Validación de integridad**: Verificar estructura después de inversión

### Avanzadas
- ✅ **Inversión de sublistas**: Invertir rango específico de posiciones
- ✅ **Inversión recursiva**: Implementación recursiva elegante
- ✅ **Inversión por grupos**: Invertir grupos de k nodos consecutivos
- ✅ **Copia invertida**: Crear nueva lista invertida sin modificar original
- ✅ **Funciones auxiliares**: Comparación, validación, visualización

### Educativas
- ✅ **Visualización paso a paso**: Mostrar proceso de inversión
- ✅ **Análisis de rendimiento**: Comparar diferentes métodos
- ✅ **Casos especiales**: Manejar listas vacías, un elemento, etc.
- ✅ **Demostración comparativa**: Iterativo vs recursivo

## Casos de Uso

### 1. Lista Normal
```
Original: 1 -> 2 -> 3 -> 4 -> 5
Invertida: 5 -> 4 -> 3 -> 2 -> 1
```

### 2. Sublista
```
Original: 1 -> 2 -> 3 -> 4 -> 5 -> 6
Invertir [1,3]: 1 -> 4 -> 3 -> 2 -> 5 -> 6
```

### 3. Por Grupos (k=2)
```
Original: 1 -> 2 -> 3 -> 4 -> 5 -> 6
Resultado: 2 -> 1 -> 4 -> 3 -> 6 -> 5
```

### 4. Casos Especiales
- **Lista vacía**: No hay cambios
- **Un elemento**: Permanece igual
- **Dos elementos**: Intercambio simple

## Análisis de Complejidad

| Operación | Tiempo | Espacio | Características |
|-----------|--------|---------|----------------|
| Inversión iterativa | O(n) | O(1) | Eficiente en memoria |
| Inversión recursiva | O(n) | O(n) | Usa call stack |
| Inversión sublista | O(n) | O(1) | Navegación + inversión |
| Inversión por grupos | O(n) | O(1) | Múltiples inversiones locales |
| Crear copia invertida | O(n) | O(n) | Preserva original |

## Compilación y Ejecución

### Compilar
```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar Programa Principal
```bash
./invertir_lista
```

### Ejecutar Tests
```bash
# Todos los tests
make test

# Tests específicos
make test_basico          # Tests básicos
make test_sublista        # Tests de sublistas  
make test_recursivo       # Tests recursivos
make test_grupos          # Tests por grupos
make test_avanzado        # Tests avanzados

# Con salida detallada
make test_verbose
```

### Verificación de Memoria
```bash
make test_memory    # Tests con Valgrind
make run_memory     # Programa con Valgrind
```

## Casos de Prueba

### Tests Básicos
- Inversión de lista normal
- Lista vacía
- Un solo elemento
- Dos elementos
- Doble inversión (restaurar original)

### Tests de Sublistas
- Inversión en el medio
- Inversión desde inicio
- Inversión hasta final
- Sublista de un elemento
- Rangos inválidos

### Tests Recursivos
- Comparación iterativo vs recursivo
- Casos base de recursión
- Listas de diferentes tamaños

### Tests por Grupos
- Grupos completos
- Grupos incompletos
- k mayor que lista
- Grupos de tamaño 1

### Tests Avanzados
- Crear copia invertida
- Validación de integridad
- Comparación de listas
- Múltiples inversiones

## Aplicaciones Prácticas

### 1. Algoritmos de Ordenamiento
- **Merge Sort**: Combinar listas ordenadas
- **Quick Sort**: Reorganizar particiones

### 2. Manipulación de Datos
- **Reverso de secuencias**: Invertir orden de procesamiento
- **Undo/Redo**: Sistemas de historial

### 3. Algoritmos de Grafos
- **DFS**: Procesamiento en orden inverso
- **Topological Sort**: Ordenamiento topológico

### 4. Algoritmos Especializados
- **Palindrome Check**: Verificar simetrías
- **Rotation**: Rotaciones de listas

## Optimizaciones Implementadas

### 1. Manejo de Metadatos
- Actualización correcta de cabeza y cola
- Mantenimiento del contador de longitud
- Validación de integridad automática

### 2. Casos Especiales Optimizados
- Detección temprana de listas vacías o unitarias
- Manejo eficiente de sublistas completas
- Optimización para grupos incompletos

### 3. Prevención de Errores
- Validación de punteros NULL
- Verificación de rangos válidos
- Manejo seguro de memoria

## Patrones de Diseño Aplicados

### 1. Template Method
- Algoritmo base de inversión con variaciones
- Reutilización de lógica común

### 2. Strategy Pattern
- Diferentes estrategias de inversión
- Selección dinámica de algoritmo

### 3. Null Object Pattern
- Manejo seguro de listas NULL
- Comportamiento consistente

## Conceptos Educativos

### 1. Manipulación de Punteros
- **Triple puntero**: prev, actual, siguiente
- **Actualización atómica**: Cambio seguro de enlaces
- **Navegación bidireccional**: Avance y retroceso

### 2. Gestión de Estado
- **Invariantes**: Propiedades que se mantienen
- **Transiciones válidas**: Estados intermedios seguros
- **Validación continua**: Verificación de integridad

### 3. Recursión vs Iteración
- **Trade-offs**: Memoria vs elegancia
- **Casos base**: Condiciones de terminación
- **Stack overflow**: Límites de recursión

## Extensiones Posibles

### 1. Inversión Condicional
- Invertir solo nodos que cumplan criterio
- Mantener ciertos valores en posición fija

### 2. Inversión Alternativa
- Invertir alternativamente grupos de k nodos
- Patrones de inversión complejos

### 3. Inversión con Memoria
- Recordar estado original para deshacer
- Historial de transformaciones

### 4. Inversión Distribuida
- Inversión en paralelo de sublistas
- Algoritmos de inversión concurrente

## Recursos Adicionales

### Algoritmos Relacionados
- **Reverse Array**: Inversión de arrays
- **Reverse String**: Inversión de cadenas
- **Cycle Detection**: Detección de ciclos

### Estructuras Relacionadas
- **Doubly Linked List**: Listas doblemente enlazadas
- **Circular Linked List**: Listas circulares
- **Trees**: Inversión de árboles

### Referencias
- "Introduction to Algorithms" - CLRS
- "Data Structures and Algorithms" - Sedgewick
- "The Art of Computer Programming" - Knuth
