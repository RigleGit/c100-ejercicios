# Ejercicio 067: Cola (Queue) con Lista Enlazada

## Descripción

Este ejercicio implementa una **cola (queue)** usando una lista enlazada. Una cola es una estructura de datos que sigue el principio **FIFO (First In, First Out)** - el primer elemento en entrar es el primero en salir.

## Conceptos Clave

### Cola (Queue)
- **Definición**: Estructura de datos lineal que permite insertar elementos por un extremo (final) y eliminar por el otro (frente)
- **Principio FIFO**: First In, First Out
- **Operaciones principales**:
  - `enqueue`: Insertar elemento al final de la cola
  - `dequeue`: Retirar elemento del frente de la cola
  - `front`: Consultar elemento del frente sin retirarlo
  - `back`: Consultar elemento del final sin retirarlo

### Lista Enlazada como Base
- **Ventajas**: Tamaño dinámico, operaciones O(1) en ambos extremos
- **Implementación**: Mantiene punteros al frente y al final
- **Eficiencia**: Todas las operaciones principales son O(1)

### Diferencia con Pila (Stack)
- **Cola (FIFO)**: El primero en entrar es el primero en salir
- **Pila (LIFO)**: El último en entrar es el primero en salir

## Funcionalidades Implementadas

### 1. Estructura Básica
```c
typedef struct Nodo {
    int valor;
    struct Nodo* siguiente;
} Nodo;

typedef struct {
    Nodo* frente;          // Primer elemento (para dequeue)
    Nodo* final;           // Último elemento (para enqueue)
    size_t tamaño;
    size_t capacidad_maxima;
} Cola;
```

### 2. Operaciones Principales
- **Crear/Destruir**: Gestión de memoria
- **Enqueue**: Insertar elemento al final
- **Dequeue**: Retirar elemento del frente
- **Front**: Consultar elemento del frente
- **Back**: Consultar elemento del final
- **Estado**: Verificar si está vacía/llena

### 3. Operaciones Avanzadas
- **Clonar**: Crear una copia independiente
- **Invertir**: Invertir el orden de todos los elementos
- **Rotar**: Rotar n elementos hacia adelante
- **Combinar**: Unir dos colas en una nueva

### 4. Versión Simple
Implementación básica que maneja solo punteros al frente y final:
```c
ColaSimple cola = {NULL, NULL};
enqueue_simple(&cola, valor);
dequeue_simple(&cola, &valor);
```

### 5. Aplicaciones Prácticas
- **Sistema de atención al cliente**
- **Buffer de impresión**
- **Scheduler FIFO de procesos**
- **Algoritmo BFS (Breadth-First Search)**

## Estructura del Proyecto

```
067-cola-lista-enlazada/
├── include/
│   └── cola_lista.h          # Declaraciones y documentación
├── src/
│   └── cola_lista.c          # Implementación completa
├── tests/
│   └── test_cola_lista.c     # Tests exhaustivos con Criterion
├── CMakeLists.txt            # Configuración de compilación
├── README.md                 # Este archivo
└── .gitignore               # Archivos a ignorar por git
```

## Compilación y Ejecución

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./cola_lista

# Ejecutar tests (si Criterion está disponible)
./test_cola_lista
make test
```

### Compilación Manual
```bash
# Programa principal
gcc -std=c11 -Wall -Wextra -g src/cola_lista.c -o cola_lista

# Tests (requiere Criterion)
gcc -std=c11 -Wall -Wextra -g tests/test_cola_lista.c src/cola_lista.c -lcriterion -o test_cola_lista
```

## Ejemplos de Uso

### Ejemplo Básico
```c
// Crear cola
Cola* cola = crear_cola(0); // Capacidad ilimitada

// Insertar elementos al final
enqueue(cola, 10);
enqueue(cola, 20);
enqueue(cola, 30);

// Consultar extremos
int valor;
front(cola, &valor); // valor = 10 (frente)
back(cola, &valor);  // valor = 30 (final)

// Retirar elementos del frente (FIFO)
dequeue(cola, &valor); // valor = 10
dequeue(cola, &valor); // valor = 20
dequeue(cola, &valor); // valor = 30

// Limpiar
destruir_cola(cola);
```

### Versión Simple
```c
ColaSimple cola = {NULL, NULL};

enqueue_simple(&cola, 10);
enqueue_simple(&cola, 20);

int valor;
dequeue_simple(&cola, &valor); // valor = 10 (FIFO)

destruir_cola_simple(&cola);
```

### Simulación de Atención al Cliente
```c
// Sistema básico de atención
Cola* cola_espera = crear_cola(0);

// Clientes llegan
enqueue(cola_espera, 1); // Cliente 1
enqueue(cola_espera, 2); // Cliente 2
enqueue(cola_espera, 3); // Cliente 3

// Se atienden en orden FIFO
int cliente;
dequeue(cola_espera, &cliente); // Atiende Cliente 1
dequeue(cola_espera, &cliente); // Atiende Cliente 2
```

## Análisis de Complejidad

| Operación | Complejidad Temporal | Complejidad Espacial |
|-----------|---------------------|----------------------|
| enqueue() | O(1) | O(1) |
| dequeue() | O(1) | O(1) |
| front() | O(1) | O(1) |
| back() | O(1) | O(1) |
| esta_vacia() | O(1) | O(1) |
| obtener_tamaño() | O(1) | O(1) |
| invertir_cola() | O(n) | O(n) |
| rotar_cola() | O(k) | O(k) |
| clonar_cola() | O(n) | O(n) |
| contiene_valor() | O(n) | O(1) |

## Ventajas vs Desventajas

### Ventajas de Lista Enlazada
- ✅ Tamaño dinámico
- ✅ Operaciones principales O(1)
- ✅ No hay desperdicio de memoria
- ✅ Acceso directo a ambos extremos

### Desventajas vs Array Circular
- ❌ Overhead de punteros
- ❌ Peor localidad de referencia
- ❌ Fragmentación de memoria

## Casos de Uso Reales

### 1. Sistemas de Atención
```
Cliente 1 → Cliente 2 → Cliente 3 → Cliente 4
FRENTE                                FINAL
(siguiente en ser atendido)    (último en llegar)
```

### 2. Buffers de Comunicación
- Buffers de red
- Buffers de impresión
- Pipes entre procesos

### 3. Algoritmos de Grafos
- **BFS (Breadth-First Search)**: Usa cola para explorar por niveles
- **Algoritmos de caminos mínimos**

### 4. Sistemas Operativos
- **Scheduler FIFO**: Procesos se ejecutan en orden de llegada
- **Buffers de dispositivos I/O**

## Comparación: Cola vs Pila

| Aspecto | Cola (FIFO) | Pila (LIFO) |
|---------|-------------|-------------|
| **Inserción** | Al final | Al tope |
| **Eliminación** | Del frente | Del tope |
| **Orden** | Primero en entrar, primero en salir | Último en entrar, primero en salir |
| **Casos de uso** | Atención al cliente, BFS | Call stack, DFS |
| **Analogía** | Fila de banco | Pila de platos |

## Tests Incluidos

El archivo `test_cola_lista.c` incluye:
- ✅ Tests de operaciones básicas (enqueue, dequeue, front, back)
- ✅ Tests de cola con capacidad limitada
- ✅ Tests de operaciones avanzadas (clonar, invertir, rotar)
- ✅ Tests de funciones simples
- ✅ Tests de operaciones de consulta
- ✅ Tests de casos límite y validación
- ✅ Tests de conversiones y utilidades
- ✅ Tests de operaciones funcionales (filtrar, transformar)

## Simulaciones Incluidas

### 1. Atención al Cliente
Simula un sistema donde los clientes llegan y son atendidos en orden FIFO.

### 2. Buffer de Impresión
Simula una cola de documentos esperando ser impresos.

### 3. Scheduler FIFO
Compara el comportamiento FIFO con schedulers por prioridad.

### 4. Algoritmo BFS
Demuestra cómo se usa una cola en búsqueda por anchura.

## Notas Pedagógicas

### Para Estudiantes
1. **Entiende FIFO**: La cola siempre procesa el primer elemento primero
2. **Visualiza**: Imagina una fila en el banco - el primero en llegar es el primero en ser atendido
3. **Compara**: Contrasta con pila (LIFO) para entender las diferencias
4. **Practica**: Traza manualmente las operaciones en papel

### Conceptos Relacionados
- **Pila (Stack)**: Estructura LIFO complementaria
- **Deque**: Cola de doble extremo
- **Buffer circular**: Implementación alternativa con array
- **BFS vs DFS**: Cola vs Pila en algoritmos de grafos

## Aplicaciones en Algoritmos

### BFS (Breadth-First Search)
```c
Cola* cola = crear_cola(0);
enqueue(cola, nodo_inicial);

while (!esta_vacia(cola)) {
    int nodo_actual;
    dequeue(cola, &nodo_actual);
    
    // Procesar nodo_actual
    // Agregar vecinos no visitados a la cola
}
```

### Buffer de Productor-Consumidor
```c
// Productor agrega elementos
enqueue(buffer, nuevo_elemento);

// Consumidor retira elementos
dequeue(buffer, &elemento_procesado);
```

## Extensiones Posibles

1. **Cola genérica**: Usar `void*` para cualquier tipo
2. **Cola thread-safe**: Agregar sincronización
3. **Cola con prioridades**: Heap para ordenamiento automático
4. **Deque**: Cola de doble extremo
5. **Cola persistente**: Guardar/cargar desde archivo

## Referencias

- Cormen, T. et al. "Introduction to Algorithms" - Capítulo sobre Colas
- Documentación sobre estructuras de datos fundamentales
- Algoritmos BFS y aplicaciones de colas
- Estándares de C11 para manejo de memoria
