/*
 * Ejercicio 073: Escribir Archivo de Texto
 * 
 * Implementación completa de funciones para escribir archivos de texto,
 * incluyendo manejo de errores, diferentes modos de escritura y funciones avanzadas.
 */

#include "../include/escribir_archivo.h"
#include <stdarg.h>
#include <sys/stat.h>

/* ================================
 * FUNCIONES BÁSICAS DE ESCRITURA
 * ================================ */

bool escribir_linea_basica(const char* nombre_archivo, const char* texto) {
    if (!nombre_archivo || !texto) {
        fprintf(stderr, "Error: Parámetros nulos en escribir_linea_basica\n");
        return false;
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escritura", nombre_archivo);
        return false;
    }
    
    // Escribir texto
    if (fputs(texto, archivo) == EOF) {
        imprimir_error_archivo("escribir en archivo", nombre_archivo);
        fclose(archivo);
        return false;
    }
    
    // Añadir nueva línea si no termina con una
    if (texto[strlen(texto) - 1] != '\n') {
        fputc('\n', archivo);
    }
    
    if (fclose(archivo) != 0) {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
        return false;
    }
    
    return true;
}

bool escribir_con_formato(const char* nombre_archivo, const char* formato, ...) {
    if (!nombre_archivo || !formato) {
        fprintf(stderr, "Error: Parámetros nulos en escribir_con_formato\n");
        return false;
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escritura con formato", nombre_archivo);
        return false;
    }
    
    va_list args;
    va_start(args, formato);
    
    int resultado = vfprintf(archivo, formato, args);
    
    va_end(args);
    
    if (resultado < 0) {
        imprimir_error_archivo("escribir con formato en archivo", nombre_archivo);
        fclose(archivo);
        return false;
    }
    
    if (fclose(archivo) != 0) {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
        return false;
    }
    
    return true;
}

bool escribir_con_fputs(const char* nombre_archivo, const char* texto) {
    if (!nombre_archivo || !texto) {
        fprintf(stderr, "Error: Parámetros nulos en escribir_con_fputs\n");
        return false;
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escritura con fputs", nombre_archivo);
        return false;
    }
    
    if (fputs(texto, archivo) == EOF) {
        imprimir_error_archivo("escribir con fputs", nombre_archivo);
        fclose(archivo);
        return false;
    }
    
    if (fclose(archivo) != 0) {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
        return false;
    }
    
    return true;
}

bool escribir_con_fwrite(const char* nombre_archivo, const void* datos, size_t tamanio) {
    if (!nombre_archivo || !datos) {
        fprintf(stderr, "Error: Parámetros nulos en escribir_con_fwrite\n");
        return false;
    }
    
    FILE* archivo = fopen(nombre_archivo, "wb");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escritura binaria", nombre_archivo);
        return false;
    }
    
    size_t escritos = fwrite(datos, 1, tamanio, archivo);
    if (escritos != tamanio) {
        fprintf(stderr, "Error: Solo se escribieron %zu de %zu bytes\n", escritos, tamanio);
        fclose(archivo);
        return false;
    }
    
    if (fclose(archivo) != 0) {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
        return false;
    }
    
    return true;
}

/* ================================
 * FUNCIONES INTERACTIVAS
 * ================================ */

bool escribir_texto_interactivo(const char* nombre_archivo) {
    if (!nombre_archivo) {
        fprintf(stderr, "Error: Nombre de archivo nulo\n");
        return false;
    }
    
    char buffer[MAX_LINEA];
    
    printf("=== ESCRITURA DE ARCHIVO INTERACTIVA ===\n");
    printf("Archivo de destino: %s\n", nombre_archivo);
    printf("Introduce el texto a escribir (máximo %d caracteres):\n", MAX_LINEA - 1);
    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "Error al leer entrada del usuario\n");
        return false;
    }
    
    // Confirmar antes de escribir
    printf("\n¿Deseas escribir este contenido en '%s'? (s/n): ", nombre_archivo);
    char confirmacion;
    scanf(" %c", &confirmacion);
    
    if (confirmacion != 's' && confirmacion != 'S') {
        printf("Operación cancelada.\n");
        return false;
    }
    
    bool exito = escribir_linea_basica(nombre_archivo, buffer);
    
    if (exito) {
        printf("✓ Texto escrito exitosamente en '%s'\n", nombre_archivo);
    } else {
        printf("✗ Error al escribir en '%s'\n", nombre_archivo);
    }
    
    return exito;
}

EstadisticasEscritura escribir_multiples_lineas_interactivo(const char* nombre_archivo, ModoEscritura modo) {
    EstadisticasEscritura stats;
    estadisticas_inicializar(&stats);
    
    if (!nombre_archivo) {
        fprintf(stderr, "Error: Nombre de archivo nulo\n");
        return stats;
    }
    
    FILE* archivo = fopen(nombre_archivo, modo_a_cadena(modo));
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escritura múltiple", nombre_archivo);
        return stats;
    }
    
    char buffer[MAX_LINEA];
    
    printf("=== ESCRITURA DE MÚLTIPLES LÍNEAS ===\n");
    printf("Archivo: %s (modo: %s)\n", nombre_archivo, descripcion_modo(modo));
    printf("Escribe líneas de texto. Escribe 'FIN' en una línea para terminar.\n\n");
    
    int numero_linea = 1;
    while (true) {
        printf("Línea %d: ", numero_linea);
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "Error al leer línea %d\n", numero_linea);
            break;
        }
        
        // Verificar comando de fin
        if (strncmp(buffer, "FIN", 3) == 0 && (buffer[3] == '\n' || buffer[3] == '\0')) {
            printf("Finalizando escritura...\n");
            break;
        }
        
        // Escribir línea al archivo
        if (fputs(buffer, archivo) == EOF) {
            imprimir_error_archivo("escribir línea en archivo", nombre_archivo);
            break;
        }
        
        stats.lineas_escritas++;
        stats.caracteres_escritos += strlen(buffer);
        numero_linea++;
    }
    
    if (fclose(archivo) == 0) {
        stats.operacion_exitosa = true;
        stats.archivos_creados = 1;
    } else {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
    }
    
    stats.tiempo_fin = time(NULL);
    return stats;
}

bool crear_archivo_interactivo(void) {
    char nombre_archivo[MAX_NOMBRE_ARCHIVO];
    int opcion_modo;
    
    printf("=== CREADOR INTERACTIVO DE ARCHIVOS ===\n\n");
    
    // Solicitar nombre de archivo
    printf("Nombre del archivo a crear: ");
    if (!fgets(nombre_archivo, sizeof(nombre_archivo), stdin)) {
        fprintf(stderr, "Error al leer nombre de archivo\n");
        return false;
    }
    
    // Remover salto de línea
    nombre_archivo[strcspn(nombre_archivo, "\n")] = 0;
    
    if (!validar_nombre_archivo(nombre_archivo)) {
        printf("Error: Nombre de archivo no válido\n");
        return false;
    }
    
    // Verificar si existe
    if (archivo_existe(nombre_archivo)) {
        printf("⚠️  El archivo '%s' ya existe.\n", nombre_archivo);
        printf("Selecciona una opción:\n");
        printf("1. Sobrescribir\n");
        printf("2. Anexar al final\n");
        printf("3. Cancelar\n");
        printf("Opción: ");
        
        scanf("%d", &opcion_modo);
        getchar(); // Limpiar buffer
        
        if (opcion_modo == 3) {
            printf("Operación cancelada.\n");
            return false;
        }
    } else {
        opcion_modo = 1; // Crear nuevo
    }
    
    // Determinar modo
    ModoEscritura modo = (opcion_modo == 2) ? MODO_ANEXAR : MODO_ESCRIBIR;
    
    // Escribir contenido
        EstadisticasEscritura stats = escribir_multiples_lineas_interactivo(nombre_archivo, modo);
    
    if (stats.operacion_exitosa) {
        printf("\n✓ Archivo creado exitosamente!\n");
        estadisticas_imprimir(&stats, "Resumen de la operación");
        
        // Mostrar información del archivo creado
        int lineas, caracteres;
        long tamanio;
        if (obtener_info_archivo_detallada(nombre_archivo, &lineas, &caracteres, &tamanio)) {
            printf("\nInformación del archivo '%s':\n", nombre_archivo);
            printf("  Líneas: %d\n", lineas);
            printf("  Caracteres: %d\n", caracteres);
            printf("  Tamaño: %ld bytes\n", tamanio);
        }
        
        return true;
    } else {
        printf("\n✗ Error al crear el archivo\n");
        return false;
    }
}

/* ================================
 * FUNCIONES AVANZADAS
 * ================================ */

EstadisticasEscritura escribir_array_lineas(const char* nombre_archivo, 
                                           char** lineas, int num_lineas, 
                                           const char* separador) {
    EstadisticasEscritura stats;
    estadisticas_inicializar(&stats);
    
    if (!nombre_archivo || !lineas || num_lineas <= 0) {
        fprintf(stderr, "Error: Parámetros inválidos en escribir_array_lineas\n");
        return stats;
    }
    
    if (!separador) {
        separador = "\n";
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escribir array", nombre_archivo);
        return stats;
    }
    
    for (int i = 0; i < num_lineas; i++) {
        if (lineas[i]) {
            if (fputs(lineas[i], archivo) == EOF) {
                imprimir_error_archivo("escribir línea del array", nombre_archivo);
                fclose(archivo);
                return stats;
            }
            
            if (i < num_lineas - 1) {
                fputs(separador, archivo);
            }
            
            stats.lineas_escritas++;
            stats.caracteres_escritos += strlen(lineas[i]);
        }
    }
    
    if (fclose(archivo) == 0) {
        stats.operacion_exitosa = true;
        stats.archivos_creados = 1;
    }
    
    stats.tiempo_fin = time(NULL);
    return stats;
}

bool escribir_con_timestamp(const char* nombre_archivo, const char* contenido, bool incluir_timestamp) {
    if (!nombre_archivo || !contenido) {
        fprintf(stderr, "Error: Parámetros nulos en escribir_con_timestamp\n");
        return false;
    }
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para escritura con timestamp", nombre_archivo);
        return false;
    }
    
    bool exito = true;
    
    if (incluir_timestamp) {
        time_t ahora = time(NULL);
        char* timestamp = ctime(&ahora);
        // Remover salto de línea del timestamp
        timestamp[strlen(timestamp) - 1] = '\0';
        
        if (fprintf(archivo, "# Creado el: %s\n\n", timestamp) < 0) {
            exito = false;
        }
    }
    
    if (exito && fputs(contenido, archivo) == EOF) {
        exito = false;
    }
    
    if (!exito) {
        imprimir_error_archivo("escribir contenido con timestamp", nombre_archivo);
    }
    
    if (fclose(archivo) != 0) {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
        return false;
    }
    
    return exito;
}

bool anexar_contenido(const char* nombre_archivo, const char* contenido) {
    if (!nombre_archivo || !contenido) {
        fprintf(stderr, "Error: Parámetros nulos en anexar_contenido\n");
        return false;
    }
    
    FILE* archivo = fopen(nombre_archivo, "a");
    if (!archivo) {
        imprimir_error_archivo("abrir archivo para anexar", nombre_archivo);
        return false;
    }
    
    bool exito = true;
    
    if (fputs(contenido, archivo) == EOF) {
        imprimir_error_archivo("anexar contenido", nombre_archivo);
        exito = false;
    }
    
    if (fclose(archivo) != 0) {
        imprimir_error_archivo("cerrar archivo", nombre_archivo);
        return false;
    }
    
    return exito;
}

bool escribir_con_backup(const char* nombre_archivo, const char* contenido, bool crear_backup) {
    if (!nombre_archivo || !contenido) {
        fprintf(stderr, "Error: Parámetros nulos en escribir_con_backup\n");
        return false;
    }
    
    // Crear backup si se solicita y el archivo existe
    if (crear_backup && archivo_existe(nombre_archivo)) {
        char nombre_backup[MAX_NOMBRE_ARCHIVO];
        if (!crear_nombre_backup(nombre_archivo, nombre_backup, sizeof(nombre_backup))) {
            fprintf(stderr, "Error: No se pudo crear nombre de backup\n");
            return false;
        }
        
        // Copiar archivo original a backup
        FILE* original = fopen(nombre_archivo, "r");
        FILE* backup = fopen(nombre_backup, "w");
        
        if (!original || !backup) {
            if (original) fclose(original);
            if (backup) fclose(backup);
            fprintf(stderr, "Error: No se pudo crear backup\n");
            return false;
        }
        
        int c;
        while ((c = fgetc(original)) != EOF) {
            fputc(c, backup);
        }
        
        fclose(original);
        fclose(backup);
        
        printf("✓ Backup creado: %s\n", nombre_backup);
    }
    
    // Escribir nuevo contenido
    return escribir_linea_basica(nombre_archivo, contenido);
}

/* ================================
 * GESTIÓN DE BUFFER
 * ================================ */

BufferTexto* buffer_crear(int capacidad_inicial) {
    if (capacidad_inicial <= 0) {
        capacidad_inicial = 10;
    }
    
    BufferTexto* buffer = malloc(sizeof(BufferTexto));
    if (!buffer) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el buffer\n");
        return NULL;
    }
    
    buffer->lineas = malloc(sizeof(char*) * capacidad_inicial);
    if (!buffer->lineas) {
        free(buffer);
        fprintf(stderr, "Error: No se pudo asignar memoria para las líneas del buffer\n");
        return NULL;
    }
    
    buffer->num_lineas = 0;
    buffer->capacidad = capacidad_inicial;
    
    return buffer;
}

void buffer_destruir(BufferTexto* buffer) {
    if (!buffer) return;
    
    for (int i = 0; i < buffer->num_lineas; i++) {
        free(buffer->lineas[i]);
    }
    
    free(buffer->lineas);
    free(buffer);
}

bool buffer_agregar_linea(BufferTexto* buffer, const char* linea) {
    if (!buffer || !linea) {
        return false;
    }
    
    // Expandir buffer si es necesario
    if (buffer->num_lineas >= buffer->capacidad) {
        int nueva_capacidad = buffer->capacidad * 2;
        char** nuevas_lineas = realloc(buffer->lineas, sizeof(char*) * nueva_capacidad);
        
        if (!nuevas_lineas) {
            fprintf(stderr, "Error: No se pudo expandir el buffer\n");
            return false;
        }
        
        buffer->lineas = nuevas_lineas;
        buffer->capacidad = nueva_capacidad;
    }
    
    // Copiar línea
    buffer->lineas[buffer->num_lineas] = malloc(strlen(linea) + 1);
    if (!buffer->lineas[buffer->num_lineas]) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la línea\n");
        return false;
    }
    
    strcpy(buffer->lineas[buffer->num_lineas], linea);
    buffer->num_lineas++;
    
    return true;
}

EstadisticasEscritura buffer_escribir_archivo(BufferTexto* buffer, const char* nombre_archivo) {
    EstadisticasEscritura stats;
    estadisticas_inicializar(&stats);
    
    if (!buffer || !nombre_archivo) {
        fprintf(stderr, "Error: Parámetros nulos en buffer_escribir_archivo\n");
        return stats;
    }
    
    return escribir_array_lineas(nombre_archivo, buffer->lineas, buffer->num_lineas, "\n");
}

/* ================================
 * VALIDACIÓN Y UTILIDADES
 * ================================ */

bool validar_nombre_archivo(const char* nombre_archivo) {
    if (!nombre_archivo || strlen(nombre_archivo) == 0) {
        return false;
    }
    
    // Verificar longitud
    if (strlen(nombre_archivo) >= MAX_NOMBRE_ARCHIVO) {
        return false;
    }
    
    // Verificar caracteres inválidos (básico)
    const char* invalidos = "<>:\"|?*";
    for (const char* p = invalidos; *p; p++) {
        if (strchr(nombre_archivo, *p)) {
            return false;
        }
    }
    
    return true;
}

bool archivo_existe(const char* nombre_archivo) {
    if (!nombre_archivo) return false;
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo) {
        fclose(archivo);
        return true;
    }
    return false;
}

long obtener_tamanio_archivo(const char* nombre_archivo) {
    if (!nombre_archivo) return -1;
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) return -1;
    
    fseek(archivo, 0, SEEK_END);
    long tamanio = ftell(archivo);
    fclose(archivo);
    
    return tamanio;
}

bool crear_nombre_backup(const char* nombre_original, char* nombre_backup, size_t tamanio_buffer) {
    if (!nombre_original || !nombre_backup || tamanio_buffer == 0) {
        return false;
    }
    
    time_t ahora = time(NULL);
    struct tm* tiempo = localtime(&ahora);
    
    // Buscar extensión
    const char* punto = strrchr(nombre_original, '.');
    
    if (punto) {
        // Archivo con extensión
        size_t longitud_base = punto - nombre_original;
        snprintf(nombre_backup, tamanio_buffer, "%.*s_backup_%04d%02d%02d_%02d%02d%02d%s",
                (int)longitud_base, nombre_original,
                tiempo->tm_year + 1900, tiempo->tm_mon + 1, tiempo->tm_mday,
                tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec,
                punto);
    } else {
        // Archivo sin extensión
        snprintf(nombre_backup, tamanio_buffer, "%s_backup_%04d%02d%02d_%02d%02d%02d",
                nombre_original,
                tiempo->tm_year + 1900, tiempo->tm_mon + 1, tiempo->tm_mday,
                tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec);
    }
    
    return true;
}

bool limpiar_archivo(const char* nombre_archivo) {
    if (!nombre_archivo) return false;
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        imprimir_error_archivo("limpiar archivo", nombre_archivo);
        return false;
    }
    
    bool exito = (fclose(archivo) == 0);
    if (!exito) {
        imprimir_error_archivo("cerrar archivo limpiado", nombre_archivo);
    }
    
    return exito;
}

/* ================================
 * MANEJO DE ERRORES
 * ================================ */

void imprimir_error_archivo(const char* operacion, const char* nombre_archivo) {
    if (!operacion) operacion = "operación desconocida";
    if (!nombre_archivo) nombre_archivo = "archivo desconocido";
    
    char buffer_error[256];
    obtener_descripcion_error(buffer_error, sizeof(buffer_error));
    
    fprintf(stderr, "Error al %s '%s': %s\n", operacion, nombre_archivo, buffer_error);
}

char* obtener_descripcion_error(char* buffer, size_t tamanio_buffer) {
    if (!buffer || tamanio_buffer == 0) return NULL;
    
    if (errno != 0) {
        strncpy(buffer, strerror(errno), tamanio_buffer - 1);
        buffer[tamanio_buffer - 1] = '\0';
    } else {
        strncpy(buffer, "Error desconocido", tamanio_buffer - 1);
        buffer[tamanio_buffer - 1] = '\0';
    }
    
    return buffer;
}

/* ================================
 * ESTADÍSTICAS E INFORMACIÓN
 * ================================ */

void estadisticas_inicializar(EstadisticasEscritura* stats) {
    if (!stats) return;
    
    stats->lineas_escritas = 0;
    stats->caracteres_escritos = 0;
    stats->archivos_creados = 0;
    stats->tiempo_inicio = time(NULL);
    stats->tiempo_fin = 0;
    stats->operacion_exitosa = false;
}

void estadisticas_imprimir(const EstadisticasEscritura* stats, const char* titulo) {
    if (!stats) return;
    
    if (!titulo) titulo = "Estadísticas de Escritura";
    
    printf("\n=== %s ===\n", titulo);
    printf("Líneas escritas: %d\n", stats->lineas_escritas);
    printf("Caracteres escritos: %d\n", stats->caracteres_escritos);
    printf("Archivos creados: %d\n", stats->archivos_creados);
    
    if (stats->tiempo_fin > stats->tiempo_inicio) {
        double duracion = difftime(stats->tiempo_fin, stats->tiempo_inicio);
        printf("Duración: %.2f segundos\n", duracion);
    }
    
    printf("Estado: %s\n", stats->operacion_exitosa ? "✓ Exitoso" : "✗ Error");
    printf("========================\n");
}

bool obtener_info_archivo_detallada(const char* nombre_archivo, int* lineas, int* caracteres, long* tamanio) {
    if (!nombre_archivo) return false;
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) return false;
    
    int conteo_lineas = 0;
    int conteo_caracteres = 0;
    int c;
    
    while ((c = fgetc(archivo)) != EOF) {
        conteo_caracteres++;
        if (c == '\n') {
            conteo_lineas++;
        }
    }
    
    // Si el archivo no termina en nueva línea pero tiene contenido, contar como una línea
    if (conteo_caracteres > 0 && c != '\n') {
        conteo_lineas++;
    }
    
    fseek(archivo, 0, SEEK_END);
    long tamanio_archivo = ftell(archivo);
    
    fclose(archivo);
    
    if (lineas) *lineas = conteo_lineas;
    if (caracteres) *caracteres = conteo_caracteres;
    if (tamanio) *tamanio = tamanio_archivo;
    
    return true;
}

/* ================================
 * CONVERSIÓN DE MODO
 * ================================ */

const char* modo_a_cadena(ModoEscritura modo) {
    switch (modo) {
        case MODO_ESCRIBIR: return "w";
        case MODO_ANEXAR: return "a";
        case MODO_CREAR_NUEVO: return "wx";
        default: return "w";
    }
}

const char* descripcion_modo(ModoEscritura modo) {
    switch (modo) {
        case MODO_ESCRIBIR: return "Sobrescribir archivo";
        case MODO_ANEXAR: return "Anexar al final";
        case MODO_CREAR_NUEVO: return "Crear nuevo (falla si existe)";
        default: return "Modo desconocido";
    }
}

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

void demo_escritura_basica(void) {
    printf("\n=== DEMOSTRACIÓN: ESCRITURA BÁSICA ===\n");
    
    const char* archivo_demo = "demo_basico.txt";
    const char* contenido = "¡Hola mundo desde la escritura básica de archivos!";
    
    printf("Escribiendo en archivo '%s'...\n", archivo_demo);
    
    if (escribir_linea_basica(archivo_demo, contenido)) {
        printf("✓ Escritura exitosa\n");
        
        // Mostrar información del archivo
        int lineas, caracteres;
        long tamanio;
        if (obtener_info_archivo_detallada(archivo_demo, &lineas, &caracteres, &tamanio)) {
            printf("Archivo creado: %d líneas, %d caracteres, %ld bytes\n", 
                   lineas, caracteres, tamanio);
        }
    } else {
        printf("✗ Error en la escritura\n");
    }
}

void demo_metodos_escritura(void) {
    printf("\n=== DEMOSTRACIÓN: MÉTODOS DE ESCRITURA ===\n");
    
    // Método 1: fputs
    printf("1. Usando fputs...\n");
    escribir_con_fputs("demo_fputs.txt", "Escrito con fputs\n");
    
    // Método 2: fprintf
    printf("2. Usando fprintf...\n");
    escribir_con_formato("demo_fprintf.txt", "Número: %d, Texto: %s, Float: %.2f\n", 
                        42, "ejemplo", 3.14159);
    
    // Método 3: fwrite
    printf("3. Usando fwrite...\n");
    const char* datos = "Datos binarios escritos con fwrite";
    escribir_con_fwrite("demo_fwrite.txt", datos, strlen(datos));
    
    printf("✓ Tres archivos de demostración creados\n");
}

void demo_escritura_interactiva(void) {
    printf("\n=== DEMOSTRACIÓN: ESCRITURA INTERACTIVA ===\n");
    
    char respuesta;
    printf("¿Quieres probar la escritura interactiva? (s/n): ");
    scanf(" %c", &respuesta);
    getchar(); // Limpiar buffer
    
    if (respuesta == 's' || respuesta == 'S') {
        escribir_texto_interactivo("archivo_interactivo.txt");
    } else {
        printf("Demostración interactiva omitida.\n");
    }
}

void demo_funciones_avanzadas(void) {
    printf("\n=== DEMOSTRACIÓN: FUNCIONES AVANZADAS ===\n");
    
    // 1. Escribir con timestamp
    printf("1. Escribiendo archivo con timestamp...\n");
    escribir_con_timestamp("demo_timestamp.txt", 
                          "Este archivo fue creado automáticamente.\n"
                          "Contiene información de tiempo de creación.\n", true);
    
    // 2. Escribir array de líneas
    printf("2. Escribiendo array de líneas...\n");
    char* lineas[] = {
        "Primera línea del array",
        "Segunda línea del array", 
        "Tercera línea del array",
        "Última línea del array"
    };
    EstadisticasEscritura stats = escribir_array_lineas("demo_array.txt", lineas, 4, "\n");
    estadisticas_imprimir(&stats, "Escritura de Array");
    
    // 3. Anexar contenido
    printf("3. Anexando contenido...\n");
    anexar_contenido("demo_array.txt", "\n--- Contenido anexado ---\n");
    anexar_contenido("demo_array.txt", "Esta línea fue agregada posteriormente.\n");
    
    // 4. Escritura con backup
    printf("4. Escribiendo con backup...\n");
    escribir_con_backup("demo_array.txt", 
                       "Contenido completamente nuevo (backup creado)\n", true);
    
    printf("✓ Demostración de funciones avanzadas completada\n");
}

void demo_completa_escritura_archivos(void) {
    printf("=== DEMOSTRACIÓN COMPLETA: ESCRITURA DE ARCHIVOS ===\n");
    
    // Ejemplo básico (como el proporcionado por el usuario)
    printf("\n--- EJEMPLO BÁSICO ---\n");
    char texto[256];
    printf("Introduce una línea de texto para guardar en el archivo:\n");
    fgets(texto, sizeof(texto), stdin);
    
    FILE *f = fopen("salida.txt", "w");
    if (f == NULL) {
        perror("No se pudo abrir el archivo");
        return;
    }
    
    fputs(texto, f);
    fclose(f);
    printf("Texto guardado correctamente en 'salida.txt'\n");
    
    // Demostraciones adicionales
    demo_escritura_basica();
    demo_metodos_escritura();
    demo_funciones_avanzadas();
    
    // Buffer de texto
    printf("\n--- DEMOSTRACIÓN: BUFFER DE TEXTO ---\n");
    BufferTexto* buffer = buffer_crear(5);
    if (buffer) {
        buffer_agregar_linea(buffer, "Línea 1 del buffer");
        buffer_agregar_linea(buffer, "Línea 2 del buffer");
        buffer_agregar_linea(buffer, "Línea 3 del buffer");
        
        EstadisticasEscritura stats = buffer_escribir_archivo(buffer, "demo_buffer.txt");
        estadisticas_imprimir(&stats, "Escritura desde Buffer");
        
        buffer_destruir(buffer);
    }
    
    // Resumen final
    printf("\n--- RESUMEN DE ARCHIVOS CREADOS ---\n");
    const char* archivos[] = {
        "salida.txt", "demo_basico.txt", "demo_fputs.txt", 
        "demo_fprintf.txt", "demo_fwrite.txt", "demo_timestamp.txt",
        "demo_array.txt", "demo_buffer.txt"
    };
    
    for (int i = 0; i < 8; i++) {
        if (archivo_existe(archivos[i])) {
            long tamanio = obtener_tamanio_archivo(archivos[i]);
            printf("✓ %s (%ld bytes)\n", archivos[i], tamanio);
        }
    }
    
    printf("\n=== FIN DE LA DEMOSTRACIÓN ===\n");
}

/* ================================
 * FUNCIÓN MAIN PARA PRUEBAS
 * ================================ */

int main(void) {
    demo_completa_escritura_archivos();
    return 0;
}

/* ================================
 * WRAPPERS PARA API DE TESTS
 * ================================ */

static ResultadoEscritura resultado_ok(const char *mensaje) {
    ResultadoEscritura r;
    r.exito = true;
    if (mensaje) {
        strncpy(r.mensaje, mensaje, sizeof(r.mensaje) - 1);
        r.mensaje[sizeof(r.mensaje) - 1] = '\0';
    } else {
        r.mensaje[0] = '\0';
    }
    return r;
}

static ResultadoEscritura resultado_error(const char *mensaje) {
    ResultadoEscritura r;
    r.exito = false;
    if (mensaje) {
        strncpy(r.mensaje, mensaje, sizeof(r.mensaje) - 1);
        r.mensaje[sizeof(r.mensaje) - 1] = '\0';
    } else {
        r.mensaje[0] = '\0';
    }
    return r;
}

ResultadoEscritura escribir_archivo_texto(const char* nombre_archivo, const char* mensaje) {
    if (!nombre_archivo) return resultado_error("Nombre de archivo inválido");
    if (!mensaje) return resultado_error("Mensaje inválido");

    FILE *f = fopen(nombre_archivo, "w");
    if (!f) return resultado_error("Error al abrir el archivo");

    if (fputs(mensaje, f) == EOF) {
        fclose(f);
        return resultado_error("Error al escribir en el archivo");
    }
    if (fclose(f) != 0) {
        return resultado_error("Error al cerrar el archivo");
    }
    return resultado_ok("Archivo escrito exitosamente");
}

ResultadoEscritura escribir_archivo_formato(const char* nombre_archivo, const char* formato, ...) {
    if (!nombre_archivo || !formato) return resultado_error("Parámetros inválidos");
    FILE *f = fopen(nombre_archivo, "w");
    if (!f) return resultado_error("Error al abrir el archivo");
    va_list args;
    va_start(args, formato);
    int res = vfprintf(f, formato, args);
    va_end(args);
    if (res < 0) {
        fclose(f);
        return resultado_error("Error al escribir en el archivo");
    }
    if (fclose(f) != 0) return resultado_error("Error al cerrar el archivo");
    return resultado_ok("Archivo escrito exitosamente");
}

ResultadoEscritura concatenar_archivos(const char* archivo1, const char* archivo2, const char* archivo_destino) {
    if (!archivo1 || !archivo2 || !archivo_destino) return resultado_error("Parámetros inválidos");
    FILE *a = fopen(archivo1, "r");
    FILE *b = fopen(archivo2, "r");
    FILE *out = fopen(archivo_destino, "w");
    if (!a || !b || !out) {
        if (a) fclose(a); if (b) fclose(b); if (out) fclose(out);
        return resultado_error("Error al abrir el archivo");
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), a)) > 0) {
        if (fwrite(buf, 1, n, out) != n) { fclose(a); fclose(b); fclose(out); return resultado_error("Error al escribir en el archivo"); }
    }
    while ((n = fread(buf, 1, sizeof(buf), b)) > 0) {
        if (fwrite(buf, 1, n, out) != n) { fclose(a); fclose(b); fclose(out); return resultado_error("Error al escribir en el archivo"); }
    }
    fclose(a); fclose(b);
    if (fclose(out) != 0) return resultado_error("Error al cerrar el archivo");
    return resultado_ok("Archivos concatenados exitosamente");
}

ResultadoEscritura escribir_multiples_lineas(const char* nombre_archivo, const char* lineas[], int num_lineas) {
    if (!nombre_archivo || !lineas || num_lineas < 0) return resultado_error("Parámetros inválidos");
    FILE *f = fopen(nombre_archivo, "w");
    if (!f) return resultado_error("Error al abrir el archivo");
    for (int i = 0; i < num_lineas; i++) {
        const char *s = lineas[i] ? lineas[i] : "";
        if (fputs(s, f) == EOF || fputc('\n', f) == EOF) {
            fclose(f);
            return resultado_error("Error al escribir en el archivo");
        }
    }
    if (fclose(f) != 0) return resultado_error("Error al cerrar el archivo");
    return resultado_ok("Archivo escrito exitosamente");
}

InfoArchivo obtener_info_archivo(const char* nombre_archivo) {
    InfoArchivo info = {0};
    if (!nombre_archivo) return info;
    struct stat st;
    if (stat(nombre_archivo, &st) != 0) {
        info.existe = false;
        info.tamaño = 0;
        info.fecha_modificacion = 0;
        return info;
    }
    info.existe = true;
    info.tamaño = (unsigned long)st.st_size;
    info.fecha_modificacion = st.st_mtime;
    return info;
}

ResultadoEscritura copiar_archivo(const char* archivo_origen, const char* archivo_destino) {
    if (!archivo_origen || !archivo_destino) return resultado_error("Parámetros inválidos");
    FILE *in = fopen(archivo_origen, "r");
    if (!in) return resultado_error("Error al abrir el archivo");
    FILE *out = fopen(archivo_destino, "w");
    if (!out) { fclose(in); return resultado_error("Error al abrir el archivo"); }
    char buf[4096]; size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) { fclose(in); fclose(out); return resultado_error("Error al escribir en el archivo"); }
    }
    fclose(in);
    if (fclose(out) != 0) return resultado_error("Error al cerrar el archivo");
    return resultado_ok("Archivo copiado correctamente");
}
