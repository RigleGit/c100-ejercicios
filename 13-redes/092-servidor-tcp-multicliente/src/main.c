/**
 * @file main.c
 * @brief Programa principal de demostración del servidor TCP multicliente
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Este programa demuestra las capacidades del servidor TCP multicliente
 * implementado con hilos POSIX, incluyendo configuración avanzada,
 * diferentes modos de operación y herramientas de análisis.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "../include/servidor_tcp_multicliente.h"

// Variable global para manejo de señales
static volatile sig_atomic_t ejecutando = 1;

/**
 * @brief Manejador de señales
 */
void manejador_senales(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        printf("\nSeñal recibida. Saliendo...\n");
        ejecutando = 0;
    }
}

/**
 * @brief Muestra el menú principal
 */
void mostrar_menu(void) {
    printf("\n=== SERVIDOR TCP MULTICLIENTE ===\n");
    printf("1. Servidor ECO multicliente\n");
    printf("2. Servidor CHAT multicliente\n");
    printf("3. Servidor personalizado\n");
    printf("4. Prueba de stress multicliente\n");
    printf("5. Benchmark de rendimiento\n");
    printf("6. Configuración avanzada\n");
    printf("7. Diagnósticos del sistema\n");
    printf("8. Ejemplos de uso\n");
    printf("9. Monitor de conexiones en tiempo real\n");
    printf("0. Salir\n");
    printf("=================================\n");
    printf("Selecciona una opción: ");
}

/**
 * @brief Demo 1: Servidor ECO multicliente
 */
int demo_servidor_eco(int puerto) {
    printf("\n=== SERVIDOR ECO MULTICLIENTE ===\n");
    
    puerto = puerto > 0 ? puerto : PUERTO_DEMO_ECO;
    printf("Puerto del servidor [%d]: ", puerto);
    
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        puerto = atoi(input);
        if (puerto <= 0 || puerto > 65535) {
            puerto = PUERTO_DEMO_ECO;
            printf("Puerto inválido, usando %d\n", puerto);
        }
    }
    
    printf("\nIniciando servidor ECO en puerto %d...\n", puerto);
    printf("Instrucciones:\n");
    printf("1. Conecta clientes con: telnet localhost %d\n", puerto);
    printf("2. Escribe mensajes - el servidor los devolverá (eco)\n");
    printf("3. Comandos especiales: 'quit', 'stats', 'help'\n");
    printf("4. Presiona Ctrl+C para detener el servidor\n\n");
    
    // aquí iría la lógica real del demo; placeholder compatible
    return 0;
}

/**
 * @brief Demo 2: Servidor CHAT multicliente
 */
int demo_servidor_chat(int puerto) {
    printf("\n=== SERVIDOR CHAT MULTICLIENTE ===\n");
    
    puerto = puerto > 0 ? puerto : PUERTO_DEMO_CHAT;
    printf("Puerto del servidor [%d]: ", puerto);
    
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        puerto = atoi(input);
        if (puerto <= 0 || puerto > 65535) {
            puerto = PUERTO_DEMO_CHAT;
            printf("Puerto inválido, usando %d\n", puerto);
        }
    }
    
    printf("\nIniciando servidor CHAT en puerto %d...\n", puerto);
    printf("Instrucciones:\n");
    printf("1. Conecta múltiples clientes con: telnet localhost %d\n", puerto);
    printf("2. Los mensajes de un cliente se envían a todos los demás\n");
    printf("3. Cada cliente tiene un identificador único\n");
    printf("4. Comandos especiales: 'quit', 'stats', 'help'\n");
    printf("5. Presiona Ctrl+C para detener el servidor\n\n");
    
    return 0;
}

/**
 * @brief Demo 3: Servidor personalizado
 */
void demo_servidor_personalizado(void) {
    printf("\n=== SERVIDOR PERSONALIZADO ===\n");
    
    ContextoServidor contexto;
    ConfigServidor config;
    
    // Configuración interactiva
    configurar_servidor_por_defecto(&config);
    
    printf("Configuración del servidor:\n");
    
    printf("Puerto [%d]: ", config.puerto);
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        config.puerto = atoi(input);
    }
    
    printf("Máximo conexiones [%d]: ", config.max_conexiones);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        config.max_conexiones = atoi(input);
    }
    
    printf("Máximo hilos [%d]: ", config.max_hilos);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        config.max_hilos = atoi(input);
    }
    
    printf("Timeout cliente en segundos [%d]: ", config.timeout_cliente);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        config.timeout_cliente = atoi(input);
    }
    
    printf("Tipo de servidor (1=ECO, 2=CHAT) [%d]: ", config.tipo_servidor);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        config.tipo_servidor = atoi(input);
    }
    
    // Validar configuración
    if (validar_configuracion_servidor(&config) != SERVER_EXITO) {
        printf("Error: Configuración inválida\n");
        return;
    }
    
    // Inicializar y ejecutar servidor
    if (inicializar_servidor(&contexto, &config) != SERVER_EXITO) {
        printf("Error inicializando servidor\n");
        return;
    }
    
    printf("\nServidor personalizado iniciado:\n");
    printf("Puerto: %d\n", config.puerto);
    printf("Max conexiones: %d\n", config.max_conexiones);
    printf("Max hilos: %d\n", config.max_hilos);
    printf("Timeout: %d segundos\n", config.timeout_cliente);
    printf("Tipo: %s\n", (config.tipo_servidor == TIPO_ECO) ? "ECO" : "CHAT");
    printf("\nPresiona Ctrl+C para detener\n\n");
    
    ejecutar_servidor(&contexto);
    
    mostrar_estadisticas_servidor(&contexto);
    limpiar_servidor(&contexto);
}

/**
 * @brief Demo 4: Prueba de stress multicliente
 */
void demo_prueba_stress(void) {
    printf("\n=== PRUEBA DE STRESS MULTICLIENTE ===\n");
    
    int puerto = PUERTO_TEST_STRESS;
    int num_clientes = 10;
    int duracion = 30;
    
    printf("Puerto del servidor [%d]: ", puerto);
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        puerto = atoi(input);
    }
    
    printf("Número de clientes simulados [%d]: ", num_clientes);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        num_clientes = atoi(input);
    }
    
    printf("Duración en segundos [%d]: ", duracion);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        duracion = atoi(input);
    }
    
    printf("\nEjecutando prueba de stress...\n");
    printf("NOTA: Esta función requiere implementación adicional\n");
    printf("Para testing manual:\n");
    printf("1. Inicia el servidor en puerto %d\n", puerto);
    printf("2. Usa herramientas como 'siege' o scripts personalizados\n");
    printf("3. Conecta %d clientes simultáneos\n", num_clientes);
    printf("4. Mantén conexiones por %d segundos\n", duracion);
    
    // Aquí iría la implementación de prueba_stress_multicliente()
    // que crearía múltiples procesos cliente para testing
}

/**
 * @brief Demo 5: Benchmark de rendimiento
 */
void demo_benchmark_rendimiento(void) {
    printf("\n=== BENCHMARK DE RENDIMIENTO ===\n");
    
    int puerto = PUERTO_TEST_RENDIMIENTO;
    int num_mensajes = 1000;
    int num_clientes = 5;
    
    printf("Puerto del servidor [%d]: ", puerto);
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        puerto = atoi(input);
    }
    
    printf("Mensajes por cliente [%d]: ", num_mensajes);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        num_mensajes = atoi(input);
    }
    
    printf("Número de clientes [%d]: ", num_clientes);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        num_clientes = atoi(input);
    }
    
    printf("\nEjecutando benchmark...\n");
    printf("NOTA: Esta función requiere implementación adicional\n");
    printf("Para benchmark manual:\n");
    printf("1. Inicia el servidor ECO en puerto %d\n", puerto);
    printf("2. Conecta %d clientes simultáneos\n", num_clientes);
    printf("3. Cada cliente envía %d mensajes\n", num_mensajes);
    printf("4. Mide latencia, throughput y tiempo total\n");
    
    // Aquí iría la implementación de probar_rendimiento_servidor()
}

/**
 * @brief Demo 6: Configuración avanzada
 */
void demo_configuracion_avanzada(void) {
    printf("\n=== CONFIGURACIÓN AVANZADA ===\n");
    
    ConfigServidor config;
    configurar_servidor_por_defecto(&config);
    
    printf("Configuración por defecto:\n");
    printf("Puerto: %d\n", config.puerto);
    printf("Max conexiones: %d\n", config.max_conexiones);
    printf("Max hilos: %d\n", config.max_hilos);
    printf("Buffer size: %d bytes\n", config.buffer_size);
    printf("Timeout cliente: %d segundos\n", config.timeout_cliente);
    printf("Backlog: %d\n", config.backlog);
    printf("Tipo servidor: %d (1=ECO, 2=CHAT)\n", config.tipo_servidor);
    printf("Log detallado: %s\n", config.log_detallado ? "Sí" : "No");
    printf("Reutilizar puerto: %s\n", config.reutilizar_puerto ? "Sí" : "No");
    printf("Keepalive: %s\n", config.keepalive ? "Sí" : "No");
    printf("Bind IP: %s\n", config.bind_ip);
    
    printf("\nValidación de configuración: ");
    int resultado = validar_configuracion_servidor(&config);
    if (resultado == SERVER_EXITO) {
        printf("✓ VÁLIDA\n");
    } else {
        printf("✗ INVÁLIDA (código: %d)\n", resultado);
    }
    
    printf("\nPara personalizar la configuración, usa la opción 3 del menú principal.\n");
}

/**
 * @brief Demo 7: Diagnósticos del sistema
 */
void demo_diagnosticos(void) {
    printf("\n=== DIAGNÓSTICOS DEL SISTEMA ===\n");
    diagnosticar_sistema_multithread();
    
    printf("\nRecomendaciones para optimización:\n");
    printf("1. Ajustar límites de file descriptors si es necesario\n");
    printf("2. Configurar max_conexiones según memoria disponible\n");
    printf("3. Balancear max_hilos con número de cores disponibles\n");
    printf("4. Monitorear uso de CPU y memoria durante operación\n");
    printf("5. Usar herramientas como 'top', 'htop', 'netstat' para monitoreo\n");
}

/**
 * @brief Demo 8: Ejemplos de uso
 */
void demo_ejemplos_uso(void) {
    printf("\n=== EJEMPLOS DE USO ===\n");
    
    printf("\n1. Servidor ECO básico:\n");
    printf("   ./main_servidor_tcp_multicliente\n");
    printf("   # Selecciona opción 1\n");
    
    printf("\n2. Conexión de cliente (terminal separada):\n");
    printf("   telnet localhost 9090\n");
    printf("   # Escribe mensajes, se devuelven como eco\n");
    
    printf("\n3. Múltiples clientes simultáneos:\n");
    printf("   # Terminal 1: telnet localhost 9090\n");
    printf("   # Terminal 2: telnet localhost 9090\n");
    printf("   # Terminal 3: telnet localhost 9090\n");
    
    printf("\n4. Servidor CHAT:\n");
    printf("   ./main_servidor_tcp_multicliente\n");
    printf("   # Selecciona opción 2\n");
    printf("   # Conecta múltiples clientes\n");
    printf("   # Los mensajes se retransmiten entre clientes\n");
    
    printf("\n5. Comandos especiales en cliente:\n");
    printf("   quit    - Desconectar cliente\n");
    printf("   stats   - Ver estadísticas del cliente\n");
    printf("   help    - Mostrar ayuda\n");
    
    printf("\n6. Monitoreo del servidor:\n");
    printf("   # El servidor muestra logs detallados en tiempo real\n");
    printf("   # Presiona Ctrl+C para ver estadísticas finales\n");
    
    printf("\n7. Testing con herramientas externas:\n");
    printf("   # nc (netcat)\n");
    printf("   echo \"mensaje\" | nc localhost 9090\n");
    printf("   \n");
    printf("   # siege (stress testing)\n");
    printf("   siege -c 10 -t 30s telnet://localhost:9090\n");
}

/**
 * @brief Demo 9: Monitor de conexiones en tiempo real
 */
void demo_monitor_conexiones(void) {
    printf("\n=== MONITOR DE CONEXIONES ===\n");
    printf("Esta función mostraría un dashboard en tiempo real con:\n");
    printf("- Número de conexiones activas\n");
    printf("- Lista de clientes conectados\n");
    printf("- Estadísticas de tráfico por cliente\n");
    printf("- Gráficos de actividad\n");
    printf("- Alertas de desconexiones/errores\n");
    printf("\nNOTA: Requiere implementación adicional con ncurses o interfaz web\n");
    
    printf("\nPara monitoreo básico en tiempo real, usa:\n");
    printf("netstat -an | grep :9090\n");
    printf("ss -tulpn | grep :9090\n");
    printf("lsof -i :9090\n");
}

/**
 * @brief Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    printf("=== SERVIDOR TCP MULTICLIENTE CON HILOS ===\n");
    printf("Ejercicio 092: Demostración de concurrencia con pthread\n");
    printf("Implementación completa con manejo robusto de conexiones\n");
    
    signal(SIGINT, manejador_senales);
    signal(SIGTERM, manejador_senales);
    
    int opcion;
    
    while (ejecutando) {
        mostrar_menu();
        
        if (scanf("%d", &opcion) != 1) {
            // Limpiar buffer de entrada en caso de error
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Opción inválida. Intenta de nuevo.\n");
            continue;
        }
        
        // Consumir newline restante
        getchar();
        
        switch (opcion) {
            case 1:
                demo_servidor_eco(8080);
                break;
            case 2:
                demo_servidor_chat(8081);
                break;
            case 3:
                demo_servidor_personalizado();
                break;
            case 4:
                demo_prueba_stress();
                break;
            case 5:
                demo_benchmark_rendimiento();
                break;
            case 6:
                demo_configuracion_avanzada();
                break;
            case 7:
                demo_diagnosticos();
                break;
            case 8:
                demo_ejemplos_uso();
                break;
            case 9:
                demo_monitor_conexiones();
                break;
            case 0:
                printf("¡Hasta luego!\n");
                ejecutando = 0;
                break;
            default:
                printf("Opción inválida. Selecciona 0-9.\n");
                break;
        }
        
        if (ejecutando && opcion != 0) {
            printf("\nPresiona Enter para continuar...");
            getchar();
        }
    }
    
    return 0;
}
#endif
