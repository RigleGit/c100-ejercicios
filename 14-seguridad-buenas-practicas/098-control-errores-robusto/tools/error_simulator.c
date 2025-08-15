#include "control_errores_robusto.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

typedef struct {
    const char *nombre;
    const char *descripcion;
    void (*simular)(void);
} simulacion_error_t;

void simular_archivo_inexistente(void) {
    printf("\n=== Simulando: Archivo Inexistente ===\n");
    printf("Intentando abrir '/ruta/que/no/existe/archivo.txt' para lectura...\n");
    
    FILE *archivo = NULL;
    result_t resultado = abrir_archivo_seguro("/ruta/que/no/existe/archivo.txt", "r", &archivo);
    
    printf("Estado: ");
    if (!resultado.success) {
        printf("❌ Error detectado (correcto)\n");
        printf("Detalles: %s\n", resultado.error_message);
        printf("Código: %d\n", resultado.error_code);
    } else {
        printf("⚠️  Archivo abierto inesperadamente\n");
        liberar_recursos_seguro(archivo, NULL);
    }
    
    printf("🎓 Lección: Siempre verificar fopen() antes de usar el FILE*\n");
}

void simular_permisos_insuficientes(void) {
    printf("\n=== Simulando: Permisos Insuficientes ===\n");
    printf("Intentando escribir en directorio protegido del sistema...\n");
    
    FILE *archivo = NULL;
    result_t resultado = abrir_archivo_seguro("/etc/archivo_protegido.txt", "w", &archivo);
    
    printf("Estado: ");
    if (!resultado.success) {
        printf("❌ Error detectado (correcto)\n");
        printf("Detalles: %s\n", resultado.error_message);
        printf("Código: %d\n", resultado.error_code);
    } else {
        printf("⚠️  Archivo abierto (permisos inesperados)\n");
        liberar_recursos_seguro(archivo, NULL);
    }
    
    printf("🎓 Lección: Los permisos pueden cambiar en runtime\n");
}

void simular_disco_lleno(void) {
    printf("\n=== Simulando: Disco Lleno ===\n");
    printf("Nota: Simulación conceptual (no llenamos el disco real)\n");
    
    // Crear archivo temporal y intentar escribir mucho
    FILE *archivo = NULL;
    result_t resultado = abrir_archivo_seguro("test_disco.tmp", "w", &archivo);
    
    if (resultado.success) {
        printf("Archivo creado exitosamente\n");
        
        // Intentar escribir datos
        char datos[] = "Datos de prueba para simulación de disco lleno\n";
        resultado = escribir_datos_seguro(archivo, datos, strlen(datos));
        
        if (resultado.success) {
            printf("✅ Escritura exitosa (disco con espacio suficiente)\n");
        } else {
            printf("❌ Error de escritura detectado\n");
            imprimir_error(&resultado);
        }
        
        liberar_recursos_seguro(archivo, NULL);
        unlink("test_disco.tmp");
    }
    
    printf("🎓 Lección: En producción, verificar fwrite() y fflush()\n");
}

void simular_memoria_insuficiente(void) {
    printf("\n=== Simulando: Memoria Insuficiente ===\n");
    
    // Intentar asignar tamaño imposible (simulación)
    void *ptr = NULL;
    printf("Intentando asignar memoria con parámetros inválidos...\n");
    
    result_t resultado = asignar_memoria_seguro(0, &ptr);
    
    printf("Estado: ");
    if (!resultado.success) {
        printf("❌ Error detectado (correcto)\n");
        printf("Detalles: %s\n", resultado.error_message);
        printf("Código: %d\n", resultado.error_code);
    } else {
        printf("⚠️  Asignación exitosa inesperada\n");
        liberar_recursos_seguro(NULL, ptr);
    }
    
    // Parámetro nulo
    printf("\nIntentando asignar con puntero nulo...\n");
    resultado = asignar_memoria_seguro(1024, NULL);
    
    printf("Estado: ");
    if (!resultado.success) {
        printf("❌ Error detectado (correcto)\n");
        printf("Detalles: %s\n", resultado.error_message);
    }
    
    printf("🎓 Lección: malloc() puede fallar incluso con tamaños pequeños\n");
}

void simular_entrada_invalida(void) {
    printf("\n=== Simulando: Entrada Inválida ===\n");
    printf("Simulando entrada de texto cuando se espera número...\n");
    
    // Crear entrada temporal simulada
    printf("Entrada simulada: 'texto_no_numerico'\n");
    
    // Simular proceso de validación
    char *entrada_invalida = "texto_no_numerico";
    char *endptr;
    long valor = strtol(entrada_invalida, &endptr, 10);
    
    printf("Resultado de strtol: %ld\n", valor);
    printf("Endptr apunta a: '%s'\n", endptr);
    
    if (endptr == entrada_invalida || *endptr != '\0') {
        printf("❌ Entrada inválida detectada correctamente\n");
        printf("🎓 La función leer_entero_seguro() detectaría este error\n");
    } else {
        printf("⚠️  Conversión inesperadamente exitosa\n");
    }
    
    printf("🎓 Lección: Validar entrada antes de usar valores convertidos\n");
}

void simular_punteros_nulos(void) {
    printf("\n=== Simulando: Punteros Nulos ===\n");
    printf("Probando funciones con parámetros nulos...\n");
    
    // Prueba 1: abrir_archivo_seguro con parámetros nulos
    printf("\n1. abrir_archivo_seguro(NULL, \"w\", ptr):\n");
    FILE *archivo = NULL;
    result_t resultado = abrir_archivo_seguro(NULL, "w", &archivo);
    if (!resultado.success) {
        printf("✅ Error detectado: %s\n", resultado.error_message);
    }
    
    // Prueba 2: escribir_datos_seguro con datos nulos
    printf("\n2. escribir_datos_seguro(file, NULL, 10):\n");
    FILE *temp = fopen("temp_null.txt", "w");
    if (temp) {
        resultado = escribir_datos_seguro(temp, NULL, 10);
        if (!resultado.success) {
            printf("✅ Error detectado: %s\n", resultado.error_message);
        }
        fclose(temp);
        unlink("temp_null.txt");
    }
    
    // Prueba 3: leer_entero_seguro con puntero nulo
    printf("\n3. leer_entero_seguro(NULL):\n");
    resultado = leer_entero_seguro(NULL);
    if (!resultado.success) {
        printf("✅ Error detectado: %s\n", resultado.error_message);
    }
    
    printf("🎓 Lección: Validar TODOS los parámetros en funciones públicas\n");
}

void simular_recursos_no_liberados(void) {
    printf("\n=== Simulando: Gestión de Recursos ===\n");
    printf("Demostrando liberación segura de recursos...\n");
    
    FILE *archivo = NULL;
    void *memoria = NULL;
    
    // Asignar recursos
    printf("\n1. Asignando recursos...\n");
    result_t resultado = abrir_archivo_seguro("temp_recursos.txt", "w", &archivo);
    if (resultado.success) {
        printf("✅ Archivo abierto\n");
    }
    
    resultado = asignar_memoria_seguro(1024, &memoria);
    if (resultado.success) {
        printf("✅ Memoria asignada\n");
    }
    
    // Simular error en el medio del proceso
    printf("\n2. Simulando error que requiere limpieza...\n");
    printf("(En código real, aquí ocurriría un error)\n");
    
    // Liberación segura
    printf("\n3. Liberando recursos de forma segura...\n");
    liberar_recursos_seguro(archivo, memoria);
    printf("✅ Recursos liberados correctamente\n");
    
    // Verificar que se puede llamar con nulos
    printf("\n4. Probando liberación con parámetros nulos...\n");
    liberar_recursos_seguro(NULL, NULL);
    printf("✅ Función manejó parámetros nulos sin problemas\n");
    
    unlink("temp_recursos.txt");
    printf("🎓 Lección: Siempre liberar recursos en TODOS los caminos de error\n");
}

void simular_cascada_errores(void) {
    printf("\n=== Simulando: Cascada de Errores ===\n");
    printf("Demostrando cómo un error puede causar una cadena de problemas...\n");
    
    printf("\nEscenario: Función que necesita archivo y memoria\n");
    printf("1. Intentar abrir archivo inexistente...\n");
    
    FILE *archivo = NULL;
    result_t resultado = abrir_archivo_seguro("/archivo/inexistente.txt", "r", &archivo);
    
    if (!resultado.success) {
        printf("❌ Error en apertura de archivo\n");
        printf("   → No se procede con asignación de memoria\n");
        printf("   → Función termina limpiamente sin recursos pendientes\n");
        printf("✅ Cascada de errores evitada por control robusto\n");
    }
    
    printf("\nContraste con código inseguro:\n");
    printf("❌ Código inseguro continuaría con archivo NULL\n");
    printf("❌ malloc() se ejecutaría innecesariamente\n");
    printf("❌ fprintf(NULL, ...) causaría crash\n");
    printf("❌ Memoria no se liberaría nunca\n");
    
    printf("🎓 Lección: Control temprano evita problemas en cascada\n");
}

simulacion_error_t simulaciones[] = {
    {"archivo_inexistente", "Intento de abrir archivo que no existe", simular_archivo_inexistente},
    {"permisos_insuficientes", "Intento de escribir sin permisos", simular_permisos_insuficientes},
    {"disco_lleno", "Simulación de disco lleno", simular_disco_lleno},
    {"memoria_insuficiente", "Fallo en asignación de memoria", simular_memoria_insuficiente},
    {"entrada_invalida", "Entrada no numérica en función numérica", simular_entrada_invalida},
    {"punteros_nulos", "Parámetros nulos en funciones", simular_punteros_nulos},
    {"recursos_no_liberados", "Gestión correcta de recursos", simular_recursos_no_liberados},
    {"cascada_errores", "Prevención de errores en cascada", simular_cascada_errores}
};

void mostrar_menu_simulaciones(void) {
    printf("\n=== SIMULADOR DE ERRORES ===\n");
    printf("Seleccione el tipo de error a simular:\n\n");
    
    for (size_t i = 0; i < sizeof(simulaciones) / sizeof(simulaciones[0]); i++) {
        printf("%zu. %s\n", i + 1, simulaciones[i].descripcion);
    }
    
    printf("0. Ejecutar todas las simulaciones\n");
    printf("q. Salir\n\n");
    printf("Opción: ");
}

int main(void) {
    printf("Simulador de Errores - Control de Errores Robusto\n");
    printf("=================================================\n");
    printf("Este simulador demuestra diferentes tipos de errores\n");
    printf("y cómo el código robusto los maneja correctamente.\n");
    
    char opcion[10];
    
    do {
        mostrar_menu_simulaciones();
        
        if (!fgets(opcion, sizeof(opcion), stdin)) {
            break;
        }
        
        // Eliminar salto de línea
        size_t len = strlen(opcion);
        if (len > 0 && opcion[len-1] == '\n') {
            opcion[len-1] = '\0';
        }
        
        if (strcmp(opcion, "q") == 0 || strcmp(opcion, "Q") == 0) {
            break;
        }
        
        if (strcmp(opcion, "0") == 0) {
            printf("\n🚀 Ejecutando todas las simulaciones...\n");
            for (size_t i = 0; i < sizeof(simulaciones) / sizeof(simulaciones[0]); i++) {
                simulaciones[i].simular();
                printf("\n" "─" "─" "─" "─" "─" "─" "─" "─" "─" "─" 
                       "─" "─" "─" "─" "─" "─" "─" "─" "─" "─" "\n");
            }
        } else {
            long seleccion = strtol(opcion, NULL, 10);
            if (seleccion >= 1 && seleccion <= (long)(sizeof(simulaciones) / sizeof(simulaciones[0]))) {
                simulaciones[seleccion - 1].simular();
            } else {
                printf("❌ Opción inválida. Intente nuevamente.\n");
            }
        }
        
        printf("\nPresione Enter para continuar...");
        getchar();
        
    } while (1);
    
    printf("\n=== RESUMEN DE LECCIONES ===\n");
    printf("✅ Verificar TODOS los valores de retorno\n");
    printf("✅ Validar parámetros de entrada\n");
    printf("✅ Liberar recursos en TODOS los caminos\n");
    printf("✅ Proporcionar mensajes de error descriptivos\n");
    printf("✅ Usar códigos de error consistentes\n");
    printf("✅ Implementar principio fail-fast\n");
    printf("✅ Considerar todos los casos de error posibles\n\n");
    
    printf("💡 El control robusto de errores es la diferencia entre\n");
    printf("   software amateur y software profesional.\n\n");
    
    printf("¡Gracias por usar el simulador!\n");
    
    return 0;
}
