/**
 * @file main.c
 * @brief Programa principal que demuestra el uso de la biblioteca mimath
 * @author Ejercicios C
 * @date 2025
 */

#include <stdio.h>
#include <stdbool.h>
#include "mimath.h"

/* ====================================================================
 * FUNCIÓN PARA MOSTRAR SEPARADORES
 * ==================================================================== */

void mostrar_separador(const char *titulo) {
    printf("\n=== %s ===\n", titulo);
}

void mostrar_subseparador(const char *titulo) {
    printf("\n--- %s ---\n", titulo);
}

/* ====================================================================
 * DEMOSTRACIONES DE FUNCIONALIDAD
 * ==================================================================== */

void demostrar_operaciones_basicas(void) {
    mostrar_separador("OPERACIONES BÁSICAS");
    
    int a = 15, b = 4;
    
    printf("Números: a = %d, b = %d\n", a, b);
    printf("Suma: %d + %d = %d\n", a, b, suma(a, b));
    printf("Resta: %d - %d = %d\n", a, b, resta(a, b));
    printf("Multiplicación: %d * %d = %d\n", a, b, multiplicacion(a, b));
    printf("División: %d / %d = %d\n", a, b, division(a, b));
    printf("Módulo: %d %% %d = %d\n", a, b, modulo(a, b));
}

void demostrar_operaciones_avanzadas(void) {
    mostrar_separador("OPERACIONES AVANZADAS");
    
    int base = 3, exponente = 4;
    int numero = 25;
    int fact_num = 6;
    
    printf("Potencia: %d^%d = %d\n", base, exponente, potencia(base, exponente));
    printf("Raíz cuadrada de %d = %d\n", numero, raiz_cuadrada(numero));
    printf("Factorial de %d = %lld\n", fact_num, factorial(fact_num));
    
    int x = 48, y = 18;
    printf("MCD de %d y %d = %d\n", x, y, mcd(x, y));
    printf("MCM de %d y %d = %d\n", x, y, mcm(x, y));
}

void demostrar_validaciones(void) {
    mostrar_separador("FUNCIONES DE VALIDACIÓN");
    
    int numeros[] = {2, 3, 4, 5, 6, 7, 28};
    int cantidad = sizeof(numeros) / sizeof(numeros[0]);
    
    for (int i = 0; i < cantidad; i++) {
        int num = numeros[i];
        printf("Número %d:\n", num);
        printf("  ¿Es primo? %s\n", es_primo(num) ? "Sí" : "No");
        printf("  ¿Es par? %s\n", es_par(num) ? "Sí" : "No");
        printf("  ¿Es perfecto? %s\n", es_perfecto(num) ? "Sí" : "No");
        printf("\n");
    }
}

void demostrar_utilidades(void) {
    mostrar_separador("FUNCIONES DE UTILIDAD");
    
    int p = -10, q = 25;
    
    printf("Valor absoluto de %d = %d\n", p, valor_absoluto(p));
    printf("Máximo entre %d y %d = %d\n", p, q, maximo(p, q));
    printf("Mínimo entre %d y %d = %d\n", p, q, minimo(p, q));
    
    mostrar_subseparador("Intercambio de variables");
    printf("Antes: p = %d, q = %d\n", p, q);
    intercambiar(&p, &q);
    printf("Después: p = %d, q = %d\n", p, q);
}

void demostrar_estadisticas_arrays(void) {
    mostrar_separador("ESTADÍSTICAS Y ARRAYS");
    
    int datos[] = {10, 25, 5, 30, 15, 20, 8};
    int tamaño = sizeof(datos) / sizeof(datos[0]);
    
    printf("Array: ");
    for (int i = 0; i < tamaño; i++) {
        printf("%d ", datos[i]);
    }
    printf("\n");
    
    printf("Suma: %d\n", suma_array(datos, tamaño));
    printf("Promedio: %.2f\n", promedio_array(datos, tamaño));
    printf("Máximo: %d\n", maximo_array(datos, tamaño));
    printf("Mínimo: %d\n", minimo_array(datos, tamaño));
}

void demostrar_constantes(void) {
    mostrar_separador("CONSTANTES MATEMÁTICAS");
    
    printf("PI = %.10f\n", PI);
    printf("E = %.10f\n", E);
    printf("Proporción áurea = %.10f\n", GOLDEN_RATIO);
}

void demostrar_casos_especiales(void) {
    mostrar_separador("CASOS ESPECIALES Y MANEJO DE ERRORES");
    
    printf("División por cero: 10 / 0 = %d\n", division(10, 0));
    printf("Módulo por cero: 10 %% 0 = %d\n", modulo(10, 0));
    printf("Potencia con exponente negativo: 2^(-3) = %d\n", potencia(2, -3));
    printf("Raíz cuadrada de número negativo: sqrt(-5) = %d\n", raiz_cuadrada(-5));
    printf("Factorial de número negativo: (-3)! = %lld\n", factorial(-3));
    
    // Demostrar manejo seguro de punteros nulos
    printf("\nIntercambio con punteros nulos (no debe causar crash):\n");
    intercambiar(NULL, NULL);
    printf("Operación completada sin errores.\n");
}

/* ====================================================================
 * FUNCIÓN PRINCIPAL
 * ==================================================================== */

#ifndef UNIT_TESTING
int main(void) {
    printf("🧮 BIBLIOTECA MATEMÁTICA MIMATH\n");
    printf("Demostración de funcionalidades\n");
    printf("================================\n");
    
    // Ejecutar todas las demostraciones
    demostrar_operaciones_basicas();
    demostrar_operaciones_avanzadas();
    demostrar_validaciones();
    demostrar_utilidades();
    demostrar_estadisticas_arrays();
    demostrar_constantes();
    demostrar_casos_especiales();
    
    mostrar_separador("COMPILACIÓN MODULAR EXITOSA");
    printf("✅ Biblioteca mimath.c compilada como módulo separado\n");
    printf("✅ Header mimath.h protegido con include guards\n");
    printf("✅ Enlazado exitoso de objetos separados\n");
    printf("✅ Demostración completa de modularización en C\n");
    
    return 0;
}
#endif
