#ifndef ARRAY_ESTRUCTURAS_H
#define ARRAY_ESTRUCTURAS_H

#include <stdbool.h>

/**
 * @file array_estructuras.h
 * @brief Biblioteca para manejo de arrays de estructuras Persona
 * 
 * Esta biblioteca demuestra el uso de arrays de estructuras en C,
 * incluyendo operaciones de filtrado, búsqueda, ordenamiento y
 * estadísticas sobre colecciones de datos estructurados.
 */

#define MAX_NOMBRE 100
#define MAX_PERSONAS 5

/**
 * @brief Estructura que representa a una persona
 * 
 * Misma definición que en el ejercicio anterior, manteniendo
 * consistencia en el modelado de datos.
 */
typedef struct {
    char nombre[MAX_NOMBRE];    /**< Nombre completo de la persona */
    int edad;                   /**< Edad en años */
} Persona;

/**
 * @brief Estructura que contiene una lista de personas
 * 
 * Encapsula un array de personas junto con información
 * sobre la cantidad actual de elementos.
 */
typedef struct {
    Persona personas[MAX_PERSONAS]; /**< Array de personas */
    int cantidad;                    /**< Número actual de personas */
} ListaPersonas;

/**
 * @brief Inicializa una lista de personas vacía
 * 
 * @param lista Puntero a la lista a inicializar
 */
void inicializar_lista(ListaPersonas *lista);

/**
 * @brief Lee datos de personas desde la entrada estándar
 * 
 * Solicita al usuario que introduzca nombre y edad para
 * MAX_PERSONAS personas.
 * 
 * @param lista Lista donde almacenar las personas
 * @return true si la operación fue exitosa, false en caso de error
 */
bool leer_personas_usuario(ListaPersonas *lista);

/**
 * @brief Imprime personas mayores de cierta edad (función principal del enunciado)
 * 
 * Esta es la función principal del ejercicio: filtrar e imprimir
 * personas que superen un umbral de edad específico.
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param edad_limite Edad mínima para incluir en el resultado
 */
void imprimir_mayores_de(const Persona personas[], int cantidad, int edad_limite);

/**
 * @brief Muestra toda la lista de personas
 * 
 * @param lista Lista de personas a mostrar
 */
void mostrar_lista_completa(const ListaPersonas *lista);

/**
 * @brief Muestra lista numerada para selección
 * 
 * @param lista Lista de personas a mostrar
 */
void mostrar_lista_numerada(const ListaPersonas *lista);

/**
 * @brief Imprime personas en un rango de edad específico
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param edad_min Edad mínima (inclusive)
 * @param edad_max Edad máxima (inclusive)
 */
void imprimir_rango_edad(const Persona personas[], int cantidad, int edad_min, int edad_max);

/**
 * @brief Busca una persona por nombre exacto
 * 
 * @param lista Lista donde buscar
 * @param nombre Nombre a buscar
 * @return Índice de la persona si se encuentra, -1 en caso contrario
 */
int buscar_persona_por_nombre(const ListaPersonas *lista, const char *nombre);

/**
 * @brief Busca e imprime información de una persona
 * 
 * @param lista Lista donde buscar
 * @param nombre Nombre de la persona a buscar
 */
void buscar_e_imprimir_persona(const ListaPersonas *lista, const char *nombre);

/**
 * @brief Ordena la lista de personas por edad (menor a mayor)
 * 
 * Utiliza algoritmo de burbuja optimizado.
 * 
 * @param lista Lista a ordenar
 */
void ordenar_por_edad(ListaPersonas *lista);

/**
 * @brief Ordena la lista de personas por nombre alfabéticamente
 * 
 * @param lista Lista a ordenar
 */
void ordenar_por_nombre(ListaPersonas *lista);

/**
 * @brief Agrega una nueva persona a la lista
 * 
 * @param lista Lista donde agregar la persona
 * @param persona Persona a agregar
 * @return true si se agregó correctamente, false si la lista está llena o hay error
 */
bool agregar_persona(ListaPersonas *lista, const Persona *persona);

/**
 * @brief Elimina una persona de la lista por índice
 * 
 * @param lista Lista de la cual eliminar
 * @param indice Índice de la persona a eliminar
 * @return true si se eliminó correctamente, false si el índice es inválido
 */
bool eliminar_persona(ListaPersonas *lista, int indice);

/**
 * @brief Muestra estadísticas detalladas de la lista
 * 
 * Calcula y muestra información como promedios, rangos,
 * distribuciones por edad, etc.
 * 
 * @param lista Lista a analizar
 */
void mostrar_estadisticas(const ListaPersonas *lista);

// Funciones de filtrado avanzado

/**
 * @brief Filtra personas cuyo nombre empiece por una letra específica
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param letra Letra inicial a buscar (case-insensitive)
 */
void filtrar_por_inicial(const Persona personas[], int cantidad, char letra);

/**
 * @brief Filtra personas en una década específica de edad
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param decada Década a filtrar (ej: 20 para 20-29, 30 para 30-39)
 */
void filtrar_por_decada(const Persona personas[], int cantidad, int decada);

/**
 * @brief Filtra personas con nombres de longitud específica
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param longitud Longitud exacta del nombre en caracteres
 */
void filtrar_por_longitud_nombre(const Persona personas[], int cantidad, int longitud);

/**
 * @brief Cuenta personas en una década específica
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param decada Década a contar
 * @return Número de personas en esa década
 */
int contar_en_decada(const Persona personas[], int cantidad, int decada);

/**
 * @brief Carga datos de ejemplo para pruebas
 * 
 * Llena la lista con personas de ejemplo para demostrar
 * las funcionalidades sin requerir entrada manual.
 * 
 * @param lista Lista donde cargar los datos
 */
void cargar_datos_ejemplo(ListaPersonas *lista);

/**
 * @brief Valida que una lista de personas sea correcta
 * 
 * @param lista Lista a validar
 * @return true si todos los datos son válidos, false en caso contrario
 */
bool validar_lista(const ListaPersonas *lista);

/**
 * @brief Copia una lista de personas a otra
 * 
 * @param destino Lista destino (debe tener suficiente espacio)
 * @param origen Lista origen
 * @return true si la copia fue exitosa, false en caso de error
 */
bool copiar_lista(ListaPersonas *destino, const ListaPersonas *origen);

#endif // ARRAY_ESTRUCTURAS_H
