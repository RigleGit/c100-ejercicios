#include "../include/estructura_dinamica.h"

// ============================================================================
// FUNCIONES DE EXPORTACIÓN E IMPORTACIÓN CSV
// ============================================================================

/**
 * @brief Exporta el array de personas a un archivo CSV
 * @param array Puntero al array de personas
 * @param nombre_archivo Nombre del archivo CSV
 * @return true si la exportación fue exitosa
 */
bool exportar_a_csv(const ArrayPersonas *array, const char *nombre_archivo) {
    if (!array || !nombre_archivo) {
        return false;
    }
    
    FILE *archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        return false;
    }
    
    // Escribir encabezado
    fprintf(archivo, "Nombre,Email,Edad,Salario\n");
    
    // Escribir datos
    for (size_t i = 0; i < array->size; i++) {
        const Persona *p = &array->personas[i];
        fprintf(archivo, "\"%s\",\"%s\",%d,%.2f\n",
                p->nombre, p->email, p->edad, p->salario);
    }
    
    fclose(archivo);
    return true;
}

/**
 * @brief Importa personas desde un archivo CSV
 * @param nombre_archivo Nombre del archivo CSV
 * @return Nuevo array con las personas importadas o NULL si hay error
 */
ArrayPersonas* importar_desde_csv(const char *nombre_archivo) {
    if (!nombre_archivo) {
        return NULL;
    }
    
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        return NULL;
    }
    
    ArrayPersonas *array = crear_array_personas();
    if (!array) {
        fclose(archivo);
        return NULL;
    }
    
    char linea[512];
    bool primera_linea = true;
    
    while (fgets(linea, sizeof(linea), archivo)) {
        // Saltar encabezado
        if (primera_linea) {
            primera_linea = false;
            continue;
        }
        
        // Parsear línea CSV
        char nombre[MAX_NOMBRE], email[MAX_EMAIL];
        int edad;
        float salario;
        
        // Simple parser CSV (asume formato correcto)
        char *token = strtok(linea, ",");
        if (!token) continue;
        
        // Quitar comillas del nombre
        if (token[0] == '"' && token[strlen(token)-1] == '"') {
            token[strlen(token)-1] = '\0';
            token++;
        }
        strncpy(nombre, token, MAX_NOMBRE - 1);
        nombre[MAX_NOMBRE - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue;
        
        // Quitar comillas del email
        if (token[0] == '"' && token[strlen(token)-1] == '"') {
            token[strlen(token)-1] = '\0';
            token++;
        }
        strncpy(email, token, MAX_EMAIL - 1);
        email[MAX_EMAIL - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue;
        edad = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        salario = atof(token);
        
        // Agregar persona al array
        if (!agregar_persona(array, nombre, email, edad, salario)) {
            liberar_array_personas(array);
            fclose(archivo);
            return NULL;
        }
    }
    
    fclose(archivo);
    return array;
}
