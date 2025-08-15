/*
 * Ejercicio 026: Paridad con Bits
 * Determina si un número es par o impar usando operaciones a nivel de bit
 * en lugar del operador módulo (%).
 * 
 * Conceptos: Operaciones bit a bit, AND (&), manipulación binaria
 * Dificultad: Básico-Intermedio
 */

#include <stdio.h>

/**
 * Determina si un número es par usando operación AND bit a bit
 * @param numero El número a verificar
 * @return 1 si es par, 0 si es impar
 */
int es_par_bits(int numero) {
    return (numero & 1) == 0;
}

/**
 * Determina si un número es impar usando operación AND bit a bit
 * @param numero El número a verificar
 * @return 1 si es impar, 0 si es par
 */
int es_impar_bits(int numero) {
    return (numero & 1) == 1;
}

/**
 * Muestra la representación binaria de un número (últimos 8 bits)
 * @param numero El número a mostrar en binario
 */
void mostrar_binario_8bits(int numero) {
    printf("Binario (8 bits): ");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (numero >> i) & 1);
    }
    printf("\n");
}

/**
 * Muestra la representación binaria completa de un número (32 bits)
 * @param numero El número a mostrar en binario
 */
void mostrar_binario_completo(int numero) {
    printf("Binario (32 bits): ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (numero >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");  // Espacios cada 4 bits
    }
    printf("\n");
}

/**
 * Cuenta cuántos números pares e impares hay en un rango
 * @param inicio Inicio del rango (inclusivo)
 * @param fin Fin del rango (inclusivo)
 */
void contar_pares_impares_rango(int inicio, int fin) {
    int pares = 0, impares = 0;
    
    printf("\nAnálisis del rango [%d, %d]:\n", inicio, fin);
    printf("Número\tBinario\t\tLSB\tParidad\n");
    printf("------\t-------\t\t---\t-------\n");
    
    for (int i = inicio; i <= fin && i <= inicio + 10; i++) {  // Limitar a 10 para no saturar
        int lsb = i & 1;
        const char* paridad = es_par_bits(i) ? "PAR" : "IMPAR";
        
        printf("%d\t", i);
        for (int j = 7; j >= 0; j--) {
            printf("%d", (i >> j) & 1);
        }
        printf("\t%d\t%s\n", lsb, paridad);
        
        if (es_par_bits(i)) {
            pares++;
        } else {
            impares++;
        }
    }
    
    if (fin > inicio + 10) {
        // Contar el resto sin mostrar
        for (int i = inicio + 11; i <= fin; i++) {
            if (es_par_bits(i)) {
                pares++;
            } else {
                impares++;
            }
        }
        printf("... (mostrando solo primeros 10)\n");
    }
    
    printf("\nResumen:\n");
    printf("Números pares: %d\n", pares);
    printf("Números impares: %d\n", impares);
}

/**
 * Demuestra diferentes métodos para verificar paridad
 * @param numero El número a analizar
 */
void analizar_paridad_completo(int numero) {
    printf("\n=== Análisis completo de paridad para %d ===\n", numero);
    
    // Método tradicional (módulo)
    int par_modulo = (numero % 2) == 0;
    
    // Método con bits
    int par_bits = es_par_bits(numero);
    
    // Mostrar representación binaria
    mostrar_binario_8bits(numero);
    mostrar_binario_completo(numero);
    
    printf("\nMétodos de verificación:\n");
    printf("Módulo (%% 2): %s\n", par_modulo ? "PAR" : "IMPAR");
    printf("Bits (& 1): %s\n", par_bits ? "PAR" : "IMPAR");
    printf("LSB (bit menos significativo): %d\n", numero & 1);
    
    printf("\nExplicación:\n");
    printf("- El bit menos significativo (LSB) determina la paridad\n");
    printf("- LSB = 0 → número par\n");
    printf("- LSB = 1 → número impar\n");
    printf("- Operación: %d & 1 = %d\n", numero, numero & 1);
}

/**
 * Demuestra operaciones con números negativos
 * @param numero El número a analizar (puede ser negativo)
 */
void analizar_numeros_negativos(int numero) {
    printf("\n=== Análisis de número negativo: %d ===\n", numero);
    
    mostrar_binario_completo(numero);
    
    printf("Paridad usando bits: %s\n", es_par_bits(numero) ? "PAR" : "IMPAR");
    printf("LSB: %d\n", numero & 1);
    
    printf("\nNota: En complemento a dos, el LSB sigue determinando la paridad\n");
}

/**
 * Función principal que demuestra las operaciones de paridad con bits
 */
int main(void) {
    printf("=== Ejercicio 026: Paridad con Bits ===\n\n");
    
    // Casos de demostración básicos
    int numeros_demo[] = {0, 1, 2, 3, 4, 5, 15, 16, 255, 256};
    int num_demos = sizeof(numeros_demo) / sizeof(numeros_demo[0]);
    
    printf("Demostración básica:\n");
    printf("Número\tBinario\t\tLSB\tParidad\n");
    printf("------\t-------\t\t---\t-------\n");
    
    for (int i = 0; i < num_demos; i++) {
        int num = numeros_demo[i];
        int lsb = num & 1;
        const char* paridad = es_par_bits(num) ? "PAR" : "IMPAR";
        
        printf("%d\t", num);
        for (int j = 7; j >= 0; j--) {
            printf("%d", (num >> j) & 1);
        }
        printf("\t%d\t%s\n", lsb, paridad);
    }
    
    // Análisis de números negativos
    printf("\n=== Números Negativos ===\n");
    int negativos[] = {-1, -2, -3, -4, -15, -16};
    int num_negativos = sizeof(negativos) / sizeof(negativos[0]);
    
    for (int i = 0; i < num_negativos; i++) {
        int num = negativos[i];
        printf("%d es %s (LSB: %d)\n", 
               num, 
               es_par_bits(num) ? "PAR" : "IMPAR",
               num & 1);
    }
    
    // Comparación de rendimiento conceptual
    printf("\n=== Comparación de Métodos ===\n");
    printf("Método\t\tOperación\tComplejidad\tVelocidad\n");
    printf("------\t\t---------\t-----------\t---------\n");
    printf("Módulo\t\tn %% 2\t\tDivisión\tMás lenta\n");
    printf("Bits\t\tn & 1\t\tAND\t\tMás rápida\n");
    
    // Demostración con potencias de 2
    printf("\n=== Potencias de 2 (todas pares excepto 2^0) ===\n");
    for (int i = 0; i <= 10; i++) {
        int potencia = 1 << i;  // 2^i usando shift
        printf("2^%d = %d\t", i, potencia);
        mostrar_binario_8bits(potencia);
    }
    
    // Contar pares e impares en un rango
    contar_pares_impares_rango(0, 20);
    
    // Entrada del usuario
    printf("\n=== Prueba Interactiva ===\n");
    int numero;
    printf("Introduce un número entero: ");
    scanf("%d", &numero);
    
    if (es_par_bits(numero)) {
        printf("%d es par.\n", numero);
    } else {
        printf("%d es impar.\n", numero);
    }
    
    // Análisis detallado del número introducido
    analizar_paridad_completo(numero);
    
    return 0;
}
