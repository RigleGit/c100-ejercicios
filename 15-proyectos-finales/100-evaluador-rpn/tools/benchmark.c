#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "evaluador_rpn.h"

#define NUM_TESTS 10000
// Redefinición local para benchmark (evitar warning)
#ifdef MAX_EXPRESSION_LENGTH
#undef MAX_EXPRESSION_LENGTH
#endif
#define MAX_EXPRESSION_LENGTH 200

// Generador de números aleatorios
double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// Generador de expresiones RPN aleatorias
void generar_expresion_aleatoria(char* buffer, int max_length) {
    char operadores[] = "+-*/^%";
    char funciones[][10] = {"sqrt", "abs", "sin", "cos", "ln", "log"};
    char constantes[][5] = {"pi", "e"};
    
    int num_operandos = rand() % 5 + 2; // 2-6 operandos
    int pos = 0;
    
    // Agregar operandos
    for (int i = 0; i < num_operandos && pos < max_length - 20; i++) {
        if (rand() % 10 < 2) { // 20% probabilidad de constante
            int const_idx = rand() % (sizeof(constantes) / sizeof(constantes[0]));
            pos += snprintf(buffer + pos, max_length - pos, "%s ", constantes[const_idx]);
        } else { // 80% probabilidad de número
            double num = random_double(-100.0, 100.0);
            if (rand() % 2) { // 50% números enteros
                pos += snprintf(buffer + pos, max_length - pos, "%.0f ", num);
            } else { // 50% números decimales
                pos += snprintf(buffer + pos, max_length - pos, "%.2f ", num);
            }
        }
    }
    
    // Agregar operadores (uno menos que operandos)
    for (int i = 0; i < num_operandos - 1 && pos < max_length - 10; i++) {
        if (rand() % 10 < 3) { // 30% probabilidad de función
            int func_idx = rand() % (sizeof(funciones) / sizeof(funciones[0]));
            pos += snprintf(buffer + pos, max_length - pos, "%s ", funciones[func_idx]);
        } else { // 70% probabilidad de operador binario
            char op = operadores[rand() % strlen(operadores)];
            pos += snprintf(buffer + pos, max_length - pos, "%c ", op);
        }
    }
    
    // Eliminar espacio final
    if (pos > 0 && buffer[pos - 1] == ' ') {
        buffer[pos - 1] = '\0';
    }
}

// Función principal de benchmark
int main(void) {
    printf("=== BENCHMARK DEL EVALUADOR RPN ===\n");
    printf("Generando y evaluando %d expresiones aleatorias...\n\n", NUM_TESTS);
    
    srand((unsigned int)time(NULL));
    
    clock_t inicio_total = clock();
    
    int exitosos = 0;
    int errores = 0;
    double tiempo_total_evaluacion = 0.0;
    double suma_resultados = 0.0;
    
    // Estadísticas por tipo de error
    int errores_division_cero = 0;
    int errores_dominio = 0;
    int errores_operandos = 0;
    int errores_otros = 0;
    
    printf("Progreso: ");
    fflush(stdout);
    
    for (int i = 0; i < NUM_TESTS; i++) {
        // Mostrar progreso
        if (i % (NUM_TESTS / 20) == 0) {
            printf("█");
            fflush(stdout);
        }
        
        char expresion[MAX_EXPRESSION_LENGTH];
        generar_expresion_aleatoria(expresion, MAX_EXPRESSION_LENGTH);
        
        clock_t inicio_eval = clock();
        resultado_evaluacion_t status = evaluar_rpn(expresion);
        clock_t fin_eval = clock();
        
        double tiempo_eval = ((double)(fin_eval - inicio_eval)) / CLOCKS_PER_SEC;
        tiempo_total_evaluacion += tiempo_eval;
        
        if (status.error == RPN_SUCCESS) {
            exitosos++;
            suma_resultados += status.resultado;
        } else {
            errores++;
            
            // Clasificar errores
            switch (status.error) {
                case RPN_ERROR_DIVISION_CERO:
                    errores_division_cero++;
                    break;
                case RPN_ERROR_EXPRESION_MALFORMADA:
                case RPN_ERROR_TOKEN_INVALIDO:
                    errores_operandos++;
                    break;
                default:
                    errores_otros++;
                    break;
            }
        }
    }
    
    clock_t fin_total = clock();
    double tiempo_total = ((double)(fin_total - inicio_total)) / CLOCKS_PER_SEC;
    
    printf("\n\n=== RESULTADOS DEL BENCHMARK ===\n");
    printf("Tiempo total: %.3f segundos\n", tiempo_total);
    printf("Tiempo promedio por evaluación: %.6f segundos\n", tiempo_total_evaluacion / NUM_TESTS);
    printf("Evaluaciones por segundo: %.0f\n", NUM_TESTS / tiempo_total);
    printf("\n");
    
    printf("=== ESTADÍSTICAS DE ÉXITO ===\n");
    printf("Evaluaciones exitosas: %d (%.1f%%)\n", exitosos, (exitosos * 100.0) / NUM_TESTS);
    printf("Evaluaciones con error: %d (%.1f%%)\n", errores, (errores * 100.0) / NUM_TESTS);
    printf("\n");
    
    if (exitosos > 0) {
        printf("=== ESTADÍSTICAS DE RESULTADOS ===\n");
        printf("Suma total de resultados: %.6f\n", suma_resultados);
        printf("Promedio de resultados: %.6f\n", suma_resultados / exitosos);
        printf("\n");
    }
    
    printf("=== TIPOS DE ERRORES ===\n");
    printf("División por cero: %d (%.1f%%)\n", errores_division_cero, (errores_division_cero * 100.0) / NUM_TESTS);
    printf("Errores de dominio: %d (%.1f%%)\n", errores_dominio, (errores_dominio * 100.0) / NUM_TESTS);
    printf("Operandos insuficientes: %d (%.1f%%)\n", errores_operandos, (errores_operandos * 100.0) / NUM_TESTS);
    printf("Otros errores: %d (%.1f%%)\n", errores_otros, (errores_otros * 100.0) / NUM_TESTS);
    printf("\n");
    
    // Benchmark de expresiones específicas
    printf("=== BENCHMARK DE EXPRESIONES ESPECÍFICAS ===\n");
    
    char* expresiones_test[] = {
        "3 4 +",
        "15 7 1 1 + - /",
        "2 3 ^ 1 + sqrt",
        "pi 2 / sin 2 ^",
        "e ln 1 + 2 / 3 * 4 - 5 +",
        NULL
    };
    
    for (int i = 0; expresiones_test[i] != NULL; i++) {
        clock_t inicio = clock();
        
        // Evaluar la misma expresión muchas veces
        for (int j = 0; j < 10000; j++) {
            (void)evaluar_rpn(expresiones_test[i]);
        }
        
        clock_t fin = clock();
        double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
        
        printf("Expresión \"%s\": %.6f seg (10k evaluaciones)\n", 
               expresiones_test[i], tiempo);
    }
    
    printf("\n");
    
    // Test de memoria con expresiones largas
    printf("=== TEST DE MEMORIA (EXPRESIONES LARGAS) ===\n");
    
    char expresion_larga[2000];
    strcpy(expresion_larga, "1");
    
    for (int i = 2; i <= 500; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), " %d +", i);
        strcat(expresion_larga, temp);
    }
    
    clock_t inicio_largo = clock();
    resultado_evaluacion_t rl_eval = evaluar_rpn(expresion_larga);
    clock_t fin_largo = clock();
    
    double tiempo_largo = ((double)(fin_largo - inicio_largo)) / CLOCKS_PER_SEC;
    
    printf("Expresión con 500 operaciones:\n");
    printf("Tiempo: %.6f segundos\n", tiempo_largo);
    printf("Estado: %s\n", rl_eval.error == RPN_SUCCESS ? "ÉXITO" : "ERROR");
    if (rl_eval.error == RPN_SUCCESS) {
        printf("Resultado: %.0f (esperado: 125250)\n", rl_eval.resultado);
    }
    
    printf("\n=== BENCHMARK COMPLETADO ===\n");
    
    return 0;
}
