#include <stdio.h>

int main(void) {
    int numero, suma = 0;

    printf("Introduce un número entero positivo: ");
    scanf("%d", &numero);

    if (numero < 0) {
        printf("El número debe ser positivo.\n");
    } else {
        while (numero > 0) {
            suma += numero % 10;   // Extrae el último dígito
            numero /= 10;          // Elimina el último dígito
        }
        printf("La suma de los dígitos es: %d\n", suma);
    }

    return 0;
}
