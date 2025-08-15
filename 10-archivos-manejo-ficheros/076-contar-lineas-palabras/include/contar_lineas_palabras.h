#ifndef CONTAR_LINEAS_PALABRAS_H
#define CONTAR_LINEAS_PALABRAS_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Estructura para almacenar estadísticas de un archivo de texto
 */
typedef struct {
    int lineas;       ///< Número de líneas en el archivo
    int palabras;     ///< Número de palabras en el archivo
    int caracteres;   ///< Número de caracteres en el archivo
    int bytes;        ///< Número de bytes en el archivo
} EstadisticasArchivo;

/**
 * @brief Cuenta líneas, palabras y caracteres en un archivo
 * @param nombre_archivo Nombre del archivo a analizar
 * @param stats Puntero a estructura donde almacenar las estadísticas
 * @return true si el análisis fue exitoso, false en caso de error
 */
bool contar_archivo_basico(const char* nombre_archivo, EstadisticasArchivo* stats);

/**
 * @brief Versión avanzada que cuenta líneas, palabras, caracteres y bytes
 * @param nombre_archivo Nombre del archivo a analizar
 * @param stats Puntero a estructura donde almacenar las estadísticas
 * @return true si el análisis fue exitoso, false en caso de error
 */
bool contar_archivo_completo(const char* nombre_archivo, EstadisticasArchivo* stats);

/**
 * @brief Cuenta estadísticas desde un stream de archivo abierto
 * @param archivo Stream de archivo abierto para lectura
 * @param stats Puntero a estructura donde almacenar las estadísticas
 * @return true si el análisis fue exitoso, false en caso de error
 */
bool contar_desde_stream(FILE* archivo, EstadisticasArchivo* stats);

/**
 * @brief Cuenta palabras usando un algoritmo más robusto
 * @param nombre_archivo Nombre del archivo a analizar
 * @param stats Puntero a estructura donde almacenar las estadísticas
 * @return true si el análisis fue exitoso, false en caso de error
 */
bool contar_palabras_robusto(const char* nombre_archivo, EstadisticasArchivo* stats);

/**
 * @brief Imprime las estadísticas en formato estándar
 * @param stats Puntero a estructura con las estadísticas
 * @param nombre_archivo Nombre del archivo (opcional, puede ser NULL)
 */
void imprimir_estadisticas(const EstadisticasArchivo* stats, const char* nombre_archivo);

/**
 * @brief Imprime las estadísticas en formato similar a wc
 * @param stats Puntero a estructura con las estadísticas
 * @param nombre_archivo Nombre del archivo (opcional, puede ser NULL)
 */
void imprimir_formato_wc(const EstadisticasArchivo* stats, const char* nombre_archivo);

/**
 * @brief Inicializa una estructura de estadísticas con valores por defecto
 * @param stats Puntero a estructura a inicializar
 */
void inicializar_estadisticas(EstadisticasArchivo* stats);

/**
 * @brief Verifica si un carácter es considerado separador de palabras
 * @param c Carácter a verificar
 * @return true si es separador, false en caso contrario
 */
bool es_separador_palabra(char c);

#endif // CONTAR_LINEAS_PALABRAS_H
