#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* trabajador(void* arg) {
    int id = *(int*)arg;
    printf("Trabajador %d iniciado\n", id);
    sleep(1 + (id % 3)); // Trabajo variable
    printf("Trabajador %d terminado\n", id);
    return NULL;
}

int main(void) {
    const int NUM_HILOS = 5;
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    // Crear hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i + 1;
        pthread_create(&hilos[i], NULL, trabajador, &ids[i]);
    }

    // Esperar hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todos los trabajadores terminaron\n");
    return 0;
}
