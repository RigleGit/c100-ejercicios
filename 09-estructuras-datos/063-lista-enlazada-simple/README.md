# Ejercicio 063: Lista Enlazada Simple

## Descripción

Este ejercicio implementa una **lista enlazada simple (singly linked list)**, una de las estructuras de datos dinámicas más fundamentales en programación. Una lista enlazada es una secuencia de nodos donde cada nodo contiene datos y un puntero al siguiente nodo, permitiendo crear secuencias de longitud variable que pueden crecer y decrecer dinámicamente.

## Conceptos Clave

### ¿Qué es una Lista Enlazada?

Una lista enlazada es una estructura de datos lineal donde:
- **Cada elemento (nodo) contiene**: datos + puntero al siguiente elemento
- **Acceso secuencial**: para llegar al elemento N, hay que pasar por todos los anteriores
- **Tamaño dinámico**: puede crecer o decrecer durante la ejecución
- **Memoria no contigua**: los nodos pueden estar dispersos en memoria

### Ventajas y Desventajas

**Ventajas:**
- ✅ Tamaño dinámico (no hay límite predefinido)
- ✅ Inserción/eliminación O(1) en posiciones conocidas
- ✅ Uso eficiente de memoria (solo la necesaria)
- ✅ Fácil implementación de otras estructuras (pilas, colas)

**Desventajas:**
- ❌ Acceso secuencial O(n) - no hay acceso aleatorio
- ❌ Overhead de memoria por los punteros
- ❌ No es cache-friendly (localidad de memoria pobre)
- ❌ No permite búsqueda binaria

### Comparación con Arrays

| Característica | Array | Lista Enlazada |
|----------------|-------|----------------|
| Acceso por índice | O(1) | O(n) |
| Inserción al inicio | O(n) | O(1) |
| Inserción al final | O(1) | O(1)* |
| Búsqueda | O(n) | O(n) |
| Memoria | Contigua | Dispersa |
| Cache performance | Excelente | Pobre |

*Con puntero a cola

## Estructura de Archivos

```
063-lista-enlazada-simple/
├── src/
│   └── lista_enlazada.c        # Implementación principal
├── include/
│   └── lista_enlazada.h        # Declaraciones y definiciones
├── tests/
│   └── test_lista_enlazada.c   # Suite de tests con Criterion
├── CMakeLists.txt              # Configuración de compilación
└── README.md                   # Este archivo
```

## Implementación

### Estructura del Nodo

```c
typedef struct Nodo {
    int valor;                  // Datos almacenados
    struct Nodo* siguiente;     // Puntero al siguiente nodo
} Nodo;
```

### Estructura de Lista Completa

```c
typedef struct {
    Nodo* cabeza;               // Primer nodo
    Nodo* cola;                 // Último nodo (optimización)
    size_t longitud;            // Número de elementos
} Lista;
```

**Ventajas de la estructura Lista:**
- Operaciones O(1) al final (con puntero a cola)
- Longitud disponible instantáneamente
- Mejor encapsulación y manejo de errores

### Funcionalidades Implementadas

#### 1. Creación y Destrucción
```c
Lista* crear_lista(void);                    // Crear lista vacía
void destruir_lista(Lista* lista);           // Liberar memoria completa
Nodo* crear_nodo(int valor);                 // Crear nodo individual
void destruir_nodo(Nodo* nodo);              // Liberar nodo individual
```

#### 2. Inserción
```c
bool insertar_final(Lista* lista, int valor);           // O(1)
bool insertar_inicio(Lista* lista, int valor);          // O(1)
bool insertar_en_posicion(Lista* lista, size_t pos, int valor);  // O(n)
bool insertar_ordenado(Lista* lista, int valor);        // O(n)
```

#### 3. Búsqueda y Acceso
```c
Nodo* buscar_valor(const Lista* lista, int valor);      // O(n)
Nodo* obtener_en_posicion(const Lista* lista, size_t pos);  // O(n)
size_t obtener_posicion(const Lista* lista, int valor);     // O(n)
bool contiene_valor(const Lista* lista, int valor);         // O(n)
```

#### 4. Consultas
```c
bool esta_vacia(const Lista* lista);                    // O(1)
size_t obtener_longitud(const Lista* lista);            // O(1)
bool obtener_primero(const Lista* lista, int* valor);   // O(1)
bool obtener_ultimo(const Lista* lista, int* valor);    // O(1)
```

#### 5. Utilidades
```c
Lista* copiar_lista(const Lista* lista);                // O(n)
void invertir_lista(Lista* lista);                      // O(n)
bool comparar_listas(const Lista* l1, const Lista* l2); // O(n)
```

#### 6. Visualización
```c
void imprimir_lista(const Lista* lista);
void imprimir_lista_detallada(const Lista* lista, const char* titulo);
void imprimir_debug_lista(const Lista* lista);
```

### Versión Simple (Compatibilidad con Enunciado)

Para mantener compatibilidad con el enunciado original, también se incluyen las funciones simples:

```c
bool insertar_final_simple(Nodo** cabeza, int valor);
void imprimir_lista_simple(const Nodo* cabeza);
void destruir_lista_simple(Nodo** cabeza);
```

## Compilación y Ejecución

### Requisitos
- CMake 3.15 o superior
- Compilador C (gcc, clang, msvc)
- Criterion (para tests) - opcional

### Compilar

```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar

```bash
# Programa principal
./lista_enlazada

# Tests (si Criterion está instalado)
make test

# Tests específicos
make test_basico        # Solo tests básicos
make test_insercion     # Solo tests de inserción
make test_busqueda      # Solo tests de búsqueda
make test_verbose       # Tests con salida detallada

# Ejecutar programa directamente
make run
```

### Instalar Criterion (para tests)

```bash
# Ubuntu/Debian
sudo apt-get install libcriterion-dev

# macOS
brew install criterion

# Arch Linux
sudo pacman -S criterion
```

## Ejemplos de Uso

### Ejemplo 1: Uso Básico (según enunciado)

```c
#include "lista_enlazada.h"

int main() {
    Nodo* lista = NULL;
    
    // Insertar elementos
    insertar_final_simple(&lista, 10);
    insertar_final_simple(&lista, 20);
    insertar_final_simple(&lista, 30);
    
    // Imprimir
    imprimir_lista_simple(lista);  // Salida: 10 -> 20 -> 30 -> NULL
    
    // Limpiar memoria
    destruir_lista_simple(&lista);
    return 0;
}
```

### Ejemplo 2: Uso Completo con Estructura Lista

```c
#include "lista_enlazada.h"

int main() {
    Lista* lista = crear_lista();
    
    // Insertar elementos
    insertar_final(lista, 1);
    insertar_final(lista, 2);
    insertar_final(lista, 3);
    insertar_inicio(lista, 0);
    
    printf("Longitud: %zu\n", obtener_longitud(lista));  // 4
    imprimir_lista(lista);  // [0 -> 1 -> 2 -> 3]
    
    // Búsquedas
    if (contiene_valor(lista, 2)) {
        printf("La lista contiene el valor 2\n");
    }
    
    size_t pos = obtener_posicion(lista, 2);
    printf("El valor 2 está en la posición %zu\n", pos);  // 2
    
    destruir_lista(lista);
    return 0;
}
```

### Ejemplo 3: Lista Ordenada

```c
Lista* lista_ordenada = crear_lista();

// Insertar valores desordenados
insertar_ordenado(lista_ordenada, 5);
insertar_ordenado(lista_ordenada, 2);
insertar_ordenado(lista_ordenada, 8);
insertar_ordenado(lista_ordenada, 1);

imprimir_lista(lista_ordenada);  // [1 -> 2 -> 5 -> 8]
```

### Ejemplo 4: Operaciones Avanzadas

```c
Lista* original = crear_lista();
insertar_final(original, 1);
insertar_final(original, 2);
insertar_final(original, 3);

// Copiar lista
Lista* copia = copiar_lista(original);

// Invertir original
invertir_lista(original);
imprimir_lista(original);  // [3 -> 2 -> 1]

// Comparar listas
if (comparar_listas(original, copia)) {
    printf("Las listas son iguales\n");
} else {
    printf("Las listas son diferentes\n");  // Este caso
}
```

## Casos de Uso Reales

### 1. Historial de Navegador
```c
typedef struct {
    char* url;
    time_t timestamp;
    struct HistorialNodo* siguiente;
} HistorialNodo;

// Insertar nueva página visitada
void visitar_pagina(HistorialNodo** historial, const char* url);
```

### 2. Lista de Reproducción Musical
```c
typedef struct {
    char* titulo;
    char* artista;
    int duracion;
    struct CancionNodo* siguiente;
} CancionNodo;

// Añadir canción al final
void agregar_cancion(CancionNodo** playlist, const char* titulo, const char* artista);
```

### 3. Buffer Circular de Logs
```c
typedef struct {
    char* mensaje;
    time_t timestamp;
    int nivel;
    struct LogNodo* siguiente;
} LogNodo;

// Insertar log manteniendo tamaño máximo
void log_mensaje(LogNodo** logs, const char* mensaje, int nivel, size_t max_logs);
```

### 4. Undo/Redo en Editor
```c
typedef struct {
    char* accion;
    void* datos_anteriores;
    struct AccionNodo* siguiente;
} AccionNodo;

// Registrar acción para deshacer
void registrar_accion(AccionNodo** historial, const char* accion, void* datos);
```

## Características Avanzadas

### Análisis de Complejidad

| Operación | Complejidad | Nota |
|-----------|-------------|------|
| `insertar_inicio` | O(1) | Siempre constante |
| `insertar_final` | O(1) | Con puntero a cola |
| `insertar_en_posicion` | O(n) | Requiere recorrido |
| `buscar_valor` | O(n) | Búsqueda lineal |
| `obtener_longitud` | O(1) | Almacenada en estructura |
| `copiar_lista` | O(n) | Proporcional al tamaño |
| `invertir_lista` | O(n) | Un solo recorrido |

### Optimizaciones Implementadas

1. **Puntero a Cola**: Inserción al final O(1)
2. **Contador de Longitud**: Consulta de tamaño O(1)
3. **Verificación de NULL**: Operaciones seguras
4. **Reutilización de Código**: Funciones modulares

### Gestión de Memoria

```c
// Patrón de gestión de memoria
Lista* lista = crear_lista();  // malloc para Lista
insertar_final(lista, 42);     // malloc para cada Nodo
// ... usar lista ...
destruir_lista(lista);         // free de todos los nodos + Lista
```

**Importante**: Siempre llamar `destruir_lista()` para evitar memory leaks.

## Tests Implementados

La suite de tests cubre exhaustivamente:

### Tests Básicos (lista_enlazada_basico)
- ✅ Creación de lista y nodos
- ✅ Inserción en lista vacía
- ✅ Inserción múltiple
- ✅ Estado de punteros cabeza/cola

### Tests de Inserción (lista_enlazada_insercion)
- ✅ Inserción al inicio
- ✅ Inserción en posiciones específicas
- ✅ Inserción ordenada
- ✅ Manejo de posiciones inválidas

### Tests de Búsqueda (lista_enlazada_busqueda)
- ✅ Búsqueda de valores existentes/inexistentes
- ✅ Obtención por posición
- ✅ Cálculo de posiciones
- ✅ Verificación de contenido

### Tests de Consultas (lista_enlazada_consultas)
- ✅ Obtención de primer/último elemento
- ✅ Cálculo de longitud
- ✅ Verificación de lista vacía

### Tests de Utilidades (lista_enlazada_utilidades)
- ✅ Copia de listas
- ✅ Comparación de listas
- ✅ Inversión de orden
- ✅ Integridad de datos

### Tests de Versión Simple (lista_enlazada_simple)
- ✅ Funciones compatibles con enunciado
- ✅ Gestión de puntero a cabeza
- ✅ Destrucción segura

### Tests de Casos Límite (lista_enlazada_edge_cases)
- ✅ Operaciones con punteros NULL
- ✅ Lista vacía
- ✅ Un solo elemento
- ✅ Posiciones inválidas

## Variantes y Extensiones

### 1. Lista Doblemente Enlazada
```c
typedef struct NodoDoble {
    int valor;
    struct NodoDoble* siguiente;
    struct NodoDoble* anterior;    // Puntero adicional
} NodoDoble;
```

### 2. Lista Circular
```c
// El último nodo apunta al primero
void hacer_circular(Lista* lista) {
    if (lista->cola) {
        lista->cola->siguiente = lista->cabeza;
    }
}
```

### 3. Lista con Centinela
```c
typedef struct {
    Nodo centinela;      // Nodo dummy que simplifica algoritmos
    size_t longitud;
} ListaCentinela;
```

### 4. Lista Genérica
```c
typedef struct NodoGen {
    void* datos;
    size_t tam_datos;
    struct NodoGen* siguiente;
} NodoGenerico;
```

## Ejercicios Propuestos

1. **Función de eliminación**: Implementar `eliminar_valor()` y `eliminar_en_posicion()`
2. **Fusión ordenada**: Fusionar dos listas ordenadas manteniendo el orden
3. **Detección de ciclos**: Algoritmo de Floyd (tortuga y liebre)
4. **Lista genérica**: Implementar con `void*` para cualquier tipo de dato
5. **Iteradores**: Implementar iteradores para recorrido seguro
6. **Persistencia**: Guardar/cargar lista desde archivo
7. **Concurrencia**: Versión thread-safe con mutexes

## Algoritmos Relacionados

### 1. Eliminación de Duplicados
```c
void eliminar_duplicados(Lista* lista) {
    if (!lista || !lista->cabeza) return;
    
    Nodo* actual = lista->cabeza;
    while (actual && actual->siguiente) {
        if (actual->valor == actual->siguiente->valor) {
            Nodo* duplicado = actual->siguiente;
            actual->siguiente = duplicado->siguiente;
            destruir_nodo(duplicado);
            lista->longitud--;
        } else {
            actual = actual->siguiente;
        }
    }
}
```

### 2. Encontrar el Medio (Algoritmo de Floyd)
```c
Nodo* encontrar_medio(const Lista* lista) {
    if (!lista || !lista->cabeza) return NULL;
    
    Nodo* lento = lista->cabeza;
    Nodo* rapido = lista->cabeza;
    
    while (rapido && rapido->siguiente) {
        lento = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
    }
    
    return lento;
}
```

### 3. Fusión de Listas Ordenadas
```c
Lista* fusionar_ordenadas(const Lista* lista1, const Lista* lista2) {
    Lista* resultado = crear_lista();
    Nodo* p1 = lista1->cabeza;
    Nodo* p2 = lista2->cabeza;
    
    while (p1 && p2) {
        if (p1->valor <= p2->valor) {
            insertar_final(resultado, p1->valor);
            p1 = p1->siguiente;
        } else {
            insertar_final(resultado, p2->valor);
            p2 = p2->siguiente;
        }
    }
    
    // Insertar elementos restantes
    while (p1) {
        insertar_final(resultado, p1->valor);
        p1 = p1->siguiente;
    }
    while (p2) {
        insertar_final(resultado, p2->valor);
        p2 = p2->siguiente;
    }
    
    return resultado;
}
```

## Consideraciones de Rendimiento

### Memory Layout
```
Array:        [1][2][3][4][5]     <- Memoria contigua
Lista:        [1|*] -> [2|*] -> [3|*] -> [4|*] -> [5|NULL]
                ↓        ↓        ↓        ↓        ↓
              addr1    addr2    addr3    addr4    addr5
```

### Cache Performance
- **Arrays**: Excelente localidad de referencia
- **Listas**: Pobre localidad, muchos cache misses

### Cuándo Usar Lista Enlazada
- ✅ Inserción/eliminación frecuente al inicio
- ✅ Tamaño muy variable
- ✅ No necesitas acceso aleatorio
- ✅ Implementando otras estructuras (pilas, colas)

### Cuándo NO Usar Lista Enlazada
- ❌ Acceso frecuente por índice
- ❌ Búsquedas frecuentes
- ❌ Datos numéricos simples con muchas operaciones
- ❌ Sistemas con memoria muy limitada (overhead de punteros)

## Notas de Implementación

### Manejo de Errores
- Todas las funciones verifican punteros NULL
- Inserción retorna `bool` para indicar éxito/fallo
- Destrucción es segura con punteros NULL

### Convenciones de Nomenclatura
- `crear_*`: Funciones que asignan memoria
- `destruir_*`: Funciones que liberan memoria
- `obtener_*`: Funciones de consulta (no modifican)
- `insertar_*`: Funciones de modificación

### Gestión de Memoria Defensiva
```c
void destruir_lista(Lista* lista) {
    if (!lista) return;          // Seguro con NULL
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        Nodo* siguiente = actual->siguiente;
        destruir_nodo(actual);   // También seguro con NULL
        actual = siguiente;
    }
    
    free(lista);                 // Liberar estructura principal
}
```

## Referencias

- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein (Capítulo 10)
- "Data Structures and Algorithm Analysis in C" - Mark Allen Weiss
- "The C Programming Language" - Kernighan & Ritchie
- [Wikipedia: Linked List](https://en.wikipedia.org/wiki/Linked_list)
- [GeeksforGeeks: Linked List Data Structure](https://www.geeksforgeeks.org/data-structures/linked-list/)
