#include <stdio.h>

// Función que calcula la potencia de forma iterativa
double potencia(double base, int exp) {
    double resultado = 1.0;

    if (exp == 0) {
        return 1.0;
    } else if (exp > 0) {
        for (int i = 0; i < exp; i++) {
            resultado *= base;
        }
    } else {
        for (int i = 0; i < -exp; i++) {
            resultado *= base;
        }
        resultado = 1.0 / resultado;
    }

    return resultado;
}

int main(void) {
    double base;
    int exponente;

    printf("Introduce la base: ");
    scanf("%lf", &base);
    printf("Introduce el exponente (entero): ");
    scanf("%d", &exponente);

    double resultado = potencia(base, exponente);

    printf("%.2f elevado a %d es: %.5f\n", base, exponente, resultado);

    return 0;
}
