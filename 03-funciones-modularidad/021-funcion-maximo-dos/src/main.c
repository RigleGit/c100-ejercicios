#include <stdio.h>

// Definición de la función que devuelve el mayor de dos enteros
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int main(void) {
    int x, y;

    printf("Introduce el primer número: ");
    scanf("%d", &x);

    printf("Introduce el segundo número: ");
    scanf("%d", &y);

    int resultado = max(x, y);

    printf("El número mayor es: %d\n", resultado);

    return 0;
}
