/**
 * @file main.c
 * @brief Programa principal para demostrar funciones seguras de cadenas
 */

#include "funciones_seguras_cadenas.h"
#include <time.h>

/**
 * @brief Muestra el menú principal
 */
void mostrar_menu(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║          FUNCIONES SEGURAS DE CADENAS - Menú Principal       ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Demo: Problemas con funciones inseguras                 ║\n");
    printf("║  2. Demo: Uso de funciones seguras                          ║\n");
    printf("║  3. Demo: Operaciones complejas seguras                     ║\n");
    printf("║  4. Ejemplo práctico interactivo                            ║\n");
    printf("║  5. Comparación lado a lado (inseguro vs seguro)            ║\n");
    printf("║  6. Prueba de estrés de seguridad                           ║\n");
    printf("║  7. Benchmark de rendimiento                                ║\n");
    printf("║  8. Ejercicio guiado: Convertir código inseguro             ║\n");
    printf("║  9. Ayuda y buenas prácticas                                ║\n");
    printf("║  0. Salir                                                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("Selecciona una opción: ");
}

/**
 * @brief Ejemplo práctico interactivo
 */
void ejemplo_practico_interactivo(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              EJEMPLO PRÁCTICO INTERACTIVO                   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    char nombre[MAX_NOMBRE];
    char apellido[MAX_NOMBRE];
    char mensaje[MAX_MENSAJE];
    char buffer_temp[64];
    int edad;
    info_operacion_t info;
    resultado_cadena_t resultado;
    
    printf("👋 Vamos a crear un mensaje personalizado usando funciones seguras\n\n");
    
    // Solicitar nombre
    printf("Introduce tu nombre (máximo %d caracteres): ", MAX_NOMBRE - 1);
    if (fgets(buffer_temp, sizeof(buffer_temp), stdin)) {
        buffer_temp[strcspn(buffer_temp, "\n")] = '\0'; // Quitar salto de línea
        resultado = copia_segura(nombre, buffer_temp, sizeof(nombre), &info);
        
        if (resultado == CADENA_ERROR_CADENA_TRUNCADA) {
            printf("⚠️  Nombre truncado para ajustarse al límite\n");
        }
        printf("✅ Nombre guardado: \"%s\"\n\n", nombre);
    }
    
    // Solicitar apellido
    printf("Introduce tu apellido: ");
    if (fgets(buffer_temp, sizeof(buffer_temp), stdin)) {
        buffer_temp[strcspn(buffer_temp, "\n")] = '\0';
        resultado = copia_segura(apellido, buffer_temp, sizeof(apellido), &info);
        
        if (resultado == CADENA_ERROR_CADENA_TRUNCADA) {
            printf("⚠️  Apellido truncado para ajustarse al límite\n");
        }
        printf("✅ Apellido guardado: \"%s\"\n\n", apellido);
    }
    
    // Solicitar edad
    printf("Introduce tu edad: ");
    if (scanf("%d", &edad) == 1) {
        // Limpiar buffer de entrada
        while (getchar() != '\n');
        printf("✅ Edad guardada: %d años\n\n", edad);
    } else {
        edad = 0;
        printf("⚠️  Edad inválida, usando 0\n\n");
        // Limpiar buffer de entrada
        while (getchar() != '\n');
    }
    
    // Construir mensaje paso a paso
    printf("🔨 Construyendo mensaje personalizado:\n\n");
    
    // Paso 1: Inicializar con saludo
    resultado = copia_segura(mensaje, "¡Hola ", sizeof(mensaje), &info);
    printf("1. Inicialización: \"%s\" (%s)\n", mensaje, resultado_cadena_string(resultado));
    
    // Paso 2: Agregar nombre
    resultado = concatena_segura(mensaje, nombre, sizeof(mensaje), &info);
    printf("2. + Nombre: \"%s\" (%s)\n", mensaje, resultado_cadena_string(resultado));
    
    // Paso 3: Agregar apellido
    resultado = concatena_con_separador(mensaje, apellido, " ", sizeof(mensaje));
    printf("3. + Apellido: \"%s\" (%s)\n", mensaje, resultado_cadena_string(resultado));
    
    // Paso 4: Agregar información de edad
    if (edad > 0) {
        resultado = formatea_seguro(buffer_temp, sizeof(buffer_temp), ", tienes %d años", edad);
        if (resultado == CADENA_OK) {
            resultado = concatena_segura(mensaje, buffer_temp, sizeof(mensaje), &info);
            printf("4. + Edad: \"%s\" (%s)\n", mensaje, resultado_cadena_string(resultado));
        }
    }
    
    // Paso 5: Finalizar mensaje
    resultado = concatena_segura(mensaje, "! Bienvenido/a.", sizeof(mensaje), &info);
    printf("5. + Final: \"%s\" (%s)\n", mensaje, resultado_cadena_string(resultado));
    
    // Mostrar estadísticas
    printf("\n📊 Estadísticas del proceso:\n");
    printf("   Tamaño del buffer: %zu bytes\n", sizeof(mensaje));
    printf("   Bytes utilizados: %zu\n", strlen(mensaje) + 1);
    printf("   Espacio restante: %zu bytes\n", espacio_restante_buffer(mensaje, sizeof(mensaje)));
    printf("   Truncación ocurrida: %s\n", info.truncado ? "Sí" : "No");
    
    printf("\n🎉 Mensaje final:\n");
    printf("   \"%s\"\n", mensaje);
}

/**
 * @brief Comparación lado a lado de código inseguro vs seguro
 */
void comparacion_lado_a_lado(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           COMPARACIÓN: INSEGURO vs SEGURO                   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("📋 Ejemplo del ejercicio original:\n\n");
    
    // Mostrar código inseguro (sin ejecutar)
    printf("🔴 CÓDIGO INSEGURO (NO EJECUTAR):\n");
    printf("   char destino[20];\n");
    printf("   char nombre[] = \"Rodrigo\";\n");
    printf("   int edad = 30;\n");
    printf("   \n");
    printf("   strcpy(destino, \"Nombre: \");         // Sin verificación de tamaño\n");
    printf("   strcat(destino, nombre);              // Sin verificar espacio\n");
    printf("   sprintf(destino, \"Edad: %%d\", edad);   // Sobrescribe sin control\n\n");
    
    printf("❌ Problemas del código inseguro:\n");
    printf("   • strcpy() no verifica límites del buffer\n");
    printf("   • strcat() puede escribir fuera del buffer\n");
    printf("   • sprintf() sobrescribe el contenido sin verificar tamaño\n");
    printf("   • Potencial buffer overflow y corrupción de memoria\n\n");
    
    // Ejecutar versión segura
    printf("✅ CÓDIGO SEGURO (EJECUTANDO):\n");
    
    char destino[20];
    char nombre[] = "Rodrigo";
    int edad = 30;
    info_operacion_t info;
    resultado_cadena_t resultado;
    
    printf("   char destino[20];\n");
    printf("   char nombre[] = \"Rodrigo\";\n");
    printf("   int edad = 30;\n");
    printf("   \n");
    
    // Copia segura
    printf("   // Usar strncpy para copiar y asegurar terminación\n");
    resultado = copia_segura(destino, "Nombre: ", sizeof(destino), &info);
    printf("   copia_segura(destino, \"Nombre: \", sizeof(destino));\n");
    printf("   ➤ Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   ➤ Contenido: \"%s\"\n", destino);
    printf("   ➤ Bytes usados: %zu de %zu\n\n", info.bytes_usados, info.bytes_disponibles);
    
    // Concatenación segura
    printf("   // Usar strncat para concatenar sin desbordar\n");
    resultado = concatena_segura(destino, nombre, sizeof(destino), &info);
    printf("   concatena_segura(destino, nombre, sizeof(destino));\n");
    printf("   ➤ Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   ➤ Contenido: \"%s\"\n", destino);
    printf("   ➤ Truncado: %s\n\n", info.truncado ? "Sí" : "No");
    
    // Formateo seguro en buffer separado
    printf("   // Usar snprintf en buffer separado para formatear\n");
    char info_edad[20];
    resultado = formatea_seguro(info_edad, sizeof(info_edad), "Edad: %d", edad);
    printf("   formatea_seguro(info_edad, sizeof(info_edad), \"Edad: %%d\", edad);\n");
    printf("   ➤ Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   ➤ Contenido: \"%s\"\n\n", info_edad);
    
    printf("✨ Ventajas del código seguro:\n");
    printf("   • Control total sobre los límites de buffer\n");
    printf("   • Prevención garantizada de buffer overflow\n");
    printf("   • Manejo controlado de truncación\n");
    printf("   • Información detallada sobre las operaciones\n");
    printf("   • Código más robusto y mantenible\n");
}

/**
 * @brief Ejercicio guiado para convertir código inseguro
 */
void ejercicio_guiado_conversion(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║          EJERCICIO GUIADO: CONVERTIR CÓDIGO INSEGURO        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("🎓 Vamos a convertir paso a paso código inseguro a seguro\n\n");
    
    // Presentar código problemático
    printf("📝 Código problemático a convertir:\n");
    printf("   ╭─────────────────────────────────────────────────────╮\n");
    printf("   │ char buffer[50];                                    │\n");
    printf("   │ char usuario[30];                                   │\n");
    printf("   │ char mensaje[100];                                  │\n");
    printf("   │                                                     │\n");
    printf("   │ printf(\"Usuario: \");                                │\n");
    printf("   │ gets(usuario);              // PROBLEMA 1          │\n");
    printf("   │                                                     │\n");
    printf("   │ strcpy(buffer, \"Hola \");     // PROBLEMA 2          │\n");
    printf("   │ strcat(buffer, usuario);    // PROBLEMA 3          │\n");
    printf("   │                                                     │\n");
    printf("   │ sprintf(mensaje, \"Usuario: %s, Tiempo: %ld\",       │\n");
    printf("   │         usuario, time(NULL)); // PROBLEMA 4        │\n");
    printf("   ╰─────────────────────────────────────────────────────╯\n");
    printf("\n");
    
    printf("🔍 Problemas identificados:\n");
    printf("   1. gets() - Función prohibida, sin límite de entrada\n");
    printf("   2. strcpy() - Sin verificación de límites\n");
    printf("   3. strcat() - Sin verificación de espacio disponible\n");
    printf("   4. sprintf() - Sin límite de salida\n\n");
    
    // Mostrar conversión paso a paso
    printf("🔧 Conversión a código seguro:\n\n");
    
    // Declaraciones
    char buffer[50];
    char usuario[30];
    char mensaje[100];
    char input_temp[64];
    resultado_cadena_t resultado;
    
    printf("Paso 1 - Reemplazar gets() por fgets():\n");
    printf("   ❌ gets(usuario);\n");
    printf("   ✅ fgets(usuario, sizeof(usuario), stdin);\n");
    printf("      // + eliminación del salto de línea\n\n");
    
    printf("   Ejecutando versión segura:\n");
    printf("   Usuario: ");
    if (fgets(input_temp, sizeof(input_temp), stdin)) {
        input_temp[strcspn(input_temp, "\n")] = '\0';
        resultado = copia_segura(usuario, input_temp, sizeof(usuario), NULL);
        printf("   ✅ Usuario guardado: \"%s\" (%s)\n\n", usuario, resultado_cadena_string(resultado));
    }
    
    printf("Paso 2 - Reemplazar strcpy() por copia_segura():\n");
    printf("   ❌ strcpy(buffer, \"Hola \");\n");
    printf("   ✅ copia_segura(buffer, \"Hola \", sizeof(buffer), NULL);\n");
    
    resultado = copia_segura(buffer, "Hola ", sizeof(buffer), NULL);
    printf("   ✅ Resultado: %s, Buffer: \"%s\"\n\n", resultado_cadena_string(resultado), buffer);
    
    printf("Paso 3 - Reemplazar strcat() por concatena_segura():\n");
    printf("   ❌ strcat(buffer, usuario);\n");
    printf("   ✅ concatena_segura(buffer, usuario, sizeof(buffer), NULL);\n");
    
    resultado = concatena_segura(buffer, usuario, sizeof(buffer), NULL);
    printf("   ✅ Resultado: %s, Buffer: \"%s\"\n\n", resultado_cadena_string(resultado), buffer);
    
    printf("Paso 4 - Reemplazar sprintf() por formatea_seguro():\n");
    printf("   ❌ sprintf(mensaje, \"Usuario: %%s, Tiempo: %%ld\", usuario, time(NULL));\n");
    printf("   ✅ formatea_seguro(mensaje, sizeof(mensaje), \"Usuario: %%s, Tiempo: %%ld\", usuario, time(NULL));\n");
    
    resultado = formatea_seguro(mensaje, sizeof(mensaje), "Usuario: %s, Tiempo: %ld", usuario, time(NULL));
    printf("   ✅ Resultado: %s\n", resultado_cadena_string(resultado));
    printf("   ✅ Mensaje: \"%s\"\n\n", mensaje);
    
    printf("🎉 Código convertido exitosamente!\n");
    printf("📋 Resumen de cambios:\n");
    printf("   • gets() → fgets() + manejo de salto de línea\n");
    printf("   • strcpy() → copia_segura() con verificación de límites\n");
    printf("   • strcat() → concatena_segura() con verificación de espacio\n");
    printf("   • sprintf() → formatea_seguro() con límite de buffer\n");
    printf("   • Agregada verificación de códigos de retorno\n");
    printf("   • Protección completa contra buffer overflow\n");
}

/**
 * @brief Mostrar ayuda y buenas prácticas
 */
void mostrar_ayuda(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                AYUDA Y BUENAS PRÁCTICAS                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("📚 FUNCIONES INSEGURAS VS SEGURAS:\n\n");
    
    printf("┌─────────────┬──────────────────┬─────────────────────────────┐\n");
    printf("│ Insegura    │ Segura           │ Por qué es mejor            │\n");
    printf("├─────────────┼──────────────────┼─────────────────────────────┤\n");
    printf("│ gets()      │ fgets()          │ Límite de entrada          │\n");
    printf("│ strcpy()    │ strncpy()        │ Límite de copia             │\n");
    printf("│ strcat()    │ strncat()        │ Límite de concatenación     │\n");
    printf("│ sprintf()   │ snprintf()       │ Límite de salida            │\n");
    printf("│ vsprintf()  │ vsnprintf()      │ Límite con va_list          │\n");
    printf("└─────────────┴──────────────────┴─────────────────────────────┘\n\n");
    
    printf("🔐 BUENAS PRÁCTICAS DE SEGURIDAD:\n\n");
    
    printf("1. 📏 SIEMPRE especifica límites de buffer:\n");
    printf("   ✅ fgets(buffer, sizeof(buffer), stdin)\n");
    printf("   ❌ gets(buffer)\n\n");
    
    printf("2. 🔚 GARANTIZA terminación nula:\n");
    printf("   ✅ strncpy(dest, src, sizeof(dest) - 1);\n");
    printf("       dest[sizeof(dest) - 1] = '\\0';\n");
    printf("   ❌ strncpy(dest, src, sizeof(dest));\n\n");
    
    printf("3. 📊 VERIFICA códigos de retorno:\n");
    printf("   ✅ if (snprintf(buffer, size, fmt, ...) >= size) { /* truncado */ }\n");
    printf("   ❌ snprintf(buffer, size, fmt, ...);\n\n");
    
    printf("4. 🧮 CALCULA espacio disponible correctamente:\n");
    printf("   ✅ size_t available = sizeof(buffer) - strlen(buffer) - 1;\n");
    printf("   ❌ strcat(buffer, src); // Sin verificar espacio\n\n");
    
    printf("5. 🚫 EVITA funciones prohibidas:\n");
    printf("   ❌ gets(), strcpy(), strcat(), sprintf()\n");
    printf("   ✅ fgets(), strncpy(), strncat(), snprintf()\n\n");
    
    printf("⚠️  SEÑALES DE ALERTA EN EL CÓDIGO:\n");
    printf("   • Uso de funciones sin 'n' (strcpy vs strncpy)\n");
    printf("   • Falta de verificación de límites\n");
    printf("   • Buffers de tamaño fijo sin validación\n");
    printf("   • Entrada de usuario sin sanitizar\n");
    printf("   • Concatenación sin verificar espacio\n\n");
    
    printf("🛠️  HERRAMIENTAS DE ANÁLISIS:\n");
    printf("   • Compilador: -Wall -Wextra -Wformat-security\n");
    printf("   • Análisis estático: clang-static-analyzer, cppcheck\n");
    printf("   • Análisis dinámico: valgrind, AddressSanitizer\n");
    printf("   • Fuzzing: AFL, libFuzzer\n\n");
    
    printf("📖 RECURSOS ADICIONALES:\n");
    printf("   • CERT C Coding Standard\n");
    printf("   • CWE-120: Buffer Copy without Checking Size\n");
    printf("   • CWE-121: Stack-based Buffer Overflow\n");
    printf("   • OWASP Secure Coding Practices\n");
    
    printf("\n💡 RECUERDA: La seguridad no es opcional, es una responsabilidad!\n");
}

/**
 * @brief Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    char input[256];
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           FUNCIONES SEGURAS DE CADENAS v%s               ║\n", FUNCIONES_SEGURAS_VERSION);
    printf("║                Ejercicio 095 - Seguridad                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\nReemplazo de funciones inseguras por alternativas robustas\n");
    printf("Prevención de buffer overflow y vulnerabilidades de memoria\n");
    
    while (1) {
        mostrar_menu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        opcion = atoi(input);
        
        switch (opcion) {
            case 1:
                demo_funciones_inseguras();
                break;
                
            case 2:
                demo_funciones_seguras();
                break;
                
            case 3:
                demo_operaciones_complejas();
                break;
                
            case 4:
                ejemplo_practico_interactivo();
                break;
                
            case 5:
                comparacion_lado_a_lado();
                break;
                
            case 6:
                {
                    int errores = prueba_estres_seguridad();
                    if (errores > 0) {
                        printf("\n⚠️  Se detectaron %d problemas de seguridad\n", errores);
                    }
                }
                break;
                
            case 7:
                printf("\nIntroduce el número de iteraciones para el benchmark (1000-100000): ");
                if (fgets(input, sizeof(input), stdin)) {
                    int iteraciones = atoi(input);
                    if (iteraciones < 1000) iteraciones = 1000;
                    if (iteraciones > 100000) iteraciones = 100000;
                    benchmark_funciones_cadenas(iteraciones);
                }
                break;
                
            case 8:
                ejercicio_guiado_conversion();
                break;
                
            case 9:
                mostrar_ayuda();
                break;
                
            case 0:
                printf("\n🎓 ¡Gracias por aprender sobre seguridad en C!\n");
                printf("Recuerda: siempre usa funciones seguras en código de producción\n");
                return 0;
                
            default:
                printf("\n❌ Opción inválida. Introduce un número del 0-9.\n");
                break;
        }
        
        if (opcion >= 1 && opcion <= 9) {
            printf("\nPresiona Enter para continuar...");
            getchar();
        }
    }
    
    return 0;
}
#endif
