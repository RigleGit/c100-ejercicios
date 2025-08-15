#include "../include/redimensionar_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Crear array redimensionable con tamaño de bloque inicial
ArrayRedimensionable* crearArrayRedimensionable(size_t bloque_inicial) {
    if (bloque_inicial == 0) {
        bloque_inicial = BLOQUE_INICIAL;
    }
    
    ArrayRedimensionable *array = malloc(sizeof(ArrayRedimensionable));
    if (!array) {
        return NULL;
    }
    
    array->datos = malloc(bloque_inicial * sizeof(int));
    if (!array->datos) {
        free(array);
        return NULL;
    }
    
    array->cantidad = 0;
    array->capacidad = bloque_inicial;
    array->bloque_tamaño = bloque_inicial;
    
    return array;
}

// Liberar memoria del array redimensionable
void liberarArrayRedimensionable(ArrayRedimensionable *array) {
    if (array) {
        free(array->datos);
        free(array);
    }
}

// Verificar si el array necesita redimensionamiento
bool necesitaRedimensionar(const ArrayRedimensionable *array) {
    if (!array) return false;
    return array->cantidad >= array->capacidad;
}

// Calcular nueva capacidad basada en bloques
size_t calcularNuevaCapacidad(const ArrayRedimensionable *array) {
    if (!array) return 0;
    return array->capacidad + array->bloque_tamaño;
}

// Redimensionar array usando realloc
bool redimensionarArray(ArrayRedimensionable *array, size_t nueva_capacidad) {
    if (!array || nueva_capacidad == 0) {
        return false;
    }
    
    printf("🔄 Redimensionando array de %zu a %zu elementos\n", 
           array->capacidad, nueva_capacidad);
    
    int *nuevos_datos = realloc(array->datos, nueva_capacidad * sizeof(int));
    if (!nuevos_datos) {
        printf("❌ Error: No se pudo redimensionar el array\n");
        return false;
    }
    
    array->datos = nuevos_datos;
    array->capacidad = nueva_capacidad;
    
    // Ajustar cantidad si se redujo la capacidad
    if (array->cantidad > nueva_capacidad) {
        array->cantidad = nueva_capacidad;
    }
    
    printf("✅ Redimensionamiento exitoso\n");
    return true;
}

// Agregar elemento con redimensionamiento automático
bool agregarElemento(ArrayRedimensionable *array, int valor) {
    if (!array) return false;
    
    // Verificar si necesita redimensionamiento
    if (necesitaRedimensionar(array)) {
        size_t nueva_capacidad = calcularNuevaCapacidad(array);
        
        if (!redimensionarArray(array, nueva_capacidad)) {
            return false;
        }
    }
    
    array->datos[array->cantidad++] = valor;
    return true;
}

// Mostrar contenido del array
void mostrarArray(const ArrayRedimensionable *array) {
    if (!array || !array->datos) {
        printf("Array inválido\n");
        return;
    }
    
    printf("Array[%zu/%zu]: [", array->cantidad, array->capacidad);
    for (size_t i = 0; i < array->cantidad; i++) {
        printf("%d", array->datos[i]);
        if (i < array->cantidad - 1) printf(", ");
    }
    printf("]\n");
}

// Mostrar estadísticas detalladas
void mostrarEstadisticas(const ArrayRedimensionable *array) {
    if (!array) {
        printf("Array inválido\n");
        return;
    }
    
    printf("\n📊 === ESTADÍSTICAS DEL ARRAY ===\n");
    printf("Elementos: %zu\n", array->cantidad);
    printf("Capacidad: %zu\n", array->capacidad);
    printf("Tamaño de bloque: %zu\n", array->bloque_tamaño);
    printf("Factor de uso: %.1f%%\n", calcularFactorUso(array));
    printf("Memoria usada: %zu bytes\n", calcularMemoriaUsada(array));
    printf("Memoria total: %zu bytes\n", calcularMemoriaTotal(array));
    printf("Memoria desperdiciada: %zu bytes\n", 
           calcularMemoriaTotal(array) - calcularMemoriaUsada(array));
    
    if (array->capacidad > 0) {
        size_t bloques_usados = (array->cantidad + array->bloque_tamaño - 1) / array->bloque_tamaño;
        size_t bloques_totales = array->capacidad / array->bloque_tamaño;
        printf("Bloques utilizados: %zu/%zu\n", bloques_usados, bloques_totales);
    }
    
    printf("===============================\n\n");
}

// Validar integridad del array
bool validarArray(const ArrayRedimensionable *array) {
    if (!array) return false;
    if (!array->datos && array->capacidad > 0) return false;
    if (array->cantidad > array->capacidad) return false;
    if (array->bloque_tamaño == 0) return false;
    
    return true;
}

// Leer números del usuario hasta encontrar valor de terminación
int leerNumerosUsuario(ArrayRedimensionable *array) {
    if (!array) return -1;
    
    int entrada;
    int numeros_leidos = 0;
    
    printf("📝 Introduce números enteros (termina con %d):\n", VALOR_TERMINACION);
    
    while (1) {
        printf("Número %d: ", numeros_leidos + 1);
        
        if (scanf("%d", &entrada) != 1) {
            printf("❌ Error: Entrada inválida\n");
            // Limpiar buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        if (entrada == VALOR_TERMINACION) {
            printf("🔚 Terminación detectada\n");
            break;
        }
        
        if (!agregarElemento(array, entrada)) {
            printf("❌ Error: No se pudo agregar el elemento\n");
            return -1;
        }
        
        numeros_leidos++;
        
        // Mostrar progreso cada 5 elementos
        if (numeros_leidos % 5 == 0) {
            printf("📈 Progreso: %d números introducidos\n", numeros_leidos);
        }
    }
    
    return numeros_leidos;
}

// Imprimir resultados del array
void imprimirResultados(const ArrayRedimensionable *array) {
    if (!array) {
        printf("❌ Array inválido\n");
        return;
    }
    
    printf("\n🎯 === RESULTADOS ===\n");
    printf("Has introducido %zu números:\n", array->cantidad);
    
    if (array->cantidad == 0) {
        printf("(ningún número introducido)\n");
        return;
    }
    
    // Mostrar números en filas de 10
    for (size_t i = 0; i < array->cantidad; i++) {
        if (i % 10 == 0 && i > 0) printf("\n");
        printf("%4d ", array->datos[i]);
    }
    printf("\n");
    
    mostrarEstadisticas(array);
}

// Copiar array redimensionable
ArrayRedimensionable* copiarArray(const ArrayRedimensionable *array) {
    if (!array) return NULL;
    
    ArrayRedimensionable *copia = crearArrayRedimensionable(array->bloque_tamaño);
    if (!copia) return NULL;
    
    // Redimensionar a la capacidad del original
    if (!redimensionarArray(copia, array->capacidad)) {
        liberarArrayRedimensionable(copia);
        return NULL;
    }
    
    // Copiar datos
    memcpy(copia->datos, array->datos, array->cantidad * sizeof(int));
    copia->cantidad = array->cantidad;
    
    return copia;
}

// Redimensionar por porcentaje
bool redimensionarPorPorcentaje(ArrayRedimensionable *array, double factor) {
    if (!array || factor <= 0.0) return false;
    
    size_t nueva_capacidad = (size_t)(array->capacidad * factor);
    if (nueva_capacidad < array->cantidad) {
        nueva_capacidad = array->cantidad; // No perder datos
    }
    
    return redimensionarArray(array, nueva_capacidad);
}

// Compactar array para minimizar memoria desperdiciada
void compactarArray(ArrayRedimensionable *array) {
    if (!array || array->cantidad == 0) return;
    
    // Calcular capacidad óptima (múltiplo del tamaño de bloque)
    size_t bloques_necesarios = (array->cantidad + array->bloque_tamaño - 1) / array->bloque_tamaño;
    size_t nueva_capacidad = bloques_necesarios * array->bloque_tamaño;
    
    if (nueva_capacidad < array->capacidad) {
        printf("🗜️ Compactando array de %zu a %zu elementos\n", 
               array->capacidad, nueva_capacidad);
        redimensionarArray(array, nueva_capacidad);
    }
}

// Insertar elemento en posición específica
bool insertarEnPosicion(ArrayRedimensionable *array, size_t posicion, int valor) {
    if (!array || posicion > array->cantidad) return false;
    
    // Verificar si necesita redimensionamiento
    if (necesitaRedimensionar(array)) {
        size_t nueva_capacidad = calcularNuevaCapacidad(array);
        if (!redimensionarArray(array, nueva_capacidad)) {
            return false;
        }
    }
    
    // Mover elementos hacia la derecha
    for (size_t i = array->cantidad; i > posicion; i--) {
        array->datos[i] = array->datos[i - 1];
    }
    
    array->datos[posicion] = valor;
    array->cantidad++;
    
    return true;
}

// Eliminar elemento en posición específica
bool eliminarEnPosicion(ArrayRedimensionable *array, size_t posicion) {
    if (!array || posicion >= array->cantidad) return false;
    
    // Mover elementos hacia la izquierda
    for (size_t i = posicion; i < array->cantidad - 1; i++) {
        array->datos[i] = array->datos[i + 1];
    }
    
    array->cantidad--;
    
    return true;
}

// Analizar patrón de crecimiento
void analizarPatronCrecimiento(const ArrayRedimensionable *array) {
    if (!array) return;
    
    printf("\n📈 === ANÁLISIS DE CRECIMIENTO ===\n");
    
    size_t redimensionamientos = array->capacidad / array->bloque_tamaño;
    if (array->capacidad % array->bloque_tamaño != 0) redimensionamientos++;
    redimensionamientos--; // Descontar asignación inicial
    
    printf("Redimensionamientos realizados: %zu\n", redimensionamientos);
    printf("Tamaño de bloque: %zu elementos\n", array->bloque_tamaño);
    printf("Crecimiento total: %zu -> %zu elementos\n", 
           array->bloque_tamaño, array->capacidad);
    
    if (redimensionamientos > 0) {
        printf("Promedio de elementos por redimensionamiento: %.1f\n",
               (double)array->cantidad / redimensionamientos);
    }
    
    printf("================================\n\n");
}

// Calcular factor de uso (porcentaje de capacidad utilizada)
double calcularFactorUso(const ArrayRedimensionable *array) {
    if (!array || array->capacidad == 0) return 0.0;
    return (double)array->cantidad / array->capacidad * 100.0;
}

// Calcular memoria usada por los datos
size_t calcularMemoriaUsada(const ArrayRedimensionable *array) {
    if (!array) return 0;
    return array->cantidad * sizeof(int);
}

// Calcular memoria total asignada
size_t calcularMemoriaTotal(const ArrayRedimensionable *array) {
    if (!array) return 0;
    return array->capacidad * sizeof(int) + sizeof(ArrayRedimensionable);
}

// Crear array con datos predefinidos (para testing)
ArrayRedimensionable* crearArrayConDatos(const int *datos, size_t cantidad, size_t bloque) {
    if (!datos && cantidad > 0) return NULL;
    
    ArrayRedimensionable *array = crearArrayRedimensionable(bloque);
    if (!array) return NULL;
    
    for (size_t i = 0; i < cantidad; i++) {
        if (!agregarElemento(array, datos[i])) {
            liberarArrayRedimensionable(array);
            return NULL;
        }
    }
    
    return array;
}

// Comparar dos arrays
bool compararArrays(const ArrayRedimensionable *array1, const ArrayRedimensionable *array2) {
    if (!array1 || !array2) return false;
    if (array1->cantidad != array2->cantidad) return false;
    
    for (size_t i = 0; i < array1->cantidad; i++) {
        if (array1->datos[i] != array2->datos[i]) {
            return false;
        }
    }
    
    return true;
}
