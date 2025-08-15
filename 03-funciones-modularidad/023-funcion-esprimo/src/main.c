#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Devuelve true si n es primo, false en caso contrario
bool esPrimo(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;

    if (n % 2 == 0) return false;

    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }

    return true;
}

int main(void) {
    int limite;

    printf("Introduce un número entero positivo: ");
    scanf("%d", &limite);

    printf("Números primos hasta %d:\n", limite);
    for (int i = 2; i <= limite; i++) {
        if (esPrimo(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
