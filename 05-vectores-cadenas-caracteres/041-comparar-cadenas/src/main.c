#include <stdio.h>

/**
 * Elimina el salto de línea al final de una cadena si existe
 * @param cadena La cadena a procesar
 * @return La nueva longitud de la cadena
 */
int eliminar_salto_linea(char *cadena) {
    int i = 0;
    
    while (cadena[i] != '\0') {
        if (cadena[i] == '\n') {
            cadena[i] = '\0';
            break;
        }
        i++;
    }
    
    return i;
}

/**
 * Compara dos cadenas carácter por carácter sin usar strcmp
 * @param cadena1 Primera cadena
 * @param cadena2 Segunda cadena
 * @return 1 si son iguales, 0 si son diferentes
 */
int comparar_cadenas(const char *cadena1, const char *cadena2) {
    int i = 0;
    
    // Comparar carácter por carácter hasta encontrar diferencia o fin
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        if (cadena1[i] != cadena2[i]) {
            return 0; // Son diferentes
        }
        i++;
    }
    
    // Verificar que ambas cadenas terminaron al mismo tiempo
    return (cadena1[i] == '\0' && cadena2[i] == '\0') ? 1 : 0;
}

/**
 * Compara cadenas y retorna valor similar a strcmp
 * @param cadena1 Primera cadena
 * @param cadena2 Segunda cadena
 * @return < 0 si cadena1 < cadena2, 0 si iguales, > 0 si cadena1 > cadena2
 */
int comparar_cadenas_ordenado(const char *cadena1, const char *cadena2) {
    int i = 0;
    
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        if (cadena1[i] != cadena2[i]) {
            return cadena1[i] - cadena2[i];
        }
        i++;
    }
    
    // Si llegamos aquí, una puede ser prefijo de la otra
    return cadena1[i] - cadena2[i];
}

/**
 * Compara cadenas ignorando mayúsculas y minúsculas
 * @param cadena1 Primera cadena
 * @param cadena2 Segunda cadena
 * @return 1 si son iguales (ignorando caso), 0 si son diferentes
 */
int comparar_cadenas_sin_caso(const char *cadena1, const char *cadena2) {
    int i = 0;
    
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        char c1 = cadena1[i];
        char c2 = cadena2[i];
        
        // Convertir a minúsculas si es mayúscula
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 = c1 + ('a' - 'A');
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 = c2 + ('a' - 'A');
        }
        
        if (c1 != c2) {
            return 0; // Son diferentes
        }
        i++;
    }
    
    return (cadena1[i] == '\0' && cadena2[i] == '\0') ? 1 : 0;
}

/**
 * Calcula la longitud de una cadena
 * @param cadena La cadena a medir
 * @return La longitud de la cadena
 */
int calcular_longitud(const char *cadena) {
    int longitud = 0;
    while (cadena[longitud] != '\0') {
        longitud++;
    }
    return longitud;
}

/**
 * Encuentra la primera diferencia entre dos cadenas
 * @param cadena1 Primera cadena
 * @param cadena2 Segunda cadena
 * @return La posición de la primera diferencia, -1 si son iguales
 */
int encontrar_primera_diferencia(const char *cadena1, const char *cadena2) {
    int i = 0;
    
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        if (cadena1[i] != cadena2[i]) {
            return i;
        }
        i++;
    }
    
    // Si una cadena es más larga que la otra
    if (cadena1[i] != cadena2[i]) {
        return i;
    }
    
    return -1; // Son iguales
}

#ifndef UNIT_TESTING
int main(void) {
    char cadena1[100], cadena2[100];

    printf("=== Comparador de Cadenas ===\n");
    printf("Introduce la primera cadena: ");
    
    if (fgets(cadena1, sizeof(cadena1), stdin) == NULL) {
        printf("Error al leer la primera cadena.\n");
        return 1;
    }
    
    printf("Introduce la segunda cadena: ");
    
    if (fgets(cadena2, sizeof(cadena2), stdin) == NULL) {
        printf("Error al leer la segunda cadena.\n");
        return 1;
    }

    // Eliminar saltos de línea
    int longitud1 = eliminar_salto_linea(cadena1);
    int longitud2 = eliminar_salto_linea(cadena2);

    printf("\n=== Resultados de Comparación ===\n");
    printf("Cadena 1: \"%s\" (longitud: %d)\n", cadena1, longitud1);
    printf("Cadena 2: \"%s\" (longitud: %d)\n", cadena2, longitud2);

    // Comparación básica de igualdad
    int son_iguales = comparar_cadenas(cadena1, cadena2);
    printf("\n1. Comparación exacta: %s\n", 
           son_iguales ? "Las cadenas son IGUALES" : "Las cadenas son DIFERENTES");

    // Comparación ordenada (como strcmp)
    int resultado_orden = comparar_cadenas_ordenado(cadena1, cadena2);
    printf("2. Comparación ordenada: ");
    if (resultado_orden < 0) {
        printf("\"%s\" es MENOR que \"%s\"\n", cadena1, cadena2);
    } else if (resultado_orden > 0) {
        printf("\"%s\" es MAYOR que \"%s\"\n", cadena1, cadena2);
    } else {
        printf("Las cadenas son IGUALES\n");
    }

    // Comparación sin distinguir mayúsculas/minúsculas
    int iguales_sin_caso = comparar_cadenas_sin_caso(cadena1, cadena2);
    printf("3. Comparación sin caso: %s\n", 
           iguales_sin_caso ? "Las cadenas son IGUALES (ignorando mayús/minús)" 
                            : "Las cadenas son DIFERENTES (aún ignorando mayús/minús)");

    // Análisis detallado si son diferentes
    if (!son_iguales) {
        int primera_diff = encontrar_primera_diferencia(cadena1, cadena2);
        printf("\n=== Análisis de Diferencias ===\n");
        
        if (primera_diff >= 0) {
            printf("Primera diferencia en posición %d:\n", primera_diff);
            
            if (primera_diff < longitud1 && primera_diff < longitud2) {
                printf("  Cadena 1[%d]: '%c' (ASCII: %d)\n", 
                       primera_diff, cadena1[primera_diff], (int)cadena1[primera_diff]);
                printf("  Cadena 2[%d]: '%c' (ASCII: %d)\n", 
                       primera_diff, cadena2[primera_diff], (int)cadena2[primera_diff]);
            } else if (primera_diff >= longitud1) {
                printf("  Cadena 1 terminó, pero cadena 2 continúa con '%c'\n", 
                       cadena2[primera_diff]);
            } else {
                printf("  Cadena 2 terminó, pero cadena 1 continúa con '%c'\n", 
                       cadena1[primera_diff]);
            }
        }
        
        printf("Diferencia de longitudes: %d\n", longitud1 - longitud2);
    }

    // Mostrar comparación carácter por carácter para propósitos educativos
    printf("\n=== Comparación Carácter por Carácter ===\n");
    int max_longitud = (longitud1 > longitud2) ? longitud1 : longitud2;
    int mostrar_hasta = (max_longitud > 15) ? 15 : max_longitud; // Limitar visualización
    
    for (int i = 0; i < mostrar_hasta; i++) {
        char c1 = (i < longitud1) ? cadena1[i] : '\0';
        char c2 = (i < longitud2) ? cadena2[i] : '\0';
        
        printf("Pos %2d: ", i);
        
        if (i < longitud1) {
            printf("'%c'(%3d)", c1, (int)c1);
        } else {
            printf("   (fin)");
        }
        
        printf(" vs ");
        
        if (i < longitud2) {
            printf("'%c'(%3d)", c2, (int)c2);
        } else {
            printf("   (fin)");
        }
        
        printf(" -> %s\n", (c1 == c2) ? "✓" : "✗");
    }
    
    if (max_longitud > 15) {
        printf("... (mostrando solo los primeros 15 caracteres)\n");
    }

    return 0;
}
#endif
