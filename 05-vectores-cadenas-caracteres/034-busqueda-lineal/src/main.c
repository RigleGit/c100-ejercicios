#include <stdio.h>

#define MAX 100

#ifndef UNIT_TESTING
int main(void) {
    int numeros[MAX];
    int n, valor, encontrado = 0;

    printf("¿Cuántos números vas a introducir (máximo %d)? ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Cantidad inválida.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Introduce el número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    printf("Introduce el valor que deseas buscar: ");
    scanf("%d", &valor);

    for (int i = 0; i < n; i++) {
        if (numeros[i] == valor) {
            printf("Valor encontrado en la posición %d (índice %d).\n", i + 1, i);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Valor no encontrado en el array.\n");
    }

    return 0;
}
#endif
