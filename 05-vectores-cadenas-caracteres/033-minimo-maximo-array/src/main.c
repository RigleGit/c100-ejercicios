#include <stdio.h>

#define MAX 100

#ifndef UNIT_TESTING
int main(void) {
    int numeros[MAX];
    int n;
    int min, max;

    printf("¿Cuántos números vas a introducir (máximo %d)? ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Cantidad inválida.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Introduce el número %d: ", i + 1);
        scanf("%d", &numeros[i]);

        if (i == 0) {
            min = max = numeros[i];
        } else {
            if (numeros[i] < min) min = numeros[i];
            if (numeros[i] > max) max = numeros[i];
        }
    }

    printf("Valor mínimo: %d\n", min);
    printf("Valor máximo: %d\n", max);

    return 0;
}
#endif
