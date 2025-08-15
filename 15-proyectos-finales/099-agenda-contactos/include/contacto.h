#ifndef CONTACTO_H
#define CONTACTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Constantes
#define TAM_NOMBRE 100
#define TAM_TELEFONO 20
#define TAM_EMAIL 100
#define TAM_DIRECCION 200
#define TAM_BUFFER 512

// Enumeraciones
typedef enum {
    RESULTADO_EXITO = 0,
    RESULTADO_ERROR_MEMORIA = 1,
    RESULTADO_CONTACTO_NO_ENCONTRADO = 2,
    RESULTADO_CONTACTO_DUPLICADO = 3,
    RESULTADO_PARAMETRO_INVALIDO = 4,
    RESULTADO_LISTA_VACIA = 5
} resultado_t;

typedef enum {
    CATEGORIA_PERSONAL = 0,
    CATEGORIA_TRABAJO = 1,
    CATEGORIA_FAMILIA = 2,
    CATEGORIA_AMIGOS = 3,
    CATEGORIA_OTROS = 4
} categoria_t;

// Estructura principal del contacto
typedef struct {
    char nombre[TAM_NOMBRE];
    char telefono[TAM_TELEFONO];
    char email[TAM_EMAIL];
    char direccion[TAM_DIRECCION];
    categoria_t categoria;
    time_t fecha_creacion;
    time_t fecha_modificacion;
    bool favorito;
    int id;  // ID único
} contacto_t;

// Nodo de la lista enlazada
typedef struct nodo {
    contacto_t contacto;
    struct nodo *siguiente;
} nodo_t;

// Estructura para estadísticas
typedef struct {
    int total_contactos;
    int contactos_por_categoria[5];
    int contactos_favoritos;
    double tiempo_busqueda_promedio;
} estadisticas_t;

// ========================================
// FUNCIONES DE GESTIÓN DE CONTACTOS
// ========================================

/**
 * @brief Crea un nuevo contacto con validación de datos
 * @param nombre Nombre del contacto
 * @param telefono Teléfono del contacto
 * @param email Email del contacto
 * @param direccion Dirección del contacto (opcional)
 * @param categoria Categoría del contacto
 * @return Contacto inicializado
 */
contacto_t crear_contacto(const char *nombre, const char *telefono, 
                         const char *email, const char *direccion, 
                         categoria_t categoria);

/**
 * @brief Valida los datos de un contacto
 * @param contacto Contacto a validar
 * @return true si es válido, false en caso contrario
 */
bool validar_contacto(const contacto_t *contacto);

/**
 * @brief Valida formato de email
 * @param email Email a validar
 * @return true si es válido, false en caso contrario
 */
bool validar_email(const char *email);

/**
 * @brief Valida formato de teléfono
 * @param telefono Teléfono a validar
 * @return true si es válido, false en caso contrario
 */
bool validar_telefono(const char *telefono);

/**
 * @brief Imprime un contacto de forma formateada
 * @param contacto Contacto a imprimir
 * @param mostrar_detalle Si mostrar información detallada
 */
void imprimir_contacto(const contacto_t *contacto, bool mostrar_detalle);

/**
 * @brief Convierte categoría a string
 * @param categoria Categoría a convertir
 * @return String con el nombre de la categoría
 */
const char *categoria_a_string(categoria_t categoria);

/**
 * @brief Convierte string a categoría
 * @param str String a convertir
 * @return Categoría correspondiente
 */
categoria_t string_a_categoria(const char *str);

// ========================================
// FUNCIONES DE LISTA ENLAZADA
// ========================================

/**
 * @brief Agrega un contacto a la lista (manteniendo orden alfabético)
 * @param cabeza Puntero a la cabeza de la lista
 * @param contacto Contacto a agregar
 * @return Código de resultado
 */
resultado_t agregar_contacto(nodo_t **cabeza, const contacto_t *contacto);

/**
 * @brief Busca un contacto por nombre (búsqueda exacta)
 * @param cabeza Cabeza de la lista
 * @param nombre Nombre a buscar
 * @return Puntero al contacto encontrado o NULL
 */
const contacto_t *buscar_contacto_por_nombre(const nodo_t *cabeza, const char *nombre);

/**
 * @brief Busca contactos por nombre parcial
 * @param cabeza Cabeza de la lista
 * @param nombre_parcial Parte del nombre a buscar
 * @param resultados Array para almacenar resultados (debe ser liberado)
 * @param max_resultados Número máximo de resultados
 * @return Número de contactos encontrados
 */
int buscar_contactos_por_nombre_parcial(const nodo_t *cabeza, const char *nombre_parcial,
                                       contacto_t **resultados, int max_resultados);

/**
 * @brief Busca contactos por categoría
 * @param cabeza Cabeza de la lista
 * @param categoria Categoría a buscar
 * @param resultados Array para almacenar resultados
 * @param max_resultados Número máximo de resultados
 * @return Número de contactos encontrados
 */
int buscar_contactos_por_categoria(const nodo_t *cabeza, categoria_t categoria,
                                  contacto_t **resultados, int max_resultados);

/**
 * @brief Busca contacto por ID
 * @param cabeza Cabeza de la lista
 * @param id ID a buscar
 * @return Puntero al contacto encontrado o NULL
 */
const contacto_t *buscar_contacto_por_id(const nodo_t *cabeza, int id);

/**
 * @brief Elimina un contacto por nombre
 * @param cabeza Puntero a la cabeza de la lista
 * @param nombre Nombre del contacto a eliminar
 * @return Código de resultado
 */
resultado_t eliminar_contacto(nodo_t **cabeza, const char *nombre);

/**
 * @brief Modifica un contacto existente
 * @param cabeza Cabeza de la lista
 * @param nombre_original Nombre del contacto a modificar
 * @param contacto_nuevo Nuevos datos del contacto
 * @return Código de resultado
 */
resultado_t modificar_contacto(nodo_t *cabeza, const char *nombre_original,
                              const contacto_t *contacto_nuevo);

/**
 * @brief Lista todos los contactos
 * @param cabeza Cabeza de la lista
 * @param mostrar_detalle Si mostrar información detallada
 */
void listar_contactos(const nodo_t *cabeza, bool mostrar_detalle);

/**
 * @brief Lista contactos favoritos
 * @param cabeza Cabeza de la lista
 */
void listar_contactos_favoritos(const nodo_t *cabeza);

/**
 * @brief Marca/desmarca un contacto como favorito
 * @param cabeza Cabeza de la lista
 * @param nombre Nombre del contacto
 * @param favorito true para marcar como favorito
 * @return Código de resultado
 */
resultado_t marcar_favorito(nodo_t *cabeza, const char *nombre, bool favorito);

/**
 * @brief Cuenta el número total de contactos
 * @param cabeza Cabeza de la lista
 * @return Número de contactos
 */
int contar_contactos(const nodo_t *cabeza);

/**
 * @brief Libera toda la memoria de la lista
 * @param cabeza Puntero a la cabeza de la lista
 */
void liberar_lista(nodo_t **cabeza);

/**
 * @brief Obtiene estadísticas de la agenda
 * @param cabeza Cabeza de la lista
 * @return Estructura con estadísticas
 */
estadisticas_t obtener_estadisticas(const nodo_t *cabeza);

/**
 * @brief Imprime estadísticas de la agenda
 * @param stats Estadísticas a imprimir
 */
void imprimir_estadisticas(const estadisticas_t *stats);

/**
 * @brief Genera un ID único para un nuevo contacto
 * @param cabeza Cabeza de la lista
 * @return ID único
 */
int generar_id_unico(const nodo_t *cabeza);

/**
 * @brief Convierte resultado a string descriptivo
 * @param resultado Código de resultado
 * @return String descriptivo del resultado
 */
const char *resultado_a_string(resultado_t resultado);

#endif // CONTACTO_H
