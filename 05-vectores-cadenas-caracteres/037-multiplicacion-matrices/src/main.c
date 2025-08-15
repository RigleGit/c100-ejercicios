#include <stdio.h>

#define FILAS_A 2
#define COLUMNAS_A 3
#define FILAS_B 3
#define COLUMNAS_B 2

#ifndef UNIT_TESTING
int main(void) {
    int A[FILAS_A][COLUMNAS_A] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int B[FILAS_B][COLUMNAS_B] = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    int resultado[FILAS_A][COLUMNAS_B] = {0};

    // Multiplicación de matrices
    for (int i = 0; i < FILAS_A; i++) {
        for (int j = 0; j < COLUMNAS_B; j++) {
            for (int k = 0; k < COLUMNAS_A; k++) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Mostrar la matriz resultante
    printf("Resultado de la multiplicación:\n");
    for (int i = 0; i < FILAS_A; i++) {
        for (int j = 0; j < COLUMNAS_B; j++) {
            printf("%d\t", resultado[i][j]);
        }
        printf("\n");
    }

    return 0;
}
#endif
