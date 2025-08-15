/**
 * 039 - Contar Vocales en Cadena
 * 
 * Programa que cuenta el número de vocales (a, e, i, o, u) en una cadena
 * de caracteres, sin distinguir entre mayúsculas y minúsculas.
 * 
 * Autor: C100 Ejercicios
 * Fecha: 2024
 */

#include <stdio.h>
#include <string.h>

/**
 * Función para verificar si un carácter es una vocal
 * @param c Carácter a verificar
 * @return 1 si es vocal, 0 si no lo es
 */
int es_vocal(char c) {
    // Versión ASCII básica
    if (c >= 'A' && c <= 'Z') {
        c = c + ('a' - 'A');
    }
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

/**
 * Función para contar vocales en una cadena
 * @param cadena Cadena de caracteres a analizar
 * @return Número de vocales encontradas
 */
int contar_vocales(const char *cadena) {
    if (cadena == NULL) {
        return 0;
    }
    int contador = 0;
    const unsigned char *s = (const unsigned char *)cadena;
    while (*s != '\0') {
        if ((*s & 0x80) == 0) {
            // ASCII
            if (es_vocal((char)*s)) contador++;
            s++;
            continue;
        }
        // UTF-8: detectar vocales acentuadas comunes en español
        if (*s == 0xC3) {
            unsigned char b = *(s + 1);
            if (b == 0x81 || b == 0xA1) contador++; // Á, á
            else if (b == 0x89 || b == 0xA9) contador++; // É, é
            else if (b == 0x8D || b == 0xAD) contador++; // Í, í
            else if (b == 0x93 || b == 0xB3) contador++; // Ó, ó
            else if (b == 0x9A || b == 0xBA || b == 0x9C || b == 0xBC) contador++; // Ú, ú, Ü, ü
            s += 2;
            continue;
        }
        // Avanzar al menos un byte para evitar bucle infinito en otros caracteres Unicode
        s++;
    }
    return contador;
}

/**
 * Función para contar cada tipo de vocal por separado
 * @param cadena Cadena a analizar
 * @param a Puntero para contar 'a'
 * @param e Puntero para contar 'e'
 * @param i Puntero para contar 'i'
 * @param o Puntero para contar 'o'
 * @param u Puntero para contar 'u'
 */
void contar_vocales_detallado(const char *cadena, int *a, int *e, int *i, int *o, int *u) {
    if (a == NULL || e == NULL || i == NULL || o == NULL || u == NULL) {
        return;
    }
    
    // Inicializar contadores
    *a = *e = *i = *o = *u = 0;
    
    if (cadena == NULL) {
        return;
    }
    const unsigned char *s = (const unsigned char *)cadena;
    while (*s != '\0') {
        if ((*s & 0x80) == 0) {
            unsigned char uc = *s;
            if (uc >= 'A' && uc <= 'Z') uc = (unsigned char)(uc + ('a' - 'A'));
            if (uc == 'a') (*a)++;
            else if (uc == 'e') (*e)++;
            else if (uc == 'i') (*i)++;
            else if (uc == 'o') (*o)++;
            else if (uc == 'u') (*u)++;
            s++;
            continue;
        }
        if (*s == 0xC3) {
            unsigned char b = *(s + 1);
            if (b == 0x81 || b == 0xA1) (*a)++; // Á, á
            else if (b == 0x89 || b == 0xA9) (*e)++; // É, é
            else if (b == 0x8D || b == 0xAD) (*i)++; // Í, í
            else if (b == 0x93 || b == 0xB3) (*o)++; // Ó, ó
            else if (b == 0x9A || b == 0xBA || b == 0x9C || b == 0xBC) (*u)++; // Ú, ú, Ü, ü
            s += 2;
            continue;
        }
        s++;
    }
}

/**
 * Función para mostrar el menú de opciones
 */
void mostrar_menu(void) {
    printf("\n=== CONTADOR DE VOCALES ===\n");
    printf("1. Contar total de vocales\n");
    printf("2. Contar vocales detallado\n");
    printf("3. Análisis de múltiples cadenas\n");
    printf("4. Salir\n");
    printf("Selecciona una opción: ");
}

/**
 * Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    char cadena[1000];
    int opcion;
    
    printf("=== EJERCICIO 039: CONTAR VOCALES EN CADENA ===\n");
    
    do {
        mostrar_menu();
        scanf("%d", &opcion);
        getchar(); // Consumir el salto de línea
        
        switch (opcion) {
            case 1: {
                printf("\nIngresa una cadena: ");
                fgets(cadena, sizeof(cadena), stdin);
                
                // Remover salto de línea si existe
                size_t len = strlen(cadena);
                if (len > 0 && cadena[len-1] == '\n') {
                    cadena[len-1] = '\0';
                }
                
                int total = contar_vocales(cadena);
                printf("La cadena \"%s\" tiene %d vocal(es)\n", cadena, total);
                break;
            }
            
            case 2: {
                printf("\nIngresa una cadena: ");
                fgets(cadena, sizeof(cadena), stdin);
                
                // Remover salto de línea si existe
                size_t len = strlen(cadena);
                if (len > 0 && cadena[len-1] == '\n') {
                    cadena[len-1] = '\0';
                }
                
                int a, e, i, o, u;
                contar_vocales_detallado(cadena, &a, &e, &i, &o, &u);
                
                printf("\nAnálisis detallado de \"%s\":\n", cadena);
                printf("  a: %d\n", a);
                printf("  e: %d\n", e);
                printf("  i: %d\n", i);
                printf("  o: %d\n", o);
                printf("  u: %d\n", u);
                printf("Total: %d vocales\n", a + e + i + o + u);
                break;
            }
            
            case 3: {
                const char *ejemplos[] = {
                    "Hola Mundo",
                    "PROGRAMACIÓN",
                    "aeiou",
                    "bcdfg",
                    "El murciélago voló hacia el cielo azul",
                    "12345!@#$%"
                };
                int num_ejemplos = sizeof(ejemplos) / sizeof(ejemplos[0]);
                
                printf("\nAnálisis de cadenas de ejemplo:\n");
                for (int j = 0; j < num_ejemplos; j++) {
                    int total = contar_vocales(ejemplos[j]);
                    printf("  \"%s\" → %d vocal(es)\n", ejemplos[j], total);
                }
                break;
            }
            
            case 4:
                printf("\n¡Hasta luego!\n");
                break;
                
            default:
                printf("\nOpción no válida. Por favor, elige 1-4.\n");
                break;
        }
        
    } while (opcion != 4);
    
    return 0;
}
#endif
