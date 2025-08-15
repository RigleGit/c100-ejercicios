#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "contacto.h"
#include <stdio.h>

// Constantes para persistencia
#define NOMBRE_ARCHIVO_DEFAULT "data/agenda.txt"
#define NOMBRE_ARCHIVO_BACKUP "data/agenda_backup.txt"
#define FORMATO_EXPORTACION_CSV "data/agenda.csv"
#define FORMATO_FECHA "%Y-%m-%d %H:%M:%S"

// Tipos de formato de archivo
typedef enum {
    FORMATO_TEXTO = 0,
    FORMATO_CSV = 1,
    FORMATO_JSON = 2,
    FORMATO_BINARIO = 3
} formato_archivo_t;

// Información de metadatos del archivo
typedef struct {
    int version;
    int total_contactos;
    time_t fecha_creacion;
    time_t ultima_modificacion;
    char aplicacion[50];
} metadatos_archivo_t;

// ========================================
// FUNCIONES DE PERSISTENCIA BÁSICA
// ========================================

/**
 * @brief Guarda todos los contactos en un archivo de texto
 * @param cabeza Cabeza de la lista de contactos
 * @param nombre_archivo Nombre del archivo (NULL para usar el default)
 * @return Código de resultado
 */
resultado_t guardar_contactos(const nodo_t *cabeza, const char *nombre_archivo);

/**
 * @brief Carga contactos desde un archivo de texto
 * @param cabeza Puntero a la cabeza de la lista
 * @param nombre_archivo Nombre del archivo (NULL para usar el default)
 * @return Código de resultado
 */
resultado_t cargar_contactos(nodo_t **cabeza, const char *nombre_archivo);

/**
 * @brief Crea una copia de seguridad de la agenda
 * @param cabeza Cabeza de la lista de contactos
 * @return Código de resultado
 */
resultado_t crear_backup(const nodo_t *cabeza);

/**
 * @brief Restaura la agenda desde el backup
 * @param cabeza Puntero a la cabeza de la lista
 * @return Código de resultado
 */
resultado_t restaurar_backup(nodo_t **cabeza);

// ========================================
// FUNCIONES DE EXPORTACIÓN/IMPORTACIÓN
// ========================================

/**
 * @brief Exporta contactos a formato CSV
 * @param cabeza Cabeza de la lista de contactos
 * @param nombre_archivo Nombre del archivo CSV
 * @return Código de resultado
 */
resultado_t exportar_csv(const nodo_t *cabeza, const char *nombre_archivo);

/**
 * @brief Importa contactos desde formato CSV
 * @param cabeza Puntero a la cabeza de la lista
 * @param nombre_archivo Nombre del archivo CSV
 * @param reemplazar Si reemplazar la lista actual
 * @return Código de resultado
 */
resultado_t importar_csv(nodo_t **cabeza, const char *nombre_archivo, bool reemplazar);

/**
 * @brief Exporta contactos a formato JSON (básico)
 * @param cabeza Cabeza de la lista de contactos
 * @param nombre_archivo Nombre del archivo JSON
 * @return Código de resultado
 */
resultado_t exportar_json(const nodo_t *cabeza, const char *nombre_archivo);

/**
 * @brief Exporta solo contactos favoritos
 * @param cabeza Cabeza de la lista de contactos
 * @param nombre_archivo Nombre del archivo
 * @param formato Formato de exportación
 * @return Código de resultado
 */
resultado_t exportar_favoritos(const nodo_t *cabeza, const char *nombre_archivo,
                              formato_archivo_t formato);

// ========================================
// FUNCIONES DE FORMATO BINARIO
// ========================================

/**
 * @brief Guarda contactos en formato binario (más eficiente)
 * @param cabeza Cabeza de la lista de contactos
 * @param nombre_archivo Nombre del archivo binario
 * @return Código de resultado
 */
resultado_t guardar_binario(const nodo_t *cabeza, const char *nombre_archivo);

/**
 * @brief Carga contactos desde formato binario
 * @param cabeza Puntero a la cabeza de la lista
 * @param nombre_archivo Nombre del archivo binario
 * @return Código de resultado
 */
resultado_t cargar_binario(nodo_t **cabeza, const char *nombre_archivo);

// ========================================
// FUNCIONES DE METADATOS Y VALIDACIÓN
// ========================================

/**
 * @brief Valida la integridad de un archivo de contactos
 * @param nombre_archivo Nombre del archivo a validar
 * @param formato Formato esperado del archivo
 * @return true si el archivo es válido
 */
bool validar_archivo(const char *nombre_archivo, formato_archivo_t formato);

/**
 * @brief Obtiene información de metadatos de un archivo
 * @param nombre_archivo Nombre del archivo
 * @return Metadatos del archivo
 */
metadatos_archivo_t obtener_metadatos_archivo(const char *nombre_archivo);

/**
 * @brief Verifica si existe un archivo
 * @param nombre_archivo Nombre del archivo
 * @return true si existe, false en caso contrario
 */
bool archivo_existe(const char *nombre_archivo);

/**
 * @brief Obtiene el tamaño de un archivo
 * @param nombre_archivo Nombre del archivo
 * @return Tamaño en bytes, -1 si hay error
 */
long obtener_tamaño_archivo(const char *nombre_archivo);

/**
 * @brief Crea el directorio de datos si no existe
 * @return Código de resultado
 */
resultado_t crear_directorio_datos(void);

// ========================================
// FUNCIONES DE UTILIDAD Y LOGGING
// ========================================

/**
 * @brief Registra una operación en el log
 * @param operacion Descripción de la operación
 * @param resultado Resultado de la operación
 */
void log_operacion(const char *operacion, resultado_t resultado);

/**
 * @brief Limpia archivos temporales y de backup antiguos
 * @param dias_antiguedad Archivos más antiguos que estos días se eliminan
 * @return Número de archivos eliminados
 */
int limpiar_archivos_antiguos(int dias_antiguedad);

/**
 * @brief Convierte tiempo a string legible
 * @param tiempo Tiempo a convertir
 * @param buffer Buffer donde escribir el resultado
 * @param tamaño_buffer Tamaño del buffer
 */
void tiempo_a_string(time_t tiempo, char *buffer, size_t tamaño_buffer);

/**
 * @brief Convierte string a tiempo
 * @param str String en formato FORMATO_FECHA
 * @return Tiempo convertido
 */
time_t string_a_tiempo(const char *str);

/**
 * @brief Escapa caracteres especiales para CSV
 * @param entrada String de entrada
 * @param salida Buffer de salida
 * @param tamaño_salida Tamaño del buffer de salida
 */
void escapar_csv(const char *entrada, char *salida, size_t tamaño_salida);

/**
 * @brief Desescapa caracteres especiales de CSV
 * @param entrada String de entrada
 * @param salida Buffer de salida
 * @param tamaño_salida Tamaño del buffer de salida
 */
void desescapar_csv(const char *entrada, char *salida, size_t tamaño_salida);

/**
 * @brief Convierte formato_archivo_t a string
 * @param formato Formato a convertir
 * @return String con el nombre del formato
 */
const char *formato_a_string(formato_archivo_t formato);

#endif // PERSISTENCIA_H
