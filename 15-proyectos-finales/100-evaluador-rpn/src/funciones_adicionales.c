#include "evaluador_rpn.h"

// ========================================
// FUNCIONES DE DEMOSTRACIÓN Y TESTING
// ========================================

typedef struct {
    const char *expresion;
    double resultado_esperado;
    rpn_error_t error_esperado;
    const char *descripcion;
} caso_prueba_t;

static const caso_prueba_t CASOS_PRUEBA[] = {
    // Casos exitosos
    {"5 3 +", 8.0, RPN_SUCCESS, "Suma básica"},
    {"10 2 -", 8.0, RPN_SUCCESS, "Resta básica"},
    {"4 3 *", 12.0, RPN_SUCCESS, "Multiplicación básica"},
    {"15 3 /", 5.0, RPN_SUCCESS, "División básica"},
    {"2 3 ^", 8.0, RPN_SUCCESS, "Potencia básica"},
    {"15 7 1 1 + - /", 3.0, RPN_SUCCESS, "Expresión compleja 1"},
    {"3 4 + 2 * 1 -", 13.0, RPN_SUCCESS, "Expresión compleja 2"},
    {"2 3 4 + *", 14.0, RPN_SUCCESS, "Expresión compleja 3"},
    {"5 1 2 + 4 * + 3 -", 14.0, RPN_SUCCESS, "Expresión compleja 4"},
    {"1 2 + 3 4 + *", 21.0, RPN_SUCCESS, "Expresión compleja 5"},
    {"-5 3 +", -2.0, RPN_SUCCESS, "Número negativo"},
    {"3.14 2 *", 6.28, RPN_SUCCESS, "Número decimal"},
    {"10 3 %", 1.0, RPN_SUCCESS, "Módulo"},
    
    // Casos de error
    {"5 +", 0.0, RPN_ERROR_EXPRESION_MALFORMADA, "Operandos insuficientes"},
    {"5 3", 0.0, RPN_ERROR_EXPRESION_MALFORMADA, "Sin operador"},
    {"5 0 /", 0.0, RPN_ERROR_DIVISION_CERO, "División por cero"},
    {"5 abc +", 0.0, RPN_ERROR_TOKEN_INVALIDO, "Token inválido"},
    {"", 0.0, RPN_ERROR_ENTRADA_INVALIDA, "Expresión vacía"},
    {"5 3 & +", 0.0, RPN_ERROR_TOKEN_INVALIDO, "Operador inválido"},
    {"5 3 + 2", 0.0, RPN_ERROR_EXPRESION_MALFORMADA, "Demasiados operandos"},
    
    {NULL, 0.0, RPN_SUCCESS, NULL} // Terminador
};

bool ejecutar_pruebas_rpn(void) {
    printf("Ejecutando suite de pruebas...\n");
    printf("─────────────────────────────────\n");
    
    int pruebas_ejecutadas = 0;
    int pruebas_pasadas = 0;
    
    for (int i = 0; CASOS_PRUEBA[i].expresion != NULL; i++) {
        pruebas_ejecutadas++;
        
        printf("Prueba %d: %s\n", i + 1, CASOS_PRUEBA[i].descripcion);
        printf("  Expresión: '%s'\n", CASOS_PRUEBA[i].expresion);
        
        resultado_evaluacion_t resultado = evaluar_rpn(CASOS_PRUEBA[i].expresion);
        
        bool prueba_exitosa = true;
        
        if (resultado.error != CASOS_PRUEBA[i].error_esperado) {
            printf("  ❌ Error esperado: %s, obtenido: %s\n",
                   error_a_string(CASOS_PRUEBA[i].error_esperado),
                   error_a_string(resultado.error));
            prueba_exitosa = false;
        }
        
        if (resultado.error == RPN_SUCCESS) {
            double diferencia = fabs(resultado.resultado - CASOS_PRUEBA[i].resultado_esperado);
            if (diferencia > 1e-6) {
                printf("  ❌ Resultado esperado: %.6f, obtenido: %.6f\n",
                       CASOS_PRUEBA[i].resultado_esperado, resultado.resultado);
                prueba_exitosa = false;
            }
        }
        
        if (prueba_exitosa) {
            printf("  ✅ PASÓ\n");
            pruebas_pasadas++;
        } else {
            printf("  ❌ FALLÓ\n");
            if (resultado.error != RPN_SUCCESS) {
                printf("     Mensaje de error: %s\n", resultado.mensaje_error);
            }
        }
        
        printf("\n");
    }
    
    printf("─────────────────────────────────\n");
    printf("Resultados: %d/%d pruebas pasaron (%.1f%%)\n",
           pruebas_pasadas, pruebas_ejecutadas,
           (double)pruebas_pasadas / pruebas_ejecutadas * 100.0);
    
    return pruebas_pasadas == pruebas_ejecutadas;
}

void demostrar_evaluador_rpn(void) {
    printf("\n=== DEMOSTRACIÓN DEL EVALUADOR RPN ===\n\n");
    
    printf("1. EVALUACIÓN PASO A PASO\n");
    printf("─────────────────────────\n");
    
    // Habilitar verbosidad para la demostración
    int verbosidad_original = 0;
    establecer_verbosidad(2);
    establecer_modo_paso_a_paso(true);
    
    const char *ejemplo = "3 4 + 2 * 1 -";
    printf("Expresión: '%s'\n", ejemplo);
    printf("Evaluación paso a paso:\n\n");
    
    resultado_evaluacion_t resultado = evaluar_rpn(ejemplo);
    
    if (resultado.error == RPN_SUCCESS) {
        printf("\nResultado final: %.6f\n", resultado.resultado);
    } else {
        printf("\nError: %s\n", resultado.mensaje_error);
    }
    
    // Restaurar configuración
    establecer_verbosidad(verbosidad_original);
    establecer_modo_paso_a_paso(false);
    
    printf("\n2. COMPARACIÓN CON NOTACIÓN INFIJA\n");
    printf("──────────────────────────────────\n");
    
    const char *ejemplos[][2] = {
        {"2 + 3", "2 3 +"},
        {"(2 + 3) * 4", "2 3 + 4 *"},
        {"2 + 3 * 4", "2 3 4 * +"},
        {"(1 + 2) * (3 + 4)", "1 2 + 3 4 + *"},
        {NULL, NULL}
    };
    
    printf("Infija              RPN              Resultado\n");
    printf("───────────────────────────────────────────────\n");
    
    for (int i = 0; ejemplos[i][0] != NULL; i++) {
        resultado_evaluacion_t res = evaluar_rpn(ejemplos[i][1]);
        printf("%-15s → %-15s = %.1f\n",
               ejemplos[i][0], ejemplos[i][1],
               res.error == RPN_SUCCESS ? res.resultado : 0.0);
    }
    
    printf("\n3. VENTAJAS DE RPN\n");
    printf("──────────────────\n");
    printf("• No requiere paréntesis\n");
    printf("• Evaluación eficiente con pila\n");
    printf("• Sin ambigüedad en precedencia\n");
    printf("• Fácil implementación en calculadoras\n");
    printf("• Usado en calculadoras HP y lenguajes como PostScript\n");
}

size_t procesar_archivo_rpn(const char *nombre_archivo, bool mostrar_pasos) {
    if (!nombre_archivo) {
        printf("Error: nombre de archivo nulo\n");
        return 0;
    }
    
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        printf("Error: no se pudo abrir el archivo '%s'\n", nombre_archivo);
        return 0;
    }
    
    size_t expresiones_procesadas = 0;
    size_t expresiones_exitosas = 0;
    char linea[MAX_EXPRESSION_LENGTH];
    int numero_linea = 0;
    
    // Configurar verbosidad según el parámetro
    int verbosidad_original = 0;
    if (mostrar_pasos) {
        establecer_verbosidad(2);
    }
    
    while (fgets(linea, sizeof(linea), archivo)) {
        numero_linea++;
        
        // Eliminar salto de línea y espacios al final
        size_t len = strlen(linea);
        while (len > 0 && (linea[len-1] == '\n' || linea[len-1] == '\r' || 
                          linea[len-1] == ' ' || linea[len-1] == '\t')) {
            linea[--len] = '\0';
        }
        
        // Saltar líneas vacías o comentarios
        if (len == 0 || linea[0] == '#' || linea[0] == ';') {
            continue;
        }
        
        expresiones_procesadas++;
        
        printf("\nLínea %d: '%s'\n", numero_linea, linea);
        
        if (mostrar_pasos) {
            printf("Evaluación paso a paso:\n");
        }
        
        resultado_evaluacion_t resultado = evaluar_rpn(linea);
        
        if (resultado.error == RPN_SUCCESS) {
            printf("✅ Resultado: %.6f\n", resultado.resultado);
            expresiones_exitosas++;
        } else {
            printf("❌ Error: %s\n", resultado.mensaje_error);
        }
        
        if (mostrar_pasos) {
            printf("Tokens procesados: %zu, Operaciones: %zu\n",
                   resultado.tokens_procesados, resultado.operaciones_realizadas);
        }
    }
    
    fclose(archivo);
    
    // Restaurar verbosidad
    if (mostrar_pasos) {
        establecer_verbosidad(verbosidad_original);
    }
    
    printf("\n=== RESUMEN ===\n");
    printf("Expresiones en archivo: %zu\n", expresiones_procesadas);
    printf("Evaluaciones exitosas: %zu\n", expresiones_exitosas);
    printf("Errores: %zu\n", expresiones_procesadas - expresiones_exitosas);
    
    if (expresiones_procesadas > 0) {
        double tasa_exito = (double)expresiones_exitosas / expresiones_procesadas * 100.0;
        printf("Tasa de éxito: %.1f%%\n", tasa_exito);
    }
    
    return expresiones_exitosas;
}

// ========================================
// FUNCIÓN DE CONVERSIÓN INFIJA A POSTFIJA
// ========================================

static int obtener_precedencia(char operador) {
    const operador_t *op = obtener_operador(operador);
    return op ? op->precedencia : -1;
}

static bool es_asociativo_izquierda(char operador) {
    const operador_t *op = obtener_operador(operador);
    return op ? op->asociativo_izquierda : true;
}

rpn_error_t infija_a_postfija(const char *infija, char *postfija, size_t tamaño_buffer) {
    if (!infija || !postfija || tamaño_buffer == 0) {
        return RPN_ERROR_ENTRADA_INVALIDA;
    }
    
    // Pila para operadores
    char pila_ops[256];
    int cima_ops = -1;
    
    size_t pos_salida = 0;
    size_t len_infija = strlen(infija);
    
    for (size_t i = 0; i < len_infija; i++) {
        char c = infija[i];
        
        // Saltar espacios
        if (isspace(c)) {
            continue;
        }
        
        // Números (incluye decimales y negativos)
        if (isdigit(c) || c == '.') {
            // Agregar espacio si no es el primer token
            if (pos_salida > 0 && postfija[pos_salida-1] != ' ') {
                if (pos_salida >= tamaño_buffer - 1) return RPN_ERROR_OVERFLOW;
                postfija[pos_salida++] = ' ';
            }
            
            // Copiar todo el número
            while (i < len_infija && (isdigit(infija[i]) || infija[i] == '.')) {
                if (pos_salida >= tamaño_buffer - 1) return RPN_ERROR_OVERFLOW;
                postfija[pos_salida++] = infija[i++];
            }
            i--; // Retroceder uno porque el for incrementará
        }
        // Paréntesis izquierdo
        else if (c == '(') {
            if (cima_ops >= 255) return RPN_ERROR_OVERFLOW;
            pila_ops[++cima_ops] = c;
        }
        // Paréntesis derecho
        else if (c == ')') {
            while (cima_ops >= 0 && pila_ops[cima_ops] != '(') {
                if (pos_salida >= tamaño_buffer - 2) return RPN_ERROR_OVERFLOW;
                if (pos_salida > 0) postfija[pos_salida++] = ' ';
                postfija[pos_salida++] = pila_ops[cima_ops--];
            }
            if (cima_ops < 0) return RPN_ERROR_EXPRESION_MALFORMADA; // Paréntesis no balanceados
            cima_ops--; // Remover '('
        }
        // Operadores
        else {
            char opstr[2] = { c, '\0' };
            if (!es_operador(opstr)) {
                return RPN_ERROR_TOKEN_INVALIDO;
            }
            int prec_actual = obtener_precedencia(c);
            if (prec_actual == -1) return RPN_ERROR_OPERADOR_DESCONOCIDO;
            
            while (cima_ops >= 0 && pila_ops[cima_ops] != '(' &&
                   ((obtener_precedencia(pila_ops[cima_ops]) > prec_actual) ||
                    (obtener_precedencia(pila_ops[cima_ops]) == prec_actual && 
                     es_asociativo_izquierda(c)))) {
                if (pos_salida >= tamaño_buffer - 2) return RPN_ERROR_OVERFLOW;
                if (pos_salida > 0) postfija[pos_salida++] = ' ';
                postfija[pos_salida++] = pila_ops[cima_ops--];
            }
            
            if (cima_ops >= 255) return RPN_ERROR_OVERFLOW;
            pila_ops[++cima_ops] = c;
        }
    }
    
    // Vaciar pila de operadores
    while (cima_ops >= 0) {
        if (pila_ops[cima_ops] == '(' || pila_ops[cima_ops] == ')') {
            return RPN_ERROR_EXPRESION_MALFORMADA; // Paréntesis no balanceados
        }
        if (pos_salida >= tamaño_buffer - 2) return RPN_ERROR_OVERFLOW;
        if (pos_salida > 0) postfija[pos_salida++] = ' ';
        postfija[pos_salida++] = pila_ops[cima_ops--];
    }
    
    postfija[pos_salida] = '\0';
    return RPN_SUCCESS;
}
