#include "../include/productor_consumidor.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

bool inicializar_buffer_circular(buffer_circular_t *buffer, size_t capacidad) {
    if (!buffer || capacidad == 0) return false;
    memset(buffer, 0, sizeof(*buffer));
    buffer->buffer = (elemento_t *)malloc(capacidad * sizeof(elemento_t));
    if (!buffer->buffer) return false;
    buffer->capacidad = capacidad;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->cond_lleno, NULL);
    pthread_cond_init(&buffer->cond_vacio, NULL);
    return true;
}

void limpiar_buffer_circular(buffer_circular_t *buffer) {
    if (!buffer) return;
    pthread_mutex_lock(&buffer->mutex);
    pthread_cond_broadcast(&buffer->cond_lleno);
    pthread_cond_broadcast(&buffer->cond_vacio);
    pthread_mutex_unlock(&buffer->mutex);
    pthread_cond_destroy(&buffer->cond_lleno);
    pthread_cond_destroy(&buffer->cond_vacio);
    pthread_mutex_destroy(&buffer->mutex);
    free(buffer->buffer);
    memset(buffer, 0, sizeof(*buffer));
}

int producir_item(buffer_circular_t *buffer, elemento_t valor) {
    if (!buffer) return -1;
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->contador == buffer->capacidad) {
        pthread_cond_wait(&buffer->cond_lleno, &buffer->mutex);
    }
    buffer->buffer[buffer->indice_entrada] = valor;
    buffer->indice_entrada = (buffer->indice_entrada + 1) % buffer->capacidad;
    buffer->contador++;
    pthread_cond_signal(&buffer->cond_vacio);
    pthread_mutex_unlock(&buffer->mutex);
    return 0;
}

int consumir_item(buffer_circular_t *buffer, elemento_t *valor) {
    if (!buffer || !valor) return -1;
    
    pthread_mutex_lock(&buffer->mutex);
    
    // Esperar con timeout para evitar colgado
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; // 1 segundo de timeout
    
    while (buffer->contador == 0) {
        int result = pthread_cond_timedwait(&buffer->cond_vacio, &buffer->mutex, &timeout);
        if (result == ETIMEDOUT) {
            pthread_mutex_unlock(&buffer->mutex);
            return -1; // Timeout
        }
    }
    
    *valor = buffer->buffer[buffer->indice_salida];
    buffer->indice_salida = (buffer->indice_salida + 1) % buffer->capacidad;
    buffer->contador--;
    
    pthread_cond_signal(&buffer->cond_lleno);
    pthread_mutex_unlock(&buffer->mutex);
    
    return 0;
}

int insertar_item(buffer_circular_t *buffer, elemento_t valor) {
    if (!buffer) return -1;
    int rc = -1;
    pthread_mutex_lock(&buffer->mutex);
    if (buffer->contador < buffer->capacidad) {
        buffer->buffer[buffer->indice_entrada] = valor;
        buffer->indice_entrada = (buffer->indice_entrada + 1) % buffer->capacidad;
        buffer->contador++;
        pthread_cond_signal(&buffer->cond_vacio);
        rc = 0;
    }
    pthread_mutex_unlock(&buffer->mutex);
    return rc;
}

int extraer_item(buffer_circular_t *buffer, elemento_t *valor) {
    if (!buffer || !valor) return -1;
    int rc = -1;
    pthread_mutex_lock(&buffer->mutex);
    if (buffer->contador > 0) {
        *valor = buffer->buffer[buffer->indice_salida];
        buffer->indice_salida = (buffer->indice_salida + 1) % buffer->capacidad;
        buffer->contador--;
        pthread_cond_signal(&buffer->cond_lleno);
        rc = 0;
    }
    pthread_mutex_unlock(&buffer->mutex);
    return rc;
}

void* hilo_productor(void *arg) {
    productor_args_t *p = (productor_args_t *)arg;
    for (int i = 0; i < p->num_datos; i++) {
        int dato = rand() % 100;
        producir_item(p->buffer, dato);
        if (p->delay_us > 0) usleep(p->delay_us);
    }
    return NULL;
}

void* hilo_consumidor(void *arg) {
    consumidor_args_t *c = (consumidor_args_t *)arg;
    c->suma = 0;
    for (int i = 0; i < c->num_datos; i++) {
        int dato = 0;
        consumir_item(c->buffer, &dato);
        c->suma += dato;
        if (c->delay_us > 0) usleep(c->delay_us);
    }
    return NULL;
}

// ================================
// API de compatibilidad para tests
// ================================

buffer_circular_t* crear_buffer(size_t capacidad) {
    buffer_circular_t* buffer = malloc(sizeof(buffer_circular_t));
    if (!buffer) return NULL;
    
    if (!inicializar_buffer_circular(buffer, capacidad)) {
        free(buffer);
        return NULL;
    }
    
    return buffer;
}

void destruir_buffer(buffer_circular_t* buffer) {
    if (buffer) {
        limpiar_buffer_circular(buffer);
        free(buffer);
    }
}

