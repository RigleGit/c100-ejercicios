# Ejercicio 066: Pila (Stack) con Lista Enlazada

## Descripción

Este ejercicio implementa una **pila (stack)** usando una lista enlazada. Una pila es una estructura de datos que sigue el principio **LIFO (Last In, First Out)** - el último elemento en entrar es el primero en salir.

## Conceptos Clave

### Pila (Stack)
- **Definición**: Estructura de datos lineal que permite insertar y eliminar elementos solo por un extremo (la cima)
- **Principio LIFO**: Last In, First Out
- **Operaciones principales**:
  - `push`: Insertar elemento en la cima
  - `pop`: Retirar elemento de la cima
  - `peek/top`: Consultar elemento de la cima sin retirarlo

### Lista Enlazada como Base
- **Ventajas**: Tamaño dinámico, operaciones O(1)
- **Implementación**: La cima de la pila es el primer nodo de la lista
- **Eficiencia**: Todas las operaciones principales son O(1)

## Funcionalidades Implementadas

### 1. Estructura Básica
```c
typedef struct Nodo {
    int valor;
    struct Nodo* siguiente;
} Nodo;

typedef struct {
    Nodo* cima;
    size_t tamaño;
    size_t capacidad_maxima;
} Pila;
```

### 2. Operaciones Principales
- **Crear/Destruir**: Gestión de memoria
- **Push**: Insertar elemento en la cima
- **Pop**: Retirar elemento de la cima
- **Peek**: Consultar elemento de la cima
- **Estado**: Verificar si está vacía/llena

### 3. Operaciones Avanzadas
- **Duplicar cima**: Duplicar el elemento superior
- **Intercambiar**: Intercambiar dos elementos superiores
- **Rotar**: Rotar n elementos superiores
- **Invertir**: Invertir el orden de todos los elementos
- **Clonar**: Crear una copia independiente

### 4. Versión Simple
Implementación básica que maneja solo un puntero a la cima:
```c
Nodo* pila_simple = NULL;
push_simple(&pila_simple, valor);
pop_simple(&pila_simple, &valor);
```

### 5. Aplicaciones Prácticas
- **Evaluación de expresiones postfijas (RPN)**
- **Verificación de paréntesis balanceados**
- **Conversión decimal a binario**
- **Simulación de call stack**

## Ejemplos de Uso

### Ejemplo Básico
```c
// Crear pila
Pila* pila = crear_pila(0); // Capacidad ilimitada

// Insertar elementos
push(pila, 10);
push(pila, 20);
push(pila, 30);

// Consultar cima
int valor;
peek(pila, &valor); // valor = 30

// Retirar elementos (LIFO)
pop(pila, &valor); // valor = 30
pop(pila, &valor); // valor = 20
pop(pila, &valor); // valor = 10

// Limpiar
destruir_pila(pila);
```

### Versión Simple
```c
Nodo* pila = NULL;

push_simple(&pila, 10);
push_simple(&pila, 20);

int valor;
pop_simple(&pila, &valor); // valor = 20

destruir_pila_simple(&pila);
```

### Aplicación: Evaluar Expresión Postfija
```c
int resultado;
evaluar_postfija("3 4 +", &resultado); // resultado = 7
evaluar_postfija("15 7 1 1 + - /", &resultado); // resultado = 3
```

### Aplicación: Verificar Paréntesis
```c
bool balanceados = verificar_parentesis_balanceados("((a + b) * c)"); // true
```

## Análisis de Complejidad

| Operación | Complejidad Temporal | Complejidad Espacial |
|-----------|---------------------|----------------------|
| push() | O(1) | O(1) |
| pop() | O(1) | O(1) |
| peek() | O(1) | O(1) |
| esta_vacia() | O(1) | O(1) |
| duplicar_cima() | O(1) | O(1) |
| intercambiar() | O(1) | O(1) |
| invertir_pila() | O(n) | O(n) |
| clonar_pila() | O(n) | O(n) |
| contiene_valor() | O(n) | O(1) |

## Ventajas vs Desventajas

### Ventajas de Lista Enlazada
- ✅ Tamaño dinámico
- ✅ Operaciones principales O(1)
- ✅ No hay desperdicio de memoria
- ✅ Fácil de implementar

### Desventajas vs Array
- ❌ Overhead de punteros
- ❌ Peor localidad de referencia
- ❌ Fragmentación de memoria

## Casos de Uso Reales

1. **Compiladores**: Análisis sintáctico y semántico
2. **Navegadores**: Historial de páginas (botón "Atrás")
3. **Editores**: Operaciones de deshacer (Undo)
4. **Calculadoras**: Evaluación de expresiones
5. **Sistemas operativos**: Call stack de funciones
6. **Algoritmos**: Backtracking, DFS

## Notas Pedagógicas

### Para Estudiantes
1. **Entiende LIFO**: La pila siempre procesa el último elemento primero
2. **Visualiza**: Imagina una pila de platos - solo puedes tomar el de arriba
3. **Practica**: Traza manualmente las operaciones en papel
4. **Experimenta**: Modifica los valores y observa el comportamiento

### Conceptos Relacionados
- **Cola (Queue)**: Estructura FIFO complementaria
- **Recursión**: Usa implícitamente una pila (call stack)
- **Memoria**: Stack vs Heap en programación
- **Algoritmos**: DFS usa pila, BFS usa cola

## Extensiones Posibles

1. **Pila genérica**: Usar `void*` para cualquier tipo
2. **Pila thread-safe**: Agregar sincronización
3. **Persistencia**: Guardar/cargar pila desde archivo
4. **Visualización**: Interfaz gráfica para ver operaciones
5. **Optimizaciones**: Pool de nodos, lazy deletion

## Referencias

- Cormen, T. et al. "Introduction to Algorithms" - Capítulo sobre Pilas
- Documentación sobre estructuras de datos fundamentales
- Estándares de C11 para manejo de memoria

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/pila_lista
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
