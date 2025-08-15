/**
 * Ejercicio 058: Array de Estructuras - Versión Simple del Enunciado
 * 
 * Este archivo contiene la implementación exacta del enunciado original,
 * sin las extensiones avanzadas del ejercicio completo.
 */

#include <stdio.h>
#include <string.h>

#define MAX_NOMBRE 100
#define MAX_PERSONAS 5

typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
} Persona;

void imprimirMayoresDe(Persona personas[], int tam, int edadLimite) {
    printf("Personas mayores de %d años:\n", edadLimite);
    for (int i = 0; i < tam; i++) {
        if (personas[i].edad > edadLimite) {
            printf("- %s (%d años)\n", personas[i].nombre, personas[i].edad);
        }
    }
}

int main(void) {
    Persona lista[MAX_PERSONAS];

    for (int i = 0; i < MAX_PERSONAS; i++) {
        printf("Introduce el nombre de la persona %d: ", i + 1);
        fgets(lista[i].nombre, MAX_NOMBRE, stdin);
        lista[i].nombre[strcspn(lista[i].nombre, "\n")] = '\0'; // Eliminar salto de línea

        printf("Introduce la edad de %s: ", lista[i].nombre);
        scanf("%d", &lista[i].edad);
        getchar(); // Limpiar el buffer del '\n'
    }

    imprimirMayoresDe(lista, MAX_PERSONAS, 30);

    return 0;
}
