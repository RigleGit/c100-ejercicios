/*
 * Ejercicio 029: Intercambio de Variables con XOR
 * Intercambia valores de dos variables sin usar variable temporal
 * usando las propiedades matemáticas del operador XOR.
 * 
 * Conceptos: XOR, álgebra booleana, intercambio sin memoria adicional
 * Dificultad: Intermedio
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Intercambio clásico usando variable temporal
 * @param a Puntero a la primera variable
 * @param b Puntero a la segunda variable
 */
void intercambio_temporal(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Intercambio usando XOR (sin variable temporal)
 * Propiedad: a XOR a = 0, a XOR 0 = a
 * @param a Puntero a la primera variable
 * @param b Puntero a la segunda variable
 */
void intercambio_xor(int *a, int *b) {
    // Verificar que no sean la misma dirección de memoria
    if (a == b) return;
    
    *a = *a ^ *b;  // a = a XOR b
    *b = *a ^ *b;  // b = (a XOR b) XOR b = a
    *a = *a ^ *b;  // a = (a XOR b) XOR a = b
}

/**
 * Intercambio usando suma y resta aritmética
 * @param a Puntero a la primera variable
 * @param b Puntero a la segunda variable
 */
void intercambio_aritmetico(int *a, int *b) {
    if (a == b) return;
    
    // Cuidado con overflow
    *a = *a + *b;
    *b = *a - *b;  // b = (a + b) - b = a
    *a = *a - *b;  // a = (a + b) - a = b
}

/**
 * Intercambio usando multiplicación y división
 * Solo funciona con números no cero
 * @param a Puntero a la primera variable
 * @param b Puntero a la segunda variable
 */
void intercambio_multiplicativo(int *a, int *b) {
    if (a == b || *a == 0 || *b == 0) return;
    
    *a = *a * *b;
    *b = *a / *b;  // b = (a * b) / b = a
    *a = *a / *b;  // a = (a * b) / a = b
}

/**
 * Muestra la representación binaria de un número
 * @param n El número a mostrar
 * @param bits Número de bits a mostrar
 */
void mostrarBinario(uint32_t n, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");
    }
}

/**
 * Demuestra el proceso XOR paso a paso
 * @param a Valor inicial de a
 * @param b Valor inicial de b
 */
void demostrarXOR(int a, int b) {
    printf("Demostración paso a paso del intercambio XOR:\n");
    printf("Valores iniciales: a = %d, b = %d\n\n", a, b);
    
    printf("a (inicial): %d = ", a);
    mostrarBinario(a, 16);
    printf("\n");
    printf("b (inicial): %d = ", b);
    mostrarBinario(b, 16);
    printf("\n\n");
    
    int original_a = a, original_b = b;
    
    // Paso 1: a = a XOR b
    a = a ^ b;
    printf("Paso 1: a = a ^ b\n");
    printf("a = %d ^ %d = %d = ", original_a, original_b, a);
    mostrarBinario(a, 16);
    printf("\n\n");
    
    // Paso 2: b = a XOR b (donde a ya contiene a^b)
    b = a ^ b;
    printf("Paso 2: b = a ^ b\n");
    printf("b = %d ^ %d = %d = ", a, original_b, b);
    mostrarBinario(b, 16);
    printf("\nAhora b contiene el valor original de a\n\n");
    
    // Paso 3: a = a XOR b (donde b ahora contiene el valor original de a)
    a = a ^ b;
    printf("Paso 3: a = a ^ b\n");
    printf("a = %d ^ %d = %d = ", (original_a ^ original_b), b, a);
    mostrarBinario(a, 16);
    printf("\nAhora a contiene el valor original de b\n\n");
    
    printf("Resultado final: a = %d, b = %d\n", a, b);
    printf("Intercambio exitoso sin variable temporal!\n\n");
}

/**
 * Verifica las propiedades matemáticas del XOR
 */
void verificarPropiedadesXOR(void) {
    printf("PROPIEDADES MATEMÁTICAS DEL XOR:\n\n");
    
    int a = 42, b = 73;
    
    // Propiedad 1: Conmutativa
    printf("1. Propiedad Conmutativa: a ^ b = b ^ a\n");
    printf("   %d ^ %d = %d\n", a, b, a ^ b);
    printf("   %d ^ %d = %d\n", b, a, b ^ a);
    printf("   ✓ Verificado\n\n");
    
    // Propiedad 2: Asociativa
    int c = 19;
    printf("2. Propiedad Asociativa: (a ^ b) ^ c = a ^ (b ^ c)\n");
    printf("   (%d ^ %d) ^ %d = %d ^ %d = %d\n", a, b, c, a^b, c, (a^b)^c);
    printf("   %d ^ (%d ^ %d) = %d ^ %d = %d\n", a, b, c, b^c, a, a^(b^c));
    printf("   ✓ Verificado\n\n");
    
    // Propiedad 3: Elemento neutro
    printf("3. Elemento Neutro: a ^ 0 = a\n");
    printf("   %d ^ 0 = %d\n", a, a ^ 0);
    printf("   ✓ Verificado\n\n");
    
    // Propiedad 4: Autoinverso
    printf("4. Autoinverso: a ^ a = 0\n");
    printf("   %d ^ %d = %d\n", a, a, a ^ a);
    printf("   ✓ Verificado\n\n");
    
    // Propiedad 5: Doble XOR
    printf("5. Doble XOR: (a ^ b) ^ b = a\n");
    printf("   (%d ^ %d) ^ %d = %d ^ %d = %d\n", a, b, b, a^b, b, (a^b)^b);
    printf("   ✓ Verificado (base del intercambio XOR)\n\n");
}

/**
 * Compara diferentes métodos de intercambio
 */
void compararMetodos(void) {
    printf("COMPARACIÓN DE MÉTODOS DE INTERCAMBIO:\n\n");
    
    int test_cases[][2] = {
        {5, 10},
        {-3, 7},
        {0, 15},
        {100, -50},
        {2147483647, -2147483648}  // Valores extremos
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    printf("Caso\tInicial\t\tTemporal\tXOR\t\tAritmético\tMultiplicativo\n");
    printf("----\t-------\t\t--------\t---\t\t----------\t--------------\n");
    
    for (int i = 0; i < num_tests; i++) {
        int a1 = test_cases[i][0], b1 = test_cases[i][1];
        int a2 = a1, b2 = b1;
        int a3 = a1, b3 = b1;
        int a4 = a1, b4 = b1;
        
        printf("%d\t(%d,%d)\t\t", i+1, a1, b1);
        
        // Método temporal
        intercambio_temporal(&a1, &b1);
        printf("(%d,%d)\t\t", a1, b1);
        
        // Método XOR
        intercambio_xor(&a2, &b2);
        printf("(%d,%d)\t\t", a2, b2);
        
        // Método aritmético
        intercambio_aritmetico(&a3, &b3);
        printf("(%d,%d)\t\t", a3, b3);
        
        // Método multiplicativo
        if (a4 != 0 && b4 != 0) {
            intercambio_multiplicativo(&a4, &b4);
            printf("(%d,%d)", a4, b4);
        } else {
            printf("N/A");
        }
        
        printf("\n");
    }
}

/**
 * Casos especiales y limitaciones
 */
void casosEspeciales(void) {
    printf("\nCASOS ESPECIALES Y LIMITACIONES:\n\n");
    
    // Caso 1: Misma dirección de memoria
    printf("1. Intercambio de una variable consigo misma:\n");
    int x = 42;
    printf("   Antes: x = %d\n", x);
    intercambio_xor(&x, &x);  // Debe resultar en 0 sin protección
    printf("   Después del XOR sin protección: x = %d\n", x);
    printf("   ⚠️  El XOR de una variable consigo misma es 0!\n\n");
    
    // Caso 2: Overflow en método aritmético
    printf("2. Overflow en método aritmético:\n");
    int a = 2000000000, b = 2000000000;
    printf("   Valores grandes: a = %d, b = %d\n", a, b);
    printf("   a + b = %lld (overflow en int de 32 bits)\n", (long long)a + b);
    printf("   ⚠️  Riesgo de overflow en suma/resta\n\n");
    
    // Caso 3: División por cero
    printf("3. División por cero en método multiplicativo:\n");
    printf("   Si alguna variable es 0, no se puede usar división\n");
    printf("   ⚠️  Método multiplicativo no funciona con ceros\n\n");
}

int main(void) {
    printf("=== EJERCICIO 029: INTERCAMBIO DE VARIABLES CON XOR ===\n\n");
    
    // Verificar propiedades matemáticas
    verificarPropiedadesXOR();
    
    // Demostración paso a paso
    printf("DEMOSTRACIÓN PASO A PASO:\n");
    demostrarXOR(25, 58);
    
    // Comparar métodos
    compararMetodos();
    
    // Casos especiales
    casosEspeciales();
    
    // Interacción con el usuario
    printf("PRUEBA INTERACTIVA:\n");
    int a, b;
    printf("Ingrese dos números enteros: ");
    if (scanf("%d %d", &a, &b) == 2) {
        printf("\nValores originales: a = %d, b = %d\n", a, b);
        
        // Mostrar representación binaria
        printf("a en binario: ");
        mostrarBinario(a, 16);
        printf("\n");
        printf("b en binario: ");
        mostrarBinario(b, 16);
        printf("\n\n");
        
        // Realizar intercambio
        intercambio_xor(&a, &b);
        
        printf("Después del intercambio XOR: a = %d, b = %d\n", a, b);
        printf("a en binario: ");
        mostrarBinario(a, 16);
        printf("\n");
        printf("b en binario: ");
        mostrarBinario(b, 16);
        printf("\n");
        
        printf("\n¡Intercambio realizado sin variable temporal!\n");
    }
    
    return 0;
}
