#ifndef ESTRUCTURA_DINAMICA_H
#define ESTRUCTURA_DINAMICA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Constantes para límites y configuración
#define MAX_NOMBRE 50
#define MAX_EMAIL 100
#define CAPACIDAD_INICIAL 4
#define FACTOR_CRECIMIENTO 2

/**
 * @brief Estructura que representa una persona con información básica
 */
typedef struct {
    char *nombre;        // Nombre dinámico
    char *email;         // Email dinámico
    int edad;            // Edad de la persona
    float salario;       // Salario de la persona
} Persona;

/**
 * @brief Estructura para manejar un array dinámico de personas
 */
typedef struct {
    Persona *personas;   // Array dinámico de personas
    size_t size;         // Número actual de personas
    size_t capacity;     // Capacidad máxima actual
} ArrayPersonas;

/**
 * @brief Estadísticas sobre el array de personas
 */
typedef struct {
    float edad_promedio;
    float salario_promedio;
    int edad_minima;
    int edad_maxima;
    float salario_minimo;
    float salario_maximo;
    size_t total_personas;
} EstadisticasPersonas;

// Funciones para gestión de Persona individual
Persona* crear_persona(const char *nombre, const char *email, int edad, float salario);
void liberar_persona(Persona *persona);
bool es_persona_valida(const Persona *persona);
void imprimir_persona(const Persona *persona);
Persona* copiar_persona(const Persona *original);
bool actualizar_nombre_persona(Persona *persona, const char *nuevo_nombre);
bool actualizar_email_persona(Persona *persona, const char *nuevo_email);

// Funciones para gestión de ArrayPersonas
ArrayPersonas* crear_array_personas(void);
void liberar_array_personas(ArrayPersonas *array);
bool agregar_persona(ArrayPersonas *array, const char *nombre, const char *email, int edad, float salario);
bool eliminar_persona_por_indice(ArrayPersonas *array, size_t indice);
bool eliminar_persona_por_nombre(ArrayPersonas *array, const char *nombre);
void imprimir_array_personas(const ArrayPersonas *array);
size_t buscar_persona_por_nombre(const ArrayPersonas *array, const char *nombre);
ArrayPersonas* filtrar_por_edad(const ArrayPersonas *array, int edad_min, int edad_max);
ArrayPersonas* filtrar_por_salario(const ArrayPersonas *array, float salario_min, float salario_max);

// Funciones de utilidad y estadísticas
bool redimensionar_array(ArrayPersonas *array, size_t nueva_capacidad);
EstadisticasPersonas calcular_estadisticas(const ArrayPersonas *array);
void imprimir_estadisticas(const EstadisticasPersonas *stats);
bool exportar_a_csv(const ArrayPersonas *array, const char *nombre_archivo);
ArrayPersonas* importar_desde_csv(const char *nombre_archivo);

// Funciones de ordenamiento
void ordenar_por_nombre(ArrayPersonas *array);
void ordenar_por_edad(ArrayPersonas *array);
void ordenar_por_salario(ArrayPersonas *array);

// Funciones auxiliares para memoria
size_t obtener_memoria_utilizada(const ArrayPersonas *array);
void mostrar_informacion_memoria(const ArrayPersonas *array);

// Función para demostración interactiva
void demo_estructura_dinamica(void);

#endif // ESTRUCTURA_DINAMICA_H
