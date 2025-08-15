#include "control_errores_robusto.h"
#include <limits.h>

void mostrar_menu(void) {
    printf("\n=== Demostración: Control de Errores Robusto ===\n");
    printf("1. Ejecutar función INSEGURA (sin verificación de errores)\n");
    printf("2. Ejecutar función SEGURA (con verificación completa)\n");
    printf("3. Demostrar manejo de archivos seguro\n");
    printf("4. Demostrar manejo de memoria seguro\n");
    printf("5. Demostrar lectura de entrada segura\n");
    printf("6. Mostrar información sobre buenas prácticas\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

void demostrar_inseguro(void) {
    printf("\n--- Función INSEGURA ---\n");
    printf("ADVERTENCIA: Esta función NO verifica errores.\n");
    printf("Problemas potenciales:\n");
    printf("- fopen() puede fallar (archivo protegido, disco lleno, etc.)\n");
    printf("- malloc() puede fallar (memoria insuficiente)\n");
    printf("- scanf() puede fallar (entrada inválida)\n");
    printf("- fprintf() puede fallar (error de escritura)\n\n");
    
    printf("¿Desea ejecutar la función insegura? (s/n): ");
    char respuesta;
    if (scanf(" %c", &respuesta) == 1 && (respuesta == 's' || respuesta == 'S')) {
        printf("\nEjecutando función insegura...\n");
        printf("Ingrese un número: ");
        int resultado = funcion_insegura();
        printf("Función terminó con código: %d\n", resultado);
        printf("NOTA: El código 0 no garantiza que todo funcionó correctamente.\n");
    }
}

void demostrar_seguro(void) {
    printf("\n--- Función SEGURA ---\n");
    printf("Esta función verifica todos los posibles errores:\n");
    printf("- Verificación de fopen()\n");
    printf("- Verificación de malloc()\n");
    printf("- Validación de entrada del usuario\n");
    printf("- Verificación de operaciones de E/S\n");
    printf("- Liberación segura de recursos\n\n");
    
    printf("Ejecutando función segura...\n");
    result_t resultado = funcion_segura();
    
    if (resultado.success) {
        printf("✓ Operación completada exitosamente\n");
    } else {
        printf("✗ Error detectado:\n");
        imprimir_error(&resultado);
    }
}

void demostrar_archivos_seguro(void) {
    printf("\n--- Manejo Seguro de Archivos ---\n");
    
    FILE *archivo = NULL;
    result_t resultado;
    
    // Intentar abrir archivo que no existe
    printf("1. Intentando abrir archivo inexistente...\n");
    resultado = abrir_archivo_seguro("archivo_inexistente.txt", "r", &archivo);
    if (!resultado.success) {
        printf("✓ Error detectado correctamente:\n");
        imprimir_error(&resultado);
    }
    
    // Crear archivo de prueba
    printf("\n2. Creando archivo de prueba...\n");
    resultado = abrir_archivo_seguro("prueba_segura.txt", "w", &archivo);
    if (resultado.success) {
        printf("✓ Archivo creado exitosamente\n");
        
        // Escribir datos de forma segura
        const char *datos = "Datos de prueba para control de errores\n";
        resultado = escribir_datos_seguro(archivo, datos, strlen(datos));
        if (resultado.success) {
            printf("✓ Datos escritos exitosamente\n");
        } else {
            imprimir_error(&resultado);
        }
        
        liberar_recursos_seguro(archivo, NULL);
    } else {
        imprimir_error(&resultado);
    }
    
    // Leer archivo
    printf("\n3. Leyendo archivo de forma segura...\n");
    resultado = abrir_archivo_seguro("prueba_segura.txt", "r", &archivo);
    if (resultado.success) {
        char buffer[256];
        resultado = leer_linea_seguro(archivo, buffer, sizeof(buffer));
        if (resultado.success) {
            printf("✓ Línea leída: %s", buffer);
        } else {
            imprimir_error(&resultado);
        }
        liberar_recursos_seguro(archivo, NULL);
    }
}

void demostrar_memoria_seguro(void) {
    printf("\n--- Manejo Seguro de Memoria ---\n");
    
    void *ptr = NULL;
    result_t resultado;
    
    // Asignación normal
    printf("1. Asignando memoria (1KB)...\n");
    resultado = asignar_memoria_seguro(1024, &ptr);
    if (resultado.success) {
        printf("✓ Memoria asignada exitosamente en %p\n", ptr);
        liberar_recursos_seguro(NULL, ptr);
        printf("✓ Memoria liberada\n");
    } else {
        imprimir_error(&resultado);
    }
    
    // Asignación con tamaño 0 (error)
    printf("\n2. Intentando asignar memoria con tamaño 0...\n");
    resultado = asignar_memoria_seguro(0, &ptr);
    if (!resultado.success) {
        printf("✓ Error detectado correctamente:\n");
        imprimir_error(&resultado);
    }
    
    // Puntero nulo (error)
    printf("\n3. Intentando asignar con puntero nulo...\n");
    resultado = asignar_memoria_seguro(100, NULL);
    if (!resultado.success) {
        printf("✓ Error detectado correctamente:\n");
        imprimir_error(&resultado);
    }
}

void demostrar_entrada_segura(void) {
    printf("\n--- Lectura Segura de Entrada ---\n");
    
    int valor;
    result_t resultado;
    
    printf("1. Ingrese un número entero: ");
    resultado = leer_entero_seguro(&valor);
    if (resultado.success) {
        printf("✓ Número leído exitosamente: %d\n", valor);
    } else {
        printf("✗ Error en la entrada:\n");
        imprimir_error(&resultado);
    }
    
    printf("\n2. Ahora ingrese texto (no numérico) para ver el error: ");
    resultado = leer_entero_seguro(&valor);
    if (resultado.success) {
        printf("✓ Número leído: %d\n", valor);
    } else {
        printf("✓ Error detectado correctamente:\n");
        imprimir_error(&resultado);
    }
}

void mostrar_buenas_practicas(void) {
    printf("\n=== Buenas Prácticas en Control de Errores ===\n\n");
    
    printf("1. SIEMPRE verificar valores de retorno:\n");
    printf("   ✓ fopen(), malloc(), scanf(), etc.\n");
    printf("   ✗ Nunca asumir que funcionarán\n\n");
    
    printf("2. Usar códigos de error consistentes:\n");
    printf("   ✓ Definir enums para códigos de error\n");
    printf("   ✓ Proporcionar mensajes descriptivos\n\n");
    
    printf("3. Liberación de recursos:\n");
    printf("   ✓ Liberar en todos los caminos de ejecución\n");
    printf("   ✓ Usar goto para cleanup en C\n");
    printf("   ✓ Verificar antes de liberar\n\n");
    
    printf("4. Validación de parámetros:\n");
    printf("   ✓ Verificar punteros nulos\n");
    printf("   ✓ Validar rangos de valores\n");
    printf("   ✓ Documentar precondiciones\n\n");
    
    printf("5. Manejo de errores de sistema:\n");
    printf("   ✓ Usar errno para errores del sistema\n");
    printf("   ✓ Proporcionar contexto en mensajes\n");
    printf("   ✓ Logs para debugging\n\n");
    
    printf("6. Principio de fail-fast:\n");
    printf("   ✓ Detectar errores lo antes posible\n");
    printf("   ✓ No propagar estados inválidos\n");
    printf("   ✓ Fallar de manera controlada\n\n");
    
    printf("ANTES (inseguro):\n");
    printf("FILE *f = fopen(\"file.txt\", \"r\");\n");
    printf("char *buf = malloc(100);\n");
    printf("fprintf(f, \"data\");\n");
    printf("fclose(f);\n");
    printf("free(buf);\n\n");
    
    printf("DESPUÉS (seguro):\n");
    printf("FILE *f = fopen(\"file.txt\", \"r\");\n");
    printf("if (!f) { perror(\"fopen\"); return -1; }\n");
    printf("char *buf = malloc(100);\n");
    printf("if (!buf) { fclose(f); return -1; }\n");
    printf("if (fprintf(f, \"data\") < 0) {\n");
    printf("    fclose(f); free(buf); return -1;\n");
    printf("}\n");
    printf("fclose(f);\n");
    printf("free(buf);\n");
}

#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    result_t resultado;
    
    printf("Control de Errores Robusto - Comparación de enfoques\n");
    printf("=====================================================\n");
    
    do {
        mostrar_menu();
        resultado = leer_entero_seguro(&opcion);
        
        if (!resultado.success) {
            printf("Error en la entrada. Intente nuevamente.\n");
            continue;
        }
        
        switch (opcion) {
            case 1:
                demostrar_inseguro();
                break;
            case 2:
                demostrar_seguro();
                break;
            case 3:
                demostrar_archivos_seguro();
                break;
            case 4:
                demostrar_memoria_seguro();
                break;
            case 5:
                demostrar_entrada_segura();
                break;
            case 6:
                mostrar_buenas_practicas();
                break;
            case 0:
                printf("¡Hasta luego!\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 0);
    
    return 0;
}
#endif
