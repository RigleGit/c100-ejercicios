#include "../include/array_dinamico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función principal del enunciado: crear array dinámico
ArrayDinamico* crearArrayDinamico(size_t tamaño_inicial) {
    if (tamaño_inicial == 0) {
        tamaño_inicial = 1; // Mínimo tamaño para evitar malloc(0)
    }
    
    ArrayDinamico *array = malloc(sizeof(ArrayDinamico));
    if (!array) {
        return NULL;
    }
    
    array->datos = malloc(tamaño_inicial * sizeof(int));
    if (!array->datos) {
        free(array);
        return NULL;
    }
    
    array->tamaño = 0;
    array->capacidad = tamaño_inicial;
    
    return array;
}

// Liberar memoria del array dinámico
void liberarArrayDinamico(ArrayDinamico *array) {
    if (array) {
        free(array->datos);
        free(array);
    }
}

// Inicializar array con un valor específico
void inicializarConValor(ArrayDinamico *array, int valor) {
    if (!array) return;
    
    for (size_t i = 0; i < array->capacidad; i++) {
        array->datos[i] = valor;
    }
    array->tamaño = array->capacidad;
}

// Mostrar contenido del array
void mostrarArray(const ArrayDinamico *array) {
    if (!array || !array->datos) {
        printf("Array inválido\n");
        return;
    }
    
    printf("Array[%zu/%zu]: [", array->tamaño, array->capacidad);
    for (size_t i = 0; i < array->tamaño; i++) {
        printf("%d", array->datos[i]);
        if (i < array->tamaño - 1) printf(", ");
    }
    printf("]\n");
}

// Redimensionar array dinámico
bool redimensionarArray(ArrayDinamico *array, size_t nuevo_tamaño) {
    if (!array || nuevo_tamaño == 0) return false;
    
    int *nuevos_datos = realloc(array->datos, nuevo_tamaño * sizeof(int));
    if (!nuevos_datos) {
        return false;
    }
    
    array->datos = nuevos_datos;
    array->capacidad = nuevo_tamaño;
    
    // Ajustar tamaño si es necesario
    if (array->tamaño > nuevo_tamaño) {
        array->tamaño = nuevo_tamaño;
    }
    
    return true;
}

// Insertar elemento en posición específica
bool insertarElemento(ArrayDinamico *array, size_t indice, int valor) {
    if (!array || indice > array->tamaño) return false;
    
    // Redimensionar si es necesario
    if (array->tamaño >= array->capacidad) {
        if (!redimensionarArray(array, array->capacidad * 2)) {
            return false;
        }
    }
    
    // Mover elementos hacia la derecha
    for (size_t i = array->tamaño; i > indice; i--) {
        array->datos[i] = array->datos[i - 1];
    }
    
    array->datos[indice] = valor;
    array->tamaño++;
    
    return true;
}

// Eliminar elemento en posición específica
bool eliminarElemento(ArrayDinamico *array, size_t indice) {
    if (!array || indice >= array->tamaño) return false;
    
    // Mover elementos hacia la izquierda
    for (size_t i = indice; i < array->tamaño - 1; i++) {
        array->datos[i] = array->datos[i + 1];
    }
    
    array->tamaño--;
    
    return true;
}

// Agregar elemento al final
bool agregarAlFinal(ArrayDinamico *array, int valor) {
    if (!array) return false;
    
    return insertarElemento(array, array->tamaño, valor);
}

// Buscar elemento (devuelve índice o -1 si no se encuentra)
int buscarElemento(const ArrayDinamico *array, int valor) {
    if (!array) return -1;
    
    for (size_t i = 0; i < array->tamaño; i++) {
        if (array->datos[i] == valor) {
            return (int)i;
        }
    }
    
    return -1;
}

// Obtener elemento máximo
int obtenerMaximo(const ArrayDinamico *array) {
    if (!array || array->tamaño == 0) return 0;
    
    int max = array->datos[0];
    for (size_t i = 1; i < array->tamaño; i++) {
        if (array->datos[i] > max) {
            max = array->datos[i];
        }
    }
    
    return max;
}

// Obtener elemento mínimo
int obtenerMinimo(const ArrayDinamico *array) {
    if (!array || array->tamaño == 0) return 0;
    
    int min = array->datos[0];
    for (size_t i = 1; i < array->tamaño; i++) {
        if (array->datos[i] < min) {
            min = array->datos[i];
        }
    }
    
    return min;
}

// Calcular promedio
double calcularPromedio(const ArrayDinamico *array) {
    if (!array || array->tamaño == 0) return 0.0;
    
    long long suma = 0;
    for (size_t i = 0; i < array->tamaño; i++) {
        suma += array->datos[i];
    }
    
    return (double)suma / array->tamaño;
}

// Contar ocurrencias de un valor
size_t contarElementos(const ArrayDinamico *array, int valor) {
    if (!array) return 0;
    
    size_t contador = 0;
    for (size_t i = 0; i < array->tamaño; i++) {
        if (array->datos[i] == valor) {
            contador++;
        }
    }
    
    return contador;
}

// Invertir array
void invertirArray(ArrayDinamico *array) {
    if (!array || array->tamaño <= 1) return;
    
    for (size_t i = 0; i < array->tamaño / 2; i++) {
        int temp = array->datos[i];
        array->datos[i] = array->datos[array->tamaño - 1 - i];
        array->datos[array->tamaño - 1 - i] = temp;
    }
}

// Ordenar array (burbuja simple)
void ordenarArray(ArrayDinamico *array) {
    if (!array || array->tamaño <= 1) return;
    
    for (size_t i = 0; i < array->tamaño - 1; i++) {
        for (size_t j = 0; j < array->tamaño - 1 - i; j++) {
            if (array->datos[j] > array->datos[j + 1]) {
                int temp = array->datos[j];
                array->datos[j] = array->datos[j + 1];
                array->datos[j + 1] = temp;
            }
        }
    }
}

// Copiar array
ArrayDinamico* copiarArray(const ArrayDinamico *array) {
    if (!array) return NULL;
    
    ArrayDinamico *copia = crearArrayDinamico(array->capacidad);
    if (!copia) return NULL;
    
    memcpy(copia->datos, array->datos, array->tamaño * sizeof(int));
    copia->tamaño = array->tamaño;
    
    return copia;
}

// Concatenar dos arrays
ArrayDinamico* concatenarArrays(const ArrayDinamico *array1, const ArrayDinamico *array2) {
    if (!array1 || !array2) return NULL;
    
    ArrayDinamico *resultado = crearArrayDinamico(array1->tamaño + array2->tamaño);
    if (!resultado) return NULL;
    
    // Copiar primer array
    memcpy(resultado->datos, array1->datos, array1->tamaño * sizeof(int));
    resultado->tamaño = array1->tamaño;
    
    // Agregar segundo array
    memcpy(resultado->datos + array1->tamaño, array2->datos, array2->tamaño * sizeof(int));
    resultado->tamaño += array2->tamaño;
    
    return resultado;
}

// Mostrar estadísticas del array
void mostrarEstadisticas(const ArrayDinamico *array) {
    if (!array) {
        printf("Array inválido\n");
        return;
    }
    
    printf("=== Estadísticas del Array ===\n");
    printf("Tamaño: %zu elementos\n", array->tamaño);
    printf("Capacidad: %zu elementos\n", array->capacidad);
    printf("Memoria usada: %zu bytes\n", array->tamaño * sizeof(int));
    printf("Memoria total: %zu bytes\n", array->capacidad * sizeof(int));
    printf("Factor de carga: %.2f%%\n", 
           array->capacidad > 0 ? (double)array->tamaño / array->capacidad * 100 : 0.0);
    
    if (array->tamaño > 0) {
        printf("Mínimo: %d\n", obtenerMinimo(array));
        printf("Máximo: %d\n", obtenerMaximo(array));
        printf("Promedio: %.2f\n", calcularPromedio(array));
    }
    printf("===============================\n");
}

// Validar integridad del array
bool validarArray(const ArrayDinamico *array) {
    if (!array) return false;
    if (!array->datos && array->capacidad > 0) return false;
    if (array->tamaño > array->capacidad) return false;
    
    return true;
}
