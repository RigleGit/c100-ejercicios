#include <stdio.h>

#define FILAS 3
#define COLUMNAS 3

#ifndef UNIT_TESTING
int main(void) {
    int A[FILAS][COLUMNAS];
    int B[FILAS][COLUMNAS];
    int resultado[FILAS][COLUMNAS];

    printf("Introduce los elementos de la primera matriz (3x3):\n");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("A[%d][%d]: ", i, j);
            scanf("%d", &A[i][j]);
        }
    }

    printf("Introduce los elementos de la segunda matriz (3x3):\n");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("B[%d][%d]: ", i, j);
            scanf("%d", &B[i][j]);
        }
    }

    // Suma de las dos matrices
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            resultado[i][j] = A[i][j] + B[i][j];
        }
    }

    // Mostrar la matriz resultante
    printf("Matriz resultante de la suma:\n");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%d\t", resultado[i][j]);
        }
        printf("\n");
    }

    return 0;
}
#endif
