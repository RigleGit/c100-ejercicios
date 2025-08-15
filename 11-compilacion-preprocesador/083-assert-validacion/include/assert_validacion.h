/**
 * @file assert_validacion.h
 * @brief Biblioteca para demostrar el uso de assert en validación de precondiciones
 * @author Ejercicios C
 * @date 2025
 */

#ifndef ASSERT_VALIDACION_H
#define ASSERT_VALIDACION_H

#include <stdbool.h>

/* ====================================================================
 * CONFIGURACIÓN DE ASSERT
 * ==================================================================== */

/*
 * NOTA IMPORTANTE: 
 * - En builds Debug, assert está activo por defecto
 * - En builds Release (-DNDEBUG), assert se desactiva
 * - Para desactivar manualmente: #define NDEBUG antes de #include <assert.h>
 */

/* ====================================================================
 * FUNCIONES MATEMÁTICAS CON VALIDACIÓN
 * ==================================================================== */

/**
 * @brief Calcula la raíz cuadrada de un número positivo
 * @param x Número del cual calcular la raíz (debe ser >= 0)
 * @return La raíz cuadrada de x
 * @pre x >= 0 (verificado con assert)
 */
double raiz_cuadrada(double x);

/**
 * @brief Calcula la raíz cuadrada con validación manual (sin assert)
 * @param x Número del cual calcular la raíz
 * @param resultado Puntero donde almacenar el resultado
 * @return true si la operación fue exitosa, false si x < 0
 */
bool raiz_cuadrada_segura(double x, double *resultado);

/**
 * @brief Calcula el logaritmo natural de un número positivo
 * @param x Número del cual calcular el logaritmo (debe ser > 0)
 * @return El logaritmo natural de x
 * @pre x > 0 (verificado con assert)
 */
double logaritmo_natural(double x);

/**
 * @brief Calcula la división de dos números
 * @param dividendo El número a dividir
 * @param divisor El número divisor (debe ser != 0)
 * @return El resultado de dividendo / divisor
 * @pre divisor != 0 (verificado con assert)
 */
double division_segura(double dividendo, double divisor);

/**
 * @brief Calcula la potencia de un número
 * @param base Base de la potencia
 * @param exponente Exponente (debe ser >= 0 para bases negativas)
 * @return base^exponente
 * @pre Si base < 0, entonces exponente debe ser entero
 */
double potencia_validada(double base, double exponente);

/* ====================================================================
 * FUNCIONES PARA ARRAYS CON VALIDACIÓN
 * ==================================================================== */

/**
 * @brief Calcula el promedio de un array
 * @param array Puntero al array (no debe ser NULL)
 * @param tamaño Tamaño del array (debe ser > 0)
 * @return El promedio de los elementos
 * @pre array != NULL && tamaño > 0
 */
double promedio_array(const double array[], int tamaño);

/**
 * @brief Busca un elemento en un array
 * @param array Puntero al array (no debe ser NULL)
 * @param tamaño Tamaño del array (debe ser > 0)
 * @param elemento Elemento a buscar
 * @return Índice del elemento o -1 si no se encuentra
 * @pre array != NULL && tamaño > 0
 */
int buscar_elemento(const double array[], int tamaño, double elemento);

/**
 * @brief Copia elementos de un array a otro
 * @param origen Array origen (no debe ser NULL)
 * @param destino Array destino (no debe ser NULL)
 * @param tamaño Número de elementos a copiar (debe ser > 0)
 * @pre origen != NULL && destino != NULL && tamaño > 0
 * @pre origen != destino (no deben ser el mismo array)
 */
void copiar_array(const double origen[], double destino[], int tamaño);

/* ====================================================================
 * FUNCIONES DE CADENAS CON VALIDACIÓN
 * ==================================================================== */

/**
 * @brief Calcula la longitud de una cadena
 * @param cadena Puntero a la cadena (no debe ser NULL)
 * @return Longitud de la cadena
 * @pre cadena != NULL
 */
int longitud_cadena(const char *cadena);

/**
 * @brief Copia una cadena a otra
 * @param origen Cadena origen (no debe ser NULL)
 * @param destino Cadena destino (no debe ser NULL)
 * @pre origen != NULL && destino != NULL
 * @pre destino debe tener suficiente espacio
 */
void copiar_cadena(const char *origen, char *destino);

/* ====================================================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ==================================================================== */

/**
 * @brief Demuestra el uso de assert con valores válidos
 */
void demostrar_casos_validos(void);

/**
 * @brief Demuestra el comportamiento de assert con valores inválidos
 * @warning Esta función causará terminación del programa si assert está activo
 */
void demostrar_casos_invalidos(void);

/**
 * @brief Demuestra alternativas sin assert para manejo de errores
 */
void demostrar_alternativas_seguras(void);

/**
 * @brief Muestra el estado de NDEBUG y si assert está activo
 */
void mostrar_estado_assert(void);

#endif /* ASSERT_VALIDACION_H */
