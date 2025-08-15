#include <stdio.h>
#include <math.h>
#include <assert.h>

double raizCuadrada(double x) {
    assert(x >= 0 && "El argumento no puede ser negativo");
    return sqrt(x);
}

int main(void) {
    double valor1 = 25.0;
    double valor2 = -9.0;

    printf("Raíz de %.2f: %.2f\n", valor1, raizCuadrada(valor1));

    // Esta llamada provocará la interrupción del programa si las aserciones están activadas
    printf("Raíz de %.2f: %.2f\n", valor2, raizCuadrada(valor2));

    return 0;
}
