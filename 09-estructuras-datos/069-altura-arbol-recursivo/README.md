# Ejercicio 069: Altura de Árbol Binario Recursivo

## 📋 Descripción

Este ejercicio implementa el cálculo de la **altura de un árbol binario** usando **recursión**, una técnica fundamental en estructuras de datos jerárquicas. La altura se define como la longitud del camino más largo desde la raíz hasta una hoja.

## 🎯 Objetivos de Aprendizaje

- Dominar el concepto de **recursión** en árboles binarios
- Entender la definición matemática de **altura** de un árbol
- Implementar algoritmos recursivos para análisis estructural
- Comparar enfoques recursivos vs iterativos
- Analizar la complejidad de algoritmos recursivos
- Manejar casos base y casos recursivos correctamente
- Optimizar operaciones en estructuras jerárquicas

## 🏗️ Estructura del Proyecto

```
069-altura-arbol-recursivo/
├── src/
│   └── altura_arbol.c        # Implementación principal
├── include/
│   └── altura_arbol.h        # Declaraciones y definiciones
├── tests/
│   └── test_altura_arbol.c   # Tests exhaustivos
├── CMakeLists.txt            # Configuración de build
├── README.md                 # Este archivo
└── .gitignore               # Archivos a ignorar
```

## 🔧 Funcionalidades Implementadas

### Funciones Core de Altura
- **`altura_recursiva()`**: Cálculo recursivo de altura (función principal)
- **`arbol_altura()`**: Wrapper para el árbol completo
- **`altura_iterativa()`**: Versión iterativa para comparación
- **`altura_nodo()`**: Altura de un nodo específico

### Análisis Estructural
- **Factor de balance**: Diferencia entre subárboles
- **Verificación de balanceo**: ¿Está el árbol balanceado?
- **Alturas teóricas**: Mínima y máxima para n nodos
- **Profundidad de nodos**: Distancia desde la raíz

### Funciones de Análisis
- **Conteo por niveles**: Nodos en cada nivel
- **Nivel más poblado**: Nivel con más nodos
- **Conteo de hojas**: Nodos sin hijos
- **Nodos internos**: Nodos con al menos un hijo

### Visualización y Demos
- **Impresión estructural**: Árbol con alturas
- **Estadísticas completas**: Métricas del árbol
- **Recorrido por niveles**: BFS del árbol
- **Comparación de métodos**: Recursivo vs iterativo

## 🧠 Conceptos Teóricos

### ¿Qué es la Altura de un Árbol?

La **altura** de un árbol binario es:
- La longitud del camino más largo desde la raíz hasta una hoja
- Se cuenta en **número de aristas** (no nodos)
- Un árbol vacío tiene altura **0**
- Un árbol con un solo nodo tiene altura **1**

### Ejemplo Visual

```
        50        ← Nivel 0 (altura = 3)
       /  \
     30    70      ← Nivel 1
    / \   / \
   20 40 60 80     ← Nivel 2 (hojas)
```

**Altura = 3** (máximo de niveles)

### Fórmula Recursiva

```c
int altura(NodoArbol* raiz) {
    if (raiz == NULL)
        return 0;  // Caso base
    
    int altIzq = altura(raiz->izq);
    int altDer = altura(raiz->der);
    
    return 1 + max(altIzq, altDer);  // Caso recursivo
}
```

### Casos de Altura

1. **Árbol Vacío**: Altura = 0
2. **Un Nodo**: Altura = 1
3. **Árbol Balanceado**: Altura ≈ log₂(n)
4. **Árbol Degenerado**: Altura = n (como lista enlazada)

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
./altura_arbol_demo
```

### Ejemplo Básico

```c
#include "altura_arbol.h"

int main() {
    // Crear árbol
    ArbolBinario *arbol = arbol_crear();
    
    // Insertar elementos (BST)
    int valores[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    for (int i = 0; i < 9; i++) {
        arbol_insertar(arbol, valores[i]);
    }
    
    // Calcular altura
    int altura = arbol_altura(arbol);
    printf("La altura del árbol es: %d\n", altura);
    
    // Mostrar estadísticas
    mostrar_estadisticas_altura(arbol);
    
    // Liberar memoria
    arbol_destruir(arbol);
    return 0;
}
```

### Ejemplo Avanzado

```c
// Comparar métodos recursivo vs iterativo
int altura_rec = altura_recursiva(arbol->raiz);
int altura_iter = altura_iterativa(arbol->raiz);

// Analizar balanceo
bool balanceado = es_balanceado(arbol->raiz);
int factor = factor_balance(arbol->raiz);

// Obtener alturas teóricas
int min_teorica = altura_minima_teorica(arbol->tamanio);
int max_teorica = altura_maxima_teorica(arbol->tamanio);

// Análisis por niveles
for (int i = 0; i < altura_rec; i++) {
    int nodos = contar_nodos_nivel(arbol->raiz, i);
    printf("Nivel %d: %d nodos\n", i, nodos);
}
```

## 🔍 API Completa

### Funciones Básicas
```c
ArbolBinario* arbol_crear(void);         // Crear árbol vacío
void arbol_destruir(ArbolBinario* arbol); // Liberar memoria
bool arbol_insertar(ArbolBinario* arbol, int valor); // Insertar (BST)
```

### Funciones de Altura
```c
int arbol_altura(ArbolBinario* arbol);    // Altura del árbol
int altura_recursiva(NodoArbol* raiz);    // Recursiva (CORE)
int altura_iterativa(NodoArbol* raiz);    // Iterativa
int altura_nodo(ArbolBinario* arbol, int valor); // Altura de nodo específico
```

### Análisis Estructural
```c
int factor_balance(NodoArbol* raiz);      // Factor de balance
bool es_balanceado(NodoArbol* raiz);      // ¿Está balanceado?
int altura_minima_teorica(int n);         // Altura mínima para n nodos
int altura_maxima_teorica(int n);         // Altura máxima para n nodos
```

### Análisis por Niveles
```c
int contar_nodos_nivel(NodoArbol* raiz, int nivel);  // Nodos en nivel
int nivel_mas_poblado(ArbolBinario* arbol);          // Nivel con más nodos
int contar_hojas(NodoArbol* raiz);                   // Número de hojas
int contar_nodos_internos(NodoArbol* raiz);          // Nodos internos
```

### Utilidades
```c
int encontrar_profundidad(NodoArbol* raiz, int valor, int prof); // Profundidad
NodoArbol* buscar_nodo(NodoArbol* raiz, int valor);  // Buscar nodo
bool arbol_vacio(ArbolBinario* arbol);               // ¿Está vacío?
```

### Visualización
```c
void imprimir_arbol_con_altura(ArbolBinario* arbol); // Estructura visual
void mostrar_estadisticas_altura(ArbolBinario* arbol); // Estadísticas
void imprimir_por_niveles(ArbolBinario* arbol);      // Recorrido BFS
```

## 🧪 Tests Incluidos

### Cobertura de Tests
- ✅ **Casos básicos**: Árbol vacío, un nodo, múltiples nodos
- ✅ **Casos especiales**: Árboles degenerados, balanceados
- ✅ **Comparación de métodos**: Recursivo vs iterativo
- ✅ **Análisis estructural**: Balance, profundidad, alturas teóricas
- ✅ **Funciones auxiliares**: Conteos, búsquedas, utilidades
- ✅ **Límites y rendimiento**: Árboles grandes, casos extremos
- ✅ **Integridad**: Consistencia, invariantes, memory leaks

### Categorías de Tests
```bash
# Todos los tests
make run_tests

# Tests básicos de altura
make test_altura_basicos

# Tests de recursión
make test_recursion

# Casos especiales
make test_casos_especiales

# Memory leaks
make test_memory
```

## 📊 Complejidad Algorítmica

### Análisis de la Función Recursiva

| Aspecto | Complejidad | Descripción |
|---------|-------------|-------------|
| **Tiempo** | O(n) | Visita cada nodo una vez |
| **Espacio** | O(h) | Stack de recursión (h = altura) |
| **Mejor caso** | O(log n) espacio | Árbol balanceado |
| **Peor caso** | O(n) espacio | Árbol degenerado |

### Comparación de Métodos

| Método | Tiempo | Espacio | Ventajas | Desventajas |
|--------|--------|---------|----------|-------------|
| **Recursivo** | O(n) | O(h) | Elegante, fácil | Stack overflow |
| **Iterativo** | O(n) | O(w) | Sin recursión | Más complejo |

**Donde**: n = nodos, h = altura, w = ancho máximo

## 🎯 Casos de Uso Prácticos

### 1. Validación de Estructuras
```c
// Verificar si un BST está balanceado
bool validar_rendimiento(ArbolBinario *arbol) {
    int altura = arbol_altura(arbol);
    int min_teorica = altura_minima_teorica(arbol->tamanio);
    return altura <= 2 * min_teorica; // Razonablemente balanceado
}
```

### 2. Optimización de Búsquedas
```c
// Decidir si rebalancear el árbol
if (arbol_altura(arbol) > 2 * log2(arbol->tamanio)) {
    printf("Árbol necesita rebalanceo\n");
    // Aplicar rotaciones AVL o crear Red-Black tree
}
```

### 3. Análisis de Rendimiento
```c
// Métricas de calidad del árbol
void evaluar_calidad(ArbolBinario *arbol) {
    int altura = arbol_altura(arbol);
    int hojas = contar_hojas(arbol->raiz);
    float factor_ramificacion = (float)arbol->tamanio / altura;
    
    printf("Factor de ramificación: %.2f\n", factor_ramificacion);
    printf("Eficiencia: %s\n", factor_ramificacion > 1.5 ? "Buena" : "Mejorable");
}
```

### 4. Generación de Estructuras
```c
// Crear árbol con altura específica
ArbolBinario* crear_arbol_altura(int altura_deseada) {
    ArbolBinario *arbol = arbol_crear();
    
    // Insertar valores para lograr altura específica
    // Estrategia balanceada vs degenerada
    
    return arbol;
}
```

## ⚠️ Consideraciones Importantes

### Recursión y Stack Overflow
- **Problema**: Árboles muy profundos pueden causar stack overflow
- **Solución**: Usar versión iterativa para árboles grandes
- **Límite típico**: ~1000-10000 niveles según el sistema

### Definiciones de Altura
- **Este ejercicio**: Altura de árbol vacío = 0
- **Alternativa común**: Altura de árbol vacío = -1
- **Consistencia**: Mantener la misma definición en todo el código

### Optimizaciones Posibles
- **Memoización**: Cachear alturas calculadas
- **Lazy evaluation**: Calcular solo cuando sea necesario
- **Balance automático**: Mantener altura óptima con AVL/Red-Black

## 🔧 Herramientas de Desarrollo

### Debugging
```bash
# Compilar con debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
gdb ./altura_arbol_demo

# Verificar recursión
(gdb) set print pretty on
(gdb) break altura_recursiva
(gdb) run
```

### Profiling
```bash
# Habilitar profiling
cmake -DENABLE_PROFILING=ON ..
make profile_demo

# Analizar resultados
less profile_report.txt
```

### Memory Analysis
```bash
# Verificar leaks
make test_memory

# Análisis detallado
valgrind --tool=massif ./altura_arbol_demo
```

## 📈 Extensiones y Mejoras

### Funcionalidades Adicionales
1. **Cálculo incremental**: Actualizar altura al insertar/eliminar
2. **Altura ponderada**: Considerar pesos de nodos
3. **Múltiples métricas**: Diámetro, ancho, profundidad promedio
4. **Visualización gráfica**: Generar diagramas del árbol

### Optimizaciones Avanzadas
1. **Paralelización**: Calcular subárboles en paralelo
2. **SIMD**: Usar instrucciones vectoriales
3. **Cache-friendly**: Optimizar acceso a memoria
4. **Tail recursion**: Optimización del compilador

### Variantes de Árboles
1. **Árboles n-arios**: Generalizar a más de 2 hijos
2. **Árboles con padre**: Almacenar puntero al padre
3. **Árboles threaded**: Optimizar recorridos
4. **Árboles persistentes**: Versiones inmutables

## 📚 Recursos Adicionales

### Conceptos Relacionados
- **Algoritmos de balanceo**: AVL, Red-Black, Splay
- **Recorridos de árboles**: DFS, BFS, Morris
- **Árboles especializados**: Segment trees, Fenwick trees
- **Análisis de algoritmos**: Master theorem, recurrencias

### Lecturas Recomendadas
- "Introduction to Algorithms" - CLRS (Capítulo 12: Binary Search Trees)
- "Data Structures and Algorithm Analysis" - Mark Allen Weiss
- "Algorithms" - Sedgewick & Wayne (Capítulo 3: Binary Search Trees)

### Herramientas Útiles
- **Visualizadores**: VisuAlgo, Algorithm Visualizer
- **Generadores**: Random tree generators
- **Benchmarks**: Comparación con librerías estándar

## 🤝 Contribución

Este ejercicio forma parte de una serie educativa sobre estructuras de datos. Las mejoras sugeridas incluyen:

1. **Algoritmos**: Implementaciones más eficientes
2. **Tests**: Casos adicionales y edge cases
3. **Documentación**: Ejemplos más detallados
4. **Visualización**: Herramientas gráficas
5. **Interactividad**: Demos interactivos

