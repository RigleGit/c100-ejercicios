#include "evaluador_rpn.h"

void mostrar_menu_principal(void) {
    printf("\n=== EVALUADOR DE EXPRESIONES RPN ===\n");
    printf("1. Evaluar expresión RPN\n");
    printf("2. Modo interactivo\n");
    printf("3. Procesar archivo de expresiones\n");
    printf("4. Ejecutar pruebas\n");
    printf("5. Demostración con ejemplos\n");
    printf("6. Ver estadísticas\n");
    printf("7. Configuración\n");
    printf("8. Ayuda\n");
    printf("0. Salir\n");
    printf("=====================================\n");
    printf("Seleccione una opción: ");
}

void mostrar_ayuda(void) {
    printf("\n=== AYUDA - NOTACIÓN POSTFIJA (RPN) ===\n\n");
    
    printf("¿Qué es RPN?\n");
    printf("La Notación Polaca Inversa (RPN) coloca los operadores\n");
    printf("después de los operandos. No requiere paréntesis.\n\n");
    
    printf("Ejemplos de conversión:\n");
    printf("Infija       →  RPN\n");
    printf("2 + 3        →  2 3 +\n");
    printf("4 * 5        →  4 5 *\n");
    printf("(2 + 3) * 4  →  2 3 + 4 *\n");
    printf("2 + 3 * 4    →  2 3 4 * +\n");
    printf("(1 + 2) * (3 + 4) → 1 2 + 3 4 + *\n\n");
    
    printf("Operadores soportados:\n");
    const operador_t *ops = obtener_operadores_soportados();
    for (int i = 0; ops[i].simbolo != 0; i++) {
        printf("  %c  - %s\n", ops[i].simbolo, ops[i].nombre);
    }
    
    printf("\nCómo evaluar:\n");
    printf("1. Lea de izquierda a derecha\n");
    printf("2. Si es número, apílelo\n");
    printf("3. Si es operador, tome 2 números de la pila,\n");
    printf("   aplique la operación y apile el resultado\n");
    printf("4. Al final debe quedar un solo número: el resultado\n\n");
    
    printf("Ejemplo paso a paso '2 3 + 4 *':\n");
    printf("Token | Pila\n");
    printf("------|------\n");
    printf("  2   | [2]\n");
    printf("  3   | [3, 2]\n");
    printf("  +   | [5]     (3+2=5)\n");
    printf("  4   | [4, 5]\n");
    printf("  *   | [20]    (5*4=20)\n");
    printf("Resultado: 20\n");
    
    printf("\n======================================\n");
}

void evaluar_expresion_simple(void) {
    char expresion[MAX_EXPRESSION_LENGTH];
    
    printf("\nIngrese una expresión en notación postfija: ");
    if (!fgets(expresion, sizeof(expresion), stdin)) {
        printf("Error leyendo la entrada.\n");
        return;
    }
    
    // Eliminar salto de línea
    expresion[strcspn(expresion, "\n")] = '\0';
    
    if (strlen(expresion) == 0) {
        printf("Expresión vacía.\n");
        return;
    }
    
    printf("\nEvaluando: '%s'\n", expresion);
    printf("─────────────────────────────────\n");
    
    resultado_evaluacion_t resultado = evaluar_rpn(expresion);
    
    if (resultado.error == RPN_SUCCESS) {
        printf("✅ Resultado: %.6f\n", resultado.resultado);
        printf("Tokens procesados: %zu\n", resultado.tokens_procesados);
        printf("Operaciones realizadas: %zu\n", resultado.operaciones_realizadas);
    } else {
        printf("❌ Error: %s\n", resultado.mensaje_error);
        printf("Tokens procesados antes del error: %zu\n", resultado.tokens_procesados);
    }
}

void modo_interactivo_completo(void) {
    printf("\n=== MODO INTERACTIVO ===\n");
    printf("Ingrese expresiones RPN (escriba 'salir' para terminar)\n");
    printf("Comandos especiales:\n");
    printf("  'verbosidad <0-3>' - Cambiar nivel de verbosidad\n");
    printf("  'paso'             - Alternar modo paso a paso\n");
    printf("  'estadisticas'     - Mostrar estadísticas\n");
    printf("  'limpiar'          - Limpiar estadísticas\n");
    printf("  'ayuda'            - Mostrar ayuda\n");
    printf("───────────────────────────────────────────────\n");
    
    char entrada[MAX_EXPRESSION_LENGTH];
    
    while (true) {
        printf("\nRPN> ");
        
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            break;
        }
        
        entrada[strcspn(entrada, "\n")] = '\0';
        
        if (strlen(entrada) == 0) continue;
        
        // Comandos especiales
        if (strcmp(entrada, "salir") == 0 || strcmp(entrada, "quit") == 0) {
            break;
        } else if (strcmp(entrada, "ayuda") == 0 || strcmp(entrada, "help") == 0) {
            mostrar_ayuda();
            continue;
        } else if (strcmp(entrada, "estadisticas") == 0 || strcmp(entrada, "stats") == 0) {
            imprimir_estadisticas();
            continue;
        } else if (strcmp(entrada, "limpiar") == 0 || strcmp(entrada, "clear") == 0) {
            reiniciar_estadisticas();
            printf("Estadísticas reiniciadas.\n");
            continue;
        } else if (strcmp(entrada, "paso") == 0) {
            static bool paso_activo = false;
            paso_activo = !paso_activo;
            establecer_modo_paso_a_paso(paso_activo);
            printf("Modo paso a paso: %s\n", paso_activo ? "ACTIVADO" : "DESACTIVADO");
            continue;
        } else if (strncmp(entrada, "verbosidad ", 11) == 0) {
            int nivel = atoi(entrada + 11);
            establecer_verbosidad(nivel);
            printf("Verbosidad establecida a nivel %d\n", nivel);
            continue;
        }
        
        // Evaluar expresión
        resultado_evaluacion_t resultado = evaluar_rpn(entrada);
        
        if (resultado.error == RPN_SUCCESS) {
            printf("= %.6f\n", resultado.resultado);
        } else {
            printf("Error: %s\n", resultado.mensaje_error);
        }
    }
    
    printf("Saliendo del modo interactivo.\n");
}

void configurar_evaluador(void) {
    int opcion;
    
    do {
        printf("\n=== CONFIGURACIÓN ===\n");
        printf("1. Establecer verbosidad (actual: %d)\n", obtener_estadisticas().total_evaluaciones >= 0 ? 0 : 0);
        printf("2. Alternar modo paso a paso\n");
        printf("3. Ver versión\n");
        printf("4. Reiniciar estadísticas\n");
        printf("0. Volver al menú principal\n");
        printf("Opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); // Limpiar buffer
            printf("Entrada inválida.\n");
            continue;
        }
        while (getchar() != '\n'); // Limpiar buffer
        
        switch (opcion) {
            case 1: {
                int nivel;
                printf("Ingrese nivel de verbosidad (0-3): ");
                if (scanf("%d", &nivel) == 1) {
                    establecer_verbosidad(nivel);
                    printf("Verbosidad establecida a nivel %d\n", nivel);
                } else {
                    printf("Entrada inválida.\n");
                }
                while (getchar() != '\n');
                break;
            }
            case 2: {
                static bool paso_activo = false;
                paso_activo = !paso_activo;
                establecer_modo_paso_a_paso(paso_activo);
                printf("Modo paso a paso: %s\n", paso_activo ? "ACTIVADO" : "DESACTIVADO");
                break;
            }
            case 3:
                printf("Evaluador RPN versión %s\n", obtener_version());
                break;
            case 4:
                reiniciar_estadisticas();
                printf("Estadísticas reiniciadas.\n");
                break;
            case 0:
                break;
            default:
                printf("Opción inválida.\n");
                break;
        }
    } while (opcion != 0);
}

void procesar_archivo_interface(void) {
    char nombre_archivo[256];
    char respuesta;
    
    printf("Ingrese el nombre del archivo: ");
    if (!fgets(nombre_archivo, sizeof(nombre_archivo), stdin)) {
        printf("Error leyendo el nombre del archivo.\n");
        return;
    }
    nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0';
    
    printf("¿Mostrar pasos de evaluación? (s/n): ");
    if (scanf(" %c", &respuesta) != 1) {
        printf("Entrada inválida.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    bool mostrar_pasos = (respuesta == 's' || respuesta == 'S');
    
    printf("\nProcesando archivo '%s'...\n", nombre_archivo);
    printf("─────────────────────────────────\n");
    
    size_t procesadas = procesar_archivo_rpn(nombre_archivo, mostrar_pasos);
    
    printf("─────────────────────────────────\n");
    printf("Expresiones procesadas exitosamente: %zu\n", procesadas);
}

bool ejecutar_pruebas_interface(void) {
    printf("\n=== EJECUTANDO PRUEBAS ===\n");
    
    // Reiniciar estadísticas para las pruebas
    reiniciar_estadisticas();
    
    // Establecer verbosidad mínima para las pruebas
    int verbosidad_original = 0; // Guardar configuración actual
    establecer_verbosidad(0);
    
    bool todas_pasaron = ejecutar_pruebas_rpn();
    
    // Restaurar configuración
    establecer_verbosidad(verbosidad_original);
    
    printf("\n");
    if (todas_pasaron) {
        printf("✅ Todas las pruebas PASARON\n");
    } else {
        printf("❌ Algunas pruebas FALLARON\n");
    }
    
    imprimir_estadisticas();
    
    return todas_pasaron;
}

void demostrar_ejemplos(void) {
    printf("\n=== DEMOSTRACIÓN CON EJEMPLOS ===\n");
    
    const char *ejemplos[] = {
        "5 3 +",                    // 8
        "15 7 1 1 + - /",          // 3
        "3 4 + 2 * 1 -",           // 13
        "2 3 4 + *",               // 14
        "5 1 2 + 4 * + 3 -",       // 14
        "1 2 + 3 4 + *",           // 21
        "4 2 + 3 5 1 - * +",       // 18
        "3 4 2 * 1 5 - 2 3 ^ ^ / +", // 3.0001220703125
        NULL
    };
    
    printf("Evaluando expresiones de ejemplo:\n");
    printf("──────────────────────────────────\n");
    
    for (int i = 0; ejemplos[i] != NULL; i++) {
        printf("%d. '%s'\n", i + 1, ejemplos[i]);
        
        resultado_evaluacion_t resultado = evaluar_rpn(ejemplos[i]);
        
        if (resultado.error == RPN_SUCCESS) {
            printf("   Resultado: %.6f\n", resultado.resultado);
        } else {
            printf("   Error: %s\n", resultado.mensaje_error);
        }
        printf("\n");
    }
    
    printf("Presione Enter para continuar...");
    getchar();
}

#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    
    // Inicializar evaluador
    inicializar_estadisticas();
    
    printf("Evaluador de Expresiones RPN (Reverse Polish Notation)\n");
    printf("Versión %s\n", obtener_version());
    printf("=====================================================\n");
    
    do {
        mostrar_menu_principal();
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); // Limpiar buffer de entrada
            printf("Entrada inválida. Intente nuevamente.\n");
            continue;
        }
        while (getchar() != '\n'); // Limpiar buffer después de scanf
        
        switch (opcion) {
            case 1:
                evaluar_expresion_simple();
                break;
            case 2:
                modo_interactivo_completo();
                break;
            case 3:
                procesar_archivo_interface();
                break;
            case 4:
                ejecutar_pruebas_interface();
                break;
            case 5:
                demostrar_ejemplos();
                break;
            case 6:
                imprimir_estadisticas();
                break;
            case 7:
                configurar_evaluador();
                break;
            case 8:
                mostrar_ayuda();
                break;
            case 0:
                printf("¡Hasta luego!\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 0);
    
    // Mostrar estadísticas finales si hubo actividad
    estadisticas_rpn_t stats = obtener_estadisticas();
    if (stats.total_evaluaciones > 0) {
        printf("\n=== ESTADÍSTICAS FINALES ===\n");
        imprimir_estadisticas();
    }
    
    return 0;
}
#endif
