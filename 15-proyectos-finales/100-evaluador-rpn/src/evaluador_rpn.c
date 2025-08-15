#include "evaluador_rpn.h"
#include <time.h>
#include <float.h>

// Variables globales para configuración y estadísticas
static estadisticas_rpn_t g_estadisticas = {0};
static int g_verbosidad = 0;
static bool g_modo_paso_a_paso = false;

// Operadores soportados
static const operador_t OPERADORES[] = {
    {'+', "suma", 1, true, NULL},
    {'-', "resta", 1, true, NULL},
    {'*', "multiplicación", 2, true, NULL},
    {'/', "división", 2, true, NULL},
    {'^', "potencia", 3, false, NULL},
    {'%', "módulo", 2, true, NULL},
    {0, NULL, 0, false, NULL}  // Terminador
};

// ========================================
// FUNCIONES DE MANEJO DE PILA
// ========================================

pila_t *pila_crear(size_t capacidad_maxima) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (!pila) return NULL;
    
    pila->cima = NULL;
    pila->tamaño = 0;
    pila->capacidad_maxima = capacidad_maxima;
    
    return pila;
}

void pila_destruir(pila_t *pila) {
    if (!pila) return;
    
    pila_limpiar(pila);
    free(pila);
}

rpn_error_t pila_push(pila_t *pila, double valor) {
    if (!pila) return RPN_ERROR_ENTRADA_INVALIDA;
    
    // Verificar capacidad máxima
    if (pila->capacidad_maxima > 0 && pila->tamaño >= pila->capacidad_maxima) {
        return RPN_ERROR_OVERFLOW;
    }
    
    // Verificar que el valor sea válido
    if (isnan(valor) || isinf(valor)) {
        return RPN_ERROR_ENTRADA_INVALIDA;
    }
    
    nodo_pila_t *nuevo = malloc(sizeof(nodo_pila_t));
    if (!nuevo) return RPN_ERROR_MEMORIA;
    
    nuevo->valor = valor;
    nuevo->siguiente = pila->cima;
    pila->cima = nuevo;
    pila->tamaño++;
    
    if (g_verbosidad >= 3) {
        printf("[DEBUG] Push: %.6f (tamaño pila: %zu)\n", valor, pila->tamaño);
    }
    
    return RPN_SUCCESS;
}

rpn_error_t pila_pop(pila_t *pila, double *valor) {
    if (!pila || !valor) return RPN_ERROR_ENTRADA_INVALIDA;
    
    if (pila_esta_vacia(pila)) {
        return RPN_ERROR_PILA_VACIA;
    }
    
    nodo_pila_t *temp = pila->cima;
    *valor = temp->valor;
    pila->cima = temp->siguiente;
    pila->tamaño--;
    
    if (g_verbosidad >= 3) {
        printf("[DEBUG] Pop: %.6f (tamaño pila: %zu)\n", *valor, pila->tamaño);
    }
    
    free(temp);
    return RPN_SUCCESS;
}

rpn_error_t pila_peek(const pila_t *pila, double *valor) {
    if (!pila || !valor) return RPN_ERROR_ENTRADA_INVALIDA;
    
    if (pila_esta_vacia(pila)) {
        return RPN_ERROR_PILA_VACIA;
    }
    
    *valor = pila->cima->valor;
    return RPN_SUCCESS;
}

bool pila_esta_vacia(const pila_t *pila) {
    return !pila || pila->cima == NULL;
}

size_t pila_tamaño(const pila_t *pila) {
    return pila ? pila->tamaño : 0;
}

void pila_limpiar(pila_t *pila) {
    if (!pila) return;
    
    while (!pila_esta_vacia(pila)) {
        double temp;
        pila_pop(pila, &temp);
    }
}

void pila_imprimir(const pila_t *pila) {
    if (!pila) {
        printf("Pila: NULL\n");
        return;
    }
    
    printf("Pila (tamaño %zu): [", pila->tamaño);
    
    nodo_pila_t *actual = pila->cima;
    bool primero = true;
    
    while (actual) {
        if (!primero) printf(", ");
        printf("%.3f", actual->valor);
        actual = actual->siguiente;
        primero = false;
    }
    
    printf("]\n");
}

// ========================================
// FUNCIONES DE EVALUACIÓN RPN
// ========================================

bool es_numero(const char *token) {
    if (!token || *token == '\0') return false;
    
    char *fin_ptr;
    errno = 0;
    strtod(token, &fin_ptr);
    
    // Verificar que se consumió todo el token y no hubo error
    return *fin_ptr == '\0' && errno == 0;
}

bool es_operador(const char *token) {
    if (!token || strlen(token) != 1) return false;
    
    char c = token[0];
    return strchr("+-*/%^", c) != NULL;
}

rpn_error_t token_a_numero(const char *token, double *valor) {
    if (!token || !valor) return RPN_ERROR_ENTRADA_INVALIDA;
    
    char *fin_ptr;
    errno = 0;
    *valor = strtod(token, &fin_ptr);
    
    if (*fin_ptr != '\0') return RPN_ERROR_TOKEN_INVALIDO;
    if (errno == ERANGE) return RPN_ERROR_OVERFLOW;
    if (isnan(*valor) || isinf(*valor)) return RPN_ERROR_OVERFLOW;
    
    return RPN_SUCCESS;
}

rpn_error_t aplicar_operador(char operador, double a, double b, double *resultado) {
    if (!resultado) return RPN_ERROR_ENTRADA_INVALIDA;
    
    if (g_verbosidad >= 2) {
        printf("[DEBUG] Aplicando: %.3f %c %.3f\n", a, operador, b);
    }
    
    switch (operador) {
        case '+':
            *resultado = a + b;
            break;
        case '-':
            *resultado = a - b;
            break;
        case '*':
            *resultado = a * b;
            break;
        case '/':
            if (b == 0.0) return RPN_ERROR_DIVISION_CERO;
            *resultado = a / b;
            break;
        case '^':
            *resultado = pow(a, b);
            if (isnan(*resultado) || isinf(*resultado)) {
                return RPN_ERROR_OVERFLOW;
            }
            break;
        case '%':
            if (b == 0.0) return RPN_ERROR_DIVISION_CERO;
            *resultado = fmod(a, b);
            break;
        default:
            return RPN_ERROR_OPERADOR_DESCONOCIDO;
    }
    
    // Verificar overflow/underflow en el resultado
    if (isnan(*resultado) || isinf(*resultado)) {
        return RPN_ERROR_OVERFLOW;
    }
    
    if (g_verbosidad >= 2) {
        printf("[DEBUG] Resultado: %.6f\n", *resultado);
    }
    
    g_estadisticas.operaciones_aritmeticas++;
    
    return RPN_SUCCESS;
}

resultado_evaluacion_t evaluar_rpn(const char *expresion) {
    resultado_evaluacion_t resultado = {0};
    
    if (!expresion) {
        resultado.error = RPN_ERROR_ENTRADA_INVALIDA;
        strcpy(resultado.mensaje_error, "Expresión nula");
        return resultado;
    }
    
    pila_t *pila = pila_crear(1000);  // Capacidad máxima razonable
    if (!pila) {
        resultado.error = RPN_ERROR_MEMORIA;
        strcpy(resultado.mensaje_error, "Error al crear pila");
        return resultado;
    }
    
    resultado = evaluar_rpn_con_pila(expresion, pila);
    pila_destruir(pila);
    
    return resultado;
}

resultado_evaluacion_t evaluar_rpn_con_pila(const char *expresion, pila_t *pila) {
    resultado_evaluacion_t resultado = {0};
    clock_t inicio = clock();
    
    g_estadisticas.total_evaluaciones++;
    
    if (!expresion || !pila) {
        resultado.error = RPN_ERROR_ENTRADA_INVALIDA;
        strcpy(resultado.mensaje_error, "Parámetros inválidos");
        return resultado;
    }
    
    // Validar expresión
    rpn_error_t error_validacion = validar_expresion_rpn(expresion);
    if (error_validacion != RPN_SUCCESS) {
        resultado.error = error_validacion;
        snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                "Error de validación: %s", error_a_string(error_validacion));
        return resultado;
    }
    
    // Limpiar la pila antes de comenzar
    pila_limpiar(pila);
    
    // Crear copia de la expresión para tokenizar
    char expresion_copia[MAX_EXPRESSION_LENGTH];
    strncpy(expresion_copia, expresion, sizeof(expresion_copia) - 1);
    expresion_copia[sizeof(expresion_copia) - 1] = '\0';
    
    char *token = strtok(expresion_copia, " \t\n");
    
    while (token != NULL) {
        resultado.tokens_procesados++;
        
        if (g_verbosidad >= 1) {
            printf("[INFO] Procesando token: '%s'\n", token);
        }
        
        if (g_modo_paso_a_paso) {
            printf("Presiona Enter para continuar...");
            getchar();
        }
        
        if (es_numero(token)) {
            double valor;
            rpn_error_t error = token_a_numero(token, &valor);
            if (error != RPN_SUCCESS) {
                resultado.error = error;
                snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                        "Error convirtiendo token '%s': %s", token, error_a_string(error));
                return resultado;
            }
            
            error = pila_push(pila, valor);
            if (error != RPN_SUCCESS) {
                resultado.error = error;
                snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                        "Error apilando valor %.3f: %s", valor, error_a_string(error));
                return resultado;
            }
        }
        else if (es_operador(token) && strlen(token) == 1) {
            if (pila_tamaño(pila) < 2) {
                resultado.error = RPN_ERROR_EXPRESION_MALFORMADA;
                snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                        "Operador '%s' requiere 2 operandos, pero la pila tiene %zu",
                        token, pila_tamaño(pila));
                return resultado;
            }
            
            double b, a;
            pila_pop(pila, &b);
            pila_pop(pila, &a);
            
            double resultado_op;
            rpn_error_t error = aplicar_operador(token[0], a, b, &resultado_op);
            if (error != RPN_SUCCESS) {
                resultado.error = error;
                snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                        "Error aplicando operador '%s': %s", token, error_a_string(error));
                return resultado;
            }
            
            error = pila_push(pila, resultado_op);
            if (error != RPN_SUCCESS) {
                resultado.error = error;
                snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                        "Error apilando resultado: %s", error_a_string(error));
                return resultado;
            }
            
            resultado.operaciones_realizadas++;
        }
        else {
            resultado.error = RPN_ERROR_TOKEN_INVALIDO;
            snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                    "Token inválido: '%s'", token);
            return resultado;
        }
        
        if (g_verbosidad >= 2) {
            pila_imprimir(pila);
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    // Verificar que quede exactamente un resultado
    if (pila_tamaño(pila) != 1) {
        resultado.error = RPN_ERROR_EXPRESION_MALFORMADA;
        snprintf(resultado.mensaje_error, sizeof(resultado.mensaje_error),
                "Expresión mal formada: quedan %zu valores en la pila (esperado: 1)",
                pila_tamaño(pila));
        return resultado;
    }
    
    // Obtener resultado final
    pila_pop(pila, &resultado.resultado);
    resultado.error = RPN_SUCCESS;
    strcpy(resultado.mensaje_error, "Evaluación exitosa");
    
    // Actualizar estadísticas
    clock_t fin = clock();
    double tiempo_ms = ((double)(fin - inicio) / CLOCKS_PER_SEC) * 1000.0;
    g_estadisticas.tiempo_total_ms += tiempo_ms;
    g_estadisticas.evaluaciones_exitosas++;
    
    if (g_verbosidad >= 1) {
        printf("[INFO] Evaluación exitosa: %.6f (tiempo: %.3f ms)\n", 
               resultado.resultado, tiempo_ms);
    }
    
    return resultado;
}

// ========================================
// FUNCIONES DE VALIDACIÓN Y UTILIDAD
// ========================================

rpn_error_t validar_expresion_rpn(const char *expresion) {
    if (!expresion) return RPN_ERROR_ENTRADA_INVALIDA;
    
    // Verificar que no esté vacía
    if (strlen(expresion) == 0) return RPN_ERROR_ENTRADA_INVALIDA;
    
    // Tokenizar y validar estructura básica
    char tokens[MAX_OPERADORES][MAX_TOKEN_LENGTH];
    size_t num_tokens;
    
    rpn_error_t error = tokenizar_expresion(expresion, tokens, MAX_OPERADORES, &num_tokens);
    if (error != RPN_SUCCESS) return error;
    
    if (num_tokens == 0) return RPN_ERROR_ENTRADA_INVALIDA;
    
    // Validar que haya al menos un token
    int operandos = 0;
    int operadores = 0;
    
    for (size_t i = 0; i < num_tokens; i++) {
        if (es_numero(tokens[i])) {
            operandos++;
        } else if (es_operador(tokens[i])) {
            operadores++;
        } else {
            return RPN_ERROR_TOKEN_INVALIDO;
        }
    }
    
    // Para una expresión RPN válida: operandos = operadores + 1
    if (operandos != operadores + 1) {
        return RPN_ERROR_EXPRESION_MALFORMADA;
    }
    
    return RPN_SUCCESS;
}

const char *error_a_string(rpn_error_t error) {
    switch (error) {
        case RPN_SUCCESS: 
            return "Éxito";
        case RPN_ERROR_PILA_VACIA: 
            return "Pila vacía";
        case RPN_ERROR_EXPRESION_MALFORMADA: 
            return "Expresión mal formada";
        case RPN_ERROR_DIVISION_CERO: 
            return "División por cero";
        case RPN_ERROR_OPERADOR_DESCONOCIDO: 
            return "Operador desconocido";
        case RPN_ERROR_TOKEN_INVALIDO: 
            return "Token inválido";
        case RPN_ERROR_MEMORIA: 
            return "Error de memoria";
        case RPN_ERROR_ENTRADA_INVALIDA: 
            return "Entrada inválida";
        case RPN_ERROR_OVERFLOW: 
            return "Overflow/Underflow numérico";
        case RPN_ERROR_UNDERFLOW: 
            return "Underflow de pila";
        default: 
            return "Error desconocido";
    }
}

const operador_t *obtener_operador(char simbolo) {
    for (int i = 0; OPERADORES[i].simbolo != 0; i++) {
        if (OPERADORES[i].simbolo == simbolo) {
            return &OPERADORES[i];
        }
    }
    return NULL;
}

const operador_t *obtener_operadores_soportados(void) {
    return OPERADORES;
}

rpn_error_t tokenizar_expresion(const char *expresion, char tokens[][MAX_TOKEN_LENGTH], 
                               size_t max_tokens, size_t *num_tokens) {
    if (!expresion || !tokens || !num_tokens) return RPN_ERROR_ENTRADA_INVALIDA;
    
    *num_tokens = 0;
    
    char expresion_copia[MAX_EXPRESSION_LENGTH];
    strncpy(expresion_copia, expresion, sizeof(expresion_copia) - 1);
    expresion_copia[sizeof(expresion_copia) - 1] = '\0';
    
    char *token = strtok(expresion_copia, " \t\n");
    
    while (token != NULL && *num_tokens < max_tokens) {
        if (strlen(token) >= MAX_TOKEN_LENGTH) {
            return RPN_ERROR_TOKEN_INVALIDO;
        }
        
        strcpy(tokens[*num_tokens], token);
        (*num_tokens)++;
        
        token = strtok(NULL, " \t\n");
    }
    
    return RPN_SUCCESS;
}

// ========================================
// FUNCIONES DE CONFIGURACIÓN
// ========================================

void establecer_verbosidad(int nivel) {
    g_verbosidad = (nivel < 0) ? 0 : (nivel > 3) ? 3 : nivel;
}

void establecer_modo_paso_a_paso(bool habilitar) {
    g_modo_paso_a_paso = habilitar;
}

const char *obtener_version(void) {
    return "1.0.0";
}

// ========================================
// FUNCIONES DE ESTADÍSTICAS
// ========================================

void inicializar_estadisticas(void) {
    memset(&g_estadisticas, 0, sizeof(g_estadisticas));
}

estadisticas_rpn_t obtener_estadisticas(void) {
    return g_estadisticas;
}

void imprimir_estadisticas(void) {
    printf("\n=== ESTADÍSTICAS DEL EVALUADOR RPN ===\n");
    printf("Total de evaluaciones: %zu\n", g_estadisticas.total_evaluaciones);
    printf("Evaluaciones exitosas: %zu\n", g_estadisticas.evaluaciones_exitosas);
    printf("Errores encontrados: %zu\n", g_estadisticas.errores_encontrados);
    printf("Operaciones aritméticas: %zu\n", g_estadisticas.operaciones_aritmeticas);
    printf("Tiempo total: %.3f ms\n", g_estadisticas.tiempo_total_ms);
    
    if (g_estadisticas.total_evaluaciones > 0) {
        double tasa_exito = (double)g_estadisticas.evaluaciones_exitosas / 
                           g_estadisticas.total_evaluaciones * 100.0;
        printf("Tasa de éxito: %.1f%%\n", tasa_exito);
        
        double tiempo_promedio = g_estadisticas.tiempo_total_ms / 
                                g_estadisticas.total_evaluaciones;
        printf("Tiempo promedio por evaluación: %.3f ms\n", tiempo_promedio);
    }
    
    printf("======================================\n\n");
}

void reiniciar_estadisticas(void) {
    inicializar_estadisticas();
}
