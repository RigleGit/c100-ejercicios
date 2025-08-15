#include <stdio.h>
#include <string.h>
#include "array_estructuras.h"

#ifndef UNIT_TESTING
int main(void) {
    printf("=== Ejercicio 058: Array de Estructuras ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("--- Ejemplo Básico (Mayores de 30) ---\n");
    
    ListaPersonas lista;
    inicializar_lista(&lista);
    
    printf("Introduce datos para %d personas:\n", MAX_PERSONAS);
    if (!leer_personas_usuario(&lista)) {
        printf("Error al leer los datos.\n");
        return 1;
    }
    
    printf("\n");
    imprimir_mayores_de(lista.personas, lista.cantidad, 30);
    
    // Ejemplos avanzados
    printf("\n--- Ejemplos Avanzados ---\n");
    
    // Menú interactivo
    int opcion;
    do {
        printf("\n--- Opciones ---\n");
        printf("1. Mostrar todas las personas\n");
        printf("2. Filtrar por edad mínima\n");
        printf("3. Filtrar por rango de edad\n");
        printf("4. Buscar por nombre\n");
        printf("5. Ordenar por edad\n");
        printf("6. Ordenar por nombre\n");
        printf("7. Estadísticas\n");
        printf("8. Agregar persona\n");
        printf("9. Eliminar persona\n");
        printf("10. Filtros avanzados\n");
        printf("11. Cargar datos de ejemplo\n");
        printf("0. Salir\n");
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);
        getchar(); // Consumir salto de línea
        
        switch(opcion) {
            case 1:
                mostrar_lista_completa(&lista);
                break;
                
            case 2: {
                int edad_minima;
                printf("Introduce la edad mínima: ");
                scanf("%d", &edad_minima);
                getchar();
                printf("\nPersonas mayores de %d años:\n", edad_minima);
                imprimir_mayores_de(lista.personas, lista.cantidad, edad_minima);
                break;
            }
            
            case 3: {
                int edad_min, edad_max;
                printf("Introduce la edad mínima: ");
                scanf("%d", &edad_min);
                printf("Introduce la edad máxima: ");
                scanf("%d", &edad_max);
                getchar();
                imprimir_rango_edad(lista.personas, lista.cantidad, edad_min, edad_max);
                break;
            }
            
            case 4: {
                char nombre[MAX_NOMBRE];
                printf("Introduce el nombre a buscar: ");
                fgets(nombre, MAX_NOMBRE, stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                buscar_e_imprimir_persona(&lista, nombre);
                break;
            }
            
            case 5:
                ordenar_por_edad(&lista);
                printf("Lista ordenada por edad:\n");
                mostrar_lista_completa(&lista);
                break;
                
            case 6:
                ordenar_por_nombre(&lista);
                printf("Lista ordenada por nombre:\n");
                mostrar_lista_completa(&lista);
                break;
                
            case 7:
                mostrar_estadisticas(&lista);
                break;
                
            case 8: {
                if (lista.cantidad >= MAX_PERSONAS) {
                    printf("La lista está llena.\n");
                    break;
                }
                
                Persona nueva_persona;
                printf("Introduce el nombre: ");
                fgets(nueva_persona.nombre, MAX_NOMBRE, stdin);
                nueva_persona.nombre[strcspn(nueva_persona.nombre, "\n")] = '\0';
                
                printf("Introduce la edad: ");
                scanf("%d", &nueva_persona.edad);
                getchar();
                
                if (agregar_persona(&lista, &nueva_persona)) {
                    printf("Persona agregada exitosamente.\n");
                } else {
                    printf("Error al agregar persona.\n");
                }
                break;
            }
            
            case 9: {
                if (lista.cantidad == 0) {
                    printf("La lista está vacía.\n");
                    break;
                }
                
                mostrar_lista_numerada(&lista);
                int indice;
                printf("Introduce el número de la persona a eliminar (1-%d): ", lista.cantidad);
                scanf("%d", &indice);
                getchar();
                
                if (eliminar_persona(&lista, indice - 1)) {
                    printf("Persona eliminada exitosamente.\n");
                } else {
                    printf("Índice inválido.\n");
                }
                break;
            }
            
            case 10: {
                printf("\n--- Filtros Avanzados ---\n");
                printf("1. Nombres que empiecen con letra específica\n");
                printf("2. Personas en década específica (20s, 30s, etc.)\n");
                printf("3. Nombres con longitud específica\n");
                printf("Selecciona filtro: ");
                
                int filtro;
                scanf("%d", &filtro);
                getchar();
                
                switch(filtro) {
                    case 1: {
                        char letra;
                        printf("Introduce la letra inicial: ");
                        scanf("%c", &letra);
                        getchar();
                        filtrar_por_inicial(lista.personas, lista.cantidad, letra);
                        break;
                    }
                    case 2: {
                        int decada;
                        printf("Introduce la década (ej: 20, 30, 40): ");
                        scanf("%d", &decada);
                        getchar();
                        filtrar_por_decada(lista.personas, lista.cantidad, decada);
                        break;
                    }
                    case 3: {
                        int longitud;
                        printf("Introduce la longitud del nombre: ");
                        scanf("%d", &longitud);
                        getchar();
                        filtrar_por_longitud_nombre(lista.personas, lista.cantidad, longitud);
                        break;
                    }
                    default:
                        printf("Opción inválida.\n");
                }
                break;
            }
            
            case 11:
                cargar_datos_ejemplo(&lista);
                printf("Datos de ejemplo cargados.\n");
                mostrar_lista_completa(&lista);
                break;
                
            case 0:
                printf("Saliendo...\n");
                break;
                
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
    
    printf("\n--- Programa finalizado ---\n");
    return 0;
}
#endif
