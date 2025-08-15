#include <stdio.h>
#define MAX 100

#ifndef UNIT_TESTING
int main(void) {
    int numeros[MAX];
    int n;

    printf("¿Cuántos números quieres introducir (máximo %d)? ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Cantidad inválida.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Introduce el número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    printf("Los números en orden inverso son:\n");
    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", numeros[i]);
    }
    printf("\n");

    return 0;
}
#endif
