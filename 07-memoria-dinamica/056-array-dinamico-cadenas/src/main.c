#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array_dinamico_cadenas.h"

int main(void) {
    printf("=== Ejercicio 056: Array Dinámico de Cadenas ===\n\n");
    
    int n;
    printf("¿Cuántos nombres vas a introducir?: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Error: El número de nombres debe ser positivo.\n");
        return 1;
    }
    
    // Crear array dinámico de cadenas
    ArrayCadenas *array = crear_array_cadenas(n);
    if (array == NULL) {
        printf("Error: No se pudo crear el array de cadenas.\n");
        return 1;
    }
    
    printf("\nIntroduce %d nombre(s):\n", n);
    
    // Leer nombres del usuario
    if (!leer_nombres_usuario(array)) {
        printf("Error al leer los nombres.\n");
        liberar_array_cadenas(array);
        return 1;
    }
    
    // Menú de opciones
    int opcion;
    do {
        printf("\n--- Opciones ---\n");
        printf("1. Mostrar nombres\n");
        printf("2. Ordenar alfabéticamente\n");
        printf("3. Buscar nombre\n");
        printf("4. Agregar nombre\n");
        printf("5. Eliminar nombre\n");
        printf("6. Estadísticas\n");
        printf("7. Filtrar por longitud\n");
        printf("8. Crear copia\n");
        printf("0. Salir\n");
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar buffer
        
        switch(opcion) {
            case 1:
                mostrar_nombres(array);
                break;
            case 2:
                ordenar_nombres(array);
                printf("Nombres ordenados alfabéticamente.\n");
                break;
            case 3: {
                char nombre_buscar[MAX_LONGITUD];
                printf("Introduce el nombre a buscar: ");
                fgets(nombre_buscar, MAX_LONGITUD, stdin);
                nombre_buscar[strcspn(nombre_buscar, "\n")] = '\0';
                int posicion = buscar_nombre(array, nombre_buscar);
                if (posicion >= 0) {
                    printf("'%s' encontrado en la posición %d.\n", nombre_buscar, posicion);
                } else {
                    printf("'%s' no encontrado.\n", nombre_buscar);
                }
                break;
            }
            case 4: {
                char nuevo_nombre[MAX_LONGITUD];
                printf("Introduce el nuevo nombre: ");
                fgets(nuevo_nombre, MAX_LONGITUD, stdin);
                nuevo_nombre[strcspn(nuevo_nombre, "\n")] = '\0';
                if (agregar_nombre(array, nuevo_nombre)) {
                    printf("Nombre '%s' agregado correctamente.\n", nuevo_nombre);
                } else {
                    printf("Error al agregar el nombre.\n");
                }
                break;
            }
            case 5: {
                int indice;
                printf("Introduce el índice del nombre a eliminar (0-%d): ", array->size - 1);
                scanf("%d", &indice);
                getchar();
                if (eliminar_nombre_por_indice(array, indice)) {
                    printf("Nombre eliminado correctamente.\n");
                } else {
                    printf("Índice inválido.\n");
                }
                break;
            }
            case 6:
                mostrar_estadisticas(array);
                break;
            case 7: {
                int min_len, max_len;
                printf("Longitud mínima: ");
                scanf("%d", &min_len);
                printf("Longitud máxima: ");
                scanf("%d", &max_len);
                getchar();
                ArrayCadenas *filtrado = filtrar_por_longitud(array, min_len, max_len);
                if (filtrado && filtrado->size > 0) {
                    printf("\nNombres filtrados (%d-%d caracteres):\n", min_len, max_len);
                    mostrar_nombres(filtrado);
                } else {
                    printf("No se encontraron nombres en ese rango.\n");
                }
                if (filtrado) liberar_array_cadenas(filtrado);
                break;
            }
            case 8: {
                ArrayCadenas *copia = copiar_array_cadenas(array);
                if (copia) {
                    printf("Copia creada exitosamente.\n");
                    printf("Contenido de la copia:\n");
                    mostrar_nombres(copia);
                    liberar_array_cadenas(copia);
                } else {
                    printf("Error al crear la copia.\n");
                }
                break;
            }
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
    
    // Liberar memoria
    liberar_array_cadenas(array);
    printf("Memoria liberada correctamente.\n");
    
    return 0;
}
