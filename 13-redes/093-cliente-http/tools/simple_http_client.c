/**
 * @file simple_http_client.c
 * @brief Cliente HTTP simple para pruebas rápidas
 */

#include "../include/cliente_http.h"

int main(int argc, char *argv[]) {
    const char *url;
    respuesta_http_t respuesta;
    int resultado;
    
    // Usar URL de línea de comandos o por defecto
    if (argc > 1) {
        url = argv[1];
    } else {
        url = "http://example.com";
        printf("Uso: %s <URL>\n", argv[0]);
        printf("Usando URL por defecto: %s\n\n", url);
    }
    
    printf("Cliente HTTP Simple\n");
    printf("==================\n");
    printf("URL: %s\n\n", url);
    
    // Realizar petición
    resultado = cliente_http_get_simple(url, &respuesta);
    
    if (resultado == CLIENTE_HTTP_OK) {
        printf("✓ Petición exitosa\n");
        printf("Código HTTP: %d %s\n", respuesta.codigo_estado, respuesta.razon_estado);
        printf("Versión HTTP: %s\n", cliente_http_version_string(respuesta.version));
        printf("Tamaño: %zu bytes\n\n", respuesta.tamaño_contenido);
        
        // Mostrar headers
        printf("=== HEADERS ===\n");
        printf("%s\n", respuesta.headers);
        
        // Mostrar contenido (limitado)
        if (respuesta.contenido && respuesta.tamaño_contenido > 0) {
            printf("=== CONTENIDO ===\n");
            
            // Buscar separador entre headers y contenido en el contenido completo
            const char *inicio_contenido = strstr(respuesta.contenido, "\r\n\r\n");
            if (inicio_contenido) {
                inicio_contenido += 4;
                
                size_t tamaño_contenido_real = strlen(inicio_contenido);
                size_t mostrar = tamaño_contenido_real > 2000 ? 2000 : tamaño_contenido_real;
                
                fwrite(inicio_contenido, 1, mostrar, stdout);
                
                if (tamaño_contenido_real > 2000) {
                    printf("\n\n... [Contenido truncado - %zu bytes totales] ...\n", 
                           tamaño_contenido_real);
                }
            } else {
                // Si no encontramos el separador, mostrar todo limitado
                size_t mostrar = respuesta.tamaño_contenido > 2000 ? 2000 : respuesta.tamaño_contenido;
                fwrite(respuesta.contenido, 1, mostrar, stdout);
                
                if (respuesta.tamaño_contenido > 2000) {
                    printf("\n\n... [Contenido truncado - %zu bytes totales] ...\n", 
                           respuesta.tamaño_contenido);
                }
            }
        }
        
        printf("\n\n=== ESTADÍSTICAS ===\n");
        printf("Tiempo de respuesta: %.3f ms\n", respuesta.tiempo_respuesta * 1000);
        printf("Bytes enviados: %zu\n", respuesta.bytes_enviados);
        printf("Bytes recibidos: %zu\n", respuesta.bytes_recibidos);
        
        cliente_http_liberar_respuesta(&respuesta);
        
    } else {
        printf("✗ Error en la petición: %s\n", cliente_http_error_string(resultado));
        return 1;
    }
    
    return 0;
}
