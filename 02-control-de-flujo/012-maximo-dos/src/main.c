/**
 * @file main.c
 * @brief Programa para determinar el mayor de dos números enteros
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que lea dos números introducidos por el usuario 
 * y muestre cuál de los dos es mayor (o si son iguales).
 * 
 * Este ejercicio es ideal para afianzar el uso de estructuras condicionales 
 * if-else y practicar la comparación entre valores. Ayuda a entender cómo 
 * los programas pueden comportarse de forma distinta según la entrada que 
 * reciben, una habilidad básica para cualquier lenguaje de programación.
 */

#include <stdio.h>

int main(void) {
    int a, b;

    // Solicita dos números al usuario
    printf("Introduce el primer número: ");
    scanf("%d", &a);

    printf("Introduce el segundo número: ");
    scanf("%d", &b);

    // Compara los valores
    if (a > b) {
        printf("El mayor es: %d\n", a);
    } else if (b > a) {
        printf("El mayor es: %d\n", b);
    } else {
        printf("Ambos números son iguales.\n");
    }

    return 0;
}
