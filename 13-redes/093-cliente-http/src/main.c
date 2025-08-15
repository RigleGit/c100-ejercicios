/**
 * @file main.c
 * @brief Programa principal del cliente HTTP
 */

#include "cliente_http.h"

/**
 * @brief Muestra el menú principal
 */
void mostrar_menu(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    CLIENTE HTTP - Menú Principal             ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Petición GET básica                                      ║\n");
    printf("║  2. Petición GET con estadísticas detalladas                ║\n");
    printf("║  3. Petición GET solo headers                                ║\n");
    printf("║  4. Múltiples peticiones                                     ║\n");
    printf("║  5. Benchmark de rendimiento                                 ║\n");
    printf("║  6. Demo con URLs predefinidas                               ║\n");
    printf("║  7. Analizar URL                                             ║\n");
    printf("║  8. Configuración personalizada                              ║\n");
    printf("║  9. Ayuda y ejemplos                                         ║\n");
    printf("║  0. Salir                                                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("Selecciona una opción: ");
}

/**
 * @brief Demo de petición GET básica
 */
void demo_get_basico(void) {
    char url[MAX_URL_LENGTH];
    
    printf("\n=== Petición GET Básica ===\n");
    printf("Introduce la URL (ej: http://example.com): ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        return;
    }
    
    // Quitar salto de línea
    url[strcspn(url, "\n")] = '\0';
    
    if (strlen(url) == 0) {
        strcpy(url, "http://example.com");
        printf("Usando URL por defecto: %s\n", url);
    }
    
    printf("\nRealizando petición...\n");
    cliente_http_demo_get_basico(url);
}

/**
 * @brief Demo con estadísticas detalladas
 */
void demo_con_estadisticas(void) {
    char url[MAX_URL_LENGTH];
    
    printf("\n=== Petición GET con Estadísticas ===\n");
    printf("Introduce la URL: ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        return;
    }
    
    url[strcspn(url, "\n")] = '\0';
    
    if (strlen(url) == 0) {
        strcpy(url, "http://httpbin.org/get");
        printf("Usando URL por defecto: %s\n", url);
    }
    
    printf("\nRealizando petición con estadísticas...\n");
    cliente_http_demo_con_estadisticas(url);
}

/**
 * @brief Demo solo headers (método HEAD simulado)
 */
void demo_solo_headers(void) {
    char url[MAX_URL_LENGTH];
    peticion_http_t peticion;
    respuesta_http_t respuesta;
    int resultado;
    
    printf("\n=== Solo Headers (HEAD) ===\n");
    printf("Introduce la URL: ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        return;
    }
    
    url[strcspn(url, "\n")] = '\0';
    
    if (strlen(url) == 0) {
        strcpy(url, "http://httpbin.org/get");
        printf("Usando URL por defecto: %s\n", url);
    }
    
    cliente_http_init_peticion(&peticion);
    peticion.metodo = HTTP_HEAD;
    
    // Parsear URL
    url_parseada_t url_parseada;
    resultado = cliente_http_parsear_url(url, &url_parseada);
    if (resultado != CLIENTE_HTTP_OK) {
        printf("Error al parsear URL: %s\n", cliente_http_error_string(resultado));
        return;
    }
    
    strcpy(peticion.host, url_parseada.host);
    strcpy(peticion.puerto, url_parseada.puerto);
    strcpy(peticion.path, url_parseada.path);
    
    printf("\nRealizando petición HEAD...\n");
    resultado = cliente_http_realizar_peticion(&peticion, &respuesta, NULL);
    
    if (resultado == CLIENTE_HTTP_OK) {
        cliente_http_imprimir_headers(&respuesta);
        cliente_http_liberar_respuesta(&respuesta);
    } else {
        printf("Error: %s\n", cliente_http_error_string(resultado));
    }
}

/**
 * @brief Demo múltiples peticiones
 */
void demo_multiples_peticiones(void) {
    char input[256];
    int num_urls;
    
    printf("\n=== Múltiples Peticiones ===\n");
    printf("¿Cuántas URLs quieres probar? (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    num_urls = atoi(input);
    if (num_urls < 1 || num_urls > 10) {
        num_urls = 3;
        printf("Usando 3 URLs por defecto\n");
    }
    
    const char* urls[10];
    char url_buffers[10][MAX_URL_LENGTH];
    
    for (int i = 0; i < num_urls; i++) {
        printf("URL %d: ", i + 1);
        if (fgets(url_buffers[i], sizeof(url_buffers[i]), stdin) == NULL) {
            break;
        }
        url_buffers[i][strcspn(url_buffers[i], "\n")] = '\0';
        
        if (strlen(url_buffers[i]) == 0) {
            // URLs por defecto
            const char* defaults[] = {
                "http://example.com",
                "http://httpbin.org/get",
                "http://httpbin.org/user-agent",
                "http://httpbin.org/headers",
                "http://httpbin.org/ip"
            };
            strcpy(url_buffers[i], defaults[i % 5]);
            printf("Usando: %s\n", url_buffers[i]);
        }
        
        urls[i] = url_buffers[i];
    }
    
    printf("\nRealizando múltiples peticiones...\n");
    cliente_http_demo_multiples_peticiones(urls, num_urls);
}

/**
 * @brief Demo benchmark
 */
void demo_benchmark(void) {
    char url[MAX_URL_LENGTH];
    char input[256];
    int num_peticiones;
    
    printf("\n=== Benchmark de Rendimiento ===\n");
    printf("Introduce la URL: ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        return;
    }
    
    url[strcspn(url, "\n")] = '\0';
    
    if (strlen(url) == 0) {
        strcpy(url, "http://httpbin.org/get");
        printf("Usando URL por defecto: %s\n", url);
    }
    
    printf("Número de peticiones (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    num_peticiones = atoi(input);
    if (num_peticiones < 1 || num_peticiones > 100) {
        num_peticiones = 10;
        printf("Usando 10 peticiones por defecto\n");
    }
    
    printf("\n⚠️  Realizando %d peticiones, esto puede tomar tiempo...\n", num_peticiones);
    cliente_http_benchmark(url, num_peticiones);
}

/**
 * @brief Demo con URLs predefinidas
 */
void demo_urls_predefinidas(void) {
    printf("\n=== Demo con URLs Predefinidas ===\n");
    
    const char* urls[] = {
        "http://example.com",
        "http://httpbin.org/get",
        "http://httpbin.org/user-agent",
        "http://httpbin.org/headers"
    };
    
    int num_urls = sizeof(urls) / sizeof(urls[0]);
    
    printf("Probando %d URLs predefinidas...\n", num_urls);
    cliente_http_demo_multiples_peticiones(urls, num_urls);
}

/**
 * @brief Analizar componentes de una URL
 */
void demo_analizar_url(void) {
    char url[MAX_URL_LENGTH];
    url_parseada_t parseada;
    int resultado;
    
    printf("\n=== Analizador de URLs ===\n");
    printf("Introduce la URL a analizar: ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        return;
    }
    
    url[strcspn(url, "\n")] = '\0';
    
    if (strlen(url) == 0) {
        strcpy(url, "https://www.example.com:8080/path/to/resource?param=value#section");
        printf("Usando URL de ejemplo: %s\n", url);
    }
    
    resultado = cliente_http_parsear_url(url, &parseada);
    
    if (resultado == CLIENTE_HTTP_OK) {
        printf("\n=== Componentes de la URL ===\n");
        printf("URL completa: %s\n", url);
        printf("Esquema:      %s\n", parseada.esquema);
        printf("Host:         %s\n", parseada.host);
        printf("Puerto:       %s\n", parseada.puerto);
        printf("Path:         %s\n", parseada.path);
        if (strlen(parseada.query) > 0) {
            printf("Query:        %s\n", parseada.query);
        }
        if (strlen(parseada.fragment) > 0) {
            printf("Fragment:     %s\n", parseada.fragment);
        }
        
        printf("\nValidación: %s\n", 
               cliente_http_validar_url(url) ? "✓ URL válida" : "✗ URL inválida");
    } else {
        printf("Error al parsear URL: %s\n", cliente_http_error_string(resultado));
    }
}

/**
 * @brief Configuración personalizada
 */
void demo_configuracion_personalizada(void) {
    peticion_http_t peticion;
    respuesta_http_t respuesta;
    char url[MAX_URL_LENGTH];
    char input[256];
    int resultado;
    
    printf("\n=== Configuración Personalizada ===\n");
    
    cliente_http_init_peticion(&peticion);
    
    printf("URL: ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        return;
    }
    url[strcspn(url, "\n")] = '\0';
    
    if (strlen(url) == 0) {
        strcpy(url, "http://httpbin.org/user-agent");
        printf("Usando URL por defecto: %s\n", url);
    }
    
    // Parsear URL
    url_parseada_t url_parseada;
    resultado = cliente_http_parsear_url(url, &url_parseada);
    if (resultado != CLIENTE_HTTP_OK) {
        printf("Error al parsear URL: %s\n", cliente_http_error_string(resultado));
        return;
    }
    
    strcpy(peticion.host, url_parseada.host);
    strcpy(peticion.puerto, url_parseada.puerto);
    strcpy(peticion.path, url_parseada.path);
    
    printf("User-Agent personalizado (Enter para defecto): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strncpy(peticion.user_agent, input, sizeof(peticion.user_agent) - 1);
        }
    }
    
    printf("Timeout en segundos (Enter para 30): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        int timeout = atoi(input);
        if (timeout > 0 && timeout <= 300) {
            peticion.timeout = timeout;
        }
    }
    
    printf("Headers adicionales (Enter para ninguno):\n");
    printf("Ejemplo: Accept: application/json\\r\\n\n");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            // Convertir \\r\\n a \r\n reales
            char *pos = strstr(input, "\\r\\n");
            while (pos) {
                *pos = '\r';
                *(pos + 1) = '\n';
                memmove(pos + 2, pos + 4, strlen(pos + 4) + 1);
                pos = strstr(pos + 2, "\\r\\n");
            }
            strncpy(peticion.headers_extra, input, sizeof(peticion.headers_extra) - 1);
            strcat(peticion.headers_extra, "\r\n");
        }
    }
    
    printf("\nRealizando petición personalizada...\n");
    resultado = cliente_http_realizar_peticion(&peticion, &respuesta, NULL);
    
    if (resultado == CLIENTE_HTTP_OK) {
        printf("Respuesta recibida (Código: %d)\n", respuesta.codigo_estado);
        cliente_http_imprimir_headers(&respuesta);
        
        if (respuesta.contenido && respuesta.tamaño_contenido > 0) {
            printf("\n=== Contenido ===\n");
            // Mostrar hasta 1000 caracteres
            size_t mostrar = respuesta.tamaño_contenido > 1000 ? 1000 : respuesta.tamaño_contenido;
            fwrite(respuesta.contenido, 1, mostrar, stdout);
            if (respuesta.tamaño_contenido > 1000) {
                printf("\n... [truncado] ...\n");
            }
            printf("\n");
        }
        
        cliente_http_liberar_respuesta(&respuesta);
    } else {
        printf("Error: %s\n", cliente_http_error_string(resultado));
    }
}

/**
 * @brief Mostrar ayuda y ejemplos
 */
void mostrar_ayuda(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    AYUDA - Cliente HTTP                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("🌐 URLS DE EJEMPLO:\n");
    printf("   • http://example.com                    - Página simple\n");
    printf("   • http://httpbin.org/get                - API de prueba\n");
    printf("   • http://httpbin.org/user-agent         - Muestra User-Agent\n");
    printf("   • http://httpbin.org/headers            - Muestra todos los headers\n");
    printf("   • http://httpbin.org/ip                 - Muestra tu IP\n");
    printf("   • http://httpbin.org/status/200         - Código de estado específico\n");
    printf("\n");
    printf("📋 FORMATOS DE URL SOPORTADOS:\n");
    printf("   • http://servidor.com                   - Puerto 80 por defecto\n");
    printf("   • http://servidor.com:8080              - Puerto personalizado\n");
    printf("   • http://servidor.com/path              - Con path\n");
    printf("   • http://servidor.com/path?query=value  - Con parámetros\n");
    printf("\n");
    printf("🔧 CARACTERÍSTICAS:\n");
    printf("   • Soporte para IPv4 e IPv6\n");
    printf("   • Timeouts configurables\n");
    printf("   • Headers personalizados\n");
    printf("   • Estadísticas detalladas\n");
    printf("   • Análisis de rendimiento\n");
    printf("   • Métodos HTTP: GET, POST, HEAD, PUT, DELETE\n");
    printf("   • Versiones HTTP: 1.0, 1.1\n");
    printf("\n");
    printf("📊 CÓDIGOS DE ESTADO HTTP COMUNES:\n");
    printf("   • 200 OK                - Éxito\n");
    printf("   • 301/302 Redirect      - Redirección\n");
    printf("   • 404 Not Found         - No encontrado\n");
    printf("   • 500 Internal Error    - Error del servidor\n");
    printf("\n");
    printf("⚡ TIPS DE USO:\n");
    printf("   • Usa httpbin.org para pruebas seguras\n");
    printf("   • El benchmark consume recursos, usa números moderados\n");
    printf("   • Las estadísticas ayudan a identificar cuellos de botella\n");
    printf("   • Los headers personalizados son útiles para APIs\n");
    printf("\n");
    printf("Presiona Enter para continuar...");
    getchar();
}

/**
 * @brief Función principal
 */
#ifndef UNIT_TESTING
int main(void) {
    int opcion;
    char input[256];
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     CLIENTE HTTP v%s                      ║\n", CLIENTE_HTTP_VERSION);
    printf("║                  Ejercicio 093 - Redes                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\nCliente HTTP de bajo nivel usando sockets TCP\n");
    printf("Soporta peticiones GET, análisis de URLs y estadísticas\n");
    
    while (1) {
        mostrar_menu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        opcion = atoi(input);
        
        switch (opcion) {
            case 1:
                demo_get_basico();
                break;
                
            case 2:
                demo_con_estadisticas();
                break;
                
            case 3:
                demo_solo_headers();
                break;
                
            case 4:
                demo_multiples_peticiones();
                break;
                
            case 5:
                demo_benchmark();
                break;
                
            case 6:
                demo_urls_predefinidas();
                break;
                
            case 7:
                demo_analizar_url();
                break;
                
            case 8:
                demo_configuracion_personalizada();
                break;
                
            case 9:
                mostrar_ayuda();
                break;
                
            case 0:
                printf("\n¡Gracias por usar el Cliente HTTP!\n");
                printf("Ejercicio completado: Peticiones HTTP con sockets\n");
                return 0;
                
            default:
                printf("\n❌ Opción inválida. Introduce un número del 0-9.\n");
                break;
        }
        
        if (opcion >= 1 && opcion <= 8) {
            printf("\nPresiona Enter para continuar...");
            getchar();
        }
    }
    
    return 0;
}
#endif
