# Ejercicio 068: Árbol Binario de Búsqueda (BST)

## 📋 Descripción

Este ejercicio implementa un **Árbol Binario de Búsqueda (Binary Search Tree - BST)** completo en C, una estructura de datos fundamental que mantiene elementos ordenados y permite operaciones eficientes de búsqueda, inserción y eliminación.

## 🎯 Objetivos de Aprendizaje

- Comprender la estructura y propiedades de un BST
- Implementar operaciones básicas: inserción, búsqueda, eliminación
- Dominar algoritmos de recorrido de árboles
- Manejar casos especiales en eliminación de nodos
- Optimizar operaciones con recursión
- Calcular métricas del árbol (altura, nodos, hojas)
- Validar la integridad del BST

## 🏗️ Estructura del Proyecto

```
068-arbol-binario-busqueda/
├── src/
│   └── arbol_bst.c           # Implementación principal
├── include/
│   └── arbol_bst.h           # Declaraciones y definiciones
├── tests/
│   └── test_arbol_bst.c      # Tests exhaustivos
├── CMakeLists.txt            # Configuración de build
├── README.md                 # Este archivo
└── .gitignore               # Archivos a ignorar
```

## 🔧 Funcionalidades Implementadas

### Operaciones Básicas
- **Creación y destrucción** del BST
- **Inserción** de elementos (sin duplicados)
- **Búsqueda** de elementos
- **Eliminación** de elementos (tres casos: hoja, un hijo, dos hijos)

### Recorridos del Árbol
- **In-order**: Elementos en orden ascendente
- **Pre-order**: Raíz, izquierdo, derecho
- **Post-order**: Izquierdo, derecho, raíz

### Operaciones Avanzadas
- **Encontrar mínimo y máximo**
- **Calcular altura del árbol**
- **Contar nodos totales**
- **Contar hojas**
- **Validar estructura BST**
- **Sumar todos los elementos**
- **Visualización de estructura**
- **Estadísticas del árbol**

## 🧠 Conceptos Teóricos

### ¿Qué es un BST?

Un **Árbol Binario de Búsqueda** es una estructura de datos jerárquica donde:

1. Cada nodo tiene como máximo dos hijos
2. Para cada nodo, todos los valores del subárbol izquierdo son menores
3. Todos los valores del subárbol derecho son mayores
4. No se permiten valores duplicados

### Propiedades del BST

```
       50
      /  \
    30    70
   / \   / \
  20 40 60 80
```

- **Búsqueda**: O(log n) en promedio, O(n) en el peor caso
- **Inserción**: O(log n) en promedio, O(n) en el peor caso
- **Eliminación**: O(log n) en promedio, O(n) en el peor caso
- **Recorrido in-order**: Produce secuencia ordenada

### Casos de Eliminación

1. **Nodo hoja**: Simplemente eliminar
2. **Un hijo**: Reemplazar por el hijo
3. **Dos hijos**: Reemplazar por sucesor in-order (o predecesor)

## 💻 Uso del Código

### Compilación

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar con CMake
cmake ..

# Compilar
make

# Ejecutar tests
make run_tests

# Ejecutar programa demo
./arbol_bst_demo
```

### Ejemplo de Uso

```c
#include "arbol_bst.h"

int main() {
    // Crear árbol
    BST *arbol = bst_crear();
    
    // Insertar elementos
    bst_insertar(arbol, 50);
    bst_insertar(arbol, 30);
    bst_insertar(arbol, 70);
    bst_insertar(arbol, 20);
    bst_insertar(arbol, 40);
    
    // Buscar elemento
    if (bst_buscar(arbol, 30)) {
        printf("Elemento 30 encontrado\n");
    }
    
    // Mostrar en orden
    printf("Recorrido in-order: ");
    bst_recorrido_inorder(arbol);
    
    // Eliminar elemento
    bst_eliminar(arbol, 30);
    
    // Mostrar estadísticas
    bst_mostrar_estadisticas(arbol);
    
    // Liberar memoria
    bst_destruir(arbol);
    
    return 0;
}
```

## 🔍 API Disponible

### Gestión del BST
```c
BST* bst_crear(void);                    // Crear BST vacío
void bst_destruir(BST *bst);             // Liberar memoria
```

### Operaciones Básicas
```c
bool bst_insertar(BST *bst, int dato);   // Insertar elemento
bool bst_buscar(BST *bst, int dato);     // Buscar elemento
bool bst_eliminar(BST *bst, int dato);   // Eliminar elemento
```

### Recorridos
```c
void bst_recorrido_inorder(BST *bst);    // Recorrido in-order
void bst_recorrido_preorder(BST *bst);   // Recorrido pre-order
void bst_recorrido_postorder(BST *bst);  // Recorrido post-order
```

### Operaciones Avanzadas
```c
Nodo* bst_encontrar_minimo(Nodo *nodo);  // Encontrar mínimo
Nodo* bst_encontrar_maximo(Nodo *nodo);  // Encontrar máximo
int bst_altura(BST *bst);                // Calcular altura
int bst_contar_nodos(BST *bst);          // Contar nodos
int bst_contar_hojas(BST *bst);          // Contar hojas
bool bst_es_valido(BST *bst);            // Validar BST
int bst_suma_elementos(BST *bst);        // Sumar elementos
```

### Utilidades
```c
void bst_imprimir_estructura(BST *bst);  // Visualizar estructura
void bst_mostrar_estadisticas(BST *bst); // Mostrar estadísticas
```

## 🧪 Tests Incluidos

### Cobertura de Tests
- ✅ Operaciones básicas (crear, insertar, buscar, eliminar)
- ✅ Casos límite (árbol vacío, un elemento, duplicados)
- ✅ Eliminación (hoja, un hijo, dos hijos, raíz)
- ✅ Recorridos (in-order, pre-order, post-order)
- ✅ Operaciones avanzadas (altura, conteos, validación)
- ✅ Árboles degenerados (izquierdo, derecho)
- ✅ Manejo de errores (punteros NULL)
- ✅ Pruebas de rendimiento (inserción/eliminación masiva)
- ✅ Verificación de integridad y memory leaks

### Ejecutar Tests
```bash
# Tests básicos
make run_tests

# Tests detallados con output
make test_detailed

# Verificar memory leaks (requiere Valgrind)
make test_memory

# Análisis estático (requiere Cppcheck)
make static_analysis
```

## 📊 Complejidad Algoritmica

| Operación | Mejor Caso | Caso Promedio | Peor Caso |
|-----------|------------|---------------|-----------|
| Búsqueda  | O(log n)   | O(log n)      | O(n)      |
| Inserción | O(log n)   | O(log n)      | O(n)      |
| Eliminación| O(log n)   | O(log n)      | O(n)      |
| Recorrido | O(n)       | O(n)          | O(n)      |

**Nota**: El peor caso O(n) ocurre cuando el árbol está degenerado (como una lista enlazada).

## 🎯 Casos de Uso Prácticos

### 1. Sistema de Búsqueda
```c
// Búsqueda eficiente en conjunto ordenado
BST *indices = bst_crear();
// Insertar IDs de productos
// Búsqueda rápida por ID
```

### 2. Validación de Rangos
```c
// Verificar si valor está en rango válido
bool en_rango(BST *bst, int min, int max) {
    // Usar recorrido in-order para verificar
}
```

### 3. Ordenamiento
```c
// Usar BST para ordenar elementos
// Insertar todos los elementos
// Recorrido in-order da secuencia ordenada
```

### 4. Autocompletado
```c
// BST de strings para sugerencias
// Búsqueda por prefijos
```

## ⚠️ Consideraciones y Limitaciones

### Ventajas
- Búsqueda, inserción y eliminación eficientes
- Recorrido in-order produce secuencia ordenada
- Estructura natural para datos jerárquicos
- Fácil de implementar y entender

### Desventajas
- Puede degenerarse en lista enlazada
- No auto-balanceante (para eso están AVL, Red-Black)
- Rendimiento depende del orden de inserción
- Uso de memoria mayor que arrays

### Mejoras Posibles
- Implementar auto-balanceo (AVL, Red-Black)
- Soporte para tipos de datos genéricos
- Iteradores para recorridos
- Persistencia en archivos
- Operaciones de conjunto (unión, intersección)

## 🔧 Herramientas de Desarrollo

### Debugging
```bash
# Compilar con debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
gdb ./arbol_bst_demo
```

### Profiling
```bash
# Analizar rendimiento
valgrind --tool=callgrind ./test_arbol_bst
```

### Memory Check
```bash
# Verificar memory leaks
valgrind --leak-check=full ./test_arbol_bst
```

## 📚 Recursos Adicionales

### Lecturas Recomendadas
- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein
- "Data Structures and Algorithm Analysis in C" - Mark Allen Weiss
- "The Art of Computer Programming, Volume 3" - Donald Knuth

### Conceptos Relacionados
- **Árboles AVL**: BST auto-balanceados
- **Árboles Red-Black**: Otra forma de balanceo
- **B-Trees**: Para sistemas de archivos
- **Tries**: Para búsqueda de strings
- **Heaps**: Para colas de prioridad

### Extensiones Posibles
- Implementar iterator pattern
- Soporte para duplicados
- Serialización/deserialización
- Operaciones de conjunto
- Visualización gráfica del árbol

## 🤝 Contribución

Este ejercicio es parte de una serie educativa. Las mejoras sugeridas incluyen:

1. **Optimizaciones**: Algoritmos más eficientes
2. **Funcionalidades**: Nuevas operaciones
3. **Tests**: Casos adicionales
4. **Documentación**: Ejemplos más claros
5. **Portabilidad**: Soporte multi-plataforma
