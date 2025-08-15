#include <stdio.h>
#include <string.h>
#include "estructura_persona.h"

#ifndef UNIT_TESTING
int main(void) {
    printf("=== Ejercicio 057: Estructura Persona ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("--- Ejemplo Básico ---\n");
    Persona p1;
    strcpy(p1.nombre, "Ana López");
    p1.edad = 28;
    
    printf("Nombre: %s\n", p1.nombre);
    printf("Edad: %d\n", p1.edad);
    
    // Ejemplos avanzados con la biblioteca
    printf("\n--- Ejemplos Avanzados ---\n");
    
    // Crear persona con función auxiliar
    Persona p2 = crear_persona("Carlos Ruiz", 35);
    mostrar_persona(&p2);
    
    // Crear persona con entrada del usuario
    printf("\n--- Crear Nueva Persona ---\n");
    Persona p3 = leer_persona_usuario();
    printf("\nDatos ingresados:\n");
    mostrar_persona_detallada(&p3);
    
    // Demostrar comparaciones
    printf("\n--- Comparaciones ---\n");
    Persona p4 = crear_persona("María García", 28);
    Persona p5 = crear_persona("José Fernández", 45);
    
    printf("Comparando edades:\n");
    mostrar_persona_simple(&p1);
    mostrar_persona_simple(&p4);
    if (misma_edad(&p1, &p4)) {
        printf("→ Tienen la misma edad\n");
    } else {
        printf("→ Tienen edades diferentes\n");
    }
    
    printf("\nPersona mayor entre %s y %s: ", p4.nombre, p5.nombre);
    Persona *mayor = persona_mayor(&p4, &p5);
    printf("%s (%d años)\n", mayor->nombre, mayor->edad);
    
    // Demostrar operaciones con arrays
    printf("\n--- Array de Personas ---\n");
    Persona personas[] = {
        {"Laura Martín", 32},
        {"Pedro Sánchez", 27},
        {"Elena Jiménez", 41},
        {"David López", 29}
    };
    int num_personas = sizeof(personas) / sizeof(personas[0]);
    
    printf("Lista de personas:\n");
    mostrar_lista_personas(personas, num_personas);
    
    // Encontrar persona mayor del grupo
    Persona *mayor_grupo = encontrar_mayor(personas, num_personas);
    printf("\nPersona de mayor edad: %s (%d años)\n", 
           mayor_grupo->nombre, mayor_grupo->edad);
    
    // Calcular edad promedio
    double promedio = edad_promedio(personas, num_personas);
    printf("Edad promedio del grupo: %.1f años\n", promedio);
    
    // Ordenar por edad
    printf("\n--- Lista Ordenada por Edad ---\n");
    Persona personas_copia[4];
    copiar_array_personas(personas_copia, personas, num_personas);
    ordenar_por_edad(personas_copia, num_personas);
    mostrar_lista_personas(personas_copia, num_personas);
    
    // Buscar por nombre
    printf("\n--- Búsqueda ---\n");
    char nombre_buscar[MAX_NOMBRE];
    printf("Introduce un nombre a buscar: ");
    fgets(nombre_buscar, MAX_NOMBRE, stdin);
    nombre_buscar[strcspn(nombre_buscar, "\n")] = '\0';
    
    int indice = buscar_por_nombre(personas, num_personas, nombre_buscar);
    if (indice >= 0) {
        printf("Encontrado: ");
        mostrar_persona_simple(&personas[indice]);
    } else {
        printf("No se encontró a '%s'\n", nombre_buscar);
    }
    
    // Demostrar validaciones
    printf("\n--- Validaciones ---\n");
    Persona p_valida = {"Juan Pérez", 25};
    Persona p_invalida = {"", -5};
    
    printf("Validando persona válida: ");
    if (validar_persona(&p_valida)) {
        printf("✓ Válida\n");
    } else {
        printf("✗ Inválida\n");
    }
    
    printf("Validando persona inválida: ");
    if (validar_persona(&p_invalida)) {
        printf("✓ Válida\n");
    } else {
        printf("✗ Inválida\n");
    }
    
    printf("\n--- Programa finalizado ---\n");
    return 0;
}
#endif
