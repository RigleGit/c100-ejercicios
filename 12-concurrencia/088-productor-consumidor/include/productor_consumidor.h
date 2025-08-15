#ifndef PRODUCTOR_CONSUMIDOR_H
#define PRODUCTOR_CONSUMIDOR_H

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

// Implementación sencilla del patrón Productor-Consumidor para el ejercicio 88

typedef int elemento_t;

typedef struct {
    elemento_t *buffer;
    size_t capacidad;
    size_t contador;
    size_t indice_entrada;
    size_t indice_salida;
    pthread_mutex_t mutex;
    pthread_cond_t cond_lleno;
    pthread_cond_t cond_vacio;
} buffer_circular_t;

// Inicialización y limpieza
bool inicializar_buffer_circular(buffer_circular_t *buffer, size_t capacidad);
void limpiar_buffer_circular(buffer_circular_t *buffer);

// Operaciones bloqueantes del productor/consumidor
int producir_item(buffer_circular_t *buffer, elemento_t valor);
int consumir_item(buffer_circular_t *buffer, elemento_t *valor);

// Utilidades y atajos usados por los tests
static inline bool esta_lleno(const buffer_circular_t *b) { return b && b->contador == b->capacidad; }
static inline bool esta_vacio(const buffer_circular_t *b) { return !b || b->contador == 0; }

// Macros de compatibilidad para tests antiguos
#define ocupados contador
#define tamaño capacidad

// API de compatibilidad para tests (equivalente a las funciones "legacy")
buffer_circular_t* crear_buffer(size_t capacidad);
void destruir_buffer(buffer_circular_t* buffer);

// Wrappers de conveniencia (no bloqueantes) usados en algunos tests
int insertar_item(buffer_circular_t *buffer, elemento_t valor);   // devuelve -1 si lleno
int extraer_item(buffer_circular_t *buffer, elemento_t *valor);   // devuelve -1 si vacío

// Helpers para pruebas con hilos
typedef struct {
    buffer_circular_t *buffer;
    int num_datos;
    int delay_us;  // puede ser 0
} productor_args_t;

typedef struct {
    buffer_circular_t *buffer;
    int num_datos;
    int delay_us;  // puede ser 0
    long long suma; // salida
} consumidor_args_t;

void* hilo_productor(void *arg);
void* hilo_consumidor(void *arg);


#endif // PRODUCTOR_CONSUMIDOR_H
