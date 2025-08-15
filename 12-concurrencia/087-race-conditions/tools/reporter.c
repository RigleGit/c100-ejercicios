/**
 * @file reporter.c
 * @brief Generador de reportes de race conditions
 */

#include "../include/race_conditions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("=== GENERADOR DE REPORTES DE RACE CONDITIONS ===\n\n");
    
    // Configuración del reporte
    configuracion_experimento_t config = {
        .num_hilos = 4,
        .incrementos_por_hilo = 50000,
        .usar_delay = false,
        .delay_microsegundos = 0,
        .mostrar_detalles = false
    };
    
    int num_experimentos = 50;
    
    if (argc > 1) {
        num_experimentos = atoi(argv[1]);
        if (num_experimentos < 1 || num_experimentos > 1000) {
            printf("Número de experimentos debe estar entre 1 y 1000\n");
            return EXIT_FAILURE;
        }
    }
    
    printf("Configuración del reporte:\n");
    printf("- Hilos: %d\n", config.num_hilos);
    printf("- Incrementos por hilo: %d\n", config.incrementos_por_hilo);
    printf("- Experimentos: %d\n", num_experimentos);
    printf("- Valor esperado por experimento: %d\n\n", 
           config.num_hilos * config.incrementos_por_hilo);
    
    // Generar timestamp para el archivo
    time_t ahora = time(NULL);
    struct tm* tm_info = localtime(&ahora);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", tm_info);
    
    char nombre_archivo[256];
    snprintf(nombre_archivo, sizeof(nombre_archivo), 
             "race_conditions_report_%s.txt", timestamp);
    
    printf("Generando reporte: %s\n", nombre_archivo);
    printf("Esto puede tomar algunos minutos...\n\n");
    
    // Ejecutar análisis estadístico
    analisis_estadistico_t analisis;
    if (!ejecutar_analisis_estadistico(&config, num_experimentos, &analisis)) {
        printf("Error ejecutando análisis estadístico\n");
        return EXIT_FAILURE;
    }
    
    // Crear archivo de reporte
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        printf("Error creando archivo de reporte\n");
        limpiar_analisis_estadistico(&analisis);
        return EXIT_FAILURE;
    }
    
    // Escribir reporte
    fprintf(archivo, "REPORTE DE ANÁLISIS DE RACE CONDITIONS\n");
    fprintf(archivo, "=====================================\n\n");
    
    fprintf(archivo, "Fecha de generación: %s\n", ctime(&ahora));
    fprintf(archivo, "Configuración:\n");
    fprintf(archivo, "  Hilos: %d\n", config.num_hilos);
    fprintf(archivo, "  Incrementos por hilo: %d\n", config.incrementos_por_hilo);
    fprintf(archivo, "  Experimentos realizados: %d\n", num_experimentos);
    fprintf(archivo, "  Valor esperado: %d\n\n", analisis.valor_esperado);
    
    fprintf(archivo, "RESULTADOS ESTADÍSTICOS:\n");
    fprintf(archivo, "========================\n");
    fprintf(archivo, "Valores obtenidos:\n");
    fprintf(archivo, "  Mínimo: %d\n", analisis.valor_min);
    fprintf(archivo, "  Máximo: %d\n", analisis.valor_max);
    fprintf(archivo, "  Promedio: %.2f\n", analisis.valor_promedio);
    fprintf(archivo, "  Desviación estándar: %.2f\n\n", analisis.desviacion_estandar);
    
    fprintf(archivo, "Diferencias (incrementos perdidos):\n");
    fprintf(archivo, "  Mínima: %d\n", analisis.diferencia_min);
    fprintf(archivo, "  Máxima: %d\n", analisis.diferencia_max);
    fprintf(archivo, "  Promedio: %.2f\n\n", analisis.diferencia_promedio);
    
    fprintf(archivo, "Errores:\n");
    fprintf(archivo, "  Error mínimo: %.2f%%\n", analisis.error_min);
    fprintf(archivo, "  Error máximo: %.2f%%\n", analisis.error_max);
    fprintf(archivo, "  Error promedio: %.2f%%\n\n", analisis.error_promedio);
    
    fprintf(archivo, "Consistencia:\n");
    fprintf(archivo, "  Experimentos correctos: %d/%d\n", 
            analisis.experimentos_correctos, analisis.num_experimentos);
    fprintf(archivo, "  Tasa de inconsistencia: %.2f%%\n\n", analisis.tasa_inconsistencia);
    
    fprintf(archivo, "ANÁLISIS DETALLADO:\n");
    fprintf(archivo, "==================\n");
    for (int i = 0; i < analisis.num_experimentos && i < 20; i++) {
        fprintf(archivo, "Experimento %02d: esperado=%d, obtenido=%d, diferencia=%d (%.2f%%)\n",
                i + 1,
                analisis.experimentos[i].valor_esperado,
                analisis.experimentos[i].valor_obtenido,
                analisis.experimentos[i].diferencia,
                analisis.experimentos[i].porcentaje_error);
    }
    
    if (analisis.num_experimentos > 20) {
        fprintf(archivo, "... (mostrando primeros 20 de %d experimentos)\n", 
                analisis.num_experimentos);
    }
    
    fprintf(archivo, "\nRECOMENDALONES:\n");
    fprintf(archivo, "===============\n");
    
    if (analisis.tasa_inconsistencia == 0.0) {
        fprintf(archivo, "• No se detectaron race conditions en esta configuración\n");
        fprintf(archivo, "• Considere aumentar la carga o número de hilos\n");
        fprintf(archivo, "• Ejecute más experimentos para mayor confianza\n");
    } else if (analisis.tasa_inconsistencia < 10.0) {
        fprintf(archivo, "• Race conditions raras pero presentes\n");
        fprintf(archivo, "• Use sincronización para garantizar correctitud\n");
        fprintf(archivo, "• Considere testing más exhaustivo\n");
    } else if (analisis.tasa_inconsistencia < 50.0) {
        fprintf(archivo, "• Race conditions notables\n");
        fprintf(archivo, "• Sincronización es CRÍTICA\n");
        fprintf(archivo, "• Use herramientas de detección como ThreadSanitizer\n");
    } else {
        fprintf(archivo, "• Race conditions muy frecuentes\n");
        fprintf(archivo, "• Sistema altamente inconsistente\n");
        fprintf(archivo, "• Sincronización OBLIGATORIA\n");
        fprintf(archivo, "• Considere rediseño para minimizar estado compartido\n");
    }
    
    fprintf(archivo, "\nSINCRONIZACIÓN RECOMENDADA:\n");
    fprintf(archivo, "===========================\n");
    fprintf(archivo, "pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;\n");
    fprintf(archivo, "\nvoid* hilo_seguro(void* arg) {\n");
    fprintf(archivo, "    for (int i = 0; i < incrementos; i++) {\n");
    fprintf(archivo, "        pthread_mutex_lock(&mutex);\n");
    fprintf(archivo, "        contador++;\n");
    fprintf(archivo, "        pthread_mutex_unlock(&mutex);\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    return NULL;\n");
    fprintf(archivo, "}\n\n");
    
    fprintf(archivo, "HERRAMIENTAS DE DETECCIÓN:\n");
    fprintf(archivo, "=========================\n");
    fprintf(archivo, "• ThreadSanitizer: gcc -fsanitize=thread\n");
    fprintf(archivo, "• Helgrind: valgrind --tool=helgrind\n");
    fprintf(archivo, "• Intel Inspector (comercial)\n");
    fprintf(archivo, "• Testing exhaustivo con diferentes cargas\n");
    
    fclose(archivo);
    
    // Mostrar resumen en pantalla
    printf("Reporte generado exitosamente: %s\n\n");
    printf("RESUMEN:\n");
    printf("Tasa de inconsistencia: %.1f%%\n", analisis.tasa_inconsistencia);
    printf("Error promedio: %.2f%%\n", analisis.error_promedio);
    printf("Incrementos perdidos (promedio): %.0f\n", analisis.diferencia_promedio);
    
    if (analisis.tasa_inconsistencia > 50.0) {
        printf("\n⚠️  ADVERTENCIA: Alta tasa de race conditions detectada\n");
        printf("   Sincronización es CRÍTICA para este código\n");
    } else if (analisis.tasa_inconsistencia > 0.0) {
        printf("\n⚠️  Race conditions detectadas - use sincronización\n");
    } else {
        printf("\n✅ No se detectaron race conditions en esta configuración\n");
    }
    
    limpiar_analisis_estadistico(&analisis);
    
    return EXIT_SUCCESS;
}
