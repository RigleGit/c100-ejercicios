/**
 * @file main.c
 * @brief Programa principal interactivo para el cliente TCP educativo
 * @description Menú interactivo que permite explorar diferentes aspectos
 *              de la programación con sockets TCP, desde conceptos básicos
 *              hasta demostraciones avanzadas y tests de rendimiento.
 * @version 1.0
 * @date 2024
 * @author Estudiante de C
 */

#include "../include/cliente_tcp.h"

/**
 * @brief Mostrar banner de bienvenida
 */
static void mostrar_banner(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    CLIENTE TCP EDUCATIVO                  ║\n");
    printf("║                         Versión %s                        ║\n", CLIENTE_TCP_VERSION);
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  Aprende programación de sockets TCP con ejemplos prácticos ║\n");
    printf("║  Incluye demostraciones, tests y análisis de rendimiento   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/**
 * @brief Mostrar menú principal
 */
static void mostrar_menu(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                        MENÚ PRINCIPAL                     ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  1. 🚀 Demostración básica                                 ║\n");
    printf("║  2. 💬 Cliente interactivo                                 ║\n");
    printf("║  3. 🔄 Test de reconexión automática                       ║\n");
    printf("║  4. 🛠️  Demostración de manejo de errores                  ║\n");
    printf("║  5. 🧪 Tests de conectividad y eco                        ║\n");
    printf("║  6. 📊 Benchmark de rendimiento                            ║\n");
    printf("║  7. 🎯 Test de stress                                      ║\n");
    printf("║  8. 🔧 Configuración personalizada                        ║\n");
    printf("║  9. 📚 Ayuda y conceptos                                   ║\n");
    printf("║  0. 🚪 Salir                                               ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\nSeleccione una opción: ");
}

/**
 * @brief Obtener configuración del servidor del usuario
 */
static int obtener_configuracion_servidor(char *servidor, int *puerto) {
    printf("\n📋 Configuración del servidor:\n");
    
    printf("Dirección del servidor [127.0.0.1]: ");
    fflush(stdout);
    
    if (fgets(servidor, 256, stdin)) {
        // Remover salto de línea
        size_t len = strlen(servidor);
        if (len > 0 && servidor[len - 1] == '\n') {
            servidor[len - 1] = '\0';
        }
        
        // Si está vacío, usar default
        if (strlen(servidor) == 0) {
            strcpy(servidor, "127.0.0.1");
        }
    } else {
        strcpy(servidor, "127.0.0.1");
    }
    
    printf("Puerto del servidor [8080]: ");
    fflush(stdout);
    
    char puerto_str[10];
    if (fgets(puerto_str, sizeof(puerto_str), stdin)) {
        if (strlen(puerto_str) > 1) { // Más que solo '\n'
            *puerto = atoi(puerto_str);
            if (*puerto <= 0 || *puerto > 65535) {
                printf("⚠️  Puerto inválido, usando puerto por defecto 8080\n");
                *puerto = PUERTO_DEFECTO;
            }
        } else {
            *puerto = PUERTO_DEFECTO;
        }
    } else {
        *puerto = PUERTO_DEFECTO;
    }
    
    printf("✅ Configurado para %s:%d\n\n", servidor, *puerto);
    return 1;
}

/**
 * @brief Mostrar ayuda y conceptos
 */
static void mostrar_ayuda(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    AYUDA Y CONCEPTOS                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\n🌐 CONCEPTOS FUNDAMENTALES:\n");
    printf("──────────────────────────────\n");
    printf("• Socket: Punto final de comunicación en red\n");
    printf("• TCP: Protocolo confiable, orientado a conexión\n");
    printf("• Cliente: Inicia la conexión al servidor\n");
    printf("• Servidor: Acepta conexiones de clientes\n");
    printf("• IP:Puerto: Dirección única en la red\n");
    
    printf("\n🔧 FUNCIONES BSD SOCKETS:\n");
    printf("─────────────────────────\n");
    printf("• socket(): Crear socket\n");
    printf("• connect(): Conectar al servidor\n");
    printf("• send(): Enviar datos\n");
    printf("• recv(): Recibir datos\n");
    printf("• close(): Cerrar socket\n");
    
    printf("\n📊 MÉTRICAS DE RENDIMIENTO:\n");
    printf("───────────────────────────\n");
    printf("• Latencia: Tiempo de ida y vuelta\n");
    printf("• Throughput: Datos por segundo\n");
    printf("• Bandwidth: Ancho de banda utilizado\n");
    printf("• Tasa de éxito: Porcentaje de transacciones exitosas\n");
    
    printf("\n🛠️  MANEJO DE ERRORES:\n");
    printf("──────────────────────\n");
    printf("• Timeouts: Evitar esperas infinitas\n");
    printf("• Reconexión: Reintento automático\n");
    printf("• Validación: Verificar parámetros\n");
    printf("• Limpieza: Liberar recursos apropiadamente\n");
    
    printf("\n🎯 CASOS DE USO COMUNES:\n");
    printf("────────────────────────\n");
    printf("• Servicios web (HTTP)\n");
    printf("• Bases de datos\n");
    printf("• Protocolos de chat\n");
    printf("• Transferencia de archivos\n");
    printf("• APIs REST\n");
    
    printf("\n💡 CONSEJOS DE PROGRAMACIÓN:\n");
    printf("────────────────────────────\n");
    printf("• Siempre verificar códigos de retorno\n");
    printf("• Manejar interrupciones (Ctrl+C)\n");
    printf("• Usar timeouts para evitar bloqueos\n");
    printf("• Limpiar recursos al terminar\n");
    printf("• Validar datos de entrada\n");
    
    printf("\n📚 PARA APRENDER MÁS:\n");
    printf("─────────────────────\n");
    printf("• RFC 793 (TCP Protocol)\n");
    printf("• UNIX Network Programming (Stevens)\n");
    printf("• man pages: socket(2), connect(2), send(2), recv(2)\n");
    printf("• Wireshark para análisis de tráfico\n");
    
    printf("\nPresione Enter para continuar...");
    getchar();
}

/**
 * @brief Configuración personalizada
 */
static void configuracion_personalizada(void) {
    char servidor[256];
    int puerto;
    
    if (!obtener_configuracion_servidor(servidor, &puerto)) {
        return;
    }
    
    printf("🔧 Configuración avanzada:\n");
    
    // Crear cliente
    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
    if (!cliente) {
        printf("❌ Error creando cliente\n");
        return;
    }
    
    cliente_config_t config = cliente->config;
    
    // Configurar timeouts
    printf("Timeout de conexión [%d segundos]: ", config.timeout_conexion);
    fflush(stdout);
    
    char input[50];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        int timeout = atoi(input);
        if (timeout > 0 && timeout <= 60) {
            config.timeout_conexion = timeout;
        }
    }
    
    printf("Timeout de recepción [%d segundos]: ", config.timeout_recv);
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        int timeout = atoi(input);
        if (timeout > 0 && timeout <= 60) {
            config.timeout_recv = timeout;
        }
    }
    
    printf("Intentos de reconexión [%d]: ", config.intentos_reconexion);
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        int intentos = atoi(input);
        if (intentos >= 0 && intentos <= 10) {
            config.intentos_reconexion = intentos;
        }
    }
    
    printf("Modo verbose (1=sí, 0=no) [%d]: ", config.modo_verbose);
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        int verbose = atoi(input);
        config.modo_verbose = (verbose != 0);
    }
    
    // Aplicar configuración
    cliente_configurar(cliente, &config);
    
    printf("\n✅ Configuración aplicada:\n");
    printf("   - Servidor: %s:%d\n", servidor, puerto);
    printf("   - Timeout conexión: %d segundos\n", config.timeout_conexion);
    printf("   - Timeout recepción: %d segundos\n", config.timeout_recv);
    printf("   - Intentos reconexión: %d\n", config.intentos_reconexion);
    printf("   - Modo verbose: %s\n", config.modo_verbose ? "Activado" : "Desactivado");
    
    // Realizar test básico con la configuración
    printf("\n🧪 Realizando test con configuración personalizada...\n");
    demo_cliente_basico(servidor, puerto);
    
    cliente_destruir(cliente);
}

/**
 * @brief Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    char servidor[256] = SERVIDOR_DEFECTO;
    int puerto = PUERTO_DEFECTO;
    
    // Configurar manejador de señales
    signal(SIGINT, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);
    
    mostrar_banner();
    
    printf("🎉 ¡Bienvenido al Cliente TCP Educativo!\n");
    printf("Este programa te ayudará a aprender programación de sockets TCP.\n");
    printf("\n💡 Para comenzar necesitas un servidor TCP ejecutándose.\n");
    printf("Puedes usar herramientas como netcat: nc -l 8080\n");
    printf("O crear tu propio servidor TCP.\n\n");
    
    // Obtener configuración inicial
    printf("🔧 Configuración inicial del servidor:\n");
    if (!obtener_configuracion_servidor(servidor, &puerto)) {
        printf("❌ Error en configuración, usando valores por defecto\n");
        strcpy(servidor, SERVIDOR_DEFECTO);
        puerto = PUERTO_DEFECTO;
    }
    
    while (1) {
        mostrar_menu();
        
        if (scanf("%d", &opcion) != 1) {
            printf("❌ Opción inválida. Intente nuevamente.\n\n");
            // Limpiar buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        // Limpiar buffer después de scanf
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        printf("\n");
        
        switch (opcion) {
            case 1:
                printf("🚀 Iniciando demostración básica...\n");
                demo_cliente_basico(servidor, puerto);
                break;
                
            case 2:
                printf("💬 Iniciando cliente interactivo...\n");
                demo_cliente_interactivo(servidor, puerto);
                break;
                
            case 3:
                printf("🔄 Iniciando test de reconexión...\n");
                demo_reconexion_automatica(servidor, puerto);
                break;
                
            case 4:
                printf("🛠️  Iniciando demostración de errores...\n");
                demo_manejo_errores(servidor, puerto);
                break;
                
            case 5: {
                printf("🧪 Iniciando tests de conectividad...\n");
                
                // Test de conectividad
                cliente_estado_t resultado = cliente_test_conectividad(servidor, puerto);
                
                if (resultado == CLIENTE_OK) {
                    // Test de eco si la conectividad funciona
                    cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
                    if (cliente) {
                        cliente_set_verbose(cliente, 1);
                        if (cliente_conectar(cliente) == CLIENTE_OK) {
                            cliente_test_eco(cliente, "Mensaje de prueba para test de eco");
                            cliente_desconectar(cliente);
                        }
                        cliente_destruir(cliente);
                    }
                }
                break;
            }
            
            case 6: {
                printf("📊 Iniciando benchmark de rendimiento...\n");
                
                cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
                if (cliente) {
                    cliente_set_verbose(cliente, 1);
                    if (cliente_conectar(cliente) == CLIENTE_OK) {
                        printf("Configuración del benchmark:\n");
                        
                        printf("Número de mensajes [1000]: ");
                        fflush(stdout);
                        
                        char input[50];
                        int num_mensajes = 1000;
                        if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
                            int num = atoi(input);
                            if (num > 0 && num <= 100000) {
                                num_mensajes = num;
                            }
                        }
                        
                        printf("Tamaño de mensaje [100 bytes]: ");
                        fflush(stdout);
                        
                        size_t tamaño_mensaje = 100;
                        if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
                            int tamaño = atoi(input);
                            if (tamaño > 0 && tamaño <= 4096) {
                                tamaño_mensaje = tamaño;
                            }
                        }
                        
                        cliente_benchmark(cliente, num_mensajes, tamaño_mensaje);
                        cliente_desconectar(cliente);
                    } else {
                        printf("❌ No se pudo conectar para el benchmark\n");
                    }
                    cliente_destruir(cliente);
                }
                break;
            }
            
            case 7: {
                printf("🎯 Iniciando test de stress...\n");
                
                cliente_tcp_t *cliente = cliente_crear(servidor, puerto);
                if (cliente) {
                    cliente_set_verbose(cliente, 0); // Menos verbose para stress
                    if (cliente_conectar(cliente) == CLIENTE_OK) {
                        printf("Configuración del test de stress:\n");
                        
                        printf("Número de transacciones [500]: ");
                        fflush(stdout);
                        
                        char input[50];
                        int num_transacciones = 500;
                        if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
                            int num = atoi(input);
                            if (num > 0 && num <= 10000) {
                                num_transacciones = num;
                            }
                        }
                        
                        cliente_test_stress(cliente, num_transacciones, "Stress test message");
                        cliente_desconectar(cliente);
                    } else {
                        printf("❌ No se pudo conectar para el test de stress\n");
                    }
                    cliente_destruir(cliente);
                }
                break;
            }
            
            case 8:
                printf("🔧 Configuración personalizada...\n");
                configuracion_personalizada();
                break;
                
            case 9:
                mostrar_ayuda();
                break;
                
            case 0:
                printf("👋 ¡Gracias por usar el Cliente TCP Educativo!\n");
                printf("🎓 Esperamos que hayas aprendido sobre sockets TCP.\n");
                printf("📚 Continúa practicando y explorando la programación en red.\n\n");
                return 0;
                
            default:
                printf("❌ Opción inválida. Seleccione un número del 0 al 9.\n\n");
                break;
        }
        
        printf("\nPresione Enter para continuar...");
        getchar();
        printf("\n");
    }
    
    return 0;
}
#endif
