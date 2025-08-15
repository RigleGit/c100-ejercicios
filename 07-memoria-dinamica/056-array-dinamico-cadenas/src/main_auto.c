#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array_dinamico_cadenas.h"

/**
 * Versión automática del main para tests sin interacción
 */
int main(void) {
    printf("=== Ejercicio 056: Array Dinámico de Cadenas (AUTOMÁTICO) ===\n\n");
    
    // Usar número predefinido de nombres para tests automáticos
    int n = 4;
    printf("Usando %d nombres predefinidos para el test automático\n", n);
    
    // Crear array dinámico de cadenas
    ArrayCadenas *array = crear_array_cadenas(n);
    if (array == NULL) {
        printf("Error: No se pudo crear el array de cadenas.\n");
        return 1;
    }
    
    printf("\nArray creado exitosamente\n");
    
    // Agregar nombres predefinidos
    printf("\n=== Agregando nombres predefinidos ===\n");
    const char *nombres_predefinidos[] = {"Ana", "Bruno", "Carlos", "Diana"};
    
    for (int i = 0; i < n; i++) {
        if (agregar_nombre(array, nombres_predefinidos[i])) {
            printf("✅ Nombre '%s' agregado correctamente\n", nombres_predefinidos[i]);
        } else {
            printf("❌ Error agregando nombre '%s'\n", nombres_predefinidos[i]);
        }
    }
    
    // Mostrar nombres
    printf("\n=== Mostrando nombres ===\n");
    mostrar_nombres(array);
    
    // Ordenar alfabéticamente
    printf("\n=== Ordenando nombres alfabéticamente ===\n");
    ordenar_nombres(array);
    printf("Nombres ordenados alfabéticamente.\n");
    mostrar_nombres(array);
    
    // Buscar nombres
    printf("\n=== Buscando nombres ===\n");
    int pos_ana = buscar_nombre(array, "Ana");
    int pos_bruno = buscar_nombre(array, "Bruno");
    int pos_inexistente = buscar_nombre(array, "NoExiste");
    
    printf("'Ana' encontrado en posición: %d\n", pos_ana);
    printf("'Bruno' encontrado en posición: %d\n", pos_bruno);
    printf("'NoExiste' encontrado en posición: %d\n", pos_inexistente);
    
    // Agregar más nombres
    printf("\n=== Agregando más nombres ===\n");
    if (agregar_nombre(array, "Elena")) {
        printf("✅ Nombre 'Elena' agregado correctamente\n");
    }
    if (agregar_nombre(array, "Francisco")) {
        printf("✅ Nombre 'Francisco' agregado correctamente\n");
    }
    
    mostrar_nombres(array);
    
    // Eliminar nombre
    printf("\n=== Eliminando nombre ===\n");
    if (eliminar_nombre_por_indice(array, 1)) {
        printf("✅ Nombre en posición 1 eliminado correctamente\n");
    } else {
        printf("❌ Error eliminando nombre\n");
    }
    
    mostrar_nombres(array);
    
    // Mostrar estadísticas
    printf("\n=== Estadísticas del array ===\n");
    mostrar_estadisticas(array);
    
    // Filtrar por longitud
    printf("\n=== Filtrando por longitud (3-5 caracteres) ===\n");
    ArrayCadenas *filtrado = filtrar_por_longitud(array, 3, 5);
    if (filtrado && filtrado->size > 0) {
        printf("Nombres filtrados (3-5 caracteres):\n");
        mostrar_nombres(filtrado);
    } else {
        printf("No se encontraron nombres en ese rango.\n");
    }
    if (filtrado) liberar_array_cadenas(filtrado);
    
    // Crear copia
    printf("\n=== Creando copia del array ===\n");
    ArrayCadenas *copia = copiar_array_cadenas(array);
    if (copia) {
        printf("✅ Copia creada exitosamente\n");
        printf("Contenido de la copia:\n");
        mostrar_nombres(copia);
        liberar_array_cadenas(copia);
    } else {
        printf("❌ Error al crear la copia\n");
    }
    
    // Liberar memoria
    liberar_array_cadenas(array);
    printf("\nMemoria liberada correctamente.\n");
    
    printf("\n✅ Ejercicio completado exitosamente (versión automática)\n");
    
    return 0;
}

