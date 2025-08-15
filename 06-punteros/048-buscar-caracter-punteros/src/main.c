#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Busca la primera ocurrencia de un carácter en una cadena
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @return Puntero a la primera ocurrencia o NULL si no se encuentra
 */
char* buscarCaracter(char *str, char c) {
    while (*str != '\0') {
        if (*str == c) {
            return str; // Retorna un puntero a la posición del carácter encontrado
        }
        str++;
    }
    return NULL; // No se encontró el carácter
}

/**
 * Busca la primera ocurrencia con información de debug
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @return Puntero a la primera ocurrencia o NULL si no se encuentra
 */
char* buscarCaracter_debug(char *str, char c) {
    char *inicio = str;
    int posicion = 0;
    
    printf("=== Búsqueda detallada del carácter '%c' ===\n", c);
    printf("Cadena: \"%s\"\n", str);
    printf("Dirección base: %p\n\n", (void *)str);
    
    while (*str != '\0') {
        printf("Posición %d:\n", posicion);
        printf("  Dirección: %p\n", (void *)str);
        printf("  Carácter: '%c' (ASCII: %d)\n", *str, *str);
        printf("  ¿Es '%c'? %s\n", c, (*str == c) ? "SÍ" : "NO");
        
        if (*str == c) {
            printf("  *** ENCONTRADO ***\n");
            printf("Offset desde inicio: %ld bytes\n", str - inicio);
            printf("========================================\n\n");
            return str;
        }
        
        str++;
        posicion++;
        printf("\n");
    }
    
    printf("Fin de cadena alcanzado (\\0)\n");
    printf("Carácter '%c' NO encontrado\n", c);
    printf("========================================\n\n");
    return NULL;
}

/**
 * Busca la última ocurrencia de un carácter en una cadena
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @return Puntero a la última ocurrencia o NULL si no se encuentra
 */
char* buscarUltimoCaracter(char *str, char c) {
    char *ultimo = NULL;
    
    while (*str != '\0') {
        if (*str == c) {
            ultimo = str;  // Actualizar la última posición encontrada
        }
        str++;
    }
    
    return ultimo;
}

/**
 * Busca un carácter ignorando mayúsculas/minúsculas
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @return Puntero a la primera ocurrencia o NULL si no se encuentra
 */
char* buscarCaracterInsensible(char *str, char c) {
    char c_lower = tolower(c);
    char c_upper = toupper(c);
    
    while (*str != '\0') {
        if (*str == c || *str == c_lower || *str == c_upper) {
            return str;
        }
        str++;
    }
    return NULL;
}

/**
 * Busca un carácter solo en letras (ignora números y símbolos)
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @return Puntero a la primera ocurrencia en letra o NULL si no se encuentra
 */
char* buscarCaracterEnLetras(char *str, char c) {
    while (*str != '\0') {
        if (isalpha(*str) && *str == c) {
            return str;
        }
        str++;
    }
    return NULL;
}

/**
 * Busca un carácter desde una posición específica
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @param desde Posición desde donde empezar (0-indexada)
 * @return Puntero a la primera ocurrencia desde la posición o NULL
 */
char* buscarCaracterDesde(char *str, char c, int desde) {
    int longitud = strlen(str);
    
    if (desde < 0 || desde >= longitud) {
        return NULL;  // Posición inválida
    }
    
    return buscarCaracter(str + desde, c);
}

/**
 * Cuenta todas las ocurrencias de un carácter
 * @param str Cadena donde contar
 * @param c Carácter a contar
 * @return Número de ocurrencias
 */
int contarCaracter(char *str, char c) {
    int contador = 0;
    
    while (*str != '\0') {
        if (*str == c) {
            contador++;
        }
        str++;
    }
    
    return contador;
}

/**
 * Encuentra todas las posiciones de un carácter
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 * @param posiciones Array donde guardar las posiciones (debe tener suficiente espacio)
 * @param max_posiciones Máximo número de posiciones a guardar
 * @return Número de posiciones encontradas
 */
int encontrarTodasPosiciones(char *str, char c, int posiciones[], int max_posiciones) {
    int encontradas = 0;
    int posicion = 0;
    
    while (*str != '\0' && encontradas < max_posiciones) {
        if (*str == c) {
            posiciones[encontradas] = posicion;
            encontradas++;
        }
        str++;
        posicion++;
    }
    
    return encontradas;
}

/**
 * Busca cualquiera de los caracteres de un conjunto
 * @param str Cadena donde buscar
 * @param conjunto Conjunto de caracteres a buscar
 * @return Puntero al primer carácter del conjunto encontrado o NULL
 */
char* buscarCualquierCaracter(char *str, const char *conjunto) {
    while (*str != '\0') {
        const char *c = conjunto;
        while (*c != '\0') {
            if (*str == *c) {
                return str;
            }
            c++;
        }
        str++;
    }
    return NULL;
}

/**
 * Busca un carácter que no esté en un conjunto dado
 * @param str Cadena donde buscar
 * @param conjunto Conjunto de caracteres a evitar
 * @return Puntero al primer carácter no presente en conjunto o NULL
 */
char* buscarCaracterNoEn(char *str, const char *conjunto) {
    while (*str != '\0') {
        const char *c = conjunto;
        int encontrado = 0;
        
        while (*c != '\0') {
            if (*str == *c) {
                encontrado = 1;
                break;
            }
            c++;
        }
        
        if (!encontrado) {
            return str;  // Carácter no está en el conjunto
        }
        str++;
    }
    return NULL;
}

/**
 * Busca el primer dígito en una cadena
 * @param str Cadena donde buscar
 * @return Puntero al primer dígito o NULL si no hay dígitos
 */
char* buscarPrimerDigito(char *str) {
    while (*str != '\0') {
        if (isdigit(*str)) {
            return str;
        }
        str++;
    }
    return NULL;
}

/**
 * Busca el primer carácter no blanco (espacio, tab, etc.)
 * @param str Cadena donde buscar
 * @return Puntero al primer carácter no blanco o NULL si solo hay blancos
 */
char* buscarPrimerNoBlanco(char *str) {
    while (*str != '\0') {
        if (!isspace(*str)) {
            return str;
        }
        str++;
    }
    return NULL;
}

/**
 * Reemplaza todas las ocurrencias de un carácter por otro
 * @param str Cadena a modificar
 * @param buscar Carácter a buscar
 * @param reemplazar Carácter de reemplazo
 * @return Número de reemplazos realizados
 */
int reemplazarCaracter(char *str, char buscar, char reemplazar) {
    int reemplazos = 0;
    
    while (*str != '\0') {
        if (*str == buscar) {
            *str = reemplazar;
            reemplazos++;
        }
        str++;
    }
    
    return reemplazos;
}

/**
 * Compara dos implementaciones de búsqueda
 * @param str Cadena donde buscar
 * @param c Carácter a buscar
 */
void compararImplementaciones(char *str, char c) {
    printf("🔹 COMPARACIÓN DE IMPLEMENTACIONES:\n\n");
    
    // Nuestra implementación
    char *resultado_nuestro = buscarCaracter(str, c);
    
    // Función estándar strchr
    char *resultado_estandar = strchr(str, c);
    
    printf("Cadena: \"%s\"\n", str);
    printf("Buscando: '%c'\n\n", c);
    
    printf("Nuestra implementación:\n");
    if (resultado_nuestro != NULL) {
        printf("  Encontrado en posición: %ld\n", resultado_nuestro - str);
        printf("  Dirección: %p\n", (void *)resultado_nuestro);
        printf("  Carácter: '%c'\n", *resultado_nuestro);
    } else {
        printf("  No encontrado\n");
    }
    
    printf("\nFunción estándar strchr:\n");
    if (resultado_estandar != NULL) {
        printf("  Encontrado en posición: %ld\n", resultado_estandar - str);
        printf("  Dirección: %p\n", (void *)resultado_estandar);
        printf("  Carácter: '%c'\n", *resultado_estandar);
    } else {
        printf("  No encontrado\n");
    }
    
    printf("\n¿Resultados iguales? %s\n\n", 
           (resultado_nuestro == resultado_estandar) ? "SÍ" : "NO");
}

/**
 * Demuestra diferentes formas de buscar con punteros
 * @param str Cadena de prueba
 * @param c Carácter a buscar
 */
void demoBusquedas(char *str, char c) {
    printf("🔹 DIFERENTES MÉTODOS DE BÚSQUEDA:\n\n");
    printf("Cadena: \"%s\"\n", str);
    printf("Buscando: '%c'\n\n", c);
    
    // Búsqueda básica
    char *resultado = buscarCaracter(str, c);
    printf("1. Búsqueda básica:\n");
    if (resultado != NULL) {
        printf("   Encontrado en posición %ld: '%c'\n", resultado - str, *resultado);
    } else {
        printf("   No encontrado\n");
    }
    
    // Última ocurrencia
    char *ultimo = buscarUltimoCaracter(str, c);
    printf("2. Última ocurrencia:\n");
    if (ultimo != NULL) {
        printf("   Encontrado en posición %ld: '%c'\n", ultimo - str, *ultimo);
    } else {
        printf("   No encontrado\n");
    }
    
    // Búsqueda insensible a mayúsculas
    char *insensible = buscarCaracterInsensible(str, c);
    printf("3. Búsqueda insensible a mayúsculas:\n");
    if (insensible != NULL) {
        printf("   Encontrado en posición %ld: '%c'\n", insensible - str, *insensible);
    } else {
        printf("   No encontrado\n");
    }
    
    // Contar ocurrencias
    int contador = contarCaracter(str, c);
    printf("4. Contar ocurrencias: %d\n", contador);
    
    // Encontrar todas las posiciones
    int posiciones[50];
    int encontradas = encontrarTodasPosiciones(str, c, posiciones, 50);
    printf("5. Todas las posiciones (%d encontradas): ", encontradas);
    for (int i = 0; i < encontradas; i++) {
        printf("%d ", posiciones[i]);
    }
    printf("\n\n");
}

/**
 * Demuestra búsquedas avanzadas
 * @param str Cadena de prueba
 */
void demoBusquedasAvanzadas(char *str) {
    printf("🔹 BÚSQUEDAS AVANZADAS:\n\n");
    printf("Cadena: \"%s\"\n\n", str);
    
    // Buscar primer dígito
    char *digito = buscarPrimerDigito(str);
    printf("Primer dígito:\n");
    if (digito != NULL) {
        printf("  Encontrado en posición %ld: '%c'\n", digito - str, *digito);
    } else {
        printf("  No se encontraron dígitos\n");
    }
    
    // Buscar primer no-blanco
    char *no_blanco = buscarPrimerNoBlanco(str);
    printf("Primer carácter no-blanco:\n");
    if (no_blanco != NULL) {
        printf("  Encontrado en posición %ld: '%c'\n", no_blanco - str, *no_blanco);
    } else {
        printf("  Solo caracteres en blanco\n");
    }
    
    // Buscar vocales
    char *vocal = buscarCualquierCaracter(str, "aeiouAEIOU");
    printf("Primera vocal:\n");
    if (vocal != NULL) {
        printf("  Encontrado en posición %ld: '%c'\n", vocal - str, *vocal);
    } else {
        printf("  No se encontraron vocales\n");
    }
    
    // Buscar consonante
    char *consonante = buscarCaracterNoEn(str, "aeiouAEIOU 0123456789.,!?");
    printf("Primera consonante:\n");
    if (consonante != NULL) {
        printf("  Encontrado en posición %ld: '%c'\n", consonante - str, *consonante);
    } else {
        printf("  No se encontraron consonantes\n");
    }
    printf("\n");
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== BUSCAR CARÁCTER CON PUNTEROS ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("🔹 EJEMPLO BÁSICO:\n");
    char texto[] = "Hola mundo";
    char objetivo = 'm';

    char *resultado = buscarCaracter(texto, objetivo);

    if (resultado != NULL) {
        printf("Carácter '%c' encontrado en la posición: %ld\n", 
               objetivo, resultado - texto);
        printf("Dirección del carácter encontrado: %p\n", (void *)resultado);
        printf("Resto de la cadena desde aquí: \"%s\"\n\n", resultado);
    } else {
        printf("Carácter '%c' no encontrado.\n\n", objetivo);
    }
    
    // Demostración detallada
    printf("🔹 ANÁLISIS DETALLADO:\n");
    char cadena_demo[] = "Programacion";
    buscarCaracter_debug(cadena_demo, 'a');
    
    // Diferentes métodos de búsqueda
    demoBusquedas("Hola mundo cruel", 'o');
    
    // Búsquedas avanzadas
    demoBusquedasAvanzadas("  Hola123 mundo!");
    
    // Comparación con función estándar
    compararImplementaciones("testing", 't');
    compararImplementaciones("hello", 'x');
    
    // Casos especiales
    printf("🔹 CASOS ESPECIALES:\n\n");
    
    // Buscar el terminador
    char *terminador = buscarCaracter("test", '\0');
    printf("Buscando terminador \\0:\n");
    if (terminador != NULL) {
        printf("  Encontrado en posición: %ld\n", terminador - "test");
    } else {
        printf("  No encontrado (esto no debería pasar)\n");
    }
    
    // Cadena vacía
    char *en_vacia = buscarCaracter("", 'a');
    printf("Buscar en cadena vacía: %s\n", (en_vacia == NULL) ? "NULL" : "Encontrado");
    
    // Buscar espacio
    char *espacio = buscarCaracter("Hola mundo", ' ');
    printf("Buscar espacio:\n");
    if (espacio != NULL) {
        printf("  Encontrado en posición: %ld\n", espacio - "Hola mundo");
    }
    
    // Primer y último carácter
    char cadena_test[] = "abcdefghijklmnop";
    char *primer_char = buscarCaracter(cadena_test, 'a');
    char *ultimo_char = buscarUltimoCaracter(cadena_test, 'p');
    printf("Primer carácter 'a': posición %ld\n", primer_char - cadena_test);
    printf("Último carácter 'p': posición %ld\n\n", ultimo_char - cadena_test);
    
    // Demostración de modificación de cadenas
    printf("🔹 MODIFICACIÓN DE CADENAS:\n");
    char cadena_modificable[] = "Hola mundo mundo";
    printf("Original: \"%s\"\n", cadena_modificable);
    
    int reemplazos = reemplazarCaracter(cadena_modificable, 'o', '0');
    printf("Después de reemplazar 'o' con '0': \"%s\"\n", cadena_modificable);
    printf("Número de reemplazos: %d\n\n", reemplazos);
    
    // Búsqueda desde posición específica
    printf("🔹 BÚSQUEDA DESDE POSICIÓN ESPECÍFICA:\n");
    char cadena_multiple[] = "banana";
    printf("Cadena: \"%s\"\n", cadena_multiple);
    
    char *primera_a = buscarCaracter(cadena_multiple, 'a');
    printf("Primera 'a': posición %ld\n", primera_a - cadena_multiple);
    
    char *segunda_a = buscarCaracterDesde(cadena_multiple, 'a', 
                                         (primera_a - cadena_multiple) + 1);
    if (segunda_a != NULL) {
        printf("Segunda 'a': posición %ld\n", segunda_a - cadena_multiple);
    }
    
    char *tercera_a = buscarCaracterDesde(cadena_multiple, 'a', 
                                         (segunda_a - cadena_multiple) + 1);
    if (tercera_a != NULL) {
        printf("Tercera 'a': posición %ld\n", tercera_a - cadena_multiple);
    }
    printf("\n");
    
    // Estadísticas de caracteres
    printf("🔹 ESTADÍSTICAS DE CARACTERES:\n");
    char frase[] = "El gato subió al tejado";
    printf("Frase: \"%s\"\n\n", frase);
    
    char caracteres_frecuentes[] = "aeiou ";
    for (int i = 0; caracteres_frecuentes[i] != '\0'; i++) {
        char c = caracteres_frecuentes[i];
        int count = contarCaracter(frase, c);
        if (c == ' ') {
            printf("Espacios: %d\n", count);
        } else {
            printf("'%c': %d ocurrencias\n", c, count);
        }
    }
    
    printf("\n🔹 CONCEPTOS CLAVE DEMOSTRADOS:\n");
    printf("✓ Recorrido de cadenas con punteros\n");
    printf("✓ Retorno de punteros desde funciones\n");
    printf("✓ Detección del terminador \\0\n");
    printf("✓ Cálculo de posiciones con aritmética de punteros\n");
    printf("✓ Búsqueda con diferentes criterios\n");
    printf("✓ Manejo de casos especiales (NULL, cadena vacía)\n");
    printf("✓ Modificación de cadenas in-situ\n");
    printf("✓ Búsqueda múltiple y estadísticas\n");
    printf("✓ Equivalencia con funciones estándar (strchr)\n");

    return 0;
}
#endif
