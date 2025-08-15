#include <stdio.h>

/**
 * Calcula la longitud de una cadena sin usar strlen()
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
 * Invierte una cadena de caracteres in-place
 * @param cadena La cadena a invertir
 * @param longitud La longitud de la cadena
 */
void invertir_cadena_inplace(char *cadena, int longitud) {
    for (int i = 0; i < longitud / 2; i++) {
        // Intercambiar caracteres desde los extremos hacia el centro
        char temp = cadena[i];
        cadena[i] = cadena[longitud - 1 - i];
        cadena[longitud - 1 - i] = temp;
    }
}

/**
 * Imprime una cadena de forma invertida sin modificar la original
 * @param cadena La cadena a imprimir invertida
 * @param longitud La longitud de la cadena
 */
void imprimir_cadena_invertida(const char *cadena, int longitud) {
    for (int i = longitud - 1; i >= 0; i--) {
        putchar(cadena[i]);
    }
}

/**
 * Copia una cadena de forma invertida a otro buffer
 * @param origen La cadena origen
 * @param destino El buffer destino
 * @param longitud La longitud de la cadena origen
 */
void copiar_cadena_invertida(const char *origen, char *destino, int longitud) {
    for (int i = 0; i < longitud; i++) {
        destino[i] = origen[longitud - 1 - i];
    }
    destino[longitud] = '\0'; // Agregar terminador nulo
}

/**
 * Elimina el salto de línea al final de una cadena si existe
 * @param cadena La cadena a procesar
 * @return La nueva longitud de la cadena
 */
int eliminar_salto_linea(char *cadena) {
    int longitud = calcular_longitud(cadena);
    
    if (longitud > 0 && cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
        longitud--;
    }
    
    return longitud;
}

#ifndef UNIT_TESTING
int main(void) {
    char cadena[100];
    char cadena_copia[100];

    printf("=== Inversor de Cadenas ===\n");
    printf("Introduce una cadena: ");
    
    if (fgets(cadena, sizeof(cadena), stdin) != NULL) {
        // Eliminar el salto de línea si existe
        int longitud = eliminar_salto_linea(cadena);
        
        if (longitud == 0) {
            printf("Cadena vacía. No hay nada que invertir.\n");
            return 0;
        }
        
        printf("\n=== Resultados ===\n");
        printf("Cadena original: \"%s\"\n", cadena);
        printf("Longitud: %d caracteres\n", longitud);
        
        // Método 1: Imprimir invertida sin modificar original
        printf("Cadena invertida (método 1): \"");
        imprimir_cadena_invertida(cadena, longitud);
        printf("\"\n");
        
        // Método 2: Crear copia invertida
        copiar_cadena_invertida(cadena, cadena_copia, longitud);
        printf("Cadena invertida (método 2): \"%s\"\n", cadena_copia);
        
        // Método 3: Invertir in-place (modifica la original)
        printf("\n=== Demostración de inversión in-place ===\n");
        printf("Antes de invertir in-place: \"%s\"\n", cadena);
        invertir_cadena_inplace(cadena, longitud);
        printf("Después de invertir in-place: \"%s\"\n", cadena);
        
        // Información educativa
        printf("\n=== Análisis de caracteres ===\n");
        printf("Posiciones de caracteres en la cadena invertida:\n");
        for (int i = 0; i < longitud && i < 10; i++) { // Limitar a 10 para visualización
            printf("  [%d]: '%c' (ASCII: %d)\n", i, cadena[i], (int)cadena[i]);
        }
        
        if (longitud > 10) {
            printf("  ... (mostrando solo los primeros 10 caracteres)\n");
        }
        
    } else {
        printf("Error al leer la cadena.\n");
        return 1;
    }

    return 0;
}
#endif
