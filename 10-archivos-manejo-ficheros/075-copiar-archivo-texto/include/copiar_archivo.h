/**
 * @file copiar_archivo.h
 * @brief Header para funciones de copia de archivos de texto
 * @author Sistema de Ejercicios C
 * @date 2024
 * 
 * Este ejercicio implementa un sistema completo para copiar archivos de texto
 * en C, incluyendo diferentes métodos de copia (línea por línea, carácter por
 * carácter, por bloques), verificación de integridad, y manejo robusto de errores.
 */

#ifndef COPIAR_ARCHIVO_H
#define COPIAR_ARCHIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

/* ================================
 * CONSTANTES Y CONFIGURACIÓN
 * ================================ */

#define MAX_NOMBRE_ARCHIVO 512
#define MAX_BUFFER 8192
#define BUFFER_LINEA 1024
#define BUFFER_PEQUEÑO 64
#define CHUNK_SIZE 4096

/* ================================
 * ENUMERACIONES
 * ================================ */

/**
 * @brief Métodos de copia disponibles
 */
typedef enum {
    METODO_LINEA_POR_LINEA,    // Copia usando fgets/fputs
    METODO_CARACTER_POR_CARACTER, // Copia usando fgetc/fputc
    METODO_BLOQUE,             // Copia usando fread/fwrite
    METODO_CHUNK,              // Copia por chunks grandes
    METODO_MMAP                // Copia usando memory mapping (avanzado)
} MetodoCopia;

/**
 * @brief Opciones de verificación
 */
typedef enum {
    VERIFICACION_NINGUNA,      // Sin verificación
    VERIFICACION_TAMAÑO,       // Solo verificar tamaño
    VERIFICACION_CONTENIDO,    // Verificación completa de contenido
    VERIFICACION_CHECKSUM      // Verificación con checksum
} TipoVerificacion;

/* ================================
 * ESTRUCTURAS DE DATOS
 * ================================ */

/**
 * @brief Resultado de operaciones de copia
 */
typedef struct {
    bool exito;                    // true si la operación fue exitosa
    char mensaje[512];             // Mensaje descriptivo del resultado
    long bytes_copiados;           // Número de bytes copiados
    double tiempo_transcurrido;    // Tiempo en segundos
    MetodoCopia metodo_usado;      // Método utilizado para la copia
} ResultadoCopia;

/**
 * @brief Estadísticas de la operación de copia
 */
typedef struct {
    long tamaño_origen;            // Tamaño del archivo origen
    long tamaño_destino;           // Tamaño del archivo destino
    int lineas_copiadas;           // Número de líneas copiadas
    double velocidad_mb_s;         // Velocidad en MB/s
    time_t inicio;                 // Tiempo de inicio
    time_t fin;                    // Tiempo de finalización
    bool verificacion_exitosa;     // Resultado de la verificación
} EstadisticasCopia;

/**
 * @brief Configuración para la copia de archivos
 */
typedef struct {
    MetodoCopia metodo;            // Método de copia a utilizar
    TipoVerificacion verificacion; // Tipo de verificación
    bool crear_backup;             // Crear backup si destino existe
    bool mostrar_progreso;         // Mostrar progreso durante la copia
    bool preservar_permisos;       // Preservar permisos del archivo
    size_t tamaño_buffer;          // Tamaño del buffer para copia por bloques
} ConfiguracionCopia;

/**
 * @brief Información de progreso durante la copia
 */
typedef struct {
    long bytes_procesados;         // Bytes procesados hasta ahora
    long bytes_totales;            // Total de bytes a procesar
    double porcentaje;             // Porcentaje completado
    double velocidad_actual;       // Velocidad actual en MB/s
    time_t tiempo_estimado;        // Tiempo estimado restante
} ProgresoCopia;

/* ================================
 * FUNCIONES BÁSICAS DE COPIA
 * ================================ */

/**
 * @brief Función básica de copia como se especifica en el enunciado
 * @param archivo_origen Nombre del archivo origen
 * @param archivo_destino Nombre del archivo destino
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_archivo_basico(const char* archivo_origen, 
                                    const char* archivo_destino);

/**
 * @brief Copia archivo línea por línea usando fgets/fputs
 * @param archivo_origen Nombre del archivo origen
 * @param archivo_destino Nombre del archivo destino
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_linea_por_linea(const char* archivo_origen, 
                                     const char* archivo_destino);

/**
 * @brief Copia archivo carácter por carácter usando fgetc/fputc
 * @param archivo_origen Nombre del archivo origen
 * @param archivo_destino Nombre del archivo destino
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_caracter_por_caracter(const char* archivo_origen, 
                                           const char* archivo_destino);

/**
 * @brief Copia archivo por bloques usando fread/fwrite
 * @param archivo_origen Nombre del archivo origen
 * @param archivo_destino Nombre del archivo destino
 * @param tamaño_buffer Tamaño del buffer a utilizar
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_por_bloques(const char* archivo_origen, 
                                 const char* archivo_destino,
                                 size_t tamaño_buffer);

/* ================================
 * FUNCIONES AVANZADAS DE COPIA
 * ================================ */

/**
 * @brief Copia archivo con configuración personalizada
 * @param archivo_origen Nombre del archivo origen
 * @param archivo_destino Nombre del archivo destino
 * @param config Configuración para la copia
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_archivo_configurado(const char* archivo_origen, 
                                         const char* archivo_destino,
                                         const ConfiguracionCopia* config);

/**
 * @brief Copia archivo con callback de progreso
 * @param archivo_origen Nombre del archivo origen
 * @param archivo_destino Nombre del archivo destino
 * @param callback Función callback para reporte de progreso
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_archivo_con_progreso(const char* archivo_origen, 
                                          const char* archivo_destino,
                                          void (*callback)(const ProgresoCopia*));

/**
 * @brief Copia múltiples archivos en lote
 * @param archivos_origen Array de nombres de archivos origen
 * @param archivos_destino Array de nombres de archivos destino
 * @param num_archivos Número de archivos a copiar
 * @param config Configuración para las copias
 * @return ResultadoCopia con estadísticas consolidadas
 */
ResultadoCopia copiar_archivos_lote(const char** archivos_origen,
                                   const char** archivos_destino,
                                   int num_archivos,
                                   const ConfiguracionCopia* config);

/* ================================
 * FUNCIONES DE VERIFICACIÓN
 * ================================ */

/**
 * @brief Verifica que dos archivos son idénticos
 * @param archivo1 Nombre del primer archivo
 * @param archivo2 Nombre del segundo archivo
 * @param tipo_verificacion Tipo de verificación a realizar
 * @return true si los archivos son idénticos
 */
bool verificar_archivos_identicos(const char* archivo1, 
                                 const char* archivo2,
                                 TipoVerificacion tipo_verificacion);

/**
 * @brief Calcula checksum simple de un archivo
 * @param nombre_archivo Nombre del archivo
 * @return Checksum del archivo (0 si hay error)
 */
unsigned long calcular_checksum_archivo(const char* nombre_archivo);

/**
 * @brief Verifica la integridad de la copia
 * @param archivo_origen Archivo origen
 * @param archivo_destino Archivo destino
 * @param estadisticas Puntero para almacenar estadísticas
 * @return ResultadoCopia con resultado de la verificación
 */
ResultadoCopia verificar_integridad_copia(const char* archivo_origen,
                                         const char* archivo_destino,
                                         EstadisticasCopia* estadisticas);

/* ================================
 * FUNCIONES DE BACKUP Y RECUPERACIÓN
 * ================================ */

/**
 * @brief Crea un backup del archivo destino si existe
 * @param archivo_destino Archivo del cual crear backup
 * @return true si el backup fue creado exitosamente
 */
bool crear_backup_archivo(const char* archivo_destino);

/**
 * @brief Restaura un archivo desde su backup
 * @param archivo_original Archivo a restaurar
 * @return ResultadoCopia con información de la restauración
 */
ResultadoCopia restaurar_desde_backup(const char* archivo_original);

/* ================================
 * FUNCIONES DE ANÁLISIS Y ESTADÍSTICAS
 * ================================ */

/**
 * @brief Obtiene estadísticas detalladas de la operación de copia
 * @param archivo_origen Archivo origen
 * @param archivo_destino Archivo destino
 * @param resultado Resultado de la operación de copia
 * @return EstadisticasCopia con información detallada
 */
EstadisticasCopia obtener_estadisticas_copia(const char* archivo_origen,
                                            const char* archivo_destino,
                                            const ResultadoCopia* resultado);

/**
 * @brief Compara el rendimiento de diferentes métodos de copia
 * @param archivo_origen Archivo a usar para las pruebas
 * @param archivo_destino_base Nombre base para archivos de destino
 * @return ResultadoCopia con resultados de la comparación
 */
ResultadoCopia comparar_metodos_copia(const char* archivo_origen,
                                     const char* archivo_destino_base);

/* ================================
 * FUNCIONES INTERACTIVAS
 * ================================ */

/**
 * @brief Solicita archivos al usuario y realiza la copia
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia copiar_archivo_interactivo(void);

/**
 * @brief Menú interactivo para opciones de copia
 * @param archivo_origen Archivo origen (NULL para solicitar al usuario)
 * @param archivo_destino Archivo destino (NULL para solicitar al usuario)
 * @return ResultadoCopia con información de la operación
 */
ResultadoCopia menu_copia_archivo(const char* archivo_origen,
                                 const char* archivo_destino);

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
 * @brief Verifica si un directorio existe y es escribible
 * @param ruta_directorio Ruta del directorio a verificar
 * @return true si el directorio existe y es escribible
 */
bool directorio_escribible(const char* ruta_directorio);

/**
 * @brief Obtiene el tamaño de un archivo
 * @param nombre_archivo Nombre del archivo
 * @return Tamaño del archivo en bytes (-1 si hay error)
 */
long obtener_tamaño_archivo(const char* nombre_archivo);

/**
 * @brief Calcula la velocidad de transferencia
 * @param bytes_transferidos Bytes transferidos
 * @param tiempo_segundos Tiempo transcurrido en segundos
 * @return Velocidad en MB/s
 */
double calcular_velocidad_transferencia(long bytes_transferidos, 
                                       double tiempo_segundos);

/**
 * @brief Formatea el tiempo transcurrido en cadena legible
 * @param segundos Tiempo en segundos
 * @param buffer Buffer donde escribir el resultado
 * @param tamaño_buffer Tamaño del buffer
 */
void formatear_tiempo(double segundos, char* buffer, size_t tamaño_buffer);

/**
 * @brief Formatea el tamaño en bytes a unidades legibles
 * @param bytes Tamaño en bytes
 * @param buffer Buffer donde escribir el resultado
 * @param tamaño_buffer Tamaño del buffer
 */
void formatear_tamaño(long bytes, char* buffer, size_t tamaño_buffer);

/**
 * @brief Imprime un mensaje de error relacionado con archivos
 * @param operacion Descripción de la operación que falló
 * @param nombre_archivo Nombre del archivo relacionado
 */
void imprimir_error_archivo(const char* operacion, const char* nombre_archivo);

/**
 * @brief Crea una configuración de copia por defecto
 * @return ConfiguracionCopia inicializada con valores por defecto
 */
ConfiguracionCopia crear_configuracion_defecto(void);

/**
 * @brief Callback por defecto para mostrar progreso
 * @param progreso Información del progreso actual
 */
void callback_progreso_defecto(const ProgresoCopia* progreso);

/**
 * @brief Función principal básica como se especifica en el enunciado
 * @return Código de salida del programa
 */
int main_basico(void);

#endif // COPIAR_ARCHIVO_H
