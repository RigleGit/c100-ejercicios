/**
 * @file leer_archivo.c
 * @brief Implementación de funciones para lectura de archivos de texto
 * @author Sistema de Ejercicios C
 * @date 2024
 * 
 * Implementación completa de funciones para leer archivos de texto,
 * incluyendo manejo de errores, análisis de contenido, búsqueda de patrones
 * y procesamiento avanzado de texto.
 */

#include "../include/leer_archivo.h"
#include <sys/stat.h>

/* ================================
 * FUNCIONES BÁSICAS DE LECTURA
 * ================================ */

ResultadoLectura leer_archivo_basico(const char* nombre_archivo) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombre de archivo inválido");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    printf("Contenido del archivo '%s':\n", nombre_archivo);
    printf("=====================================\n");
    
    char linea[MAX_LINEA];
    int lineas_leidas = 0;
    int caracteres_leidos = 0;
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
        lineas_leidas++;
        caracteres_leidos += strlen(linea);
    }
    
    fclose(archivo);
    
    printf("\n=====================================\n");
    printf("Archivo leído exitosamente: %d líneas, %d caracteres\n", 
           lineas_leidas, caracteres_leidos);
    
    resultado.exito = true;
    resultado.lineas_leidas = lineas_leidas;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Archivo leído exitosamente");
    
    return resultado;
}

ResultadoLectura leer_archivo_numerado(const char* nombre_archivo) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombre de archivo inválido");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    printf("Contenido del archivo '%s' (con números de línea):\n", nombre_archivo);
    printf("================================================\n");
    
    char linea[MAX_LINEA];
    int numero_linea = 1;
    int caracteres_leidos = 0;
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%4d: %s", numero_linea, linea);
        numero_linea++;
        caracteres_leidos += strlen(linea);
    }
    
    fclose(archivo);
    
    printf("================================================\n");
    printf("Total: %d líneas, %d caracteres\n", 
           numero_linea - 1, caracteres_leidos);
    
    resultado.exito = true;
    resultado.lineas_leidas = numero_linea - 1;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Archivo leído con numeración exitosamente");
    
    return resultado;
}

ResultadoLectura leer_archivo_configurado(const char* nombre_archivo, 
                                         const ConfiguracionLectura* config) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo || !config) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    printf("Contenido del archivo '%s':\n", nombre_archivo);
    printf("=====================================\n");
    
    char linea[MAX_LINEA];
    int numero_linea = 1;
    int lineas_mostradas = 0;
    int caracteres_leidos = 0;
    int lineas_vacias = 0;
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Verificar límite de líneas
        if (config->max_lineas_mostrar > 0 && 
            lineas_mostradas >= config->max_lineas_mostrar) {
            break;
        }
        
        // Verificar si es línea vacía
        bool es_vacia = (strlen(linea) <= 1 || 
                        (strlen(linea) == 2 && linea[0] == '\r' && linea[1] == '\n'));
        
        if (config->ignorar_lineas_vacias && es_vacia) {
            lineas_vacias++;
            numero_linea++;
            continue;
        }
        
        // Mostrar número de línea si está configurado
        if (config->mostrar_numeros_linea) {
            printf("%4d: ", numero_linea);
        }
        
        // Convertir a mayúsculas si está configurado
        if (config->convertir_a_mayusculas) {
            char linea_mayusculas[MAX_LINEA];
            strcpy(linea_mayusculas, linea);
            for (int i = 0; linea_mayusculas[i]; i++) {
                linea_mayusculas[i] = toupper(linea_mayusculas[i]);
            }
            printf("%s", linea_mayusculas);
        } else {
            printf("%s", linea);
        }
        
        lineas_mostradas++;
        numero_linea++;
        caracteres_leidos += strlen(linea);
    }
    
    fclose(archivo);
    
    if (config->mostrar_estadisticas) {
        printf("\n=====================================\n");
        printf("Estadísticas de lectura:\n");
        printf("  Líneas leídas: %d\n", numero_linea - 1);
        printf("  Líneas mostradas: %d\n", lineas_mostradas);
        printf("  Líneas vacías omitidas: %d\n", lineas_vacias);
        printf("  Caracteres procesados: %d\n", caracteres_leidos);
    }
    
    resultado.exito = true;
    resultado.lineas_leidas = numero_linea - 1;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Archivo leído con configuración personalizada");
    
    return resultado;
}

/* ================================
 * FUNCIONES DE ALMACENAMIENTO
 * ================================ */

ResultadoLectura cargar_archivo_en_memoria(const char* nombre_archivo, 
                                          BufferLineas* buffer) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo || !buffer) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    // Inicializar buffer
    buffer->capacidad = MAX_LINEAS_MEMORIA;
    buffer->lineas = malloc(buffer->capacidad * sizeof(char*));
    buffer->num_lineas = 0;
    
    if (!buffer->lineas) {
        fclose(archivo);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error de memoria al crear buffer");
        return resultado;
    }
    
    char linea_temp[MAX_LINEA];
    int caracteres_leidos = 0;
    
    while (fgets(linea_temp, sizeof(linea_temp), archivo) != NULL && 
           buffer->num_lineas < buffer->capacidad) {
        
        // Allocar memoria para la línea
        buffer->lineas[buffer->num_lineas] = malloc(strlen(linea_temp) + 1);
        if (!buffer->lineas[buffer->num_lineas]) {
            // Error de memoria, limpiar lo ya asignado
            liberar_buffer_lineas(buffer);
            fclose(archivo);
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error de memoria al almacenar línea %d", buffer->num_lineas + 1);
            return resultado;
        }
        
        strcpy(buffer->lineas[buffer->num_lineas], linea_temp);
        buffer->num_lineas++;
        caracteres_leidos += strlen(linea_temp);
    }
    
    fclose(archivo);
    
    resultado.exito = true;
    resultado.lineas_leidas = buffer->num_lineas;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Archivo cargado en memoria: %d líneas", buffer->num_lineas);
    
    return resultado;
}

void liberar_buffer_lineas(BufferLineas* buffer) {
    if (!buffer) return;
    
    for (int i = 0; i < buffer->num_lineas; i++) {
        free(buffer->lineas[i]);
    }
    free(buffer->lineas);
    buffer->lineas = NULL;
    buffer->num_lineas = 0;
    buffer->capacidad = 0;
}

/* ================================
 * FUNCIONES DE ANÁLISIS
 * ================================ */

EstadisticasArchivo obtener_estadisticas_archivo(const char* nombre_archivo) {
    EstadisticasArchivo stats = {false, 0, 0, 0, 0, 0};
    
    if (!nombre_archivo) return stats;
    
    // Verificar si el archivo existe usando stat
    struct stat info_archivo;
    if (stat(nombre_archivo, &info_archivo) != 0) {
        return stats;
    }
    
    stats.existe = true;
    stats.tamaño = info_archivo.st_size;
    stats.fecha_modificacion = info_archivo.st_mtime;
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) return stats;
    
    char linea[MAX_LINEA];
    int en_palabra = 0;
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        stats.numero_lineas++;
        
        for (int i = 0; linea[i]; i++) {
            if (!isspace(linea[i])) {
                stats.numero_caracteres++;
                if (!en_palabra) {
                    stats.numero_palabras++;
                    en_palabra = 1;
                }
            } else {
                en_palabra = 0;
            }
        }
    }
    
    fclose(archivo);
    return stats;
}

ResultadoLectura buscar_palabra_en_archivo(const char* nombre_archivo, 
                                          const char* palabra,
                                          bool sensible_mayusculas) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo || !palabra) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    char linea[MAX_LINEA];
    char palabra_buscar[256];
    int numero_linea = 1;
    int coincidencias = 0;
    int caracteres_leidos = 0;
    
    // Preparar palabra de búsqueda
    strcpy(palabra_buscar, palabra);
    if (!sensible_mayusculas) {
        for (int i = 0; palabra_buscar[i]; i++) {
            palabra_buscar[i] = tolower(palabra_buscar[i]);
        }
    }
    
    printf("Buscando '%s' en '%s':\n", palabra, nombre_archivo);
    printf("=====================================\n");
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        char linea_busqueda[MAX_LINEA];
        strcpy(linea_busqueda, linea);
        
        if (!sensible_mayusculas) {
            for (int i = 0; linea_busqueda[i]; i++) {
                linea_busqueda[i] = tolower(linea_busqueda[i]);
            }
        }
        
        if (strstr(linea_busqueda, palabra_buscar) != NULL) {
            printf("Línea %4d: %s", numero_linea, linea);
            coincidencias++;
        }
        
        numero_linea++;
        caracteres_leidos += strlen(linea);
    }
    
    fclose(archivo);
    
    printf("=====================================\n");
    printf("Búsqueda completada: %d coincidencias encontradas\n", coincidencias);
    
    resultado.exito = true;
    resultado.lineas_leidas = numero_linea - 1;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Búsqueda completada: %d coincidencias", coincidencias);
    
    return resultado;
}

int contar_frecuencia_palabras(const char* nombre_archivo, 
                              InfoPalabra* palabras, 
                              int max_palabras) {
    if (!nombre_archivo || !palabras || max_palabras <= 0) {
        return 0;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) return 0;
    
    char linea[MAX_LINEA];
    int numero_linea = 1;
    int num_palabras_unicas = 0;
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        char* palabra = strtok(linea, " \t\n\r\f\v.,;:!?()[]{}\"'");
        
        while (palabra != NULL && num_palabras_unicas < max_palabras) {
            // Convertir a minúsculas para comparación
            char palabra_minuscula[64];
            strncpy(palabra_minuscula, palabra, sizeof(palabra_minuscula) - 1);
            palabra_minuscula[sizeof(palabra_minuscula) - 1] = '\0';
            
            for (int i = 0; palabra_minuscula[i]; i++) {
                palabra_minuscula[i] = tolower(palabra_minuscula[i]);
            }
            
            // Buscar si ya existe
            int indice_existente = -1;
            for (int i = 0; i < num_palabras_unicas; i++) {
                if (strcmp(palabras[i].palabra, palabra_minuscula) == 0) {
                    indice_existente = i;
                    break;
                }
            }
            
            if (indice_existente >= 0) {
                // Palabra ya existe, incrementar frecuencia
                palabras[indice_existente].frecuencia++;
            } else {
                // Nueva palabra
                strncpy(palabras[num_palabras_unicas].palabra, palabra_minuscula, 
                       sizeof(palabras[num_palabras_unicas].palabra) - 1);
                palabras[num_palabras_unicas].palabra[sizeof(palabras[num_palabras_unicas].palabra) - 1] = '\0';
                palabras[num_palabras_unicas].frecuencia = 1;
                palabras[num_palabras_unicas].primera_linea = numero_linea;
                num_palabras_unicas++;
            }
            
            palabra = strtok(NULL, " \t\n\r\f\v.,;:!?()[]{}\"'");
        }
        
        numero_linea++;
    }
    
    fclose(archivo);
    return num_palabras_unicas;
}

/* ================================
 * FUNCIONES DE PROCESAMIENTO
 * ================================ */

ResultadoLectura filtrar_lineas_por_patron(const char* nombre_archivo,
                                           const char* patron,
                                           bool sensible_mayusculas) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo || !patron) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    char linea[MAX_LINEA];
    char patron_busqueda[256];
    int numero_linea = 1;
    int lineas_coincidentes = 0;
    int caracteres_leidos = 0;
    
    // Preparar patrón de búsqueda
    strcpy(patron_busqueda, patron);
    if (!sensible_mayusculas) {
        for (int i = 0; patron_busqueda[i]; i++) {
            patron_busqueda[i] = tolower(patron_busqueda[i]);
        }
    }
    
    printf("Líneas que contienen '%s' en '%s':\n", patron, nombre_archivo);
    printf("=====================================\n");
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        char linea_busqueda[MAX_LINEA];
        strcpy(linea_busqueda, linea);
        
        if (!sensible_mayusculas) {
            for (int i = 0; linea_busqueda[i]; i++) {
                linea_busqueda[i] = tolower(linea_busqueda[i]);
            }
        }
        
        if (strstr(linea_busqueda, patron_busqueda) != NULL) {
            printf("%4d: %s", numero_linea, linea);
            lineas_coincidentes++;
        }
        
        numero_linea++;
        caracteres_leidos += strlen(linea);
    }
    
    fclose(archivo);
    
    printf("=====================================\n");
    printf("Filtrado completado: %d líneas coincidentes\n", lineas_coincidentes);
    
    resultado.exito = true;
    resultado.lineas_leidas = numero_linea - 1;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Filtrado completado: %d líneas coincidentes", lineas_coincidentes);
    
    return resultado;
}

ResultadoLectura leer_rango_lineas(const char* nombre_archivo,
                                  int linea_inicio,
                                  int linea_fin) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!nombre_archivo || linea_inicio < 1) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir el archivo: %s", strerror(errno));
        return resultado;
    }
    
    char linea[MAX_LINEA];
    int numero_linea = 1;
    int lineas_mostradas = 0;
    int caracteres_leidos = 0;
    
    printf("Líneas %d", linea_inicio);
    if (linea_fin > 0) {
        printf("-%d", linea_fin);
    } else {
        printf("-final");
    }
    printf(" del archivo '%s':\n", nombre_archivo);
    printf("=====================================\n");
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (numero_linea >= linea_inicio) {
            if (linea_fin > 0 && numero_linea > linea_fin) {
                break;
            }
            
            printf("%4d: %s", numero_linea, linea);
            lineas_mostradas++;
        }
        
        numero_linea++;
        caracteres_leidos += strlen(linea);
    }
    
    fclose(archivo);
    
    printf("=====================================\n");
    printf("Rango leído: %d líneas mostradas\n", lineas_mostradas);
    
    resultado.exito = true;
    resultado.lineas_leidas = lineas_mostradas;
    resultado.caracteres_leidos = caracteres_leidos;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Rango leído exitosamente: %d líneas", lineas_mostradas);
    
    return resultado;
}

/* ================================
 * FUNCIONES DE COMPARACIÓN
 * ================================ */

ResultadoLectura comparar_archivos(const char* archivo1,
                                  const char* archivo2,
                                  bool mostrar_diferencias) {
    ResultadoLectura resultado = {false, "", 0, 0};
    
    if (!archivo1 || !archivo2) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    FILE* f1 = fopen(archivo1, "r");
    FILE* f2 = fopen(archivo2, "r");
    
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir uno o ambos archivos");
        return resultado;
    }
    
    char linea1[MAX_LINEA], linea2[MAX_LINEA];
    int numero_linea = 1;
    int diferencias = 0;
    bool archivo1_termino = false, archivo2_termino = false;
    
    printf("Comparando '%s' con '%s':\n", archivo1, archivo2);
    printf("=====================================\n");
    
    while (!archivo1_termino || !archivo2_termino) {
        char* result1 = fgets(linea1, sizeof(linea1), f1);
        char* result2 = fgets(linea2, sizeof(linea2), f2);
        
        archivo1_termino = (result1 == NULL);
        archivo2_termino = (result2 == NULL);
        
        if (archivo1_termino && archivo2_termino) {
            break; // Ambos archivos terminaron
        }
        
        if (archivo1_termino || archivo2_termino || strcmp(linea1, linea2) != 0) {
            diferencias++;
            if (mostrar_diferencias) {
                printf("Diferencia en línea %d:\n", numero_linea);
                if (!archivo1_termino) {
                    printf("  %s: %s", archivo1, linea1);
                } else {
                    printf("  %s: [EOF]\n", archivo1);
                }
                if (!archivo2_termino) {
                    printf("  %s: %s", archivo2, linea2);
                } else {
                    printf("  %s: [EOF]\n", archivo2);
                }
                printf("\n");
            }
        }
        
        numero_linea++;
    }
    
    fclose(f1);
    fclose(f2);
    
    if (diferencias == 0) {
        printf("Los archivos son idénticos.\n");
    } else {
        printf("Se encontraron %d diferencias.\n", diferencias);
    }
    
    resultado.exito = true;
    resultado.lineas_leidas = numero_linea - 1;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Comparación completada: %d diferencias", diferencias);
    
    return resultado;
}

/* ================================
 * FUNCIONES INTERACTIVAS
 * ================================ */

ResultadoLectura leer_archivo_interactivo(void) {
    char nombre_archivo[MAX_NOMBRE_ARCHIVO];
    
    printf("=== Lector de Archivos Interactivo ===\n");
    printf("Ingrese el nombre del archivo a leer: ");
    
    if (fgets(nombre_archivo, sizeof(nombre_archivo), stdin) == NULL) {
        ResultadoLectura resultado = {false, "Error al leer entrada del usuario", 0, 0};
        return resultado;
    }
    
    // Remover salto de línea
    nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0';
    
    if (strlen(nombre_archivo) == 0) {
        ResultadoLectura resultado = {false, "Nombre de archivo vacío", 0, 0};
        return resultado;
    }
    
    return leer_archivo_basico(nombre_archivo);
}

ResultadoLectura menu_lectura_archivo(const char* nombre_archivo) {
    char archivo_trabajo[MAX_NOMBRE_ARCHIVO];
    
    // Si no se proporciona archivo, solicitarlo
    if (!nombre_archivo) {
        printf("Ingrese el nombre del archivo: ");
        if (fgets(archivo_trabajo, sizeof(archivo_trabajo), stdin) == NULL) {
            ResultadoLectura resultado = {false, "Error al leer entrada", 0, 0};
            return resultado;
        }
        archivo_trabajo[strcspn(archivo_trabajo, "\n")] = '\0';
    } else {
        strcpy(archivo_trabajo, nombre_archivo);
    }
    
    int opcion;
    ConfiguracionLectura config;
    ResultadoLectura resultado = {false, "", 0, 0};
    
    do {
        printf("\n=== Menú de Lectura de Archivos ===\n");
        printf("Archivo: %s\n", archivo_trabajo);
        printf("1. Lectura básica\n");
        printf("2. Lectura con números de línea\n");
        printf("3. Lectura configurada\n");
        printf("4. Obtener estadísticas\n");
        printf("5. Buscar palabra\n");
        printf("6. Filtrar líneas\n");
        printf("7. Leer rango de líneas\n");
        printf("8. Cargar en memoria\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("Opción inválida.\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        while (getchar() != '\n'); // Limpiar buffer
        
        switch (opcion) {
            case 1:
                resultado = leer_archivo_basico(archivo_trabajo);
                break;
            case 2:
                resultado = leer_archivo_numerado(archivo_trabajo);
                break;
            case 3:
                config = crear_configuracion_defecto();
                resultado = leer_archivo_configurado(archivo_trabajo, &config);
                break;
            case 4: {
                EstadisticasArchivo stats = obtener_estadisticas_archivo(archivo_trabajo);
                if (stats.existe) {
                    printf("\n=== Estadísticas del archivo ===\n");
                    printf("Tamaño: %ld bytes\n", stats.tamaño);
                    printf("Líneas: %d\n", stats.numero_lineas);
                    printf("Palabras: %d\n", stats.numero_palabras);
                    printf("Caracteres: %d\n", stats.numero_caracteres);
                    printf("Modificado: %s", ctime(&stats.fecha_modificacion));
                    resultado.exito = true;
                    strcpy(resultado.mensaje, "Estadísticas obtenidas");
                } else {
                    printf("No se pudo obtener información del archivo.\n");
                }
                break;
            }
            case 5: {
                char palabra[100];
                printf("Ingrese la palabra a buscar: ");
                if (fgets(palabra, sizeof(palabra), stdin)) {
                    palabra[strcspn(palabra, "\n")] = '\0';
                    resultado = buscar_palabra_en_archivo(archivo_trabajo, palabra, false);
                }
                break;
            }
            case 6: {
                char patron[100];
                printf("Ingrese el patrón a filtrar: ");
                if (fgets(patron, sizeof(patron), stdin)) {
                    patron[strcspn(patron, "\n")] = '\0';
                    resultado = filtrar_lineas_por_patron(archivo_trabajo, patron, false);
                }
                break;
            }
            case 7: {
                int inicio, fin;
                printf("Ingrese línea de inicio: ");
                scanf("%d", &inicio);
                printf("Ingrese línea final (-1 para hasta el final): ");
                scanf("%d", &fin);
                while (getchar() != '\n'); // Limpiar buffer
                resultado = leer_rango_lineas(archivo_trabajo, inicio, fin);
                break;
            }
            case 8: {
                BufferLineas buffer;
                resultado = cargar_archivo_en_memoria(archivo_trabajo, &buffer);
                if (resultado.exito) {
                    printf("Archivo cargado en memoria exitosamente.\n");
                    printf("Primeras 5 líneas:\n");
                    for (int i = 0; i < buffer.num_lineas && i < 5; i++) {
                        printf("%d: %s", i + 1, buffer.lineas[i]);
                    }
                    liberar_buffer_lineas(&buffer);
                }
                break;
            }
            case 0:
                printf("Saliendo del menú.\n");
                resultado.exito = true;
                strcpy(resultado.mensaje, "Menú finalizado");
                break;
            default:
                printf("Opción inválida.\n");
                break;
        }
        
        if (opcion != 0 && resultado.exito) {
            printf("\n%s\n", resultado.mensaje);
        }
        
    } while (opcion != 0);
    
    return resultado;
}

/* ================================
 * FUNCIONES AUXILIARES
 * ================================ */

bool archivo_existe_y_legible(const char* nombre_archivo) {
    if (!nombre_archivo) return false;
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo) {
        fclose(archivo);
        return true;
    }
    return false;
}

void imprimir_error_archivo(const char* operacion, const char* nombre_archivo) {
    if (operacion && nombre_archivo) {
        fprintf(stderr, "Error al %s '%s': %s\n", 
               operacion, nombre_archivo, strerror(errno));
    }
}

ConfiguracionLectura crear_configuracion_defecto(void) {
    ConfiguracionLectura config = {
        .mostrar_numeros_linea = true,
        .ignorar_lineas_vacias = false,
        .convertir_a_mayusculas = false,
        .mostrar_estadisticas = true,
        .max_lineas_mostrar = -1  // Sin límite
    };
    return config;
}

/* ================================
 * FUNCIÓN PRINCIPAL BÁSICA
 * ================================ */

int main_basico(void) {
    char linea[256];
    FILE *f = fopen("salida.txt", "r");

    if (f == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    printf("Contenido del archivo:\n");
    while (fgets(linea, sizeof(linea), f) != NULL) {
        printf("%s", linea);
    }

    fclose(f);
    return 0;
}

/* ================================
 * FUNCIÓN PRINCIPAL
 * ================================ */

int main(void) {
    printf("=== Ejercicio 074: Leer Archivo de Texto ===\n\n");
    
    // Crear archivo de ejemplo si no existe
    if (!archivo_existe_y_legible("salida.txt")) {
        FILE* archivo_ejemplo = fopen("salida.txt", "w");
        if (archivo_ejemplo) {
            fprintf(archivo_ejemplo, "Esta es la primera línea del archivo.\n");
            fprintf(archivo_ejemplo, "Esta es la segunda línea con números: 12345.\n");
            fprintf(archivo_ejemplo, "Tercera línea con caracteres especiales: !@#$%%.\n");
            fprintf(archivo_ejemplo, "\n");  // Línea vacía
            fprintf(archivo_ejemplo, "Quinta línea después de una línea vacía.\n");
            fprintf(archivo_ejemplo, "Última línea del archivo de ejemplo.\n");
            fclose(archivo_ejemplo);
            printf("Archivo de ejemplo 'salida.txt' creado.\n\n");
        }
    }
    
    // Ejecutar función básica del enunciado
    printf("1. Ejecutando función básica del enunciado:\n");
    printf("==========================================\n");
    main_basico();
    
    // Demostración de funciones avanzadas
    printf("\n\n2. Demostraciones avanzadas:\n");
    printf("============================\n");
    
    // Lectura con números de línea
    printf("\n--- Lectura con números de línea ---\n");
    leer_archivo_numerado("salida.txt");
    
    // Obtener estadísticas
    printf("\n--- Estadísticas del archivo ---\n");
    EstadisticasArchivo stats = obtener_estadisticas_archivo("salida.txt");
    if (stats.existe) {
        printf("Tamaño: %ld bytes\n", stats.tamaño);
        printf("Líneas: %d\n", stats.numero_lineas);
        printf("Palabras: %d\n", stats.numero_palabras);
        printf("Caracteres: %d\n", stats.numero_caracteres);
    }
    
    // Buscar palabra
    printf("\n--- Búsqueda de palabra ---\n");
    buscar_palabra_en_archivo("salida.txt", "línea", false);
    
    // Leer rango de líneas
    printf("\n--- Leer rango de líneas (2-4) ---\n");
    leer_rango_lineas("salida.txt", 2, 4);
    
    printf("\n=== Programa completado exitosamente ===\n");
    return 0;
}
