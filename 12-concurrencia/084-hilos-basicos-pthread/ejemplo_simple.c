#include <stdio.h>
#include <pthread.h>

// Función que ejecutará el hilo
void* funcionDelHilo(void* arg) {
    (void)arg; // Suprimir warning
    printf("Hola desde el hilo hijo.\n");
    return NULL;
}

int main(void) {
    pthread_t hilo;

    // Crear el hilo
    if (pthread_create(&hilo, NULL, funcionDelHilo, NULL) != 0) {
        perror("Error al crear el hilo");
        return 1;
    }

    printf("Hilo principal esperando al hilo hijo...\n");

    // Esperar a que el hilo termine
    if (pthread_join(hilo, NULL) != 0) {
        perror("Error al esperar el hilo");
        return 1;
    }

    printf("Hilo hijo ha terminado. Fin del programa.\n");

    return 0;
}
