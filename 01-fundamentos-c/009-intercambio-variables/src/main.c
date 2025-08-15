/**
 * @file main.c
 * @brief Programa para intercambiar los valores de dos variables usando variable auxiliar
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea dos números enteros e intercambie sus 
 * valores utilizando una variable auxiliar. Luego, muestra los valores intercambiados.
 * 
 * Este ejercicio es un clásico en programación y sirve para reforzar la idea de 
 * asignación de valores, uso de variables auxiliares y el orden en el que se 
 * deben realizar las operaciones para no perder datos.
 */

#include <stdio.h>

int main(void) {
    int a, b, temp;

    // Solicita los dos valores al usuario
    printf("Introduce dos números enteros: ");
    scanf("%d %d", &a, &b);

    // Muestra los valores originales
    printf("Antes del intercambio: a = %d, b = %d\n", a, b);

    // Intercambio con variable auxiliar
    temp = a;
    a = b;
    b = temp;

    // Muestra los valores intercambiados
    printf("Después del intercambio: a = %d, b = %d\n", a, b);

    return 0;
}
