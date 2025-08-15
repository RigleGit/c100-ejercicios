/*
 * Ejercicio 073: Escribir Archivo de Texto
 * 
 * Header con definiciones de funciones para manejo básico de archivos,
 * específicamente escritura de texto en archivos.
 * 
 * Este ejercicio introduce conceptos fundamentales de E/S de archivos
 * en C, incluyendo apertura, escritura y cierre seguro de archivos.
 */

#ifndef ESCRIBIR_ARCHIVO_H
#define ESCRIBIR_ARCHIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

/* ================================
 * CONSTANTES Y CONFIGURACIÓN
 * ================================ */

#define MAX_LINEA 1024
#define MAX_NOMBRE_ARCHIVO 256
#define MAX_BUFFER 4096
#define MAX_LINEAS_MEMORIA 1000

/* ================================
 * ESTRUCTURAS DE DATOS
 * ================================ */

typedef struct {
    int lineas_escritas;
    int caracteres_escritos;
    int archivos_creados;
    time_t tiempo_inicio;
    time_t tiempo_fin;
    bool operacion_exitosa;
} EstadisticasEscritura;

// Estructuras esperadas por los tests de Criterion
typedef struct {
    bool exito;
    char mensaje[256];
} ResultadoEscritura;

typedef struct {
    bool existe;
    unsigned long tamaño;
    time_t fecha_modificacion;
} InfoArchivo;

typedef struct {
    char** lineas;
    int num_lineas;
    int capacidad;
} BufferTexto;

typedef enum {
    MODO_ESCRIBIR,      // "w" - sobrescribe archivo existente
    MODO_ANEXAR,        // "a" - añade al final del archivo
    MODO_CREAR_NUEVO    // "wx" - falla si el archivo ya existe
} ModoEscritura;

/* ================================
 * FUNCIONES BÁSICAS DE ESCRITURA
 * ================================ */

/**
 * @brief Función básica para escribir una línea de texto en un archivo
 * @param nombre_archivo Nombre del archivo a crear/escribir
 * @param texto Texto a escribir
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool escribir_linea_basica(const char* nombre_archivo, const char* texto);

// API usada por los tests
ResultadoEscritura escribir_archivo_texto(const char* nombre_archivo, const char* mensaje);
ResultadoEscritura escribir_archivo_formato(const char* nombre_archivo, const char* formato, ...);
ResultadoEscritura concatenar_archivos(const char* archivo1, const char* archivo2, const char* archivo_destino);
ResultadoEscritura escribir_multiples_lineas(const char* nombre_archivo, const char* lineas[], int num_lineas);
InfoArchivo obtener_info_archivo(const char* nombre_archivo);
ResultadoEscritura copiar_archivo(const char* archivo_origen, const char* archivo_destino);

/**
 * @brief Escribe texto usando fprintf (formateo avanzado)
 * @param nombre_archivo Nombre del archivo
 * @param formato Cadena de formato (como printf)
 * @param ... Argumentos variables para el formato
 * @return true si exitoso, false en caso contrario
 */
bool escribir_con_formato(const char* nombre_archivo, const char* formato, ...);

/**
 * @brief Escribe texto usando fputs (más eficiente para texto plano)
 * @param nombre_archivo Nombre del archivo
 * @param texto Texto a escribir
 * @return true si exitoso, false en caso contrario
 */
bool escribir_con_fputs(const char* nombre_archivo, const char* texto);

/**
 * @brief Escribe texto usando fwrite (control de bytes)
 * @param nombre_archivo Nombre del archivo
 * @param datos Datos a escribir
 * @param tamanio Número de bytes a escribir
 * @return true si exitoso, false en caso contrario
 */
bool escribir_con_fwrite(const char* nombre_archivo, const void* datos, size_t tamanio);

/* ================================
 * FUNCIONES INTERACTIVAS
 * ================================ */

/**
 * @brief Solicita texto al usuario y lo escribe en un archivo
 * @param nombre_archivo Nombre del archivo de destino
 * @return true si exitoso, false en caso contrario
 */
bool escribir_texto_interactivo(const char* nombre_archivo);

/**
 * @brief Solicita múltiples líneas al usuario hasta que escriba "FIN"
 * @param nombre_archivo Nombre del archivo de destino
 * @param modo Modo de escritura (escribir, anexar, crear nuevo)
 * @return Estadísticas de la operación
 */
// Versión interactiva (renombrada para no colisionar con la API de tests)
EstadisticasEscritura escribir_multiples_lineas_interactivo(const char* nombre_archivo, ModoEscritura modo);

/**
 * @brief Permite al usuario crear un archivo con menú interactivo
 * @return true si se completó exitosamente
 */
bool crear_archivo_interactivo(void);

/* ================================
 * FUNCIONES AVANZADAS
 * ================================ */

/**
 * @brief Escribe un array de cadenas en un archivo
 * @param nombre_archivo Nombre del archivo
 * @param lineas Array de cadenas
 * @param num_lineas Número de líneas en el array
 * @param separador Separador entre líneas (por defecto "\n")
 * @return Estadísticas de la escritura
 */
EstadisticasEscritura escribir_array_lineas(const char* nombre_archivo, 
                                           char** lineas, int num_lineas, 
                                           const char* separador);

/**
 * @brief Escribe contenido con timestamps
 * @param nombre_archivo Nombre del archivo
 * @param contenido Contenido a escribir
 * @param incluir_timestamp Si incluir marca de tiempo
 * @return true si exitoso
 */
bool escribir_con_timestamp(const char* nombre_archivo, const char* contenido, bool incluir_timestamp);

/**
 * @brief Anexa contenido a un archivo existente
 * @param nombre_archivo Nombre del archivo
 * @param contenido Contenido a anexar
 * @return true si exitoso
 */
bool anexar_contenido(const char* nombre_archivo, const char* contenido);

/**
 * @brief Escribe contenido con backup automático
 * @param nombre_archivo Nombre del archivo
 * @param contenido Contenido a escribir
 * @param crear_backup Si crear copia de seguridad del archivo existente
 * @return true si exitoso
 */
bool escribir_con_backup(const char* nombre_archivo, const char* contenido, bool crear_backup);

/* ================================
 * GESTIÓN DE BUFFER
 * ================================ */

/**
 * @brief Crea un buffer de texto en memoria
 * @param capacidad_inicial Capacidad inicial del buffer
 * @return Puntero al buffer creado o NULL si falla
 */
BufferTexto* buffer_crear(int capacidad_inicial);

/**
 * @brief Destruye un buffer de texto
 * @param buffer Buffer a destruir
 */
void buffer_destruir(BufferTexto* buffer);

/**
 * @brief Añade una línea al buffer
 * @param buffer Buffer de destino
 * @param linea Línea a añadir
 * @return true si exitoso
 */
bool buffer_agregar_linea(BufferTexto* buffer, const char* linea);

/**
 * @brief Escribe todo el contenido del buffer a un archivo
 * @param buffer Buffer de origen
 * @param nombre_archivo Archivo de destino
 * @return Estadísticas de la escritura
 */
EstadisticasEscritura buffer_escribir_archivo(BufferTexto* buffer, const char* nombre_archivo);

/* ================================
 * VALIDACIÓN Y UTILIDADES
 * ================================ */

/**
 * @brief Valida si un nombre de archivo es válido
 * @param nombre_archivo Nombre a validar
 * @return true si es válido
 */
bool validar_nombre_archivo(const char* nombre_archivo);

/**
 * @brief Verifica si un archivo existe
 * @param nombre_archivo Nombre del archivo
 * @return true si existe
 */
bool archivo_existe(const char* nombre_archivo);

/**
 * @brief Obtiene el tamaño de un archivo
 * @param nombre_archivo Nombre del archivo
 * @return Tamaño en bytes, -1 si error
 */
long obtener_tamanio_archivo(const char* nombre_archivo);

/**
 * @brief Crea un nombre de archivo backup
 * @param nombre_original Nombre original
 * @param nombre_backup Buffer para el nombre backup
 * @param tamanio_buffer Tamaño del buffer
 * @return true si exitoso
 */
bool crear_nombre_backup(const char* nombre_original, char* nombre_backup, size_t tamanio_buffer);

/**
 * @brief Limpia el contenido de un archivo (lo deja vacío)
 * @param nombre_archivo Nombre del archivo
 * @return true si exitoso
 */
bool limpiar_archivo(const char* nombre_archivo);

/* ================================
 * MANEJO DE ERRORES
 * ================================ */

/**
 * @brief Imprime mensaje de error detallado
 * @param operacion Descripción de la operación que falló
 * @param nombre_archivo Nombre del archivo relacionado
 */
void imprimir_error_archivo(const char* operacion, const char* nombre_archivo);

/**
 * @brief Obtiene descripción del último error de archivo
 * @param buffer Buffer para la descripción
 * @param tamanio_buffer Tamaño del buffer
 * @return Puntero al buffer con la descripción
 */
char* obtener_descripcion_error(char* buffer, size_t tamanio_buffer);

/* ================================
 * ESTADÍSTICAS E INFORMACIÓN
 * ================================ */

/**
 * @brief Inicializa estructura de estadísticas
 * @param stats Estructura a inicializar
 */
void estadisticas_inicializar(EstadisticasEscritura* stats);

/**
 * @brief Imprime estadísticas de escritura
 * @param stats Estadísticas a imprimir
 * @param titulo Título para el reporte
 */
void estadisticas_imprimir(const EstadisticasEscritura* stats, const char* titulo);

/**
 * @brief Obtiene información de un archivo
 * @param nombre_archivo Nombre del archivo
 * @param lineas Puntero para guardar número de líneas
 * @param caracteres Puntero para guardar número de caracteres
 * @param tamanio Puntero para guardar tamaño en bytes
 * @return true si exitoso
 */
// Versión detallada (renombrada para no colisionar con la API de tests)
bool obtener_info_archivo_detallada(const char* nombre_archivo, int* lineas, int* caracteres, long* tamanio);

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

/**
 * @brief Demostración básica de escritura de archivos
 */
void demo_escritura_basica(void);

/**
 * @brief Demostración de diferentes métodos de escritura
 */
void demo_metodos_escritura(void);

/**
 * @brief Demostración de escritura interactiva
 */
void demo_escritura_interactiva(void);

/**
 * @brief Demostración de funciones avanzadas
 */
void demo_funciones_avanzadas(void);

/**
 * @brief Demostración completa de todas las funcionalidades
 */
void demo_completa_escritura_archivos(void);

/* ================================
 * CONVERSIÓN DE MODO
 * ================================ */

/**
 * @brief Convierte enum ModoEscritura a cadena para fopen
 * @param modo Modo de escritura
 * @return Cadena correspondiente para fopen
 */
const char* modo_a_cadena(ModoEscritura modo);

/**
 * @brief Obtiene descripción legible del modo de escritura
 * @param modo Modo de escritura
 * @return Descripción del modo
 */
const char* descripcion_modo(ModoEscritura modo);

#endif /* ESCRIBIR_ARCHIVO_H */
