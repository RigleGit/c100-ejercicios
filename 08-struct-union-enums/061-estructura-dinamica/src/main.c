#include "../include/estructura_dinamica.h"

/**
 * @brief Función de demostración interactiva del sistema de gestión de personas
 */
void demo_estructura_dinamica(void) {
    printf("=== DEMOSTRACIÓN: ESTRUCTURAS DINÁMICAS ===\n\n");
    
    // Crear array de personas
    ArrayPersonas *empresa = crear_array_personas();
    if (!empresa) {
        printf("Error: No se pudo crear el array de personas\n");
        return;
    }
    
    printf("1. Creando array dinámico de personas...\n");
    mostrar_informacion_memoria(empresa);
    
    // Agregar personas de ejemplo
    printf("\n2. Agregando personas al array...\n");
    agregar_persona(empresa, "Ana García", "ana.garcia@email.com", 28, 45000.0);
    agregar_persona(empresa, "Carlos López", "carlos.lopez@email.com", 35, 55000.0);
    agregar_persona(empresa, "María Rodríguez", "maria.rodriguez@email.com", 42, 65000.0);
    agregar_persona(empresa, "Juan Pérez", "juan.perez@email.com", 31, 48000.0);
    agregar_persona(empresa, "Elena Torres", "elena.torres@email.com", 26, 42000.0);
    
    imprimir_array_personas(empresa);
    mostrar_informacion_memoria(empresa);
    
    // Calcular y mostrar estadísticas
    printf("\n3. Calculando estadísticas...\n");
    EstadisticasPersonas stats = calcular_estadisticas(empresa);
    imprimir_estadisticas(&stats);
    
    // Demostrar filtrado por edad
    printf("\n4. Filtrando personas por edad (25-35 años)...\n");
    ArrayPersonas *jovenes = filtrar_por_edad(empresa, 25, 35);
    if (jovenes) {
        imprimir_array_personas(jovenes);
        liberar_array_personas(jovenes);
    }
    
    // Demostrar filtrado por salario
    printf("\n5. Filtrando personas por salario ($50,000 - $70,000)...\n");
    ArrayPersonas *salario_alto = filtrar_por_salario(empresa, 50000.0, 70000.0);
    if (salario_alto) {
        imprimir_array_personas(salario_alto);
        liberar_array_personas(salario_alto);
    }
    
    // Demostrar ordenamiento
    printf("\n6. Ordenando por nombre...\n");
    ordenar_por_nombre(empresa);
    imprimir_array_personas(empresa);
    
    printf("\n7. Ordenando por edad...\n");
    ordenar_por_edad(empresa);
    imprimir_array_personas(empresa);
    
    printf("\n8. Ordenando por salario...\n");
    ordenar_por_salario(empresa);
    imprimir_array_personas(empresa);
    
    // Demostrar búsqueda y eliminación
    printf("\n9. Buscando y eliminando 'Carlos López'...\n");
    size_t indice = buscar_persona_por_nombre(empresa, "Carlos López");
    if (indice != SIZE_MAX) {
        printf("Encontrado en índice: %zu\n", indice);
        eliminar_persona_por_nombre(empresa, "Carlos López");
        printf("Eliminado exitosamente.\n");
        imprimir_array_personas(empresa);
    }
    
    // Demostrar actualización de datos
    printf("\n10. Actualizando email de 'Ana García'...\n");
    indice = buscar_persona_por_nombre(empresa, "Ana García");
    if (indice != SIZE_MAX) {
        if (actualizar_email_persona(&empresa->personas[indice], "ana.garcia.nueva@empresa.com")) {
            printf("Email actualizado exitosamente.\n");
            imprimir_persona(&empresa->personas[indice]);
        }
    }
    
    // Agregar más personas para demostrar redimensionamiento
    printf("\n11. Agregando más personas para demostrar redimensionamiento...\n");
    mostrar_informacion_memoria(empresa);
    
    agregar_persona(empresa, "Roberto Silva", "roberto.silva@email.com", 38, 52000.0);
    agregar_persona(empresa, "Lucía Morales", "lucia.morales@email.com", 29, 47000.0);
    agregar_persona(empresa, "Diego Herrera", "diego.herrera@email.com", 33, 51000.0);
    
    printf("Después de agregar más personas:\n");
    mostrar_informacion_memoria(empresa);
    
    // Estadísticas finales
    printf("\n12. Estadísticas finales...\n");
    stats = calcular_estadisticas(empresa);
    imprimir_estadisticas(&stats);
    
    // Demostrar copia de persona
    printf("\n13. Demostrando copia de persona...\n");
    if (empresa->size > 0) {
        Persona *copia = copiar_persona(&empresa->personas[0]);
        if (copia) {
            printf("Original: ");
            imprimir_persona(&empresa->personas[0]);
            printf("Copia: ");
            imprimir_persona(copia);
            liberar_persona(copia);
        }
    }
    
    // Liberación de memoria
    printf("\n14. Liberando memoria...\n");
    size_t memoria_antes = obtener_memoria_utilizada(empresa);
    printf("Memoria utilizada antes de liberar: %zu bytes\n", memoria_antes);
    
    liberar_array_personas(empresa);
    printf("Memoria liberada exitosamente.\n");
    
    printf("\n=== FIN DE LA DEMOSTRACIÓN ===\n");
}

/**
 * @brief Función principal que muestra el uso de estructuras dinámicas
 */
#ifndef UNIT_TESTING
int main(void) {
    printf("Ejercicio 061: Estructuras Dinámicas con malloc\n");
    printf("=============================================\n\n");
    
    // Ejecutar demostración
    demo_estructura_dinamica();
    
    // Ejemplo básico adicional
    printf("\n=== EJEMPLO BÁSICO ===\n");
    
    // Crear una persona individual
    Persona *persona = crear_persona("Test User", "test@example.com", 25, 35000.0);
    if (persona) {
        printf("Persona creada: ");
        imprimir_persona(persona);
        
        // Actualizar información
        actualizar_nombre_persona(persona, "Usuario de Prueba");
        actualizar_email_persona(persona, "usuario.prueba@ejemplo.com");
        
        printf("Persona actualizada: ");
        imprimir_persona(persona);
        
        liberar_persona(persona);
    }
    
    // Crear array simple
    ArrayPersonas *lista = crear_array_personas();
    if (lista) {
        agregar_persona(lista, "Persona 1", "p1@test.com", 20, 30000.0);
        agregar_persona(lista, "Persona 2", "p2@test.com", 30, 40000.0);
        
        printf("\nLista simple:\n");
        imprimir_array_personas(lista);
        
        EstadisticasPersonas stats = calcular_estadisticas(lista);
        imprimir_estadisticas(&stats);
        
        liberar_array_personas(lista);
    }
    
    printf("\n=== PROGRAMA TERMINADO ===\n");
    
    return 0;
}
#endif
