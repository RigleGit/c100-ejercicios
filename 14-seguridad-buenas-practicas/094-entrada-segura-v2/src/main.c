/**
 * @file main.c
 * @brief Programa principal para demostrar entrada segura de datos
 * @details Programa interactivo que demuestra la diferencia entre funciones
 *          de entrada inseguras (gets) y seguras (fgets), mostrando por qué
 *          es crucial usar funciones que verifican límites de buffer.
 */

#include "../include/entrada_segura.h"
#include <stdlib.h>

/**
 * @brief Muestra el menú principal de opciones
 */
static void imprimir_separador(int n) { for (int i = 0; i < n; i++) putchar('='); putchar('\n'); }

void mostrar_menu(void) {
    putchar('\n');
    imprimir_separador(60);
    printf("🛡️  EJERCICIO 094: ENTRADA SEGURA DE DATOS\n");
    imprimir_separador(60);
    printf("\n📋 OPCIONES DISPONIBLES:\n");
    printf("   1. 🔴 Demostrar vulnerabilidad de gets()\n");
    printf("   2. 🟢 Demostrar seguridad de fgets()\n");
    printf("   3. ⚖️  Comparación lado a lado\n");
    printf("   4. 🎓 Demostración completa educativa\n");
    printf("   5. 🧪 Pruebas interactivas de funciones\n");
    printf("   6. 📚 Información teórica\n");
    printf("   0. 🚪 Salir\n");
    printf("\nSeleccione una opción: ");
}

/**
 * @brief Muestra información teórica sobre seguridad en entrada de datos
 */
void mostrar_teoria(void) {
    putchar('\n');
    imprimir_separador(70);
    printf("📚 TEORÍA: SEGURIDAD EN ENTRADA DE DATOS\n");
    imprimir_separador(70);
    
    printf("\n🎯 ¿QUÉ ES UN BUFFER OVERFLOW?\n");
    printf("Un buffer overflow ocurre cuando un programa escribe datos más allá\n");
    printf("de los límites de un buffer asignado. Esto puede:\n");
    printf("   • Corromper datos adyacentes en memoria\n");
    printf("   • Sobrescribir direcciones de retorno\n");
    printf("   • Permitir ejecución de código arbitrario\n");
    printf("   • Causar crashes del programa\n");
    
    printf("\n🚨 ¿POR QUÉ gets() ES PELIGROSO?\n");
    printf("La función gets() lee caracteres hasta encontrar un '\\n', pero:\n");
    printf("   ❌ NO verifica el tamaño del buffer destino\n");
    printf("   ❌ NO tiene forma de saber cuándo parar\n");
    printf("   ❌ Puede escribir indefinidamente en memoria\n");
    printf("   ❌ Es responsable de incontables vulnerabilidades\n");
    
    printf("\n✅ ¿POR QUÉ fgets() ES SEGURO?\n");
    printf("La función fgets() requiere especificar el tamaño máximo:\n");
    printf("   ✅ Siempre respeta el límite especificado\n");
    printf("   ✅ Lee máximo (n-1) caracteres + '\\0'\n");
    printf("   ✅ Imposible que cause buffer overflow\n");
    printf("   ✅ Preserva la nueva línea si cabe en el buffer\n");
    
    printf("\n🛡️  OTRAS FUNCIONES INSEGURAS A EVITAR:\n");
    printf("   • scanf(\"%%s\", buffer)     → Usar scanf(\"%%49s\", buffer)\n");
    printf("   • strcpy(dest, src)       → Usar strncpy() o strlcpy()\n");
    printf("   • strcat(dest, src)       → Usar strncat() o strlcat()\n");
    printf("   • sprintf(buffer, ...)    → Usar snprintf()\n");
    
    printf("\n📜 HISTORIA:\n");
    printf("   • gets() fue marcado como obsoleto en C99 (1999)\n");
    printf("   • Fue completamente removido del estándar C11 (2011)\n");
    printf("   • Morris Worm (1988) explotó gets() en fingerd\n");
    printf("   • Incontables exploits han usado buffer overflows\n");
    
    printf("\nPresione Enter para continuar...");
    getchar();
}

/**
 * @brief Ejecuta pruebas interactivas de las funciones
 */
void pruebas_interactivas(void) {
    char buffer[100];
    estadisticas_entrada_t stats;
    int opcion;
    
    putchar('\n');
    imprimir_separador(60);
    printf("🧪 PRUEBAS INTERACTIVAS\n");
    imprimir_separador(60);
    
    do {
        printf("\n📋 Pruebas disponibles:\n");
        printf("   1. Probar entrada_segura_fgets()\n");
        printf("   2. Probar entrada_segura_linea_limpia()\n");
        printf("   3. Probar entrada_segura_validada()\n");
        printf("   4. Probar scanf seguro con límites\n");
        printf("   0. Volver al menú principal\n");
        printf("\nSeleccione: ");
        
        scanf("%d", &opcion);
        getchar(); // Limpiar buffer
        
        switch (opcion) {
            case 1:
                printf("\n🔬 Probando entrada_segura_fgets():\n");
                printf("Ingrese texto: ");
                resultado_entrada_t result = entrada_segura_fgets(buffer, sizeof(buffer), stdin);
                printf("Resultado: %s\n", resultado_a_string(result));
                printf("Buffer: \"%s\"\n", buffer);
                break;
                
            case 2:
                printf("\n🔬 Probando entrada_segura_linea_limpia():\n");
                printf("Ingrese texto: ");
                result = entrada_segura_linea_limpia(buffer, sizeof(buffer), stdin);
                printf("Resultado: %s\n", resultado_a_string(result));
                printf("Buffer: \"%s\"\n", buffer);
                break;
                
            case 3:
                printf("\n🔬 Probando entrada_segura_validada():\n");
                inicializar_estadisticas(&stats);
                printf("Ingrese texto: ");
                result = entrada_segura_validada(buffer, sizeof(buffer), stdin, &stats);
                printf("Resultado: %s\n", resultado_a_string(result));
                printf("Buffer: \"%s\"\n", buffer);
                imprimir_estadisticas(&stats);
                break;
                
            case 4:
                printf("\n🔬 Probando scanf seguro:\n");
                printf("Ingrese una palabra: ");
                int leidos = entrada_segura_scanf_limitado(buffer, sizeof(buffer), "%s");
                printf("Elementos leídos: %d\n", leidos);
                if (leidos > 0) {
                    printf("Buffer: \"%s\"\n", buffer);
                }
                break;
                
            case 0:
                printf("Volviendo al menú principal...\n");
                break;
                
            default:
                printf("❌ Opción inválida\n");
        }
    } while (opcion != 0);
}

/**
 * @brief Función principal del programa
 */
#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    
    printf("Bienvenido al ejercicio de Entrada Segura de Datos!\n");
    printf("Este programa demuestra por qué gets() es peligroso y cómo usar fgets() de forma segura.\n");
    
    do {
        mostrar_menu();
        
        if (scanf("%d", &opcion) != 1) {
            printf("❌ Entrada inválida. Ingrese un número.\n");
            limpiar_buffer_entrada();
            continue;
        }
        getchar(); // Limpiar buffer después de scanf
        
        switch (opcion) {
            case 1:
                demostrar_vulnerabilidad_gets();
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
                
            case 2:
                demostrar_seguridad_fgets();
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
                
            case 3:
                comparacion_gets_vs_fgets();
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
                
            case 4:
                demostracion_completa_entrada_segura();
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
                
            case 5:
                pruebas_interactivas();
                break;
                
            case 6:
                mostrar_teoria();
                break;
                
            case 0:
                printf("\n👋 ¡Gracias por aprender sobre entrada segura!\n");
                printf("Recuerde: NUNCA usar gets() - SIEMPRE usar fgets()\n");
                break;
                
            default:
                printf("❌ Opción inválida. Seleccione un número del 0 al 6.\n");
        }
        
    } while (opcion != 0);
    
    return EXIT_SUCCESS;
}
#endif
