/**
 * @file main.c
 * @brief Programa para calcular el volumen de una esfera a partir de su radio
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que calcule el volumen de una esfera a partir 
 * del valor de su radio. Utiliza la fórmula: V = (4/3) × π × r³
 * 
 * Este ejercicio refuerza el uso de constantes simbólicas, operaciones 
 * aritméticas combinadas y el manejo de variables de tipo float o double. 
 * Es similar al ejercicio del área del círculo, pero aquí se introduce una 
 * fórmula con un operador de potencia y una división no entera, ideal para 
 * afianzar la precedencia de operadores y la precisión numérica.
 */

#include <stdio.h>
#include <math.h>

#define PI 3.14159265359

int main(void) {
    double radio, volumen;

    // Solicita el radio al usuario (en una línea aparte para facilitar pruebas)
    printf("Introduce el radio de la esfera:\n");
    scanf("%lf", &radio);

    // Valida que el radio no sea negativo
    if (radio < 0) {
        printf("Error: El radio no puede ser negativo\n");
        return 1;
    }

    // Calcula el volumen de la esfera
    volumen = (4.0 / 3.0) * PI * pow(radio, 3);

    // Muestra el resultado
    printf("El volumen de la esfera con radio %.2lf es: %.6lf unidades cúbicas\n", radio, volumen);

    return 0;
}
