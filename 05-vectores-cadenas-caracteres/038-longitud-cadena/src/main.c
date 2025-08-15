#include <stdio.h>

/**
 * Calcula la longitud de una cadena sin usar strlen()
 * @param cadena La cadena a medir
 * @return La longitud de la cadena (sin incluir el terminador nulo)
 */
int calcular_longitud_cadena(const char *cadena) {
    int longitud = 0;
    const unsigned char* s = (const unsigned char*)cadena;
    
    // Contar puntos de código UTF-8 (caracteres), no bytes
    while (*s != '\0') {
        // Incrementar al inicio de cada secuencia (no byte de continuación 10xxxxxx)
        if ( (*s & 0xC0) != 0x80 ) {
            longitud++;
        }
        s++;
    }
    
    return longitud;
}

/**
 * Calcula la longitud de una cadena ignorando el salto de línea final
 * @param cadena La cadena a medir
 * @return La longitud de la cadena (sin incluir '\n' ni '\0')
 */
int calcular_longitud_sin_newline(const char *cadena) {
    int longitud = 0;
    const unsigned char* s = (const unsigned char*)cadena;
    
    // Recorrer puntos de código hasta '\n' o fin
    while (*s != '\0') {
        if (*s == '\n') {
            break;
        }
        if ( (*s & 0xC0) != 0x80 ) {
            longitud++;
        }
        s++;
    }
    
    return longitud;
}

#ifndef UNIT_TESTING
int main(void) {
    char cadena[100];
    int longitud = 0;

    printf("=== Calculadora de Longitud de Cadenas ===\n");
    printf("Introduce una cadena: ");
    
    // Leer la cadena con fgets para mayor seguridad
    if (fgets(cadena, sizeof(cadena), stdin) != NULL) {
        // Método 1: Contar hasta '\0' o '\n' (como en el enunciado)
        longitud = calcular_longitud_sin_newline(cadena);
        printf("La longitud de la cadena es: %d\n", longitud);
        
        // Método 2: Mostrar también la longitud total (incluyendo '\n')
        int longitud_total = calcular_longitud_cadena(cadena);
        printf("Longitud total (incluyendo '\\n' si existe): %d\n", longitud_total);
        
        // Información adicional para aprendizaje
        printf("\n=== Análisis de la cadena ===\n");
        printf("Cadena introducida: \"%s\"", cadena);
        printf("Caracteres analizados:\n");
        
        for (int i = 0; i < longitud && i < 20; i++) { // Limitar a 20 chars para visualización
            printf("  [%d]: '%c' (ASCII: %d)\n", i, cadena[i], (int)cadena[i]);
        }
        
        if (longitud > 20) {
            printf("  ... (mostrando solo los primeros 20 caracteres)\n");
        }
        
        // Mostrar el terminador
        if (cadena[longitud] == '\n') {
            printf("  [%d]: '\\n' (ASCII: %d)\n", longitud, (int)cadena[longitud]);
        }
        printf("  [%d]: '\\0' (ASCII: %d) <- Terminador nulo\n", 
               longitud_total, (int)cadena[longitud_total]);
    } else {
        printf("Error al leer la cadena.\n");
        return 1;
    }

    return 0;
}
#endif
