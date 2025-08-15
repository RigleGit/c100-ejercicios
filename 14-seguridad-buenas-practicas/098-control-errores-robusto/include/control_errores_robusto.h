#ifndef CONTROL_ERRORES_ROBUSTO_H
#define CONTROL_ERRORES_ROBUSTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Códigos de error personalizados
typedef enum {
    ERROR_NONE = 0,
    ERROR_FILE_OPEN = 1,
    ERROR_MEMORY_ALLOC = 2,
    ERROR_INVALID_INPUT = 3,
    ERROR_FILE_READ = 4,
    ERROR_FILE_WRITE = 5
} error_code_t;

// Estructura para manejar resultados con error
typedef struct {
    int success;
    error_code_t error_code;
    char error_message[256];
} result_t;

/**
 * @brief Función insegura que no verifica errores
 * @return 0 en éxito, código de error en fallo
 */
int funcion_insegura(void);

/**
 * @brief Función segura con verificación completa de errores
 * @return result_t con información del resultado
 */
result_t funcion_segura(void);

/**
 * @brief Abre un archivo de forma segura con verificación de errores
 * @param filename Nombre del archivo a abrir
 * @param mode Modo de apertura ("r", "w", "a", etc.)
 * @param file Puntero donde almacenar el FILE*
 * @return result_t con información del resultado
 */
result_t abrir_archivo_seguro(const char *filename, const char *mode, FILE **file);

/**
 * @brief Asigna memoria de forma segura con verificación
 * @param size Tamaño en bytes a asignar
 * @param ptr Puntero donde almacenar la dirección de memoria
 * @return result_t con información del resultado
 */
result_t asignar_memoria_seguro(size_t size, void **ptr);

/**
 * @brief Lee un entero de forma segura desde stdin
 * @param valor Puntero donde almacenar el valor leído
 * @return result_t con información del resultado
 */
result_t leer_entero_seguro(int *valor);

/**
 * @brief Lee una línea de forma segura desde un archivo
 * @param file Archivo desde donde leer
 * @param buffer Buffer donde almacenar la línea
 * @param size Tamaño del buffer
 * @return result_t con información del resultado
 */
result_t leer_linea_seguro(FILE *file, char *buffer, size_t size);

/**
 * @brief Escribe datos de forma segura a un archivo
 * @param file Archivo donde escribir
 * @param data Datos a escribir
 * @param size Tamaño de los datos
 * @return result_t con información del resultado
 */
result_t escribir_datos_seguro(FILE *file, const void *data, size_t size);

/**
 * @brief Libera recursos de forma segura y maneja errores
 * @param file Archivo a cerrar (puede ser NULL)
 * @param ptr Memoria a liberar (puede ser NULL)
 */
void liberar_recursos_seguro(FILE *file, void *ptr);

/**
 * @brief Inicializa un result_t con éxito
 * @return result_t inicializado
 */
result_t result_ok(void);

/**
 * @brief Crea un result_t de error
 * @param code Código de error
 * @param message Mensaje de error
 * @return result_t con error
 */
result_t result_error(error_code_t code, const char *message);

/**
 * @brief Imprime información de error de forma consistente
 * @param result Resultado que contiene el error
 */
void imprimir_error(const result_t *result);

#endif // CONTROL_ERRORES_ROBUSTO_H
