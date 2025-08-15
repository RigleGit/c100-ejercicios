/*
 * Ejercicio 024: Función MCD (Máximo Común Divisor)
 * Implementa una función que calcule el máximo común divisor de dos números
 * usando el algoritmo de Euclides (recursivo).
 * 
 * Conceptos: Funciones, recursividad, algoritmo de Euclides, matemáticas
 * Dificultad: Intermedio
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Calcula el máximo común divisor de dos números usando el algoritmo de Euclides
 * @param a Primer número (se trabaja con valor absoluto)
 * @param b Segundo número (se trabaja con valor absoluto)
 * @return El MCD de a y b
 */
int mcd(int a, int b) {
    // Trabajamos con valores absolutos para manejar números negativos
    a = abs(a);
    b = abs(b);
    
    // Caso base: si b es 0, el MCD es a
    if (b == 0) {
        return a;
    }
    
    // Caso recursivo: MCD(a, b) = MCD(b, a mod b)
    return mcd(b, a % b);
}

/**
 * Implementación iterativa del MCD para comparación
 * @param a Primer número
 * @param b Segundo número
 * @return El MCD de a y b
 */
int mcd_iterativo(int a, int b) {
    a = abs(a);
    b = abs(b);
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    
    return a;
}

/**
 * Calcula el mínimo común múltiplo usando la fórmula: MCM(a,b) = |a*b| / MCD(a,b)
 * @param a Primer número
 * @param b Segundo número
 * @return El MCM de a y b
 */
int mcm(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    
    return abs(a * b) / mcd(a, b);
}

/**
 * Verifica si dos números son coprimos (MCD = 1)
 * @param a Primer número
 * @param b Segundo número
 * @return 1 si son coprimos, 0 en caso contrario
 */
int son_coprimos(int a, int b) {
    return mcd(a, b) == 1;
}

/**
 * Calcula el MCD extendido usando el algoritmo de Euclides extendido
 * Encuentra enteros x, y tales que ax + by = mcd(a, b)
 * @param a Primer número
 * @param b Segundo número
 * @param x Puntero para almacenar el coeficiente x
 * @param y Puntero para almacenar el coeficiente y
 * @return El MCD de a y b
 */
int mcd_extendido(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    
    int x1, y1;
    int gcd = mcd_extendido(b, a % b, &x1, &y1);
    
    *x = y1;
    *y = x1 - (a / b) * y1;
    
    return gcd;
}

/**
 * Función principal que demuestra el uso de las funciones MCD
 */
int main(void) {
    printf("=== Ejercicio 024: Función MCD ===\n\n");
    
    // Casos de demostración
    int pares[][2] = {
        {48, 18},
        {56, 42},
        {17, 13},
        {100, 75},
        {24, -18},
        {0, 5},
        {13, 13}
    };
    
    int num_pares = sizeof(pares) / sizeof(pares[0]);
    
    printf("Demostraciones del MCD:\n");
    printf("%-8s %-8s %-12s %-12s %-8s %-10s\n", 
           "a", "b", "MCD(rec)", "MCD(iter)", "MCM", "Coprimos");
    printf("%-8s %-8s %-12s %-12s %-8s %-10s\n", 
           "---", "---", "--------", "--------", "---", "--------");
    
    for (int i = 0; i < num_pares; i++) {
        int a = pares[i][0];
        int b = pares[i][1];
        int mcd_rec = mcd(a, b);
        int mcd_iter = mcd_iterativo(a, b);
        int mcm_val = mcm(a, b);
        const char* coprimos = son_coprimos(a, b) ? "Sí" : "No";
        
        printf("%-8d %-8d %-12d %-12d %-8d %-10s\n", 
               a, b, mcd_rec, mcd_iter, mcm_val, coprimos);
    }
    
    // Demostración del algoritmo extendido
    printf("\nEjemplo del algoritmo de Euclides extendido:\n");
    int a = 56, b = 42;
    int x, y;
    int gcd = mcd_extendido(a, b, &x, &y);
    
    printf("Para a = %d, b = %d:\n", a, b);
    printf("MCD(%d, %d) = %d\n", a, b, gcd);
    printf("Coeficientes: x = %d, y = %d\n", x, y);
    printf("Verificación: %d × %d + %d × %d = %d\n", 
           a, x, b, y, a * x + b * y);
    
    // Demostración con secuencia de Fibonacci
    printf("\nMCD de números consecutivos de Fibonacci (caso peor):\n");
    int fib[10] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
    
    for (int i = 1; i < 9; i++) {
        int gcd_fib = mcd(fib[i+1], fib[i]);
        printf("MCD(%d, %d) = %d\n", fib[i+1], fib[i], gcd_fib);
    }
    
    return 0;
}
