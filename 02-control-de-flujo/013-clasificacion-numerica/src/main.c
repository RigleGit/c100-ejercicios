/**
 * @file main.c
 * @brief Programa para clasificar un número como positivo, negativo o cero
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea un número entero introducido por el 
 * usuario y determine si es positivo, negativo o cero.
 * 
 * Este ejercicio refuerza el uso de estructuras condicionales encadenadas 
 * (if, else if, else), permitiéndote manejar múltiples condiciones posibles 
 * de forma clara. También es una forma de introducir la idea de flujos de 
 * control con múltiples ramas, que será clave para tomar decisiones más 
 * complejas más adelante.
 */

#include <stdio.h>

int main(void) {
    int numero;

    // Solicita el número al usuario (línea propia para lectura por tests)
    printf("Introduce un número entero:\n");
    scanf("%d", &numero);

    // Clasifica el número según su valor
    if (numero > 0) {
        printf("El número es positivo.\n");
    } else if (numero < 0) {
        printf("El número es negativo.\n");
    } else {
        printf("El número es cero.\n");
    }

    return 0;
}
