/**
 * @file main.c
 * @brief Programa para sumar dos números enteros introducidos por el usuario
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que pida al usuario dos números enteros, 
 * los sume y muestre el resultado en pantalla.
 * 
 * Este ejercicio tiene como objetivo reforzar el uso de entrada y salida 
 * estándar en C, introduciendo una nueva función: scanf, que permite leer 
 * datos proporcionados por el usuario. Es también una forma práctica de 
 * comenzar a trabajar con variables, operaciones aritméticas y el flujo 
 * básico de interacción entre el usuario y el programa. Aunque la operación 
 * es sencilla, el ejercicio introduce el concepto de almacenar datos en 
 * memoria y manipularlos mediante expresiones, dos pilares fundamentales 
 * en programación.
 */

#include <stdio.h>

int main(void) {
    int a, b, suma;

    // Solicita al usuario que introduzca dos números enteros
    printf("Introduce el primer número: ");
    scanf("%d", &a);

    printf("Introduce el segundo número: ");
    scanf("%d", &b);

    // Realiza la suma de los dos números
    suma = a + b;

    // Muestra el resultado
    printf("La suma de %d y %d es %d\n", a, b, suma);

    return 0;
}
