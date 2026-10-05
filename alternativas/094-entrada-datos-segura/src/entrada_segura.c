/**
 * @file entrada_segura.c
 * @brief Implementación de funciones para entrada de datos segura
 */

#include "entrada_segura.h"
#include <time.h>
#include <sys/time.h>

/* ================================
 * FUNCIONES AUXILIARES INTERNAS
 * ================================ */

/**
 * @brief Obtiene timestamp actual
 */
static double obtener_timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/* ================================
 * IMPLEMENTACIÓN DE FUNCIONES PRINCIPALES
 * ================================ */

resultado_entrada_t entrada_cadena_segura(char *buffer, size_t size, FILE *stream) {
    if (!buffer || size == 0 || !stream) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    // Limpiar buffer
    memset(buffer, 0, size);
    
    // Usar fgets para lectura segura
    if (fgets(buffer, size, stream) == NULL) {
        if (feof(stream)) {
            return ENTRADA_ERROR_EOF;
        }
        return ENTRADA_ERROR_FORMATO;
    }
    
    // Verificar si se leyó la línea completa
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        // Eliminar el salto de línea
        buffer[len - 1] = '\0';
    } else if (len == size - 1) {
        // Posible overflow, limpiar buffer de entrada
        limpiar_buffer_entrada(stream);
        return ENTRADA_ERROR_BUFFER_OVERFLOW;
    }
    
    return ENTRADA_OK;
}

resultado_entrada_t entrada_linea_configurada(char *buffer, size_t size, 
                                              const config_entrada_t *config,
                                              stats_entrada_t *stats) {
    if (!buffer || size == 0 || !config) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    double tiempo_inicio = obtener_timestamp();
    int intentos = 0;
    resultado_entrada_t resultado;
    
    if (stats) {
        entrada_init_stats(stats);
    }
    
    do {
        intentos++;
        
        // Mostrar prompt si está configurado
        if (strlen(config->prompt) > 0) {
            printf("%s", config->prompt);
            fflush(stdout);
        }
        
        resultado = entrada_cadena_segura(buffer, size, stdin);
        
        if (resultado == ENTRADA_OK) {
            // Aplicar trim si está configurado
            if (config->trim_whitespace) {
                trim_whitespace(buffer);
            }
            
            // Verificar si se permite entrada vacía
            if (!config->permitir_vacio && strlen(buffer) == 0) {
                if (config->mostrar_errores) {
                    printf("Error: La entrada no puede estar vacía.\n");
                }
                resultado = ENTRADA_ERROR_FORMATO;
                continue;
            }
            
            break;
            
        } else {
            if (stats) {
                if (resultado == ENTRADA_ERROR_BUFFER_OVERFLOW) {
                    stats->errores_overflow++;
                } else {
                    stats->errores_formato++;
                }
            }
            
            if (config->mostrar_errores) {
                if (strlen(config->error_message) > 0) {
                    printf("%s\n", config->error_message);
                } else {
                    printf("Error: %s\n", entrada_error_string(resultado));
                }
            }
        }
        
    } while (intentos < config->max_reintentos);
    
    if (stats) {
        stats->intentos_realizados = intentos;
        stats->tiempo_total = obtener_timestamp() - tiempo_inicio;
        if (resultado == ENTRADA_OK) {
            stats->caracteres_leidos = strlen(buffer);
        }
    }
    
    if (intentos >= config->max_reintentos && resultado != ENTRADA_OK) {
        return ENTRADA_ERROR_DEMASIADOS_INTENTOS;
    }
    
    return resultado;
}

resultado_entrada_t entrada_entero_seguro(int *valor, int min, int max, const char *prompt) {
    if (!valor) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    char buffer[32];
    char *endptr;
    long temp;
    int intentos = 0;
    
    do {
        intentos++;
        
        if (prompt) {
            printf("%s (%d - %d): ", prompt, min, max);
            fflush(stdout);
        }
        
        resultado_entrada_t resultado = entrada_cadena_segura(buffer, sizeof(buffer), stdin);
        if (resultado != ENTRADA_OK) {
            printf("Error al leer entrada: %s\n", entrada_error_string(resultado));
            continue;
        }
        
        // Verificar entrada vacía
        if (strlen(buffer) == 0) {
            printf("Error: Debes introducir un número.\n");
            continue;
        }
        
        // Convertir a entero
        errno = 0;
        temp = strtol(buffer, &endptr, 10);
        
        // Verificar errores de conversión
        if (errno == ERANGE || temp < INT_MIN || temp > INT_MAX) {
            printf("Error: Número fuera del rango de enteros.\n");
            continue;
        }
        
        if (endptr == buffer || *endptr != '\0') {
            printf("Error: Entrada no es un número válido.\n");
            continue;
        }
        
        // Verificar rango
        if (temp < min || temp > max) {
            printf("Error: El número debe estar entre %d y %d.\n", min, max);
            continue;
        }
        
        *valor = (int)temp;
        return ENTRADA_OK;
        
    } while (intentos < MAX_REINTENTOS);
    
    return ENTRADA_ERROR_DEMASIADOS_INTENTOS;
}

resultado_entrada_t entrada_flotante_seguro(double *valor, double min, double max, const char *prompt) {
    if (!valor) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    char buffer[64];
    char *endptr;
    double temp;
    int intentos = 0;
    
    do {
        intentos++;
        
        if (prompt) {
            printf("%s (%.2f - %.2f): ", prompt, min, max);
            fflush(stdout);
        }
        
        resultado_entrada_t resultado = entrada_cadena_segura(buffer, sizeof(buffer), stdin);
        if (resultado != ENTRADA_OK) {
            printf("Error al leer entrada: %s\n", entrada_error_string(resultado));
            continue;
        }
        
        if (strlen(buffer) == 0) {
            printf("Error: Debes introducir un número.\n");
            continue;
        }
        
        errno = 0;
        temp = strtod(buffer, &endptr);
        
        if (errno == ERANGE) {
            printf("Error: Número fuera del rango válido.\n");
            continue;
        }
        
        if (endptr == buffer || *endptr != '\0') {
            printf("Error: Entrada no es un número válido.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Error: El número debe estar entre %.2f y %.2f.\n", min, max);
            continue;
        }
        
        *valor = temp;
        return ENTRADA_OK;
        
    } while (intentos < MAX_REINTENTOS);
    
    return ENTRADA_ERROR_DEMASIADOS_INTENTOS;
}

resultado_entrada_t entrada_caracter_seguro(char *caracter, const char *validos, const char *prompt) {
    if (!caracter) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    char buffer[8];
    int intentos = 0;
    
    do {
        intentos++;
        
        if (prompt) {
            printf("%s", prompt);
            if (validos) {
                printf(" [%s]", validos);
            }
            printf(": ");
            fflush(stdout);
        }
        
        resultado_entrada_t resultado = entrada_cadena_segura(buffer, sizeof(buffer), stdin);
        if (resultado != ENTRADA_OK) {
            printf("Error al leer entrada: %s\n", entrada_error_string(resultado));
            continue;
        }
        
        if (strlen(buffer) != 1) {
            printf("Error: Debes introducir exactamente un carácter.\n");
            continue;
        }
        
        if (validos && strchr(validos, buffer[0]) == NULL) {
            printf("Error: Carácter no válido. Usa uno de: %s\n", validos);
            continue;
        }
        
        *caracter = buffer[0];
        return ENTRADA_OK;
        
    } while (intentos < MAX_REINTENTOS);
    
    return ENTRADA_ERROR_DEMASIADOS_INTENTOS;
}

resultado_entrada_t entrada_si_no(bool *respuesta, const char *prompt) {
    if (!respuesta) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    char caracter;
    resultado_entrada_t resultado = entrada_caracter_seguro(&caracter, "SsNn", prompt);
    
    if (resultado == ENTRADA_OK) {
        *respuesta = (caracter == 'S' || caracter == 's');
    }
    
    return resultado;
}

/* ================================
 * FUNCIONES DE VALIDACIÓN
 * ================================ */

bool validar_solo_letras(const char *cadena) {
    if (!cadena) return false;
    
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        if (!isalpha((unsigned char)cadena[i])) {
            return false;
        }
    }
    return true;
}

bool validar_solo_numeros(const char *cadena) {
    if (!cadena || strlen(cadena) == 0) return false;
    
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        if (!isdigit((unsigned char)cadena[i])) {
            return false;
        }
    }
    return true;
}

bool validar_alfanumerico(const char *cadena) {
    if (!cadena) return false;
    
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        if (!isalnum((unsigned char)cadena[i])) {
            return false;
        }
    }
    return true;
}

bool validar_email(const char *email) {
    if (!email || strlen(email) == 0) return false;
    
    // Validación básica de email
    const char *arroba = strchr(email, '@');
    if (!arroba || arroba == email) return false;
    
    const char *punto = strchr(arroba, '.');
    if (!punto || punto == arroba + 1 || punto[1] == '\0') return false;
    
    // Verificar que no haya caracteres ilegales
    for (size_t i = 0; email[i] != '\0'; i++) {
        char c = email[i];
        if (!isalnum(c) && c != '@' && c != '.' && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool validar_longitud(const char *cadena, size_t min_len, size_t max_len) {
    if (!cadena) return false;
    
    size_t len = strlen(cadena);
    return len >= min_len && len <= max_len;
}

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

void limpiar_buffer_entrada(FILE *stream) {
    int c;
    while ((c = fgetc(stream)) != '\n' && c != EOF) {
        // Descartar caracteres hasta el final de línea
    }
}

char* trim_whitespace(char *cadena) {
    if (!cadena) return NULL;
    
    // Eliminar espacios del final
    size_t len = strlen(cadena);
    while (len > 0 && isspace((unsigned char)cadena[len - 1])) {
        cadena[--len] = '\0';
    }
    
    // Encontrar el inicio sin espacios
    char *inicio = cadena;
    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    
    // Mover el contenido si es necesario
    if (inicio != cadena) {
        memmove(cadena, inicio, strlen(inicio) + 1);
    }
    
    return cadena;
}

char* a_minusculas(char *cadena) {
    if (!cadena) return NULL;
    
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = tolower((unsigned char)cadena[i]);
    }
    
    return cadena;
}

char* a_mayusculas(char *cadena) {
    if (!cadena) return NULL;
    
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = toupper((unsigned char)cadena[i]);
    }
    
    return cadena;
}

const char* entrada_error_string(resultado_entrada_t resultado) {
    switch (resultado) {
        case ENTRADA_OK:
            return "Éxito";
        case ENTRADA_ERROR_BUFFER_OVERFLOW:
            return "Desbordamiento de buffer";
        case ENTRADA_ERROR_EOF:
            return "Final de archivo";
        case ENTRADA_ERROR_FORMATO:
            return "Formato inválido";
        case ENTRADA_ERROR_RANGO:
            return "Valor fuera de rango";
        case ENTRADA_ERROR_MEMORIA:
            return "Error de memoria";
        case ENTRADA_ERROR_PARAMETRO:
            return "Parámetro inválido";
        case ENTRADA_ERROR_TIMEOUT:
            return "Timeout";
        case ENTRADA_ERROR_DEMASIADOS_INTENTOS:
            return "Demasiados intentos fallidos";
        default:
            return "Error desconocido";
    }
}

/* ================================
 * FUNCIONES DE CONFIGURACIÓN
 * ================================ */

void entrada_init_config(config_entrada_t *config) {
    if (!config) return;
    
    memset(config, 0, sizeof(config_entrada_t));
    config->buffer_size = BUFFER_SIZE_DEFAULT;
    config->max_reintentos = MAX_REINTENTOS;
    config->trim_whitespace = true;
    config->permitir_vacio = false;
    config->mostrar_errores = true;
    strcpy(config->prompt, "Introduce valor: ");
    strcpy(config->error_message, "Entrada inválida, intenta de nuevo.");
}

void entrada_init_stats(stats_entrada_t *stats) {
    if (!stats) return;
    
    memset(stats, 0, sizeof(stats_entrada_t));
}

void entrada_imprimir_stats(const stats_entrada_t *stats) {
    if (!stats) return;
    
    printf("\n=== Estadísticas de Entrada ===\n");
    printf("Intentos realizados: %d\n", stats->intentos_realizados);
    printf("Errores de overflow: %d\n", stats->errores_overflow);
    printf("Errores de formato: %d\n", stats->errores_formato);
    printf("Errores de rango: %d\n", stats->errores_rango);
    printf("Caracteres leídos: %zu\n", stats->caracteres_leidos);
    printf("Tiempo total: %.3f ms\n", stats->tiempo_total * 1000);
}

/* ================================
 * FUNCIONES WRAPPER SEGURAS
 * ================================ */

char* gets_seguro(char *buffer, size_t size) {
    resultado_entrada_t resultado = entrada_cadena_segura(buffer, size, stdin);
    return (resultado == ENTRADA_OK) ? buffer : NULL;
}

resultado_entrada_t strcpy_seguro(char *destino, const char *origen, size_t size) {
    if (!destino || !origen || size == 0) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    size_t len_origen = strlen(origen);
    if (len_origen >= size) {
        return ENTRADA_ERROR_BUFFER_OVERFLOW;
    }
    
    memcpy(destino, origen, len_origen + 1);
    return ENTRADA_OK;
}

resultado_entrada_t strcat_seguro(char *destino, const char *origen, size_t size) {
    if (!destino || !origen || size == 0) {
        return ENTRADA_ERROR_PARAMETRO;
    }
    
    size_t len_destino = strlen(destino);
    size_t len_origen = strlen(origen);
    
    if (len_destino + len_origen >= size) {
        return ENTRADA_ERROR_BUFFER_OVERFLOW;
    }
    
    memcpy(destino + len_destino, origen, len_origen + 1);
    return ENTRADA_OK;
}

/* ================================
 * FUNCIONES EDUCATIVAS
 * ================================ */

void mostrar_vulnerabilidades_comunes(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                  VULNERABILIDADES COMUNES                   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("🚨 FUNCIONES PELIGROSAS:\n\n");
    
    printf("1. gets() - NUNCA USAR\n");
    printf("   Problema: No verifica límites del buffer\n");
    printf("   Código vulnerable:\n");
    printf("     char buffer[10];\n");
    printf("     gets(buffer); // Puede escribir más de 10 caracteres\n");
    printf("   \n");
    printf("   Solución: Usar fgets()\n");
    printf("     fgets(buffer, sizeof(buffer), stdin);\n\n");
    
    printf("2. scanf() sin límites\n");
    printf("   Problema: %%s sin especificar tamaño máximo\n");
    printf("   Código vulnerable:\n");
    printf("     char nombre[20];\n");
    printf("     scanf(\"%%s\", nombre); // Puede escribir más de 20 caracteres\n");
    printf("   \n");
    printf("   Solución: Especificar límite\n");
    printf("     scanf(\"%%19s\", nombre); // Máximo 19 caracteres + '\\0'\n\n");
    
    printf("3. strcpy() sin verificación\n");
    printf("   Problema: No verifica si el destino es suficientemente grande\n");
    printf("   Código vulnerable:\n");
    printf("     char dest[10];\n");
    printf("     strcpy(dest, \"Una cadena muy larga\"); // Overflow\n");
    printf("   \n");
    printf("   Solución: Usar strncpy() o verificar tamaños\n");
    printf("     strncpy(dest, origen, sizeof(dest) - 1);\n");
    printf("     dest[sizeof(dest) - 1] = '\\0';\n\n");
    
    printf("4. strcat() sin verificación\n");
    printf("   Problema: No verifica espacio disponible\n");
    printf("   Código vulnerable:\n");
    printf("     char buffer[10] = \"Hello\";\n");
    printf("     strcat(buffer, \" World!\"); // Puede causar overflow\n");
    printf("   \n");
    printf("   Solución: Usar strncat() o verificar espacio\n");
    printf("     strncat(buffer, \" World!\", sizeof(buffer) - strlen(buffer) - 1);\n\n");
}

void mostrar_buenas_practicas(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     BUENAS PRÁCTICAS                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("✅ PRINCIPIOS DE SEGURIDAD:\n\n");
    
    printf("1. VALIDACIÓN DE ENTRADA\n");
    printf("   • Siempre validar datos de entrada\n");
    printf("   • Verificar rangos y formatos\n");
    printf("   • Sanitizar datos antes de usar\n");
    printf("   • Nunca confiar en entrada del usuario\n\n");
    
    printf("2. GESTIÓN DE BUFFERS\n");
    printf("   • Siempre especificar tamaños máximos\n");
    printf("   • Verificar límites antes de escribir\n");
    printf("   • Inicializar buffers con datos conocidos\n");
    printf("   • Null-terminar cadenas explícitamente\n\n");
    
    printf("3. MANEJO DE ERRORES\n");
    printf("   • Verificar valores de retorno\n");
    printf("   • Proporcionar mensajes de error claros\n");
    printf("   • Implementar recuperación de errores\n");
    printf("   • Limpiar recursos en caso de error\n\n");
    
    printf("4. FUNCIONES SEGURAS RECOMENDADAS\n");
    printf("   • fgets() en lugar de gets()\n");
    printf("   • snprintf() en lugar de sprintf()\n");
    printf("   • strncpy() en lugar de strcpy()\n");
    printf("   • strncat() en lugar de strcat()\n");
    printf("   • scanf() con límites de campo\n\n");
    
    printf("5. TÉCNICAS ADICIONALES\n");
    printf("   • Usar herramientas de análisis estático\n");
    printf("   • Compilar con flags de seguridad\n");
    printf("   • Usar sanitizers durante desarrollo\n");
    printf("   • Realizar pruebas de fuzzing\n");
    printf("   • Code review enfocado en seguridad\n\n");
}

void comparar_funciones_seguridad(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               COMPARACIÓN FUNCIONES SEGURAS                 ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("📊 TABLA DE COMPARACIÓN:\n\n");
    printf("┌─────────────────┬─────────────────┬─────────────────────────┐\n");
    printf("│   INSEGURA      │     SEGURA      │       DIFERENCIA        │\n");
    printf("├─────────────────┼─────────────────┼─────────────────────────┤\n");
    printf("│ gets()          │ fgets()         │ Limita tamaño de buffer │\n");
    printf("│ scanf(\"%%s\")     │ scanf(\"%%19s\")   │ Especifica tamaño max   │\n");
    printf("│ strcpy()        │ strncpy()       │ Limita bytes copiados   │\n");
    printf("│ strcat()        │ strncat()       │ Limita bytes añadidos   │\n");
    printf("│ sprintf()       │ snprintf()      │ Previene overflow       │\n");
    printf("└─────────────────┴─────────────────┴─────────────────────────┘\n\n");
    
    printf("📝 EJEMPLOS PRÁCTICOS:\n\n");
    
    printf("ENTRADA DE TEXTO:\n");
    printf("❌ Inseguro:\n");
    printf("   char nombre[20];\n");
    printf("   gets(nombre);\n\n");
    printf("✅ Seguro:\n");
    printf("   char nombre[20];\n");
    printf("   if (fgets(nombre, sizeof(nombre), stdin)) {\n");
    printf("       // Eliminar \\n si existe\n");
    printf("       nombre[strcspn(nombre, \"\\n\")] = '\\0';\n");
    printf("   }\n\n");
    
    printf("COPIA DE CADENAS:\n");
    printf("❌ Inseguro:\n");
    printf("   strcpy(destino, origen);\n\n");
    printf("✅ Seguro:\n");
    printf("   strncpy(destino, origen, sizeof(destino) - 1);\n");
    printf("   destino[sizeof(destino) - 1] = '\\0';\n\n");
    
    printf("FORMATEO DE CADENAS:\n");
    printf("❌ Inseguro:\n");
    printf("   sprintf(buffer, \"Hola %%s\", nombre);\n\n");
    printf("✅ Seguro:\n");
    printf("   snprintf(buffer, sizeof(buffer), \"Hola %%s\", nombre);\n\n");
}

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

int demo_comparacion_seguridad(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║            DEMO: COMPARACIÓN ENTRADA SEGURA                 ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    char nombre[10];
    
    printf("Este demo muestra la diferencia entre entrada segura e insegura.\n");
    printf("Buffer de tamaño: %zu caracteres\n\n", sizeof(nombre));
    
    printf("=== MÉTODO SEGURO CON fgets() ===\n");
    printf("Introduce tu nombre (máximo %zu caracteres): ", sizeof(nombre) - 1);
    fflush(stdout);
    
    resultado_entrada_t resultado = entrada_cadena_segura(nombre, sizeof(nombre), stdin);
    
    if (resultado == ENTRADA_OK) {
        printf("✅ Entrada segura exitosa: '%s'\n", nombre);
        printf("Longitud: %zu caracteres\n", strlen(nombre));
    } else if (resultado == ENTRADA_ERROR_BUFFER_OVERFLOW) {
        printf("⚠️  Se detectó intento de overflow - entrada truncada de forma segura\n");
        printf("Contenido del buffer: '%s'\n", nombre);
        printf("El buffer se mantuvo seguro y null-terminado\n");
    } else {
        printf("❌ Error: %s\n", entrada_error_string(resultado));
    }
    
    printf("\n=== EXPLICACIÓN ===\n");
    printf("• fgets() lee máximo N-1 caracteres + '\\0'\n");
    printf("• Si la entrada es muy larga, se trunca de forma segura\n");
    printf("• El buffer siempre queda null-terminado\n");
    printf("• Se detecta el overflow y se limpia el buffer de entrada\n");
    printf("• No hay corrupción de memoria ni comportamiento indefinido\n");
    
    return 0;
}

int demo_tipos_entrada(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              DEMO: TIPOS DE ENTRADA SEGURA                  ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Entrada de entero
    int edad;
    resultado_entrada_t resultado = entrada_entero_seguro(&edad, 0, 120, "Introduce tu edad");
    if (resultado == ENTRADA_OK) {
        printf("✅ Edad ingresada: %d años\n", edad);
    } else {
        printf("❌ Error al leer edad: %s\n", entrada_error_string(resultado));
    }
    
    // Entrada de flotante
    double altura;
    resultado = entrada_flotante_seguro(&altura, 0.5, 3.0, "Introduce tu altura (m)");
    if (resultado == ENTRADA_OK) {
        printf("✅ Altura ingresada: %.2f metros\n", altura);
    } else {
        printf("❌ Error al leer altura: %s\n", entrada_error_string(resultado));
    }
    
    // Entrada de carácter
    char inicial;
    resultado = entrada_caracter_seguro(&inicial, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 
                                       "Introduce la inicial de tu apellido");
    if (resultado == ENTRADA_OK) {
        printf("✅ Inicial ingresada: %c\n", inicial);
    } else {
        printf("❌ Error al leer inicial: %s\n", entrada_error_string(resultado));
    }
    
    // Entrada sí/no
    bool activo;
    resultado = entrada_si_no(&activo, "¿Quieres continuar? (S/N)");
    if (resultado == ENTRADA_OK) {
        printf("✅ Respuesta: %s\n", activo ? "Sí" : "No");
    } else {
        printf("❌ Error al leer respuesta: %s\n", entrada_error_string(resultado));
    }
    
    return 0;
}

int demo_validacion(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               DEMO: VALIDACIÓN DE ENTRADA                   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    char buffer[64];
    
    // Validación de solo letras
    printf("=== VALIDACIÓN: Solo letras ===\n");
    printf("Introduce un nombre (solo letras): ");
    fflush(stdout);
    
    if (entrada_cadena_segura(buffer, sizeof(buffer), stdin) == ENTRADA_OK) {
        trim_whitespace(buffer);
        if (validar_solo_letras(buffer)) {
            printf("✅ Nombre válido: %s\n", buffer);
        } else {
            printf("❌ Error: El nombre debe contener solo letras\n");
        }
    }
    
    // Validación de email
    printf("\n=== VALIDACIÓN: Email ===\n");
    printf("Introduce un email: ");
    fflush(stdout);
    
    if (entrada_cadena_segura(buffer, sizeof(buffer), stdin) == ENTRADA_OK) {
        trim_whitespace(buffer);
        if (validar_email(buffer)) {
            printf("✅ Email válido: %s\n", buffer);
        } else {
            printf("❌ Error: Formato de email inválido\n");
        }
    }
    
    // Validación de longitud
    printf("\n=== VALIDACIÓN: Longitud ===\n");
    printf("Introduce una contraseña (8-20 caracteres): ");
    fflush(stdout);
    
    if (entrada_cadena_segura(buffer, sizeof(buffer), stdin) == ENTRADA_OK) {
        trim_whitespace(buffer);
        if (validar_longitud(buffer, 8, 20)) {
            printf("✅ Contraseña válida (longitud: %zu)\n", strlen(buffer));
        } else {
            printf("❌ Error: La contraseña debe tener entre 8 y 20 caracteres\n");
        }
    }
    
    return 0;
}

int demo_configuracion_avanzada(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║            DEMO: CONFIGURACIÓN AVANZADA                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    char buffer[32];
    config_entrada_t config;
    stats_entrada_t stats;
    
    // Configurar entrada personalizada
    entrada_init_config(&config);
    config.max_reintentos = 5;
    config.trim_whitespace = true;
    config.permitir_vacio = false;
    strcpy(config.prompt, "Código de producto (6-10 caracteres alfanuméricos): ");
    strcpy(config.error_message, "❌ Código inválido. Debe ser alfanumérico de 6-10 caracteres.");
    
    printf("Demo de entrada con configuración personalizada\n");
    printf("• Máximo 5 intentos\n");
    printf("• Trim automático de espacios\n");
    printf("• No se permite entrada vacía\n");
    printf("• Validación de longitud\n\n");
    
    resultado_entrada_t resultado = entrada_linea_configurada(buffer, sizeof(buffer), &config, &stats);
    
    if (resultado == ENTRADA_OK) {
        // Validar que sea alfanumérico y longitud correcta
        if (validar_alfanumerico(buffer) && validar_longitud(buffer, 6, 10)) {
            printf("✅ Código válido: %s\n", buffer);
        } else {
            printf("❌ Código no cumple los requisitos\n");
        }
    } else {
        printf("❌ Error: %s\n", entrada_error_string(resultado));
    }
    
    // Mostrar estadísticas
    entrada_imprimir_stats(&stats);
    
    return 0;
}

int demo_stress_test(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                  DEMO: STRESS TEST                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("Este test demuestra la robustez de la entrada segura.\n");
    printf("Puedes intentar:\n");
    printf("• Entrada muy larga (más de 100 caracteres)\n");
    printf("• Caracteres especiales\n");
    printf("• Entrada vacía\n");
    printf("• Ctrl+D (EOF)\n\n");
    
    char buffer[16]; // Buffer pequeño para demostrar protección
    config_entrada_t config;
    stats_entrada_t stats;
    
    entrada_init_config(&config);
    config.max_reintentos = 3;
    strcpy(config.prompt, "Introduce texto (máximo 15 caracteres): ");
    
    resultado_entrada_t resultado = entrada_linea_configurada(buffer, sizeof(buffer), &config, &stats);
    
    printf("\n=== RESULTADOS ===\n");
    if (resultado == ENTRADA_OK) {
        printf("✅ Entrada exitosa: '%s'\n", buffer);
        printf("Longitud: %zu caracteres\n", strlen(buffer));
    } else {
        printf("❌ Entrada falló: %s\n", entrada_error_string(resultado));
    }
    
    entrada_imprimir_stats(&stats);
    
    // Verificar integridad del buffer
    printf("\n=== VERIFICACIÓN DE SEGURIDAD ===\n");
    printf("Buffer size: %zu bytes\n", sizeof(buffer));
    printf("Contenido: '");
    for (size_t i = 0; i < sizeof(buffer); i++) {
        if (buffer[i] == '\0') {
            printf("\\0");
        } else if (isprint(buffer[i])) {
            printf("%c", buffer[i]);
        } else {
            printf("\\x%02x", (unsigned char)buffer[i]);
        }
    }
    printf("'\n");
    printf("✅ Buffer mantuvo su integridad - no hay overflow\n");
    
    return 0;
}

int tutorial_entrada_segura(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              TUTORIAL ENTRADA SEGURA                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    bool continuar = true;
    char opcion;
    
    while (continuar) {
        printf("\n📚 TUTORIAL INTERACTIVO:\n");
        printf("1. Ver vulnerabilidades comunes\n");
        printf("2. Ver buenas prácticas\n");
        printf("3. Comparar funciones seguras vs inseguras\n");
        printf("4. Demo de entrada segura\n");
        printf("5. Demo de validación\n");
        printf("6. Demo de configuración avanzada\n");
        printf("7. Stress test\n");
        printf("0. Salir\n\n");
        
        resultado_entrada_t resultado = entrada_caracter_seguro(&opcion, "01234567", "Elige una opción");
        
        if (resultado != ENTRADA_OK) {
            printf("Error al leer opción\n");
            continue;
        }
        
        printf("\n");
        
        switch (opcion) {
            case '1':
                mostrar_vulnerabilidades_comunes();
                break;
            case '2':
                mostrar_buenas_practicas();
                break;
            case '3':
                comparar_funciones_seguridad();
                break;
            case '4':
                demo_comparacion_seguridad();
                break;
            case '5':
                demo_validacion();
                break;
            case '6':
                demo_configuracion_avanzada();
                break;
            case '7':
                demo_stress_test();
                break;
            case '0':
                continuar = false;
                break;
        }
        
        if (continuar) {
            printf("\nPresiona Enter para continuar...");
            char dummy[8];
            entrada_cadena_segura(dummy, sizeof(dummy), stdin);
        }
    }
    
    printf("\n✅ Tutorial completado. ¡Usa siempre funciones seguras!\n");
    return 0;
}
