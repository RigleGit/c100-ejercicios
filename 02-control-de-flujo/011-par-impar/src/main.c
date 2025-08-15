/**
 * @file main.c
 * @brief Programa para determinar si un número entero es par o impar
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que determine si un número entero 
 * introducido por el usuario es par o impar.
 * 
 * Este ejercicio es un clásico para introducir el uso de estructuras 
 * condicionales. Permite practicar el operador módulo (%) y entender 
 * cómo los programas pueden tomar decisiones en función de los datos 
 * que reciben. Es también una oportunidad para reforzar el control 
 * del flujo con if.
 */

#include <stdio.h>

int main(void) {
    int numero;

    // Solicita un número al usuario (línea aparte para lectura por tests)
    printf("Introduce un número entero:\n");
    scanf("%d", &numero);

    // Verifica si es par o impar
    if (numero % 2 == 0) {
        printf("El número %d es par.\n", numero);
    } else {
        printf("El número %d es impar.\n", numero);
    }

    return 0;
}
