/**
 * @file main.c
 * @brief Programa para convertir temperatura de Celsius a Fahrenheit
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que convierta una temperatura dada en 
 * grados Celsius a grados Fahrenheit.
 * 
 * Este ejercicio es un clásico en la enseñanza de programación y sirve como 
 * una excelente oportunidad para practicar con variables de tipo decimal 
 * (float o double) y operaciones aritméticas con fórmulas. También refuerza 
 * la lectura de datos mediante scanf y la salida formateada con printf, 
 * incluyendo control sobre los decimales mostrados. Además, introduce al 
 * estudiante a los tipos de datos con punto flotante, necesarios cuando 
 * trabajamos con medidas reales.
 */

#include <stdio.h>

int main(void) {
    float celsius, fahrenheit;

    // Solicita al usuario una temperatura en grados Celsius
    printf("Introduce la temperatura en grados Celsius: ");
    scanf("%f", &celsius);

    // Realiza la conversión a Fahrenheit
    fahrenheit = (celsius * 9 / 5) + 32;

    // Muestra el resultado con un decimal
    printf("%.1f grados Celsius equivalen a %.1f grados Fahrenheit\n", celsius, fahrenheit);

    return 0;
}
