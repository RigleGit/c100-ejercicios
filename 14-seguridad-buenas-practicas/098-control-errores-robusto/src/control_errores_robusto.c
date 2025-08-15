#include "control_errores_robusto.h"
#include <limits.h>

// Función insegura que NO verifica errores
int funcion_insegura(void) {
    FILE *archivo = fopen("datos.txt", "w");
    char *buffer = malloc(100);
    int numero;
    
    fprintf(archivo, "Escribiendo datos...\n");
    scanf("%d", &numero);
    sprintf(buffer, "Número: %d", numero);
    fprintf(archivo, "%s\n", buffer);
    
    fclose(archivo);
    free(buffer);
    
    return 0;
}

// Función segura con verificación completa de errores
result_t funcion_segura(void) {
    FILE *archivo = NULL;
    char *buffer = NULL;
    int numero;
    result_t resultado;
    
    // Abrir archivo de forma segura
    resultado = abrir_archivo_seguro("datos.txt", "w", &archivo);
    if (!resultado.success) {
        return resultado;
    }
    
    // Asignar memoria de forma segura
    resultado = asignar_memoria_seguro(100, (void**)&buffer);
    if (!resultado.success) {
        liberar_recursos_seguro(archivo, NULL);
        return resultado;
    }
    
    // Escribir datos de forma segura
    const char *mensaje = "Escribiendo datos...\n";
    resultado = escribir_datos_seguro(archivo, mensaje, strlen(mensaje));
    if (!resultado.success) {
        liberar_recursos_seguro(archivo, buffer);
        return resultado;
    }
    
    // Leer entero de forma segura
    printf("Ingrese un número: ");
    resultado = leer_entero_seguro(&numero);
    if (!resultado.success) {
        liberar_recursos_seguro(archivo, buffer);
        return resultado;
    }
    
    // Formatear string de forma segura
    int ret = snprintf(buffer, 100, "Número: %d\n", numero);
    if (ret < 0 || ret >= 100) {
        liberar_recursos_seguro(archivo, buffer);
        return result_error(ERROR_INVALID_INPUT, "Error al formatear string");
    }
    
    // Escribir buffer de forma segura
    resultado = escribir_datos_seguro(archivo, buffer, strlen(buffer));
    if (!resultado.success) {
        liberar_recursos_seguro(archivo, buffer);
        return resultado;
    }
    
    // Liberar recursos
    liberar_recursos_seguro(archivo, buffer);
    
    return result_ok();
}

result_t abrir_archivo_seguro(const char *filename, const char *mode, FILE **file) {
    if (!filename || !mode || !file) {
        return result_error(ERROR_INVALID_INPUT, "Parámetros nulos en abrir_archivo_seguro");
    }
    
    *file = fopen(filename, mode);
    if (!*file) {
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error al abrir archivo '%s': %s", 
                filename, strerror(errno));
        return result_error(ERROR_FILE_OPEN, mensaje);
    }
    
    return result_ok();
}

result_t asignar_memoria_seguro(size_t size, void **ptr) {
    if (!ptr || size == 0) {
        return result_error(ERROR_INVALID_INPUT, "Parámetros inválidos en asignar_memoria_seguro");
    }
    
    *ptr = malloc(size);
    if (!*ptr) {
        return result_error(ERROR_MEMORY_ALLOC, "Error al asignar memoria: memoria insuficiente");
    }
    
    return result_ok();
}

result_t leer_entero_seguro(int *valor) {
    if (!valor) {
        return result_error(ERROR_INVALID_INPUT, "Puntero nulo en leer_entero_seguro");
    }
    
    char buffer[32];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return result_error(ERROR_INVALID_INPUT, "Error al leer desde stdin");
    }
    
    // Eliminar salto de línea si existe
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    char *endptr;
    errno = 0;
    long temp = strtol(buffer, &endptr, 10);
    
    // Verificar errores de conversión
    if (errno != 0 || endptr == buffer || *endptr != '\0') {
        return result_error(ERROR_INVALID_INPUT, "Entrada no es un número válido");
    }
    
    // Verificar rango de int
    if (temp < INT_MIN || temp > INT_MAX) {
        return result_error(ERROR_INVALID_INPUT, "Número fuera del rango de int");
    }
    
    *valor = (int)temp;
    return result_ok();
}

result_t leer_linea_seguro(FILE *file, char *buffer, size_t size) {
    if (!file || !buffer || size == 0) {
        return result_error(ERROR_INVALID_INPUT, "Parámetros inválidos en leer_linea_seguro");
    }
    
    if (!fgets(buffer, size, file)) {
        if (feof(file)) {
            return result_error(ERROR_FILE_READ, "Fin de archivo alcanzado");
        } else {
            return result_error(ERROR_FILE_READ, "Error al leer del archivo");
        }
    }
    
    return result_ok();
}

result_t escribir_datos_seguro(FILE *file, const void *data, size_t size) {
    if (!file || !data || size == 0) {
        return result_error(ERROR_INVALID_INPUT, "Parámetros inválidos en escribir_datos_seguro");
    }
    
    size_t written = fwrite(data, 1, size, file);
    if (written != size) {
        return result_error(ERROR_FILE_WRITE, "Error al escribir al archivo");
    }
    
    // Forzar escritura al disco
    if (fflush(file) != 0) {
        return result_error(ERROR_FILE_WRITE, "Error al hacer flush del archivo");
    }
    
    return result_ok();
}

void liberar_recursos_seguro(FILE *file, void *ptr) {
    if (file) {
        if (fclose(file) != 0) {
            fprintf(stderr, "Advertencia: Error al cerrar archivo: %s\n", strerror(errno));
        }
    }
    
    if (ptr) {
        free(ptr);
    }
}

result_t result_ok(void) {
    result_t resultado = {0};
    resultado.success = 1;
    resultado.error_code = ERROR_NONE;
    strcpy(resultado.error_message, "Operación exitosa");
    return resultado;
}

result_t result_error(error_code_t code, const char *message) {
    result_t resultado = {0};
    resultado.success = 0;
    resultado.error_code = code;
    
    if (message) {
        strncpy(resultado.error_message, message, sizeof(resultado.error_message) - 1);
        resultado.error_message[sizeof(resultado.error_message) - 1] = '\0';
    } else {
        strcpy(resultado.error_message, "Error desconocido");
    }
    
    return resultado;
}

void imprimir_error(const result_t *result) {
    if (!result || result->success) {
        return;
    }
    
    const char *tipo_error;
    switch (result->error_code) {
        case ERROR_FILE_OPEN:
            tipo_error = "ERROR_ARCHIVO";
            break;
        case ERROR_MEMORY_ALLOC:
            tipo_error = "ERROR_MEMORIA";
            break;
        case ERROR_INVALID_INPUT:
            tipo_error = "ERROR_ENTRADA";
            break;
        case ERROR_FILE_READ:
            tipo_error = "ERROR_LECTURA";
            break;
        case ERROR_FILE_WRITE:
            tipo_error = "ERROR_ESCRITURA";
            break;
        default:
            tipo_error = "ERROR_DESCONOCIDO";
            break;
    }
    
    fprintf(stderr, "[%s] %s\n", tipo_error, result->error_message);
}
