/*
 * Ejercicio 028: Potencia de 2 con Bits
 * Determina si un número es potencia de 2 usando operaciones bit a bit.
 * Demuestra el elegante truco: n & (n-1) == 0
 * 
 * Conceptos: Potencias de 2, operaciones AND, análisis binario, optimización
 * Dificultad: Intermedio
 */

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

/**
 * Verifica si un número es potencia de 2 usando el método clásico de bits
 * @param n El número a verificar
 * @return true si es potencia de 2, false en caso contrario
 */
bool esPotenciaDe2(unsigned int n) {
    return n != 0 && (n & (n - 1)) == 0;
}

/**
 * Método alternativo: contar bits establecidos (debe ser exactamente 1)
 * @param n El número a verificar
 * @return true si es potencia de 2, false en caso contrario
 */
bool esPotenciaDe2_contandoBits(unsigned int n) {
    if (n == 0) return false;
    
    int bits = 0;
    while (n) {
        if (n & 1) bits++;
        if (bits > 1) return false;  // Optimización temprana
        n >>= 1;
    }
    return bits == 1;
}

/**
 * Método usando división iterativa (menos eficiente, pero educativo)
 * @param n El número a verificar
 * @return true si es potencia de 2, false en caso contrario
 */
bool esPotenciaDe2_division(unsigned int n) {
    if (n == 0) return false;
    
    while (n > 1) {
        if (n % 2 != 0) return false;  // Si no es divisible por 2
        n /= 2;
    }
    return true;
}

/**
 * Método usando logaritmos (requiere verificación adicional por precisión)
 * @param n El número a verificar
 * @return true si es potencia de 2, false en caso contrario
 */
bool esPotenciaDe2_logaritmo(unsigned int n) {
    if (n == 0) return false;
    
    // Verificar usando el truco de bits como referencia
    // En la práctica, los logaritmos tienen problemas de precisión
    return esPotenciaDe2(n);
}

/**
 * Encuentra la potencia de 2 más cercana (mayor o igual)
 * @param n El número base
 * @return La siguiente potencia de 2 >= n
 */
unsigned int siguientePotenciaDe2(unsigned int n) {
    if (n == 0) return 1;
    if (esPotenciaDe2(n)) return n;
    
    // Método usando operaciones de bits
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

/**
 * Encuentra la potencia de 2 más cercana (menor o igual)
 * @param n El número base
 * @return La potencia de 2 anterior <= n
 */
unsigned int anteriorPotenciaDe2(unsigned int n) {
    if (n == 0) return 0;
    
    // Encontrar el bit más significativo
    unsigned int resultado = 1;
    while (resultado <= n / 2) {
        resultado <<= 1;
    }
    return resultado;
}

/**
 * Calcula qué potencia de 2 es un número (si lo es)
 * @param n El número a analizar
 * @return La potencia k tal que 2^k = n, o -1 si no es potencia de 2
 */
int quePotenciaDe2(unsigned int n) {
    if (!esPotenciaDe2(n)) return -1;
    
    int potencia = 0;
    while (n > 1) {
        n >>= 1;
        potencia++;
    }
    return potencia;
}

/**
 * Genera todas las potencias de 2 hasta un límite
 * @param limite El valor máximo
 */
void generarPotenciasDe2(unsigned int limite) {
    printf("\nPotencias de 2 hasta %u:\n", limite);
    printf("Potencia\tValor\t\tBinario\t\tn & (n-1)\n");
    printf("--------\t-----\t\t-------\t\t---------\n");
    
    for (int i = 0; i < 32; i++) {
        unsigned int potencia = 1U << i;
        if (potencia > limite) break;
        
        unsigned int operacion = potencia & (potencia - 1);
        
        printf("2^%d\t\t%u\t\t", i, potencia);
        
        // Mostrar binario (16 bits para claridad)
        for (int j = 15; j >= 0; j--) {
            printf("%d", (potencia >> j) & 1);
        }
        
        printf("\t%u\n", operacion);
    }
}

/**
 * Demuestra por qué funciona el truco n & (n-1) == 0
 * @param n El número a analizar
 */
void demostrarTruco(unsigned int n) {
    printf("\n=== Demostración del truco para %u ===\n", n);
    
    printf("n = %u\n", n);
    printf("n - 1 = %u\n", n - 1);
    
    // Mostrar representación binaria
    printf("\nRepresentación binaria:\n");
    printf("n     = ");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf(" (%u)\n", n);
    
    printf("n - 1 = ");
    for (int i = 15; i >= 0; i--) {
        printf("%d", ((n - 1) >> i) & 1);
    }
    printf(" (%u)\n", n - 1);
    
    unsigned int resultado = n & (n - 1);
    printf("n & (n-1) = ");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (resultado >> i) & 1);
    }
    printf(" (%u)\n", resultado);
    
    printf("\nExplicación:\n");
    if (esPotenciaDe2(n)) {
        printf("- %u es potencia de 2 (un solo bit en 1)\n", n);
        printf("- Al restar 1, todos los bits a la derecha se vuelven 1\n");
        printf("- El AND entre estos patrones da 0\n");
        printf("- Por tanto, %u & (%u - 1) = 0 ✓\n", n, n);
    } else {
        printf("- %u NO es potencia de 2 (múltiples bits en 1)\n", n);
        printf("- Al restar 1, algunos bits en 1 permanecen\n");
        printf("- El AND entre estos patrones NO da 0\n");
        printf("- Por tanto, %u & (%u - 1) = %u ≠ 0 ✗\n", n, n, resultado);
    }
}

/**
 * Analiza un rango de números para encontrar potencias de 2
 * @param inicio Inicio del rango
 * @param fin Fin del rango
 */
void analizarRango(unsigned int inicio, unsigned int fin) {
    printf("\n=== Análisis del rango [%u, %u] ===\n", inicio, fin);
    printf("Número\tPot2?\tBinario\t\tn&(n-1)\tMétodo bits\tMétodo div\n");
    printf("------\t-----\t-------\t\t-------\t-----------\t----------\n");
    
    for (unsigned int i = inicio; i <= fin && i <= inicio + 20; i++) {
        bool es_pot2_bits = esPotenciaDe2(i);
        bool es_pot2_conteo = esPotenciaDe2_contandoBits(i);
        bool es_pot2_div = esPotenciaDe2_division(i);
        unsigned int operacion = i & (i - 1);
        
        printf("%u\t%s\t", i, es_pot2_bits ? "SÍ" : "NO");
        
        // Binario (8 bits)
        for (int j = 7; j >= 0; j--) {
            printf("%d", (i >> j) & 1);
        }
        
        printf("\t%u\t%s\t\t%s\n", 
               operacion,
               es_pot2_conteo ? "SÍ" : "NO",
               es_pot2_div ? "SÍ" : "NO");
        
        // Verificar consistencia
        if (es_pot2_bits != es_pot2_conteo || es_pot2_bits != es_pot2_div) {
            printf("ERROR: Inconsistencia en métodos para %u\n", i);
        }
    }
    
    if (fin > inicio + 20) {
        printf("... (mostrando solo primeros 20)\n");
    }
}

/**
 * Demuestra aplicaciones prácticas de las potencias de 2
 */
void aplicacionesPracticas(void) {
    printf("\n=== Aplicaciones Prácticas ===\n");
    
    // Alineación de memoria
    printf("1. Alineación de memoria:\n");
    unsigned int tamanos[] = {7, 15, 33, 127, 200};
    for (int i = 0; i < 5; i++) {
        unsigned int tam = tamanos[i];
        unsigned int alineado = siguientePotenciaDe2(tam);
        printf("   %u bytes → alinear a %u bytes\n", tam, alineado);
    }
    
    // Tamaños de buffer
    printf("\n2. Tamaños de buffer óptimos:\n");
    printf("   Para 1000 elementos → buffer de %u\n", siguientePotenciaDe2(1000));
    printf("   Para 5000 elementos → buffer de %u\n", siguientePotenciaDe2(5000));
    
    // Máscaras de bits
    printf("\n3. Máscaras para operaciones modulares:\n");
    printf("   x %% 8 = x & 7   (8 = 2^3, máscara = 7)\n");
    printf("   x %% 16 = x & 15 (16 = 2^4, máscara = 15)\n");
    printf("   x %% 32 = x & 31 (32 = 2^5, máscara = 31)\n");
}

/**
 * Función principal que demuestra la verificación de potencias de 2
 */
int main(void) {
    printf("=== Ejercicio 028: Potencia de 2 con Bits ===\n\n");
    
    // Demostración básica
    unsigned int numeros_demo[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 31, 32, 63, 64, 100, 128, 255, 256, 512, 1000, 1024};
    int num_demos = sizeof(numeros_demo) / sizeof(numeros_demo[0]);
    
    printf("Demostración básica:\n");
    printf("Número\tPotencia de 2?\tn & (n-1)\tExponente\n");
    printf("------\t--------------\t---------\t---------\n");
    
    for (int i = 0; i < num_demos; i++) {
        unsigned int num = numeros_demo[i];
        bool es_pot2 = esPotenciaDe2(num);
        unsigned int operacion = num & (num - 1);
        int exponente = quePotenciaDe2(num);
        
        printf("%u\t%s\t\t%u\t\t", num, es_pot2 ? "SÍ" : "NO", operacion);
        if (exponente >= 0) {
            printf("2^%d", exponente);
        } else {
            printf("N/A");
        }
        printf("\n");
    }
    
    // Generar todas las potencias de 2
    generarPotenciasDe2(65536);
    
    // Comparación de métodos
    printf("\n=== Comparación de Métodos ===\n");
    printf("Método\t\tComplejidad\tDescripción\n");
    printf("------\t\t-----------\t-----------\n");
    printf("n&(n-1)==0\tO(1)\t\tTruco de bits elegante\n");
    printf("Contar bits\tO(log n)\tContar bits establecidos\n");
    printf("División\tO(log n)\tDivisiones sucesivas por 2\n");
    printf("Logaritmo\tO(1)\t\tProblemas de precisión\n");
    
    // Análisis de rango
    analizarRango(0, 40);
    
    // Aplicaciones prácticas
    aplicacionesPracticas();
    
    // Entrada del usuario
    printf("\n=== Prueba Interactiva ===\n");
    unsigned int numero;
    printf("Introduce un número entero positivo: ");
    scanf("%u", &numero);
    
    if (esPotenciaDe2(numero)) {
        printf("%u es una potencia de 2.\n", numero);
        int exp = quePotenciaDe2(numero);
        printf("Específicamente: 2^%d = %u\n", exp, numero);
    } else {
        printf("%u NO es una potencia de 2.\n", numero);
        
        unsigned int anterior = anteriorPotenciaDe2(numero);
        unsigned int siguiente = siguientePotenciaDe2(numero);
        
        printf("Potencia anterior: %u\n", anterior);
        printf("Potencia siguiente: %u\n", siguiente);
    }
    
    // Demostración detallada del truco
    demostrarTruco(numero);
    
    // Casos adicionales interesantes
    printf("\n=== Casos Especiales ===\n");
    printf("0: %s (por definición, 0 no es potencia de 2)\n", 
           esPotenciaDe2(0) ? "SÍ" : "NO");
    printf("1: %s (2^0 = 1)\n", 
           esPotenciaDe2(1) ? "SÍ" : "NO");
    printf("UINT_MAX: %s (todos los bits en 1)\n", 
           esPotenciaDe2(UINT_MAX) ? "SÍ" : "NO");
    printf("2^31: %s (máxima potencia de 2 en 32 bits)\n", 
           esPotenciaDe2(1U << 31) ? "SÍ" : "NO");
    
    return 0;
}
