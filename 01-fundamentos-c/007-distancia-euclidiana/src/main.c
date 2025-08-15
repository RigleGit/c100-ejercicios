/**
 * @file main.c
 * @brief Programa para calcular la distancia euclídea entre dos puntos en el plano cartesiano
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea las coordenadas de dos puntos en el 
 * plano cartesiano y calcule la distancia euclídea entre ellos.
 * 
 * Este ejercicio te permite aplicar fórmulas matemáticas reales en un contexto 
 * de programación. Practicarás el uso de variables tipo float o double, el 
 * cálculo de potencias y raíces cuadradas usando la biblioteca <math.h>, y 
 * entenderás cómo resolver problemas geométricos simples en C.
 */

#include <stdio.h>
#include <math.h>

int main(void) {
    double x1, y1, x2, y2, distancia;

    // Solicita las coordenadas al usuario
    printf("Introduce las coordenadas del primer punto (x1 y1): ");
    scanf("%lf %lf", &x1, &y1);

    printf("Introduce las coordenadas del segundo punto (x2 y2): ");
    scanf("%lf %lf", &x2, &y2);

    // Calcula la distancia euclídea
    distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

    // Muestra el resultado con dos decimales
    printf("La distancia entre los dos puntos es: %.2lf\n", distancia);

    return 0;
}
