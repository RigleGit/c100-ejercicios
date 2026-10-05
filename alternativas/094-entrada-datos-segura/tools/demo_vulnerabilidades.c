/**
 * @file demo_vulnerabilidades.c
 * @brief Demostración educativa de vulnerabilidades en entrada de datos
 * 
 * ADVERTENCIA: Este código contiene vulnerabilidades INTENCIONALMENTE
 * para propósitos educativos. NO usar en producción.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "entrada_segura.h"

/**
 * @brief Demostración de vulnerabilidad con gets() - PELIGROSA
 * 
 * Esta función demuestra por qué gets() es extremadamente peligrosa
 * y cómo puede causar buffer overflow fácilmente.
 */
static void demo_gets_vulnerable(void) {
    printf("\n" COLOR_ROJO "=== DEMOSTRACIÓN: gets() VULNERABLE ===" COLOR_RESET "\n");
    printf(COLOR_AMARILLO "ADVERTENCIA: Esta demostración es PELIGROSA y solo para educación\n" COLOR_RESET);
    printf("gets() no verifica límites del buffer - NUNCA usarla\n\n");
    
    char buffer_pequeno[10];  // Buffer muy pequeño intencionalmente
    
    printf("Buffer de solo 10 bytes. Ingresa más de 9 caracteres:\n");
    printf("Entrada: ");
    fflush(stdout);
    
    // Simulamos gets() de forma más segura para la demo
    char input_demo[1000];
    if (fgets(input_demo, sizeof(input_demo), stdin) != NULL) {
        // Removemos newline
        input_demo[strcspn(input_demo, "\n")] = '\0';
        
        printf("\nTamaño de entrada: %zu bytes\n", strlen(input_demo));
        printf("Tamaño del buffer: %zu bytes\n", sizeof(buffer_pequeno));
        
        if (strlen(input_demo) >= sizeof(buffer_pequeno)) {
            printf(COLOR_ROJO "¡BUFFER OVERFLOW! gets() habría corrompido memoria\n" COLOR_RESET);
            printf("Bytes que se saldrían: %zu\n", strlen(input_demo) - sizeof(buffer_pequeno) + 1);
        } else {
            strcpy(buffer_pequeno, input_demo);
            printf(COLOR_VERDE "Entrada cabe en el buffer: '%s'\n" COLOR_RESET, buffer_pequeno);
        }
    }
    
    printf("\nEsto es lo que gets() habría hecho:\n");
    printf("  gets(buffer_pequeno);  // ❌ PELIGROSO\n");
    printf("  // No verifica si la entrada cabe en el buffer\n");
    printf("  // Puede corromper variables adyacentes\n");
    printf("  // Puede permitir ejecución de código malicioso\n");
}

/**
 * @brief Demostración de scanf sin límites - PELIGROSA
 */
static void demo_scanf_vulnerable(void) {
    printf("\n" COLOR_ROJO "=== DEMOSTRACIÓN: scanf() SIN LÍMITES ===" COLOR_RESET "\n");
    printf("scanf() sin especificar límites puede causar overflow\n\n");
    
    char buffer[20];
    
    printf("Buffer de 20 bytes. Ingresa una palabra (prueba con >19 caracteres):\n");
    printf("Entrada: ");
    fflush(stdout);
    
    // Simulamos scanf inseguro de forma más controlada
    char input_demo[1000];
    if (fgets(input_demo, sizeof(input_demo), stdin) != NULL) {
        input_demo[strcspn(input_demo, "\n")] = '\0';
        
        printf("\nTamaño de entrada: %zu bytes\n", strlen(input_demo));
        printf("Tamaño del buffer: %zu bytes\n", sizeof(buffer));
        
        if (strlen(input_demo) >= sizeof(buffer)) {
            printf(COLOR_ROJO "¡OVERFLOW! scanf(\"%s\", buffer) habría fallado\n" COLOR_RESET, "%s");
            printf("Forma insegura:   scanf(\"%s\", buffer);     // ❌ PELIGROSO\n", "%s");
            printf("Forma segura:     scanf(\"%19s\", buffer);   // ✅ SEGURO\n");
        } else {
            strcpy(buffer, input_demo);
            printf(COLOR_VERDE "Entrada segura: '%s'\n" COLOR_RESET, buffer);
        }
    }
}

/**
 * @brief Demostración de strcpy sin validación - PELIGROSA
 */
static void demo_strcpy_vulnerable(void) {
    printf("\n" COLOR_ROJO "=== DEMOSTRACIÓN: strcpy() SIN VALIDACIÓN ===" COLOR_RESET "\n");
    printf("strcpy() no verifica si el destino tiene espacio suficiente\n\n");
    
    char origen[100];
    char destino[20];
    
    printf("Ingresa texto para copiar (destino tiene solo 20 bytes):\n");
    printf("Origen: ");
    fflush(stdout);
    
    if (fgets(origen, sizeof(origen), stdin) != NULL) {
        origen[strcspn(origen, "\n")] = '\0';
        
        printf("\nTamaño origen: %zu bytes\n", strlen(origen));
        printf("Tamaño destino: %zu bytes\n", sizeof(destino));
        
        if (strlen(origen) >= sizeof(destino)) {
            printf(COLOR_ROJO "¡OVERFLOW! strcpy() habría corrompido memoria\n" COLOR_RESET);
            printf("Forma insegura:  strcpy(destino, origen);           // ❌ PELIGROSO\n");
            printf("Forma segura:    strncpy(destino, origen, 19);      // ✅ MEJOR\n");
            printf("Más segura:      strlcpy(destino, origen, 20);      // ✅ IDEAL\n");
            
            // Demostración segura
            strncpy(destino, origen, sizeof(destino) - 1);
            destino[sizeof(destino) - 1] = '\0';
            printf("Resultado truncado: '%s'\n", destino);
        } else {
            strcpy(destino, origen);
            printf(COLOR_VERDE "Copia segura: '%s'\n" COLOR_RESET, destino);
        }
    }
}

/**
 * @brief Demostración de format string vulnerabilities
 */
static void demo_format_string_vulnerable(void) {
    printf("\n" COLOR_ROJO "=== DEMOSTRACIÓN: FORMAT STRING VULNERABILITY ===" COLOR_RESET "\n");
    printf("Usar entrada del usuario directamente en printf() es PELIGROSO\n\n");
    
    char input[100];
    
    printf("Ingresa texto (prueba con: %%x %%s %%n):\n");
    printf("Entrada: ");
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        printf("\nForma VULNERABLE:\n");
        printf("  printf(user_input);  // ❌ EXTREMADAMENTE PELIGROSO\n");
        printf("  // Permite leer memoria, escribir memoria, ejecutar código\n\n");
        
        printf("Forma SEGURA:\n");
        printf("  printf(\"%%s\", user_input);  // ✅ SEGURO\n");
        printf("Resultado seguro: %s\n", input);
        
        if (strstr(input, "%") != NULL) {
            printf(COLOR_AMARILLO "\n¡DETECTADOS FORMAT SPECIFIERS!\n" COLOR_RESET);
            printf("Si esto fuera printf(input) directamente, sería un exploit\n");
        }
    }
}

/**
 * @brief Comparación de métodos seguros vs inseguros
 */
static void comparacion_metodos(void) {
    printf("\n" COLOR_AZUL "=== COMPARACIÓN: MÉTODOS SEGUROS VS INSEGUROS ===" COLOR_RESET "\n\n");
    
    printf("┌─────────────────┬─────────────────────┬──────────────────────┐\n");
    printf("│ Función Insegura│ Problema            │ Alternativa Segura   │\n");
    printf("├─────────────────┼─────────────────────┼──────────────────────┤\n");
    printf("│ gets()          │ No límites          │ fgets()              │\n");
    printf("│ scanf(\"%%s\")     │ No límites          │ scanf(\"%%99s\")       │\n");
    printf("│ strcpy()        │ No verificación     │ strncpy(), strlcpy() │\n");
    printf("│ strcat()        │ No verificación     │ strncat(), strlcat() │\n");
    printf("│ sprintf()       │ No límites          │ snprintf()           │\n");
    printf("│ printf(user)    │ Format string       │ printf(\"%%s\", user)  │\n");
    printf("└─────────────────┴─────────────────────┴──────────────────────┘\n");
}

/**
 * @brief Demostración con entrada segura como contraste
 */
static void demo_entrada_segura_contraste(void) {
    printf("\n" COLOR_VERDE "=== CONTRASTE: ENTRADA SEGURA ===" COLOR_RESET "\n");
    printf("Ahora veamos cómo hacer entrada de datos de forma segura:\n\n");
    
    char buffer[50];
    resultado_entrada_t resultado;
    
    printf("Ingresa tu nombre (máximo 49 caracteres):\n");
    resultado = leer_cadena_segura(buffer, sizeof(buffer));
    
    switch (resultado) {
        case ENTRADA_OK:
            printf(COLOR_VERDE "✅ Entrada segura exitosa: '%s'\n" COLOR_RESET, buffer);
            break;
        case ENTRADA_ERROR_BUFFER_OVERFLOW:
            printf(COLOR_AMARILLO "⚠️  Entrada demasiado larga, truncada\n" COLOR_RESET);
            break;
        case ENTRADA_ERROR_EOF:
            printf(COLOR_ROJO "❌ Error de entrada\n" COLOR_RESET);
            break;
        default:
            printf(COLOR_ROJO "❌ Error desconocido\n" COLOR_RESET);
            break;
    }
    
    int edad;
    printf("\nIngresa tu edad (0-150):\n");
    resultado = leer_entero_seguro(&edad, 0, 150);
    
    if (resultado == ENTRADA_OK) {
        printf(COLOR_VERDE "✅ Edad válida: %d años\n" COLOR_RESET, edad);
    } else {
        printf(COLOR_AMARILLO "⚠️  Edad inválida o fuera de rango\n" COLOR_RESET);
    }
}

/**
 * @brief Menú principal de demostración
 */
static void mostrar_menu(void) {
    printf("\n" COLOR_AZUL "=== DEMO DE VULNERABILIDADES DE ENTRADA ===" COLOR_RESET "\n");
    printf("1. Demostración gets() - Buffer Overflow\n");
    printf("2. Demostración scanf() sin límites\n");
    printf("3. Demostración strcpy() sin validación\n");
    printf("4. Demostración Format String Vulnerability\n");
    printf("5. Comparación de métodos\n");
    printf("6. Contraste con entrada segura\n");
    printf("7. Mostrar todas las demos\n");
    printf("0. Salir\n");
    printf("\nSelecciona una opción: ");
}

/**
 * @brief Ejecuta todas las demostraciones
 */
static void ejecutar_todas_demos(void) {
    printf(COLOR_AZUL "\n=== EJECUTANDO TODAS LAS DEMOSTRACIONES ===\n" COLOR_RESET);
    
    demo_gets_vulnerable();
    printf("\nPresiona Enter para continuar...");
    getchar();
    
    demo_scanf_vulnerable();
    printf("\nPresiona Enter para continuar...");
    getchar();
    
    demo_strcpy_vulnerable();
    printf("\nPresiona Enter para continuar...");
    getchar();
    
    demo_format_string_vulnerable();
    printf("\nPresiona Enter para continuar...");
    getchar();
    
    comparacion_metodos();
    printf("\nPresiona Enter para continuar...");
    getchar();
    
    demo_entrada_segura_contraste();
}

/**
 * @brief Función principal
 */
int main(void) {
    printf(COLOR_AMARILLO "ADVERTENCIA: Este programa demuestra vulnerabilidades para fines educativos\n" COLOR_RESET);
    printf("NO ejecutar con privilegios elevados\n");
    printf("NO usar este código en aplicaciones de producción\n");
    
    int opcion;
    
    do {
        mostrar_menu();
        
        resultado_entrada_t resultado = leer_entero_seguro(&opcion, 0, 7);
        
        if (resultado != ENTRADA_OK) {
            printf(COLOR_ROJO "Opción inválida. Intenta de nuevo.\n" COLOR_RESET);
            continue;
        }
        
        switch (opcion) {
            case 1:
                demo_gets_vulnerable();
                break;
            case 2:
                demo_scanf_vulnerable();
                break;
            case 3:
                demo_strcpy_vulnerable();
                break;
            case 4:
                demo_format_string_vulnerable();
                break;
            case 5:
                comparacion_metodos();
                break;
            case 6:
                demo_entrada_segura_contraste();
                break;
            case 7:
                ejecutar_todas_demos();
                break;
            case 0:
                printf(COLOR_VERDE "¡Gracias por aprender sobre seguridad!\n" COLOR_RESET);
                break;
            default:
                printf(COLOR_ROJO "Opción no válida\n" COLOR_RESET);
                break;
        }
        
        if (opcion != 0 && opcion != 7) {
            printf("\nPresiona Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 0);
    
    return 0;
}
