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
 * Busca una subcadena dentro de otra cadena (implementación básica)
 * @param cadena La cadena principal donde buscar
 * @param subcadena La subcadena a encontrar
 * @return La posición donde comienza la subcadena, o -1 si no se encuentra
 */
int buscar_subcadena(const char *cadena, const char *subcadena) {
    int i, j;
    
    // Si la subcadena está vacía, se considera encontrada en posición 0
    if (subcadena[0] == '\0') {
        return 0;
    }
    
    for (i = 0; cadena[i] != '\0'; i++) {
        // Intentar hacer match desde la posición i
        for (j = 0; subcadena[j] != '\0'; j++) {
            if (cadena[i + j] == '\0' || cadena[i + j] != subcadena[j]) {
                break;  // No coincide o se acabó la cadena principal
            }
        }
        
        // Si llegamos al final de la subcadena, la encontramos
        if (subcadena[j] == '\0') {
            return i;
        }
    }
    
    return -1;  // No encontrada
}

/**
 * Busca todas las ocurrencias de una subcadena
 * @param cadena La cadena principal donde buscar
 * @param subcadena La subcadena a encontrar
 * @param posiciones Array para almacenar las posiciones encontradas
 * @param max_posiciones Máximo número de posiciones a almacenar
 * @return El número de ocurrencias encontradas
 */
int buscar_todas_ocurrencias(const char *cadena, const char *subcadena, 
                            int *posiciones, int max_posiciones) {
    int count = 0;
    int inicio = 0;
    int posicion;
    
    if (subcadena[0] == '\0') {
        return 0;  // Subcadena vacía
    }
    
    while (count < max_posiciones) {
        posicion = buscar_subcadena(cadena + inicio, subcadena);
        
        if (posicion == -1) {
            break;  // No más ocurrencias
        }
        
        posiciones[count] = inicio + posicion;
        count++;
        inicio = inicio + posicion + 1;  // Buscar desde la siguiente posición
    }
    
    return count;
}

/**
 * Busca una subcadena ignorando mayúsculas y minúsculas
 * @param cadena La cadena principal donde buscar
 * @param subcadena La subcadena a encontrar
 * @return La posición donde comienza la subcadena, o -1 si no se encuentra
 */
int buscar_subcadena_sin_caso(const char *cadena, const char *subcadena) {
    int i, j;
    
    if (subcadena[0] == '\0') {
        return 0;
    }
    
    for (i = 0; cadena[i] != '\0'; i++) {
        for (j = 0; subcadena[j] != '\0'; j++) {
            if (cadena[i + j] == '\0') {
                break;  // Se acabó la cadena principal
            }
            
            char c1 = cadena[i + j];
            char c2 = subcadena[j];
            
            // Convertir a minúsculas para comparar
            if (c1 >= 'A' && c1 <= 'Z') {
                c1 = c1 + ('a' - 'A');
            }
            if (c2 >= 'A' && c2 <= 'Z') {
                c2 = c2 + ('a' - 'A');
            }
            
            if (c1 != c2) {
                break;  // No coincide
            }
        }
        
        if (subcadena[j] == '\0') {
            return i;  // Encontrada
        }
    }
    
    return -1;  // No encontrada
}

/**
 * Busca una subcadena desde el final hacia adelante
 * @param cadena La cadena principal donde buscar
 * @param subcadena La subcadena a encontrar
 * @return La posición de la última ocurrencia, o -1 si no se encuentra
 */
int buscar_ultima_ocurrencia(const char *cadena, const char *subcadena) {
    int longitud_cadena = calcular_longitud(cadena);
    int longitud_subcadena = calcular_longitud(subcadena);
    int i, j;
    
    if (longitud_subcadena == 0) {
        return longitud_cadena;  // Subcadena vacía se encuentra al final
    }
    
    if (longitud_subcadena > longitud_cadena) {
        return -1;  // Subcadena más larga que la cadena principal
    }
    
    // Buscar desde el final hacia adelante
    for (i = longitud_cadena - longitud_subcadena; i >= 0; i--) {
        for (j = 0; j < longitud_subcadena; j++) {
            if (cadena[i + j] != subcadena[j]) {
                break;
            }
        }
        
        if (j == longitud_subcadena) {
            return i;  // Encontrada
        }
    }
    
    return -1;  // No encontrada
}

/**
 * Cuenta el número de ocurrencias de una subcadena
 * @param cadena La cadena principal donde buscar
 * @param subcadena La subcadena a contar
 * @return El número de ocurrencias encontradas
 */
int contar_ocurrencias(const char *cadena, const char *subcadena) {
    int count = 0;
    int inicio = 0;
    int posicion;
    
    if (subcadena[0] == '\0') {
        return 0;  // Subcadena vacía
    }
    
    while (1) {
        posicion = buscar_subcadena(cadena + inicio, subcadena);
        
        if (posicion == -1) {
            break;  // No más ocurrencias
        }
        
        count++;
        inicio = inicio + posicion + 1;  // Buscar desde la siguiente posición
    }
    
    return count;
}

#ifndef UNIT_TESTING
int main(void) {
    char texto[200], subcadena[100];
    int posiciones[20];  // Para almacenar hasta 20 ocurrencias

    printf("=== Buscador de Subcadenas ===\n");
    printf("Introduce el texto principal: ");
    
    if (fgets(texto, sizeof(texto), stdin) == NULL) {
        printf("Error al leer el texto principal.\n");
        return 1;
    }
    
    printf("Introduce la subcadena a buscar: ");
    
    if (fgets(subcadena, sizeof(subcadena), stdin) == NULL) {
        printf("Error al leer la subcadena.\n");
        return 1;
    }

    // Eliminar saltos de línea
    int longitud_texto = eliminar_salto_linea(texto);
    int longitud_subcadena = eliminar_salto_linea(subcadena);

    printf("\n=== Datos de Entrada ===\n");
    printf("Texto principal: \"%s\" (longitud: %d)\n", texto, longitud_texto);
    printf("Subcadena a buscar: \"%s\" (longitud: %d)\n", subcadena, longitud_subcadena);

    // Validar entrada
    if (longitud_subcadena == 0) {
        printf("\nLa subcadena está vacía. No hay nada que buscar.\n");
        return 0;
    }

    if (longitud_texto == 0) {
        printf("\nEl texto principal está vacío. No se puede buscar nada.\n");
        return 0;
    }

    printf("\n=== Resultados de Búsqueda ===\n");

    // 1. Búsqueda básica (primera ocurrencia)
    int primera_posicion = buscar_subcadena(texto, subcadena);
    if (primera_posicion != -1) {
        printf("✓ Primera ocurrencia encontrada en posición %d\n", primera_posicion);
    } else {
        printf("✗ Subcadena no encontrada\n");
    }

    // 2. Búsqueda de última ocurrencia
    int ultima_posicion = buscar_ultima_ocurrencia(texto, subcadena);
    if (ultima_posicion != -1) {
        printf("✓ Última ocurrencia encontrada en posición %d\n", ultima_posicion);
    } else {
        printf("✗ No hay última ocurrencia (no encontrada)\n");
    }

    // 3. Contar todas las ocurrencias
    int total_ocurrencias = contar_ocurrencias(texto, subcadena);
    printf("📊 Total de ocurrencias: %d\n", total_ocurrencias);

    // 4. Buscar todas las posiciones
    if (total_ocurrencias > 0) {
        int num_encontradas = buscar_todas_ocurrencias(texto, subcadena, posiciones, 20);
        printf("📍 Posiciones encontradas: ");
        for (int i = 0; i < num_encontradas; i++) {
            printf("%d", posiciones[i]);
            if (i < num_encontradas - 1) printf(", ");
        }
        printf("\n");
        
        if (num_encontradas >= 20) {
            printf("   (máximo 20 posiciones mostradas)\n");
        }
    }

    // 5. Búsqueda sin distinguir mayúsculas/minúsculas
    int posicion_sin_caso = buscar_subcadena_sin_caso(texto, subcadena);
    if (posicion_sin_caso != -1) {
        printf("✓ Encontrada ignorando mayús/minús en posición %d\n", posicion_sin_caso);
    } else {
        printf("✗ No encontrada ni ignorando mayús/minús\n");
    }

    // 6. Análisis detallado (solo si se encontró)
    if (primera_posicion != -1) {
        printf("\n=== Análisis Detallado ===\n");
        printf("Contexto de la primera ocurrencia:\n");
        
        // Mostrar contexto (10 caracteres antes y después)
        int inicio_contexto = (primera_posicion >= 10) ? primera_posicion - 10 : 0;
        int fin_contexto = primera_posicion + longitud_subcadena + 10;
        if (fin_contexto > longitud_texto) {
            fin_contexto = longitud_texto;
        }
        
        printf("Posición: ");
        for (int i = inicio_contexto; i < fin_contexto; i++) {
            printf("%2d ", i);
        }
        printf("\n");
        
        printf("Texto:    ");
        for (int i = inicio_contexto; i < fin_contexto; i++) {
            printf(" %c ", texto[i]);
        }
        printf("\n");
        
        printf("Match:    ");
        for (int i = inicio_contexto; i < fin_contexto; i++) {
            if (i >= primera_posicion && i < primera_posicion + longitud_subcadena) {
                printf(" ^ ");
            } else {
                printf("   ");
            }
        }
        printf("\n");
        
        printf("\nCadena coincidente: \"");
        for (int i = 0; i < longitud_subcadena; i++) {
            printf("%c", texto[primera_posicion + i]);
        }
        printf("\"\n");
    }

    return 0;
}
#endif
