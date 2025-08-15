#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matriz_dinamica_2d.h"

/**
 * Versión automática del main para tests sin interacción
 */
int main(void) {
    printf("=== Ejercicio 055: Matriz Dinámica 2D (AUTOMÁTICO) ===\n\n");
    
    // Usar dimensiones predefinidas para tests automáticos
    int filas = 4;
    int columnas = 4;
    
    printf("Usando dimensiones predefinidas: %dx%d\n", filas, columnas);
    
    // Crear matriz dinámicamente
    int **matriz = crear_matriz_2d(filas, columnas);
    if (matriz == NULL) {
        printf("Error: No se pudo crear la matriz.\n");
        return 1;
    }
    
    printf("\nMatriz creada exitosamente (%dx%d)\n", filas, columnas);
    
    // Rellenar con valores automáticos
    printf("\n=== Rellenando matriz con valores automáticos ===\n");
    rellenar_automatico(matriz, filas, columnas);
    printf("Matriz rellenada automáticamente (i*j).\n");
    
    // Mostrar matriz
    printf("\n=== Mostrando matriz ===\n");
    mostrar_matriz(matriz, filas, columnas);
    
    // Obtener estadísticas
    printf("\n=== Estadísticas de la matriz ===\n");
    mostrar_estadisticas(matriz, filas, columnas);
    
    // Buscar algunos valores
    printf("\n=== Buscando valores ===\n");
    buscar_valor(matriz, filas, columnas, 6);  // Debería encontrarlo
    buscar_valor(matriz, filas, columnas, 99); // No debería encontrarlo
    
    // Rellenar con valores aleatorios
    printf("\n=== Rellenando con valores aleatorios ===\n");
    rellenar_aleatorio(matriz, filas, columnas);
    printf("Matriz rellenada con valores aleatorios.\n");
    
    // Mostrar matriz aleatoria
    printf("\n=== Matriz con valores aleatorios ===\n");
    mostrar_matriz(matriz, filas, columnas);
    
    // Estadísticas de matriz aleatoria
    printf("\n=== Estadísticas de matriz aleatoria ===\n");
    mostrar_estadisticas(matriz, filas, columnas);
    
    // Liberar memoria
    liberar_matriz_2d(matriz, filas);
    printf("\nMemoria liberada correctamente.\n");
    
    printf("\n✅ Ejercicio completado exitosamente (versión automática)\n");
    
    return 0;
}

