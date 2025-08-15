# Ejercicio 077: Escribir Registros Binario

## Descripción

Este ejercicio implementa un programa en C que define una estructura `Producto` y escribe registros de productos a un archivo binario usando `fwrite`. Demuestra el manejo de archivos binarios, estructuras de datos y funciones de escritura de bajo nivel.

## Enunciado

Define una estructura `Producto` con campos como código (entero) y precio (float). Crea un array con varios productos, asígnales valores, y escribe todos los registros en un archivo binario llamado `productos.dat` usando `fwrite`. Asegúrate de usar `sizeof` correctamente para escribir el número adecuado de elementos.

Este ejercicio introduce el uso de archivos binarios y refuerza el manejo de estructuras y funciones de escritura en C.

## Conceptos Clave

### 1. Archivos Binarios vs Archivos de Texto
```c
// Archivo de texto
FILE *archivo_texto = fopen("datos.txt", "w");

// Archivo binario
FILE *archivo_binario = fopen("datos.dat", "wb");
```

### 2. Estructura de Datos
```c
typedef struct {
    int codigo;       // 4 bytes
    float precio;     // 4 bytes
} Producto;           // Total: 8 bytes
```

### 3. Escritura con fwrite
```c
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
//              ↑              ↑            ↑             ↑
//           datos          tamaño       cantidad      archivo
//                        por elem.    elementos
```

### 4. Uso Correcto de sizeof
```c
Producto productos[3];
size_t elementos_escritos = fwrite(productos, sizeof(Producto), 3, archivo);
```

## Implementación

### Estructura del Proyecto
```
077-escribir-registros-binario/
├── include/
│   └── escribir_registros_binario.h    # Declaraciones y estructuras
├── src/
│   └── escribir_registros_binario.c    # Implementación
├── tests/
│   └── test_escribir_registros_binario.c # Tests unitarios
├── CMakeLists.txt                       # Configuración de build
└── README.md                            # Este archivo
```

### Funciones Principales

#### 1. Versión Básica del Usuario
```c
int main_usuario_basico() {
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
    size_t elementosEscritos = fwrite(lista, sizeof(Producto), 3, archivo);
    fclose(archivo);
}
```

#### 2. Versión Modular Avanzada
```c
bool escribir_productos_basico(const Producto* productos, size_t num_productos, 
                              const char* nombre_archivo, EstadisticasEscritura* stats);
bool escribir_productos_con_validacion(const Producto* productos, size_t num_productos,
                                       const char* nombre_archivo, bool incluir_checksum);
bool escribir_productos_con_header(const Producto* productos, size_t num_productos,
                                   const char* nombre_archivo);
```

### Estructuras de Datos

#### 1. Producto Básico
```c
typedef struct {
    int codigo;       // Código del producto
    float precio;     // Precio del producto
} Producto;
```

#### 2. Producto Extendido
```c
typedef struct {
    int codigo;              // Código del producto
    float precio;            // Precio del producto
    char nombre[50];         // Nombre del producto
    int stock;               // Cantidad en stock
    char categoria[20];      // Categoría del producto
    double descuento;        // Porcentaje de descuento
} ProductoExtendido;
```

#### 3. Estadísticas de Escritura
```c
typedef struct {
    size_t elementos_escritos;    // Número de elementos escritos
    size_t bytes_escritos;        // Número de bytes escritos
    bool operacion_exitosa;       // Si la operación fue exitosa
} EstadisticasEscritura;
```

## Algoritmos y Técnicas

### 1. Escritura Básica
```c
bool escribir_productos_basico(const Producto* productos, size_t num_productos, 
                              const char* nombre_archivo, EstadisticasEscritura* stats) {
    FILE* archivo = fopen(nombre_archivo, "wb");
    if (!archivo) return false;
    
    size_t escritos = fwrite(productos, sizeof(Producto), num_productos, archivo);
    fclose(archivo);
    
    return escritos == num_productos;
}
```

### 2. Escritura con Validación
```c
bool escribir_productos_con_validacion(const Producto* productos, size_t num_productos,
                                       const char* nombre_archivo, bool incluir_checksum) {
    // Validar productos antes de escribir
    for (size_t i = 0; i < num_productos; i++) {
        if (!validar_producto(&productos[i])) {
            return false;
        }
    }
    
    FILE* archivo = fopen(nombre_archivo, "wb");
    
    // Escribir checksum si se solicita
    if (incluir_checksum) {
        unsigned int checksum = calcular_checksum_productos(productos, num_productos);
        fwrite(&checksum, sizeof(unsigned int), 1, archivo);
    }
    
    // Escribir productos
    size_t escritos = fwrite(productos, sizeof(Producto), num_productos, archivo);
    fclose(archivo);
    
    return escritos == num_productos;
}
```

### 3. Escritura con Header de Metadatos
```c
typedef struct {
    char magic[4];        // "PROD" como identificador
    int version;          // Versión del formato
    size_t num_registros; // Número de registros
    time_t timestamp;     // Timestamp de creación
    unsigned int checksum; // Checksum para validación
} HeaderArchivo;
```

### 4. Escritura Individual y Anexado
```c
bool anexar_producto(const Producto* producto, const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "ab"); // Modo append binario
    if (!archivo) return false;
    
    size_t escritos = fwrite(producto, sizeof(Producto), 1, archivo);
    fclose(archivo);
    
    return escritos == 1;
}
```

## Casos de Prueba

### Tests Unitarios
1. **Creación de productos**: Verificar inicialización correcta
2. **Validación de productos**: Códigos y precios válidos
3. **Escritura básica**: Array de productos válidos
4. **Parámetros inválidos**: NULL pointers, arrays vacíos
5. **Productos inválidos**: Códigos negativos, precios negativos
6. **Escritura con validación**: Con y sin checksum
7. **Escritura con header**: Metadatos y formato
8. **Escritura individual**: Crear y anexar productos
9. **Consistencia de datos**: Verificar integridad después de escritura
10. **Archivos de solo lectura**: Manejo de errores de permisos
11. **Escritura masiva**: Performance con muchos registros

### Verificación de Integridad
```c
// Escribir productos
escribir_productos_basico(productos_originales, 2, "test.dat", NULL);

// Leer de vuelta para verificar
FILE* archivo = fopen("test.dat", "rb");
Producto productos_leidos[2];
fread(productos_leidos, sizeof(Producto), 2, archivo);
fclose(archivo);

// Comparar datos
for (int i = 0; i < 2; i++) {
    assert(productos_leidos[i].codigo == productos_originales[i].codigo);
    assert(productos_leidos[i].precio == productos_originales[i].precio);
}
```

## Compilación y Ejecución

### Prerrequisitos
- Compilador C (gcc, clang)
- CMake 3.16+
- Criterion (para tests): `brew install criterion` (macOS)

### Compilar
```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar
```bash
# Programa principal (versión avanzada)
./escribir_registros_binario

# Versión básica del usuario
./demo_basico
```

### Ejecutar Tests
```bash
make test
# o directamente
./test_escribir_registros_binario
```

### Herramientas de Análisis
```bash
# Limpiar archivos de prueba
make clean_test_files

# Inspeccionar archivos binarios con hexdump
make inspect_files

# Comparar tamaños de archivos
make compare_sizes
```

## Salida Esperada

### Programa Principal
```
=== Escritura de Registros Binarios ===

Productos a escribir:
  Producto { código: 101, precio: 12.99 }
  Producto { código: 102, precio: 8.50 }
  Producto { código: 103, precio: 15.75 }
  Producto { código: 104, precio: 25.30 }
  Producto { código: 105, precio: 7.95 }

✅ Escritura básica exitosa:
=== Estadísticas de Escritura ===
Elementos escritos: 5
Bytes escritos: 40
Operación exitosa: Sí
================================

✅ Escritura con validación exitosa (con checksum)
✅ Escritura con header exitosa

Productos extendidos:
ProductoExtendido {
  código: 201
  precio: 45.99
  nombre: "Laptop Gaming"
  stock: 10
  categoría: "Electrónicos"
  descuento: 10.00%
}
...

=== Archivos generados ===
- productos.dat (básico + producto anexado)
- productos_validados.dat (con checksum)
- productos_con_header.dat (con metadatos)
- productos_extendidos.dat (estructura extendida)
```

### Inspección Hexadecimal
```bash
# hexdump -C productos.dat
00000000  65 00 00 00 c3 f5 4f 41  66 00 00 00 00 00 08 41  |e.....OAf......A|
00000010  67 00 00 00 00 00 7c 41  68 00 00 00 33 33 ca 41  |g.....|Ah...33.A|
00000020  69 00 00 00 cd cc fe 40                           |i......@|
```

## Formatos de Archivos Generados

### 1. productos.dat (Básico)
```
[Producto][Producto][Producto]...
```

### 2. productos_validados.dat (Con Checksum)
```
[Checksum(4 bytes)][Producto][Producto][Producto]...
```

### 3. productos_con_header.dat (Con Header)
```
[HeaderArchivo][Producto][Producto][Producto]...
```

### 4. productos_extendidos.dat (Estructura Extendida)
```
[ProductoExtendido][ProductoExtendido]...
```

## Conceptos Educativos

### 1. Representación Binaria de Estructuras
```c
// En memoria:
// int codigo (4 bytes) + float precio (4 bytes) = 8 bytes total
// Orden: Little-endian en x86/x64
```

### 2. Alineación de Memoria (Padding)
```c
typedef struct {
    char c;      // 1 byte
    int i;       // 4 bytes, pero puede empezar en offset 4 (padding de 3 bytes)
    double d;    // 8 bytes, alineado
} EjemploPadding; // Tamaño total puede ser 16 bytes, no 13
```

### 3. Portabilidad de Archivos Binarios
- **Endianness**: Orden de bytes en enteros/floats
- **Tamaño de tipos**: `int` puede ser 2, 4 u 8 bytes según plataforma
- **Alineación**: Compiladores pueden añadir padding

### 4. Ventajas de Archivos Binarios
- **Eficiencia**: Acceso directo sin parsing
- **Tamaño**: Más compactos que texto
- **Velocidad**: Lectura/escritura más rápida
- **Tipos nativos**: Preservan representación exacta

### 5. Desventajas de Archivos Binarios
- **Portabilidad**: Dependientes de arquitectura
- **Legibilidad**: No human-readable
- **Debugging**: Requieren herramientas especiales
- **Versionado**: Cambios de estructura rompen compatibilidad

## Extensiones y Variantes

### 1. Índices y Catálogos
```c
// Escribir índice al final del archivo
typedef struct {
    size_t offset;    // Posición del registro
    int codigo;       // Código para búsqueda rápida
} IndiceRegistro;
```

### 2. Compresión Simple
```c
// Usar técnicas de empaquetado de bits
typedef struct __attribute__((packed)) {
    int codigo;
    float precio;
} ProductoPacked; // Sin padding
```

### 3. Versionado de Formatos
```c
#define FORMATO_VERSION_1 1
#define FORMATO_VERSION_2 2

typedef struct {
    int version;
    // Campos según versión
} HeaderVersionado;
```

## Herramientas de Desarrollo

### Análisis de Archivos Binarios
```bash
# Ver contenido hexadecimal
hexdump -C archivo.dat

# Ver como enteros
od -t d4 archivo.dat

# Ver como floats
od -t f4 archivo.dat

# Información del archivo
file archivo.dat
stat archivo.dat
```

### Debugging
```bash
# Con GDB
gdb ./escribir_registros_binario
(gdb) break escribir_productos_basico
(gdb) run
(gdb) print productos[0]

# Con Valgrind
valgrind --leak-check=full ./escribir_registros_binario
```

## Aplicaciones Prácticas

1. **Bases de datos simples**: Almacenamiento eficiente de registros
2. **Formatos de archivo personalizados**: Configuraciones, logs estructurados
3. **Intercambio de datos**: Entre aplicaciones C/C++
4. **Serialización**: Persistencia de estructuras de datos
5. **Sistemas embebidos**: Almacenamiento eficiente en memoria flash

## Troubleshooting

### Problemas Comunes
1. **Tamaño incorrecto**: Verificar `sizeof(estructura)`
2. **Padding inesperado**: Usar `__attribute__((packed))` si necesario
3. **Corrupción de datos**: Validar antes de escribir
4. **Permisos de archivo**: Verificar modo de apertura y permisos del sistema

### Mejores Prácticas
1. **Siempre validar** entrada antes de escribir
2. **Verificar retorno** de `fwrite`
3. **Usar checksums** para integridad
4. **Documentar formato** de archivo
5. **Considerar portabilidad** desde el diseño

## Referencias

- Manual de `fwrite`: `man 3 fwrite`
- Estándar C99: ISO/IEC 9899:1999
- "The C Programming Language" - Kernighan & Ritchie
- "Expert C Programming" - Peter van der Linden
