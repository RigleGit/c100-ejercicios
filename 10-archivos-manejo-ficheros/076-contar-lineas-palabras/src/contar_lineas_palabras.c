#include "contar_lineas_palabras.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

void inicializar_estadisticas(EstadisticasArchivo* stats) {
    if (stats) {
        stats->lineas = 0;
        stats->palabras = 0;
        stats->caracteres = 0;
        stats->bytes = 0;
    }
}

bool es_separador_palabra(char c) {
    return isspace((unsigned char)c) || c == '\t' || c == '\n' || c == '\r';
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
    bool en_palabra = false;
    int c;
    while ((c = fgetc(archivo)) != EOF) {
        if (stats->caracteres == INT_MAX || stats->bytes == INT_MAX) {
            fclose(archivo);
            return false;
        }
        stats->caracteres++;
        stats->bytes++;
        if (c == '\n') stats->lineas++;
        if (isalnum((unsigned char)c) || c == '_' || c == '-') {
            if (!en_palabra) {
                en_palabra = true;
                stats->palabras++;
            }
        } else {
            en_palabra = false;
        }
    }
    bool ok = !ferror(archivo);
    if (fclose(archivo) != 0) ok = false;
    return ok;
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
    int c;

    while ((c = fgetc(archivo)) != EOF) {
        caracteres++;

        if (c == '\n') {
            lineas++;
        }

        if (isspace((unsigned char)c)) {
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
