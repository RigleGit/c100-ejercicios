#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    int secreto, intento;

    // Inicializa el generador de números aleatorios
    srand(time(NULL));
    secreto = rand() % 100 + 1;  // Número entre 1 y 100

    printf("Adivina el número (entre 1 y 100):\n");

    do {
        printf("Tu intento: ");
        scanf("%d", &intento);

        if (intento < secreto) {
            printf("El número es mayor.\n");
        } else if (intento > secreto) {
            printf("El número es menor.\n");
        } else {
            printf("¡Correcto! Has adivinado el número.\n");
        }
    } while (intento != secreto);

    return 0;
}
