/**
 * @file main.c
 * @brief Programa para calcular el factorial de un número usando bucle for
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que calcule el factorial de un número entero 
 * positivo introducido por el usuario, usando un bucle for.
 * 
 * Este ejercicio sirve para introducir los bucles controlados por contador, 
 * en este caso, el cálculo de un factorial n! = 1 × 2 × ... × n, que es un 
 * clásico por su sencillez conceptual y su utilidad para practicar multiplicación 
 * acumulativa. Además, refuerza la necesidad de inicializar correctamente 
 * variables y gestionar adecuadamente los límites del bucle.
 */

#include <stdio.h>

int main(void) {
    int n;
    unsigned long long factorial = 1;

    printf("Introduce un número entero positivo:\n");
    scanf("%d", &n);

    if (n < 0) {
        printf("El factorial no está definido para números negativos.\n");
    } else {
        for (int i = 1; i <= n; ++i) {
            factorial *= i;
        }
        printf("El factorial de %d es %llu\n", n, factorial);
    }

    return 0;
}
