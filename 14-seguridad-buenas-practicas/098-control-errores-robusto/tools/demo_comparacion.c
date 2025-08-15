#include "control_errores_robusto.h"
#include <unistd.h>
#include <sys/stat.h>

void demostrar_diferencias(void) {
    printf("=== DEMOSTRACIÓN: Código Inseguro vs Seguro ===\n\n");
    
    printf("CÓDIGO INSEGURO (❌ NO hacer esto):\n");
    printf("------------------------------------\n");
    printf("FILE *archivo = fopen(\"datos.txt\", \"w\");\n");
    printf("char *buffer = malloc(100);\n");
    printf("int numero;\n");
    printf("fprintf(archivo, \"Escribiendo datos...\\n\");\n");
    printf("scanf(\"%%d\", &numero);\n");
    printf("sprintf(buffer, \"Número: %%d\", numero);\n");
    printf("fprintf(archivo, \"%%s\\n\", buffer);\n");
    printf("fclose(archivo);\n");
    printf("free(buffer);\n");
    printf("return 0;\n\n");
    
    printf("PROBLEMAS:\n");
    printf("• fopen() puede fallar → crash en fprintf()\n");
    printf("• malloc() puede fallar → crash en sprintf()\n");
    printf("• scanf() puede fallar → valor indefinido\n");
    printf("• sprintf() puede causar buffer overflow\n");
    printf("• fprintf() puede fallar silenciosamente\n");
    printf("• No hay limpieza si algo falla\n\n");
    
    printf("CÓDIGO SEGURO (✅ Así SÍ):\n");
    printf("-------------------------\n");
    printf("result_t resultado;\n");
    printf("FILE *archivo = NULL;\n");
    printf("char *buffer = NULL;\n\n");
    
    printf("// Verificar apertura de archivo\n");
    printf("resultado = abrir_archivo_seguro(\"datos.txt\", \"w\", &archivo);\n");
    printf("if (!resultado.success) {\n");
    printf("    imprimir_error(&resultado);\n");
    printf("    return resultado;\n");
    printf("}\n\n");
    
    printf("// Verificar asignación de memoria\n");
    printf("resultado = asignar_memoria_seguro(100, (void**)&buffer);\n");
    printf("if (!resultado.success) {\n");
    printf("    liberar_recursos_seguro(archivo, NULL);\n");
    printf("    return resultado;\n");
    printf("}\n\n");
    
    printf("// Verificar entrada del usuario\n");
    printf("resultado = leer_entero_seguro(&numero);\n");
    printf("if (!resultado.success) {\n");
    printf("    liberar_recursos_seguro(archivo, buffer);\n");
    printf("    return resultado;\n");
    printf("}\n\n");
    
    printf("// Formateo seguro\n");
    printf("int ret = snprintf(buffer, 100, \"Número: %%d\", numero);\n");
    printf("if (ret < 0 || ret >= 100) {\n");
    printf("    liberar_recursos_seguro(archivo, buffer);\n");
    printf("    return result_error(ERROR_INVALID_INPUT, \"Error formato\");\n");
    printf("}\n\n");
    
    printf("// Verificar escritura\n");
    printf("resultado = escribir_datos_seguro(archivo, buffer, strlen(buffer));\n");
    printf("if (!resultado.success) {\n");
    printf("    liberar_recursos_seguro(archivo, buffer);\n");
    printf("    return resultado;\n");
    printf("}\n\n");
    
    printf("liberar_recursos_seguro(archivo, buffer);\n");
    printf("return result_ok();\n\n");
    
    printf("BENEFICIOS:\n");
    printf("• Detecta errores inmediatamente\n");
    printf("• Proporciona mensajes descriptivos\n");
    printf("• Libera recursos en todos los casos\n");
    printf("• Previene crashes y corrupción\n");
    printf("• Facilita debugging y mantenimiento\n");
}

void ejecutar_comparacion_practica(void) {
    printf("\n=== COMPARACIÓN PRÁCTICA ===\n");
    
    printf("\n1. Probando con condiciones normales:\n");
    printf("------------------------------------\n");
    
    // Crear un archivo temporal para la prueba
    FILE *temp = fopen("temp_input.txt", "w");
    if (temp) {
        fprintf(temp, "42\n");
        fclose(temp);
    }
    
    printf("Función INSEGURA: ");
    int resultado_inseguro = funcion_insegura();
    printf("Código retornado: %d (⚠️  siempre 0, sin verificación)\n", resultado_inseguro);
    
    printf("Función SEGURA: ");
    result_t resultado_seguro = funcion_segura();
    if (resultado_seguro.success) {
        printf("✅ Éxito - %s\n", resultado_seguro.error_message);
    } else {
        printf("❌ Error detectado:\n");
        imprimir_error(&resultado_seguro);
    }
    
    // Limpiar archivos de prueba
    unlink("datos.txt");
    unlink("temp_input.txt");
}

void simular_condiciones_error(void) {
    printf("\n=== SIMULACIÓN DE ERRORES ===\n");
    
    printf("\n1. Error de apertura de archivo (directorio protegido):\n");
    printf("-----------------------------------------------------\n");
    
    FILE *archivo = NULL;
    result_t resultado = abrir_archivo_seguro("/root/archivo_protegido.txt", "w", &archivo);
    if (!resultado.success) {
        printf("✅ Error detectado correctamente:\n");
        imprimir_error(&resultado);
    } else {
        printf("⚠️  Archivo abierto (permisos inesperados)\n");
        liberar_recursos_seguro(archivo, NULL);
    }
    
    printf("\n2. Error de validación de parámetros:\n");
    printf("------------------------------------\n");
    
    resultado = abrir_archivo_seguro(NULL, "w", &archivo);
    if (!resultado.success) {
        printf("✅ Error de parámetros detectado:\n");
        imprimir_error(&resultado);
    }
    
    printf("\n3. Error de asignación de memoria (parámetros inválidos):\n");
    printf("--------------------------------------------------------\n");
    
    void *ptr = NULL;
    resultado = asignar_memoria_seguro(0, &ptr);
    if (!resultado.success) {
        printf("✅ Error de parámetros detectado:\n");
        imprimir_error(&resultado);
    }
    
    printf("\n4. Liberación segura con parámetros nulos:\n");
    printf("-----------------------------------------\n");
    
    printf("Llamando liberar_recursos_seguro(NULL, NULL)...\n");
    liberar_recursos_seguro(NULL, NULL);
    printf("✅ Función manejó parámetros nulos sin fallar\n");
}

void mostrar_estadisticas_codigo(void) {
    printf("\n=== ESTADÍSTICAS DE CÓDIGO ===\n");
    printf("------------------------------\n");
    
    printf("Función INSEGURA:\n");
    printf("• Líneas de código: ~10\n");
    printf("• Verificaciones de error: 0\n");
    printf("• Puntos de fallo: 5+\n");
    printf("• Liberación de recursos: Básica\n");
    printf("• Información de error: Ninguna\n\n");
    
    printf("Función SEGURA:\n");
    printf("• Líneas de código: ~50\n");
    printf("• Verificaciones de error: 100%%\n");
    printf("• Puntos de fallo: 0\n");
    printf("• Liberación de recursos: Completa\n");
    printf("• Información de error: Detallada\n\n");
    
    printf("INVERSIÓN vs BENEFICIO:\n");
    printf("• +400%% código → +∞%% confiabilidad\n");
    printf("• +5 minutos desarrollo → -5 horas debugging\n");
    printf("• +verificaciones → -crashes en producción\n");
    printf("• +mantenibilidad → -costos a largo plazo\n");
}

int main(void) {
    printf("Demo de Comparación: Control de Errores Robusto\n");
    printf("===============================================\n");
    
    demostrar_diferencias();
    
    printf("\n¿Desea ver la comparación práctica? (s/n): ");
    char respuesta;
    if (scanf(" %c", &respuesta) == 1 && (respuesta == 's' || respuesta == 'S')) {
        ejecutar_comparacion_practica();
    }
    
    printf("\n¿Desea ver simulación de errores? (s/n): ");
    if (scanf(" %c", &respuesta) == 1 && (respuesta == 's' || respuesta == 'S')) {
        simular_condiciones_error();
    }
    
    printf("\n¿Desea ver estadísticas de código? (s/n): ");
    if (scanf(" %c", &respuesta) == 1 && (respuesta == 's' || respuesta == 'S')) {
        mostrar_estadisticas_codigo();
    }
    
    printf("\n=== CONCLUSIÓN ===\n");
    printf("El control robusto de errores:\n");
    printf("✅ Previene crashes y comportamiento impredecible\n");
    printf("✅ Facilita debugging y mantenimiento\n");
    printf("✅ Mejora la experiencia del usuario\n");
    printf("✅ Reduce costos de desarrollo a largo plazo\n");
    printf("✅ Es ESENCIAL en software de producción\n\n");
    
    printf("💡 Recuerda: 'El tiempo invertido en verificación de errores\n");
    printf("   siempre se recupera en reducción de bugs y problemas.'\n");
    
    return 0;
}
