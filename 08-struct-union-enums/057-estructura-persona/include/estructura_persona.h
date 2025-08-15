#ifndef ESTRUCTURA_PERSONA_H
#define ESTRUCTURA_PERSONA_H

#include <stdbool.h>

/**
 * @file estructura_persona.h
 * @brief Biblioteca para manejo de estructuras Persona
 * 
 * Esta biblioteca demuestra el uso de estructuras (struct) en C para
 * modelar entidades del mundo real. Incluye definición de tipos con
 * typedef y operaciones comunes sobre estructuras.
 */

#define MAX_NOMBRE 100

/**
 * @brief Estructura que representa a una persona
 * 
 * Agrupa información relacionada sobre una persona en una sola entidad.
 * Utiliza typedef para simplificar la declaración de variables.
 */
typedef struct {
    char nombre[MAX_NOMBRE];    /**< Nombre completo de la persona */
    int edad;                   /**< Edad en años */
} Persona;

/**
 * @brief Crea una nueva persona con los valores especificados
 * 
 * @param nombre Nombre de la persona (se copia de forma segura)
 * @param edad Edad de la persona
 * @return Estructura Persona inicializada
 * 
 * @note El nombre se trunca si excede MAX_NOMBRE-1 caracteres
 */
Persona crear_persona(const char *nombre, int edad);

/**
 * @brief Lee los datos de una persona desde la entrada estándar
 * 
 * Solicita al usuario que introduzca nombre y edad.
 * 
 * @return Estructura Persona con los datos introducidos
 */
Persona leer_persona_usuario(void);

/**
 * @brief Muestra los datos de una persona en formato simple
 * 
 * @param p Puntero a la persona a mostrar
 */
void mostrar_persona(const Persona *p);

/**
 * @brief Muestra una persona en una línea compacta
 * 
 * @param p Puntero a la persona a mostrar
 */
void mostrar_persona_simple(const Persona *p);

/**
 * @brief Muestra los datos de una persona en formato detallado
 * 
 * Incluye información adicional como categoría de edad y validación.
 * 
 * @param p Puntero a la persona a mostrar
 */
void mostrar_persona_detallada(const Persona *p);

/**
 * @brief Valida que los datos de una persona sean correctos
 * 
 * Verifica que el nombre no esté vacío y que la edad esté en rango válido.
 * 
 * @param p Puntero a la persona a validar
 * @return true si los datos son válidos, false en caso contrario
 */
bool validar_persona(const Persona *p);

/**
 * @brief Comprueba si dos personas tienen la misma edad
 * 
 * @param p1 Primera persona
 * @param p2 Segunda persona
 * @return true si tienen la misma edad, false en caso contrario
 */
bool misma_edad(const Persona *p1, const Persona *p2);

/**
 * @brief Determina cuál de dos personas es mayor
 * 
 * @param p1 Primera persona
 * @param p2 Segunda persona
 * @return Puntero a la persona de mayor edad
 */
Persona* persona_mayor(Persona *p1, Persona *p2);

/**
 * @brief Copia los datos de una persona a otra
 * 
 * @param destino Persona donde copiar los datos
 * @param origen Persona de la cual copiar los datos
 */
void copiar_persona(Persona *destino, const Persona *origen);

/**
 * @brief Obtiene la categoría de edad como cadena
 * 
 * @param edad Edad a categorizar
 * @return Cadena con la categoría correspondiente
 */
const char* obtener_categoria_edad(int edad);

// Funciones para trabajar con arrays de personas

/**
 * @brief Muestra una lista de personas numerada
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 */
void mostrar_lista_personas(const Persona personas[], int cantidad);

/**
 * @brief Encuentra la persona de mayor edad en un array
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @return Puntero a la persona de mayor edad, o NULL si el array está vacío
 */
Persona* encontrar_mayor(Persona personas[], int cantidad);

/**
 * @brief Encuentra la persona de menor edad en un array
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @return Puntero a la persona de menor edad, o NULL si el array está vacío
 */
Persona* encontrar_menor(Persona personas[], int cantidad);

/**
 * @brief Calcula la edad promedio de un grupo de personas
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @return Edad promedio como número decimal
 */
double edad_promedio(const Persona personas[], int cantidad);

/**
 * @brief Busca una persona por nombre exacto
 * 
 * @param personas Array de personas donde buscar
 * @param cantidad Número de personas en el array
 * @param nombre Nombre a buscar
 * @return Índice de la persona si se encuentra, -1 en caso contrario
 */
int buscar_por_nombre(const Persona personas[], int cantidad, const char *nombre);

/**
 * @brief Busca una persona por nombre (insensible a mayúsculas/minúsculas)
 * 
 * @param personas Array de personas donde buscar
 * @param cantidad Número de personas en el array
 * @param nombre Nombre a buscar
 * @return Índice de la persona si se encuentra, -1 en caso contrario
 */
int buscar_por_nombre_insensible(const Persona personas[], int cantidad, const char *nombre);

/**
 * @brief Ordena un array de personas por edad (menor a mayor)
 * 
 * Utiliza algoritmo de burbuja optimizado.
 * 
 * @param personas Array de personas a ordenar
 * @param cantidad Número de personas en el array
 */
void ordenar_por_edad(Persona personas[], int cantidad);

/**
 * @brief Ordena un array de personas por nombre alfabéticamente
 * 
 * @param personas Array de personas a ordenar
 * @param cantidad Número de personas en el array
 */
void ordenar_por_nombre(Persona personas[], int cantidad);

/**
 * @brief Copia un array de personas a otro
 * 
 * @param destino Array destino (debe tener suficiente espacio)
 * @param origen Array origen
 * @param cantidad Número de personas a copiar
 */
void copiar_array_personas(Persona destino[], const Persona origen[], int cantidad);

/**
 * @brief Cuenta personas en un rango de edad específico
 * 
 * @param personas Array de personas
 * @param cantidad Número de personas en el array
 * @param edad_min Edad mínima (inclusive)
 * @param edad_max Edad máxima (inclusive)
 * @return Número de personas en el rango especificado
 */
int contar_en_rango_edad(const Persona personas[], int cantidad, int edad_min, int edad_max);

/**
 * @brief Verifica si todas las personas en un array son válidas
 * 
 * @param personas Array de personas a verificar
 * @param cantidad Número de personas en el array
 * @return true si todas son válidas, false en caso contrario
 */
bool todas_validas(const Persona personas[], int cantidad);

#ifndef HAVE_STRCASECMP
/**
 * @brief Comparación de cadenas insensible a mayúsculas/minúsculas
 * 
 * Implementación propia para sistemas que no tienen strcasecmp.
 * 
 * @param s1 Primera cadena
 * @param s2 Segunda cadena
 * @return 0 si son iguales, <0 si s1<s2, >0 si s1>s2
 */
int strcasecmp(const char *s1, const char *s2);
#endif

#endif // ESTRUCTURA_PERSONA_H
