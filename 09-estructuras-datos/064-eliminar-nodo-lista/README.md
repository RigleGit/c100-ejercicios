# Ejercicio 064: Eliminar Nodo de Lista Enlazada

## Descripción

Este ejercicio extiende el ejercicio anterior de listas enlazadas implementando **operaciones de eliminación de nodos**. La eliminación es una operación fundamental en estructuras de datos dinámicas que requiere manejo cuidadoso de punteros y gestión correcta de memoria para mantener la integridad de la estructura.

## Conceptos Clave

### ¿Por qué es Importante la Eliminación?

La eliminación de nodos en listas enlazadas presenta varios desafíos únicos:

- **Reenlace de punteros**: Conectar correctamente los nodos restantes
- **Gestión de memoria**: Liberar la memoria del nodo eliminado
- **Casos especiales**: Manejar eliminación de cabeza, cola y único elemento
- **Integridad de estructura**: Mantener consistencia en metadatos (longitud, punteros)

### Complejidad de Operaciones de Eliminación

| Operación | Complejidad | Descripción |
|-----------|-------------|-------------|
| `eliminar_primero()` | O(1) | Acceso directo a la cabeza |
| `eliminar_ultimo()` | O(n) | Requiere encontrar penúltimo nodo |
| `eliminar_nodo(valor)` | O(n) | Búsqueda lineal + eliminación |
| `eliminar_en_posicion(pos)` | O(n) | Recorrido hasta posición |
| `eliminar_todos(valor)` | O(n) | Un recorrido, múltiples eliminaciones |

### Casos Especiales Críticos

1. **Lista vacía**: Operación debe fallar graciosamente
2. **Único elemento**: Cabeza y cola se vuelven NULL
3. **Eliminar cabeza**: Actualizar puntero de cabeza
4. **Eliminar cola**: Actualizar puntero de cola y penúltimo nodo
5. **Valor inexistente**: Operación sin efecto, retornar false

## Estructura de Archivos

```
064-eliminar-nodo-lista/
├── src/
│   └── eliminar_nodo.c        # Implementación principal
├── include/
│   └── eliminar_nodo.h        # Declaraciones y definiciones
├── tests/
│   └── test_eliminar_nodo.c   # Suite de tests con Criterion
├── CMakeLists.txt             # Configuración de compilación
└── README.md                  # Este archivo
```

## Implementación

### Estructura Base

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

### Funciones de Eliminación Implementadas

#### 1. Eliminación por Valor
```c
bool eliminar_nodo(Lista* lista, int valor);
```
**Algoritmo:**
1. Verificar casos especiales (lista vacía, NULL)
2. Si es la cabeza, usar `eliminar_primero()`
3. Recorrer lista manteniendo puntero anterior
4. Al encontrar valor: reenlazar anterior→siguiente
5. Actualizar cola si es necesario
6. Liberar memoria y decrementar longitud

#### 2. Eliminación por Posición
```c
bool eliminar_en_posicion(Lista* lista, size_t posicion);
```
**Casos especiales:**
- Posición 0: eliminar cabeza
- Posición >= longitud: fallar
- Posición válida: recorrer y eliminar

#### 3. Eliminación de Extremos
```c
bool eliminar_primero(Lista* lista);   // O(1)
bool eliminar_ultimo(Lista* lista);    // O(n)
```

#### 4. Eliminación Múltiple
```c
size_t eliminar_todos(Lista* lista, int valor);
void vaciar_lista(Lista* lista);
```

### Algoritmo Principal (eliminar_nodo)

```c
bool eliminar_nodo(Lista* lista, int valor) {
    if (!lista || !lista->cabeza) return false;
    
    // Caso especial: eliminar cabeza
    if (lista->cabeza->valor == valor) {
        return eliminar_primero(lista);
    }
    
    // Buscar en el resto de la lista
    Nodo* anterior = lista->cabeza;
    Nodo* actual = lista->cabeza->siguiente;
    
    while (actual) {
        if (actual->valor == valor) {
            // Reenlazar: anterior apunta al siguiente del actual
            anterior->siguiente = actual->siguiente;
            
            // Actualizar cola si eliminamos el último
            if (actual == lista->cola) {
                lista->cola = anterior;
            }
            
            // Liberar memoria y actualizar contador
            destruir_nodo(actual);
            lista->longitud--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    return false; // No encontrado
}
```

### Versión Simple (Compatibilidad con Enunciado)

```c
bool eliminar_nodo_simple(Nodo** cabeza, int valor) {
    if (!cabeza || !*cabeza) return false;
    
    Nodo* actual = *cabeza;
    Nodo* anterior = NULL;
    
    while (actual) {
        if (actual->valor == valor) {
            if (anterior == NULL) {
                // Eliminar cabeza
                *cabeza = actual->siguiente;
            } else {
                // Reenlazar
                anterior->siguiente = actual->siguiente;
            }
            free(actual);
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    return false;
}
```

## Compilación y Ejecución

### Requisitos
- CMake 3.15 o superior
- Compilador C (gcc, clang, msvc)
- Criterion (para tests) - opcional
- Valgrind (para verificación de memoria) - opcional

### Compilar

```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar

```bash
# Programa principal
./eliminar_nodo

# Tests completos
make test

# Tests específicos por categoría
make test_basico          # Tests básicos
make test_posicion        # Eliminación por posición
make test_extremos        # Primero y último
make test_multiple        # Eliminación múltiple
make test_avanzado        # Funciones avanzadas
make test_simple          # Versión simple
make test_edge_cases      # Casos límite

# Con salida detallada
make test_verbose

# Verificación de memoria (requiere Valgrind)
make test_memory
make run_memory
```

## Ejemplos de Uso

### Ejemplo 1: Uso Básico (según enunciado)

```c
#include "eliminar_nodo.h"

int main() {
    Nodo* lista = NULL;
    
    // Crear lista: 10 -> 20 -> 30 -> 40
    insertar_final_simple(&lista, 10);
    insertar_final_simple(&lista, 20);
    insertar_final_simple(&lista, 30);
    insertar_final_simple(&lista, 40);
    
    printf("Lista original: ");
    imprimir_lista_simple(lista);  // 10 -> 20 -> 30 -> 40 -> NULL
    
    // Eliminar nodo del medio
    eliminar_nodo_simple(&lista, 30);
    printf("Después de eliminar 30: ");
    imprimir_lista_simple(lista);  // 10 -> 20 -> 40 -> NULL
    
    // Eliminar cabeza
    eliminar_nodo_simple(&lista, 10);
    printf("Después de eliminar cabeza: ");
    imprimir_lista_simple(lista);  // 20 -> 40 -> NULL
    
    destruir_lista_simple(&lista);
    return 0;
}
```

### Ejemplo 2: Uso Completo con Estructura Lista

```c
Lista* lista = crear_lista();

// Crear lista: 1 -> 2 -> 3 -> 4 -> 5
for (int i = 1; i <= 5; i++) {
    insertar_final(lista, i);
}

printf("Lista inicial: ");
imprimir_lista(lista);  // [1 -> 2 -> 3 -> 4 -> 5]

// Eliminar por valor
if (eliminar_nodo(lista, 3)) {
    printf("Nodo 3 eliminado\n");
}
imprimir_lista(lista);  // [1 -> 2 -> 4 -> 5]

// Eliminar por posición
if (eliminar_en_posicion(lista, 1)) {  // Eliminar posición 1 (valor 2)
    printf("Posición 1 eliminada\n");
}
imprimir_lista(lista);  // [1 -> 4 -> 5]

// Eliminar extremos
eliminar_primero(lista);  // Eliminar 1
eliminar_ultimo(lista);   // Eliminar 5
imprimir_lista(lista);    // [4]

destruir_lista(lista);
```

### Ejemplo 3: Eliminación Múltiple

```c
Lista* lista = crear_lista();

// Lista con duplicados: 1 -> 2 -> 1 -> 3 -> 1 -> 4
int valores[] = {1, 2, 1, 3, 1, 4};
for (int i = 0; i < 6; i++) {
    insertar_final(lista, valores[i]);
}

printf("Lista con duplicados: ");
imprimir_lista(lista);  // [1 -> 2 -> 1 -> 3 -> 1 -> 4]

// Eliminar todas las ocurrencias de 1
size_t eliminados = eliminar_todos(lista, 1);
printf("Se eliminaron %zu nodos\n", eliminados);  // 3

imprimir_lista(lista);  // [2 -> 3 -> 4]
```

### Ejemplo 4: Eliminación de Duplicados

```c
Lista* lista = crear_lista();

// Lista: 1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4
int valores[] = {1, 1, 2, 3, 3, 3, 4};
for (int i = 0; i < 7; i++) {
    insertar_final(lista, valores[i]);
}

printf("Con duplicados: ");
imprimir_lista(lista);  // [1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4]

size_t duplicados = eliminar_duplicados(lista);
printf("Duplicados eliminados: %zu\n", duplicados);  // 3

imprimir_lista(lista);  // [1 -> 2 -> 3 -> 4]
```

## Patrones de Eliminación

### 1. Patrón de Dos Punteros

```c
// Mantener puntero al nodo anterior para reenlace
Nodo* anterior = NULL;
Nodo* actual = lista->cabeza;

while (actual) {
    if (condicion_eliminar(actual)) {
        if (anterior) {
            anterior->siguiente = actual->siguiente;
        } else {
            lista->cabeza = actual->siguiente;
        }
        destruir_nodo(actual);
        // No avanzar anterior
    } else {
        anterior = actual;
    }
    actual = actual->siguiente;
}
```

### 2. Patrón de Eliminación Múltiple

```c
// Eliminar desde la cabeza primero
while (lista->cabeza && condicion(lista->cabeza)) {
    eliminar_primero(lista);
}

// Luego procesar el resto
if (lista->cabeza) {
    Nodo* anterior = lista->cabeza;
    Nodo* actual = anterior->siguiente;
    
    while (actual) {
        if (condicion(actual)) {
            anterior->siguiente = actual->siguiente;
            if (actual == lista->cola) {
                lista->cola = anterior;
            }
            destruir_nodo(actual);
            actual = anterior->siguiente;
            lista->longitud--;
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}
```

### 3. Patrón de Validación

```c
bool eliminar_con_validacion(Lista* lista, /* parámetros */) {
    // Validación de entrada
    if (!lista || /* otras validaciones */) {
        return false;
    }
    
    // Guardar estado para rollback si es necesario
    size_t longitud_original = lista->longitud;
    
    // Realizar operación
    bool resultado = /* operación de eliminación */;
    
    // Validar integridad post-operación
    if (!validar_integridad_lista(lista)) {
        // Rollback si es posible
        return false;
    }
    
    return resultado;
}
```

## Casos de Uso Reales

### 1. Cache LRU (Least Recently Used)

```c
typedef struct CacheNode {
    int clave;
    int valor;
    struct CacheNode* siguiente;
} CacheNode;

typedef struct {
    CacheNode* cabeza;
    size_t capacidad;
    size_t tamaño;
} CacheLRU;

void lru_eliminar_menos_usado(CacheLRU* cache) {
    if (cache->cabeza && cache->tamaño >= cache->capacidad) {
        // Eliminar último elemento (menos usado)
        eliminar_ultimo_cache(cache);
    }
}
```

### 2. Lista de Tareas Pendientes

```c
typedef struct Tarea {
    int id;
    char* descripcion;
    bool completada;
    struct Tarea* siguiente;
} Tarea;

void completar_tarea(Tarea** lista_tareas, int id) {
    Tarea* anterior = NULL;
    Tarea* actual = *lista_tareas;
    
    while (actual) {
        if (actual->id == id) {
            actual->completada = true;
            // Opcionalmente, mover a lista de completadas
            // o eliminar directamente
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                *lista_tareas = actual->siguiente;
            }
            free(actual->descripcion);
            free(actual);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}
```

### 3. Sistema de Gestión de Procesos

```c
typedef struct Proceso {
    int pid;
    char* nombre;
    int prioridad;
    struct Proceso* siguiente;
} Proceso;

void terminar_proceso(Proceso** lista_procesos, int pid) {
    eliminar_nodo_por_criterio(lista_procesos, 
        [](Proceso* p) -> bool { return p->pid == pid; });
}

void limpiar_procesos_zombies(Proceso** lista_procesos) {
    eliminar_todos_por_criterio(lista_procesos,
        [](Proceso* p) -> bool { return es_zombie(p); });
}
```

### 4. Historial de Navegación con Límite

```c
typedef struct PaginaWeb {
    char* url;
    time_t timestamp;
    struct PaginaWeb* siguiente;
} PaginaWeb;

void agregar_pagina_historial(PaginaWeb** historial, const char* url, size_t max_paginas) {
    // Insertar nueva página al inicio
    insertar_inicio_historial(historial, url);
    
    // Eliminar páginas viejas si excede el límite
    size_t contador = 0;
    PaginaWeb* actual = *historial;
    PaginaWeb* anterior = NULL;
    
    while (actual) {
        contador++;
        if (contador > max_paginas) {
            // Eliminar el resto de la lista
            anterior->siguiente = NULL;
            destruir_lista_historial(actual);
            break;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}
```

## Características Avanzadas

### Eliminación con Predicados

```c
// Función que determina si un nodo debe eliminarse
typedef bool (*PredicadoEliminacion)(int valor);

size_t eliminar_con_predicado(Lista* lista, PredicadoEliminacion predicado) {
    if (!lista || !predicado) return 0;
    
    size_t eliminados = 0;
    
    // Eliminar de la cabeza
    while (lista->cabeza && predicado(lista->cabeza->valor)) {
        eliminar_primero(lista);
        eliminados++;
    }
    
    // Eliminar del resto
    if (lista->cabeza) {
        Nodo* anterior = lista->cabeza;
        Nodo* actual = anterior->siguiente;
        
        while (actual) {
            if (predicado(actual->valor)) {
                anterior->siguiente = actual->siguiente;
                if (actual == lista->cola) {
                    lista->cola = anterior;
                }
                destruir_nodo(actual);
                actual = anterior->siguiente;
                lista->longitud--;
                eliminados++;
            } else {
                anterior = actual;
                actual = actual->siguiente;
            }
        }
    }
    
    return eliminados;
}

// Ejemplos de uso
bool es_par(int valor) { return valor % 2 == 0; }
bool es_negativo(int valor) { return valor < 0; }
bool es_mayor_que_100(int valor) { return valor > 100; }

// Eliminar todos los números pares
size_t pares_eliminados = eliminar_con_predicado(lista, es_par);
```

### Eliminación por Rango

```c
size_t eliminar_rango(Lista* lista, size_t inicio, size_t fin) {
    if (!lista || inicio > fin || inicio >= lista->longitud) {
        return 0;
    }
    
    // Ajustar fin al último índice válido
    if (fin >= lista->longitud) {
        fin = lista->longitud - 1;
    }
    
    size_t eliminados = 0;
    
    // Eliminar desde el final hacia el inicio para mantener índices válidos
    for (size_t i = fin; i >= inicio && i != SIZE_MAX; i--) {
        if (eliminar_en_posicion(lista, i)) {
            eliminados++;
        }
    }
    
    return eliminados;
}
```

### Validación de Integridad

```c
bool validar_integridad_lista(const Lista* lista) {
    if (!lista) return false;
    
    // Lista vacía
    if (lista->longitud == 0) {
        return lista->cabeza == NULL && lista->cola == NULL;
    }
    
    // Lista no vacía debe tener cabeza
    if (!lista->cabeza) return false;
    
    // Contar nodos manualmente
    size_t contador = 0;
    Nodo* actual = lista->cabeza;
    Nodo* ultimo = NULL;
    
    while (actual) {
        contador++;
        ultimo = actual;
        actual = actual->siguiente;
        
        // Detectar ciclos (opcional, para listas muy grandes usar Floyd)
        if (contador > lista->longitud + 1) {
            return false; // Posible ciclo
        }
    }
    
    // Verificar longitud y cola
    return contador == lista->longitud && lista->cola == ultimo;
}
```

## Tests Implementados

La suite de tests incluye:

### Tests Básicos (eliminar_nodo_basico)
- ✅ Eliminación de nodo en el medio
- ✅ Eliminación de cabeza
- ✅ Eliminación de cola
- ✅ Eliminación de único elemento
- ✅ Intento de eliminación de valor inexistente

### Tests por Posición (eliminar_por_posicion)
- ✅ Eliminación en posición válida
- ✅ Eliminación en posición 0 (cabeza)
- ✅ Eliminación en posición inválida

### Tests de Extremos (eliminar_extremos)
- ✅ Eliminación de primero y último
- ✅ Operaciones en lista vacía

### Tests Múltiples (eliminar_multiple)
- ✅ Eliminar todas las ocurrencias
- ✅ Eliminar todos desde cabeza
- ✅ Vaciar lista completa

### Tests Avanzados (eliminar_avanzado)
- ✅ Eliminación de duplicados consecutivos
- ✅ Eliminación por rango
- ✅ Eliminación con predicados

### Tests Versión Simple (eliminar_simple)
- ✅ Compatibilidad con enunciado original
- ✅ Eliminación de cabeza con doble puntero

### Tests de Casos Límite (eliminar_edge_cases)
- ✅ Operaciones con punteros NULL
- ✅ Validación de integridad
- ✅ Rangos inválidos

## Consideraciones de Rendimiento

### Análisis de Complejidad

| Operación | Mejor Caso | Caso Promedio | Peor Caso | Nota |
|-----------|------------|---------------|-----------|------|
| `eliminar_primero()` | O(1) | O(1) | O(1) | Acceso directo |
| `eliminar_ultimo()` | O(n) | O(n) | O(n) | Recorrido completo |
| `eliminar_nodo(valor)` | O(1) | O(n/2) | O(n) | Depende de posición |
| `eliminar_en_posicion(k)` | O(1) | O(k) | O(n) | Proporcional a posición |
| `eliminar_todos(valor)` | O(n) | O(n) | O(n) | Un solo recorrido |

### Optimizaciones Implementadas

1. **Puntero a Cola**: Detección rápida del último nodo
2. **Casos Especiales**: Optimización para cabeza y cola
3. **Eliminación Múltiple**: Un solo recorrido para múltiples eliminaciones
4. **Validación Temprana**: Verificaciones rápidas antes del procesamiento

### Gestión de Memoria

```c
// Patrón seguro de eliminación
void eliminacion_segura(Nodo* nodo) {
    if (nodo) {
        // Limpiar datos si es necesario
        // (strings, arrays dinámicos, etc.)
        
        free(nodo);
        nodo = NULL;  // Prevenir double-free (aunque sea local)
    }
}
```

## Ejercicios Propuestos

1. **Eliminación eficiente del último**: Implementar lista doblemente enlazada para O(1)
2. **Eliminación con rollback**: Implementar deshacer eliminaciones
3. **Eliminación lazy**: Marcar como eliminado sin liberar memoria inmediatamente
4. **Eliminación atómica**: Versión thread-safe con mutexes
5. **Eliminación con callback**: Notificar antes/después de eliminar
6. **Garbage collection**: Sistema automático de limpieza de nodos no referenciados

## Debugging y Troubleshooting

### Errores Comunes

1. **Double-free**: Liberar el mismo nodo múltiples veces
```c
// ❌ Incorrecto
free(nodo);
free(nodo);  // Double-free!

// ✅ Correcto
free(nodo);
nodo = NULL;
```

2. **Dangling pointers**: Referencias a memoria liberada
```c
// ❌ Incorrecto
Nodo* nodo = lista->cabeza;
eliminar_primero(lista);
printf("%d", nodo->valor);  // Dangling pointer!

// ✅ Correcto
int valor = lista->cabeza->valor;
eliminar_primero(lista);
printf("%d", valor);
```

3. **Memory leaks**: No liberar nodos eliminados
```c
// ❌ Incorrecto
anterior->siguiente = actual->siguiente;
// ¡Olvidamos liberar actual!

// ✅ Correcto
anterior->siguiente = actual->siguiente;
free(actual);
```

### Herramientas de Debugging

```bash
# Verificar memory leaks
valgrind --tool=memcheck --leak-check=full ./eliminar_nodo

# Detectar errores de memoria
valgrind --tool=memcheck --track-origins=yes ./test_eliminar_nodo

# Profiling de rendimiento
valgrind --tool=callgrind ./eliminar_nodo
```

## Referencias

- "Introduction to Algorithms" - Cormen et al. (Capítulo 10.2)
- "Data Structures and Algorithm Analysis in C" - Mark Allen Weiss
- "The C Programming Language" - Kernighan & Ritchie
- [Linux Kernel Linked Lists](https://kernelnewbies.org/FAQ/LinkedLists)
- [Memory Management Best Practices](https://www.kernel.org/doc/Documentation/memory-management/)
