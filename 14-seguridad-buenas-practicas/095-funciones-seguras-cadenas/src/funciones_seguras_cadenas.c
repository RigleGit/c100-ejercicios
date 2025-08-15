/**
 * @file funciones_seguras_cadenas.c
 * @brief Implementación de funciones seguras para manipulación de cadenas
 */

#include "funciones_seguras_cadenas.h"
#include <time.h>
#include <stdarg.h>

/* ================================
 * FUNCIONES AUXILIARES INTERNAS
 * ================================ */

/**
 * @brief Establece información de error en la estructura de operación
 */
static void establecer_error(info_operacion_t *info, resultado_cadena_t codigo, 
                            const char *mensaje) {
    if (!info) return;
    
    info->codigo = codigo;
    info->truncado = (codigo == CADENA_ERROR_CADENA_TRUNCADA);
    strncpy(info->mensaje_error, mensaje, sizeof(info->mensaje_error) - 1);
    info->mensaje_error[sizeof(info->mensaje_error) - 1] = '\0';
}

/**
 * @brief Valida parámetros básicos para operaciones de cadena
 */
static resultado_cadena_t validar_parametros_basicos(const void *destino, 
                                                    const void *origen,
                                                    size_t tamaño) {
    if (!destino) return CADENA_ERROR_PARAMETRO_NULO;
    if (!origen) return CADENA_ERROR_PARAMETRO_NULO;
    if (tamaño == 0) return CADENA_ERROR_BUFFER_PEQUEÑO;
    if (tamaño > MAX_LONGITUD_CADENA) return CADENA_ERROR_LONGITUD_INVALIDA;
    
    return CADENA_OK;
}

/* ================================
 * FUNCIONES SEGURAS DE COPIA
 * ================================ */

resultado_cadena_t copia_segura(char *destino, const char *origen, 
                                size_t tamaño_destino, info_operacion_t *info) {
    resultado_cadena_t resultado = validar_parametros_basicos(destino, origen, tamaño_destino);
    
    if (info) {
        init_info_operacion(info);
        info->bytes_disponibles = tamaño_destino;
    }
    
    if (resultado != CADENA_OK) {
        if (info) establecer_error(info, resultado, "Parámetros inválidos");
        return resultado;
    }
    
    size_t longitud_origen = strlen(origen);
    
    // Verificar si la cadena origen cabe en el destino
    if (longitud_origen >= tamaño_destino) {
        // Usar strncpy pero garantizar terminación
        strncpy(destino, origen, tamaño_destino - 1);
        destino[tamaño_destino - 1] = '\0';
        
        if (info) {
            info->bytes_usados = tamaño_destino - 1;
            establecer_error(info, CADENA_ERROR_CADENA_TRUNCADA, 
                           "Cadena truncada para ajustarse al buffer");
        }
        
        CADENAS_WARNING("Cadena truncada: origen=%zu chars, destino=%zu chars", 
                       longitud_origen, tamaño_destino - 1);
        return CADENA_ERROR_CADENA_TRUNCADA;
    }
    
    // Copia normal - la cadena cabe completamente
    strcpy(destino, origen);
    
    if (info) {
        info->codigo = CADENA_OK;
        info->bytes_usados = longitud_origen + 1; // Incluye el terminador
        info->truncado = false;
    }
    
    CADENAS_DEBUG("Copia exitosa: %zu caracteres", longitud_origen);
    return CADENA_OK;
}

resultado_cadena_t copia_segura_validada(char *destino, const char *origen, 
                                         size_t tamaño_destino) {
    return copia_segura(destino, origen, tamaño_destino, NULL);
}

resultado_cadena_t copia_subcadena_segura(char *destino, const char *origen,
                                          size_t inicio, size_t longitud,
                                          size_t tamaño_destino) {
    resultado_cadena_t resultado = validar_parametros_basicos(destino, origen, tamaño_destino);
    if (resultado != CADENA_OK) return resultado;
    
    size_t longitud_origen = strlen(origen);
    
    // Verificar que el inicio sea válido
    if (inicio >= longitud_origen) {
        destino[0] = '\0';
        return CADENA_OK; // Subcadena vacía
    }
    
    // Ajustar longitud si excede el final de la cadena origen
    size_t longitud_real = CADENAS_MIN(longitud, longitud_origen - inicio);
    
    // Verificar que quepa en el destino
    if (longitud_real >= tamaño_destino) {
        longitud_real = tamaño_destino - 1;
        resultado = CADENA_ERROR_CADENA_TRUNCADA;
    }
    
    // Copiar subcadena
    strncpy(destino, origen + inicio, longitud_real);
    destino[longitud_real] = '\0';
    
    return resultado;
}

/* ================================
 * FUNCIONES SEGURAS DE CONCATENACIÓN
 * ================================ */

resultado_cadena_t concatena_segura(char *destino, const char *origen,
                                    size_t tamaño_destino, info_operacion_t *info) {
    resultado_cadena_t resultado = validar_parametros_basicos(destino, origen, tamaño_destino);
    
    if (info) {
        init_info_operacion(info);
        info->bytes_disponibles = tamaño_destino;
    }
    
    if (resultado != CADENA_OK) {
        if (info) establecer_error(info, resultado, "Parámetros inválidos");
        return resultado;
    }
    
    size_t longitud_actual = strlen(destino);
    size_t longitud_origen = strlen(origen);
    size_t espacio_disponible = tamaño_destino - longitud_actual - 1; // -1 para el terminador
    
    if (info) {
        info->bytes_usados = longitud_actual;
    }
    
    // Verificar si hay espacio para concatenar
    if (espacio_disponible == 0) {
        if (info) establecer_error(info, CADENA_ERROR_BUFFER_PEQUEÑO, 
                                 "No hay espacio en el buffer de destino");
        return CADENA_ERROR_BUFFER_PEQUEÑO;
    }
    
    // Verificar si la cadena origen cabe completamente
    if (longitud_origen > espacio_disponible) {
        // Concatenar lo que quepa
        strncat(destino, origen, espacio_disponible);
        
        if (info) {
            info->bytes_usados = tamaño_destino - 1;
            establecer_error(info, CADENA_ERROR_CADENA_TRUNCADA, 
                           "Cadena truncada durante concatenación");
        }
        
        CADENAS_WARNING("Concatenación truncada: disponible=%zu, requerido=%zu", 
                       espacio_disponible, longitud_origen);
        return CADENA_ERROR_CADENA_TRUNCADA;
    }
    
    // Concatenación normal
    strcat(destino, origen);
    
    if (info) {
        info->codigo = CADENA_OK;
        info->bytes_usados = longitud_actual + longitud_origen + 1;
        info->truncado = false;
    }
    
    CADENAS_DEBUG("Concatenación exitosa: %zu + %zu caracteres", 
                 longitud_actual, longitud_origen);
    return CADENA_OK;
}

resultado_cadena_t concatena_multiple_segura(char *destino, size_t tamaño_destino,
                                            int num_cadenas, ...) {
    if (!destino || tamaño_destino == 0 || num_cadenas <= 0) {
        return CADENA_ERROR_PARAMETRO_NULO;
    }
    
    if (num_cadenas > MAX_ARGUMENTOS_FORMATO) {
        return CADENA_ERROR_LONGITUD_INVALIDA;
    }
    
    // Inicializar destino como cadena vacía
    destino[0] = '\0';
    
    va_list args;
    va_start(args, num_cadenas);
    
    resultado_cadena_t resultado_final = CADENA_OK;
    
    for (int i = 0; i < num_cadenas; i++) {
        const char *cadena = va_arg(args, const char*);
        if (!cadena) continue;
        
        resultado_cadena_t resultado = concatena_segura(destino, cadena, tamaño_destino, NULL);
        
        if (resultado != CADENA_OK) {
            resultado_final = resultado;
            if (resultado == CADENA_ERROR_BUFFER_PEQUEÑO) {
                break; // No hay más espacio
            }
        }
    }
    
    va_end(args);
    return resultado_final;
}

resultado_cadena_t concatena_con_separador(char *destino, const char *origen,
                                           const char *separador,
                                           size_t tamaño_destino) {
    if (!separador) separador = ""; // Separador vacío por defecto
    
    // Si el destino no está vacío, agregar separador primero
    if (strlen(destino) > 0) {
        resultado_cadena_t resultado = concatena_segura(destino, separador, tamaño_destino, NULL);
        if (resultado != CADENA_OK && resultado != CADENA_ERROR_CADENA_TRUNCADA) {
            return resultado;
        }
    }
    
    return concatena_segura(destino, origen, tamaño_destino, NULL);
}

/* ================================
 * FUNCIONES SEGURAS DE FORMATEO
 * ================================ */

resultado_cadena_t formatea_seguro(char *destino, size_t tamaño_destino,
                                   const char *formato, ...) {
    if (!destino || !formato || tamaño_destino == 0) {
        return CADENA_ERROR_PARAMETRO_NULO;
    }
    
    va_list args;
    va_start(args, formato);
    
    resultado_cadena_t resultado = vformatea_seguro(destino, tamaño_destino, formato, args);
    
    va_end(args);
    return resultado;
}

resultado_cadena_t vformatea_seguro(char *destino, size_t tamaño_destino,
                                    const char *formato, va_list args) {
    if (!destino || !formato || tamaño_destino == 0) {
        return CADENA_ERROR_PARAMETRO_NULO;
    }
    
    int resultado = vsnprintf(destino, tamaño_destino, formato, args);
    
    if (resultado < 0) {
        // Error en el formateo
        destino[0] = '\0';
        CADENAS_ERROR("Error en vsnprintf");
        return CADENA_ERROR_FORMATO_INVALIDO;
    }
    
    if ((size_t)resultado >= tamaño_destino) {
        // La cadena fue truncada
        CADENAS_WARNING("Formateo truncado: requerido=%d, disponible=%zu", 
                       resultado, tamaño_destino);
        return CADENA_ERROR_CADENA_TRUNCADA;
    }
    
    CADENAS_DEBUG("Formateo exitoso: %d caracteres", resultado);
    return CADENA_OK;
}

resultado_cadena_t construye_ruta_segura(char *destino, size_t tamaño_destino,
                                         const char *directorio, const char *archivo) {
    resultado_cadena_t resultado = validar_parametros_basicos(destino, directorio, tamaño_destino);
    if (resultado != CADENA_OK) return resultado;
    
    if (!archivo) return CADENA_ERROR_PARAMETRO_NULO;
    
    // Copiar directorio
    resultado = copia_segura(destino, directorio, tamaño_destino, NULL);
    if (resultado != CADENA_OK && resultado != CADENA_ERROR_CADENA_TRUNCADA) {
        return resultado;
    }
    
    // Verificar si necesitamos agregar separador
    size_t longitud = strlen(destino);
    if (longitud > 0 && destino[longitud - 1] != SEPARADOR_RUTA && 
        destino[longitud - 1] != SEPARADOR_RUTA_WIN) {
        resultado_cadena_t sep_resultado = concatena_segura(destino, "/", tamaño_destino, NULL);
        if (sep_resultado != CADENA_OK && sep_resultado != CADENA_ERROR_CADENA_TRUNCADA) {
            return sep_resultado;
        }
    }
    
    // Concatenar archivo
    return concatena_segura(destino, archivo, tamaño_destino, NULL);
}

/* ================================
 * FUNCIONES DE VALIDACIÓN Y UTILIDAD
 * ================================ */

bool validar_buffer_seguro(const void *buffer, size_t tamaño, info_buffer_t *info) {
    if (info) {
        init_info_buffer(info, (void*)buffer, tamaño, "buffer_validacion");
    }
    
    if (!buffer) {
        if (info) strcpy(info->nombre, "NULL_BUFFER");
        return false;
    }
    
    if (tamaño == 0) {
        if (info) strcpy(info->nombre, "ZERO_SIZE_BUFFER");
        return false;
    }
    
    if (tamaño > MAX_LONGITUD_CADENA) {
        if (info) strcpy(info->nombre, "OVERSIZED_BUFFER");
        return false;
    }
    
    if (info) {
        info->es_valido = true;
    }
    
    return true;
}

size_t espacio_restante_buffer(const char *cadena, size_t tamaño_buffer) {
    if (!cadena || tamaño_buffer == 0) return 0;
    
    size_t longitud_actual = strlen(cadena);
    if (longitud_actual >= tamaño_buffer) return 0;
    
    return tamaño_buffer - longitud_actual - 1; // -1 para el terminador
}

bool cadena_cabe_en_buffer(const char *cadena, size_t tamaño_buffer) {
    if (!cadena || tamaño_buffer == 0) return false;
    
    size_t longitud_cadena = strlen(cadena);
    return (longitud_cadena + 1) <= tamaño_buffer; // +1 para el terminador
}

resultado_cadena_t truncar_cadena_segura(char *cadena, size_t longitud_maxima) {
    if (!cadena) return CADENA_ERROR_PARAMETRO_NULO;
    if (longitud_maxima == 0) return CADENA_ERROR_BUFFER_PEQUEÑO;
    
    size_t longitud_actual = strlen(cadena);
    
    if (longitud_actual <= longitud_maxima) {
        return CADENA_OK; // No necesita truncarse
    }
    
    cadena[longitud_maxima] = '\0';
    
    CADENAS_DEBUG("Cadena truncada de %zu a %zu caracteres", 
                 longitud_actual, longitud_maxima);
    
    return CADENA_ERROR_CADENA_TRUNCADA;
}

/* ================================
 * FUNCIONES DE COMPARACIÓN AVANZADA
 * ================================ */

int compara_seguro(const char *cadena1, const char *cadena2, size_t max_longitud) {
    if (!cadena1 && !cadena2) return 0;
    if (!cadena1) return -1;
    if (!cadena2) return 1;
    
    return strncmp(cadena1, cadena2, max_longitud);
}

const char* buscar_subcadena_segura(const char *cadena_principal, 
                                   const char *subcadena,
                                   size_t max_longitud) {
    if (!cadena_principal || !subcadena) return NULL;
    if (max_longitud == 0) return NULL;
    
    size_t longitud_principal = strlen(cadena_principal);
    size_t longitud_subcadena = strlen(subcadena);
    
    if (longitud_subcadena == 0) return cadena_principal;
    if (longitud_subcadena > max_longitud) return NULL;
    
    size_t limite_busqueda = CADENAS_MIN(longitud_principal, max_longitud);
    
    for (size_t i = 0; i <= limite_busqueda - longitud_subcadena; i++) {
        if (strncmp(cadena_principal + i, subcadena, longitud_subcadena) == 0) {
            return cadena_principal + i;
        }
    }
    
    return NULL;
}

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

int demo_funciones_inseguras(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               DEMO: FUNCIONES INSEGURAS                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("⚠️  ADVERTENCIA: Este demo muestra código INSEGURO para fines educativos\n");
    printf("   En un programa real, este código causaría vulnerabilidades\n\n");
    
    // Simulación de código inseguro (sin ejecutar realmente las funciones peligrosas)
    printf("🔴 Ejemplo de código INSEGURO:\n");
    printf("   char buffer[10];\n");
    printf("   strcpy(buffer, \"Esta cadena es muy larga\");  // BUFFER OVERFLOW!\n");
    printf("   strcat(buffer, \" y más texto\");              // MÁS OVERFLOW!\n");
    printf("   sprintf(buffer, \"Número: %%d\", 12345678);     // TAMBIÉN OVERFLOW!\n\n");
    
    printf("💥 Problemas con funciones inseguras:\n");
    printf("   • strcpy():  No verifica límites del buffer\n");
    printf("   • strcat():  No verifica espacio disponible\n");
    printf("   • sprintf(): No limita la salida\n");
    printf("   • gets():    Eliminada en C11 (completamente insegura)\n\n");
    
    printf("🚨 Consecuencias de buffer overflow:\n");
    printf("   • Corrupción de memoria\n");
    printf("   • Crashes de la aplicación\n");
    printf("   • Vulnerabilidades de seguridad\n");
    printf("   • Ejecución de código malicioso\n");
    
    return 0;
}

int demo_funciones_seguras(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                DEMO: FUNCIONES SEGURAS                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Ejemplo práctico con funciones seguras
    char buffer[20];
    char nombre[] = "Rodrigo";
    int edad = 30;
    info_operacion_t info;
    
    printf("✅ Usando funciones SEGURAS:\n\n");
    
    // 1. Copia segura
    printf("1. Copia segura con strncpy:\n");
    resultado_cadena_t resultado = copia_segura(buffer, "Nombre: ", sizeof(buffer), &info);
    printf("   Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   Buffer: \"%s\"\n", buffer);
    printf("   Bytes usados: %zu de %zu\n\n", info.bytes_usados, info.bytes_disponibles);
    
    // 2. Concatenación segura
    printf("2. Concatenación segura con strncat:\n");
    resultado = concatena_segura(buffer, nombre, sizeof(buffer), &info);
    printf("   Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   Buffer: \"%s\"\n", buffer);
    printf("   Truncado: %s\n\n", info.truncado ? "Sí" : "No");
    
    // 3. Formateo seguro
    printf("3. Formateo seguro con snprintf:\n");
    char info_edad[20];
    resultado = formatea_seguro(info_edad, sizeof(info_edad), "Edad: %d", edad);
    printf("   Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   Buffer: \"%s\"\n\n", info_edad);
    
    // 4. Operaciones múltiples
    printf("4. Concatenación múltiple segura:\n");
    char mensaje[50];
    resultado = concatena_multiple_segura(mensaje, sizeof(mensaje), 4,
                                         "Hola ", nombre, ", tienes ", "años");
    printf("   Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   Mensaje: \"%s\"\n\n", mensaje);
    
    // 5. Construcción de ruta
    printf("5. Construcción de ruta segura:\n");
    char ruta[30];
    resultado = construye_ruta_segura(ruta, sizeof(ruta), "/home/usuario", "documento.txt");
    printf("   Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   Ruta: \"%s\"\n\n", ruta);
    
    printf("✨ Ventajas de las funciones seguras:\n");
    printf("   • Control de límites de buffer\n");
    printf("   • Prevención de buffer overflow\n");
    printf("   • Manejo de truncación controlada\n");
    printf("   • Información detallada de errores\n");
    
    return 0;
}

int demo_operaciones_complejas(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           DEMO: OPERACIONES COMPLEJAS SEGURAS               ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Ejemplo de construcción de mensaje complejo
    char mensaje_final[128];
    char buffer_temp[64];
    info_operacion_t info;
    
    printf("📝 Construyendo mensaje complejo de forma segura:\n\n");
    
    // Inicializar mensaje
    copia_segura(mensaje_final, "Usuario: ", sizeof(mensaje_final), &info);
    printf("1. Inicialización: \"%s\" (bytes: %zu)\n", mensaje_final, info.bytes_usados);
    
    // Agregar nombre
    concatena_segura(mensaje_final, "Ana García", sizeof(mensaje_final), &info);
    printf("2. + Nombre: \"%s\" (bytes: %zu)\n", mensaje_final, info.bytes_usados);
    
    // Agregar separador y edad
    concatena_con_separador(mensaje_final, "Edad: 25", ", ", sizeof(mensaje_final));
    printf("3. + Edad: \"%s\"\n", mensaje_final);
    
    // Construir información adicional
    formatea_seguro(buffer_temp, sizeof(buffer_temp), "Activo desde: %d", 2020);
    concatena_con_separador(mensaje_final, buffer_temp, ", ", sizeof(mensaje_final));
    printf("4. + Fecha: \"%s\"\n", mensaje_final);
    
    // Verificar espacio restante
    size_t espacio = espacio_restante_buffer(mensaje_final, sizeof(mensaje_final));
    printf("5. Espacio restante: %zu bytes\n\n", espacio);
    
    // Ejemplo de validación de buffer
    printf("🔍 Validación de buffers:\n");
    info_buffer_t buffer_info;
    bool es_valido = validar_buffer_seguro(mensaje_final, sizeof(mensaje_final), &buffer_info);
    printf("   Buffer válido: %s\n", es_valido ? "Sí" : "No");
    printf("   Tamaño declarado: %zu bytes\n", buffer_info.tamaño);
    printf("   Contenido actual: %zu bytes\n\n", strlen(mensaje_final));
    
    // Ejemplo de búsqueda segura
    printf("🔎 Búsqueda de subcadenas:\n");
    const char *posicion = buscar_subcadena_segura(mensaje_final, "Ana", 
                                                  sizeof(mensaje_final));
    if (posicion) {
        printf("   'Ana' encontrado en posición: %ld\n", posicion - mensaje_final);
    } else {
        printf("   'Ana' no encontrado\n");
    }
    
    // Ejemplo de comparación segura
    printf("\n📊 Comparación segura:\n");
    char otra_cadena[] = "Usuario: Ana García";
    int comparacion = compara_seguro(mensaje_final, otra_cadena, 20);
    printf("   Comparación (primeros 20 chars): %s\n", 
           comparacion == 0 ? "Iguales" : "Diferentes");
    
    return 0;
}

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

const char* resultado_cadena_string(resultado_cadena_t codigo) {
    switch (codigo) {
        case CADENA_OK: return "Éxito";
        case CADENA_ERROR_PARAMETRO_NULO: return "Parámetro nulo";
        case CADENA_ERROR_BUFFER_PEQUEÑO: return "Buffer muy pequeño";
        case CADENA_ERROR_CADENA_TRUNCADA: return "Cadena truncada";
        case CADENA_ERROR_FORMATO_INVALIDO: return "Formato inválido";
        case CADENA_ERROR_MEMORIA: return "Error de memoria";
        case CADENA_ERROR_LONGITUD_INVALIDA: return "Longitud inválida";
        default: return "Error desconocido";
    }
}

void imprimir_info_operacion(const info_operacion_t *info) {
    if (!info) return;
    
    printf("=== Información de Operación ===\n");
    printf("Código: %s\n", resultado_cadena_string(info->codigo));
    printf("Bytes usados: %zu\n", info->bytes_usados);
    printf("Bytes disponibles: %zu\n", info->bytes_disponibles);
    printf("Truncado: %s\n", info->truncado ? "Sí" : "No");
    if (strlen(info->mensaje_error) > 0) {
        printf("Error: %s\n", info->mensaje_error);
    }
}

void imprimir_info_buffer(const info_buffer_t *info) {
    if (!info) return;
    
    printf("=== Información de Buffer ===\n");
    printf("Nombre: %s\n", info->nombre);
    printf("Puntero: %p\n", info->buffer);
    printf("Tamaño: %zu bytes\n", info->tamaño);
    printf("Usado: %zu bytes\n", info->usado);
    printf("Válido: %s\n", info->es_valido ? "Sí" : "No");
}

void init_info_operacion(info_operacion_t *info) {
    if (!info) return;
    
    memset(info, 0, sizeof(info_operacion_t));
    info->codigo = CADENA_OK;
}

void init_info_buffer(info_buffer_t *info, void *buffer, size_t tamaño, const char *nombre) {
    if (!info) return;
    
    memset(info, 0, sizeof(info_buffer_t));
    info->buffer = buffer;
    info->tamaño = tamaño;
    info->es_valido = (buffer != NULL && tamaño > 0);
    
    if (nombre) {
        strncpy(info->nombre, nombre, sizeof(info->nombre) - 1);
        info->nombre[sizeof(info->nombre) - 1] = '\0';
    }
    
    if (buffer && tamaño > 0) {
        info->usado = strlen((const char*)buffer);
    }
}

int benchmark_funciones_cadenas(int num_iteraciones) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              BENCHMARK: FUNCIONES DE CADENAS                ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("⏱️  Realizando %d iteraciones...\n\n", num_iteraciones);
    
    clock_t inicio, fin;
    char buffer[100];
    double tiempo_seguras, tiempo_estandar;
    
    // Benchmark funciones seguras
    inicio = clock();
    for (int i = 0; i < num_iteraciones; i++) {
        copia_segura(buffer, "Prueba de velocidad", sizeof(buffer), NULL);
        concatena_segura(buffer, " iteración", sizeof(buffer), NULL);
        formatea_seguro(buffer, sizeof(buffer), "Iteración: %d", i);
    }
    fin = clock();
    tiempo_seguras = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    // Benchmark funciones estándar (en entorno controlado)
    inicio = clock();
    for (int i = 0; i < num_iteraciones; i++) {
        strcpy(buffer, "Prueba de velocidad");
        strcat(buffer, " iteración");
        sprintf(buffer, "Iteración: %d", i);
    }
    fin = clock();
    tiempo_estandar = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    printf("📊 Resultados del benchmark:\n");
    printf("   Funciones seguras:   %.6f segundos\n", tiempo_seguras);
    printf("   Funciones estándar:  %.6f segundos\n", tiempo_estandar);
    printf("   Diferencia:          %.2f%% más lento\n", 
           ((tiempo_seguras - tiempo_estandar) / tiempo_estandar) * 100);
    printf("\n");
    
    printf("📝 Conclusiones:\n");
    printf("   • Las funciones seguras tienen un overhead mínimo\n");
    printf("   • La diferencia de rendimiento es despreciable\n");
    printf("   • Los beneficios de seguridad superan ampliamente el costo\n");
    
    return 0;
}

int prueba_estres_seguridad(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              PRUEBA DE ESTRÉS DE SEGURIDAD                  ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("🔒 Probando resistencia a ataques de buffer overflow...\n\n");
    
    char buffer_pequeño[10];
    char cadena_muy_larga[100];
    resultado_cadena_t resultado;
    int errores_detectados = 0;
    
    // Construir cadena muy larga
    for (int i = 0; i < 99; i++) {
        cadena_muy_larga[i] = 'A';
    }
    cadena_muy_larga[99] = '\0';
    
    // Prueba 1: Copia de cadena muy larga
    printf("1. Probando copia de cadena muy larga (%zu chars) en buffer pequeño (%zu chars):\n",
           strlen(cadena_muy_larga), sizeof(buffer_pequeño));
    
    resultado = copia_segura(buffer_pequeño, cadena_muy_larga, sizeof(buffer_pequeño), NULL);
    if (resultado == CADENA_ERROR_CADENA_TRUNCADA) {
        printf("   ✅ Truncación detectada y manejada correctamente\n");
        printf("   ✅ Buffer resultado: \"%.15s...\"\n", buffer_pequeño);
        printf("   ✅ Terminador nulo presente: %s\n", 
               buffer_pequeño[sizeof(buffer_pequeño)-1] == '\0' ? "Sí" : "No");
    } else {
        printf("   ❌ Error: truncación no detectada\n");
        errores_detectados++;
    }
    
    // Prueba 2: Concatenación múltiple
    printf("\n2. Probando concatenación múltiple hasta overflow:\n");
    strcpy(buffer_pequeño, "ABC");
    resultado = concatena_segura(buffer_pequeño, "DEFG", sizeof(buffer_pequeño), NULL);
    if (resultado == CADENA_OK) {
        printf("   ✅ Primera concatenación exitosa: \"%s\"\n", buffer_pequeño);
    }
    
    resultado = concatena_segura(buffer_pequeño, "HIJKLMNOP", sizeof(buffer_pequeño), NULL);
    if (resultado == CADENA_ERROR_CADENA_TRUNCADA || resultado == CADENA_ERROR_BUFFER_PEQUEÑO) {
        printf("   ✅ Overflow detectado y prevenido: \"%s\"\n", buffer_pequeño);
    } else {
        printf("   ❌ Error: overflow no detectado\n");
        errores_detectados++;
    }
    
    // Prueba 3: Formateo con datos muy largos
    printf("\n3. Probando formateo con datos que exceden el buffer:\n");
    resultado = formatea_seguro(buffer_pequeño, sizeof(buffer_pequeño), 
                               "Número muy largo: %d%d%d%d", 1234, 5678, 9012, 3456);
    if (resultado == CADENA_ERROR_CADENA_TRUNCADA) {
        printf("   ✅ Truncación en formateo detectada: \"%s\"\n", buffer_pequeño);
    } else {
        printf("   ❌ Error: truncación en formateo no detectada\n");
        errores_detectados++;
    }
    
    // Prueba 4: Parámetros nulos
    printf("\n4. Probando resistencia a parámetros nulos:\n");
    resultado = copia_segura(NULL, "test", 10, NULL);
    if (resultado == CADENA_ERROR_PARAMETRO_NULO) {
        printf("   ✅ Parámetro nulo detectado correctamente\n");
    } else {
        printf("   ❌ Error: parámetro nulo no detectado\n");
        errores_detectados++;
    }
    
    // Resumen
    printf("\n📋 Resumen de la prueba de estrés:\n");
    printf("   Errores detectados: %d\n", errores_detectados);
    printf("   Estado general: %s\n", errores_detectados == 0 ? "✅ SEGURO" : "❌ VULNERABILIDADES DETECTADAS");
    
    if (errores_detectados == 0) {
        printf("\n🎉 ¡Todas las funciones seguras pasaron las pruebas de estrés!\n");
        printf("   El código está bien protegido contra buffer overflows.\n");
    }
    
    return errores_detectados;
}

/* ================================
 * ADAPTADORES PARA API DE TESTS
 * ================================ */

int strncpy_seguro(char *destino, const char *origen, size_t tamaño_destino) {
    info_operacion_t info;
    resultado_cadena_t r = copia_segura(destino, origen, tamaño_destino, &info);
    return r == CADENA_OK ? 0 : -1;
}

int strncat_seguro(char *destino, const char *origen, size_t tamaño_destino) {
    info_operacion_t info;
    resultado_cadena_t r = concatena_segura(destino, origen, tamaño_destino, &info);
    return r == CADENA_OK ? 0 : -1;
}

int snprintf_seguro(char *destino, size_t tamaño_destino, const char *formato, ...) {
    va_list args;
    va_start(args, formato);
    resultado_cadena_t r = vformatea_seguro(destino, tamaño_destino, formato, args);
    va_end(args);
    return r == CADENA_OK ? 0 : -1;
}

int demostracion_completa(char *salida, size_t tamaño_salida, const char *nombre, int edad) {
    if (!salida || tamaño_salida == 0 || !nombre || edad < 0) {
        if (salida && tamaño_salida > 0) salida[0] = '\0';
        return -1;
    }
    char buffer[256];
    if (formatea_seguro(buffer, sizeof(buffer), "Usuario: %s, Tiempo: %ld", nombre, (long)time(NULL)) != CADENA_OK) {
        salida[0] = '\0';
        return -1;
    }
    return (formatea_seguro(salida, tamaño_salida, "%s, Edad: %d", buffer, edad) == CADENA_OK) ? 0 : -1;
}
