#include "../include/estructura_dinamica.h"

// ============================================================================
// FUNCIONES PARA GESTIÓN DE PERSONA INDIVIDUAL
// ============================================================================

/**
 * @brief Crea una nueva persona con memoria dinámica
 * @param nombre Nombre de la persona
 * @param email Email de la persona
 * @param edad Edad de la persona
 * @param salario Salario de la persona
 * @return Puntero a la nueva persona o NULL si hay error
 */
Persona* crear_persona(const char *nombre, const char *email, int edad, float salario) {
    if (!nombre || !email || edad < 0 || salario < 0) {
        return NULL;
    }
    
    Persona *persona = malloc(sizeof(Persona));
    if (!persona) {
        return NULL;
    }
    
    // Asignar memoria para el nombre
    persona->nombre = malloc(strlen(nombre) + 1);
    if (!persona->nombre) {
        free(persona);
        return NULL;
    }
    strcpy(persona->nombre, nombre);
    
    // Asignar memoria para el email
    persona->email = malloc(strlen(email) + 1);
    if (!persona->email) {
        free(persona->nombre);
        free(persona);
        return NULL;
    }
    strcpy(persona->email, email);
    
    persona->edad = edad;
    persona->salario = salario;
    
    return persona;
}

/**
 * @brief Libera la memoria de una persona
 * @param persona Puntero a la persona a liberar
 */
void liberar_persona(Persona *persona) {
    if (persona) {
        free(persona->nombre);
        free(persona->email);
        free(persona);
    }
}

/**
 * @brief Verifica si una persona es válida
 * @param persona Puntero a la persona
 * @return true si es válida, false en caso contrario
 */
bool es_persona_valida(const Persona *persona) {
    return persona && persona->nombre && persona->email && 
           persona->edad >= 0 && persona->salario >= 0;
}

/**
 * @brief Imprime la información de una persona
 * @param persona Puntero a la persona
 */
void imprimir_persona(const Persona *persona) {
    if (es_persona_valida(persona)) {
        printf("Nombre: %s, Email: %s, Edad: %d, Salario: %.2f\n",
               persona->nombre, persona->email, persona->edad, persona->salario);
    } else {
        printf("Persona inválida\n");
    }
}

/**
 * @brief Crea una copia de una persona
 * @param original Persona original
 * @return Nueva persona copiada o NULL si hay error
 */
Persona* copiar_persona(const Persona *original) {
    if (!es_persona_valida(original)) {
        return NULL;
    }
    
    return crear_persona(original->nombre, original->email, 
                        original->edad, original->salario);
}

/**
 * @brief Actualiza el nombre de una persona
 * @param persona Puntero a la persona
 * @param nuevo_nombre Nuevo nombre
 * @return true si la actualización fue exitosa
 */
bool actualizar_nombre_persona(Persona *persona, const char *nuevo_nombre) {
    if (!persona || !nuevo_nombre) {
        return false;
    }
    
    char *nuevo_ptr = realloc(persona->nombre, strlen(nuevo_nombre) + 1);
    if (!nuevo_ptr) {
        return false;
    }
    
    persona->nombre = nuevo_ptr;
    strcpy(persona->nombre, nuevo_nombre);
    return true;
}

/**
 * @brief Actualiza el email de una persona
 * @param persona Puntero a la persona
 * @param nuevo_email Nuevo email
 * @return true si la actualización fue exitosa
 */
bool actualizar_email_persona(Persona *persona, const char *nuevo_email) {
    if (!persona || !nuevo_email) {
        return false;
    }
    
    char *nuevo_ptr = realloc(persona->email, strlen(nuevo_email) + 1);
    if (!nuevo_ptr) {
        return false;
    }
    
    persona->email = nuevo_ptr;
    strcpy(persona->email, nuevo_email);
    return true;
}

// ============================================================================
// FUNCIONES PARA GESTIÓN DE ARRAYPERSONAS
// ============================================================================

/**
 * @brief Crea un nuevo array dinámico de personas
 * @return Puntero al array o NULL si hay error
 */
ArrayPersonas* crear_array_personas(void) {
    ArrayPersonas *array = malloc(sizeof(ArrayPersonas));
    if (!array) {
        return NULL;
    }
    
    array->personas = malloc(CAPACIDAD_INICIAL * sizeof(Persona));
    if (!array->personas) {
        free(array);
        return NULL;
    }
    
    array->size = 0;
    array->capacity = CAPACIDAD_INICIAL;
    
    return array;
}

/**
 * @brief Libera la memoria del array de personas
 * @param array Puntero al array
 */
void liberar_array_personas(ArrayPersonas *array) {
    if (array) {
        // Liberar los strings de cada persona (nombre y email)
        for (size_t i = 0; i < array->size; i++) {
            if (array->personas[i].nombre) {
                free(array->personas[i].nombre);
                array->personas[i].nombre = NULL;
            }
            if (array->personas[i].email) {
                free(array->personas[i].email);
                array->personas[i].email = NULL;
            }
        }
        // Liberar el array de personas
        free(array->personas);
        // Liberar la estructura del array
        free(array);
    }
}

/**
 * @brief Redimensiona el array si es necesario
 * @param array Puntero al array
 * @param nueva_capacidad Nueva capacidad
 * @return true si la redimensión fue exitosa
 */
bool redimensionar_array(ArrayPersonas *array, size_t nueva_capacidad) {
    if (!array || nueva_capacidad < array->size) {
        return false;
    }
    
    Persona *nuevo_ptr = realloc(array->personas, nueva_capacidad * sizeof(Persona));
    if (!nuevo_ptr) {
        return false;
    }
    
    array->personas = nuevo_ptr;
    array->capacity = nueva_capacidad;
    
    return true;
}

/**
 * @brief Agrega una nueva persona al array
 * @param array Puntero al array
 * @param nombre Nombre de la persona
 * @param email Email de la persona
 * @param edad Edad de la persona
 * @param salario Salario de la persona
 * @return true si se agregó exitosamente
 */
bool agregar_persona(ArrayPersonas *array, const char *nombre, const char *email, 
                    int edad, float salario) {
    if (!array || !nombre || !email || edad < 0 || salario < 0) {
        return false;
    }
    
    // Redimensionar si es necesario
    if (array->size >= array->capacity) {
        if (!redimensionar_array(array, array->capacity * FACTOR_CRECIMIENTO)) {
            return false;
        }
    }
    
    // Crear directamente en el array
    Persona *p = &array->personas[array->size];
    
    // Asignar memoria para el nombre
    p->nombre = malloc(strlen(nombre) + 1);
    if (!p->nombre) {
        return false;
    }
    strcpy(p->nombre, nombre);
    
    // Asignar memoria para el email
    p->email = malloc(strlen(email) + 1);
    if (!p->email) {
        free(p->nombre);
        return false;
    }
    strcpy(p->email, email);
    
    p->edad = edad;
    p->salario = salario;
    
    array->size++;
    return true;
}

/**
 * @brief Elimina una persona por índice
 * @param array Puntero al array
 * @param indice Índice de la persona a eliminar
 * @return true si se eliminó exitosamente
 */
bool eliminar_persona_por_indice(ArrayPersonas *array, size_t indice) {
    if (!array || indice >= array->size) {
        return false;
    }
    
    // Liberar memoria de la persona a eliminar
    free(array->personas[indice].nombre);
    free(array->personas[indice].email);
    
    // Mover elementos hacia la izquierda
    for (size_t i = indice; i < array->size - 1; i++) {
        array->personas[i] = array->personas[i + 1];
    }
    
    array->size--;
    
    // Reducir capacidad si es necesario
    if (array->size < array->capacity / 4 && array->capacity > CAPACIDAD_INICIAL) {
        redimensionar_array(array, array->capacity / 2);
    }
    
    return true;
}

/**
 * @brief Busca una persona por nombre
 * @param array Puntero al array
 * @param nombre Nombre a buscar
 * @return Índice de la persona o SIZE_MAX si no se encuentra
 */
size_t buscar_persona_por_nombre(const ArrayPersonas *array, const char *nombre) {
    if (!array || !nombre) {
        return SIZE_MAX;
    }
    
    for (size_t i = 0; i < array->size; i++) {
        if (array->personas[i].nombre && strcmp(array->personas[i].nombre, nombre) == 0) {
            return i;
        }
    }
    
    return SIZE_MAX;
}

/**
 * @brief Elimina una persona por nombre
 * @param array Puntero al array
 * @param nombre Nombre de la persona a eliminar
 * @return true si se eliminó exitosamente
 */
bool eliminar_persona_por_nombre(ArrayPersonas *array, const char *nombre) {
    size_t indice = buscar_persona_por_nombre(array, nombre);
    if (indice == SIZE_MAX) {
        return false;
    }
    
    return eliminar_persona_por_indice(array, indice);
}

/**
 * @brief Imprime todas las personas del array
 * @param array Puntero al array
 */
void imprimir_array_personas(const ArrayPersonas *array) {
    if (!array) {
        printf("Array inválido\n");
        return;
    }
    
    printf("\n=== Array de Personas (Total: %zu) ===\n", array->size);
    for (size_t i = 0; i < array->size; i++) {
        printf("[%zu] ", i);
        imprimir_persona(&array->personas[i]);
    }
    printf("======================================\n");
}

// ============================================================================
// FUNCIONES DE FILTRADO
// ============================================================================

/**
 * @brief Filtra personas por rango de edad
 * @param array Array original
 * @param edad_min Edad mínima
 * @param edad_max Edad máxima
 * @return Nuevo array filtrado o NULL si hay error
 */
ArrayPersonas* filtrar_por_edad(const ArrayPersonas *array, int edad_min, int edad_max) {
    if (!array || edad_min > edad_max) {
        return NULL;
    }
    
    ArrayPersonas *filtrado = crear_array_personas();
    if (!filtrado) {
        return NULL;
    }
    
    for (size_t i = 0; i < array->size; i++) {
        const Persona *p = &array->personas[i];
        if (p->edad >= edad_min && p->edad <= edad_max) {
            if (!agregar_persona(filtrado, p->nombre, p->email, p->edad, p->salario)) {
                liberar_array_personas(filtrado);
                return NULL;
            }
        }
    }
    
    return filtrado;
}

/**
 * @brief Filtra personas por rango de salario
 * @param array Array original
 * @param salario_min Salario mínimo
 * @param salario_max Salario máximo
 * @return Nuevo array filtrado o NULL si hay error
 */
ArrayPersonas* filtrar_por_salario(const ArrayPersonas *array, float salario_min, float salario_max) {
    if (!array || salario_min > salario_max) {
        return NULL;
    }
    
    ArrayPersonas *filtrado = crear_array_personas();
    if (!filtrado) {
        return NULL;
    }
    
    for (size_t i = 0; i < array->size; i++) {
        const Persona *p = &array->personas[i];
        if (p->salario >= salario_min && p->salario <= salario_max) {
            if (!agregar_persona(filtrado, p->nombre, p->email, p->edad, p->salario)) {
                liberar_array_personas(filtrado);
                return NULL;
            }
        }
    }
    
    return filtrado;
}

// ============================================================================
// FUNCIONES DE ESTADÍSTICAS
// ============================================================================

/**
 * @brief Calcula estadísticas del array de personas
 * @param array Puntero al array
 * @return Estructura con las estadísticas
 */
EstadisticasPersonas calcular_estadisticas(const ArrayPersonas *array) {
    EstadisticasPersonas stats = {0};
    
    if (!array || array->size == 0) {
        return stats;
    }
    
    stats.total_personas = array->size;
    stats.edad_minima = array->personas[0].edad;
    stats.edad_maxima = array->personas[0].edad;
    stats.salario_minimo = array->personas[0].salario;
    stats.salario_maximo = array->personas[0].salario;
    
    float suma_edad = 0, suma_salario = 0;
    
    for (size_t i = 0; i < array->size; i++) {
        const Persona *p = &array->personas[i];
        
        suma_edad += p->edad;
        suma_salario += p->salario;
        
        if (p->edad < stats.edad_minima) stats.edad_minima = p->edad;
        if (p->edad > stats.edad_maxima) stats.edad_maxima = p->edad;
        if (p->salario < stats.salario_minimo) stats.salario_minimo = p->salario;
        if (p->salario > stats.salario_maximo) stats.salario_maximo = p->salario;
    }
    
    stats.edad_promedio = suma_edad / array->size;
    stats.salario_promedio = suma_salario / array->size;
    
    return stats;
}

/**
 * @brief Imprime las estadísticas
 * @param stats Puntero a las estadísticas
 */
void imprimir_estadisticas(const EstadisticasPersonas *stats) {
    if (!stats || stats->total_personas == 0) {
        printf("No hay datos para mostrar estadísticas\n");
        return;
    }
    
    printf("\n=== ESTADÍSTICAS ===\n");
    printf("Total de personas: %zu\n", stats->total_personas);
    printf("Edad promedio: %.2f años\n", stats->edad_promedio);
    printf("Edad mínima: %d años\n", stats->edad_minima);
    printf("Edad máxima: %d años\n", stats->edad_maxima);
    printf("Salario promedio: $%.2f\n", stats->salario_promedio);
    printf("Salario mínimo: $%.2f\n", stats->salario_minimo);
    printf("Salario máximo: $%.2f\n", stats->salario_maximo);
    printf("==================\n");
}

// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

/**
 * @brief Calcula la memoria utilizada por el array
 * @param array Puntero al array
 * @return Bytes utilizados
 */
size_t obtener_memoria_utilizada(const ArrayPersonas *array) {
    if (!array) {
        return 0;
    }
    
    size_t memoria = sizeof(ArrayPersonas);
    memoria += array->capacity * sizeof(Persona);
    
    for (size_t i = 0; i < array->size; i++) {
        const Persona *p = &array->personas[i];
        if (p->nombre) memoria += strlen(p->nombre) + 1;
        if (p->email) memoria += strlen(p->email) + 1;
    }
    
    return memoria;
}

/**
 * @brief Muestra información sobre el uso de memoria
 * @param array Puntero al array
 */
void mostrar_informacion_memoria(const ArrayPersonas *array) {
    if (!array) {
        printf("Array inválido\n");
        return;
    }
    
    size_t memoria_total = obtener_memoria_utilizada(array);
    size_t memoria_estructura = sizeof(ArrayPersonas) + array->capacity * sizeof(Persona);
    size_t memoria_strings = memoria_total - memoria_estructura;
    
    printf("\n=== INFORMACIÓN DE MEMORIA ===\n");
    printf("Capacidad: %zu elementos\n", array->capacity);
    printf("Elementos utilizados: %zu\n", array->size);
    printf("Factor de utilización: %.2f%%\n", 
           (double)array->size / array->capacity * 100);
    printf("Memoria total: %zu bytes\n", memoria_total);
    printf("Memoria estructuras: %zu bytes\n", memoria_estructura);
    printf("Memoria strings: %zu bytes\n", memoria_strings);
    printf("Memoria promedio por persona: %.2f bytes\n",
           array->size > 0 ? (double)memoria_total / array->size : 0);
    printf("=============================\n");
}

// ============================================================================
// FUNCIONES DE ORDENAMIENTO
// ============================================================================

// Función auxiliar para intercambiar dos personas
static void intercambiar_personas(Persona *a, Persona *b) {
    Persona temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Ordena el array por nombre (alfabéticamente)
 * @param array Puntero al array
 */
void ordenar_por_nombre(ArrayPersonas *array) {
    if (!array || array->size < 2) {
        return;
    }
    
    // Ordenamiento burbuja por simplicidad pedagógica
    for (size_t i = 0; i < array->size - 1; i++) {
        for (size_t j = 0; j < array->size - 1 - i; j++) {
            if (strcmp(array->personas[j].nombre, array->personas[j + 1].nombre) > 0) {
                intercambiar_personas(&array->personas[j], &array->personas[j + 1]);
            }
        }
    }
}

/**
 * @brief Ordena el array por edad (ascendente)
 * @param array Puntero al array
 */
void ordenar_por_edad(ArrayPersonas *array) {
    if (!array || array->size < 2) {
        return;
    }
    
    for (size_t i = 0; i < array->size - 1; i++) {
        for (size_t j = 0; j < array->size - 1 - i; j++) {
            if (array->personas[j].edad > array->personas[j + 1].edad) {
                intercambiar_personas(&array->personas[j], &array->personas[j + 1]);
            }
        }
    }
}

/**
 * @brief Ordena el array por salario (ascendente)
 * @param array Puntero al array
 */
void ordenar_por_salario(ArrayPersonas *array) {
    if (!array || array->size < 2) {
        return;
    }
    
    for (size_t i = 0; i < array->size - 1; i++) {
        for (size_t j = 0; j < array->size - 1 - i; j++) {
            if (array->personas[j].salario > array->personas[j + 1].salario) {
                intercambiar_personas(&array->personas[j], &array->personas[j + 1]);
            }
        }
    }
}
