/**
 * @file main.c
 * @brief Programa principal del servidor TCP de eco - Ejercicio 090
 * @author Ejercicios de C
 * @date 2025
 */

#include "../include/servidor_tcp.h"

// ============================================================================
// FUNCIONES DE UTILIDAD PARA EL MENÚ
// ============================================================================

/**
 * @brief Imprimir el menú principal
 */
static void imprimir_menu(void) {
    printf("\n=== SERVIDOR TCP DE ECO - EJERCICIO 090 ===\n");
    printf("1. Servidor básico (modo secuencial)\n");
    printf("2. Servidor con fork (proceso por cliente)\n");
    printf("3. Servidor con threads (hilo por cliente)\n");
    printf("4. Servidor con select (I/O multiplexing)\n");
    printf("5. Configurar servidor personalizado\n");
    printf("6. Ejecutar stress test\n");
    printf("7. Demo de rendimiento\n");
    printf("8. Información sobre TCP y sockets\n");
    printf("9. Salir\n");
    printf("==========================================\n");
    printf("Selecciona una opción (1-9): ");
}

/**
 * @brief Imprimir información educativa sobre TCP y sockets
 */
static void imprimir_info_tcp(void) {
    printf("\n=== INFORMACIÓN SOBRE TCP Y SOCKETS ===\n\n");
    
    printf("📡 PROTOCOLO TCP (Transmission Control Protocol):\n");
    printf("   • Protocolo orientado a conexión\n");
    printf("   • Garantiza entrega y orden de los datos\n");
    printf("   • Control de flujo y congestión\n");
    printf("   • Handshake de tres vías para establecer conexión\n\n");
    
    printf("🔌 SOCKETS BSD:\n");
    printf("   • socket(): Crear endpoint de comunicación\n");
    printf("   • bind(): Asociar socket con dirección local\n");
    printf("   • listen(): Poner socket en modo escucha\n");
    printf("   • accept(): Aceptar conexión entrante\n");
    printf("   • recv(): Recibir datos del cliente\n");
    printf("   • send(): Enviar datos al cliente\n");
    printf("   • close(): Cerrar socket\n\n");
    
    printf("🏗️ ARQUITECTURA SERVIDOR:\n");
    printf("   • SECUENCIAL: Un cliente a la vez (simple, lento)\n");
    printf("   • FORK: Proceso por cliente (robusto, recursos)\n");
    printf("   • THREAD: Hilo por cliente (eficiente, sincronización)\n");
    printf("   • SELECT: I/O multiplexing (escalable, complejo)\n\n");
    
    printf("📊 CONSIDERACIONES DE RENDIMIENTO:\n");
    printf("   • Número máximo de conexiones simultáneas\n");
    printf("   • Tamaño del buffer de recepción\n");
    printf("   • Timeouts para evitar bloqueos\n");
    printf("   • Gestión de memoria y recursos\n");
    printf("   • Manejo graceful de señales\n\n");
    
    printf("🔧 OPCIONES DE SOCKET:\n");
    printf("   • SO_REUSEADDR: Reutilizar dirección inmediatamente\n");
    printf("   • SO_KEEPALIVE: Mantener conexión viva\n");
    printf("   • TCP_NODELAY: Deshabilitar algoritmo de Nagle\n");
    printf("   • SO_RCVTIMEO/SO_SNDTIMEO: Timeouts de E/S\n\n");
    
    printf("⚠️ MANEJO DE ERRORES:\n");
    printf("   • Verificar valores de retorno de todas las funciones\n");
    printf("   • Usar perror() para diagnósticos detallados\n");
    printf("   • Limpiar recursos en caso de error\n");
    printf("   • Manejar señales (SIGINT, SIGTERM, SIGCHLD)\n\n");
    
    printf("📝 PROTOCOLO DE ECO:\n");
    printf("   1. Cliente se conecta al servidor\n");
    printf("   2. Cliente envía mensaje\n");
    printf("   3. Servidor recibe mensaje\n");
    printf("   4. Servidor envía de vuelta el mismo mensaje\n");
    printf("   5. Cliente recibe la respuesta\n");
    printf("   6. Repetir desde paso 2 o cerrar conexión\n\n");
}

/**
 * @brief Configurar servidor de forma interactiva
 */
static config_servidor_t configurar_servidor_interactivo(void) {
    config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
    char buffer[256];
    int opcion;
    
    printf("\n=== CONFIGURACIÓN DEL SERVIDOR ===\n");
    
    printf("Puerto (actual %d): ", config.puerto);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        config.puerto = atoi(buffer);
        if (config.puerto <= 0 || config.puerto > 65535) {
            printf("Puerto inválido, usando %d\n", PUERTO_DEFECTO);
            config.puerto = PUERTO_DEFECTO;
        }
    }
    
    printf("Máximo de clientes simultáneos (actual %d): ", config.max_clientes);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        config.max_clientes = atoi(buffer);
        if (config.max_clientes <= 0) {
            printf("Valor inválido, usando %d\n", MAX_CLIENTES);
            config.max_clientes = MAX_CLIENTES;
        }
    }
    
    printf("Timeout por cliente en segundos (actual %d): ", config.timeout_cliente_seg);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        config.timeout_cliente_seg = atoi(buffer);
        if (config.timeout_cliente_seg < 0) {
            printf("Valor inválido, usando %d\n", TIMEOUT_CLIENTE_SEG);
            config.timeout_cliente_seg = TIMEOUT_CLIENTE_SEG;
        }
    }
    
    printf("Tamaño del buffer (actual %d): ", config.buffer_size);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        config.buffer_size = atoi(buffer);
        if (config.buffer_size <= 0) {
            printf("Valor inválido, usando %d\n", BUFFER_MAXIMO);
            config.buffer_size = BUFFER_MAXIMO;
        }
    }
    
    printf("\nModos de operación:\n");
    printf("1. Secuencial (un cliente a la vez)\n");
    printf("2. Fork (proceso por cliente)\n");
    printf("3. Thread (hilo por cliente)\n");
    printf("4. Select (I/O multiplexing)\n");
    printf("Seleccionar modo (1-4): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        opcion = atoi(buffer);
        switch (opcion) {
            case 1: config.modo = MODO_SECUENCIAL; break;
            case 2: config.modo = MODO_FORK; break;
            case 3: config.modo = MODO_THREAD; break;
            case 4: config.modo = MODO_SELECT; break;
            default:
                printf("Modo inválido, usando SECUENCIAL\n");
                config.modo = MODO_SECUENCIAL;
        }
    }
    
    printf("Habilitar logs detallados? (s/N): ");
    if (fgets(buffer, sizeof(buffer), stdin) && 
        (buffer[0] == 's' || buffer[0] == 'S')) {
        config.verbose = 1;
    }
    
    printf("Habilitar TCP_NODELAY? (s/N): ");
    if (fgets(buffer, sizeof(buffer), stdin) && 
        (buffer[0] == 's' || buffer[0] == 'S')) {
        config.no_delay = 1;
    }
    
    return config;
}

/**
 * @brief Ejecutar stress test interactivo
 */
static void ejecutar_stress_test(void) {
    printf("\n=== STRESS TEST ===\n");
    printf("ATENCIÓN: Asegúrate de que hay un servidor ejecutándose\n");
    printf("en otro terminal antes de continuar.\n\n");
    
    char buffer[256];
    char host[256] = "127.0.0.1";
    int puerto = PUERTO_DEFECTO;
    int num_clientes = 10;
    int mensajes_por_cliente = 5;
    
    printf("Host del servidor (actual %s): ", host);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        strncpy(host, buffer, sizeof(host) - 1);
        host[strlen(host) - 1] = '\0'; // Quitar \n
    }
    
    printf("Puerto del servidor (actual %d): ", puerto);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        puerto = atoi(buffer);
        if (puerto <= 0 || puerto > 65535) {
            printf("Puerto inválido, usando %d\n", PUERTO_DEFECTO);
            puerto = PUERTO_DEFECTO;
        }
    }
    
    printf("Número de clientes concurrentes (actual %d): ", num_clientes);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        num_clientes = atoi(buffer);
        if (num_clientes <= 0 || num_clientes > 100) {
            printf("Valor inválido, usando 10\n");
            num_clientes = 10;
        }
    }
    
    printf("Mensajes por cliente (actual %d): ", mensajes_por_cliente);
    if (fgets(buffer, sizeof(buffer), stdin) && strlen(buffer) > 1) {
        mensajes_por_cliente = atoi(buffer);
        if (mensajes_por_cliente <= 0 || mensajes_por_cliente > 100) {
            printf("Valor inválido, usando 5\n");
            mensajes_por_cliente = 5;
        }
    }
    
    printf("\nEjecutando stress test...\n");
    time_t inicio = time(NULL);
    
    resultado_servidor_t resultado = stress_test_clientes(host, puerto, 
                                                        num_clientes, mensajes_por_cliente);
    
    time_t fin = time(NULL);
    
    if (resultado == SERVIDOR_EXITO) {
        printf("✅ Stress test completado exitosamente\n");
        printf("Tiempo total: %ld segundos\n", fin - inicio);
        printf("Total de transacciones: %d\n", num_clientes * mensajes_por_cliente);
        printf("Transacciones por segundo: %.2f\n", 
               (double)(num_clientes * mensajes_por_cliente) / (fin - inicio));
    } else {
        printf("❌ Error en stress test: %s\n", servidor_strerror(resultado));
    }
}

/**
 * @brief Ejecutar servidor con configuración específica
 */
static void ejecutar_servidor_con_config(const config_servidor_t* config) {
    printf("\n=== INICIANDO SERVIDOR ===\n");
    
    servidor_tcp_t* servidor = crear_servidor(config);
    if (!servidor) {
        fprintf(stderr, "❌ Error creando servidor\n");
        return;
    }
    
    resultado_servidor_t resultado = iniciar_servidor(servidor);
    if (resultado != SERVIDOR_EXITO) {
        fprintf(stderr, "❌ Error iniciando servidor: %s\n", servidor_strerror(resultado));
        destruir_servidor(servidor);
        return;
    }
    
    printf("✅ Servidor iniciado correctamente\n");
    printf("🌐 Dirección: %s:%d\n", config->host, config->puerto);
    printf("📊 Modo: %s\n", 
           config->modo == MODO_SECUENCIAL ? "SECUENCIAL" :
           config->modo == MODO_FORK ? "FORK" :
           config->modo == MODO_THREAD ? "THREAD" :
           config->modo == MODO_SELECT ? "SELECT" : "DESCONOCIDO");
    printf("👥 Max clientes: %d\n", config->max_clientes);
    printf("⏱️ Timeout: %d segundos\n", config->timeout_cliente_seg);
    printf("\n📡 Prueba la conexión con:\n");
    printf("   telnet %s %d\n", strcmp(config->host, "0.0.0.0") == 0 ? "localhost" : config->host, config->puerto);
    printf("   nc %s %d\n", strcmp(config->host, "0.0.0.0") == 0 ? "localhost" : config->host, config->puerto);
    printf("\n🛑 Presiona Ctrl+C para detener el servidor\n");
    printf("================================================\n\n");
    
    // Ejecutar servidor
    resultado = ejecutar_servidor(servidor);
    
    if (resultado == SERVIDOR_EXITO || resultado == SERVIDOR_SHUTDOWN) {
        printf("\n✅ Servidor detenido correctamente\n");
    } else {
        printf("\n❌ Servidor terminó con error: %s\n", servidor_strerror(resultado));
    }
    
    // Mostrar estadísticas finales
    imprimir_estadisticas_servidor(servidor, NULL);
    
    destruir_servidor(servidor);
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    char buffer[256];
    
    printf("🚀 Servidor TCP de Eco - Ejercicio 090\n");
    printf("Este programa implementa un servidor TCP que devuelve (eco)\n");
    printf("los mensajes recibidos de los clientes.\n");
    
    while (1) {
        imprimir_menu();
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError leyendo entrada. Saliendo...\n");
            break;
        }
        
        opcion = atoi(buffer);
        
        switch (opcion) {
            case 1: {
                printf("\n--- Ejecutando servidor básico (modo secuencial) ---\n");
                config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
                config.modo = MODO_SECUENCIAL;
                config.verbose = 1;
                ejecutar_servidor_con_config(&config);
                break;
            }
            
            case 2: {
                printf("\n--- Ejecutando servidor con fork ---\n");
                config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
                config.modo = MODO_FORK;
                config.max_clientes = 5;
                config.verbose = 1;
                ejecutar_servidor_con_config(&config);
                break;
            }
            
            case 3: {
                printf("\n--- Ejecutando servidor con threads ---\n");
                config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
                config.modo = MODO_THREAD;
                config.max_clientes = 10;
                config.verbose = 1;
                ejecutar_servidor_con_config(&config);
                break;
            }
            
            case 4: {
                printf("\n--- Ejecutando servidor con select ---\n");
                config_servidor_t config = CONFIG_SERVIDOR_DEFECTO;
                config.modo = MODO_SELECT;
                config.max_clientes = 20;
                config.verbose = 1;
                ejecutar_servidor_con_config(&config);
                break;
            }
            
            case 5: {
                printf("\n--- Configuración personalizada ---\n");
                config_servidor_t config = configurar_servidor_interactivo();
                ejecutar_servidor_con_config(&config);
                break;
            }
            
            case 6: {
                ejecutar_stress_test();
                break;
            }
            
            case 7: {
                printf("\n--- Demo de rendimiento ---\n");
                demo_servidor_rendimiento();
                break;
            }
            
            case 8: {
                imprimir_info_tcp();
                printf("Presiona Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
                break;
            }
            
            case 9: {
                printf("\n👋 ¡Gracias por usar el servidor TCP de eco!\n");
                printf("📚 Has aprendido sobre:\n");
                printf("   • Sockets de servidor BSD\n");
                printf("   • Diferentes arquitecturas de servidor\n");
                printf("   • Manejo de múltiples clientes\n");
                printf("   • Protocolo TCP y comunicación de red\n");
                printf("   • Gestión de recursos y errores\n\n");
                printf("🎯 Próximos pasos sugeridos:\n");
                printf("   • Implementar un servidor UDP\n");
                printf("   • Agregar cifrado TLS/SSL\n");
                printf("   • Crear un protocolo personalizado\n");
                printf("   • Implementar un servidor HTTP básico\n\n");
                return 0;
            }
            
            default: {
                printf("❌ Opción inválida. Por favor selecciona 1-9.\n");
                break;
            }
        }
        
        printf("\nPresiona Enter para continuar...");
        fgets(buffer, sizeof(buffer), stdin);
    }
    
    return 0;
}
#endif
