#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array_dinamico_cadenas.h"

// Crear array dinámico de cadenas
ArrayCadenas* crear_array_cadenas(int capacidad) {
    if (capacidad <= 0) {
        return NULL;
    }
    
    ArrayCadenas *array = (ArrayCadenas*)malloc(sizeof(ArrayCadenas));
    if (array == NULL) {
        return NULL;
    }
    
    array->cadenas = (char**)malloc(capacidad * sizeof(char*));
    if (array->cadenas == NULL) {
        free(array);
        return NULL;
    }
    
    array->size = 0;
    array->capacidad = capacidad;
    
    // Inicializar punteros a NULL
    for (int i = 0; i < capacidad; i++) {
        array->cadenas[i] = NULL;
    }
    
    return array;
}

// Liberar array dinámico de cadenas
void liberar_array_cadenas(ArrayCadenas* array) {
    if (array == NULL) {
        return;
    }
    
    // Liberar cada cadena
    for (int i = 0; i < array->size; i++) {
        if (array->cadenas[i] != NULL) {
            free(array->cadenas[i]);
        }
    }
    
    // Liberar array de punteros
    free(array->cadenas);
    
    // Liberar estructura
    free(array);
}

// Leer nombres del usuario
bool leer_nombres_usuario(ArrayCadenas* array) {
    if (array == NULL) {
        return false;
    }
    
    getchar(); // Limpiar buffer después del scanf inicial
    
    for (int i = 0; i < array->capacidad; i++) {
        char buffer[MAX_LONGITUD];
        printf("Nombre %d: ", i + 1);
        
        if (fgets(buffer, MAX_LONGITUD, stdin) == NULL) {
            return false;
        }
        
        // Eliminar salto de línea
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // Verificar que no esté vacío
        if (strlen(buffer) == 0) {
            printf("Nombre vacío no permitido. Intenta de nuevo.\n");
            i--; // Repetir esta iteración
            continue;
        }
        
        // Reservar memoria exacta para la cadena
        array->cadenas[i] = (char*)malloc(strlen(buffer) + 1);
        if (array->cadenas[i] == NULL) {
            printf("Error al reservar memoria para el nombre %d.\n", i + 1);
            return false;
        }
        
        strcpy(array->cadenas[i], buffer);
        array->size++;
    }
    
    return true;
}

// Mostrar nombres
void mostrar_nombres(const ArrayCadenas* array) {
    if (array == NULL || array->size == 0) {
        printf("No hay nombres para mostrar.\n");
        return;
    }
    
    printf("\nNombres (%d/%d):\n", array->size, array->capacidad);
    for (int i = 0; i < array->size; i++) {
        printf("%2d. %s\n", i + 1, array->cadenas[i]);
    }
}

// Ordenar nombres alfabéticamente
void ordenar_nombres(ArrayCadenas* array) {
    if (array == NULL || array->size <= 1) {
        return;
    }
    
    // Algoritmo de burbuja mejorado
    for (int i = 0; i < array->size - 1; i++) {
        bool intercambio = false;
        for (int j = 0; j < array->size - 1 - i; j++) {
            if (strcmp(array->cadenas[j], array->cadenas[j + 1]) > 0) {
                // Intercambiar punteros
                char *temp = array->cadenas[j];
                array->cadenas[j] = array->cadenas[j + 1];
                array->cadenas[j + 1] = temp;
                intercambio = true;
            }
        }
        if (!intercambio) break; // Ya está ordenado
    }
}

// Buscar nombre
int buscar_nombre(const ArrayCadenas* array, const char* nombre) {
    if (array == NULL || nombre == NULL) {
        return -1;
    }
    
    for (int i = 0; i < array->size; i++) {
        if (strcmp(array->cadenas[i], nombre) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Redimensionar array (función auxiliar)
static bool redimensionar_array(ArrayCadenas* array, int nueva_capacidad) {
    if (array == NULL || nueva_capacidad <= 0) {
        return false;
    }
    
    char **nuevo_array = (char**)realloc(array->cadenas, nueva_capacidad * sizeof(char*));
    if (nuevo_array == NULL) {
        return false;
    }
    
    array->cadenas = nuevo_array;
    
    // Inicializar nuevos elementos si la capacidad aumentó
    if (nueva_capacidad > array->capacidad) {
        for (int i = array->capacidad; i < nueva_capacidad; i++) {
            array->cadenas[i] = NULL;
        }
    }
    
    array->capacidad = nueva_capacidad;
    
    return true;
}

// Agregar nombre
bool agregar_nombre(ArrayCadenas* array, const char* nombre) {
    if (array == NULL || nombre == NULL || strlen(nombre) == 0) {
        return false;
    }
    
    // Redimensionar si es necesario
    if (array->size >= array->capacidad) {
        int nueva_capacidad = array->capacidad * 2;
        if (!redimensionar_array(array, nueva_capacidad)) {
            return false;
        }
    }
    
    // Reservar memoria para el nuevo nombre
    array->cadenas[array->size] = (char*)malloc(strlen(nombre) + 1);
    if (array->cadenas[array->size] == NULL) {
        return false;
    }
    
    strcpy(array->cadenas[array->size], nombre);
    array->size++;
    
    return true;
}

// Eliminar nombre por índice
bool eliminar_nombre_por_indice(ArrayCadenas* array, int indice) {
    if (array == NULL || indice < 0 || indice >= array->size) {
        return false;
    }
    
    // Liberar memoria del nombre a eliminar
    free(array->cadenas[indice]);
    
    // Desplazar elementos hacia la izquierda
    for (int i = indice; i < array->size - 1; i++) {
        array->cadenas[i] = array->cadenas[i + 1];
    }
    
    array->size--;
    array->cadenas[array->size] = NULL;
    
    return true;
}

// Mostrar estadísticas
void mostrar_estadisticas(const ArrayCadenas* array) {
    if (array == NULL || array->size == 0) {
        printf("No hay nombres para analizar.\n");
        return;
    }
    
    int longitud_total = 0;
    int longitud_min = strlen(array->cadenas[0]);
    int longitud_max = strlen(array->cadenas[0]);
    char *nombre_mas_corto = array->cadenas[0];
    char *nombre_mas_largo = array->cadenas[0];
    
    for (int i = 0; i < array->size; i++) {
        int longitud = strlen(array->cadenas[i]);
        longitud_total += longitud;
        
        if (longitud < longitud_min) {
            longitud_min = longitud;
            nombre_mas_corto = array->cadenas[i];
        }
        
        if (longitud > longitud_max) {
            longitud_max = longitud;
            nombre_mas_largo = array->cadenas[i];
        }
    }
    
    double longitud_promedio = (double)longitud_total / array->size;
    
    printf("\n--- Estadísticas del Array ---\n");
    printf("Cantidad de nombres: %d\n", array->size);
    printf("Capacidad del array: %d\n", array->capacidad);
    printf("Longitud total de caracteres: %d\n", longitud_total);
    printf("Longitud promedio: %.2f caracteres\n", longitud_promedio);
    printf("Nombre más corto: \"%s\" (%d caracteres)\n", nombre_mas_corto, longitud_min);
    printf("Nombre más largo: \"%s\" (%d caracteres)\n", nombre_mas_largo, longitud_max);
    
    // Memoria utilizada
    size_t memoria_nombres = longitud_total + array->size; // +1 por cada '\0'
    size_t memoria_punteros = array->capacidad * sizeof(char*);
    size_t memoria_estructura = sizeof(ArrayCadenas);
    size_t memoria_total = memoria_nombres + memoria_punteros + memoria_estructura;
    
    printf("Memoria utilizada:\n");
    printf("  - Nombres: %zu bytes\n", memoria_nombres);
    printf("  - Punteros: %zu bytes\n", memoria_punteros);
    printf("  - Estructura: %zu bytes\n", memoria_estructura);
    printf("  - Total: %zu bytes\n", memoria_total);
}

// Filtrar por longitud
ArrayCadenas* filtrar_por_longitud(const ArrayCadenas* array, int min_len, int max_len) {
    if (array == NULL || min_len < 0 || max_len < min_len) {
        return NULL;
    }
    
    // Contar cuántos nombres cumplen el criterio
    int contador = 0;
    for (int i = 0; i < array->size; i++) {
        int longitud = strlen(array->cadenas[i]);
        if (longitud >= min_len && longitud <= max_len) {
            contador++;
        }
    }
    
    if (contador == 0) {
        return crear_array_cadenas(1); // Array vacío
    }
    
    // Crear nuevo array
    ArrayCadenas *filtrado = crear_array_cadenas(contador);
    if (filtrado == NULL) {
        return NULL;
    }
    
    // Copiar nombres que cumplen el criterio
    for (int i = 0; i < array->size; i++) {
        int longitud = strlen(array->cadenas[i]);
        if (longitud >= min_len && longitud <= max_len) {
            if (!agregar_nombre(filtrado, array->cadenas[i])) {
                liberar_array_cadenas(filtrado);
                return NULL;
            }
        }
    }
    
    return filtrado;
}

// Copiar array de cadenas
ArrayCadenas* copiar_array_cadenas(const ArrayCadenas* array) {
    if (array == NULL) {
        return NULL;
    }
    
    ArrayCadenas *copia = crear_array_cadenas(array->capacidad);
    if (copia == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < array->size; i++) {
        if (!agregar_nombre(copia, array->cadenas[i])) {
            liberar_array_cadenas(copia);
            return NULL;
        }
    }
    
    return copia;
}

// Versión básica para tests
ArrayCadenas* crear_array_cadenas_basico(int n, char nombres[][MAX_LONGITUD]) {
    if (n <= 0 || nombres == NULL) {
        return NULL;
    }
    
    ArrayCadenas *array = crear_array_cadenas(n);
    if (array == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        if (!agregar_nombre(array, nombres[i])) {
            liberar_array_cadenas(array);
            return NULL;
        }
    }
    
    return array;
}

// Obtener nombre por índice
char* obtener_nombre(const ArrayCadenas* array, int indice) {
    if (array == NULL || indice < 0 || indice >= array->size) {
        return NULL;
    }
    
    return array->cadenas[indice];
}

// Buscar nombre insensible a mayúsculas/minúsculas
int buscar_nombre_insensible(const ArrayCadenas* array, const char* nombre) {
    if (array == NULL || nombre == NULL) {
        return -1;
    }
    
    for (int i = 0; i < array->size; i++) {
        if (strcasecmp(array->cadenas[i], nombre) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Función auxiliar para strcasecmp en sistemas que no la tienen
#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        s1++;
        s2++;
    }
    
    char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
    char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
    
    return c1 - c2;
}
#endif
