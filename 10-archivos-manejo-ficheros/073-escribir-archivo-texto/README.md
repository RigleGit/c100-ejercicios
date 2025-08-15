# Ejercicio 073: Escribir Archivo de Texto

## Descripción

Este ejercicio implementa un sistema completo para escribir archivos de texto en C, incluyendo funciones básicas y avanzadas para el manejo de archivos, escritura con formato, concatenación, copia y obtención de información de archivos.

## Funcionalidades

### Funciones Básicas

1. **`escribir_archivo_texto()`** - Escribir texto simple a un archivo
2. **`escribir_multiples_lineas()`** - Escribir múltiples líneas de texto
3. **`escribir_archivo_formato()`** - Escritura con formato usando printf
4. **`obtener_info_archivo()`** - Obtener información del archivo
5. **`concatenar_archivos()`** - Concatenar dos archivos
6. **`copiar_archivo()`** - Copiar un archivo a otro destino

### Características Avanzadas

- **Manejo robusto de errores** con estructura `ResultadoEscritura`
- **Validación de parámetros** de entrada
- **Información detallada** de archivos (tamaño, fecha de modificación)
- **Operaciones de archivos** seguras y eficientes
- **Soporte para texto multilínea** y caracteres especiales

## Compilación

### Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar el programa principal
make run

# Ejecutar tests (requiere Criterion)
make test_run
```

### Compilación Manual

```bash
# Compilar biblioteca
gcc -Wall -Wextra -std=c99 -c src/escribir_archivo.c -I include -o escribir_archivo.o

# Compilar programa principal
gcc -Wall -Wextra -std=c99 escribir_archivo.o -o escribir_archivo

# Compilar tests (requiere Criterion)
gcc -Wall -Wextra -std=c99 tests/test_escribir_archivo.c escribir_archivo.o -I include -lcriterion -o test_escribir_archivo
```

## Uso

### Ejemplo Básico

```c
#include "escribir_archivo.h"
#include <stdio.h>

int main() {
    // Escribir texto simple
    ResultadoEscritura resultado = escribir_archivo_texto("saludo.txt", "¡Hola, mundo!");
    
    if (resultado.exito) {
        printf("Éxito: %s\n", resultado.mensaje);
    } else {
        printf("Error: %s\n", resultado.mensaje);
    }
    
    return 0;
}
```

### Ejemplo Avanzado

```c
#include "escribir_archivo.h"
#include <stdio.h>

int main() {
    // Escribir con formato
    ResultadoEscritura resultado = escribir_archivo_formato("datos.txt", 
        "Nombre: %s\nEdad: %d\nSalario: %.2f\n", 
        "Ana García", 28, 3500.75);
    
    // Obtener información del archivo
    InfoArchivo info = obtener_info_archivo("datos.txt");
    printf("Archivo creado - Tamaño: %ld bytes\n", info.tamaño);
    
    // Copiar archivo
    copiar_archivo("datos.txt", "copia_datos.txt");
    
    return 0;
}
```

## Estructura de Datos

### ResultadoEscritura

```c
typedef struct {
    bool exito;           // true si la operación fue exitosa
    char mensaje[256];    // Mensaje descriptivo del resultado
} ResultadoEscritura;
```

### InfoArchivo

```c
typedef struct {
    bool existe;              // true si el archivo existe
    long tamaño;             // Tamaño del archivo en bytes
    time_t fecha_modificacion; // Fecha de última modificación
} InfoArchivo;
```

## Tests

El proyecto incluye una suite completa de tests usando Criterion:

```bash
# Ejecutar todos los tests
./test_escribir_archivo

# Ejecutar con salida detallada
./test_escribir_archivo --verbose

# Ejecutar tests específicos
./test_escribir_archivo --pattern="escribir_basico"
```

### Casos de Prueba

- ✅ Escritura básica de texto
- ✅ Escritura de cadena vacía
- ✅ Manejo de parámetros NULL
- ✅ Escritura en rutas inexistentes
- ✅ Caracteres especiales y acentos
- ✅ Texto multilínea
- ✅ Concatenación de archivos
- ✅ Escritura con formato
- ✅ Información de archivos
- ✅ Copia de archivos
- ✅ Rendimiento con archivos grandes

## Conceptos Educativos

### 1. Manejo de Archivos en C

```c
FILE* archivo = fopen("archivo.txt", "w");  // Abrir para escritura
if (archivo == NULL) {
    // Manejar error
    return;
}

fprintf(archivo, "Texto a escribir");  // Escribir texto
fclose(archivo);  // Cerrar archivo
```

### 2. Modos de Apertura

- `"w"` - Escribir (trunca si existe)
- `"a"` - Agregar al final
- `"r+"` - Leer y escribir
- `"w+"` - Leer y escribir (trunca)

### 3. Funciones de Escritura

```c
// Escribir cadena
fputs("texto", archivo);

// Escribir con formato
fprintf(archivo, "Número: %d\n", 42);

// Escribir caracteres
fputc('A', archivo);

// Escribir bloque de datos
fwrite(datos, tamaño, cantidad, archivo);
```

### 4. Manejo de Errores

```c
FILE* archivo = fopen("archivo.txt", "w");
if (archivo == NULL) {
    perror("Error al abrir archivo");
    return -1;
}

if (fprintf(archivo, "datos") < 0) {
    perror("Error al escribir");
    fclose(archivo);
    return -1;
}
```

### 5. Información de Archivos

```c
#include <sys/stat.h>

struct stat info;
if (stat("archivo.txt", &info) == 0) {
    printf("Tamaño: %ld bytes\n", info.st_size);
    printf("Última modificación: %s", ctime(&info.st_mtime));
}
```

## Aplicaciones Prácticas

1. **Logging de aplicaciones** - Escribir registros de eventos
2. **Configuración de programas** - Guardar configuraciones
3. **Exportación de datos** - Generar reportes en texto
4. **Backup de información** - Respaldar datos importantes
5. **Generación de código** - Crear archivos de código automáticamente

## Notas de Implementación

- **Seguridad**: Validación exhaustiva de parámetros
- **Robustez**: Manejo completo de errores de E/O
- **Eficiencia**: Uso óptimo de buffers y operaciones de archivo
- **Portabilidad**: Compatible con sistemas POSIX
- **Pedagogía**: Código bien documentado y estructurado

## Recursos Adicionales

- [Manejo de archivos en C - Tutorial](https://www.cprogramming.com/tutorial/cfileio.html)
- [Funciones de E/O estándar](https://en.cppreference.com/w/c/io)
- [Manejo de errores en C](https://www.gnu.org/software/libc/manual/html_node/Error-Reporting.html)