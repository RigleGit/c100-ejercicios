#include "escribir_registros_binario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estructura para el header del archivo
typedef struct {
    char magic[4];        // "PROD" como identificador
    int version;          // Versión del formato
    size_t num_registros; // Número de registros
    time_t timestamp;     // Timestamp de creación
    unsigned int checksum; // Checksum para validación
} HeaderArchivo;

void inicializar_estadisticas_escritura(EstadisticasEscritura* stats) {
    if (stats) {
        stats->elementos_escritos = 0;
        stats->bytes_escritos = 0;
        stats->operacion_exitosa = false;
    }
}

Producto crear_producto(int codigo, float precio) {
    Producto p;
    p.codigo = codigo;
    p.precio = precio;
    return p;
}

ProductoExtendido crear_producto_extendido(int codigo, float precio, const char* nombre,
                                          int stock, const char* categoria, double descuento) {
    ProductoExtendido p;
    p.codigo = codigo;
    p.precio = precio;
    p.stock = stock;
    p.descuento = descuento;
    
    // Copiar strings de forma segura
    if (nombre) {
        strncpy(p.nombre, nombre, sizeof(p.nombre) - 1);
        p.nombre[sizeof(p.nombre) - 1] = '\0';
    } else {
        strcpy(p.nombre, "Sin nombre");
    }
    
    if (categoria) {
        strncpy(p.categoria, categoria, sizeof(p.categoria) - 1);
        p.categoria[sizeof(p.categoria) - 1] = '\0';
    } else {
        strcpy(p.categoria, "General");
    }
    
    return p;
}

bool validar_producto(const Producto* producto) {
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

bool escribir_productos_basico(const Producto* productos, size_t num_productos, 
                              const char* nombre_archivo, EstadisticasEscritura* stats) {
    if (!productos || !nombre_archivo || num_productos == 0) {
        return false;
    }

    if (stats) {
        inicializar_estadisticas_escritura(stats);
    }

    FILE* archivo = fopen(nombre_archivo, "wb");
    if (!archivo) {
        return false;
    }

    // Validar todos los productos antes de escribir
    for (size_t i = 0; i < num_productos; i++) {
        if (!validar_producto(&productos[i])) {
            fclose(archivo);
            return false;
        }
    }

    size_t elementos_escritos = fwrite(productos, sizeof(Producto), num_productos, archivo);
    
    if (stats) {
        stats->elementos_escritos = elementos_escritos;
        stats->bytes_escritos = elementos_escritos * sizeof(Producto);
        stats->operacion_exitosa = (elementos_escritos == num_productos);
    }

    fclose(archivo);
    return elementos_escritos == num_productos;
}

bool escribir_productos_extendido(const ProductoExtendido* productos, size_t num_productos,
                                 const char* nombre_archivo, EstadisticasEscritura* stats) {
    if (!productos || !nombre_archivo || num_productos == 0) {
        return false;
    }

    if (stats) {
        inicializar_estadisticas_escritura(stats);
    }

    FILE* archivo = fopen(nombre_archivo, "wb");
    if (!archivo) {
        return false;
    }

    size_t elementos_escritos = fwrite(productos, sizeof(ProductoExtendido), num_productos, archivo);
    
    if (stats) {
        stats->elementos_escritos = elementos_escritos;
        stats->bytes_escritos = elementos_escritos * sizeof(ProductoExtendido);
        stats->operacion_exitosa = (elementos_escritos == num_productos);
    }

    fclose(archivo);
    return elementos_escritos == num_productos;
}

bool escribir_producto_individual(const Producto* producto, const char* nombre_archivo, 
                                 bool es_primer_elemento) {
    if (!producto || !nombre_archivo) {
        return false;
    }

    if (!validar_producto(producto)) {
        return false;
    }

    const char* modo = es_primer_elemento ? "wb" : "ab";
    FILE* archivo = fopen(nombre_archivo, modo);
    if (!archivo) {
        return false;
    }

    size_t elementos_escritos = fwrite(producto, sizeof(Producto), 1, archivo);
    fclose(archivo);

    return elementos_escritos == 1;
}

bool anexar_producto(const Producto* producto, const char* nombre_archivo) {
    return escribir_producto_individual(producto, nombre_archivo, false);
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

bool escribir_productos_con_validacion(const Producto* productos, size_t num_productos,
                                       const char* nombre_archivo, bool incluir_checksum) {
    if (!productos || !nombre_archivo || num_productos == 0) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "wb");
    if (!archivo) {
        return false;
    }

    bool exito = true;

    // Escribir checksum al inicio si se solicita
    if (incluir_checksum) {
        unsigned int checksum = calcular_checksum_productos(productos, num_productos);
        if (fwrite(&checksum, sizeof(unsigned int), 1, archivo) != 1) {
            exito = false;
        }
    }

    // Escribir productos con validación individual
    if (exito) {
        for (size_t i = 0; i < num_productos && exito; i++) {
            if (!validar_producto(&productos[i])) {
                exito = false;
            } else {
                if (fwrite(&productos[i], sizeof(Producto), 1, archivo) != 1) {
                    exito = false;
                }
            }
        }
    }

    fclose(archivo);
    return exito;
}

bool escribir_productos_con_header(const Producto* productos, size_t num_productos,
                                   const char* nombre_archivo) {
    if (!productos || !nombre_archivo || num_productos == 0) {
        return false;
    }

    FILE* archivo = fopen(nombre_archivo, "wb");
    if (!archivo) {
        return false;
    }

    // Crear header
    HeaderArchivo header;
    memcpy(header.magic, "PROD", 4);
    header.version = 1;
    header.num_registros = num_productos;
    header.timestamp = time(NULL);
    header.checksum = calcular_checksum_productos(productos, num_productos);

    // Escribir header
    if (fwrite(&header, sizeof(HeaderArchivo), 1, archivo) != 1) {
        fclose(archivo);
        return false;
    }

    // Escribir productos
    size_t elementos_escritos = fwrite(productos, sizeof(Producto), num_productos, archivo);
    fclose(archivo);

    return elementos_escritos == num_productos;
}

void imprimir_producto(const Producto* producto) {
    if (!producto) {
        printf("Producto NULL\n");
        return;
    }

    printf("Producto { código: %d, precio: %.2f }\n", 
           producto->codigo, producto->precio);
}

void imprimir_producto_extendido(const ProductoExtendido* producto) {
    if (!producto) {
        printf("ProductoExtendido NULL\n");
        return;
    }

    printf("ProductoExtendido {\n");
    printf("  código: %d\n", producto->codigo);
    printf("  precio: %.2f\n", producto->precio);
    printf("  nombre: \"%s\"\n", producto->nombre);
    printf("  stock: %d\n", producto->stock);
    printf("  categoría: \"%s\"\n", producto->categoria);
    printf("  descuento: %.2f%%\n", producto->descuento * 100);
    printf("}\n");
}

void imprimir_estadisticas_escritura(const EstadisticasEscritura* stats) {
    if (!stats) {
        printf("Estadísticas NULL\n");
        return;
    }

    printf("=== Estadísticas de Escritura ===\n");
    printf("Elementos escritos: %zu\n", stats->elementos_escritos);
    printf("Bytes escritos: %zu\n", stats->bytes_escritos);
    printf("Operación exitosa: %s\n", stats->operacion_exitosa ? "Sí" : "No");
    printf("================================\n");
}

// Función principal del usuario (versión básica del enunciado)
int main_usuario_basico(void) {
    typedef struct {
        int codigo;
        float precio;
    } Producto;

    Producto lista[] = {
        {101, 12.99},
        {102, 8.50},
        {103, 15.75}
    };

    FILE *archivo = fopen("productos.dat", "wb");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    size_t elementosEscritos = fwrite(lista, sizeof(Producto), 3, archivo);
    if (elementosEscritos != 3) {
        printf("Error al escribir en el archivo.\n");
    }

    fclose(archivo);
    printf("Productos guardados correctamente en binario.\n");

    return 0;
}

// Función principal mejorada
int main(void) {
    printf("=== Escritura de Registros Binarios ===\n\n");

    // Crear productos de ejemplo
    Producto productos[] = {
        crear_producto(101, 12.99f),
        crear_producto(102, 8.50f),
        crear_producto(103, 15.75f),
        crear_producto(104, 25.30f),
        crear_producto(105, 7.95f)
    };
    size_t num_productos = sizeof(productos) / sizeof(productos[0]);

    printf("Productos a escribir:\n");
    for (size_t i = 0; i < num_productos; i++) {
        printf("  ");
        imprimir_producto(&productos[i]);
    }
    printf("\n");

    // Demostración 1: Escritura básica
    EstadisticasEscritura stats;
    if (escribir_productos_basico(productos, num_productos, "productos.dat", &stats)) {
        printf("✅ Escritura básica exitosa:\n");
        imprimir_estadisticas_escritura(&stats);
    } else {
        printf("❌ Error en escritura básica\n");
    }

    // Demostración 2: Escritura con validación
    if (escribir_productos_con_validacion(productos, num_productos, "productos_validados.dat", true)) {
        printf("✅ Escritura con validación exitosa (con checksum)\n");
    } else {
        printf("❌ Error en escritura con validación\n");
    }

    // Demostración 3: Escritura con header
    if (escribir_productos_con_header(productos, num_productos, "productos_con_header.dat")) {
        printf("✅ Escritura con header exitosa\n");
    } else {
        printf("❌ Error en escritura con header\n");
    }

    // Demostración 4: Productos extendidos
    ProductoExtendido productos_ext[] = {
        crear_producto_extendido(201, 45.99f, "Laptop Gaming", 10, "Electrónicos", 0.1),
        crear_producto_extendido(202, 299.99f, "Monitor 4K", 5, "Electrónicos", 0.05),
        crear_producto_extendido(203, 89.50f, "Teclado Mecánico", 20, "Accesorios", 0.15)
    };
    size_t num_productos_ext = sizeof(productos_ext) / sizeof(productos_ext[0]);

    printf("\nProductos extendidos:\n");
    for (size_t i = 0; i < num_productos_ext; i++) {
        imprimir_producto_extendido(&productos_ext[i]);
    }

    EstadisticasEscritura stats_ext;
    if (escribir_productos_extendido(productos_ext, num_productos_ext, "productos_extendidos.dat", &stats_ext)) {
        printf("✅ Escritura de productos extendidos exitosa:\n");
        imprimir_estadisticas_escritura(&stats_ext);
    } else {
        printf("❌ Error en escritura de productos extendidos\n");
    }

    // Demostración 5: Escritura individual y anexado
    Producto nuevo_producto = crear_producto(106, 33.75f);
    if (anexar_producto(&nuevo_producto, "productos.dat")) {
        printf("✅ Producto anexado correctamente\n");
    } else {
        printf("❌ Error al anexar producto\n");
    }

    printf("\n=== Archivos generados ===\n");
    printf("- productos.dat (básico + producto anexado)\n");
    printf("- productos_validados.dat (con checksum)\n");
    printf("- productos_con_header.dat (con metadatos)\n");
    printf("- productos_extendidos.dat (estructura extendida)\n");

    return 0;
}
