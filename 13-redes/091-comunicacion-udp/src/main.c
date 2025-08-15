/**
 * @file main.c
 * @brief Programa principal de demostración de comunicación UDP
 * @author Autor: Tu Nombre
 * @date 2024
 * 
 * Este programa demuestra las capacidades de comunicación UDP implementadas
 * en comunicacion_udp.c, incluyendo envío, recepción, configuración avanzada
 * y análisis de rendimiento.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "../include/comunicacion_udp.h"

// Variable global para control de ejecución
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
    printf("\n=== DEMOSTRACIÓN COMUNICACIÓN UDP ===\n");
    printf("1. Crear emisor UDP\n");
    printf("2. Crear receptor UDP\n");
    printf("3. Demo básico (emisor + receptor)\n");
    printf("4. Demo eco (cliente-servidor)\n");
    printf("5. Demo broadcast\n");
    printf("6. Demo multicast\n");
    printf("7. Prueba de rendimiento\n");
    printf("8. Configuración y diagnósticos\n");
    printf("9. Ejemplos de uso\n");
    printf("0. Salir\n");
    printf("=====================================\n");
    printf("Selecciona una opción: ");
}

/**
 * @brief Demo 1: Crear emisor UDP
 */
void demo_emisor(void) {
    printf("\n=== DEMO EMISOR UDP ===\n");
    
    char ip[64] = "127.0.0.1";
    int puerto = 9090;
    char mensaje[256] = "Hola desde el emisor UDP";
    
    printf("Configuración del emisor:\n");
    printf("IP destino [%s]: ", ip);
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        input[strcspn(input, "\n")] = 0;
        strncpy(ip, input, sizeof(ip) - 1);
    }
    
    printf("Puerto destino [%d]: ", puerto);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        puerto = atoi(input);
    }
    
    printf("Mensaje [%s]: ", mensaje);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        input[strcspn(input, "\n")] = 0;
        strncpy(mensaje, input, sizeof(mensaje) - 1);
    }
    
    ConfigUDP config = {0};
    strncpy(config.direccion_ip, ip, sizeof(config.direccion_ip) - 1);
    config.puerto = puerto;
    config.timeout_segundos = 5;
    config.buffer_size = 1024;
    
    printf("\nCreando emisor UDP...\n");
    int sockfd = crear_socket_udp(&config);
    if (sockfd < 0) {
        printf("Error creando socket emisor\n");
        return;
    }
    
    printf("Enviando mensaje...\n");
    if (enviar_datagrama(sockfd, &config, mensaje, strlen(mensaje)) > 0) {
        printf("✓ Mensaje enviado exitosamente\n");
    } else {
        printf("✗ Error enviando mensaje\n");
    }
    
    close(sockfd);
    printf("Emisor cerrado\n");
}

/**
 * @brief Demo 2: Crear receptor UDP
 */
void demo_receptor(void) {
    printf("\n=== DEMO RECEPTOR UDP ===\n");
    
    int puerto = 9090;
    printf("Puerto de escucha [%d]: ", puerto);
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        puerto = atoi(input);
    }
    
    ConfigUDP config = {0};
    config.puerto = puerto;
    config.timeout_segundos = 30;
    config.buffer_size = 1024;
    
    printf("\nCreando receptor UDP en puerto %d...\n", puerto);
    int sockfd = crear_socket_udp(&config);
    if (sockfd < 0) {
        printf("Error creando socket receptor\n");
        return;
    }
    
    if (configurar_receptor(sockfd, &config) < 0) {
        printf("Error configurando receptor\n");
        close(sockfd);
        return;
    }
    
    printf("Receptor activo. Esperando mensajes (Ctrl+C para salir)...\n");
    
    char buffer[1024];
    EstadisticasUDP stats = {0};
    
    signal(SIGINT, manejador_senales);
    ejecutando = 1;
    
    while (ejecutando) {
        ssize_t bytes = recibir_datagrama(sockfd, &config, buffer, sizeof(buffer) - 1, &stats);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Recibido: \"%s\" (%zd bytes) de %s:%d\n", 
                   buffer, bytes, config.direccion_ip, config.puerto_remoto);
        } else if (bytes == 0) {
            printf("Timeout en recepción\n");
        }
    }
    
    close(sockfd);
    printf("\nReceptor cerrado\n");
    mostrar_estadisticas(&stats);
}

/**
 * @brief Demo 3: Demo básico con emisor y receptor
 */
void demo_basico(void) {
    printf("\n=== DEMO BÁSICO UDP ===\n");
    printf("Iniciando demo con emisor y receptor en procesos separados...\n");
    
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo - Receptor
        printf("Proceso receptor iniciado\n");
        
        ConfigUDP config = {0};
        config.puerto = 9091;
        config.timeout_segundos = 10;
        config.buffer_size = 1024;
        
        int sockfd = crear_socket_udp(&config);
        if (sockfd < 0) {
            exit(1);
        }
        
        if (configurar_receptor(sockfd, &config) < 0) {
            close(sockfd);
            exit(1);
        }
        
        printf("Receptor esperando en puerto %d...\n", config.puerto);
        
        char buffer[1024];
        EstadisticasUDP stats = {0};
        
        for (int i = 0; i < 3; i++) {
            ssize_t bytes = recibir_datagrama(sockfd, &config, buffer, sizeof(buffer) - 1, &stats);
            if (bytes > 0) {
                buffer[bytes] = '\0';
                printf("Receptor: \"%s\"\n", buffer);
            }
        }
        
        close(sockfd);
        exit(0);
    } else if (pid > 0) {
        // Proceso padre - Emisor
        sleep(1); // Dar tiempo al receptor para iniciarse
        
        printf("Proceso emisor iniciado\n");
        
        ConfigUDP config = {0};
        strncpy(config.direccion_ip, "127.0.0.1", sizeof(config.direccion_ip) - 1);
        config.puerto = 9091;
        config.timeout_segundos = 5;
        config.buffer_size = 1024;
        
        int sockfd = crear_socket_udp(&config);
        if (sockfd < 0) {
            kill(pid, SIGTERM);
            wait(NULL);
            return;
        }
        
        const char* mensajes[] = {
            "Primer mensaje UDP",
            "Segundo mensaje UDP", 
            "Tercer mensaje UDP"
        };
        
        for (int i = 0; i < 3; i++) {
            printf("Enviando: \"%s\"\n", mensajes[i]);
            enviar_datagrama(sockfd, &config, mensajes[i], strlen(mensajes[i]));
            sleep(1);
        }
        
        close(sockfd);
        wait(NULL);
        printf("Demo básico completado\n");
    } else {
        perror("Error en fork");
    }
}

/**
 * @brief Demo 4: Demo eco cliente-servidor
 */
void demo_eco(void) {
    printf("\n=== DEMO ECO UDP ===\n");
    printf("Este demo requiere que ejecutes manualmente:\n");
    printf("Terminal 1: ./tools/receptor_udp -e 9092\n");
    printf("Terminal 2: ./tools/emisor_udp 127.0.0.1 9092 \"mensaje de prueba\"\n");
    printf("\nPresiona Enter para ejecutar un cliente de prueba automático...");
    getchar();
    
    // Ejecutar demo eco usando las funciones de la librería
    demo_eco_udp();
}

/**
 * @brief Demo 5: Demo broadcast
 */
void demo_broadcast(void) {
    printf("\n=== DEMO BROADCAST UDP ===\n");
    printf("Ejecutando demo de broadcast...\n");
    demo_broadcast_udp();
}

/**
 * @brief Demo 6: Demo multicast
 */
void demo_multicast(void) {
    printf("\n=== DEMO MULTICAST UDP ===\n");
    printf("Ejecutando demo de multicast...\n");
    demo_multicast_udp();
}

/**
 * @brief Demo 7: Prueba de rendimiento
 */
void demo_rendimiento(void) {
    printf("\n=== PRUEBA DE RENDIMIENTO UDP ===\n");
    printf("Ejecutando pruebas de rendimiento...\n");
    
    // Configurar parámetros de prueba
    int num_mensajes = 1000;
    int tamaño_mensaje = 512;
    
    printf("Número de mensajes [%d]: ", num_mensajes);
    char input[256];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        num_mensajes = atoi(input);
    }
    
    printf("Tamaño de mensaje [%d]: ", tamaño_mensaje);
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        tamaño_mensaje = atoi(input);
    }
    
    probar_rendimiento_udp(num_mensajes, tamaño_mensaje);
}

/**
 * @brief Demo 8: Configuración y diagnósticos
 */
void demo_configuracion(void) {
    printf("\n=== CONFIGURACIÓN Y DIAGNÓSTICOS ===\n");
    
    ConfigUDP config = {0};
    configurar_udp_por_defecto(&config);
    
    printf("Configuración por defecto:\n");
    printf("IP: %s\n", config.direccion_ip);
    printf("Puerto: %d\n", config.puerto);
    printf("Timeout: %d segundos\n", config.timeout_segundos);
    printf("Buffer: %d bytes\n", config.buffer_size);
    printf("Reutilizar dirección: %s\n", config.reutilizar_direccion ? "Sí" : "No");
    printf("Broadcast: %s\n", config.habilitar_broadcast ? "Sí" : "No");
    
    printf("\nEjecutando diagnósticos del sistema...\n");
    diagnosticar_sistema_udp();
}

/**
 * @brief Demo 9: Ejemplos de uso
 */
void demo_ejemplos(void) {
    printf("\n=== EJEMPLOS DE USO ===\n");
    printf("\n1. Emisor básico:\n");
    printf("   ./tools/emisor_udp 127.0.0.1 9090 \"Hola UDP\"\n");
    
    printf("\n2. Receptor básico:\n");
    printf("   ./tools/receptor_udp 9090\n");
    
    printf("\n3. Emisor con múltiples mensajes:\n");
    printf("   ./tools/emisor_udp -n 5 -i 1000 127.0.0.1 9090 \"Mensaje\"\n");
    
    printf("\n4. Receptor con eco:\n");
    printf("   ./tools/receptor_udp -e -v 9090\n");
    
    printf("\n5. Receptor con estadísticas:\n");
    printf("   ./tools/receptor_udp -s -t 60 9090\n");
    
    printf("\n6. Emisor verboso:\n");
    printf("   ./tools/emisor_udp -v -t 10 192.168.1.100 8080 \"Datos\"\n");
    
    printf("\nConsulta los README.md de cada ejercicio para más ejemplos.\n");
}

/**
 * @brief Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    printf("=== COMUNICACIÓN UDP EN C ===\n");
    printf("Ejercicio 091: Demostración de comunicación UDP\n");
    printf("Implementación completa con emisor, receptor y utilidades\n");
    
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
                demo_emisor();
                break;
            case 2:
                demo_receptor();
                break;
            case 3:
                demo_basico();
                break;
            case 4:
                demo_eco();
                break;
            case 5:
                demo_broadcast();
                break;
            case 6:
                demo_multicast();
                break;
            case 7:
                demo_rendimiento();
                break;
            case 8:
                demo_configuracion();
                break;
            case 9:
                demo_ejemplos();
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
