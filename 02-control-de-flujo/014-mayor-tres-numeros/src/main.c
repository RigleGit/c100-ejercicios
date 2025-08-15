/**
 * @file main.c
 * @brief Programa para determinar el mayor de tres números enteros
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea tres números enteros introducidos 
 * por el usuario y determine cuál de ellos es el mayor.
 * 
 * Este ejercicio es ideal para reforzar estructuras condicionales compuestas 
 * y encadenadas, al obligarte a comparar múltiples valores y tomar decisiones 
 * basadas en más de una condición. También es útil para empezar a pensar en 
 * estructuras en cascada, donde una decisión depende de otra anterior.
 */

#include <stdio.h>

int main(void) {
    int a, b, c;

    // Solicita tres números al usuario (imprime salto de línea para tests)
    printf("Introduce tres números enteros separados por espacios:\n");
    scanf("%d %d %d", &a, &b, &c);

    // Determina cuál es el mayor
    if (a >= b && a >= c) {
        printf("El mayor es: %d\n", a);
    } else if (b >= a && b >= c) {
        printf("El mayor es: %d\n", b);
    } else {
        printf("El mayor es: %d\n", c);
    }

    return 0;
}
