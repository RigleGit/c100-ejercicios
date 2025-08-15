/**
 * @file leer_archivo.h
 * @brief Header para funciones de lectura de archivos de texto
 * @author Sistema de Ejercicios C
 * @date 2024
 * 
 * Este ejercicio implementa un sistema completo para la lectura de archivos
 * de texto en C, incluyendo funciones básicas y avanzadas para el manejo
 * de archivos, lectura línea por línea, procesamiento de contenido y
 * análisis de texto.
 */

#ifndef LEER_ARCHIVO_H
#define LEER_ARCHIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

/* ================================
 * CONSTANTES Y CONFIGURACIÓN
 * ================================ */

#define MAX_LINEA 1024
#define MAX_NOMBRE_ARCHIVO 256
#define MAX_BUFFER 4096
#define MAX_LINEAS_MEMORIA 1000
#define MAX_PALABRAS 10000

/* ================================
 * ESTRUCTURAS DE DATOS
 * ================================ */

/**
 * @brief Resultado de operaciones de lectura
 */
typedef struct {
    bool exito;           // true si la operación fue exitosa
    char mensaje[256];    // Mensaje descriptivo del resultado
    int lineas_leidas;    // Número de líneas procesadas
    int caracteres_leidos; // Número de caracteres leídos
} ResultadoLectura;

/**
 * @brief Información estadística de un archivo
 */
typedef struct {
    bool existe;              // true si el archivo existe
    long tamaño;             // Tamaño del archivo en bytes
    int numero_lineas;       // Número de líneas en el archivo
    int numero_palabras;     // Número de palabras en el archivo
    int numero_caracteres;   // Número de caracteres (sin espacios)
    time_t fecha_modificacion; // Fecha de última modificación
} EstadisticasArchivo;

/**
 * @brief Buffer para almacenar líneas de texto
 */
typedef struct {
    char** lineas;        // Array de punteros a líneas
    int num_lineas;       // Número actual de líneas
    int capacidad;        // Capacidad máxima del buffer
} BufferLineas;

/**
 * @brief Información de una palabra encontrada
 */
typedef struct {
    char palabra[64];     // La palabra encontrada
    int frecuencia;       // Número de veces que aparece
    int primera_linea;    // Línea donde aparece por primera vez
} InfoPalabra;

/**
 * @brief Configuración para la lectura de archivos
 */
typedef struct {
    bool mostrar_numeros_linea;   // Mostrar números de línea
    bool ignorar_lineas_vacias;   // Omitir líneas vacías
    bool convertir_a_mayusculas;  // Convertir a mayúsculas
    bool mostrar_estadisticas;    // Mostrar estadísticas al final
    int max_lineas_mostrar;       // Máximo de líneas a mostrar (-1 = todas)
} ConfiguracionLectura;

/* ================================
 * FUNCIONES BÁSICAS DE LECTURA
 * ================================ */

/**
 * @brief Función básica para leer y mostrar un archivo línea por línea
 * @param nombre_archivo Nombre del archivo a leer
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura leer_archivo_basico(const char* nombre_archivo);

/**
 * @brief Lee un archivo y muestra su contenido con números de línea
 * @param nombre_archivo Nombre del archivo a leer
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura leer_archivo_numerado(const char* nombre_archivo);

/**
 * @brief Lee un archivo aplicando configuración específica
 * @param nombre_archivo Nombre del archivo a leer
 * @param config Configuración para la lectura
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura leer_archivo_configurado(const char* nombre_archivo, 
                                         const ConfiguracionLectura* config);

/* ================================
 * FUNCIONES DE ALMACENAMIENTO
 * ================================ */

/**
 * @brief Lee todas las líneas de un archivo y las almacena en memoria
 * @param nombre_archivo Nombre del archivo a leer
 * @param buffer Buffer donde almacenar las líneas
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura cargar_archivo_en_memoria(const char* nombre_archivo, 
                                          BufferLineas* buffer);

/**
 * @brief Libera la memoria utilizada por un BufferLineas
 * @param buffer Buffer a liberar
 */
void liberar_buffer_lineas(BufferLineas* buffer);

/* ================================
 * FUNCIONES DE ANÁLISIS
 * ================================ */

/**
 * @brief Obtiene estadísticas completas de un archivo
 * @param nombre_archivo Nombre del archivo a analizar
 * @return EstadisticasArchivo con información del archivo
 */
EstadisticasArchivo obtener_estadisticas_archivo(const char* nombre_archivo);

/**
 * @brief Busca una palabra específica en un archivo
 * @param nombre_archivo Nombre del archivo donde buscar
 * @param palabra Palabra a buscar
 * @param sensible_mayusculas true para búsqueda sensible a mayúsculas
 * @return ResultadoLectura con información de la búsqueda
 */
ResultadoLectura buscar_palabra_en_archivo(const char* nombre_archivo, 
                                          const char* palabra,
                                          bool sensible_mayusculas);

/**
 * @brief Cuenta la frecuencia de todas las palabras en un archivo
 * @param nombre_archivo Nombre del archivo a analizar
 * @param palabras Array donde almacenar la información de palabras
 * @param max_palabras Tamaño máximo del array de palabras
 * @return Número de palabras únicas encontradas
 */
int contar_frecuencia_palabras(const char* nombre_archivo, 
                              InfoPalabra* palabras, 
                              int max_palabras);

/* ================================
 * FUNCIONES DE PROCESAMIENTO
 * ================================ */

/**
 * @brief Lee líneas que coinciden con un patrón específico
 * @param nombre_archivo Nombre del archivo a procesar
 * @param patron Patrón a buscar en cada línea
 * @param sensible_mayusculas true para búsqueda sensible a mayúsculas
 * @return ResultadoLectura con información del procesamiento
 */
ResultadoLectura filtrar_lineas_por_patron(const char* nombre_archivo,
                                           const char* patron,
                                           bool sensible_mayusculas);

/**
 * @brief Lee un rango específico de líneas del archivo
 * @param nombre_archivo Nombre del archivo a leer
 * @param linea_inicio Línea de inicio (1-indexada)
 * @param linea_fin Línea final (1-indexada, -1 para hasta el final)
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura leer_rango_lineas(const char* nombre_archivo,
                                  int linea_inicio,
                                  int linea_fin);

/* ================================
 * FUNCIONES DE COMPARACIÓN
 * ================================ */

/**
 * @brief Compara dos archivos línea por línea
 * @param archivo1 Nombre del primer archivo
 * @param archivo2 Nombre del segundo archivo
 * @param mostrar_diferencias true para mostrar las diferencias
 * @return ResultadoLectura con información de la comparación
 */
ResultadoLectura comparar_archivos(const char* archivo1,
                                  const char* archivo2,
                                  bool mostrar_diferencias);

/* ================================
 * FUNCIONES INTERACTIVAS
 * ================================ */

/**
 * @brief Solicita al usuario un archivo y lo muestra
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura leer_archivo_interactivo(void);

/**
 * @brief Menú interactivo para opciones de lectura de archivos
 * @param nombre_archivo Archivo a procesar (NULL para solicitar al usuario)
 * @return ResultadoLectura con información de la operación
 */
ResultadoLectura menu_lectura_archivo(const char* nombre_archivo);

/* ================================
 * FUNCIONES AUXILIARES
 * ================================ */

/**
 * @brief Verifica si un archivo existe y es legible
 * @param nombre_archivo Nombre del archivo a verificar
 * @return true si el archivo existe y es legible
 */
bool archivo_existe_y_legible(const char* nombre_archivo);

/**
 * @brief Imprime un mensaje de error relacionado con archivos
 * @param operacion Descripción de la operación que falló
 * @param nombre_archivo Nombre del archivo relacionado
 */
void imprimir_error_archivo(const char* operacion, const char* nombre_archivo);

/**
 * @brief Crea una configuración de lectura por defecto
 * @return ConfiguracionLectura inicializada con valores por defecto
 */
ConfiguracionLectura crear_configuracion_defecto(void);

/**
 * @brief Función principal básica como se proporciona en el enunciado
 * @return Código de salida del programa
 */
int main_basico(void);

#endif // LEER_ARCHIVO_H
