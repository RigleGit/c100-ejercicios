/**
 * @file main.c
 * @brief Programa para calcular el área de un círculo a partir de su radio
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que calcule el área de un círculo a partir 
 * del valor de su radio.
 * 
 * Este ejercicio refuerza el uso de variables de tipo double, introduce el 
 * uso de la librería <math.h> para realizar cálculos con potencias y promueve 
 * buenas prácticas al declarar constantes simbólicas en lugar de usar valores 
 * "mágicos". En este caso, el valor de π debe declararse de forma explícita 
 * como constante, facilitando la legibilidad, mantenimiento y reutilización 
 * del código.
 */

#include <stdio.h>
#include <math.h>

#define PI 3.1416  // También podrías usar: const double PI = 3.1416;

int main(void) {
    double radio, area;

    // Solicita al usuario el valor del radio
    printf("Introduce el radio del círculo: ");
    scanf("%lf", &radio);

    // Calcula el área usando la fórmula A = π * r^2
    area = PI * pow(radio, 2);

    // Redondeo estable a 2 decimales (mitades hacia arriba) con un pequeño
    // ajuste para evitar que imprecisiones binarias (p.ej. 19.634999999)
    // se redondeen hacia abajo en el borde .005
    double area_redondeada = floor((area + 1e-9) * 100.0 + 0.5) / 100.0;

    // Muestra el resultado con dos decimales
    printf("El área del círculo es %.2f\n", area_redondeada);
    
    return 0;
}
