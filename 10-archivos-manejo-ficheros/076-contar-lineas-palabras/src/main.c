#include "contar_lineas_palabras.h"
#include <stdio.h>

// Función principal mejorada
int main(void) {
    const char* nombre_archivo = "entrada.txt";
    EstadisticasArchivo stats;

    printf("=== Contador de líneas, palabras y caracteres ===\n\n");

    // Intentar análisis básico
    if (contar_archivo_basico(nombre_archivo, &stats)) {
        printf("Análisis básico:\n");
        imprimir_estadisticas(&stats, nombre_archivo);
        printf("\nFormato wc:\n");
        imprimir_formato_wc(&stats, nombre_archivo);
    } else {
        printf("Error: No se pudo abrir el archivo '%s'\n", nombre_archivo);
        printf("Creando un archivo de ejemplo...\n");
        
        // Crear archivo de ejemplo para demostración
        FILE* ejemplo = fopen(nombre_archivo, "w");
        if (ejemplo) {
            fprintf(ejemplo, "Hola mundo\n");
            fprintf(ejemplo, "Este es un archivo de prueba\n");
            fprintf(ejemplo, "con varias líneas y palabras.\n");
            fprintf(ejemplo, "\n");
            fprintf(ejemplo, "¡Funciona perfectamente!\n");
            fclose(ejemplo);
            
            printf("Archivo de ejemplo creado. Analizando...\n\n");
            
            if (contar_archivo_basico(nombre_archivo, &stats)) {
                printf("Análisis del archivo de ejemplo:\n");
                imprimir_estadisticas(&stats, nombre_archivo);
                printf("\nFormato wc:\n");
                imprimir_formato_wc(&stats, nombre_archivo);
            }
        }
    }

    return 0;
}
