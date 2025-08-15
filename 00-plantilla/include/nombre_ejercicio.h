/**
 * @file nombre_ejercicio.h
 * @brief [DESCRIPCIÓN BREVE DEL EJERCICIO]
 * 
 * [DESCRIPCIÓN DETALLADA DEL EJERCICIO]
 * 
 * Este ejercicio cubre:
 * - [CONCEPTO 1]
 * - [CONCEPTO 2] 
 * - [CONCEPTO 3]
 * 
 * @author C100 Ejercicios
 * @date [FECHA]
 * @version 1.0
 */

#ifndef NOMBRE_EJERCICIO_H
#define NOMBRE_EJERCICIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

// Incluir headers específicos según el ejercicio
// #include <time.h>      // Para ejercicios con tiempo
// #include <ctype.h>     // Para ejercicios con caracteres
// #include <stdint.h>    // Para tipos enteros específicos
// #include <stdbool.h>   // Para valores booleanos

/**
 * @brief Códigos de resultado para las operaciones
 */
typedef enum {
    RESULTADO_EXITO = 0,              /**< Operación exitosa */
    RESULTADO_ERROR_PARAMETRO = -1,   /**< Parámetros inválidos */
    RESULTADO_ERROR_MEMORIA = -2,     /**< Error de memoria */
    RESULTADO_ERROR_ENTRADA = -3,     /**< Error en entrada de datos */
    RESULTADO_ERROR_CALCULO = -4,     /**< Error en cálculo */
    RESULTADO_ERROR_DESBORDAMIENTO = -5 /**< Desbordamiento numérico */
} resultado_t;

/**
 * @brief Estructura principal del ejercicio (si aplica)
 * 
 * Ejemplo para ejercicios que requieren estructuras de datos
 */
typedef struct {
    int campo1;           /**< Descripción del campo 1 */
    double campo2;        /**< Descripción del campo 2 */
    char campo3[100];     /**< Descripción del campo 3 */
    // Agregar campos según necesidades del ejercicio
} estructura_principal_t;

/**
 * @brief Constantes del ejercicio
 */
#define MAX_BUFFER 256              /**< Tamaño máximo de buffer */
#define MAX_ELEMENTOS 100           /**< Número máximo de elementos */
#define PRECISION_DECIMAL 1e-6      /**< Precisión para comparaciones */

// Definir constantes específicas según el ejercicio
// #define PI 3.14159265359
// #define MAX_ITERACIONES 1000

/**
 * @brief Función principal del ejercicio
 * 
 * [DESCRIPCIÓN DE LA FUNCIÓN PRINCIPAL]
 * 
 * @param parametro1 [Descripción del parámetro 1]
 * @param parametro2 [Descripción del parámetro 2]
 * @param resultado [Descripción del parámetro de salida]
 * @return resultado_t Código de resultado de la operación
 * 
 * @note [NOTAS IMPORTANTES]
 * @warning [ADVERTENCIAS SI LAS HAY]
 * 
 * Ejemplo de uso:
 * @code
 * double resultado;
 * resultado_t status = funcion_principal(param1, param2, &resultado);
 * if (status == RESULTADO_EXITO) {
 *     printf("Resultado: %.2f\n", resultado);
 * }
 * @endcode
 */
resultado_t funcion_principal(int parametro1, double parametro2, double* resultado);

/**
 * @brief Función auxiliar 1
 * 
 * [DESCRIPCIÓN DE LA FUNCIÓN AUXILIAR]
 * 
 * @param entrada [Descripción del parámetro de entrada]
 * @return int [Descripción del valor de retorno]
 */
int funcion_auxiliar1(const char* entrada);

/**
 * @brief Función auxiliar 2
 * 
 * [DESCRIPCIÓN DE LA FUNCIÓN AUXILIAR]
 * 
 * @param array [Descripción del array]
 * @param tamaño [Descripción del tamaño]
 * @return double [Descripción del valor de retorno]
 */
double funcion_auxiliar2(const double* array, int tamaño);

/**
 * @brief Función de validación de entrada
 * 
 * Valida que los parámetros de entrada sean correctos
 * 
 * @param parametro [Descripción del parámetro a validar]
 * @return int 1 si es válido, 0 si no es válido
 */
int validar_entrada(const void* parametro);

/**
 * @brief Función de inicialización
 * 
 * Inicializa las estructuras necesarias para el ejercicio
 * 
 * @param estructura [Puntero a la estructura a inicializar]
 * @return resultado_t Código de resultado
 */
resultado_t inicializar_estructura(estructura_principal_t* estructura);

/**
 * @brief Función de liberación de memoria
 * 
 * Libera la memoria asociada con las estructuras del ejercicio
 * 
 * @param estructura [Puntero a la estructura a liberar]
 */
void liberar_estructura(estructura_principal_t* estructura);

/**
 * @brief Función de impresión de resultados
 * 
 * Imprime los resultados de manera formateada
 * 
 * @param resultado [Resultado a imprimir]
 * @param formato [Formato de salida]
 */
void imprimir_resultado(double resultado, const char* formato);

/**
 * @brief Función de manejo de errores
 * 
 * Convierte códigos de error en mensajes legibles
 * 
 * @param codigo [Código de error]
 * @return const char* Mensaje de error correspondiente
 */
const char* obtener_mensaje_error(resultado_t codigo);

// Declaraciones adicionales según el tipo de ejercicio:

// Para ejercicios con arrays/vectores:
/**
 * @brief Procesar array de números
 * @param array Array de entrada
 * @param tamaño Tamaño del array
 * @param resultado Puntero al resultado
 * @return resultado_t Código de resultado
 */
// resultado_t procesar_array(const double* array, int tamaño, double* resultado);

// Para ejercicios con cadenas:
/**
 * @brief Procesar cadena de caracteres
 * @param cadena Cadena de entrada
 * @param resultado Buffer para el resultado
 * @param tamaño_resultado Tamaño del buffer
 * @return resultado_t Código de resultado
 */
// resultado_t procesar_cadena(const char* cadena, char* resultado, int tamaño_resultado);

// Para ejercicios con estructuras de datos:
/**
 * @brief Crear nueva estructura de datos
 * @return estructura_principal_t* Puntero a la nueva estructura o NULL en caso de error
 */
// estructura_principal_t* crear_estructura(void);

// Para ejercicios con archivos:
/**
 * @brief Procesar archivo
 * @param nombre_archivo Nombre del archivo a procesar
 * @param resultado Puntero al resultado
 * @return resultado_t Código de resultado
 */
// resultado_t procesar_archivo(const char* nombre_archivo, double* resultado);

#endif /* NOMBRE_EJERCICIO_H */
