#include <stdio.h>

int main(void) {
    int N;

    printf("Introduce un número entero positivo: ");
    scanf("%d", &N);

    for (int num = 2; num <= N; num++) {
        int esPrimo = 1; // Suponemos que es primo

        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                esPrimo = 0; // No es primo
                break;
            }
        }

        if (esPrimo) {
            printf("%d ", num);
        }
    }

    printf("\n");
    return 0;
}
