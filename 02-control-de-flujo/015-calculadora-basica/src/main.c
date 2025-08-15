/**
 * @file main.c
 * @brief Calculadora básica con operaciones aritméticas fundamentales
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea dos números y un operador (+, -, *, /) 
 * introducidos por el usuario, y realice la operación correspondiente mostrando 
 * el resultado. Usa switch para seleccionar la operación.
 * 
 * Este ejercicio tiene como objetivo practicar estructuras condicionales con 
 * switch, una alternativa clara y estructurada al uso de múltiples if/else 
 * cuando se trabaja con múltiples casos concretos. Además, refuerza el uso de 
 * entrada de caracteres, operaciones aritméticas y control de errores básicos.
 */

#include <stdio.h>

int main(void) {
    double num1, num2, resultado;
    char operador;

    // Solicita al usuario los dos números y el operador (salto de línea para lectura por tests)
    printf("Introduce una operación (ejemplo: 4.5 * 2):\n");
    scanf("%lf %c %lf", &num1, &operador, &num2);

    // Evalúa el operador usando switch
    switch (operador) {
        case '+':
            resultado = num1 + num2;
            printf("Resultado: %.2lf\n", resultado);
            break;
        case '-':
            resultado = num1 - num2;
            printf("Resultado: %.2lf\n", resultado);
            break;
        case '*':
            resultado = num1 * num2;
            printf("Resultado: %.2lf\n", resultado);
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
                printf("Resultado: %.2lf\n", resultado);
            } else {
                printf("Error: división por cero.\n");
            }
            break;
        default:
            printf("Operador no válido.\n");
    }

    return 0;
}
