/**
 * @file main.c
 * @brief Programa para calcular el promedio de tres números reales
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea tres números reales ingresados por 
 * el usuario y calcule su promedio.
 * 
 * Este ejercicio refuerza el manejo de entrada múltiple, operaciones con 
 * números reales y precisión en divisiones. Es ideal para practicar cómo 
 * evitar errores comunes al mezclar enteros y flotantes, y cómo presentar 
 * resultados con formato adecuado.
 */

#include <stdio.h>

int main(void) {
    float num1, num2, num3, promedio;

    // Solicita los tres números al usuario
    printf("Introduce tres números (pueden tener decimales): ");
    scanf("%f %f %f", &num1, &num2, &num3);

    // Calcula el promedio
    promedio = (num1 + num2 + num3) / 3;

    // Muestra el resultado con dos decimales
    printf("El promedio es: %.2f\n", promedio);

    return 0;
}
