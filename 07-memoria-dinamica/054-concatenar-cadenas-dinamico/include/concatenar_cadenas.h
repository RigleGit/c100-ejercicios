#ifndef CONCATENAR_CADENAS_H
#define CONCATENAR_CADENAS_H

#include <stddef.h>
#include <stdbool.h>

// Constantes para el ejercicio
#define MAX_ENTRADA 1000
#define BUFFER_INICIAL 100

// Estructura para gestionar cadenas dinámicas
typedef struct {
    char *contenido;
    size_t longitud;
    size_t capacidad;
} CadenaDinamica;

// Funciones principales del enunciado
char* concatenarCadenas(const char *cadena1, const char *cadena2);
char* leerCadenaUsuario(const char *prompt);
void liberarCadena(char *cadena);

// Funciones de gestión de cadenas dinámicas
CadenaDinamica* crearCadenaDinamica(size_t capacidad_inicial);
void liberarCadenaDinamica(CadenaDinamica *cadena);
bool asignarContenido(CadenaDinamica *cadena, const char *contenido);
bool concatenarACadena(CadenaDinamica *cadena, const char *sufijo);

// Operaciones de concatenación avanzadas
char* concatenarMultiples(const char **cadenas, size_t cantidad);
char* concatenarConSeparador(const char *cadena1, const char *cadena2, const char *separador);
char* concatenarConFormato(const char *formato, ...);

// Funciones de utilidad
size_t calcularLongitudTotal(const char *cadena1, const char *cadena2);
bool validarCadena(const char *cadena);
char* duplicarCadena(const char *cadena);
char* subcadena(const char *cadena, size_t inicio, size_t longitud);

// Operaciones de manipulación
char* invertirCadena(const char *cadena);
char* convertirAMayusculas(const char *cadena);
char* convertirAMinusculas(const char *cadena);
char* eliminarEspacios(const char *cadena);

// Funciones de búsqueda y análisis
int buscarSubcadena(const char *cadena, const char *subcadena);
size_t contarOcurrencias(const char *cadena, char caracter);
bool empiezaCon(const char *cadena, const char *prefijo);
bool terminaCon(const char *cadena, const char *sufijo);

// Utilidades para entrada/salida
char* leerCadenaSegura(const char *prompt, size_t max_longitud);
void imprimirCadenaConInfo(const char *cadena, const char *etiqueta);
void mostrarEstadisticasCadena(const char *cadena);

// Funciones para testing
char* crearCadenaAleatoria(size_t longitud);
bool compararCadenas(const char *cadena1, const char *cadena2);
void pruebaRendimiento(size_t tamaño_cadena);

// Gestión de errores
typedef enum {
    CADENA_OK,
    CADENA_ERROR_MEMORIA,
    CADENA_ERROR_PARAMETRO_NULO,
    CADENA_ERROR_LONGITUD_INVALIDA,
    CADENA_ERROR_ENTRADA
} ErrorCadena;

const char* obtenerMensajeError(ErrorCadena error);

#endif // CONCATENAR_CADENAS_H
