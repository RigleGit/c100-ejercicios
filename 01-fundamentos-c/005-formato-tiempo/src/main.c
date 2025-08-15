/**
 * @file main.c
 * @brief Programa para convertir segundos totales a formato horas:minutos:segundos
 * @author Curso C - 100 Ejercicios
 * 
 * Enunciado: Escribe un programa que reciba una cantidad total de segundos 
 * e imprima su equivalente en horas, minutos y segundos.
 * 
 * Este ejercicio te ayuda a practicar operaciones con enteros, especialmente 
 * la división y el operador módulo (%), que son esenciales para descomponer 
 * cantidades en partes. También te introduce al formato de salida más 
 * estructurado, útil en aplicaciones como temporizadores, cronómetros o 
 * sistemas de registro.
 */

#include <stdio.h>

int main(void) {
    int total_segundos, horas, minutos, segundos;

    // Solicita al usuario el total de segundos
    printf("Introduce el total de segundos: ");
    scanf("%d", &total_segundos);

    // Calcula las horas, minutos y segundos
    horas = total_segundos / 3600;
    minutos = (total_segundos % 3600) / 60;
    segundos = total_segundos % 60;

    // Muestra el resultado en formato hh:mm:ss
    printf("Equivalente: %02d:%02d:%02d\n", horas, minutos, segundos);

    return 0;
}
