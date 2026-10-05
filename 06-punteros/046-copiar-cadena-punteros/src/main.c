#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Copia una cadena usando punteros (implementación básica)
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 */
void copiarCadena(char *dest, const char *orig) {
    while (*orig != '\0') {
        *dest = *orig;
        dest++;
        orig++;
    }
    *dest = '\0'; // No olvidar el terminador
}

/**
 * Versión optimizada con incremento en la expresión
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 */
void copiarCadena_optimizada(char *dest, const char *orig) {
    while ((*dest++ = *orig++) != '\0') {
        // El cuerpo del bucle está vacío
        // La copia y el incremento se hacen en la condición
    }
}

/**
 * Versión con retorno del puntero destino (como strcpy estándar)
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 * @return Puntero al inicio del buffer destino
 */
char *copiarCadena_retorno(char *dest, const char *orig) {
    char *inicio = dest; // Guardar el puntero inicial
    
    while (*orig != '\0') {
        *dest++ = *orig++;
    }
    *dest = '\0';
    
    return inicio; // Retornar puntero al inicio
}

/**
 * Versión segura con límite de tamaño
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 * @param tamaño_max Tamaño máximo del buffer destino
 * @return Número de caracteres copiados (sin contar '\0')
 */
size_t copiarCadena_segura(char *dest, const char *orig, size_t tamaño_max) {
    if (dest == NULL || orig == NULL || tamaño_max == 0) {
        return 0;
    }
    
    size_t copiados = 0;
    
    // Dejar espacio para el terminador nulo
    while (*orig != '\0' && copiados < tamaño_max - 1) {
        *dest++ = *orig++;
        copiados++;
    }
    
    *dest = '\0'; // Asegurar terminación nula
    return copiados;
}

/**
 * Versión que maneja overlapping (solapamiento)
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 * @return Puntero al inicio del buffer destino
 */
char *copiarCadena_overlap(char *dest, const char *orig) {
    if (dest == NULL || orig == NULL) {
        return dest;
    }
    
    memmove(dest, orig, strlen(orig) + 1);
    return dest;
}

/**
 * Versión con información de debugging
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 */
void copiarCadena_debug(char *dest, const char *orig) {
    printf("=== Copia de Cadena Paso a Paso ===\n");
    printf("Origen: \"%s\" (dirección: %p)\n", orig, (void *)orig);
    printf("Destino inicial: %p\n", (void *)dest);
    
    char *dest_inicio = dest;
    int contador = 0;
    
    while (*orig != '\0') {
        printf("Paso %d: copiando '%c' de %p a %p\n", 
               ++contador, *orig, (void *)orig, (void *)dest);
        *dest = *orig;
        dest++;
        orig++;
    }
    
    printf("Paso %d: copiando terminador '\\0'\n", ++contador);
    *dest = '\0';
    
    printf("Resultado: \"%s\"\n", dest_inicio);
    printf("Caracteres copiados: %d\n", contador - 1);
    printf("===================================\n\n");
}

/**
 * Copia solo una parte de la cadena (subcadena)
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen (solo lectura)
 * @param inicio Índice de inicio
 * @param longitud Número de caracteres a copiar
 * @return Puntero al inicio del buffer destino
 */
char *copiarSubcadena(char *dest, const char *orig, size_t inicio, size_t longitud) {
    if (dest == NULL || orig == NULL) {
        return dest;
    }
    
    char *dest_inicio = dest;
    
    // Avanzar al punto de inicio
    orig += inicio;
    
    // Copiar la longitud especificada
    size_t copiados = 0;
    while (*orig != '\0' && copiados < longitud) {
        *dest++ = *orig++;
        copiados++;
    }
    
    *dest = '\0';
    return dest_inicio;
}

/**
 * Concatena cadenas usando punteros
 * @param dest Buffer destino que ya contiene una cadena
 * @param orig Cadena a concatenar
 * @return Puntero al inicio del buffer destino
 */
char *concatenarCadena(char *dest, const char *orig) {
    if (dest == NULL || orig == NULL) {
        return dest;
    }
    
    char *inicio = dest;
    
    // Encontrar el final de la cadena destino
    while (*dest != '\0') {
        dest++;
    }
    
    // Copiar la cadena origen al final
    while ((*dest++ = *orig++) != '\0') {
        // Cuerpo vacío
    }
    
    return inicio;
}

/**
 * Cuenta la longitud de una cadena usando punteros
 * @param str Puntero a la cadena
 * @return Longitud de la cadena (sin contar '\0')
 */
size_t longitudCadena(const char *str) {
    const char *inicio = str;
    
    while (*str != '\0') {
        str++;
    }
    
    return str - inicio; // Aritmética de punteros
}

/**
 * Invierte una cadena in-place usando punteros
 * @param str Puntero a la cadena a invertir
 * @return Puntero a la cadena invertida
 */
char *invertirCadena(char *str) {
    if (str == NULL) {
        return str;
    }
    
    char *inicio = str;
    char *fin = str;
    
    // Encontrar el final de la cadena
    while (*fin != '\0') {
        fin++;
    }
    if (fin == str) return str;
    fin--; // Retroceder al último carácter válido
    
    // Intercambiar caracteres desde los extremos hacia el centro
    while (inicio < fin) {
        char temp = *inicio;
        *inicio = *fin;
        *fin = temp;
        inicio++;
        fin--;
    }
    
    return str;
}

/**
 * Función de demostración incorrecta (peligrosa)
 * @param dest Puntero al buffer de destino
 * @param orig Puntero a la cadena origen
 */
void copiarCadena_peligrosa(char *dest, const char *orig) {
    printf("⚠️  FUNCIÓN PELIGROSA - Solo para demostración\n");
    printf("Esta función NO verifica límites del buffer\n\n");
    
    // Esta función es peligrosa porque no verifica el tamaño del destino
    while (*orig != '\0') {
        *dest++ = *orig++;
    }
    *dest = '\0';
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== COPIA DE CADENAS CON PUNTEROS ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("🔹 EJEMPLO BÁSICO:\n");
    char origen[] = "Hola, mundo!";
    char destino[50]; // Asegurarse de que hay suficiente espacio

    copiarCadena(destino, origen);
    printf("Cadena original: \"%s\"\n", origen);
    printf("Cadena copiada: \"%s\"\n\n", destino);

    // Demostración detallada
    printf("🔹 COPIA DETALLADA:\n");
    char origen2[] = "C Programming";
    char destino2[50];
    copiarCadena_debug(destino2, origen2);

    // Versión optimizada
    printf("🔹 VERSIÓN OPTIMIZADA:\n");
    char origen3[] = "Optimized!";
    char destino3[50];
    copiarCadena_optimizada(destino3, origen3);
    printf("Resultado optimizado: \"%s\"\n\n", destino3);

    // Versión con retorno
    printf("🔹 VERSIÓN CON RETORNO:\n");
    char origen4[] = "Return value";
    char destino4[50];
    char *resultado = copiarCadena_retorno(destino4, origen4);
    printf("Retorno de función: \"%s\"\n", resultado);
    printf("¿Mismo puntero? %s\n\n", 
           (resultado == destino4) ? "Sí" : "No");

    // Versión segura
    printf("🔹 VERSIÓN SEGURA CON LÍMITES:\n");
    char origen5[] = "Esta es una cadena muy larga que podría desbordar";
    char destino5[20];
    size_t copiados = copiarCadena_segura(destino5, origen5, sizeof(destino5));
    printf("Cadena original: \"%s\"\n", origen5);
    printf("Cadena truncada: \"%s\"\n", destino5);
    printf("Caracteres copiados: %zu\n\n", copiados);

    // Manejo de casos especiales
    printf("🔹 CASOS ESPECIALES:\n");
    
    // Cadena vacía
    char origen_vacio[] = "";
    char destino_vacio[10];
    copiarCadena(destino_vacio, origen_vacio);
    printf("Cadena vacía copiada: \"%s\" (longitud: %zu)\n", 
           destino_vacio, longitudCadena(destino_vacio));

    // Cadena de un carácter
    char origen_char[] = "A";
    char destino_char[10];
    copiarCadena(destino_char, origen_char);
    printf("Un carácter copiado: \"%s\"\n", destino_char);

    // Cadena con espacios
    char origen_espacios[] = "  Con espacios  ";
    char destino_espacios[50];
    copiarCadena(destino_espacios, origen_espacios);
    printf("Con espacios: \"%s\"\n\n", destino_espacios);

    // Funciones auxiliares
    printf("🔹 FUNCIONES AUXILIARES:\n");
    
    // Subcadena
    char texto[] = "Programación en C";
    char subcadena[50];
    copiarSubcadena(subcadena, texto, 13, 5); // "en C"
    printf("Texto original: \"%s\"\n", texto);
    printf("Subcadena (pos 13, len 5): \"%s\"\n", subcadena);

    // Concatenación
    char base[] = "Hola ";
    char complemento[] = "mundo!";
    char concatenado[50];
    copiarCadena(concatenado, base);
    concatenarCadena(concatenado, complemento);
    printf("Concatenación: \"%s\" + \"%s\" = \"%s\"\n", 
           base, complemento, concatenado);

    // Longitud con punteros
    char para_medir[] = "Medir longitud";
    printf("Longitud de \"%s\": %zu caracteres\n", 
           para_medir, longitudCadena(para_medir));

    // Inversión
    char para_invertir[] = "abcdef";
    printf("Original: \"%s\"\n", para_invertir);
    invertirCadena(para_invertir);
    printf("Invertida: \"%s\"\n\n", para_invertir);

    // Comparación con funciones estándar
    printf("🔹 COMPARACIÓN CON FUNCIONES ESTÁNDAR:\n");
    char test_orig[] = "Test comparison";
    char test_custom[50];
    char test_stdlib[50];
    
    // Nuestra implementación
    copiarCadena(test_custom, test_orig);
    
    // Función estándar
    strcpy(test_stdlib, test_orig);
    
    printf("Original: \"%s\"\n", test_orig);
    printf("Nuestra función: \"%s\"\n", test_custom);
    printf("strcpy estándar: \"%s\"\n", test_stdlib);
    printf("¿Iguales? %s\n\n", 
           (strcmp(test_custom, test_stdlib) == 0) ? "Sí" : "No");

    // Casos de error y validación
    printf("🔹 VALIDACIÓN Y MANEJO DE ERRORES:\n");
    
    // Test con punteros nulos (función segura)
    printf("Test con punteros nulos:\n");
    char buffer_test[20];
    size_t resultado_nulo = copiarCadena_segura(buffer_test, NULL, sizeof(buffer_test));
    printf("Resultado con origen NULL: %zu caracteres\n", resultado_nulo);
    
    resultado_nulo = copiarCadena_segura(NULL, "test", 10);
    printf("Resultado con destino NULL: %zu caracteres\n", resultado_nulo);

    // Test con buffer muy pequeño
    char buffer_pequeño[5];
    char cadena_grande[] = "Esta cadena es demasiado grande";
    size_t truncados = copiarCadena_segura(buffer_pequeño, cadena_grande, sizeof(buffer_pequeño));
    printf("Buffer pequeño: \"%s\" (%zu chars de %zu originales)\n", 
           buffer_pequeño, truncados, strlen(cadena_grande));

    printf("\n🔹 INFORMACIÓN TÉCNICA:\n");
    printf("Tamaño de char*: %zu bytes\n", sizeof(char*));
    printf("Tamaño de char: %zu byte\n", sizeof(char));
    
    // Mostrar direcciones de memoria
    char demo[] = "Demo";
    printf("Dirección de array 'demo': %p\n", (void*)demo);
    printf("Dirección de demo[0]: %p\n", (void*)&demo[0]);
    printf("¿Son iguales? %s\n", (demo == &demo[0]) ? "Sí" : "No");

    // Aritmética de punteros
    printf("demo + 1 apunta a: '%c' (dirección: %p)\n", *(demo + 1), (void*)(demo + 1));
    printf("*(demo + 2) = '%c'\n", *(demo + 2));

    return 0;
}
#endif
