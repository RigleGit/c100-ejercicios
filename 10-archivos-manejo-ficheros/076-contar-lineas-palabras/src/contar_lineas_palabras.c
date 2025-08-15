#include "contar_lineas_palabras.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void inicializar_estadisticas(EstadisticasArchivo* stats) {
    if (stats) {
        stats->lineas = 0;
        stats->palabras = 0;
        stats->caracteres = 0;
        stats->bytes = 0;
    }
}

bool es_separador_palabra(char c) {
    return isspace(c) || c == '\t' || c == '\n' || c == '\r';
}

bool contar_archivo_basico(const char* nombre_archivo, EstadisticasArchivo* stats) {
    if (!nombre_archivo || !stats) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        return false;
    }

    inicializar_estadisticas(stats);
    bool en_palabra = false;
    int c;

    while ((c = fgetc(archivo)) != EOF) {
        stats->caracteres++;

        // Contar líneas
        if (c == '\n') {
            stats->lineas++;
        }

        // Contar palabras
        if (es_separador_palabra(c)) {
            en_palabra = false;
        } else if (!en_palabra) {
            en_palabra = true;
            stats->palabras++;
        }
    }

    fclose(archivo);
    return true;
}

bool contar_archivo_completo(const char* nombre_archivo, EstadisticasArchivo* stats) {
    if (!nombre_archivo || !stats) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "rb"); // Modo binario para contar bytes exactos
    if (!archivo) {
        return false;
    }

    inicializar_estadisticas(stats);
    bool en_palabra = false;
    int c;

    while ((c = fgetc(archivo)) != EOF) {
        stats->bytes++;

        // Solo contar como carácter si es válido (no binario)
        if (c >= 0 && c <= 127) {
            stats->caracteres++;
        }

        // Contar líneas
        if (c == '\n') {
            stats->lineas++;
        }

        // Contar palabras
        if (es_separador_palabra(c)) {
            en_palabra = false;
        } else if (!en_palabra && isprint(c)) {
            en_palabra = true;
            stats->palabras++;
        }
    }

    fclose(archivo);
    return true;
}

bool contar_desde_stream(FILE* archivo, EstadisticasArchivo* stats) {
    if (!archivo || !stats) {
        return false;
    }

    inicializar_estadisticas(stats);
    bool en_palabra = false;
    int c;
    long pos_inicial = ftell(archivo);

    while ((c = fgetc(archivo)) != EOF) {
        stats->caracteres++;

        // Contar líneas
        if (c == '\n') {
            stats->lineas++;
        }

        // Contar palabras
        if (es_separador_palabra(c)) {
            en_palabra = false;
        } else if (!en_palabra) {
            en_palabra = true;
            stats->palabras++;
        }
    }

    // Restaurar posición del archivo si es posible
    if (pos_inicial >= 0) {
        fseek(archivo, pos_inicial, SEEK_SET);
    }

    return true;
}

bool contar_palabras_robusto(const char* nombre_archivo, EstadisticasArchivo* stats) {
    if (!nombre_archivo || !stats) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        return false;
    }

    inicializar_estadisticas(stats);
    char buffer[1024];
    bool en_palabra = false;

    while (fgets(buffer, sizeof(buffer), archivo)) {
        stats->lineas++;
        
        for (int i = 0; buffer[i] != '\0'; i++) {
            stats->caracteres++;

            // Lógica más robusta para palabras
            if (isalnum(buffer[i]) || buffer[i] == '_' || buffer[i] == '-') {
                if (!en_palabra) {
                    en_palabra = true;
                    stats->palabras++;
                }
            } else {
                en_palabra = false;
            }
        }
    }

    fclose(archivo);
    return true;
}

void imprimir_estadisticas(const EstadisticasArchivo* stats, const char* nombre_archivo) {
    if (!stats) {
        return;
    }

    printf("=== Estadísticas del archivo ===\n");
    if (nombre_archivo) {
        printf("Archivo: %s\n", nombre_archivo);
    }
    printf("Líneas:     %d\n", stats->lineas);
    printf("Palabras:   %d\n", stats->palabras);
    printf("Caracteres: %d\n", stats->caracteres);
    if (stats->bytes > 0) {
        printf("Bytes:      %d\n", stats->bytes);
    }
    printf("============================\n");
}

void imprimir_formato_wc(const EstadisticasArchivo* stats, const char* nombre_archivo) {
    if (!stats) {
        return;
    }

    // Formato similar a wc: líneas palabras caracteres archivo
    printf("%8d %8d %8d", stats->lineas, stats->palabras, stats->caracteres);
    if (nombre_archivo) {
        printf(" %s", nombre_archivo);
    }
    printf("\n");
}

// Función principal del usuario (versión básica)
int main_usuario_basico(void) {
    FILE *archivo = fopen("entrada.txt", "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    int caracteres = 0, palabras = 0, lineas = 0;
    int en_palabra = 0;
    char c;

    while ((c = fgetc(archivo)) != EOF) {
        caracteres++;

        if (c == '\n') {
            lineas++;
        }

        if (isspace(c)) {
            en_palabra = 0;
        } else if (!en_palabra) {
            en_palabra = 1;
            palabras++;
        }
    }

    fclose(archivo);

    printf("Líneas: %d\n", lineas);
    printf("Palabras: %d\n", palabras);
    printf("Caracteres: %d\n", caracteres);

    return 0;
}

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
