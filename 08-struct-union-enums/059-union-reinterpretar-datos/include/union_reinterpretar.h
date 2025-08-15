#ifndef UNION_REINTERPRETAR_H
#define UNION_REINTERPRETAR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file union_reinterpretar.h
 * @brief Biblioteca para explorar uniones y reinterpretación de datos en C
 * 
 * Esta biblioteca demuestra el uso de uniones para reinterpretar datos
 * en memoria, explorando conceptos como endianness, representación binaria
 * y manipulación de datos a nivel de bytes.
 */

/**
 * @brief Unión principal del enunciado: entero de 32 bits y array de 4 bytes
 * 
 * Permite observar cómo se almacena un entero de 32 bits en memoria
 * accediendo a sus bytes individuales.
 */
union Datos {
    int32_t entero;     /**< Entero de 32 bits con signo */
    char bytes[4];      /**< Array de 4 bytes (chars) */
};

/**
 * @brief Unión extendida para diferentes tipos de datos
 * 
 * Demuestra cómo múltiples tipos pueden compartir la misma memoria.
 */
union DatosExtendidos {
    int32_t entero;         /**< Entero de 32 bits */
    uint32_t sin_signo;     /**< Entero sin signo de 32 bits */
    float flotante;         /**< Número de punto flotante */
    char bytes[4];          /**< Array de bytes */
    uint16_t words[2];      /**< Array de 2 words de 16 bits */
    uint8_t octetos[4];     /**< Array de 4 octetos sin signo */
};

/**
 * @brief Unión para explorar números de punto flotante
 */
union FloatBits {
    float valor;            /**< Valor de punto flotante */
    uint32_t bits;          /**< Representación en bits */
    struct {
        uint32_t mantissa : 23;  /**< Mantisa (23 bits) */
        uint32_t exponente : 8;  /**< Exponente (8 bits) */
        uint32_t signo : 1;      /**< Bit de signo (1 bit) */
    } ieee754;              /**< Estructura IEEE 754 */
};

/**
 * @brief Unión para trabajar con direcciones de memoria
 */
union Puntero {
    void *direccion;        /**< Puntero genérico */
    uintptr_t valor;        /**< Valor numérico de la dirección */
    uint8_t bytes[sizeof(void*)]; /**< Bytes de la dirección */
};

// Funciones principales del enunciado

/**
 * @brief Muestra la representación en bytes de un entero (función del enunciado)
 * 
 * @param valor Valor entero a analizar
 */
void mostrar_bytes_entero(int32_t valor);

/**
 * @brief Crea y analiza una unión Datos con un valor específico
 * 
 * @param valor Valor a asignar al entero
 * @return union Datos La unión creada
 */
union Datos crear_datos(int32_t valor);

/**
 * @brief Imprime información detallada de una unión Datos
 * 
 * @param datos Unión a analizar
 */
void imprimir_analisis_datos(union Datos datos);

// Funciones de análisis avanzado

/**
 * @brief Detecta el endianness del sistema
 * 
 * @return true si es little-endian, false si es big-endian
 */
bool es_little_endian(void);

/**
 * @brief Muestra información sobre el endianness del sistema
 */
void mostrar_endianness(void);

/**
 * @brief Analiza la representación de diferentes tipos de datos
 * 
 * @param datos Unión extendida a analizar
 */
void analizar_tipos_datos(union DatosExtendidos datos);

/**
 * @brief Convierte un entero entre little-endian y big-endian
 * 
 * @param valor Valor a convertir
 * @return int32_t Valor con bytes intercambiados
 */
int32_t intercambiar_endianness(int32_t valor);

/**
 * @brief Muestra la representación binaria de un entero
 * 
 * @param valor Valor a mostrar en binario
 */
void mostrar_representacion_binaria(int32_t valor);

/**
 * @brief Analiza un número de punto flotante bit por bit
 * 
 * @param valor Valor flotante a analizar
 */
void analizar_float_ieee754(float valor);

/**
 * @brief Demuestra operaciones de manipulación de bits usando uniones
 * 
 * @param valor Valor inicial
 */
void demo_manipulacion_bits(int32_t valor);

/**
 * @brief Compara representaciones de enteros con y sin signo
 * 
 * @param valor Valor a comparar
 */
void comparar_signos(int32_t valor);

/**
 * @brief Analiza la dirección de memoria de una variable
 * 
 * @param ptr Puntero a analizar
 */
void analizar_direccion_memoria(void *ptr);

/**
 * @brief Demuestra el uso de uniones para reinterpretación de tipos
 */
void demo_reinterpretacion_tipos(void);

/**
 * @brief Muestra ejemplos prácticos de uso de uniones
 */
void ejemplos_practicos_uniones(void);

// Funciones de utilidad

/**
 * @brief Convierte un byte a cadena binaria
 * 
 * @param byte Byte a convertir
 * @param buffer Buffer donde escribir la cadena (al menos 9 caracteres)
 */
void byte_a_binario(uint8_t byte, char *buffer);

/**
 * @brief Convierte un entero a cadena hexadecimal
 * 
 * @param valor Valor a convertir
 * @param buffer Buffer donde escribir la cadena
 * @param buffer_size Tamaño del buffer
 */
void entero_a_hex(int32_t valor, char *buffer, size_t buffer_size);

/**
 * @brief Valida que un puntero sea válido para análisis
 * 
 * @param ptr Puntero a validar
 * @return true si el puntero es válido
 */
bool validar_puntero(void *ptr);

/**
 * @brief Imprime un separador visual para organizar la salida
 * 
 * @param titulo Título del separador
 */
void imprimir_separador(const char *titulo);

#endif // UNION_REINTERPRETAR_H
