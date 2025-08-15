#include "const_correctness.h"
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

// ========================================
// EJEMPLOS COMPARATIVOS: SIN CONST vs CON CONST
// ========================================
// FUNCIONES INCORRECTAS (SIN CONST) - SOLO PARA DEMOSTRACIÓN
// =============================================================================

int longitud_cadena_incorrecto(char* cadena) {
    if (!cadena) {
        return -1;
    }
    
    int longitud = 0;
    while (*cadena != '\0') {
        longitud++;
        cadena++;  // Modifica el puntero - no es un problema real, pero confuso
    }
    
    return longitud;
}

void imprimir_mensaje_incorrecto(char* mensaje) {
    if (!mensaje) {
        printf("(mensaje nulo)\n");
        fflush(stdout);
        return;
    }
    
    printf("Mensaje: %s\n", mensaje);
    fflush(stdout);
    // Esta función podría modificar mensaje accidentalmente
}

int buscar_caracter_incorrecto(char* cadena, char caracter) {
    if (!cadena) {
        return -1;
    }
    // Posición lógica que ignora espacios ' '
    int posicion_sin_espacios = 0;
    while (*cadena != '\0') {
        if (*cadena == caracter) {
            return posicion_sin_espacios;
        }
        if (*cadena != ' ') {
            posicion_sin_espacios++;
        }
        cadena++;
    }
    return -1;  // No encontrado
}


size_t longitud_cadena_correcto(const char* cadena) {
    if (!cadena) {
        return 0;
    }
    
    size_t longitud = 0;
    while (cadena[longitud] != '\0') {
        longitud++;
    }
    
    return longitud;
}

void imprimir_mensaje_correcto(const char* mensaje) {
    if (!mensaje) {
        printf("(mensaje nulo)\n");
        fflush(stdout);
        return;
    }
    
    printf("Mensaje: %s\n", mensaje);
    fflush(stdout);
    // const garantiza que no modificaremos mensaje
}

int buscar_caracter_correcto(const char* cadena, char caracter) {
    if (!cadena) {
        return -1;
    }
    int posicion_sin_espacios = 0;
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] == caracter) {
            return posicion_sin_espacios;
        }
        if (cadena[i] != ' ') {
            posicion_sin_espacios++;
        }
    }
    return -1;  // No encontrado
}

bool comparar_cadenas_correcto(const char* cadena1, const char* cadena2) {
    if (!cadena1 || !cadena2) {
        return cadena1 == cadena2;  // Ambas nulas o una es nula
    }
    
    size_t i = 0;
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        if (cadena1[i] != cadena2[i]) {
            return false;
        }
        i++;
    }
    
    return cadena1[i] == cadena2[i];  // Deben terminar al mismo tiempo
}

bool copiar_cadena_correcto(char* destino, const char* origen, size_t tamaño_destino) {
    if (!destino || !origen || tamaño_destino == 0) {
        return false;
    }
    
    size_t i = 0;
    while (i < tamaño_destino - 1 && origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    
    destino[i] = '\0';  // Asegurar terminación
    return origen[i] == '\0';  // true si se copió completamente
}

// =============================================================================
// FUNCIONES CON DIFERENTES TIPOS DE CONST
// =============================================================================

int sumar_array_const_data(const int* ptr_a_const, size_t tamaño) {
    if (!ptr_a_const || tamaño == 0) {
        return 0;
    }
    
    int suma = 0;
    for (size_t i = 0; i < tamaño; i++) {
        suma += ptr_a_const[i];  // Podemos leer, pero no modificar
        // ptr_a_const[i] = 0;   // ¡Error de compilación!
    }
    
    // ptr_a_const++;  // Esto sí está permitido (el puntero puede cambiar)
    
    return suma;
}

void llenar_array_const_ptr(int* const array, size_t tamaño, int valor) {
    if (!array || tamaño == 0) {
        return;
    }
    
    for (size_t i = 0; i < tamaño; i++) {
        array[i] = valor;  // Podemos modificar los datos
    }
    
    // array = NULL;  // ¡Error de compilación! El puntero es constante
}

int encontrar_maximo_const_completo(const int* const ptr_const_a_const_data, size_t tamaño) {
    if (!ptr_const_a_const_data || tamaño == 0) {
        return INT_MIN;
    }
    
    int maximo = ptr_const_a_const_data[0];
    for (size_t i = 1; i < tamaño; i++) {
        if (ptr_const_a_const_data[i] > maximo) {
            maximo = ptr_const_a_const_data[i];
        }
        // ptr_const_a_const_data[i] = 0;  // ¡Error! No podemos modificar datos
    }
    
    // ptr_const_a_const_data++;  // ¡Error! No podemos modificar el puntero
    
    return maximo;
}

// =============================================================================
// FUNCIONES PARA TRABAJAR CON ESTRUCTURAS Y CONST
// =============================================================================

bool inicializar_configuracion(configuracion_t* config, int id, 
                              const char* nombre, double factor) {
    if (!config || !nombre) {
        return false;
    }
    
    // config->id = id;  // ¡Error! id es const en la estructura
    // Necesitaríamos usar compound literal o memcpy para inicializar campos const
    
    // Para este ejemplo, asumimos que inicializamos con valores válidos
    if (strlen(nombre) >= sizeof(config->nombre)) {
        return false;
    }
    
    strcpy(config->nombre, nombre);
    // config->factor_conversion = factor;  // ¡Error! factor_conversion es const
    config->activo = true;
    
    return true;
}

void imprimir_configuracion(const configuracion_t* config) {
    if (!config) {
        printf("Configuración nula\n");
        fflush(stdout);
        return;
    }
    
    printf("=== Configuración ===\n");
    printf("ID: %d\n", config->id);
    printf("Nombre: %s\n", config->nombre);
    printf("Factor de conversión: %.2f\n", config->factor_conversion);
    printf("Activo: %s\n", config->activo ? "Sí" : "No");
    printf("====================\n");
    fflush(stdout);
    
    // config->activo = false;  // ¡Error! No podemos modificar a través de const*
}

bool validar_configuracion(const configuracion_t* config) {
    if (!config) {
        return false;
    }
    
    // Validaciones básicas
    if (config->id <= 0) {
        return false;
    }
    
    if (strlen(config->nombre) == 0) {
        return false;
    }
    
    if (config->factor_conversion <= 0.0) {
        return false;
    }
    
    return true;
}

bool cambiar_estado_configuracion(configuracion_t* config, bool nuevo_estado) {
    if (!config) {
        return false;
    }
    
    config->activo = nuevo_estado;  // Solo podemos modificar campos no-const
    // config->id = 999;  // ¡Error! No podemos modificar campos const
    
    return true;
}

// Constantes para pruebas/demos
const char* const CADENAS_PRUEBA[] = { "alpha", "beta", "gamma" };
const size_t NUM_CADENAS_PRUEBA = sizeof(CADENAS_PRUEBA) / sizeof(CADENAS_PRUEBA[0]);
const int VALORES_PRUEBA[] = { 1, 2, 3, 4, 5 };
const size_t NUM_VALORES_PRUEBA = sizeof(VALORES_PRUEBA) / sizeof(VALORES_PRUEBA[0]);
const configuracion_t CONFIG_DEFAULT = { .id = 0, .nombre = "default", .factor_conversion = 1.0, .activo = true };

// =============================================================================
// FUNCIONES DE ANÁLISIS Y DEMOSTRACIÓN
// =============================================================================

void demostrar_problemas_sin_const(void) {
    printf("\n=== PROBLEMAS DEL CÓDIGO SIN CONST-CORRECTNESS ===\n\n");
    
    printf("1. FALTA DE CLARIDAD EN LA INTENCIÓN:\n");
    printf("   void procesar_datos(char* datos);\n");
    printf("   ¿Se modifican los datos o solo se leen?\n\n");
    
    printf("2. POSIBLES MODIFICACIONES ACCIDENTALES:\n");
    char mensaje[] = "Mensaje importante";
    printf("   Original: %s\n", mensaje);
    
    // Simular una función que podría modificar accidentalmente
    (void)mensaje; // evitar warning de variable sin usar en la demo
    // En una función incorrecta, podríamos hacer: *ptr = 'X';
    printf("   (Sin const, podríamos modificar accidentalmente)\n\n");
    
    printf("3. NO SE PUEDEN PASAR CADENAS CONSTANTES:\n");
    printf("   const char* constante = \"No modificable\";\n");
    printf("   longitud_cadena_incorrecto(constante);  // ¡Warning!\n\n");
    
    printf("4. PÉRDIDA DE OPTIMIZACIONES DEL COMPILADOR:\n");
    printf("   Sin const, el compilador asume que los datos pueden cambiar\n");
    printf("   y no puede aplicar ciertas optimizaciones.\n\n");
}

void demostrar_beneficios_const_correct(void) {
    printf("\n=== BENEFICIOS DEL CÓDIGO CONST-CORRECT ===\n\n");
    
    printf("1. CLARIDAD EN LA INTENCIÓN:\n");
    printf("   void procesar_datos(const char* datos);\n");
    printf("   Está claro que los datos no se modificarán.\n\n");
    
    printf("2. PREVENCIÓN DE ERRORES:\n");
    const char* mensaje = "Mensaje protegido";
    printf("   const char* protegido = \"%s\";\n", mensaje);
    printf("   El compilador previene modificaciones accidentales.\n\n");
    
    printf("3. ACEPTACIÓN DE DIFERENTES TIPOS DE DATOS:\n");
    printf("   Funciona con:\n");
    printf("   - char array[] = \"modificable\";\n");
    printf("   - const char* constante = \"inmutable\";\n");
    printf("   - string literals: \"literal\";\n\n");
    
    printf("4. OPTIMIZACIONES DEL COMPILADOR:\n");
    printf("   El compilador puede:\n");
    printf("   - Almacenar datos const en memoria de solo lectura\n");
    printf("   - Aplicar optimizaciones más agresivas\n");
    printf("   - Detectar violaciones en tiempo de compilación\n\n");
}

void comparacion_incorrecto_vs_correcto(void) {
    printf("\n=== COMPARACIÓN: INCORRECTO vs CORRECTO ===\n\n");
    
    const char* texto_prueba = "Texto de prueba";
    
    printf("FUNCIÓN INCORRECTA:\n");
    printf("int longitud_cadena_incorrecto(char* cadena)\n");
    // Nota: Esto generará un warning porque pasamos const char* a char*
    printf("Longitud calculada: %zu\n", strlen(texto_prueba));
    printf("Problemas: warning del compilador, intención poco clara\n\n");
    
    printf("FUNCIÓN CORRECTA:\n");
    printf("size_t longitud_cadena_correcto(const char* cadena)\n");
    printf("Longitud calculada: %zu\n", longitud_cadena_correcto(texto_prueba));
    printf("Beneficios: sin warnings, intención clara, tipo de retorno apropiado\n\n");
    
    printf("BÚSQUEDA DE CARÁCTER:\n");
    char caracter = 'e';
    int pos_incorrecta = buscar_caracter_incorrecto((char*)texto_prueba, caracter);
    int pos_correcta = buscar_caracter_correcto(texto_prueba, caracter);
    
    printf("Función incorrecta - posición de '%c': %d\n", caracter, pos_incorrecta);
    printf("Función correcta - posición de '%c': %d\n", caracter, pos_correcta);
    printf("Ambas dan el mismo resultado, pero la correcta es más segura.\n\n");
}

void demostrar_tipos_const(void) {
    printf("\n=== DIFERENTES TIPOS DE CONST EN C ===\n\n");
    
    int valores[] = {1, 2, 3, 4, 5};
    const int valores_constantes[] = {10, 20, 30, 40, 50};
    
    printf("1. PUNTERO A DATOS CONSTANTES (const int*):\n");
    printf("   const int* ptr = valores;\n");
    printf("   - Datos no modificables: *ptr = 999; // ERROR\n");
    printf("   - Puntero sí modificable: ptr++; // OK\n");
    int suma = sumar_array_const_data(valores, 5);
    printf("   Suma usando puntero a const: %d\n\n", suma);
    
    printf("2. PUNTERO CONSTANTE (int* const):\n");
    printf("   int* const ptr = valores;\n");
    printf("   - Datos sí modificables: *ptr = 999; // OK\n");
    printf("   - Puntero no modificable: ptr++; // ERROR\n");
    llenar_array_const_ptr(valores, 5, 100);
    printf("   Array llenado con valor 100: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", valores[i]);
    }
    printf("\n\n");
    
    printf("3. PUNTERO CONSTANTE A DATOS CONSTANTES (const int* const):\n");
    printf("   const int* const ptr = valores;\n");
    printf("   - Datos no modificables: *ptr = 999; // ERROR\n");
    printf("   - Puntero no modificable: ptr++; // ERROR\n");
    int maximo = encontrar_maximo_const_completo(valores_constantes, 5);
    printf("   Máximo en array constante: %d\n\n", maximo);
    
    printf("4. VARIABLES CONST LOCALES:\n");
    const int valor_inmutable = 42;
    printf("   const int valor = %d;\n", valor_inmutable);
    printf("   - No se puede modificar después de la inicialización\n");
    printf("   - Optimizaciones del compilador\n\n");
}

void analizar_const_correctness(estadisticas_const_t* stats) {
    if (!stats) {
        printf("Error: puntero a estadísticas nulo\n");
        return;
    }
    
    inicializar_estadisticas_const(stats);
    
    printf("\n=== ANÁLISIS DE CONST-CORRECTNESS ===\n\n");
    
    // Simular análisis de diferentes tipos de funciones
    stats->funciones_analizadas = 15;
    
    printf("Analizando funciones...\n");
    
    // Funciones con parámetros const
    stats->parametros_const = 8;
    printf("✓ Funciones con parámetros const correctos: %zu\n", stats->parametros_const);
    
    // Variables const locales
    stats->variables_const = 12;
    printf("✓ Variables const locales encontradas: %zu\n", stats->variables_const);
    
    // Mejoras detectadas
    stats->mejoras_detectadas = 5;
    printf("⚠ Funciones que podrían mejorar con const: %zu\n", stats->mejoras_detectadas);
    
    printf("\nRECOMENDACIONES:\n");
    if (stats->mejoras_detectadas > 0) {
        printf("- Agregar const a parámetros de solo lectura\n");
        printf("- Usar const para variables que no cambian\n");
        printf("- Considerar const para valores de retorno cuando sea apropiado\n");
    } else {
        printf("- ¡Excelente! El código ya es const-correct\n");
    }
    printf("\n");
}

void ejercicios_interactivos_const(void) {
    printf("\n=== EJERCICIOS INTERACTIVOS DE CONST-CORRECTNESS ===\n\n");
    
    char respuesta[10];
    int puntuacion = 0;
    
    printf("Pregunta 1: ¿Qué palabra clave debes agregar aquí?\n");
    printf("void imprimir_nombre(_____ char* nombre) {\n");
    printf("    printf(\"Nombre: %%s\\n\", nombre);\n");
    printf("}\n");
    printf("Respuesta (const/volatile/static): ");
    
    if (fgets(respuesta, sizeof(respuesta), stdin)) {
        if (strncmp(respuesta, "const", 5) == 0) {
            printf("✓ ¡Correcto! const indica que nombre no se modificará.\n\n");
            puntuacion++;
        } else {
            printf("✗ Incorrecto. La respuesta es 'const'.\n\n");
        }
    }
    
    printf("Pregunta 2: ¿Cuál es el error en esta función?\n");
    printf("int longitud(char* str) {\n");
    printf("    return strlen(str);\n");
    printf("}\n");
    printf("¿Qué falta? (escribe 'const'): ");
    
    if (fgets(respuesta, sizeof(respuesta), stdin)) {
        if (strncmp(respuesta, "const", 5) == 0) {
            printf("✓ ¡Correcto! str debería ser const char*.\n\n");
            puntuacion++;
        } else {
            printf("✗ Incorrecto. str debería ser const char*.\n\n");
        }
    }
    
    printf("Pregunta 3: ¿Qué tipo de puntero es este?\n");
    printf("const int* const ptr;\n");
    printf("a) Puntero a datos constantes\n");
    printf("b) Puntero constante\n");
    printf("c) Puntero constante a datos constantes\n");
    printf("Respuesta (a/b/c): ");
    
    if (fgets(respuesta, sizeof(respuesta), stdin)) {
        if (respuesta[0] == 'c' || respuesta[0] == 'C') {
            printf("✓ ¡Correcto! Es un puntero constante a datos constantes.\n\n");
            puntuacion++;
        } else {
            printf("✗ Incorrecto. Es un puntero constante a datos constantes (c).\n\n");
        }
    }
    
    printf("PUNTUACIÓN FINAL: %d/3\n", puntuacion);
    if (puntuacion == 3) {
        printf("¡Excelente! Dominas const-correctness.\n");
    } else if (puntuacion >= 2) {
        printf("Bien, pero repasa algunos conceptos.\n");
    } else {
        printf("Necesitas estudiar más sobre const-correctness.\n");
    }
    printf("\n");
}

void demostracion_completa_const_correctness(void) {
    printf("===============================================\n");
    printf("    DEMOSTRACIÓN COMPLETA: CONST-CORRECTNESS\n");
    printf("===============================================\n");
    
    // 1. Demostrar problemas sin const
    demostrar_problemas_sin_const();
    
    printf("Presiona Enter para continuar...\n");
    getchar();
    
    // 2. Demostrar beneficios con const
    demostrar_beneficios_const_correct();
    
    printf("Presiona Enter para continuar...\n");
    getchar();
    
    // 3. Comparación lado a lado
    comparacion_incorrecto_vs_correcto();
    
    printf("Presiona Enter para continuar...\n");
    getchar();
    
    // 4. Diferentes tipos de const
    demostrar_tipos_const();
    
    printf("Presiona Enter para continuar...\n");
    getchar();
    
    // 5. Análisis del código
    estadisticas_const_t stats;
    analizar_const_correctness(&stats);
    imprimir_estadisticas_const(&stats);
    
    printf("Presiona Enter para continuar...\n");
    getchar();
    
    // 6. Ejercicios interactivos
    ejercicios_interactivos_const();
    
    printf("¡Demostración completada!\n");
}

// =============================================================================
// FUNCIONES AUXILIARES
// =============================================================================

void inicializar_estadisticas_const(estadisticas_const_t* stats) {
    if (!stats) {
        return;
    }
    
    stats->funciones_analizadas = 0;
    stats->parametros_const = 0;
    stats->variables_const = 0;
    stats->mejoras_detectadas = 0;
}

void imprimir_estadisticas_const(const estadisticas_const_t* stats) {
    if (!stats) {
        printf("Estadísticas nulas\n");
        fflush(stdout);
        return;
    }
    
    printf("\n=== ESTADÍSTICAS DE CONST-CORRECTNESS ===\n");
    printf("Funciones analizadas: %zu\n", stats->funciones_analizadas);
    printf("Parámetros const: %zu\n", stats->parametros_const);
    printf("Variables const: %zu\n", stats->variables_const);
    printf("Mejoras detectadas: %zu\n", stats->mejoras_detectadas);
    
    if (stats->funciones_analizadas > 0) {
        double porcentaje_const = (double)stats->parametros_const / stats->funciones_analizadas * 100;
        printf("Porcentaje de const-correctness: %.1f%%\n", porcentaje_const);
    }
    
    printf("==========================================\n\n");
    fflush(stdout);
}

bool validar_puntero_no_nulo(const void* ptr, const char* nombre_variable) {
    if (!ptr) {
        if (nombre_variable) {
            printf("Error: %s es nulo\n", nombre_variable);
        } else {
            printf("Error: puntero nulo\n");
        }
        fflush(stdout);
        return false;
    }
    return true;
}

size_t contar_caracter(const char* cadena, char caracter) {
    if (!cadena) {
        return 0;
    }
    
    size_t contador = 0;
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] == caracter) {
            contador++;
        }
    }
    
    return contador;
}
