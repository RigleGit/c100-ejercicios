#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matriz_dinamica_2d.h"

// Crear matriz dinámica 2D
int** crear_matriz_2d(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0) {
        return NULL;
    }
    
    // Reservar memoria para array de punteros a filas
    int **matriz = (int**)malloc(filas * sizeof(int*));
    if (matriz == NULL) {
        return NULL;
    }
    
    // Reservar memoria para cada fila
    for (int i = 0; i < filas; i++) {
        matriz[i] = (int*)malloc(columnas * sizeof(int));
        if (matriz[i] == NULL) {
            // Si falla, liberar memoria ya reservada
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    
    return matriz;
}

// Liberar memoria de la matriz 2D
void liberar_matriz_2d(int** matriz, int filas) {
    if (matriz == NULL) {
        return;
    }
    
    // Liberar cada fila
    for (int i = 0; i < filas; i++) {
        free(matriz[i]);
    }
    
    // Liberar array de punteros
    free(matriz);
}

// Rellenar matriz manualmente
void rellenar_manual(int** matriz, int filas, int columnas) {
    printf("Introduce los valores de la matriz:\n");
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("Elemento [%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }
    printf("Matriz rellenada manualmente.\n");
}

// Rellenar matriz automáticamente (i*j)
void rellenar_automatico(int** matriz, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = i * j;
        }
    }
    printf("Matriz rellenada automáticamente (i*j).\n");
}

// Rellenar matriz con valores aleatorios
void rellenar_aleatorio(int** matriz, int filas, int columnas) {
    srand(time(NULL));
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = rand() % 100; // Números del 0 al 99
        }
    }
    printf("Matriz rellenada con valores aleatorios (0-99).\n");
}

// Mostrar matriz
void mostrar_matriz(int** matriz, int filas, int columnas) {
    printf("\nMatriz [%dx%d]:\n", filas, columnas);
    
    // Calcular ancho máximo para formateo
    int max_ancho = 3; // Mínimo 3 caracteres
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int ancho = snprintf(NULL, 0, "%d", matriz[i][j]);
            if (ancho > max_ancho) {
                max_ancho = ancho;
            }
        }
    }
    
    // Mostrar matriz formateada
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%*d ", max_ancho, matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Calcular y mostrar estadísticas de la matriz
void mostrar_estadisticas(int** matriz, int filas, int columnas) {
    if (filas == 0 || columnas == 0) {
        printf("Matriz vacía.\n");
        return;
    }
    
    int suma = 0;
    int min = matriz[0][0];
    int max = matriz[0][0];
    int pos_min_i = 0, pos_min_j = 0;
    int pos_max_i = 0, pos_max_j = 0;
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int valor = matriz[i][j];
            suma += valor;
            
            if (valor < min) {
                min = valor;
                pos_min_i = i;
                pos_min_j = j;
            }
            
            if (valor > max) {
                max = valor;
                pos_max_i = i;
                pos_max_j = j;
            }
        }
    }
    
    int total_elementos = filas * columnas;
    double promedio = (double)suma / total_elementos;
    
    printf("\n--- Estadísticas de la Matriz ---\n");
    printf("Dimensiones: %dx%d (%d elementos)\n", filas, columnas, total_elementos);
    printf("Suma total: %d\n", suma);
    printf("Promedio: %.2f\n", promedio);
    printf("Valor mínimo: %d en posición [%d][%d]\n", min, pos_min_i, pos_min_j);
    printf("Valor máximo: %d en posición [%d][%d]\n", max, pos_max_i, pos_max_j);
}

// Buscar un valor en la matriz
void buscar_valor(int** matriz, int filas, int columnas, int valor) {
    int encontrados = 0;
    
    printf("\nBuscando el valor %d en la matriz:\n", valor);
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] == valor) {
                printf("Encontrado en posición [%d][%d]\n", i, j);
                encontrados++;
            }
        }
    }
    
    if (encontrados == 0) {
        printf("El valor %d no se encontró en la matriz.\n", valor);
    } else {
        printf("Se encontraron %d ocurrencias del valor %d.\n", encontrados, valor);
    }
}

// Versión básica para tests: crear matriz simple
int** crear_matriz_basica(int filas, int columnas, int valor_inicial) {
    int **matriz = crear_matriz_2d(filas, columnas);
    if (matriz == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = valor_inicial;
        }
    }
    
    return matriz;
}

// Obtener elemento de la matriz
int obtener_elemento(int** matriz, int fila, int columna) {
    return matriz[fila][columna];
}

// Establecer elemento de la matriz
void establecer_elemento(int** matriz, int fila, int columna, int valor) {
    matriz[fila][columna] = valor;
}

// Copiar matriz
int** copiar_matriz(int** origen, int filas, int columnas) {
    int **copia = crear_matriz_2d(filas, columnas);
    if (copia == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            copia[i][j] = origen[i][j];
        }
    }
    
    return copia;
}
