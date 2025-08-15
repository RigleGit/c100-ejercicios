/**
 * @file fuzz_entrada.c
 * @brief Fuzzing target para entrada segura
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "entrada_segura.h"

#define MAX_INPUT_SIZE 10000
#define MAX_ITERATIONS 1000000

/**
 * @brief Genera datos aleatorios
 */
static void generar_datos_aleatorios(char *buffer, size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        buffer[i] = (char)(rand() % 256);
    }
    buffer[size - 1] = '\0';
}

/**
 * @brief Fuzzing de leer_cadena_segura
 */
static void fuzz_cadena_segura(size_t iteraciones) {
    printf("Fuzzing leer_cadena_segura() con %zu iteraciones...\n", iteraciones);
    
    char input_buffer[MAX_INPUT_SIZE];
    char output_buffer[256];
    size_t errores = 0;
    size_t crashes = 0;
    
    FILE *fp_original = stdin;
    
    for (size_t i = 0; i < iteraciones; i++) {
        // Generar tamaño aleatorio
        size_t input_size = 1 + (rand() % (MAX_INPUT_SIZE - 1));
        
        // Generar datos aleatorios
        generar_datos_aleatorios(input_buffer, input_size);
        
        // Crear archivo temporal con los datos
        FILE *fp = tmpfile();
        if (!fp) {
            errores++;
            continue;
        }
        
        fwrite(input_buffer, 1, input_size, fp);
        rewind(fp);
        stdin = fp;
        
        // Probar la función
        resultado_entrada_t resultado = leer_cadena_segura(output_buffer, sizeof(output_buffer));
        
        // Verificar que no haya crash
        if (resultado < ENTRADA_ERROR_DEMASIADOS_INTENTOS) {
            crashes++;
        }
        
        fclose(fp);
        
        if (i % 10000 == 0) {
            printf("Progreso: %zu/%zu (%.1f%%)\r", i, iteraciones, (i * 100.0) / iteraciones);
            fflush(stdout);
        }
    }
    
    stdin = fp_original;
    
    printf("\nResultados del fuzzing:\n");
    printf("Iteraciones: %zu\n", iteraciones);
    printf("Errores: %zu (%.2f%%)\n", errores, (errores * 100.0) / iteraciones);
    printf("Crashes: %zu (%.2f%%)\n", crashes, (crashes * 100.0) / iteraciones);
}

/**
 * @brief Fuzzing de leer_entero_seguro
 */
static void fuzz_entero_seguro(size_t iteraciones) {
    printf("Fuzzing leer_entero_seguro() con %zu iteraciones...\n", iteraciones);
    
    char input_buffer[1000];
    int output_value;
    size_t errores = 0;
    size_t crashes = 0;
    
    FILE *fp_original = stdin;
    
    for (size_t i = 0; i < iteraciones; i++) {
        // Generar entrada aleatoria
        size_t input_size = 1 + (rand() % 999);
        generar_datos_aleatorios(input_buffer, input_size);
        
        FILE *fp = tmpfile();
        if (!fp) {
            errores++;
            continue;
        }
        
        fwrite(input_buffer, 1, input_size, fp);
        rewind(fp);
        stdin = fp;
        
        // Rangos aleatorios
        int min_val = rand() % 1000 - 500;
        int max_val = min_val + (rand() % 1000);
        
        resultado_entrada_t resultado = leer_entero_seguro(&output_value, min_val, max_val);
        
        if (resultado < ENTRADA_ERROR_DEMASIADOS_INTENTOS) {
            crashes++;
        }
        
        fclose(fp);
        
        if (i % 10000 == 0) {
            printf("Progreso: %zu/%zu (%.1f%%)\r", i, iteraciones, (i * 100.0) / iteraciones);
            fflush(stdout);
        }
    }
    
    stdin = fp_original;
    
    printf("\nResultados del fuzzing:\n");
    printf("Iteraciones: %zu\n", iteraciones);
    printf("Errores: %zu (%.2f%%)\n", errores, (errores * 100.0) / iteraciones);
    printf("Crashes: %zu (%.2f%%)\n", crashes, (crashes * 100.0) / iteraciones);
}

/**
 * @brief Fuzzing con casos específicos problemáticos
 */
static void fuzz_casos_especiales(void) {
    printf("Fuzzing con casos especiales...\n");
    
    const char* casos_problematicos[] = {
        "",                           // Cadena vacía
        "\n",                        // Solo newline
        "\0",                        // Null byte
        "%s%s%s%s%s",               // Format string
        "%x%x%x%x%x",               // Format hex
        "A" "A" "A" "A" "A",        // Caracteres repetidos
        "\xff\xff\xff\xff",         // Bytes altos
        "2147483647",               // MAX_INT
        "-2147483648",              // MIN_INT
        "999999999999999999999",    // Overflow
        "abc123def",                // Mezclado
        "\t\r\n\v\f",              // Whitespace
        "../../etc/passwd",         // Path traversal
        "<script>alert(1)</script>", // XSS
        "'; DROP TABLE users; --",   // SQL injection
    };
    
    char buffer[256];
    int valor_entero;
    FILE *fp_original = stdin;
    size_t num_casos = sizeof(casos_problematicos) / sizeof(casos_problematicos[0]);
    
    printf("Probando %zu casos especiales...\n", num_casos);
    
    for (size_t i = 0; i < num_casos; i++) {
        FILE *fp = tmpfile();
        if (!fp) continue;
        
        fprintf(fp, "%s\n", casos_problematicos[i]);
        rewind(fp);
        stdin = fp;
        
        printf("Caso %zu: ", i + 1);
        
        // Probar cadena
        resultado_entrada_t resultado1 = leer_cadena_segura(buffer, sizeof(buffer));
        printf("cadena=%d ", resultado1);
        
        // Resetear para entero
        rewind(fp);
        resultado_entrada_t resultado2 = leer_entero_seguro(&valor_entero, -1000, 1000);
        printf("entero=%d ", resultado2);
        
        printf("✓\n");
        
        fclose(fp);
    }
    
    stdin = fp_original;
}

/**
 * @brief Fuzzing de límites de buffer
 */
static void fuzz_limites_buffer(void) {
    printf("Fuzzing límites de buffer...\n");
    
    FILE *fp_original = stdin;
    
    // Probar diferentes tamaños de buffer
    size_t tamaños[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    size_t num_tamaños = sizeof(tamaños) / sizeof(tamaños[0]);
    
    for (size_t i = 0; i < num_tamaños; i++) {
        size_t tamaño_buffer = tamaños[i];
        char *buffer = malloc(tamaño_buffer);
        if (!buffer) continue;
        
        // Generar entrada más larga que el buffer
        size_t tamaño_entrada = tamaño_buffer + (rand() % 100);
        char *entrada = malloc(tamaño_entrada + 1);
        if (!entrada) {
            free(buffer);
            continue;
        }
        
        generar_datos_aleatorios(entrada, tamaño_entrada);
        
        FILE *fp = tmpfile();
        if (!fp) {
            free(buffer);
            free(entrada);
            continue;
        }
        
        fprintf(fp, "%s\n", entrada);
        rewind(fp);
        stdin = fp;
        
        resultado_entrada_t resultado = leer_cadena_segura(buffer, tamaño_buffer);
        
        printf("Buffer %zu bytes, entrada %zu bytes: resultado=%d ✓\n", 
               tamaño_buffer, tamaño_entrada, resultado);
        
        fclose(fp);
        free(buffer);
        free(entrada);
    }
    
    stdin = fp_original;
}

/**
 * @brief Menú principal
 */
static void mostrar_menu(void) {
    printf("\n" COLOR_AZUL "=== FUZZING DE ENTRADA SEGURA ===" COLOR_RESET "\n");
    printf("1. Fuzzing rápido (1,000 iteraciones)\n");
    printf("2. Fuzzing medio (10,000 iteraciones)\n");
    printf("3. Fuzzing intensivo (100,000 iteraciones)\n");
    printf("4. Casos especiales problemáticos\n");
    printf("5. Fuzzing de límites de buffer\n");
    printf("6. Fuzzing completo\n");
    printf("7. Fuzzing personalizado\n");
    printf("0. Salir\n");
    printf("\nSelecciona una opción: ");
}

/**
 * @brief Ejecuta fuzzing completo
 */
static void fuzzing_completo(void) {
    printf(COLOR_AZUL "=== FUZZING COMPLETO ===" COLOR_RESET "\n");
    
    printf("\n1. Fuzzing de cadenas seguras...\n");
    fuzz_cadena_segura(50000);
    
    printf("\n2. Fuzzing de enteros seguros...\n");
    fuzz_entero_seguro(50000);
    
    printf("\n3. Casos especiales...\n");
    fuzz_casos_especiales();
    
    printf("\n4. Límites de buffer...\n");
    fuzz_limites_buffer();
    
    printf(COLOR_VERDE "\n¡Fuzzing completo terminado!\n" COLOR_RESET);
}

/**
 * @brief Función principal
 */
int main(int argc, char *argv[]) {
    // Inicializar generador de números aleatorios
    srand((unsigned int)time(NULL));
    
    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--auto") == 0) {
            fuzzing_completo();
            return 0;
        } else if (strcmp(argv[i], "--quick") == 0) {
            fuzz_cadena_segura(1000);
            fuzz_entero_seguro(1000);
            return 0;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Uso: %s [opciones]\n", argv[0]);
            printf("Opciones:\n");
            printf("  --auto     Ejecutar fuzzing completo automático\n");
            printf("  --quick    Ejecutar fuzzing rápido\n");
            printf("  --help     Mostrar esta ayuda\n");
            return 0;
        }
    }
    
    int opcion;
    
    do {
        mostrar_menu();
        
        resultado_entrada_t resultado = leer_entero_seguro(&opcion, 0, 7);
        
        if (resultado != ENTRADA_OK) {
            printf(COLOR_ROJO "Opción inválida\n" COLOR_RESET);
            continue;
        }
        
        switch (opcion) {
            case 1:
                fuzz_cadena_segura(1000);
                fuzz_entero_seguro(1000);
                break;
            case 2:
                fuzz_cadena_segura(10000);
                fuzz_entero_seguro(10000);
                break;
            case 3:
                fuzz_cadena_segura(100000);
                fuzz_entero_seguro(100000);
                break;
            case 4:
                fuzz_casos_especiales();
                break;
            case 5:
                fuzz_limites_buffer();
                break;
            case 6:
                fuzzing_completo();
                break;
            case 7: {
                size_t iteraciones;
                printf("Ingresa número de iteraciones: ");
                if (leer_entero_seguro((int*)&iteraciones, 1, MAX_ITERATIONS) == ENTRADA_OK) {
                    fuzz_cadena_segura(iteraciones);
                    fuzz_entero_seguro(iteraciones);
                }
                break;
            }
            case 0:
                printf(COLOR_VERDE "¡Fuzzing terminado!\n" COLOR_RESET);
                break;
            default:
                printf(COLOR_ROJO "Opción no válida\n" COLOR_RESET);
                break;
        }
        
        if (opcion != 0) {
            printf("\nPresiona Enter para continuar...");
            getchar();
        }
        
    } while (opcion != 0);
    
    return 0;
}
