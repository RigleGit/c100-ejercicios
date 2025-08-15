#ifndef LEER_REGISTROS_BINARIO_H
#define LEER_REGISTROS_BINARIO_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

/**
 * @brief Estructura básica de Producto (compatible con ejercicio 077)
 */
typedef struct {
    int codigo;       ///< Código del producto (entero)
    float precio;     ///< Precio del producto (float)
} Producto;

/**
 * @brief Estructura extendida de Producto (compatible con ejercicio 077)
 */
typedef struct {
    int codigo;              ///< Código del producto
    float precio;            ///< Precio del producto
    char nombre[50];         ///< Nombre del producto
    int stock;               ///< Cantidad en stock
    char categoria[20];      ///< Categoría del producto
    double descuento;        ///< Porcentaje de descuento
} ProductoExtendido;

/**
 * @brief Header de archivo con metadatos (compatible con ejercicio 077)
 */
typedef struct {
    char magic[4];        ///< "PROD" como identificador
    int version;          ///< Versión del formato
    size_t num_registros; ///< Número de registros
    time_t timestamp;     ///< Timestamp de creación
    unsigned int checksum; ///< Checksum para validación
} HeaderArchivo;

/**
 * @brief Estructura para estadísticas de operaciones de lectura
 */
typedef struct {
    size_t elementos_leidos;      ///< Número de elementos leídos
    size_t bytes_leidos;          ///< Número de bytes leídos
    bool operacion_exitosa;       ///< Si la operación fue exitosa
    bool archivo_existe;          ///< Si el archivo existe
    bool formato_valido;          ///< Si el formato es válido
    size_t tamaño_archivo;        ///< Tamaño total del archivo
} EstadisticasLectura;

/**
 * @brief Resultado de operación de lectura con información detallada
 */
typedef struct {
    Producto* productos;          ///< Array de productos leídos
    size_t num_productos;         ///< Número de productos leídos
    EstadisticasLectura stats;    ///< Estadísticas de la operación
    bool necesita_liberacion;     ///< Si el array fue asignado dinámicamente
} ResultadoLectura;

/**
 * @brief Lee productos básicos de un archivo binario
 * @param nombre_archivo Nombre del archivo a leer
 * @param productos Buffer donde almacenar los productos leídos
 * @param max_productos Capacidad máxima del buffer
 * @param stats Puntero a estructura para almacenar estadísticas (opcional)
 * @return Número de productos leídos, o 0 en caso de error
 */
size_t leer_productos_basico(const char* nombre_archivo, Producto* productos, 
                            size_t max_productos, EstadisticasLectura* stats);

/**
 * @brief Lee productos con asignación dinámica de memoria
 * @param nombre_archivo Nombre del archivo a leer
 * @param resultado Puntero a estructura donde almacenar el resultado
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool leer_productos_dinamico(const char* nombre_archivo, ResultadoLectura* resultado);

/**
 * @brief Lee productos extendidos de un archivo binario
 * @param nombre_archivo Nombre del archivo a leer
 * @param productos Buffer donde almacenar los productos leídos
 * @param max_productos Capacidad máxima del buffer
 * @param stats Puntero a estructura para almacenar estadísticas (opcional)
 * @return Número de productos leídos, o 0 en caso de error
 */
size_t leer_productos_extendido(const char* nombre_archivo, ProductoExtendido* productos,
                               size_t max_productos, EstadisticasLectura* stats);

/**
 * @brief Lee productos de archivo con validación de checksum
 * @param nombre_archivo Nombre del archivo a leer
 * @param productos Buffer donde almacenar los productos leídos
 * @param max_productos Capacidad máxima del buffer
 * @param validar_checksum Si validar el checksum del archivo
 * @return Número de productos leídos, o 0 en caso de error o validación fallida
 */
size_t leer_productos_con_validacion(const char* nombre_archivo, Producto* productos,
                                     size_t max_productos, bool validar_checksum);

/**
 * @brief Lee productos de archivo con header de metadatos
 * @param nombre_archivo Nombre del archivo a leer
 * @param productos Buffer donde almacenar los productos leídos
 * @param max_productos Capacidad máxima del buffer
 * @param header Puntero donde almacenar el header leído (opcional)
 * @return Número de productos leídos, o 0 en caso de error
 */
size_t leer_productos_con_header(const char* nombre_archivo, Producto* productos,
                                 size_t max_productos, HeaderArchivo* header);

/**
 * @brief Lee un producto específico por posición (índice)
 * @param nombre_archivo Nombre del archivo a leer
 * @param indice Índice del producto a leer (0-based)
 * @param producto Puntero donde almacenar el producto leído
 * @return true si se leyó correctamente, false en caso contrario
 */
bool leer_producto_por_indice(const char* nombre_archivo, size_t indice, Producto* producto);

/**
 * @brief Busca un producto por código en el archivo
 * @param nombre_archivo Nombre del archivo a leer
 * @param codigo Código del producto a buscar
 * @param producto Puntero donde almacenar el producto encontrado
 * @return true si se encontró el producto, false en caso contrario
 */
bool buscar_producto_por_codigo(const char* nombre_archivo, int codigo, Producto* producto);

/**
 * @brief Cuenta el número de productos en un archivo binario
 * @param nombre_archivo Nombre del archivo a analizar
 * @return Número de productos en el archivo, o 0 en caso de error
 */
size_t contar_productos_en_archivo(const char* nombre_archivo);

/**
 * @brief Obtiene información del archivo (tamaño, número de registros, etc.)
 * @param nombre_archivo Nombre del archivo a analizar
 * @param stats Puntero donde almacenar las estadísticas
 * @return true si se obtuvo la información correctamente, false en caso contrario
 */
bool obtener_info_archivo(const char* nombre_archivo, EstadisticasLectura* stats);

/**
 * @brief Valida la integridad de un archivo de productos
 * @param nombre_archivo Nombre del archivo a validar
 * @return true si el archivo es válido, false en caso contrario
 */
bool validar_integridad_archivo(const char* nombre_archivo);

/**
 * @brief Valida un producto individual después de leerlo
 * @param producto Puntero al producto a validar
 * @return true si el producto es válido, false en caso contrario
 */
bool validar_producto_leido(const Producto* producto);

/**
 * @brief Imprime información de un producto
 * @param producto Puntero al producto a imprimir
 * @param indice Índice del producto (para numeración)
 */
void imprimir_producto(const Producto* producto, size_t indice);

/**
 * @brief Imprime información de un producto extendido
 * @param producto Puntero al producto extendido a imprimir
 * @param indice Índice del producto (para numeración)
 */
void imprimir_producto_extendido(const ProductoExtendido* producto, size_t indice);

/**
 * @brief Imprime estadísticas de lectura
 * @param stats Puntero a las estadísticas a imprimir
 */
void imprimir_estadisticas_lectura(const EstadisticasLectura* stats);

/**
 * @brief Imprime información del header de archivo
 * @param header Puntero al header a imprimir
 */
void imprimir_header_archivo(const HeaderArchivo* header);

/**
 * @brief Inicializa estructura de estadísticas de lectura
 * @param stats Puntero a estructura a inicializar
 */
void inicializar_estadisticas_lectura(EstadisticasLectura* stats);

/**
 * @brief Libera memoria de un resultado de lectura dinámica
 * @param resultado Puntero al resultado a liberar
 */
void liberar_resultado_lectura(ResultadoLectura* resultado);

/**
 * @brief Calcula checksum de un array de productos (para validación)
 * @param productos Array de productos
 * @param num_productos Número de productos
 * @return Checksum calculado
 */
unsigned int calcular_checksum_productos(const Producto* productos, size_t num_productos);

/**
 * @brief Convierte timestamp a string legible
 * @param timestamp Timestamp a convertir
 * @param buffer Buffer donde almacenar el string (mínimo 26 caracteres)
 * @return Puntero al buffer con el string
 */
char* timestamp_a_string(time_t timestamp, char* buffer);

#endif // LEER_REGISTROS_BINARIO_H
