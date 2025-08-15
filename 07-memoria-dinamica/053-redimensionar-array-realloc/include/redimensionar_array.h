#ifndef REDIMENSIONAR_ARRAY_H
#define REDIMENSIONAR_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

// Constantes para el ejercicio
#define BLOQUE_INICIAL 10
#define BLOQUE_CRECIMIENTO 10
#define VALOR_TERMINACION -1

// Estructura para gestionar el array redimensionable
typedef struct {
    int *datos;
    size_t cantidad;
    size_t capacidad;
    size_t bloque_tamaño;
} ArrayRedimensionable;

// Funciones principales del enunciado
ArrayRedimensionable* crearArrayRedimensionable(size_t bloque_inicial);
bool agregarElemento(ArrayRedimensionable *array, int valor);
void liberarArrayRedimensionable(ArrayRedimensionable *array);

// Funciones de gestión de memoria
bool redimensionarArray(ArrayRedimensionable *array, size_t nueva_capacidad);
bool necesitaRedimensionar(const ArrayRedimensionable *array);
size_t calcularNuevaCapacidad(const ArrayRedimensionable *array);

// Funciones de utilidad
void mostrarArray(const ArrayRedimensionable *array);
void mostrarEstadisticas(const ArrayRedimensionable *array);
bool validarArray(const ArrayRedimensionable *array);

// Funciones de entrada interactiva
int leerNumerosUsuario(ArrayRedimensionable *array);
void imprimirResultados(const ArrayRedimensionable *array);

// Funciones avanzadas
ArrayRedimensionable* copiarArray(const ArrayRedimensionable *array);
bool redimensionarPorPorcentaje(ArrayRedimensionable *array, double factor);
void compactarArray(ArrayRedimensionable *array);
bool insertarEnPosicion(ArrayRedimensionable *array, size_t posicion, int valor);
bool eliminarEnPosicion(ArrayRedimensionable *array, size_t posicion);

// Estadísticas y análisis
void analizarPatronCrecimiento(const ArrayRedimensionable *array);
double calcularFactorUso(const ArrayRedimensionable *array);
size_t calcularMemoriaUsada(const ArrayRedimensionable *array);
size_t calcularMemoriaTotal(const ArrayRedimensionable *array);

// Utilidades para testing
ArrayRedimensionable* crearArrayConDatos(const int *datos, size_t cantidad, size_t bloque);
bool compararArrays(const ArrayRedimensionable *array1, const ArrayRedimensionable *array2);

#endif // REDIMENSIONAR_ARRAY_H
