/**
 * @file entrada_segura.c
 * @brief Implementación de funciones para entrada segura de datos
 * @details Implementa la comparación entre gets() (inseguro) y fgets() (seguro),
 *          demostrando por qué es crucial usar funciones seguras para prevenir
 *          vulnerabilidades de buffer overflow.
 */

#include "../include/entrada_segura.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
static void imprimir_separador(int num)
{
    for (int i = 0; i < num; i++) putchar('=');
    putchar('\n');
}

// Contar "caracteres" en UTF-8 (no bytes): cuenta solo bytes que no son continuaciones 10xxxxxx
// (Eliminado: cómputo inline para evitar warning de función no usada)

// =============================================================================
// FUNCIONES SEGURAS
// =============================================================================

resultado_entrada_t entrada_segura_fgets(char* buffer, size_t tamaño, FILE* archivo) {
    if (buffer == NULL || tamaño < 2 || tamaño > INT_MAX || archivo == NULL) {
        return ENTRADA_ERROR;
    }
    
    buffer[0] = '\0';
    if (fgets(buffer, (int)tamaño, archivo) == NULL) {
        if (feof(archivo)) {
            return ENTRADA_EOF;
        }
        return ENTRADA_ERROR;
    }
    
    // Verificar si la entrada fue truncada
    size_t len = strlen(buffer);
    if (len == tamaño - 1 && buffer[len - 1] != '\n') {
        return ENTRADA_TRUNCADA;
    }
    
    return ENTRADA_EXITOSA;
}

resultado_entrada_t entrada_segura_linea_limpia(char* buffer, size_t tamaño, FILE* archivo) {
    if (buffer == NULL || tamaño < 2 || tamaño > INT_MAX || archivo == NULL) {
        return ENTRADA_ERROR;
    }
    
    resultado_entrada_t resultado = entrada_segura_fgets(buffer, tamaño, archivo);
    
    if (resultado == ENTRADA_EXITOSA || resultado == ENTRADA_TRUNCADA) {
        // Eliminar el '\n' final si existe
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        // Si fue truncada, limpiar el resto de la línea del stream
        if (resultado == ENTRADA_TRUNCADA) {
            int c;
            while ((c = fgetc(archivo)) != '\n' && c != EOF) {
                // Descartar caracteres restantes
            }
        }
    }
    
    return resultado;
}

resultado_entrada_t entrada_segura_validada(char* buffer, size_t tamaño, 
                                           FILE* archivo, estadisticas_entrada_t* stats) {
    if (buffer == NULL || tamaño < 2 || tamaño > INT_MAX || archivo == NULL) {
        if (stats) {
            stats->codigo_ultimo_error = EINVAL;
        }
        return ENTRADA_ERROR;
    }
    
    // Inicializar estadísticas si se proporcionan
    if (stats) {
        stats->caracteres_leidos = 0;
        stats->truncamiento_ocurrido = false;
        stats->codigo_ultimo_error = 0;
    }
    
    resultado_entrada_t resultado = entrada_segura_linea_limpia(buffer, tamaño, archivo);
    
    // Actualizar estadísticas
    if (stats) {
        if (resultado == ENTRADA_EXITOSA || resultado == ENTRADA_TRUNCADA) {
            stats->caracteres_leidos = strlen(buffer);
            stats->lineas_procesadas++;
        }
        
        if (resultado == ENTRADA_TRUNCADA) {
            stats->truncamiento_ocurrido = true;
        }
        
        if (resultado == ENTRADA_ERROR) {
            stats->codigo_ultimo_error = errno;
        }
    }
    
    return resultado;
}

int entrada_segura_scanf_limitado(char* buffer, size_t tamaño, const char* formato) {
    if (buffer == NULL || tamaño < 2 || formato == NULL) {
        return 0;
    }
    
    // Crear formato con límite específico
    char formato_seguro[50];
    snprintf(formato_seguro, sizeof(formato_seguro), "%%%zu[^\n]", tamaño - 1);
    
    int resultado = scanf(formato_seguro, buffer);
    
    // Limpiar buffer si se leyó algo
    if (resultado > 0) {
        limpiar_buffer_entrada();
    }
    
    return resultado;
}

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// =============================================================================

void demostrar_vulnerabilidad_gets(void) {
    char buffer[BUFFER_PEQUEÑO] = {0};
    printf("Demostración de límites: un buffer de %zu bytes admite %zu bytes y '\\0'.\n",
           sizeof buffer, sizeof buffer - 1);
    printf("gets(buffer) no recibe el tamaño; una entrada larga lo desbordaría.\n");
    printf("La demostración usa una lectura limitada. Ingrese una cadena larga: ");
    resultado_entrada_t resultado = entrada_segura_linea_limpia(buffer, sizeof buffer, stdin);
    printf("Resultado: %s\nTexto conservado: \"%s\"\n", resultado_a_string(resultado), buffer);
}

void demostrar_seguridad_fgets(void) {
    char buffer[BUFFER_SIZE] = {0};
    estadisticas_entrada_t stats;
    
    putchar('\n');
    imprimir_separador(60);
    printf("DEMOSTRACIÓN: SEGURIDAD DE fgets()\n");
    imprimir_separador(60);
    
    printf("\n📋 Configuración segura:\n");
    printf("   - Buffer size: %d bytes\n", BUFFER_SIZE);
    printf("   - Función: fgets() con límite de tamaño\n");
    printf("   - Protección: Límite igual a la capacidad real\n");
    
    inicializar_estadisticas(&stats);
    
    printf("\n🔒 fgets() respeta el tamaño indicado; debe ser la capacidad real.\n");
    printf("Puede ingresar cualquier cantidad de texto - estará protegido.\n");
    printf("Ingrese texto (máximo %d caracteres útiles): ", BUFFER_SIZE - 1);
    
    resultado_entrada_t resultado = entrada_segura_validada(buffer, sizeof(buffer), 
                                                          stdin, &stats);
    
    printf("\n📊 Resultados seguros:\n");
    printf("   - Buffer leído: \"%s\"\n", buffer);
    printf("   - Longitud: %zu caracteres\n", stats.caracteres_leidos);
    printf("   - Estado: %s\n", resultado_a_string(resultado));
    
    if (resultado == ENTRADA_TRUNCADA) {
        printf("   ⚠️  Entrada truncada por seguridad (excedió %d caracteres)\n", 
               BUFFER_SIZE - 1);
        printf("   ✅ No hay riesgo de buffer overflow - memoria protegida.\n");
    } else {
        printf("   ✅ Entrada completa leída de forma segura.\n");
    }
    
    imprimir_estadisticas(&stats);
}

void comparacion_gets_vs_fgets(void) {
    putchar('\n');
    imprimir_separador(70);
    printf("COMPARACIÓN LADO A LADO: gets() vs fgets()\n");
    imprimir_separador(70);
    
    printf("\n📋 Tabla comparativa:\n");
    printf("┌─────────────────────┬─────────────────┬─────────────────────┐\n");
    printf("│ Característica      │ gets()          │ fgets()             │\n");
    printf("├─────────────────────┼─────────────────┼─────────────────────┤\n");
    printf("│ Verificación límites│ ❌ NO           │ ✅ SÍ               │\n");
    printf("│ Buffer overflow     │ ❌ POSIBLE      │ ✅ IMPOSIBLE        │\n");
    printf("│ Seguridad           │ ❌ INSEGURO     │ ✅ SEGURO           │\n");
    printf("│ Estándar C          │ ❌ REMOVIDO C11 │ ✅ ESTÁNDAR         │\n");
    printf("│ Uso recomendado     │ ❌ NUNCA        │ ✅ SIEMPRE          │\n");
    printf("└─────────────────────┴─────────────────┴─────────────────────┘\n");
    
    printf("\n🎯 Regla de oro: NUNCA usar gets() - SIEMPRE usar fgets()\n");
    
    printf("\n🧪 Demostración interactiva:\n");
    printf("Presione Enter para continuar con cada demostración...\n");
    getchar();
    
    demostrar_vulnerabilidad_gets();
    
    printf("\nPresione Enter para ver la alternativa segura...\n");
    getchar();
    
    demostrar_seguridad_fgets();
}

void demostracion_completa_entrada_segura(void) {
    printf("\n" "🛡️  EJERCICIO 094: ENTRADA SEGURA DE DATOS" "\n");
    imprimir_separador(70);
    
    printf("\n📚 OBJETIVOS DE APRENDIZAJE:\n");
    printf("   • Entender por qué gets() es peligroso\n");
    printf("   • Aprender a usar fgets() de forma segura\n");
    printf("   • Prevenir vulnerabilidades de buffer overflow\n");
    printf("   • Implementar mejores prácticas de entrada\n");
    
    printf("\n⚠️  CONCEPTOS CLAVE:\n");
    printf("   • Buffer Overflow: Escribir más allá de los límites del buffer\n");
    printf("   • Verificación de límites: Validar tamaños antes de escribir\n");
    printf("   • Funciones seguras: Siempre especificar límites máximos\n");
    
    printf("\n¿Desea ejecutar la demostración completa? (s/n): ");
    char respuesta;
    if (scanf(" %c", &respuesta) != 1) return;
    getchar(); // Limpiar buffer
    
    if (respuesta == 's' || respuesta == 'S') {
        comparacion_gets_vs_fgets();
        
        printf("\n🎓 LECCIONES APRENDIDAS:\n");
        printf("   ✅ gets() está prohibido - causa vulnerabilidades\n");
        printf("   ✅ fgets() es la alternativa segura\n");
        printf("   ✅ Siempre especificar límites en funciones de entrada\n");
        printf("   ✅ Validar y manejar casos de truncamiento\n");
        
        printf("\n💡 MEJORES PRÁCTICAS:\n");
        printf("   • Usar fgets() en lugar de gets()\n");
        printf("   • Usar scanf() con especificadores de ancho de campo\n");
        printf("   • Validar entrada antes de procesarla\n");
        printf("   • Manejar casos de entrada truncada gracefully\n");
    }
}

// =============================================================================
// FUNCIONES AUXILIARES
// =============================================================================

const char* resultado_a_string(resultado_entrada_t resultado) {
    switch (resultado) {
        case ENTRADA_EXITOSA:
            return "Entrada exitosa";
        case ENTRADA_TRUNCADA:
            return "Entrada truncada (excedió buffer)";
        case ENTRADA_ERROR:
            return "Error en la entrada";
        case ENTRADA_EOF:
            return "Fin de archivo";
        default:
            return "Resultado desconocido";
    }
}

void limpiar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Descartar caracteres hasta nueva línea o EOF
    }
}

void imprimir_estadisticas(const estadisticas_entrada_t* stats) {
    if (stats == NULL) {
        return;
    }
    
    printf("\n📈 Estadísticas de entrada:\n");
    printf("   - Caracteres leídos: %zu\n", stats->caracteres_leidos);
    printf("   - Líneas procesadas: %zu\n", stats->lineas_procesadas);
    printf("   - Truncamiento: %s\n", stats->truncamiento_ocurrido ? "Sí" : "No");
    
    if (stats->codigo_ultimo_error != 0) {
        printf("   - Último error: %d\n", stats->codigo_ultimo_error);
    }
}

void inicializar_estadisticas(estadisticas_entrada_t* stats) {
    if (stats == NULL) {
        return;
    }
    
    stats->caracteres_leidos = 0;
    stats->lineas_procesadas = 0;
    stats->truncamiento_ocurrido = false;
    stats->codigo_ultimo_error = 0;
}
