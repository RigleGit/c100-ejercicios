/**
 * @file main.c
 * @brief Programa principal para demostrar const-correctness en C
 * @details Este programa ofrece un menú interactivo para explorar
 *          diferentes aspectos de const-correctness en C.
 * 
 * @author Curso C - Ejercicio 097
 * @date 2024
 * @version 1.0
 */

#include "const_correctness.h"
#include <ctype.h>

/**
 * @brief Limpia el buffer de entrada
 */
static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Obtiene una opción del usuario
 * @return Carácter seleccionado por el usuario
 */
static char obtener_opcion_usuario(void) {
    char opcion;
    printf("Seleccione una opción: ");
    scanf(" %c", &opcion);
    limpiar_buffer();
    return tolower(opcion);
}

/**
 * @brief Muestra el menú principal
 */
static void mostrar_menu_principal(void) {
    printf("\n============================================================\n");
    printf("           CONST-CORRECTNESS EN C - MENÚ PRINCIPAL\n");
    printf("============================================================\n");
    printf("1. Demostrar problemas sin const-correctness\n");
    printf("2. Mostrar beneficios del código const-correct\n");
    printf("3. Comparación lado a lado (incorrecto vs correcto)\n");
    printf("4. Explorar diferentes tipos de const\n");
    printf("5. Análisis de const-correctness\n");
    printf("6. Ejercicios interactivos\n");
    printf("7. Ejemplos prácticos con estructuras\n");
    printf("8. Demostración completa\n");
    printf("9. Mostrar código fuente de ejemplos\n");
    printf("0. Salir\n");
    printf("============================================================\n");
}

/**
 * @brief Demuestra ejemplos prácticos con estructuras
 */
static void ejemplos_practicos_estructuras(void) {
    printf("\n=== EJEMPLOS PRÁCTICOS CON ESTRUCTURAS ===\n\n");
    
    // Crear una configuración local (simulando inicialización)
    configuracion_t config = {
        .id = 42,
        .nombre = "Config de prueba",
        .factor_conversion = 2.54,
        .activo = true
    };
    
    printf("1. IMPRIMIR CONFIGURACIÓN (función const-correct):\n");
    imprimir_configuracion(&config);
    
    printf("2. VALIDAR CONFIGURACIÓN (función const-correct):\n");
    bool es_valida = validar_configuracion(&config);
    printf("La configuración es %s\n\n", es_valida ? "válida" : "inválida");
    
    printf("3. CAMBIAR ESTADO (función que modifica):\n");
    printf("Estado original: %s\n", config.activo ? "activo" : "inactivo");
    bool cambio_exitoso = cambiar_estado_configuracion(&config, false);
    printf("Cambio %s\n", cambio_exitoso ? "exitoso" : "fallido");
    printf("Estado nuevo: %s\n\n", config.activo ? "activo" : "inactivo");
    
    printf("4. TRABAJAR CON CONFIGURACIÓN CONSTANTE:\n");
    printf("Usando CONFIG_DEFAULT (inmutable):\n");
    // Ejemplo simplificado: crear una configuración local (sin CONFIG_DEFAULT)
    configuracion_t cfg = { .id = 1, .nombre = "Demo", .factor_conversion = 1.0, .activo = true };
    imprimir_configuracion(&cfg);
    printf("✓ Esta configuración no puede modificarse\n\n");
}

/**
 * @brief Muestra código fuente de ejemplos clave
 */
static void mostrar_codigo_fuente_ejemplos(void) {
    printf("\n=== CÓDIGO FUENTE DE EJEMPLOS CLAVE ===\n\n");
    
    printf("1. FUNCIÓN INCORRECTA (sin const):\n");
    printf("```c\n");
    printf("int longitud_cadena_incorrecto(char* cadena) {\n");
    printf("    if (!cadena) return -1;\n");
    printf("    int longitud = 0;\n");
    printf("    while (*cadena != '\\0') {\n");
    printf("        longitud++;\n");
    printf("        cadena++;  // Modifica el puntero\n");
    printf("    }\n");
    printf("    return longitud;\n");
    printf("}\n");
    printf("```\n\n");
    
    printf("PROBLEMAS:\n");
    printf("- No acepta const char*\n");
    printf("- Modifica el puntero (confuso)\n");
    printf("- Tipo de retorno int (limitado)\n\n");
    
    printf("2. FUNCIÓN CORRECTA (con const):\n");
    printf("```c\n");
    printf("size_t longitud_cadena_correcto(const char* cadena) {\n");
    printf("    if (!cadena) return 0;\n");
    printf("    size_t longitud = 0;\n");
    printf("    while (cadena[longitud] != '\\0') {\n");
    printf("        longitud++;\n");
    printf("    }\n");
    printf("    return longitud;\n");
    printf("}\n");
    printf("```\n\n");
    
    printf("MEJORAS:\n");
    printf("- Acepta const char* (más flexible)\n");
    printf("- No modifica el puntero original\n");
    printf("- Tipo de retorno size_t (apropiado)\n");
    printf("- Intención clara (solo lectura)\n\n");
    
    printf("3. DIFERENTES TIPOS DE CONST:\n");
    printf("```c\n");
    printf("// Puntero a datos constantes\n");
    printf("const int* ptr1 = &valor;     // *ptr1 = 5; ERROR\n");
    printf("                              // ptr1++; OK\n\n");
    printf("// Puntero constante\n");
    printf("int* const ptr2 = &valor;     // *ptr2 = 5; OK\n");
    printf("                              // ptr2++; ERROR\n\n");
    printf("// Puntero constante a datos constantes\n");
    printf("const int* const ptr3 = &val; // *ptr3 = 5; ERROR\n");
    printf("                              // ptr3++; ERROR\n");
    printf("```\n\n");
}

/**
 * @brief Ejecuta pruebas rápidas de todas las funciones
 */
static void ejecutar_pruebas_rapidas(void) {
    printf("\n=== PRUEBAS RÁPIDAS DE FUNCIONES ===\n\n");
    
    const char* texto = "Hola mundo";
    
    printf("Texto de prueba: \"%s\"\n\n", texto);
    
    // Prueba de longitud
    printf("1. Longitud de cadena:\n");
    printf("   Función incorrecta: %d\n", longitud_cadena_incorrecto((char*)texto));
    printf("   Función correcta: %zu\n\n", longitud_cadena_correcto(texto));
    
    // Prueba de búsqueda
    printf("2. Búsqueda de carácter 'o':\n");
    printf("   Función incorrecta: posición %d\n", buscar_caracter_incorrecto((char*)texto, 'o'));
    printf("   Función correcta: posición %d\n\n", buscar_caracter_correcto(texto, 'o'));
    
    // Prueba de comparación
    printf("3. Comparación de cadenas:\n");
    const char* texto2 = "Hola mundo";
    const char* texto3 = "Otro texto";
    printf("   \"%s\" == \"%s\": %s\n", texto, texto2, 
           comparar_cadenas_correcto(texto, texto2) ? "true" : "false");
    printf("   \"%s\" == \"%s\": %s\n\n", texto, texto3, 
           comparar_cadenas_correcto(texto, texto3) ? "true" : "false");
    
    // Prueba de copia
    printf("4. Copia de cadena:\n");
    char destino[50];
    bool copia_exitosa = copiar_cadena_correcto(destino, texto, sizeof(destino));
    printf("   Origen: \"%s\"\n", texto);
    printf("   Destino: \"%s\"\n", destino);
    printf("   Copia exitosa: %s\n\n", copia_exitosa ? "Sí" : "No");
    
    // Prueba con arrays
    printf("5. Operaciones con arrays:\n");
    int valores[] = {1, 2, 3, 4, 5};
    size_t tamaño = sizeof(valores) / sizeof(valores[0]);
    
    int suma = sumar_array_const_data(valores, tamaño);
    printf("   Suma del array: %d\n", suma);
    
    llenar_array_const_ptr(valores, tamaño, 10);
    printf("   Array llenado con 10: ");
    for (size_t i = 0; i < tamaño; i++) {
        printf("%d ", valores[i]);
    }
    printf("\n");
    
    int valores_prueba[] = { 10, 20, 5, 30 };
    int maximo = encontrar_maximo_const_completo(valores_prueba, (sizeof(valores_prueba)/sizeof(valores_prueba[0])));
    printf("   Máximo en VALORES_PRUEBA: %d\n\n", maximo);
}

/**
 * @brief Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    char opcion;
    bool continuar = true;
    
    printf("¡Bienvenido al ejercicio de const-correctness en C!\n");
    printf("Este programa demuestra la importancia y aplicación correcta de const.\n");
    
    while (continuar) {
        mostrar_menu_principal();
        opcion = obtener_opcion_usuario();
        
        switch (opcion) {
            case '1':
                demostrar_problemas_sin_const();
                break;
                
            case '2':
                demostrar_beneficios_const_correct();
                break;
                
            case '3':
                comparacion_incorrecto_vs_correcto();
                break;
                
            case '4':
                demostrar_tipos_const();
                break;
                
            case '5': {
                estadisticas_const_t stats;
                analizar_const_correctness(&stats);
                imprimir_estadisticas_const(&stats);
                break;
            }
            
            case '6':
                ejercicios_interactivos_const();
                break;
                
            case '7':
                ejemplos_practicos_estructuras();
                break;
                
            case '8':
                printf("\n¿Está seguro de ejecutar la demostración completa? (s/n): ");
                char confirmacion;
                scanf(" %c", &confirmacion);
                limpiar_buffer();
                if (tolower(confirmacion) == 's') {
                    demostracion_completa_const_correctness();
                }
                break;
                
            case '9':
                mostrar_codigo_fuente_ejemplos();
                break;
                
            case 't': // Opción oculta para pruebas
                ejecutar_pruebas_rapidas();
                break;
                
            case '0':
                continuar = false;
                printf("\n¡Gracias por aprender sobre const-correctness!\n");
                printf("Recuerda: usar const correctamente hace tu código más seguro,\n");
                printf("claro y eficiente. ¡Es una buena práctica fundamental!\n\n");
                break;
                
            default:
                printf("\nOpción no válida. Por favor, seleccione una opción del 0-9.\n");
                printf("Tip: Escriba 't' para ejecutar pruebas rápidas.\n");
                break;
        }
        
        if (continuar && opcion != '8') {  // No pausar después de demo completa
            printf("\nPresiona Enter para volver al menú...");
            getchar();
        }
    }
    
    return 0;
}
#endif
