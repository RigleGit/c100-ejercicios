#ifndef ARRAY_DINAMICO_CADENAS_H
#define ARRAY_DINAMICO_CADENAS_H

#include <stdbool.h>

/**
 * @file array_dinamico_cadenas.h
 * @brief Biblioteca para manejo de arrays dinámicos de cadenas
 * 
 * Esta biblioteca proporciona una estructura de datos para manejar
 * colecciones dinámicas de cadenas de caracteres, con capacidad de
 * redimensionamiento automático y gestión eficiente de memoria.
 */

#define MAX_LONGITUD 100

/**
 * @brief Estructura para array dinámico de cadenas
 * 
 * Mantiene un array de punteros a cadenas con información de
 * tamaño actual y capacidad máxima.
 */
typedef struct {
    char **cadenas;     /**< Array de punteros a cadenas */
    int size;           /**< Número actual de cadenas */
    int capacidad;      /**< Capacidad máxima del array */
} ArrayCadenas;

/**
 * @brief Crea un nuevo array dinámico de cadenas
 * 
 * Reserva memoria para un array que puede contener hasta
 * 'capacidad' punteros a cadenas.
 * 
 * @param capacidad Capacidad inicial del array (debe ser > 0)
 * @return Puntero al array creado, o NULL si hay error
 * 
 * @note El llamador debe liberar la memoria con liberar_array_cadenas()
 */
ArrayCadenas* crear_array_cadenas(int capacidad);

/**
 * @brief Libera toda la memoria del array de cadenas
 * 
 * Libera cada cadena individual, el array de punteros y la estructura.
 * 
 * @param array Puntero al array a liberar
 * 
 * @note Es seguro pasar NULL como parámetro
 */
void liberar_array_cadenas(ArrayCadenas* array);

/**
 * @brief Lee nombres del usuario e los almacena en el array
 * 
 * Solicita al usuario que introduzca nombres hasta llenar
 * la capacidad del array. Cada nombre se almacena con la
 * cantidad exacta de memoria necesaria.
 * 
 * @param array Puntero al array donde almacenar los nombres
 * @return true si la operación fue exitosa, false en caso de error
 */
bool leer_nombres_usuario(ArrayCadenas* array);

/**
 * @brief Muestra todos los nombres del array
 * 
 * Imprime cada nombre con su índice correspondiente.
 * 
 * @param array Puntero al array a mostrar
 */
void mostrar_nombres(const ArrayCadenas* array);

/**
 * @brief Ordena los nombres alfabéticamente
 * 
 * Utiliza algoritmo de burbuja optimizado para ordenar
 * las cadenas en orden lexicográfico ascendente.
 * 
 * @param array Puntero al array a ordenar
 */
void ordenar_nombres(ArrayCadenas* array);

/**
 * @brief Busca un nombre específico en el array
 * 
 * Realiza búsqueda lineal del nombre especificado.
 * 
 * @param array Puntero al array donde buscar
 * @param nombre Nombre a buscar
 * @return Índice del nombre si se encuentra, -1 si no existe
 */
int buscar_nombre(const ArrayCadenas* array, const char* nombre);

/**
 * @brief Busca un nombre ignorando mayúsculas/minúsculas
 * 
 * @param array Puntero al array donde buscar
 * @param nombre Nombre a buscar
 * @return Índice del nombre si se encuentra, -1 si no existe
 */
int buscar_nombre_insensible(const ArrayCadenas* array, const char* nombre);

/**
 * @brief Agrega un nuevo nombre al array
 * 
 * Si es necesario, redimensiona automáticamente el array
 * para acomodar el nuevo elemento.
 * 
 * @param array Puntero al array
 * @param nombre Nombre a agregar
 * @return true si se agregó correctamente, false en caso de error
 */
bool agregar_nombre(ArrayCadenas* array, const char* nombre);

/**
 * @brief Elimina un nombre por su índice
 * 
 * Remueve el nombre en la posición especificada y compacta el array.
 * 
 * @param array Puntero al array
 * @param indice Índice del nombre a eliminar
 * @return true si se eliminó correctamente, false si el índice es inválido
 */
bool eliminar_nombre_por_indice(ArrayCadenas* array, int indice);

/**
 * @brief Muestra estadísticas del array
 * 
 * Calcula y muestra información como longitudes promedio,
 * nombres más corto/largo, uso de memoria, etc.
 * 
 * @param array Puntero al array a analizar
 */
void mostrar_estadisticas(const ArrayCadenas* array);

/**
 * @brief Filtra nombres por longitud
 * 
 * Crea un nuevo array con los nombres que tienen longitud
 * entre min_len y max_len (inclusive).
 * 
 * @param array Array original
 * @param min_len Longitud mínima
 * @param max_len Longitud máxima
 * @return Nuevo array con nombres filtrados, o NULL si hay error
 */
ArrayCadenas* filtrar_por_longitud(const ArrayCadenas* array, int min_len, int max_len);

/**
 * @brief Copia un array de cadenas
 * 
 * Crea una copia independiente del array especificado.
 * 
 * @param array Array a copiar
 * @return Nuevo array copiado, o NULL si hay error
 */
ArrayCadenas* copiar_array_cadenas(const ArrayCadenas* array);

/**
 * @brief Crea array básico con nombres predefinidos (para tests)
 * 
 * Versión simplificada que crea un array e inicializa con
 * nombres de un array bidimensional.
 * 
 * @param n Número de nombres
 * @param nombres Array de nombres a copiar
 * @return Nuevo array inicializado, o NULL si hay error
 */
ArrayCadenas* crear_array_cadenas_basico(int n, char nombres[][MAX_LONGITUD]);

/**
 * @brief Obtiene un nombre por su índice
 * 
 * @param array Puntero al array
 * @param indice Índice del nombre a obtener
 * @return Puntero al nombre, o NULL si el índice es inválido
 */
char* obtener_nombre(const ArrayCadenas* array, int indice);

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

#endif // ARRAY_DINAMICO_CADENAS_H
