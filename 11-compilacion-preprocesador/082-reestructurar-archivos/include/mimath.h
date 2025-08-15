/**
 * @file mimath.h
 * @brief Biblioteca matemática simple con operaciones básicas
 * @author Ejercicios C
 * @date 2025
 */

#ifndef MIMATH_H
#define MIMATH_H

#include <stdbool.h>

/* ====================================================================
 * CONSTANTES MATEMÁTICAS
 * ==================================================================== */

#define PI 3.14159265359
#define E 2.71828182846
#define GOLDEN_RATIO 1.61803398875

/* ====================================================================
 * OPERACIONES BÁSICAS
 * ==================================================================== */

/**
 * @brief Suma dos números enteros
 * @param a Primer número
 * @param b Segundo número
 * @return La suma de a y b
 */
int suma(int a, int b);

/**
 * @brief Resta dos números enteros
 * @param a Minuendo
 * @param b Sustraendo
 * @return La diferencia a - b
 */
int resta(int a, int b);

/**
 * @brief Multiplica dos números enteros
 * @param a Primer factor
 * @param b Segundo factor
 * @return El producto de a y b
 */
int multiplicacion(int a, int b);

/**
 * @brief Divide dos números enteros
 * @param a Dividendo
 * @param b Divisor
 * @return El cociente de a / b, o 0 si b es 0
 */
int division(int a, int b);

/**
 * @brief Calcula el módulo de dos números
 * @param a Dividendo
 * @param b Divisor
 * @return El resto de a % b, o 0 si b es 0
 */
int modulo(int a, int b);

/* ====================================================================
 * OPERACIONES AVANZADAS
 * ==================================================================== */

/**
 * @brief Calcula la potencia de un número
 * @param base Base de la potencia
 * @param exponente Exponente (debe ser >= 0)
 * @return base^exponente
 */
int potencia(int base, int exponente);

/**
 * @brief Calcula la raíz cuadrada entera
 * @param numero Número del cual calcular la raíz
 * @return La raíz cuadrada entera (truncada)
 */
int raiz_cuadrada(int numero);

/**
 * @brief Calcula el factorial de un número
 * @param n Número del cual calcular el factorial (debe ser >= 0)
 * @return n! o -1 si hay error
 */
long long factorial(int n);

/**
 * @brief Calcula el máximo común divisor de dos números
 * @param a Primer número
 * @param b Segundo número
 * @return El MCD de a y b
 */
int mcd(int a, int b);

/**
 * @brief Calcula el mínimo común múltiplo de dos números
 * @param a Primer número
 * @param b Segundo número
 * @return El MCM de a y b
 */
int mcm(int a, int b);

/* ====================================================================
 * FUNCIONES DE VALIDACIÓN
 * ==================================================================== */

/**
 * @brief Verifica si un número es primo
 * @param numero Número a verificar
 * @return true si es primo, false en caso contrario
 */
bool es_primo(int numero);

/**
 * @brief Verifica si un número es par
 * @param numero Número a verificar
 * @return true si es par, false si es impar
 */
bool es_par(int numero);

/**
 * @brief Verifica si un número es perfecto
 * @param numero Número a verificar
 * @return true si es perfecto, false en caso contrario
 */
bool es_perfecto(int numero);

/* ====================================================================
 * FUNCIONES DE UTILIDAD
 * ==================================================================== */

/**
 * @brief Calcula el valor absoluto de un número
 * @param numero Número
 * @return Valor absoluto del número
 */
int valor_absoluto(int numero);

/**
 * @brief Encuentra el máximo de dos números
 * @param a Primer número
 * @param b Segundo número
 * @return El mayor de los dos números
 */
int maximo(int a, int b);

/**
 * @brief Encuentra el mínimo de dos números
 * @param a Primer número
 * @param b Segundo número
 * @return El menor de los dos números
 */
int minimo(int a, int b);

/**
 * @brief Intercambia dos números
 * @param a Puntero al primer número
 * @param b Puntero al segundo número
 */
void intercambiar(int *a, int *b);

/* ====================================================================
 * ESTADÍSTICAS Y ARRAYS
 * ==================================================================== */

/**
 * @brief Calcula la suma de un array
 * @param arr Array de números
 * @param tamaño Tamaño del array
 * @return La suma de todos los elementos
 */
int suma_array(const int arr[], int tamaño);

/**
 * @brief Calcula el promedio de un array
 * @param arr Array de números
 * @param tamaño Tamaño del array
 * @return El promedio de los elementos
 */
double promedio_array(const int arr[], int tamaño);

/**
 * @brief Encuentra el máximo en un array
 * @param arr Array de números
 * @param tamaño Tamaño del array
 * @return El valor máximo en el array
 */
int maximo_array(const int arr[], int tamaño);

/**
 * @brief Encuentra el mínimo en un array
 * @param arr Array de números
 * @param tamaño Tamaño del array
 * @return El valor mínimo en el array
 */
int minimo_array(const int arr[], int tamaño);

#endif /* MIMATH_H */
