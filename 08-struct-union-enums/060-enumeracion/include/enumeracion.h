#ifndef ENUMERACION_H
#define ENUMERACION_H

#include <stdbool.h>

/**
 * @file enumeracion.h
 * @brief Biblioteca para trabajar con enumeraciones en C
 * 
 * Esta biblioteca demuestra el uso de enumeraciones para representar
 * constantes simbólicas, mejorar la legibilidad del código y trabajar
 * con conjuntos finitos de valores bien definidos.
 */

/**
 * @brief Enumeración principal del enunciado: días de la semana
 * 
 * Define los días de la semana empezando desde 1 (lunes) hasta 7 (domingo).
 * Esto facilita la conversión directa desde input del usuario.
 */
enum DiaSemana { 
    LUN = 1,    /**< Lunes (1) */
    MAR,        /**< Martes (2) */
    MIE,        /**< Miércoles (3) */
    JUE,        /**< Jueves (4) */
    VIE,        /**< Viernes (5) */
    SAB,        /**< Sábado (6) */
    DOM         /**< Domingo (7) */
};

/**
 * @brief Enumeración para meses del año
 * 
 * Ejemplo de enumeración más extensa para demostrar conceptos avanzados.
 */
enum Mes {
    ENERO = 1, FEBRERO, MARZO, ABRIL, MAYO, JUNIO,
    JULIO, AGOSTO, SEPTIEMBRE, OCTUBRE, NOVIEMBRE, DICIEMBRE
};

/**
 * @brief Enumeración para estaciones del año
 */
enum Estacion {
    PRIMAVERA,  /**< Primavera (0) */
    VERANO,     /**< Verano (1) */
    OTONO,      /**< Otoño (2) */
    INVIERNO    /**< Invierno (3) */
};

/**
 * @brief Enumeración para estados de una máquina
 * 
 * Demuestra el uso de enums para máquinas de estado.
 */
enum EstadoMaquina {
    APAGADO = 0,
    INICIANDO = 10,
    FUNCIONANDO = 20,
    PAUSADO = 30,
    ERROR = 99
};

/**
 * @brief Enumeración para niveles de prioridad
 */
enum Prioridad {
    BAJA = 1,
    MEDIA = 5,
    ALTA = 10,
    CRITICA = 20
};

/**
 * @brief Enumeración para códigos de error
 */
enum CodigoError {
    SIN_ERROR = 0,
    ERROR_ENTRADA = 1,
    ERROR_MEMORIA = 2,
    ERROR_ARCHIVO = 3,
    ERROR_RED = 4,
    ERROR_DESCONOCIDO = 99
};

// Funciones principales del enunciado

/**
 * @brief Convierte un día de la semana a su nombre en español (función del enunciado)
 * 
 * @param dia Día de la semana (enum DiaSemana)
 * @return const char* Nombre del día en español
 */
const char* nombreDia(enum DiaSemana dia);

/**
 * @brief Convierte un número (1-7) a día de la semana
 * 
 * @param numero Número del 1 al 7
 * @return enum DiaSemana Día correspondiente, o valor inválido si fuera de rango
 */
enum DiaSemana numeroDia(int numero);

/**
 * @brief Valida si un número representa un día válido
 * 
 * @param numero Número a validar
 * @return true si es un día válido (1-7), false en caso contrario
 */
bool esDiaValido(int numero);

// Funciones extendidas para días

/**
 * @brief Obtiene el siguiente día de la semana
 * 
 * @param dia Día actual
 * @return enum DiaSemana Día siguiente (domingo -> lunes)
 */
enum DiaSemana diaSiguiente(enum DiaSemana dia);

/**
 * @brief Obtiene el día anterior de la semana
 * 
 * @param dia Día actual
 * @return enum DiaSemana Día anterior (lunes -> domingo)
 */
enum DiaSemana diaAnterior(enum DiaSemana dia);

/**
 * @brief Verifica si un día es fin de semana
 * 
 * @param dia Día a verificar
 * @return true si es sábado o domingo, false en caso contrario
 */
bool esFinDeSemana(enum DiaSemana dia);

/**
 * @brief Verifica si un día es día laboral
 * 
 * @param dia Día a verificar
 * @return true si es de lunes a viernes, false en caso contrario
 */
bool esDiaLaboral(enum DiaSemana dia);

/**
 * @brief Obtiene el nombre del día en formato abreviado
 * 
 * @param dia Día de la semana
 * @return const char* Nombre abreviado (Lun, Mar, etc.)
 */
const char* nombreDiaAbreviado(enum DiaSemana dia);

/**
 * @brief Obtiene el nombre del día en inglés
 * 
 * @param dia Día de la semana
 * @return const char* Nombre en inglés
 */
const char* nombreDiaIngles(enum DiaSemana dia);

// Funciones para meses

/**
 * @brief Convierte un mes a su nombre en español
 * 
 * @param mes Mes del año (enum Mes)
 * @return const char* Nombre del mes en español
 */
const char* nombreMes(enum Mes mes);

/**
 * @brief Obtiene el número de días de un mes
 * 
 * @param mes Mes del año
 * @param esBisiesto true si el año es bisiesto
 * @return int Número de días del mes
 */
int diasDelMes(enum Mes mes, bool esBisiesto);

/**
 * @brief Obtiene la estación correspondiente a un mes
 * 
 * @param mes Mes del año
 * @return enum Estacion Estación correspondiente (hemisferio norte)
 */
enum Estacion estacionDelMes(enum Mes mes);

// Funciones para estaciones

/**
 * @brief Convierte una estación a su nombre en español
 * 
 * @param estacion Estación del año
 * @return const char* Nombre de la estación
 */
const char* nombreEstacion(enum Estacion estacion);

/**
 * @brief Obtiene la siguiente estación
 * 
 * @param estacion Estación actual
 * @return enum Estacion Siguiente estación
 */
enum Estacion estacionSiguiente(enum Estacion estacion);

// Funciones para estados de máquina

/**
 * @brief Convierte un estado de máquina a su descripción
 * 
 * @param estado Estado de la máquina
 * @return const char* Descripción del estado
 */
const char* descripcionEstado(enum EstadoMaquina estado);

/**
 * @brief Verifica si un estado permite transiciones
 * 
 * @param estado Estado a verificar
 * @return true si permite transiciones, false en caso contrario
 */
bool estadoPermiteTransicion(enum EstadoMaquina estado);

/**
 * @brief Obtiene el siguiente estado válido de una máquina
 * 
 * @param estadoActual Estado actual de la máquina
 * @return enum EstadoMaquina Siguiente estado válido
 */
enum EstadoMaquina siguienteEstado(enum EstadoMaquina estadoActual);

// Funciones para prioridades

/**
 * @brief Convierte una prioridad a su descripción
 * 
 * @param prioridad Nivel de prioridad
 * @return const char* Descripción de la prioridad
 */
const char* descripcionPrioridad(enum Prioridad prioridad);

/**
 * @brief Compara dos prioridades
 * 
 * @param p1 Primera prioridad
 * @param p2 Segunda prioridad
 * @return int < 0 si p1 < p2, 0 si iguales, > 0 si p1 > p2
 */
int compararPrioridades(enum Prioridad p1, enum Prioridad p2);

// Funciones para códigos de error

/**
 * @brief Convierte un código de error a su descripción
 * 
 * @param codigo Código de error
 * @return const char* Descripción del error
 */
const char* descripcionError(enum CodigoError codigo);

/**
 * @brief Verifica si un código representa un error
 * 
 * @param codigo Código a verificar
 * @return true si es un error, false si es SIN_ERROR
 */
bool esError(enum CodigoError codigo);

// Funciones de demostración y utilidad

/**
 * @brief Imprime todos los días de la semana con sus valores
 */
void mostrarTodosDiasSemana(void);

/**
 * @brief Imprime todos los meses del año con información
 */
void mostrarTodosMeses(void);

/**
 * @brief Demuestra operaciones con días de la semana
 * 
 * @param dia Día para la demostración
 */
void demostracionDiasSemana(enum DiaSemana dia);

/**
 * @brief Demuestra el uso de máquinas de estado
 */
void demostracionMaquinaEstados(void);

/**
 * @brief Convierte un string a día de la semana
 * 
 * @param nombre Nombre del día (español o abreviado)
 * @return enum DiaSemana Día correspondiente, o valor inválido
 */
enum DiaSemana stringADia(const char* nombre);

/**
 * @brief Cuenta los días laborales entre dos días (inclusive)
 * 
 * @param inicio Día de inicio
 * @param fin Día de fin
 * @return int Número de días laborales
 */
int contarDiasLaborales(enum DiaSemana inicio, enum DiaSemana fin);

/**
 * @brief Programa interactivo para explorar enumeraciones
 */
void programaInteractivoEnum(void);

#endif // ENUMERACION_H
