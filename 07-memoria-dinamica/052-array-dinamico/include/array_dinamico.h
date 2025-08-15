#ifndef ARRAY_DINAMICO_H
#define ARRAY_DINAMICO_H

#include <stddef.h>
#include <stdbool.h>

// Estructura para array dinámico
typedef struct {
    int *datos;
    size_t tamaño;
    size_t capacidad;
} ArrayDinamico;

// Función principal del enunciado
ArrayDinamico* crearArrayDinamico(size_t tamaño_inicial);
void liberarArrayDinamico(ArrayDinamico *array);

// Funciones básicas de gestión
void inicializarConValor(ArrayDinamico *array, int valor);
void mostrarArray(const ArrayDinamico *array);

// Funciones avanzadas
bool redimensionarArray(ArrayDinamico *array, size_t nuevo_tamaño);
bool insertarElemento(ArrayDinamico *array, size_t indice, int valor);
bool eliminarElemento(ArrayDinamico *array, size_t indice);
bool agregarAlFinal(ArrayDinamico *array, int valor);

// Operaciones de búsqueda y estadísticas
int buscarElemento(const ArrayDinamico *array, int valor);
int obtenerMaximo(const ArrayDinamico *array);
int obtenerMinimo(const ArrayDinamico *array);
double calcularPromedio(const ArrayDinamico *array);
size_t contarElementos(const ArrayDinamico *array, int valor);

// Operaciones avanzadas
void invertirArray(ArrayDinamico *array);
void ordenarArray(ArrayDinamico *array);
ArrayDinamico* copiarArray(const ArrayDinamico *array);
ArrayDinamico* concatenarArrays(const ArrayDinamico *array1, const ArrayDinamico *array2);

// Utilidades para depuración
void mostrarEstadisticas(const ArrayDinamico *array);
bool validarArray(const ArrayDinamico *array);

#endif // ARRAY_DINAMICO_H
