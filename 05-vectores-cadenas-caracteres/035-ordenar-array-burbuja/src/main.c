#include <stdio.h>

#define MAX 100

#ifndef UNIT_TESTING
int main(void) {
    int numeros[MAX];
    int n, temp;

    printf("¿Cuántos números quieres ordenar (máximo %d)? ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Cantidad inválida.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Introduce el número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    // Algoritmo de ordenamiento burbuja
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (numeros[j] > numeros[j + 1]) {
                temp = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = temp;
            }
        }
    }

    printf("Array ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", numeros[i]);
    }
    printf("\n");

    return 0;
}
#endif
