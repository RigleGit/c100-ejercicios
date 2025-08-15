#include "leer_registros_binario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void inicializar_estadisticas_lectura(EstadisticasLectura* stats) {
    if (stats) {
        stats->elementos_leidos = 0;
        stats->bytes_leidos = 0;
        stats->operacion_exitosa = false;
        stats->archivo_existe = false;
        stats->formato_valido = false;
        stats->tamaño_archivo = 0;
    }
}

bool validar_producto_leido(const Producto* producto) {
    if (!producto) {
        return false;
    }
    
    // Validaciones básicas
    if (producto->codigo <= 0) {
        return false;
    }
    
    if (producto->precio < 0.0f) {
        return false;
    }
    
    return true;
}

size_t obtener_tamaño_archivo(const char* nombre_archivo) {
    struct stat st;
    if (stat(nombre_archivo, &st) == 0) {
        return st.st_size;
    }
    return 0;
}

bool obtener_info_archivo(const char* nombre_archivo, EstadisticasLectura* stats) {
    if (!nombre_archivo || !stats) {
        return false;
    }

    inicializar_estadisticas_lectura(stats);

    // Verificar que el archivo existe
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return false;
    }

    stats->archivo_existe = true;
    stats->tamaño_archivo = obtener_tamaño_archivo(nombre_archivo);
    
    // Verificar que el tamaño es múltiplo del tamaño de Producto
    if (stats->tamaño_archivo % sizeof(Producto) == 0) {
        stats->formato_valido = true;
    }

    fclose(archivo);
    return true;
}

size_t contar_productos_en_archivo(const char* nombre_archivo) {
    EstadisticasLectura stats;
    if (!obtener_info_archivo(nombre_archivo, &stats)) {
        return 0;
    }

    if (!stats.formato_valido) {
        return 0;
    }

    return stats.tamaño_archivo / sizeof(Producto);
}

size_t leer_productos_basico(const char* nombre_archivo, Producto* productos, 
                            size_t max_productos, EstadisticasLectura* stats) {
    if (!nombre_archivo || !productos || max_productos == 0) {
        return 0;
    }

    if (stats) {
        inicializar_estadisticas_lectura(stats);
    }

    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return 0;
    }

    if (stats) {
        stats->archivo_existe = true;
        stats->tamaño_archivo = obtener_tamaño_archivo(nombre_archivo);
    }

    size_t elementos_leidos = fread(productos, sizeof(Producto), max_productos, archivo);
    
    if (stats) {
        stats->elementos_leidos = elementos_leidos;
        stats->bytes_leidos = elementos_leidos * sizeof(Producto);
        stats->operacion_exitosa = (elementos_leidos > 0);
        stats->formato_valido = true;
    }

    // Validar productos leídos
    for (size_t i = 0; i < elementos_leidos; i++) {
        if (!validar_producto_leido(&productos[i])) {
            if (stats) {
                stats->formato_valido = false;
            }
            fclose(archivo);
            return 0;
        }
    }

    fclose(archivo);
    return elementos_leidos;
}

bool leer_productos_dinamico(const char* nombre_archivo, ResultadoLectura* resultado) {
    if (!nombre_archivo || !resultado) {
        return false;
    }

    // Inicializar resultado
    resultado->productos = NULL;
    resultado->num_productos = 0;
    resultado->necesita_liberacion = false;
    inicializar_estadisticas_lectura(&resultado->stats);

    // Contar productos en el archivo
    size_t num_productos = contar_productos_en_archivo(nombre_archivo);
    if (num_productos == 0) {
        return false;
    }

    // Asignar memoria dinámicamente
    resultado->productos = malloc(num_productos * sizeof(Producto));
    if (!resultado->productos) {
        return false;
    }

    resultado->necesita_liberacion = true;

    // Leer productos
    size_t leidos = leer_productos_basico(nombre_archivo, resultado->productos, 
                                         num_productos, &resultado->stats);
    
    if (leidos > 0) {
        resultado->num_productos = leidos;
        return true;
    }

    // Error: liberar memoria
    free(resultado->productos);
    resultado->productos = NULL;
    resultado->necesita_liberacion = false;
    return false;
}

size_t leer_productos_extendido(const char* nombre_archivo, ProductoExtendido* productos,
                               size_t max_productos, EstadisticasLectura* stats) {
    if (!nombre_archivo || !productos || max_productos == 0) {
        return 0;
    }

    if (stats) {
        inicializar_estadisticas_lectura(stats);
    }

    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return 0;
    }

    if (stats) {
        stats->archivo_existe = true;
        stats->tamaño_archivo = obtener_tamaño_archivo(nombre_archivo);
    }

    size_t elementos_leidos = fread(productos, sizeof(ProductoExtendido), max_productos, archivo);
    
    if (stats) {
        stats->elementos_leidos = elementos_leidos;
        stats->bytes_leidos = elementos_leidos * sizeof(ProductoExtendido);
        stats->operacion_exitosa = (elementos_leidos > 0);
        stats->formato_valido = (stats->tamaño_archivo % sizeof(ProductoExtendido) == 0);
    }

    fclose(archivo);
    return elementos_leidos;
}

unsigned int calcular_checksum_productos(const Producto* productos, size_t num_productos) {
    if (!productos || num_productos == 0) {
        return 0;
    }

    unsigned int checksum = 0;
    const unsigned char* bytes = (const unsigned char*)productos;
    size_t total_bytes = num_productos * sizeof(Producto);

    for (size_t i = 0; i < total_bytes; i++) {
        checksum += bytes[i];
        checksum = (checksum << 1) | (checksum >> 31); // Rotación simple
    }

    return checksum;
}

size_t leer_productos_con_validacion(const char* nombre_archivo, Producto* productos,
                                     size_t max_productos, bool validar_checksum) {
    if (!nombre_archivo || !productos || max_productos == 0) {
        return 0;
    }

    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return 0;
    }

    unsigned int checksum_archivo = 0;
    size_t elementos_leidos = 0;

    // Leer checksum si se solicita validación
    if (validar_checksum) {
        if (fread(&checksum_archivo, sizeof(unsigned int), 1, archivo) != 1) {
            fclose(archivo);
            return 0;
        }
    }

    // Leer productos
    elementos_leidos = fread(productos, sizeof(Producto), max_productos, archivo);
    fclose(archivo);

    if (elementos_leidos == 0) {
        return 0;
    }

    // Validar checksum si se solicita
    if (validar_checksum) {
        unsigned int checksum_calculado = calcular_checksum_productos(productos, elementos_leidos);
        if (checksum_calculado != checksum_archivo) {
            return 0; // Checksum no coincide
        }
    }

    return elementos_leidos;
}

size_t leer_productos_con_header(const char* nombre_archivo, Producto* productos,
                                 size_t max_productos, HeaderArchivo* header) {
    if (!nombre_archivo || !productos || max_productos == 0) {
        return 0;
    }

    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return 0;
    }

    HeaderArchivo header_local;
    HeaderArchivo* header_ptr = header ? header : &header_local;

    // Leer header
    if (fread(header_ptr, sizeof(HeaderArchivo), 1, archivo) != 1) {
        fclose(archivo);
        return 0;
    }

    // Validar magic number
    if (memcmp(header_ptr->magic, "PROD", 4) != 0) {
        fclose(archivo);
        return 0;
    }

    // Leer productos (limitado por header y buffer)
    size_t productos_a_leer = (header_ptr->num_registros < max_productos) ? 
                              header_ptr->num_registros : max_productos;
    
    size_t elementos_leidos = fread(productos, sizeof(Producto), productos_a_leer, archivo);
    fclose(archivo);

    // Validar checksum del header
    if (elementos_leidos > 0) {
        unsigned int checksum_calculado = calcular_checksum_productos(productos, elementos_leidos);
        if (checksum_calculado != header_ptr->checksum) {
            return 0; // Checksum no coincide
        }
    }

    return elementos_leidos;
}

bool leer_producto_por_indice(const char* nombre_archivo, size_t indice, Producto* producto) {
    if (!nombre_archivo || !producto) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return false;
    }

    // Calcular posición y saltar al producto deseado
    long posicion = indice * sizeof(Producto);
    if (fseek(archivo, posicion, SEEK_SET) != 0) {
        fclose(archivo);
        return false;
    }

    // Leer producto
    size_t leidos = fread(producto, sizeof(Producto), 1, archivo);
    fclose(archivo);

    if (leidos == 1 && validar_producto_leido(producto)) {
        return true;
    }

    return false;
}

bool buscar_producto_por_codigo(const char* nombre_archivo, int codigo, Producto* producto) {
    if (!nombre_archivo || !producto || codigo <= 0) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        return false;
    }

    Producto temp;
    while (fread(&temp, sizeof(Producto), 1, archivo) == 1) {
        if (temp.codigo == codigo && validar_producto_leido(&temp)) {
            *producto = temp;
            fclose(archivo);
            return true;
        }
    }

    fclose(archivo);
    return false;
}

bool validar_integridad_archivo(const char* nombre_archivo) {
    EstadisticasLectura stats;
    if (!obtener_info_archivo(nombre_archivo, &stats)) {
        return false;
    }

    if (!stats.formato_valido) {
        return false;
    }

    // Intentar leer y validar todos los productos
    size_t num_productos = stats.tamaño_archivo / sizeof(Producto);
    if (num_productos == 0) {
        return true; // Archivo vacío es válido
    }

    Producto* productos = malloc(num_productos * sizeof(Producto));
    if (!productos) {
        return false;
    }

    size_t leidos = leer_productos_basico(nombre_archivo, productos, num_productos, NULL);
    bool valido = (leidos == num_productos);

    free(productos);
    return valido;
}

char* timestamp_a_string(time_t timestamp, char* buffer) {
    if (!buffer) {
        return NULL;
    }

    struct tm* tm_info = localtime(&timestamp);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

void imprimir_producto(const Producto* producto, size_t indice) {
    if (!producto) {
        printf("[%zu] Producto NULL\n", indice);
        return;
    }

    printf("[%zu] Código: %d, Precio: $%.2f\n", 
           indice, producto->codigo, producto->precio);
}

void imprimir_producto_extendido(const ProductoExtendido* producto, size_t indice) {
    if (!producto) {
        printf("[%zu] ProductoExtendido NULL\n", indice);
        return;
    }

    printf("[%zu] ProductoExtendido {\n", indice);
    printf("    Código: %d\n", producto->codigo);
    printf("    Precio: $%.2f\n", producto->precio);
    printf("    Nombre: \"%s\"\n", producto->nombre);
    printf("    Stock: %d\n", producto->stock);
    printf("    Categoría: \"%s\"\n", producto->categoria);
    printf("    Descuento: %.2f%%\n", producto->descuento * 100);
    printf("}\n");
}

void imprimir_estadisticas_lectura(const EstadisticasLectura* stats) {
    if (!stats) {
        printf("Estadísticas NULL\n");
        return;
    }

    printf("=== Estadísticas de Lectura ===\n");
    printf("Archivo existe: %s\n", stats->archivo_existe ? "Sí" : "No");
    printf("Formato válido: %s\n", stats->formato_valido ? "Sí" : "No");
    printf("Tamaño archivo: %zu bytes\n", stats->tamaño_archivo);
    printf("Elementos leídos: %zu\n", stats->elementos_leidos);
    printf("Bytes leídos: %zu\n", stats->bytes_leidos);
    printf("Operación exitosa: %s\n", stats->operacion_exitosa ? "Sí" : "No");
    printf("==============================\n");
}

void imprimir_header_archivo(const HeaderArchivo* header) {
    if (!header) {
        printf("Header NULL\n");
        return;
    }

    char timestamp_str[26];
    timestamp_a_string(header->timestamp, timestamp_str);

    printf("=== Header del Archivo ===\n");
    printf("Magic: %.4s\n", header->magic);
    printf("Versión: %d\n", header->version);
    printf("Número de registros: %zu\n", header->num_registros);
    printf("Timestamp: %s\n", timestamp_str);
    printf("Checksum: 0x%08X\n", header->checksum);
    printf("==========================\n");
}

void liberar_resultado_lectura(ResultadoLectura* resultado) {
    if (resultado && resultado->necesita_liberacion && resultado->productos) {
        free(resultado->productos);
        resultado->productos = NULL;
        resultado->num_productos = 0;
        resultado->necesita_liberacion = false;
    }
}

// Función principal del usuario (versión básica del enunciado)
int main_usuario_basico(void) {
    typedef struct {
        int codigo;
        float precio;
    } Producto;

    Producto lista[10]; // Asumimos como máximo 10 productos
    size_t leidos;

    FILE *archivo = fopen("productos.dat", "rb");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    leidos = fread(lista, sizeof(Producto), 10, archivo);
    if (leidos == 0) {
        printf("No se leyeron registros.\n");
        fclose(archivo);
        return 1;
    }

    printf("Productos leídos desde el archivo:\n");
    for (size_t i = 0; i < leidos; i++) {
        printf("Código: %d, Precio: %.2f\n", lista[i].codigo, lista[i].precio);
    }

    fclose(archivo);
    return 0;
}

// Función principal mejorada
static void imprimir_separador(void) {
    putchar('\n');
    for (int i = 0; i < 50; i++) {
        putchar('=');
    }
    putchar('\n');
    putchar('\n');
}

int main(void) {
    printf("=== Lectura de Registros Binarios ===\n\n");

    const char* archivo_basico = "productos.dat";
    const char* archivo_validacion = "productos_validados.dat";
    const char* archivo_header = "productos_con_header.dat";
    const char* archivo_extendido = "productos_extendidos.dat";

    // Demostración 1: Lectura básica
    printf("1. Lectura básica de %s:\n", archivo_basico);
    EstadisticasLectura stats;
    Producto productos[20];
    
    size_t leidos = leer_productos_basico(archivo_basico, productos, 20, &stats);
    if (leidos > 0) {
        printf("✅ Se leyeron %zu productos:\n", leidos);
        for (size_t i = 0; i < leidos; i++) {
            imprimir_producto(&productos[i], i + 1);
        }
        printf("\n");
        imprimir_estadisticas_lectura(&stats);
    } else {
        printf("❌ No se pudieron leer productos de %s\n", archivo_basico);
        printf("   (Asegúrate de ejecutar primero el ejercicio 077)\n");
    }

    imprimir_separador();

    // Demostración 2: Lectura dinámica
    printf("2. Lectura con asignación dinámica:\n");
    ResultadoLectura resultado;
    if (leer_productos_dinamico(archivo_basico, &resultado)) {
        printf("✅ Lectura dinámica exitosa:\n");
        printf("Productos leídos: %zu\n", resultado.num_productos);
        for (size_t i = 0; i < resultado.num_productos && i < 5; i++) {
            imprimir_producto(&resultado.productos[i], i + 1);
        }
        if (resultado.num_productos > 5) {
            printf("... y %zu más\n", resultado.num_productos - 5);
        }
        liberar_resultado_lectura(&resultado);
    } else {
        printf("❌ Error en lectura dinámica\n");
    }

    imprimir_separador();

    // Demostración 3: Lectura con validación de checksum
    printf("3. Lectura con validación de checksum:\n");
    Producto productos_validados[10];
    size_t leidos_validados = leer_productos_con_validacion(archivo_validacion, 
                                                           productos_validados, 10, true);
    if (leidos_validados > 0) {
        printf("✅ Validación de checksum exitosa. Productos leídos: %zu\n", leidos_validados);
        for (size_t i = 0; i < leidos_validados; i++) {
            imprimir_producto(&productos_validados[i], i + 1);
        }
    } else {
        printf("❌ Error en validación de checksum o archivo no encontrado\n");
    }

    imprimir_separador();

    // Demostración 4: Lectura con header
    printf("4. Lectura con header de metadatos:\n");
    HeaderArchivo header;
    Producto productos_header[10];
    size_t leidos_header = leer_productos_con_header(archivo_header, productos_header, 10, &header);
    if (leidos_header > 0) {
        printf("✅ Lectura con header exitosa:\n");
        imprimir_header_archivo(&header);
        printf("\nProductos leídos: %zu\n", leidos_header);
        for (size_t i = 0; i < leidos_header; i++) {
            imprimir_producto(&productos_header[i], i + 1);
        }
    } else {
        printf("❌ Error en lectura con header o archivo no encontrado\n");
    }

    imprimir_separador();

    // Demostración 5: Lectura de productos extendidos
    printf("5. Lectura de productos extendidos:\n");
    ProductoExtendido productos_ext[5];
    EstadisticasLectura stats_ext;
    size_t leidos_ext = leer_productos_extendido(archivo_extendido, productos_ext, 5, &stats_ext);
    if (leidos_ext > 0) {
        printf("✅ Productos extendidos leídos: %zu\n", leidos_ext);
        for (size_t i = 0; i < leidos_ext; i++) {
            imprimir_producto_extendido(&productos_ext[i], i + 1);
        }
    } else {
        printf("❌ Error en lectura de productos extendidos\n");
    }

    imprimir_separador();

    // Demostración 6: Búsqueda por código
    printf("6. Búsqueda por código:\n");
    Producto producto_encontrado;
    int codigo_buscar = 102;
    if (buscar_producto_por_codigo(archivo_basico, codigo_buscar, &producto_encontrado)) {
        printf("✅ Producto con código %d encontrado:\n", codigo_buscar);
        imprimir_producto(&producto_encontrado, 0);
    } else {
        printf("❌ No se encontró producto con código %d\n", codigo_buscar);
    }

    imprimir_separador();

    // Demostración 7: Información de archivos
    printf("7. Información de archivos:\n");
    const char* archivos[] = {archivo_basico, archivo_validacion, archivo_header, archivo_extendido};
    const char* nombres[] = {"Básico", "Con validación", "Con header", "Extendido"};
    
    for (int i = 0; i < 4; i++) {
        printf("%s (%s):\n", nombres[i], archivos[i]);
        size_t num_productos = contar_productos_en_archivo(archivos[i]);
        bool integridad = validar_integridad_archivo(archivos[i]);
        printf("  Productos: %zu\n", num_productos);
        printf("  Integridad: %s\n", integridad ? "✅ Válida" : "❌ Inválida");
        printf("\n");
    }

    return 0;
}
