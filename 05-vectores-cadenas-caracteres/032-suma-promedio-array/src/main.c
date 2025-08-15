#include <stdio.h>

#define MAX 100

#ifndef UNIT_TESTING
int main(void) {
    int numeros[MAX];
    int n;
    int suma = 0;
    double promedio;

    printf("¿Cuántos números vas a introducir (máximo %d)? ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Cantidad inválida.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Introduce el número %d: ", i + 1);
        scanf("%d", &numeros[i]);
        suma += numeros[i];
    }

    promedio = (double)suma / n;

    printf("La suma es: %d\n", suma);
    printf("El promedio es: %.2f\n", promedio);

    return 0;
}
#endif
