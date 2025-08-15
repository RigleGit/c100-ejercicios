# Ejercicio 078: Leer Registros Binario

## Descripción

Este ejercicio implementa un programa en C que lee registros de productos desde archivos binarios creados previamente (ejercicio 077), utilizando `fread` para la lectura de estructuras. Demuestra técnicas de lectura segura, validación de datos y manejo de diferentes formatos de archivos binarios.

## Enunciado

Abre el archivo binario `productos.dat` creado previamente, lee los registros almacenados (estructura `Producto`) usando `fread`, y muestra los datos por pantalla. Comprueba que se hayan leído correctamente los elementos y cierra el archivo al finalizar.

Este ejercicio refuerza el manejo de archivos binarios, estructuras y lectura segura de datos en memoria.

## Conceptos Clave

### 1. Lectura de Archivos Binarios
```c
FILE *archivo = fopen("productos.dat", "rb");  // Modo binario lectura
```

### 2. Función fread
```c
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
//             ↑          ↑           ↑             ↑
//          buffer      tamaño     cantidad      archivo
//                    por elem.   elementos
```

### 3. Lectura Segura de Estructuras
```c
Producto productos[10];
size_t leidos = fread(productos, sizeof(Producto), 10, archivo);
if (leidos > 0) {
    // Procesar productos leídos
}
```

### 4. Validación Post-Lectura
```c
for (size_t i = 0; i < leidos; i++) {
    if (productos[i].codigo <= 0 || productos[i].precio < 0) {
        // Dato inválido
    }
}
```

## Implementación

### Estructura del Proyecto
```
078-leer-registros-binario/
├── include/
│   └── leer_registros_binario.h      # Declaraciones y estructuras
├── src/
│   └── leer_registros_binario.c      # Implementación
├── tests/
│   └── test_leer_registros_binario.c # Tests unitarios
├── CMakeLists.txt                    # Configuración de build
└── README.md                         # Este archivo
```

### Funciones Principales

#### 1. Versión Básica del Usuario
```c
int main_usuario_basico() {
    Producto lista[10]; // Buffer estático
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
```

#### 2. Versión Modular Avanzada
```c
size_t leer_productos_basico(const char* nombre_archivo, Producto* productos, 
                            size_t max_productos, EstadisticasLectura* stats);
bool leer_productos_dinamico(const char* nombre_archivo, ResultadoLectura* resultado);
size_t leer_productos_con_validacion(const char* nombre_archivo, Producto* productos,
                                     size_t max_productos, bool validar_checksum);
```

### Estructuras de Datos

#### 1. Producto (Compatible con ejercicio 077)
```c
typedef struct {
    int codigo;       // Código del producto
    float precio;     // Precio del producto
} Producto;
```

#### 2. Estadísticas de Lectura
```c
typedef struct {
    size_t elementos_leidos;      // Número de elementos leídos
    size_t bytes_leidos;          // Número de bytes leídos
    bool operacion_exitosa;       // Si la operación fue exitosa
    bool archivo_existe;          // Si el archivo existe
    bool formato_valido;          // Si el formato es válido
    size_t tamaño_archivo;        // Tamaño total del archivo
} EstadisticasLectura;
```

#### 3. Resultado de Lectura Dinámica
```c
typedef struct {
    Producto* productos;          // Array de productos leídos
    size_t num_productos;         // Número de productos leídos
    EstadisticasLectura stats;    // Estadísticas de la operación
    bool necesita_liberacion;     // Si el array fue asignado dinámicamente
} ResultadoLectura;
```

## Algoritmos y Técnicas

### 1. Lectura Básica con Buffer Estático
```c
size_t leer_productos_basico(const char* nombre_archivo, Producto* productos, 
                            size_t max_productos, EstadisticasLectura* stats) {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) return 0;
    
    size_t elementos_leidos = fread(productos, sizeof(Producto), max_productos, archivo);
    
    // Validar productos leídos
    for (size_t i = 0; i < elementos_leidos; i++) {
        if (!validar_producto_leido(&productos[i])) {
            fclose(archivo);
            return 0;
        }
    }
    
    fclose(archivo);
    return elementos_leidos;
}
```

### 2. Lectura con Asignación Dinámica
```c
bool leer_productos_dinamico(const char* nombre_archivo, ResultadoLectura* resultado) {
    // Contar productos en el archivo
    size_t num_productos = contar_productos_en_archivo(nombre_archivo);
    if (num_productos == 0) return false;
    
    // Asignar memoria dinámicamente
    resultado->productos = malloc(num_productos * sizeof(Producto));
    if (!resultado->productos) return false;
    
    resultado->necesita_liberacion = true;
    
    // Leer productos
    size_t leidos = leer_productos_basico(nombre_archivo, resultado->productos, 
                                         num_productos, &resultado->stats);
    
    resultado->num_productos = leidos;
    return leidos > 0;
}
```

### 3. Lectura con Validación de Checksum
```c
size_t leer_productos_con_validacion(const char* nombre_archivo, Producto* productos,
                                     size_t max_productos, bool validar_checksum) {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) return 0;
    
    unsigned int checksum_archivo = 0;
    
    // Leer checksum si se solicita validación
    if (validar_checksum) {
        if (fread(&checksum_archivo, sizeof(unsigned int), 1, archivo) != 1) {
            fclose(archivo);
            return 0;
        }
    }
    
    // Leer productos
    size_t elementos_leidos = fread(productos, sizeof(Producto), max_productos, archivo);
    fclose(archivo);
    
    // Validar checksum
    if (validar_checksum && elementos_leidos > 0) {
        unsigned int checksum_calculado = calcular_checksum_productos(productos, elementos_leidos);
        if (checksum_calculado != checksum_archivo) {
            return 0; // Checksum no coincide
        }
    }
    
    return elementos_leidos;
}
```

### 4. Lectura con Header de Metadatos
```c
size_t leer_productos_con_header(const char* nombre_archivo, Producto* productos,
                                 size_t max_productos, HeaderArchivo* header) {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) return 0;
    
    // Leer header
    if (fread(header, sizeof(HeaderArchivo), 1, archivo) != 1) {
        fclose(archivo);
        return 0;
    }
    
    // Validar magic number
    if (memcmp(header->magic, "PROD", 4) != 0) {
        fclose(archivo);
        return 0;
    }
    
    // Leer productos según el header
    size_t productos_a_leer = (header->num_registros < max_productos) ? 
                              header->num_registros : max_productos;
    
    return fread(productos, sizeof(Producto), productos_a_leer, archivo);
}
```

### 5. Acceso Aleatorio (Búsqueda por Índice)
```c
bool leer_producto_por_indice(const char* nombre_archivo, size_t indice, Producto* producto) {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) return false;
    
    // Calcular posición y saltar al producto deseado
    long posicion = indice * sizeof(Producto);
    if (fseek(archivo, posicion, SEEK_SET) != 0) {
        fclose(archivo);
        return false;
    }
    
    // Leer producto específico
    size_t leidos = fread(producto, sizeof(Producto), 1, archivo);
    fclose(archivo);
    
    return (leidos == 1 && validar_producto_leido(producto));
}
```

## Casos de Prueba

### Tests Unitarios
1. **Lectura básica**: Array de productos válidos
2. **Buffer pequeño**: Lectura parcial cuando el buffer es menor que el archivo
3. **Archivo vacío**: Manejo correcto de archivos sin datos
4. **Archivo inexistente**: Manejo de errores de apertura
5. **Parámetros inválidos**: NULL pointers, buffers de tamaño 0
6. **Lectura dinámica**: Asignación automática de memoria
7. **Validación de checksum**: Verificación de integridad
8. **Lectura con header**: Procesamiento de metadatos
9. **Búsqueda por código**: Encontrar productos específicos
10. **Acceso por índice**: Lectura de productos específicos
11. **Consistencia escritura-lectura**: Verificar que los datos leídos coinciden con los escritos
12. **Archivos corruptos**: Manejo de formatos inválidos
13. **Archivos grandes**: Performance con muchos registros

### Validación de Integridad
```c
Test(leer_registros_binario, consistencia_escritura_lectura) {
    Producto productos_originales[] = {
        {500, 99.99f},
        {501, 149.50f},
        {502, 299.99f}
    };
    
    // Escribir productos
    FILE* archivo = fopen("test_consistencia.dat", "wb");
    fwrite(productos_originales, sizeof(Producto), 3, archivo);
    fclose(archivo);
    
    // Leer productos
    Producto productos_leidos[3];
    size_t leidos = leer_productos_basico("test_consistencia.dat", productos_leidos, 3, NULL);
    
    // Verificar consistencia
    for (int i = 0; i < 3; i++) {
        cr_assert(eq(int, productos_leidos[i].codigo, productos_originales[i].codigo));
        cr_assert(eq(flt, productos_leidos[i].precio, productos_originales[i].precio, 0.001f));
    }
}
```

## Compilación y Ejecución

### Prerrequisitos
- Compilador C (gcc, clang)
- CMake 3.16+
- Criterion (para tests): `brew install criterion` (macOS)
- **Archivos .dat del ejercicio 077**

### Preparación de Archivos
```bash
# Opción 1: Ejecutar ejercicio 077 primero
cd ../077-escribir-registros-binario/build
./escribir_registros_binario
cp *.dat ../078-leer-registros-binario/build/

# Opción 2: Usar target de CMake
make copy_from_077
```

### Compilar
```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar
```bash
# Programa principal (versión avanzada)
./leer_registros_binario

# Versión básica del usuario
./demo_basico
```

### Ejecutar Tests
```bash
make test
# o directamente
./test_leer_registros_binario
```

### Herramientas de Análisis
```bash
# Copiar archivos del ejercicio 077
make copy_from_077

# Analizar archivos binarios
make analyze_files

# Verificar compatibilidad
make verify_compatibility

# Limpiar archivos de prueba
make clean_test_files
```

## Salida Esperada

### Programa Principal
```
=== Lectura de Registros Binarios ===

1. Lectura básica de productos.dat:
✅ Se leyeron 5 productos:
[1] Código: 101, Precio: $12.99
[2] Código: 102, Precio: $8.50
[3] Código: 103, Precio: $15.75
[4] Código: 104, Precio: $25.30
[5] Código: 105, Precio: $7.95

=== Estadísticas de Lectura ===
Archivo existe: Sí
Formato válido: Sí
Tamaño archivo: 40 bytes
Elementos leídos: 5
Bytes leídos: 40
Operación exitosa: Sí
==============================

==================================================

2. Lectura con asignación dinámica:
✅ Lectura dinámica exitosa:
Productos leídos: 5
[1] Código: 101, Precio: $12.99
[2] Código: 102, Precio: $8.50
...

6. Búsqueda por código:
✅ Producto con código 102 encontrado:
[0] Código: 102, Precio: $8.50

7. Información de archivos:
Básico (productos.dat):
  Productos: 5
  Integridad: ✅ Válida

Con validación (productos_validados.dat):
  Productos: 4
  Integridad: ✅ Válida
...
```

## Funciones Avanzadas Implementadas

### 1. Análisis de Archivos
```c
size_t contar_productos_en_archivo(const char* nombre_archivo);
bool obtener_info_archivo(const char* nombre_archivo, EstadisticasLectura* stats);
bool validar_integridad_archivo(const char* nombre_archivo);
```

### 2. Búsqueda y Acceso
```c
bool leer_producto_por_indice(const char* nombre_archivo, size_t indice, Producto* producto);
bool buscar_producto_por_codigo(const char* nombre_archivo, int codigo, Producto* producto);
```

### 3. Compatibilidad con Formatos del Ejercicio 077
- **productos.dat**: Formato básico
- **productos_validados.dat**: Con checksum de validación
- **productos_con_header.dat**: Con metadatos y timestamp
- **productos_extendidos.dat**: Estructura extendida

## Conceptos Educativos

### 1. Diferencias entre fread y fgets
```c
// fread: Lectura binaria de bytes exactos
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

// fgets: Lectura de texto hasta nueva línea
char *fgets(char *str, int size, FILE *stream);
```

### 2. Manejo de Endianness
```c
// Los archivos binarios conservan la representación de la máquina
// Un int 0x12345678 se almacena como:
// Little-endian: 78 56 34 12
// Big-endian:    12 34 56 78
```

### 3. Validación de Datos Post-Lectura
```c
bool validar_producto_leido(const Producto* producto) {
    if (!producto) return false;
    if (producto->codigo <= 0) return false;  // Códigos positivos
    if (producto->precio < 0.0f) return false;  // Precios no negativos
    return true;
}
```

### 4. Gestión de Memoria Dinámica
```c
ResultadoLectura resultado;
if (leer_productos_dinamico("archivo.dat", &resultado)) {
    // Usar resultado.productos
    liberar_resultado_lectura(&resultado);  // ¡Importante!
}
```

### 5. Manejo de Errores Robusto
```c
if (!archivo) {
    perror("Error al abrir archivo");
    return 0;
}

if (elementos_leidos == 0) {
    if (feof(archivo)) {
        printf("Archivo vacío\n");
    } else if (ferror(archivo)) {
        printf("Error de lectura\n");
    }
}
```

## Compatibilidad con Ejercicio 077

### Formatos Soportados
1. **Básico**: Array directo de estructuras `Producto`
2. **Con checksum**: `[checksum(4 bytes)][productos...]`
3. **Con header**: `[HeaderArchivo][productos...]`
4. **Extendido**: Array de estructuras `ProductoExtendido`

### Verificación de Compatibilidad
```bash
make verify_compatibility
```

## Extensiones y Mejoras

### 1. Streaming de Archivos Grandes
```c
// Leer archivo en chunks para archivos muy grandes
bool procesar_archivo_streaming(const char* nombre_archivo, 
                               void (*callback)(const Producto*, size_t));
```

### 2. Cache de Lecturas
```c
typedef struct {
    char nombre_archivo[256];
    Producto* productos;
    size_t num_productos;
    time_t timestamp_cache;
} CacheArchivo;
```

### 3. Conversión de Formatos
```c
bool convertir_formato_basico_a_header(const char* origen, const char* destino);
bool convertir_formato_extendido_a_basico(const char* origen, const char* destino);
```

## Troubleshooting

### Problemas Comunes
1. **Archivo no encontrado**: Verificar que se ejecutó el ejercicio 077
2. **Datos corruptos**: Validar integridad con checksums
3. **Tamaño incorrecto**: Verificar que el tamaño sea múltiplo de `sizeof(Producto)`
4. **Memoria insuficiente**: En lectura dinámica con archivos grandes

### Herramientas de Debug
```bash
# Verificar archivos binarios
hexdump -C productos.dat

# Información de archivos
stat productos.dat

# Debug con GDB
gdb ./leer_registros_binario
(gdb) break leer_productos_basico
```

## Aplicaciones Prácticas

1. **Lectura de configuraciones**: Cargar datos de configuración binarios
2. **Procesamiento de logs**: Análisis de archivos de log estructurados
3. **Importación de datos**: Migrar datos entre sistemas
4. **Verificación de integridad**: Validar archivos de datos críticos
5. **Análisis forense**: Examinar contenido de archivos binarios

## Referencias

- Manual de `fread`: `man 3 fread`
- Manual de `fseek`: `man 3 fseek`
- Estándar C99: ISO/IEC 9899:1999
- "The C Programming Language" - Kernighan & Ritchie
- "Expert C Programming" - Peter van der Linden
