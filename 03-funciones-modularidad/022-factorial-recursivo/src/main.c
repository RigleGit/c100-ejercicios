#include <stdio.h>

// Función recursiva que calcula el factorial
long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1; // Caso base
    } else {
        return n * factorial(n - 1); // Caso recursivo
    }
}

int main(void) {
    int num;

    printf("Introduce un número entero no negativo: ");
    scanf("%d", &num);

    if (num < 0) {
        printf("El factorial no está definido para números negativos.\n");
    } else {
        long resultado = factorial(num);
        printf("El factorial de %d es: %ld\n", num, resultado);
    }

    return 0;
}
