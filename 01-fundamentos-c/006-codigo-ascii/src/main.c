/**
 * @file main.c
 * @brief Programa para mostrar el valor ASCII de un carácter introducido por el usuario
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea un carácter introducido por el usuario 
 * y muestre su valor numérico según la tabla ASCII.
 * 
 * Este ejercicio te permite practicar la lectura de caracteres con %c y entender 
 * cómo los caracteres se representan internamente como números enteros. Es un 
 * paso importante para comprender cómo funciona la codificación de texto en C, 
 * y te introduce de manera natural a la conversión entre tipos char e int.
 */

#include <stdio.h>

int main(void) {
    char caracter;

    // Solicita un carácter al usuario
    printf("Introduce un carácter: ");
    // Nota: se podría usar " %c" para ignorar espacios en blanco previos,
    // pero aquí leemos exactamente el carácter (incluido espacio).
    scanf("%c", &caracter);

    // Muestra el valor ASCII del carácter introducido
    printf("El código ASCII de '%c' es: %d\n", caracter, (int)caracter);

    return 0;
}
