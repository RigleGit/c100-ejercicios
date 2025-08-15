#ifndef ESCRIBIR_REGISTROS_BINARIO_H
#define ESCRIBIR_REGISTROS_BINARIO_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Estructura básica de Producto según el enunciado
 */
typedef struct {
    int codigo;       ///< Código del producto (entero)
    float precio;     ///< Precio del producto (float)
} Producto;

/**
 * @brief Estructura extendida de Producto con más campos
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
 * @brief Estructura para estadísticas de operaciones
 */
typedef struct {
    size_t elementos_escritos;    ///< Número de elementos escritos
    size_t bytes_escritos;        ///< Número de bytes escritos
    bool operacion_exitosa;       ///< Si la operación fue exitosa
} EstadisticasEscritura;

/**
 * @brief Escribe un array de productos básicos a un archivo binario
 * @param productos Array de productos a escribir
 * @param num_productos Número de productos en el array
 * @param nombre_archivo Nombre del archivo de destino
 * @param stats Puntero a estructura para almacenar estadísticas (opcional)
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool escribir_productos_basico(const Producto* productos, size_t num_productos, 
                              const char* nombre_archivo, EstadisticasEscritura* stats);

/**
 * @brief Escribe productos extendidos a un archivo binario
 * @param productos Array de productos extendidos a escribir
 * @param num_productos Número de productos en el array
 * @param nombre_archivo Nombre del archivo de destino
 * @param stats Puntero a estructura para almacenar estadísticas (opcional)
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool escribir_productos_extendido(const ProductoExtendido* productos, size_t num_productos,
                                 const char* nombre_archivo, EstadisticasEscritura* stats);

/**
 * @brief Escribe productos uno por uno a un archivo binario (modo append)
 * @param producto Puntero al producto a escribir
 * @param nombre_archivo Nombre del archivo de destino
 * @param es_primer_elemento Si es el primer elemento (crear archivo nuevo)
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool escribir_producto_individual(const Producto* producto, const char* nombre_archivo, 
                                 bool es_primer_elemento);

/**
 * @brief Añade un producto al final de un archivo binario existente
 * @param producto Puntero al producto a añadir
 * @param nombre_archivo Nombre del archivo de destino
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool anexar_producto(const Producto* producto, const char* nombre_archivo);

/**
 * @brief Escribe productos con validación de integridad
 * @param productos Array de productos a escribir
 * @param num_productos Número de productos en el array
 * @param nombre_archivo Nombre del archivo de destino
 * @param incluir_checksum Si incluir checksum para validación
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool escribir_productos_con_validacion(const Producto* productos, size_t num_productos,
                                       const char* nombre_archivo, bool incluir_checksum);

/**
 * @brief Escribe un header con metadatos antes de los productos
 * @param productos Array de productos a escribir
 * @param num_productos Número de productos en el array
 * @param nombre_archivo Nombre del archivo de destino
 * @return true si la operación fue exitosa, false en caso contrario
 */
bool escribir_productos_con_header(const Producto* productos, size_t num_productos,
                                   const char* nombre_archivo);

/**
 * @brief Crea un producto con valores por defecto
 * @param codigo Código del producto
 * @param precio Precio del producto
 * @return Producto inicializado
 */
Producto crear_producto(int codigo, float precio);

/**
 * @brief Crea un producto extendido con valores completos
 * @param codigo Código del producto
 * @param precio Precio del producto
 * @param nombre Nombre del producto
 * @param stock Cantidad en stock
 * @param categoria Categoría del producto
 * @param descuento Porcentaje de descuento
 * @return ProductoExtendido inicializado
 */
ProductoExtendido crear_producto_extendido(int codigo, float precio, const char* nombre,
                                          int stock, const char* categoria, double descuento);

/**
 * @brief Valida un producto antes de escribir
 * @param producto Puntero al producto a validar
 * @return true si el producto es válido, false en caso contrario
 */
bool validar_producto(const Producto* producto);

/**
 * @brief Imprime información de un producto
 * @param producto Puntero al producto a imprimir
 */
void imprimir_producto(const Producto* producto);

/**
 * @brief Imprime información de un producto extendido
 * @param producto Puntero al producto extendido a imprimir
 */
void imprimir_producto_extendido(const ProductoExtendido* producto);

/**
 * @brief Imprime estadísticas de escritura
 * @param stats Puntero a las estadísticas a imprimir
 */
void imprimir_estadisticas_escritura(const EstadisticasEscritura* stats);

/**
 * @brief Inicializa estructura de estadísticas
 * @param stats Puntero a estructura a inicializar
 */
void inicializar_estadisticas_escritura(EstadisticasEscritura* stats);

/**
 * @brief Calcula checksum simple de un array de productos
 * @param productos Array de productos
 * @param num_productos Número de productos
 * @return Checksum calculado
 */
unsigned int calcular_checksum_productos(const Producto* productos, size_t num_productos);

#endif // ESCRIBIR_REGISTROS_BINARIO_H
