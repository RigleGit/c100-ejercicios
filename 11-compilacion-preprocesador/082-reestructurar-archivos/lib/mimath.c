/**
 * @file mimath.c
 * @brief Implementación de la biblioteca matemática simple
 * @author Ejercicios C
 * @date 2025
 */

#include "mimath.h"
#include <stdbool.h>
#include <stddef.h>

/* ====================================================================
 * OPERACIONES BÁSICAS
 * ==================================================================== */

int suma(int a, int b) {
    return a + b;
}

int resta(int a, int b) {
    return a - b;
}

int multiplicacion(int a, int b) {
    return a * b;
}

int division(int a, int b) {
    if (b == 0) {
        return 0; // Evitar división por cero
    }
    return a / b;
}

int modulo(int a, int b) {
    if (b == 0) {
        return 0; // Evitar módulo por cero
    }
    return a % b;
}

/* ====================================================================
 * OPERACIONES AVANZADAS
 * ==================================================================== */

int potencia(int base, int exponente) {
    if (exponente < 0) {
        return 0; // No manejamos exponentes negativos para enteros
    }
    
    int resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    return resultado;
}

int raiz_cuadrada(int numero) {
    if (numero < 0) {
        return -1; // No hay raíz cuadrada real de números negativos
    }
    
    if (numero == 0 || numero == 1) {
        return numero;
    }
    
    // Método de Newton-Raphson simplificado para enteros
    int raiz = numero / 2;
    int temp = 0;
    
    while (raiz != temp) {
        temp = raiz;
        raiz = (numero / raiz + raiz) / 2;
    }
    
    return raiz;
}

long long factorial(int n) {
    if (n < 0) {
        return -1; // Error: factorial no definido para negativos
    }
    
    if (n == 0 || n == 1) {
        return 1;
    }
    
    long long resultado = 1;
    for (int i = 2; i <= n; i++) {
        resultado *= i;
    }
    
    return resultado;
}

int mcd(int a, int b) {
    // Algoritmo de Euclides
    a = valor_absoluto(a);
    b = valor_absoluto(b);
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    
    return a;
}

int mcm(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    
    return valor_absoluto(a * b) / mcd(a, b);
}

/* ====================================================================
 * FUNCIONES DE VALIDACIÓN
 * ==================================================================== */

bool es_primo(int numero) {
    if (numero <= 1) {
        return false;
    }
    
    if (numero <= 3) {
        return true;
    }
    
    if (numero % 2 == 0 || numero % 3 == 0) {
        return false;
    }
    
    // Verificar divisores de la forma 6k ± 1
    for (int i = 5; i * i <= numero; i += 6) {
        if (numero % i == 0 || numero % (i + 2) == 0) {
            return false;
        }
    }
    
    return true;
}

bool es_par(int numero) {
    return numero % 2 == 0;
}

bool es_perfecto(int numero) {
    if (numero <= 1) {
        return false;
    }
    
    int suma_divisores = 1; // 1 siempre es divisor
    
    for (int i = 2; i * i <= numero; i++) {
        if (numero % i == 0) {
            suma_divisores += i;
            
            // Si i no es la raíz cuadrada, también agregar numero/i
            if (i != numero / i) {
                suma_divisores += numero / i;
            }
        }
    }
    
    return suma_divisores == numero;
}

/* ====================================================================
 * FUNCIONES DE UTILIDAD
 * ==================================================================== */

int valor_absoluto(int numero) {
    return numero < 0 ? -numero : numero;
}

int maximo(int a, int b) {
    return a > b ? a : b;
}

int minimo(int a, int b) {
    return a < b ? a : b;
}

void intercambiar(int *a, int *b) {
    if (a != NULL && b != NULL && a != b) {
        int temp = *a;
        *a = *b;
        *b = temp;
    }
}

/* ====================================================================
 * ESTADÍSTICAS Y ARRAYS
 * ==================================================================== */

int suma_array(const int arr[], int tamaño) {
    if (arr == NULL || tamaño <= 0) {
        return 0;
    }
    
    int suma_total = 0;
    for (int i = 0; i < tamaño; i++) {
        suma_total += arr[i];
    }
    
    return suma_total;
}

double promedio_array(const int arr[], int tamaño) {
    if (arr == NULL || tamaño <= 0) {
        return 0.0;
    }
    
    return (double)suma_array(arr, tamaño) / tamaño;
}

int maximo_array(const int arr[], int tamaño) {
    if (arr == NULL || tamaño <= 0) {
        return 0;
    }
    
    int max = arr[0];
    for (int i = 1; i < tamaño; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    
    return max;
}

int minimo_array(const int arr[], int tamaño) {
    if (arr == NULL || tamaño <= 0) {
        return 0;
    }
    
    int min = arr[0];
    for (int i = 1; i < tamaño; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    
    return min;
}
