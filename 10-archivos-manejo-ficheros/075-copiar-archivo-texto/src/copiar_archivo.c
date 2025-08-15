/**
 * @file copiar_archivo.c
 * @brief Implementación de funciones para copia de archivos de texto
 * @author Sistema de Ejercicios C
 * @date 2024
 * 
 * Implementación completa de funciones para copiar archivos de texto,
 * incluyendo diferentes métodos de copia, verificación de integridad,
 * manejo de errores y análisis de rendimiento.
 */

#include "../include/copiar_archivo.h"
#include <unistd.h>

/* ================================
 * FUNCIONES BÁSICAS DE COPIA
 * ================================ */

ResultadoCopia copiar_archivo_basico(const char* archivo_origen, 
                                    const char* archivo_destino) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    if (!archivo_origen || !archivo_destino) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombres de archivo inválidos");
        return resultado;
    }
    
    clock_t inicio = clock();
    
    FILE* origen = fopen(archivo_origen, "r");
    FILE* destino = fopen(archivo_destino, "w");
    char buffer[256];
    
    if (origen == NULL || destino == NULL) {
        if (origen) fclose(origen);
        if (destino) fclose(destino);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivos: %s", strerror(errno));
        return resultado;
    }
    
    long bytes_copiados = 0;
    
    while (fgets(buffer, sizeof(buffer), origen) != NULL) {
        if (fputs(buffer, destino) == EOF) {
            fclose(origen);
            fclose(destino);
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error al escribir en el archivo destino: %s", strerror(errno));
            return resultado;
        }
        bytes_copiados += strlen(buffer);
    }
    
    fclose(origen);
    fclose(destino);
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    resultado.exito = true;
    resultado.bytes_copiados = bytes_copiados;
    resultado.tiempo_transcurrido = tiempo;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Archivo copiado correctamente");
    
    return resultado;
}

ResultadoCopia copiar_linea_por_linea(const char* archivo_origen, 
                                     const char* archivo_destino) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    if (!archivo_origen || !archivo_destino) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombres de archivo inválidos");
        return resultado;
    }
    
    clock_t inicio = clock();
    
    FILE* origen = fopen(archivo_origen, "r");
    if (!origen) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo origen: %s", strerror(errno));
        return resultado;
    }
    
    FILE* destino = fopen(archivo_destino, "w");
    if (!destino) {
        fclose(origen);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo destino: %s", strerror(errno));
        return resultado;
    }
    
    char linea[BUFFER_LINEA];
    long bytes_copiados = 0;
    int lineas_copiadas = 0;
    
    while (fgets(linea, sizeof(linea), origen) != NULL) {
        if (fputs(linea, destino) == EOF) {
            fclose(origen);
            fclose(destino);
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error al escribir línea %d: %s", lineas_copiadas + 1, strerror(errno));
            return resultado;
        }
        bytes_copiados += strlen(linea);
        lineas_copiadas++;
    }
    
    // Verificar si hubo error en la lectura
    if (ferror(origen)) {
        fclose(origen);
        fclose(destino);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al leer archivo origen");
        return resultado;
    }
    
    fclose(origen);
    fclose(destino);
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    resultado.exito = true;
    resultado.bytes_copiados = bytes_copiados;
    resultado.tiempo_transcurrido = tiempo;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Copia línea por línea exitosa: %d líneas, %ld bytes", 
            lineas_copiadas, bytes_copiados);
    
    return resultado;
}

ResultadoCopia copiar_caracter_por_caracter(const char* archivo_origen, 
                                           const char* archivo_destino) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_CARACTER_POR_CARACTER};
    
    if (!archivo_origen || !archivo_destino) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombres de archivo inválidos");
        return resultado;
    }
    
    clock_t inicio = clock();
    
    FILE* origen = fopen(archivo_origen, "r");
    if (!origen) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo origen: %s", strerror(errno));
        return resultado;
    }
    
    FILE* destino = fopen(archivo_destino, "w");
    if (!destino) {
        fclose(origen);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo destino: %s", strerror(errno));
        return resultado;
    }
    
    int caracter;
    long caracteres_copiados = 0;
    
    while ((caracter = fgetc(origen)) != EOF) {
        if (fputc(caracter, destino) == EOF) {
            fclose(origen);
            fclose(destino);
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error al escribir carácter %ld: %s", 
                    caracteres_copiados + 1, strerror(errno));
            return resultado;
        }
        caracteres_copiados++;
    }
    
    // Verificar si hubo error en la lectura
    if (ferror(origen)) {
        fclose(origen);
        fclose(destino);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al leer archivo origen");
        return resultado;
    }
    
    fclose(origen);
    fclose(destino);
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    resultado.exito = true;
    resultado.bytes_copiados = caracteres_copiados;
    resultado.tiempo_transcurrido = tiempo;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Copia carácter por carácter exitosa: %ld caracteres", 
            caracteres_copiados);
    
    return resultado;
}

ResultadoCopia copiar_por_bloques(const char* archivo_origen, 
                                 const char* archivo_destino,
                                 size_t tamaño_buffer) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_BLOQUE};
    
    if (!archivo_origen || !archivo_destino || tamaño_buffer == 0) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    clock_t inicio = clock();
    
    FILE* origen = fopen(archivo_origen, "rb");  // Modo binario para bloques
    if (!origen) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo origen: %s", strerror(errno));
        return resultado;
    }
    
    FILE* destino = fopen(archivo_destino, "wb");  // Modo binario para bloques
    if (!destino) {
        fclose(origen);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo destino: %s", strerror(errno));
        return resultado;
    }
    
    char* buffer = malloc(tamaño_buffer);
    if (!buffer) {
        fclose(origen);
        fclose(destino);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error de memoria al allocar buffer de %zu bytes", tamaño_buffer);
        return resultado;
    }
    
    size_t bytes_leidos;
    long total_copiado = 0;
    
    while ((bytes_leidos = fread(buffer, 1, tamaño_buffer, origen)) > 0) {
        size_t bytes_escritos = fwrite(buffer, 1, bytes_leidos, destino);
        
        if (bytes_escritos != bytes_leidos) {
            free(buffer);
            fclose(origen);
            fclose(destino);
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error al escribir bloque: esperado %zu, escrito %zu", 
                    bytes_leidos, bytes_escritos);
            return resultado;
        }
        
        total_copiado += bytes_escritos;
    }
    
    // Verificar si hubo error en la lectura
    if (ferror(origen)) {
        free(buffer);
        fclose(origen);
        fclose(destino);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al leer archivo origen");
        return resultado;
    }
    
    free(buffer);
    fclose(origen);
    fclose(destino);
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    resultado.exito = true;
    resultado.bytes_copiados = total_copiado;
    resultado.tiempo_transcurrido = tiempo;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Copia por bloques exitosa: %ld bytes (buffer: %zu bytes)", 
            total_copiado, tamaño_buffer);
    
    return resultado;
}

/* ================================
 * FUNCIONES AVANZADAS DE COPIA
 * ================================ */

ResultadoCopia copiar_archivo_configurado(const char* archivo_origen, 
                                         const char* archivo_destino,
                                         const ConfiguracionCopia* config) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    if (!archivo_origen || !archivo_destino || !config) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    // Verificar que el archivo origen existe
    if (!archivo_existe_y_legible(archivo_origen)) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Archivo origen no existe o no es legible: %s", archivo_origen);
        return resultado;
    }
    
    // Crear backup si está configurado y el destino existe
    if (config->crear_backup && archivo_existe_y_legible(archivo_destino)) {
        if (!crear_backup_archivo(archivo_destino)) {
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error al crear backup del archivo destino");
            return resultado;
        }
        printf("Backup creado para %s\n", archivo_destino);
    }
    
    // Ejecutar copia según el método configurado
    switch (config->metodo) {
        case METODO_LINEA_POR_LINEA:
            resultado = copiar_linea_por_linea(archivo_origen, archivo_destino);
            break;
        case METODO_CARACTER_POR_CARACTER:
            resultado = copiar_caracter_por_caracter(archivo_origen, archivo_destino);
            break;
        case METODO_BLOQUE:
            resultado = copiar_por_bloques(archivo_origen, archivo_destino, 
                                         config->tamaño_buffer);
            break;
        case METODO_CHUNK:
            resultado = copiar_por_bloques(archivo_origen, archivo_destino, CHUNK_SIZE);
            break;
        default:
            resultado = copiar_linea_por_linea(archivo_origen, archivo_destino);
            break;
    }
    
    if (!resultado.exito) {
        return resultado;
    }
    
    // Realizar verificación si está configurada
    if (config->verificacion != VERIFICACION_NINGUNA) {
        bool verificacion_ok = verificar_archivos_identicos(archivo_origen, 
                                                           archivo_destino,
                                                           config->verificacion);
        if (!verificacion_ok) {
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Copia realizada pero falló la verificación de integridad");
            resultado.exito = false;
            return resultado;
        }
        
        // Actualizar mensaje con información de verificación
        char mensaje_temp[512];
        snprintf(mensaje_temp, sizeof(mensaje_temp), 
                "%s - Verificación exitosa", resultado.mensaje);
        strncpy(resultado.mensaje, mensaje_temp, sizeof(resultado.mensaje) - 1);
        resultado.mensaje[sizeof(resultado.mensaje) - 1] = '\0';
    }
    
    resultado.metodo_usado = config->metodo;
    return resultado;
}

ResultadoCopia copiar_archivo_con_progreso(const char* archivo_origen, 
                                          const char* archivo_destino,
                                          void (*callback)(const ProgresoCopia*)) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_BLOQUE};
    
    if (!archivo_origen || !archivo_destino) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombres de archivo inválidos");
        return resultado;
    }
    
    // Obtener tamaño del archivo origen
    long tamaño_origen = obtener_tamaño_archivo(archivo_origen);
    if (tamaño_origen < 0) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al obtener tamaño del archivo origen");
        return resultado;
    }
    
    clock_t inicio = clock();
    
    FILE* origen = fopen(archivo_origen, "rb");
    if (!origen) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo origen: %s", strerror(errno));
        return resultado;
    }
    
    FILE* destino = fopen(archivo_destino, "wb");
    if (!destino) {
        fclose(origen);
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al abrir archivo destino: %s", strerror(errno));
        return resultado;
    }
    
    char buffer[CHUNK_SIZE];
    size_t bytes_leidos;
    long total_copiado = 0;
    ProgresoCopia progreso = {0};
    
    progreso.bytes_totales = tamaño_origen;
    
    while ((bytes_leidos = fread(buffer, 1, sizeof(buffer), origen)) > 0) {
        size_t bytes_escritos = fwrite(buffer, 1, bytes_leidos, destino);
        
        if (bytes_escritos != bytes_leidos) {
            fclose(origen);
            fclose(destino);
            snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                    "Error al escribir datos");
            return resultado;
        }
        
        total_copiado += bytes_escritos;
        
        // Actualizar progreso
        progreso.bytes_procesados = total_copiado;
        progreso.porcentaje = (double)total_copiado / tamaño_origen * 100.0;
        
        clock_t actual = clock();
        double tiempo_transcurrido = ((double)(actual - inicio)) / CLOCKS_PER_SEC;
        if (tiempo_transcurrido > 0) {
            progreso.velocidad_actual = calcular_velocidad_transferencia(total_copiado, 
                                                                        tiempo_transcurrido);
            
            if (progreso.velocidad_actual > 0) {
                double bytes_restantes = tamaño_origen - total_copiado;
                progreso.tiempo_estimado = (time_t)(bytes_restantes / 
                                                   (progreso.velocidad_actual * 1024 * 1024));
            }
        }
        
        // Llamar callback si está disponible
        if (callback) {
            callback(&progreso);
        }
    }
    
    fclose(origen);
    fclose(destino);
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    resultado.exito = true;
    resultado.bytes_copiados = total_copiado;
    resultado.tiempo_transcurrido = tiempo;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Copia con progreso exitosa: %ld bytes", total_copiado);
    
    return resultado;
}

ResultadoCopia copiar_archivos_lote(const char** archivos_origen,
                                   const char** archivos_destino,
                                   int num_archivos,
                                   const ConfiguracionCopia* config) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    if (!archivos_origen || !archivos_destino || num_archivos <= 0 || !config) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos para copia en lote");
        return resultado;
    }
    
    clock_t inicio = clock();
    long total_bytes = 0;
    int archivos_exitosos = 0;
    int archivos_fallidos = 0;
    
    printf("Iniciando copia en lote de %d archivos...\n", num_archivos);
    
    for (int i = 0; i < num_archivos; i++) {
        printf("Copiando %d/%d: %s -> %s\n", 
               i + 1, num_archivos, archivos_origen[i], archivos_destino[i]);
        
        ResultadoCopia resultado_individual = copiar_archivo_configurado(
            archivos_origen[i], archivos_destino[i], config);
        
        if (resultado_individual.exito) {
            total_bytes += resultado_individual.bytes_copiados;
            archivos_exitosos++;
            printf("  ✓ Exitoso: %ld bytes\n", resultado_individual.bytes_copiados);
        } else {
            archivos_fallidos++;
            printf("  ✗ Error: %s\n", resultado_individual.mensaje);
        }
    }
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    resultado.exito = (archivos_exitosos > 0);
    resultado.bytes_copiados = total_bytes;
    resultado.tiempo_transcurrido = tiempo;
    resultado.metodo_usado = config->metodo;
    
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Copia en lote completada: %d exitosos, %d fallidos, %ld bytes totales",
            archivos_exitosos, archivos_fallidos, total_bytes);
    
    return resultado;
}

/* ================================
 * FUNCIONES DE VERIFICACIÓN
 * ================================ */

bool verificar_archivos_identicos(const char* archivo1, 
                                 const char* archivo2,
                                 TipoVerificacion tipo_verificacion) {
    if (!archivo1 || !archivo2) return false;
    
    switch (tipo_verificacion) {
        case VERIFICACION_NINGUNA:
            return true;
            
        case VERIFICACION_TAMAÑO: {
            long tamaño1 = obtener_tamaño_archivo(archivo1);
            long tamaño2 = obtener_tamaño_archivo(archivo2);
            return (tamaño1 >= 0 && tamaño2 >= 0 && tamaño1 == tamaño2);
        }
        
        case VERIFICACION_CONTENIDO: {
            FILE* f1 = fopen(archivo1, "rb");
            FILE* f2 = fopen(archivo2, "rb");
            
            if (!f1 || !f2) {
                if (f1) fclose(f1);
                if (f2) fclose(f2);
                return false;
            }
            
            char buffer1[CHUNK_SIZE], buffer2[CHUNK_SIZE];
            size_t bytes1, bytes2;
            bool identicos = true;
            
            do {
                bytes1 = fread(buffer1, 1, sizeof(buffer1), f1);
                bytes2 = fread(buffer2, 1, sizeof(buffer2), f2);
                
                if (bytes1 != bytes2 || memcmp(buffer1, buffer2, bytes1) != 0) {
                    identicos = false;
                    break;
                }
            } while (bytes1 > 0);
            
            fclose(f1);
            fclose(f2);
            return identicos;
        }
        
        case VERIFICACION_CHECKSUM: {
            unsigned long checksum1 = calcular_checksum_archivo(archivo1);
            unsigned long checksum2 = calcular_checksum_archivo(archivo2);
            return (checksum1 != 0 && checksum2 != 0 && checksum1 == checksum2);
        }
        
        default:
            return false;
    }
}

unsigned long calcular_checksum_archivo(const char* nombre_archivo) {
    if (!nombre_archivo) return 0;
    
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) return 0;
    
    unsigned long checksum = 0;
    int caracter;
    
    while ((caracter = fgetc(archivo)) != EOF) {
        checksum = checksum * 31 + caracter;  // Simple polynomial rolling hash
    }
    
    fclose(archivo);
    return checksum;
}

ResultadoCopia verificar_integridad_copia(const char* archivo_origen,
                                         const char* archivo_destino,
                                         EstadisticasCopia* estadisticas) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    if (!archivo_origen || !archivo_destino) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombres de archivo inválidos");
        return resultado;
    }
    
    // Verificar que ambos archivos existen
    if (!archivo_existe_y_legible(archivo_origen)) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Archivo origen no existe: %s", archivo_origen);
        return resultado;
    }
    
    if (!archivo_existe_y_legible(archivo_destino)) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Archivo destino no existe: %s", archivo_destino);
        return resultado;
    }
    
    clock_t inicio = clock();
    
    // Obtener tamaños
    long tamaño_origen = obtener_tamaño_archivo(archivo_origen);
    long tamaño_destino = obtener_tamaño_archivo(archivo_destino);
    
    if (estadisticas) {
        estadisticas->tamaño_origen = tamaño_origen;
        estadisticas->tamaño_destino = tamaño_destino;
        estadisticas->inicio = time(NULL);
    }
    
    // Verificar tamaños
    if (tamaño_origen != tamaño_destino) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Tamaños diferentes: origen %ld bytes, destino %ld bytes",
                tamaño_origen, tamaño_destino);
        if (estadisticas) {
            estadisticas->verificacion_exitosa = false;
        }
        return resultado;
    }
    
    // Verificar contenido
    bool contenido_identico = verificar_archivos_identicos(archivo_origen, 
                                                          archivo_destino,
                                                          VERIFICACION_CONTENIDO);
    
    if (!contenido_identico) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Los archivos tienen diferente contenido");
        if (estadisticas) {
            estadisticas->verificacion_exitosa = false;
        }
        return resultado;
    }
    
    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    if (estadisticas) {
        estadisticas->fin = time(NULL);
        estadisticas->verificacion_exitosa = true;
    }
    
    resultado.exito = true;
    resultado.bytes_copiados = tamaño_origen;
    resultado.tiempo_transcurrido = tiempo;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Verificación exitosa: archivos idénticos (%ld bytes)", tamaño_origen);
    
    return resultado;
}

/* ================================
 * FUNCIONES DE BACKUP Y RECUPERACIÓN
 * ================================ */

bool crear_backup_archivo(const char* archivo_destino) {
    if (!archivo_destino || !archivo_existe_y_legible(archivo_destino)) {
        return false;
    }
    
    char nombre_backup[MAX_NOMBRE_ARCHIVO];
    snprintf(nombre_backup, sizeof(nombre_backup), "%s.backup", archivo_destino);
    
    ResultadoCopia resultado = copiar_por_bloques(archivo_destino, nombre_backup, CHUNK_SIZE);
    return resultado.exito;
}

ResultadoCopia restaurar_desde_backup(const char* archivo_original) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_BLOQUE};
    
    if (!archivo_original) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Nombre de archivo inválido");
        return resultado;
    }
    
    char nombre_backup[MAX_NOMBRE_ARCHIVO];
    snprintf(nombre_backup, sizeof(nombre_backup), "%s.backup", archivo_original);
    
    if (!archivo_existe_y_legible(nombre_backup)) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Archivo de backup no existe: %s", nombre_backup);
        return resultado;
    }
    
    resultado = copiar_por_bloques(nombre_backup, archivo_original, CHUNK_SIZE);
    
    if (resultado.exito) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Archivo restaurado desde backup exitosamente");
    }
    
    return resultado;
}

/* ================================
 * FUNCIONES DE ANÁLISIS Y ESTADÍSTICAS
 * ================================ */

EstadisticasCopia obtener_estadisticas_copia(const char* archivo_origen,
                                            const char* archivo_destino,
                                            const ResultadoCopia* resultado) {
    EstadisticasCopia stats = {0};
    
    if (!archivo_origen || !archivo_destino || !resultado) {
        return stats;
    }
    
    stats.tamaño_origen = obtener_tamaño_archivo(archivo_origen);
    stats.tamaño_destino = obtener_tamaño_archivo(archivo_destino);
    stats.inicio = time(NULL) - (time_t)resultado->tiempo_transcurrido;
    stats.fin = time(NULL);
    
    if (resultado->tiempo_transcurrido > 0) {
        stats.velocidad_mb_s = calcular_velocidad_transferencia(
            resultado->bytes_copiados, resultado->tiempo_transcurrido);
    }
    
    // Contar líneas si es texto
    if (resultado->metodo_usado == METODO_LINEA_POR_LINEA) {
        FILE* archivo = fopen(archivo_destino, "r");
        if (archivo) {
            char linea[BUFFER_LINEA];
            while (fgets(linea, sizeof(linea), archivo) != NULL) {
                stats.lineas_copiadas++;
            }
            fclose(archivo);
        }
    }
    
    stats.verificacion_exitosa = verificar_archivos_identicos(
        archivo_origen, archivo_destino, VERIFICACION_TAMAÑO);
    
    return stats;
}

ResultadoCopia comparar_metodos_copia(const char* archivo_origen,
                                     const char* archivo_destino_base) {
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    if (!archivo_origen || !archivo_destino_base) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Parámetros inválidos");
        return resultado;
    }
    
    printf("=== Comparación de Métodos de Copia ===\n");
    printf("Archivo origen: %s\n", archivo_origen);
    
    long tamaño_archivo = obtener_tamaño_archivo(archivo_origen);
    if (tamaño_archivo < 0) {
        snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
                "Error al obtener tamaño del archivo origen");
        return resultado;
    }
    
    printf("Tamaño del archivo: ");
    char buffer_tamaño[64];
    formatear_tamaño(tamaño_archivo, buffer_tamaño, sizeof(buffer_tamaño));
    printf("%s\n\n", buffer_tamaño);
    
    // Probar cada método
    struct {
        MetodoCopia metodo;
        const char* nombre;
        size_t buffer_size;
    } metodos[] = {
        {METODO_LINEA_POR_LINEA, "Línea por línea", 0},
        {METODO_CARACTER_POR_CARACTER, "Carácter por carácter", 0},
        {METODO_BLOQUE, "Bloques (1KB)", 1024},
        {METODO_BLOQUE, "Bloques (4KB)", 4096},
        {METODO_BLOQUE, "Bloques (64KB)", 65536}
    };
    
    double tiempo_total = 0;
    long bytes_total = 0;
    
    for (size_t i = 0; i < sizeof(metodos) / sizeof(metodos[0]); i++) {
        char archivo_destino[MAX_NOMBRE_ARCHIVO];
        snprintf(archivo_destino, sizeof(archivo_destino), 
                "%s_%zu.tmp", archivo_destino_base, i);
        
        ResultadoCopia res;
        
        printf("%-25s: ", metodos[i].nombre);
        fflush(stdout);
        
        switch (metodos[i].metodo) {
            case METODO_LINEA_POR_LINEA:
                res = copiar_linea_por_linea(archivo_origen, archivo_destino);
                break;
            case METODO_CARACTER_POR_CARACTER:
                res = copiar_caracter_por_caracter(archivo_origen, archivo_destino);
                break;
            case METODO_BLOQUE:
                res = copiar_por_bloques(archivo_origen, archivo_destino, 
                                       metodos[i].buffer_size);
                break;
            default:
                continue;
        }
        
        if (res.exito) {
            double velocidad = calcular_velocidad_transferencia(
                res.bytes_copiados, res.tiempo_transcurrido);
            
            char tiempo_str[32];
            formatear_tiempo(res.tiempo_transcurrido, tiempo_str, sizeof(tiempo_str));
            
            printf("%s, %.2f MB/s\n", tiempo_str, velocidad);
            
            tiempo_total += res.tiempo_transcurrido;
            bytes_total += res.bytes_copiados;
        } else {
            printf("ERROR: %s\n", res.mensaje);
        }
        
        // Limpiar archivo temporal
        remove(archivo_destino);
    }
    
    printf("\n=== Resumen ===\n");
    printf("Tiempo total de pruebas: ");
    char tiempo_str[32];
    formatear_tiempo(tiempo_total, tiempo_str, sizeof(tiempo_str));
    printf("%s\n", tiempo_str);
    
    resultado.exito = true;
    resultado.bytes_copiados = bytes_total;
    resultado.tiempo_transcurrido = tiempo_total;
    snprintf(resultado.mensaje, sizeof(resultado.mensaje), 
            "Comparación de métodos completada");
    
    return resultado;
}

/* ================================
 * FUNCIONES INTERACTIVAS
 * ================================ */

ResultadoCopia copiar_archivo_interactivo(void) {
    char archivo_origen[MAX_NOMBRE_ARCHIVO];
    char archivo_destino[MAX_NOMBRE_ARCHIVO];
    
    printf("=== Copia de Archivos Interactiva ===\n");
    
    printf("Ingrese el nombre del archivo origen: ");
    if (fgets(archivo_origen, sizeof(archivo_origen), stdin) == NULL) {
        ResultadoCopia resultado = {false, "Error al leer entrada del usuario", 0, 0.0, METODO_LINEA_POR_LINEA};
        return resultado;
    }
    archivo_origen[strcspn(archivo_origen, "\n")] = '\0';
    
    printf("Ingrese el nombre del archivo destino: ");
    if (fgets(archivo_destino, sizeof(archivo_destino), stdin) == NULL) {
        ResultadoCopia resultado = {false, "Error al leer entrada del usuario", 0, 0.0, METODO_LINEA_POR_LINEA};
        return resultado;
    }
    archivo_destino[strcspn(archivo_destino, "\n")] = '\0';
    
    if (strlen(archivo_origen) == 0 || strlen(archivo_destino) == 0) {
        ResultadoCopia resultado = {false, "Nombres de archivo vacíos", 0, 0.0, METODO_LINEA_POR_LINEA};
        return resultado;
    }
    
    ConfiguracionCopia config = crear_configuracion_defecto();
    return copiar_archivo_configurado(archivo_origen, archivo_destino, &config);
}

ResultadoCopia menu_copia_archivo(const char* archivo_origen,
                                 const char* archivo_destino) {
    char origen_trabajo[MAX_NOMBRE_ARCHIVO];
    char destino_trabajo[MAX_NOMBRE_ARCHIVO];
    
    // Si no se proporcionan archivos, solicitarlos
    if (!archivo_origen) {
        printf("Ingrese el archivo origen: ");
        if (fgets(origen_trabajo, sizeof(origen_trabajo), stdin) == NULL) {
            ResultadoCopia resultado = {false, "Error al leer entrada", 0, 0.0, METODO_LINEA_POR_LINEA};
            return resultado;
        }
        origen_trabajo[strcspn(origen_trabajo, "\n")] = '\0';
    } else {
        strcpy(origen_trabajo, archivo_origen);
    }
    
    if (!archivo_destino) {
        printf("Ingrese el archivo destino: ");
        if (fgets(destino_trabajo, sizeof(destino_trabajo), stdin) == NULL) {
            ResultadoCopia resultado = {false, "Error al leer entrada", 0, 0.0, METODO_LINEA_POR_LINEA};
            return resultado;
        }
        destino_trabajo[strcspn(destino_trabajo, "\n")] = '\0';
    } else {
        strcpy(destino_trabajo, archivo_destino);
    }
    
    int opcion;
    ConfiguracionCopia config;
    ResultadoCopia resultado = {false, "", 0, 0.0, METODO_LINEA_POR_LINEA};
    
    do {
        printf("\n=== Menú de Copia de Archivos ===\n");
        printf("Origen: %s\n", origen_trabajo);
        printf("Destino: %s\n", destino_trabajo);
        printf("1. Copia básica (línea por línea)\n");
        printf("2. Copia carácter por carácter\n");
        printf("3. Copia por bloques\n");
        printf("4. Copia con progreso\n");
        printf("5. Copia con verificación\n");
        printf("6. Comparar métodos de copia\n");
        printf("7. Verificar archivos existentes\n");
        printf("8. Crear backup antes de copiar\n");
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
                resultado = copiar_linea_por_linea(origen_trabajo, destino_trabajo);
                break;
            case 2:
                resultado = copiar_caracter_por_caracter(origen_trabajo, destino_trabajo);
                break;
            case 3:
                resultado = copiar_por_bloques(origen_trabajo, destino_trabajo, CHUNK_SIZE);
                break;
            case 4:
                resultado = copiar_archivo_con_progreso(origen_trabajo, destino_trabajo, 
                                                       callback_progreso_defecto);
                break;
            case 5:
                config = crear_configuracion_defecto();
                config.verificacion = VERIFICACION_CONTENIDO;
                resultado = copiar_archivo_configurado(origen_trabajo, destino_trabajo, &config);
                break;
            case 6:
                resultado = comparar_metodos_copia(origen_trabajo, "test_comparacion");
                break;
            case 7: {
                EstadisticasCopia stats;
                resultado = verificar_integridad_copia(origen_trabajo, destino_trabajo, &stats);
                if (resultado.exito) {
                    printf("Archivos verificados como idénticos.\n");
                    printf("Tamaño: %ld bytes\n", stats.tamaño_origen);
                }
                break;
            }
            case 8:
                config = crear_configuracion_defecto();
                config.crear_backup = true;
                resultado = copiar_archivo_configurado(origen_trabajo, destino_trabajo, &config);
                break;
            case 0:
                printf("Saliendo del menú.\n");
                resultado.exito = true;
                strcpy(resultado.mensaje, "Menú finalizado");
                break;
            default:
                printf("Opción inválida.\n");
                break;
        }
        
        if (opcion != 0) {
            if (resultado.exito) {
                printf("\n✓ %s\n", resultado.mensaje);
                printf("  Bytes copiados: %ld\n", resultado.bytes_copiados);
                printf("  Tiempo: %.3f segundos\n", resultado.tiempo_transcurrido);
                if (resultado.tiempo_transcurrido > 0) {
                    double velocidad = calcular_velocidad_transferencia(
                        resultado.bytes_copiados, resultado.tiempo_transcurrido);
                    printf("  Velocidad: %.2f MB/s\n", velocidad);
                }
            } else {
                printf("\n✗ Error: %s\n", resultado.mensaje);
            }
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

bool directorio_escribible(const char* ruta_directorio) {
    if (!ruta_directorio) return false;
    
    return access(ruta_directorio, W_OK) == 0;
}

long obtener_tamaño_archivo(const char* nombre_archivo) {
    if (!nombre_archivo) return -1;
    
    struct stat info;
    if (stat(nombre_archivo, &info) == 0) {
        return info.st_size;
    }
    return -1;
}

double calcular_velocidad_transferencia(long bytes_transferidos, 
                                       double tiempo_segundos) {
    if (tiempo_segundos <= 0) return 0.0;
    
    double mb_transferidos = bytes_transferidos / (1024.0 * 1024.0);
    return mb_transferidos / tiempo_segundos;
}

void formatear_tiempo(double segundos, char* buffer, size_t tamaño_buffer) {
    if (!buffer || tamaño_buffer == 0) return;
    
    if (segundos < 1.0) {
        snprintf(buffer, tamaño_buffer, "%.0f ms", segundos * 1000);
    } else if (segundos < 60.0) {
        snprintf(buffer, tamaño_buffer, "%.3f s", segundos);
    } else {
        int minutos = (int)(segundos / 60);
        double seg_restantes = segundos - (minutos * 60);
        snprintf(buffer, tamaño_buffer, "%d min %.1f s", minutos, seg_restantes);
    }
}

void formatear_tamaño(long bytes, char* buffer, size_t tamaño_buffer) {
    if (!buffer || tamaño_buffer == 0) return;
    
    const char* unidades[] = {"B", "KB", "MB", "GB", "TB"};
    double tamaño = bytes;
    int unidad = 0;
    
    while (tamaño >= 1024.0 && unidad < 4) {
        tamaño /= 1024.0;
        unidad++;
    }
    
    if (unidad == 0) {
        snprintf(buffer, tamaño_buffer, "%ld %s", bytes, unidades[unidad]);
    } else {
        snprintf(buffer, tamaño_buffer, "%.2f %s", tamaño, unidades[unidad]);
    }
}

void imprimir_error_archivo(const char* operacion, const char* nombre_archivo) {
    if (operacion && nombre_archivo) {
        fprintf(stderr, "Error al %s '%s': %s\n", 
               operacion, nombre_archivo, strerror(errno));
    }
}

ConfiguracionCopia crear_configuracion_defecto(void) {
    ConfiguracionCopia config = {
        .metodo = METODO_LINEA_POR_LINEA,
        .verificacion = VERIFICACION_TAMAÑO,
        .crear_backup = false,
        .mostrar_progreso = false,
        .preservar_permisos = false,
        .tamaño_buffer = CHUNK_SIZE
    };
    return config;
}

void callback_progreso_defecto(const ProgresoCopia* progreso) {
    if (!progreso) return;
    
    printf("\rProgreso: %.1f%% (%ld/%ld bytes) - %.2f MB/s", 
           progreso->porcentaje,
           progreso->bytes_procesados,
           progreso->bytes_totales,
           progreso->velocidad_actual);
    
    if (progreso->porcentaje >= 100.0) {
        printf("\n");
    }
    
    fflush(stdout);
}

/* ================================
 * FUNCIÓN PRINCIPAL BÁSICA
 * ================================ */

int main_basico(void) {
    FILE *origen = fopen("origen.txt", "r");
    FILE *destino = fopen("copia.txt", "w");
    char buffer[256];

    if (origen == NULL || destino == NULL) {
        perror("Error al abrir archivos");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), origen) != NULL) {
        if (fputs(buffer, destino) == EOF) {
            perror("Error al escribir en el archivo destino");
            fclose(origen);
            fclose(destino);
            return 1;
        }
    }

    fclose(origen);
    fclose(destino);

    printf("Archivo copiado correctamente.\n");
    return 0;
}

/* ================================
 * FUNCIÓN PRINCIPAL
 * ================================ */

int main(void) {
    printf("=== Ejercicio 075: Copiar Archivo de Texto ===\n\n");
    
    // Crear archivo de origen de ejemplo si no existe
    if (!archivo_existe_y_legible("origen.txt")) {
        FILE* archivo_ejemplo = fopen("origen.txt", "w");
        if (archivo_ejemplo) {
            fprintf(archivo_ejemplo, "Esta es la primera línea del archivo origen.\n");
            fprintf(archivo_ejemplo, "Segunda línea con algunos datos de ejemplo.\n");
            fprintf(archivo_ejemplo, "Tercera línea con números: 12345 y símbolos: !@#$%%.\n");
            fprintf(archivo_ejemplo, "Cuarta línea para probar la copia completa.\n");
            fprintf(archivo_ejemplo, "Quinta y última línea del archivo de prueba.\n");
            fclose(archivo_ejemplo);
            printf("Archivo de ejemplo 'origen.txt' creado.\n\n");
        }
    }
    
    // Ejecutar función básica del enunciado
    printf("1. Ejecutando función básica del enunciado:\n");
    printf("==========================================\n");
    int resultado_basico = main_basico();
    
    if (resultado_basico == 0) {
        printf("✓ Copia básica completada exitosamente.\n");
        
        // Verificar la copia
        if (archivo_existe_y_legible("copia.txt")) {
            long tamaño_origen = obtener_tamaño_archivo("origen.txt");
            long tamaño_copia = obtener_tamaño_archivo("copia.txt");
            printf("✓ Verificación: origen %ld bytes, copia %ld bytes\n", 
                   tamaño_origen, tamaño_copia);
        }
    } else {
        printf("✗ Error en la copia básica.\n");
    }
    
    // Demostración de funciones avanzadas
    printf("\n\n2. Demostraciones avanzadas:\n");
    printf("============================\n");
    
    // Copia con diferentes métodos
    printf("\n--- Comparación de métodos de copia ---\n");
    comparar_metodos_copia("origen.txt", "test_metodos");
    
    // Copia con verificación
    printf("\n--- Copia con verificación ---\n");
    ConfiguracionCopia config = crear_configuracion_defecto();
    config.verificacion = VERIFICACION_CONTENIDO;
    config.crear_backup = true;
    
    ResultadoCopia resultado = copiar_archivo_configurado("origen.txt", "copia_verificada.txt", &config);
    if (resultado.exito) {
        printf("✓ %s\n", resultado.mensaje);
        
        char tiempo_str[32], tamaño_str[32];
        formatear_tiempo(resultado.tiempo_transcurrido, tiempo_str, sizeof(tiempo_str));
        formatear_tamaño(resultado.bytes_copiados, tamaño_str, sizeof(tamaño_str));
        
        printf("  Tiempo: %s\n", tiempo_str);
        printf("  Tamaño: %s\n", tamaño_str);
        
        if (resultado.tiempo_transcurrido > 0) {
            double velocidad = calcular_velocidad_transferencia(
                resultado.bytes_copiados, resultado.tiempo_transcurrido);
            printf("  Velocidad: %.2f MB/s\n", velocidad);
        }
    } else {
        printf("✗ Error: %s\n", resultado.mensaje);
    }
    
    // Copia con progreso
    printf("\n--- Copia con progreso ---\n");
    copiar_archivo_con_progreso("origen.txt", "copia_progreso.txt", callback_progreso_defecto);
    
    // Copia en lote
    printf("\n--- Copia en lote ---\n");
    const char* archivos_origen[] = {"origen.txt", "origen.txt", "origen.txt"};
    const char* archivos_destino[] = {"lote1.txt", "lote2.txt", "lote3.txt"};
    
    ResultadoCopia resultado_lote = copiar_archivos_lote(archivos_origen, archivos_destino, 3, &config);
    if (resultado_lote.exito) {
        printf("✓ %s\n", resultado_lote.mensaje);
    }
    
    // Verificación de integridad
    printf("\n--- Verificación de integridad ---\n");
    EstadisticasCopia stats;
    ResultadoCopia verificacion = verificar_integridad_copia("origen.txt", "copia.txt", &stats);
    if (verificacion.exito) {
        printf("✓ %s\n", verificacion.mensaje);
        printf("  Estadísticas:\n");
        printf("    Origen: %ld bytes\n", stats.tamaño_origen);
        printf("    Destino: %ld bytes\n", stats.tamaño_destino);
        printf("    Verificación: %s\n", stats.verificacion_exitosa ? "Exitosa" : "Fallida");
    }
    
    printf("\n=== Programa completado exitosamente ===\n");
    
    // Limpiar archivos temporales
    remove("copia_verificada.txt");
    remove("copia_progreso.txt");
    remove("lote1.txt");
    remove("lote2.txt");
    remove("lote3.txt");
    
    return 0;
}
