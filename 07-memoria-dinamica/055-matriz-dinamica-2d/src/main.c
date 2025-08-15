#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matriz_dinamica_2d.h"

int main(void) {
    printf("=== Ejercicio 055: Matriz Dinámica 2D ===\n\n");
    
    int filas, columnas;
    printf("Introduce el número de filas: ");
    scanf("%d", &filas);
    printf("Introduce el número de columnas: ");
    scanf("%d", &columnas);
    
    if (filas <= 0 || columnas <= 0) {
        printf("Error: Las dimensiones deben ser positivas.\n");
        return 1;
    }
    
    // Crear matriz dinámicamente
    int **matriz = crear_matriz_2d(filas, columnas);
    if (matriz == NULL) {
        printf("Error: No se pudo crear la matriz.\n");
        return 1;
    }
    
    printf("\nMatriz creada exitosamente (%dx%d)\n", filas, columnas);
    
    // Menú de opciones
    int opcion;
    do {
        printf("\n--- Opciones ---\n");
        printf("1. Rellenar con valores manuales\n");
        printf("2. Rellenar con valores automáticos (i*j)\n");
        printf("3. Rellenar con valores aleatorios\n");
        printf("4. Mostrar matriz\n");
        printf("5. Obtener estadísticas\n");
        printf("6. Buscar valor\n");
        printf("0. Salir\n");
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1:
                rellenar_manual(matriz, filas, columnas);
                break;
            case 2:
                rellenar_automatico(matriz, filas, columnas);
                break;
            case 3:
                rellenar_aleatorio(matriz, filas, columnas);
                break;
            case 4:
                mostrar_matriz(matriz, filas, columnas);
                break;
            case 5:
                mostrar_estadisticas(matriz, filas, columnas);
                break;
            case 6: {
                int valor;
                printf("Introduce el valor a buscar: ");
                scanf("%d", &valor);
                buscar_valor(matriz, filas, columnas, valor);
                break;
            }
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
    
    // Liberar memoria
    liberar_matriz_2d(matriz, filas);
    printf("Memoria liberada correctamente.\n");
    
    return 0;
}
