# Ejercicio 070 — Lista Doblemente Enlazada

## Descripción

Implementación completa de una lista doblemente enlazada en C con operaciones de inserción, eliminación, búsqueda, recorrido bidireccional, iteradores y utilidades de validación/visualización.

## Objetivos

- Practicar manejo de punteros y estructuras en C.
- Implementar una lista doblemente enlazada robusta y segura.
- Proveer funciones de utilidad y demostraciones para comprobar la integridad de la lista.

## Estructura del ejercicio

- `include/lista_doble.h` — Definición de estructuras y API pública.
- `src/lista_doble.c` — Implementación completa.
- `tests/` — Tests de Criterion (si están presentes) y utilidades de prueba.
- `CMakeLists.txt` — Configuración de compilación.

## API pública (firmas importantes)

### Estructuras

```c
typedef struct Nodo {
    int dato;
    struct Nodo* siguiente;
    struct Nodo* anterior;
} Nodo;

typedef struct ListaDoble {
    Nodo* cabeza;
    Nodo* cola;
    size_t tamanio;
} ListaDoble;

typedef struct IteradorDoble {
    Nodo* actual;
    ListaDoble* lista;
    bool adelante;
} IteradorDoble;
```

### Funciones clave

```c
ListaDoble* lista_crear(void);
void lista_destruir(ListaDoble* lista);
Nodo* nodo_crear(int dato);
void nodo_destruir(Nodo* nodo);

bool lista_insertar_inicio(ListaDoble* lista, int dato);
bool lista_insertar_final(ListaDoble* lista, int dato);
bool lista_insertar_posicion(ListaDoble* lista, size_t posicion, int dato);

bool lista_eliminar_inicio(ListaDoble* lista);
bool lista_eliminar_final(ListaDoble* lista);
bool lista_eliminar_posicion(ListaDoble* lista, size_t posicion);

Nodo* lista_buscar(ListaDoble* lista, int dato);
Nodo* lista_obtener_posicion(ListaDoble* lista, size_t posicion);

void lista_imprimir_adelante(ListaDoble* lista);
void lista_imprimir_atras(ListaDoble* lista);

IteradorDoble iterador_crear(ListaDoble* lista, bool adelante);
bool iterador_tiene_siguiente(IteradorDoble* iter);
int iterador_siguiente(IteradorDoble* iter);
void iterador_reiniciar(IteradorDoble* iter);
void iterador_cambiar_direccion(IteradorDoble* iter);

bool lista_vacia(ListaDoble* lista);
size_t lista_tamanio(ListaDoble* lista);

bool lista_validar_integridad(ListaDoble* lista);
size_t lista_contar_adelante(ListaDoble* lista);
size_t lista_contar_atras(ListaDoble* lista);

void lista_invertir(ListaDoble* lista);
void lista_ordenar(ListaDoble* lista, bool ascendente);
ListaDoble* lista_clonar(ListaDoble* lista);
size_t lista_eliminar_duplicados(ListaDoble* lista);

void lista_mostrar_estructura(ListaDoble* lista);
void lista_mostrar_estadisticas(ListaDoble* lista);
```

## Cómo compilar

Este ejercicio usa CMake. Desde la raíz del ejercicio:

```bash
mkdir -p build && cd build
cmake ..
make
```

## Ejecutar tests

Si el proyecto contiene tests (subdirectorio `tests/`) y Criterion está instalado, se generará un ejecutable de tests en `build/` tras ejecutar `cmake` y `make`. Usa el script central del repositorio para ejecutar los tests de todos los ejercicios:

```bash
./tools/quick_test.sh 9
```

## Ejemplos

- Usa `lista_crear()` para obtener una lista vacía, luego `lista_insertar_final()` o `lista_insertar_inicio()` para añadir elementos.
- Usa `lista_imprimir_adelante()` / `lista_imprimir_atras()` para mostrar contenidos.
- Las funciones `demo_*` y `lista_mostrar_estructura()` ayudan a depuración y visualización interna.

## Notas

- La implementación optimiza búsquedas por posición seleccionando recorrido desde cabeza o cola según la posición.
- Muchas funciones validan punteros y condiciones de borde para evitar accesos inválidos.