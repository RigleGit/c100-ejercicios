/**
 * @file main.c
 * @brief Programa principal para [NOMBRE DEL EJERCICIO]
 * 
 * [DESCRIPCIÓN DEL PROGRAMA PRINCIPAL]
 * 
 * Modo de uso:
 *   ./nombre_ejercicio                    - Modo interactivo
 *   ./nombre_ejercicio archivo.txt        - Procesar archivo
 *   ./nombre_ejercicio --test             - Ejecutar casos de prueba
 *   ./nombre_ejercicio --demo             - Ejecutar demostración
 *   ./nombre_ejercicio --help             - Mostrar ayuda
 * 
 * @author C100 Ejercicios
 * @date [FECHA]
 */

#include "nombre_ejercicio.h"
#include <climits>

// Prototipos de funciones auxiliares del main
static void mostrar_ayuda(const char* nombre_programa);
static void mostrar_menu_interactivo(void);
static int procesar_modo_interactivo(void);
static int procesar_archivo_entrada(const char* nombre_archivo);
static int ejecutar_casos_prueba(void);
static int ejecutar_demostracion(void);
static void limpiar_buffer_entrada(void);
static int leer_entero_seguro(const char* prompt, int* valor);
static int leer_double_seguro(const char* prompt, double* valor);

/**
 * @brief Función principal del programa
 */
int main(int argc, char* argv[]) {
    printf("=== EJERCICIO: [NOMBRE DEL EJERCICIO] ===\n");
    printf("Autor: C100 Ejercicios\n");
    printf("Descripción: [DESCRIPCIÓN BREVE]\n\n");
    
    // Procesar argumentos de línea de comandos
    if (argc == 1) {
        // Modo interactivo por defecto
        return procesar_modo_interactivo();
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            mostrar_ayuda(argv[0]);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[1], "--test") == 0) {
            return ejecutar_casos_prueba();
        } else if (strcmp(argv[1], "--demo") == 0) {
            return ejecutar_demostracion();
        } else {
            // Asumir que es un archivo de entrada
            return procesar_archivo_entrada(argv[1]);
        }
    } else {
        fprintf(stderr, "Error: Número incorrecto de argumentos.\n");
        mostrar_ayuda(argv[0]);
        return EXIT_FAILURE;
    }
}

/**
 * @brief Mostrar ayuda del programa
 */
static void mostrar_ayuda(const char* nombre_programa) {
    printf("Uso: %s [opciones] [archivo]\n\n", nombre_programa);
    printf("Opciones:\n");
    printf("  (sin argumentos)    Ejecutar en modo interactivo\n");
    printf("  archivo.txt         Procesar archivo de entrada\n");
    printf("  --test              Ejecutar casos de prueba automáticos\n");
    printf("  --demo              Ejecutar demostración con ejemplos\n");
    printf("  --help, -h          Mostrar esta ayuda\n\n");
    
    printf("Descripción:\n");
    printf("  [DESCRIPCIÓN DETALLADA DEL EJERCICIO]\n\n");
    
    printf("Ejemplos:\n");
    printf("  %s                  # Modo interactivo\n", nombre_programa);
    printf("  %s datos.txt        # Procesar archivo\n", nombre_programa);
    printf("  %s --demo           # Ver demostración\n", nombre_programa);
    printf("  %s --test           # Ejecutar tests\n\n", nombre_programa);
    
    printf("Formato de archivo de entrada:\n");
    printf("  [DESCRIPCIÓN DEL FORMATO ESPERADO]\n");
    printf("  Ejemplo:\n");
    printf("    5 3.14\n");
    printf("    10 2.71\n");
    printf("    ...\n\n");
}

/**
 * @brief Mostrar menú del modo interactivo
 */
static void mostrar_menu_interactivo(void) {
    printf("\n=== MENÚ INTERACTIVO ===\n");
    printf("1. Calcular resultado con parámetros\n");
    printf("2. Demostración con ejemplos\n");
    printf("3. Ejecutar casos de prueba\n");
    printf("4. Mostrar información del ejercicio\n");
    printf("5. Salir\n");
    printf("Seleccione una opción (1-5): ");
}

/**
 * @brief Procesar modo interactivo
 */
static int procesar_modo_interactivo(void) {
    int opcion;
    int continuar = 1;
    
    printf("Modo interactivo iniciado.\n");
    printf("Presione Ctrl+C para salir en cualquier momento.\n");
    
    while (continuar) {
        mostrar_menu_interactivo();
        
        if (leer_entero_seguro("", &opcion) != 0) {
            printf("Error: entrada inválida. Intente nuevamente.\n");
            continue;
        }
        
        switch (opcion) {
            case 1: {
                // Solicitar parámetros al usuario
                int parametro1;
                double parametro2;
                double resultado;
                
                printf("\n--- Cálculo con parámetros personalizados ---\n");
                
                if (leer_entero_seguro("Ingrese el primer parámetro (entero): ", &parametro1) != 0) {
                    printf("Error en la entrada del primer parámetro.\n");
                    break;
                }
                
                if (leer_double_seguro("Ingrese el segundo parámetro (decimal): ", &parametro2) != 0) {
                    printf("Error en la entrada del segundo parámetro.\n");
                    break;
                }
                
                // Llamar a la función principal
                resultado_t status = funcion_principal(parametro1, parametro2, &resultado);
                
                if (status == RESULTADO_EXITO) {
                    printf("\n✓ Cálculo exitoso:\n");
                    printf("Parámetro 1: %d\n", parametro1);
                    printf("Parámetro 2: %.6f\n", parametro2);
                    imprimir_resultado(resultado, "%.6f");
                } else {
                    printf("\n✗ Error en el cálculo: %s\n", obtener_mensaje_error(status));
                }
                break;
            }
            
            case 2:
                printf("\nEjecutando demostración...\n");
                ejecutar_demostracion();
                break;
                
            case 3:
                printf("\nEjecutando casos de prueba...\n");
                ejecutar_casos_prueba();
                break;
                
            case 4:
                printf("\n=== INFORMACIÓN DEL EJERCICIO ===\n");
                printf("Nombre: [NOMBRE DEL EJERCICIO]\n");
                printf("Categoría: [CATEGORÍA]\n");
                printf("Dificultad: [NIVEL DE DIFICULTAD]\n");
                printf("Conceptos: [LISTA DE CONCEPTOS]\n");
                printf("Descripción: [DESCRIPCIÓN DETALLADA]\n");
                break;
                
            case 5:
                continuar = 0;
                printf("¡Hasta luego!\n");
                break;
                
            default:
                printf("Opción inválida. Seleccione 1-5.\n");
                break;
        }
        
        if (continuar && opcion != 5) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
    }
    
    return EXIT_SUCCESS;
}

/**
 * @brief Procesar archivo de entrada
 */
static int procesar_archivo_entrada(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo '%s'\n", nombre_archivo);
        perror("Detalles del error");
        return EXIT_FAILURE;
    }
    
    printf("Procesando archivo: %s\n\n", nombre_archivo);
    
    int parametro1;
    double parametro2;
    int linea = 0;
    int casos_procesados = 0;
    int casos_exitosos = 0;
    
    // Leer línea por línea
    while (fscanf(archivo, "%d %lf", &parametro1, &parametro2) == 2) {
        linea++;
        casos_procesados++;
        
        double resultado;
        resultado_t status = funcion_principal(parametro1, parametro2, &resultado);
        
        printf("Línea %d: ", linea);
        if (status == RESULTADO_EXITO) {
            printf("✓ Parámetros (%d, %.6f) → Resultado: %.6f\n", 
                   parametro1, parametro2, resultado);
            casos_exitosos++;
        } else {
            printf("✗ Error con parámetros (%d, %.6f): %s\n", 
                   parametro1, parametro2, obtener_mensaje_error(status));
        }
    }
    
    fclose(archivo);
    
    // Mostrar resumen
    printf("\n=== RESUMEN DEL PROCESAMIENTO ===\n");
    printf("Casos procesados: %d\n", casos_procesados);
    printf("Casos exitosos: %d\n", casos_exitosos);
    printf("Casos con error: %d\n", casos_procesados - casos_exitosos);
    printf("Tasa de éxito: %.1f%%\n", 
           casos_procesados > 0 ? (casos_exitosos * 100.0) / casos_procesados : 0.0);
    
    return casos_procesados > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

/**
 * @brief Ejecutar casos de prueba automáticos
 */
static int ejecutar_casos_prueba(void) {
    printf("=== CASOS DE PRUEBA AUTOMÁTICOS ===\n\n");
    
    typedef struct {
        int param1;
        double param2;
        double resultado_esperado;
        resultado_t status_esperado;
        const char* descripcion;
    } caso_prueba_t;
    
    // Definir casos de prueba
    caso_prueba_t casos[] = {
        {5, 3.0, 15.0, RESULTADO_EXITO, "Caso básico positivo"},
        {0, 5.0, 0.0, RESULTADO_EXITO, "Multiplicación por cero"},
        {-3, 2.0, -6.0, RESULTADO_EXITO, "Número negativo"},
        {10, 0.0, 0.0, RESULTADO_ERROR_PARAMETRO, "Segundo parámetro cero (error esperado)"},
        {-1, 3.0, 0.0, RESULTADO_ERROR_PARAMETRO, "Primer parámetro negativo (error esperado)"}
    };
    
    int num_casos = sizeof(casos) / sizeof(casos[0]);
    int casos_exitosos = 0;
    
    for (int i = 0; i < num_casos; i++) {
        printf("Caso %d: %s\n", i + 1, casos[i].descripcion);
        printf("  Entrada: (%d, %.6f)\n", casos[i].param1, casos[i].param2);
        
        double resultado_obtenido;
        resultado_t status_obtenido = funcion_principal(casos[i].param1, casos[i].param2, &resultado_obtenido);
        
        if (status_obtenido == casos[i].status_esperado) {
            if (status_obtenido == RESULTADO_EXITO) {
                double diferencia = fabs(resultado_obtenido - casos[i].resultado_esperado);
                if (diferencia < PRECISION_DECIMAL) {
                    printf("  ✓ PASSED: Resultado %.6f (esperado %.6f)\n", 
                           resultado_obtenido, casos[i].resultado_esperado);
                    casos_exitosos++;
                } else {
                    printf("  ✗ FAILED: Resultado %.6f, esperado %.6f (diferencia: %.6f)\n", 
                           resultado_obtenido, casos[i].resultado_esperado, diferencia);
                }
            } else {
                printf("  ✓ PASSED: Error detectado correctamente (%s)\n", 
                       obtener_mensaje_error(status_obtenido));
                casos_exitosos++;
            }
        } else {
            printf("  ✗ FAILED: Status %s, esperado %s\n", 
                   obtener_mensaje_error(status_obtenido), 
                   obtener_mensaje_error(casos[i].status_esperado));
        }
        printf("\n");
    }
    
    // Mostrar resumen de pruebas
    printf("=== RESUMEN DE PRUEBAS ===\n");
    printf("Casos ejecutados: %d\n", num_casos);
    printf("Casos exitosos: %d\n", casos_exitosos);
    printf("Casos fallidos: %d\n", num_casos - casos_exitosos);
    printf("Tasa de éxito: %.1f%%\n", (casos_exitosos * 100.0) / num_casos);
    
    return casos_exitosos == num_casos ? EXIT_SUCCESS : EXIT_FAILURE;
}

/**
 * @brief Ejecutar demostración con ejemplos
 */
static int ejecutar_demostracion(void) {
    printf("=== DEMOSTRACIÓN DEL EJERCICIO ===\n\n");
    
    printf("Este ejercicio demuestra: [LISTA DE CONCEPTOS]\n\n");
    
    // Ejemplo 1
    printf("--- Ejemplo 1: [DESCRIPCIÓN] ---\n");
    {
        double resultado;
        resultado_t status = funcion_principal(5, 3.14, &resultado);
        
        if (status == RESULTADO_EXITO) {
            printf("Entrada: parametro1=5, parametro2=3.14\n");
            printf("Resultado: %.6f\n", resultado);
        } else {
            printf("Error: %s\n", obtener_mensaje_error(status));
        }
    }
    printf("\n");
    
    // Ejemplo 2
    printf("--- Ejemplo 2: [DESCRIPCIÓN] ---\n");
    {
        double resultado;
        resultado_t status = funcion_principal(10, 2.71, &resultado);
        
        if (status == RESULTADO_EXITO) {
            printf("Entrada: parametro1=10, parametro2=2.71\n");
            printf("Resultado: %.6f\n", resultado);
        } else {
            printf("Error: %s\n", obtener_mensaje_error(status));
        }
    }
    printf("\n");
    
    // Ejemplo de error
    printf("--- Ejemplo 3: Manejo de errores ---\n");
    {
        double resultado;
        resultado_t status = funcion_principal(-1, 5.0, &resultado);
        
        printf("Entrada inválida: parametro1=-1, parametro2=5.0\n");
        printf("Resultado: %s\n", obtener_mensaje_error(status));
    }
    printf("\n");
    
    printf("Demostración completada.\n");
    return EXIT_SUCCESS;
}

// Funciones auxiliares para entrada segura

/**
 * @brief Limpiar buffer de entrada
 */
static void limpiar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Leer entero de manera segura
 */
static int leer_entero_seguro(const char* prompt, int* valor) {
    char buffer[100];
    
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    // Eliminar salto de línea
    buffer[strcspn(buffer, "\n")] = '\0';
    
    char* endptr;
    errno = 0;
    long temp = strtol(buffer, &endptr, 10);
    
    if (errno != 0 || *endptr != '\0' || temp > INT_MAX || temp < INT_MIN) {
        return -1;
    }
    
    *valor = (int)temp;
    return 0;
}

/**
 * @brief Leer double de manera segura
 */
static int leer_double_seguro(const char* prompt, double* valor) {
    char buffer[100];
    
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    // Eliminar salto de línea
    buffer[strcspn(buffer, "\n")] = '\0';
    
    char* endptr;
    errno = 0;
    double temp = strtod(buffer, &endptr);
    
    if (errno != 0 || *endptr != '\0') {
        return -1;
    }
    
    *valor = temp;
    return 0;
}
