# Ejercicio 074: Leer Archivo de Texto

## Descripción

Este ejercicio implementa un sistema completo para leer archivos de texto en C, incluyendo funciones básicas y avanzadas para el manejo de archivos, lectura línea por línea, análisis de contenido, búsqueda de patrones y procesamiento de texto.

## Funcionalidades

### Funciones Básicas de Lectura

1. **`leer_archivo_basico()`** - Lectura simple línea por línea
2. **`leer_archivo_numerado()`** - Lectura con números de línea
3. **`leer_archivo_configurado()`** - Lectura con configuración personalizada
4. **`cargar_archivo_en_memoria()`** - Cargar todo el archivo en memoria
5. **`obtener_estadisticas_archivo()`** - Obtener estadísticas del archivo

### Funciones de Análisis y Búsqueda

6. **`buscar_palabra_en_archivo()`** - Buscar palabra específica
7. **`contar_frecuencia_palabras()`** - Contar frecuencia de palabras
8. **`filtrar_lineas_por_patron()`** - Filtrar líneas que coinciden con patrón
9. **`leer_rango_lineas()`** - Leer rango específico de líneas
10. **`comparar_archivos()`** - Comparar dos archivos línea por línea

### Características Avanzadas

- **Manejo robusto de errores** con estructura `ResultadoLectura`
- **Configuración flexible** de lectura (`ConfiguracionLectura`)
- **Análisis estadístico** completo de archivos
- **Búsqueda sensible/insensible** a mayúsculas
- **Gestión eficiente de memoria** para archivos grandes
- **Interfaz interactiva** con menú de opciones

## Compilación

### Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Crear archivos de ejemplo
make create_examples

# Ejecutar el programa principal
make run

# Ejecutar tests (requiere Criterion)
make test_run
```

### Compilación Manual

```bash
# Compilar biblioteca
gcc -Wall -Wextra -std=c99 -c src/leer_archivo.c -I include -o leer_archivo.o

# Compilar programa principal
gcc -Wall -Wextra -std=c99 leer_archivo.o -o leer_archivo

# Compilar tests (requiere Criterion)
gcc -Wall -Wextra -std=c99 tests/test_leer_archivo.c leer_archivo.o -I include -lcriterion -o test_leer_archivo
```

## Uso

### Ejemplo Básico (del Enunciado)

```c
#include <stdio.h>

int main() {
    char linea[256];
    FILE *f = fopen("salida.txt", "r");

    if (f == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    printf("Contenido del archivo:\n");
    while (fgets(linea, sizeof(linea), f) != NULL) {
        printf("%s", linea);
    }

    fclose(f);
    return 0;
}
```

### Ejemplo Avanzado con Funciones del Ejercicio

```c
#include "leer_archivo.h"
#include <stdio.h>

int main() {
    // Lectura básica
    ResultadoLectura resultado = leer_archivo_basico("archivo.txt");
    
    if (resultado.exito) {
        printf("Archivo leído: %d líneas, %d caracteres\n", 
               resultado.lineas_leidas, resultado.caracteres_leidos);
    }
    
    // Obtener estadísticas
    EstadisticasArchivo stats = obtener_estadisticas_archivo("archivo.txt");
    if (stats.existe) {
        printf("Estadísticas:\n");
        printf("  Tamaño: %ld bytes\n", stats.tamaño);
        printf("  Líneas: %d\n", stats.numero_lineas);
        printf("  Palabras: %d\n", stats.numero_palabras);
        printf("  Caracteres: %d\n", stats.numero_caracteres);
    }
    
    // Buscar palabra
    buscar_palabra_en_archivo("archivo.txt", "palabra", false);
    
    return 0;
}
```

### Ejemplo con Configuración Personalizada

```c
#include "leer_archivo.h"

int main() {
    ConfiguracionLectura config = {
        .mostrar_numeros_linea = true,
        .ignorar_lineas_vacias = true,
        .convertir_a_mayusculas = false,
        .mostrar_estadisticas = true,
        .max_lineas_mostrar = 10
    };
    
    ResultadoLectura resultado = leer_archivo_configurado("archivo.txt", &config);
    
    return 0;
}
```

## Estructuras de Datos

### ResultadoLectura

```c
typedef struct {
    bool exito;              // true si la operación fue exitosa
    char mensaje[256];       // Mensaje descriptivo del resultado
    int lineas_leidas;       // Número de líneas procesadas
    int caracteres_leidos;   // Número de caracteres leídos
} ResultadoLectura;
```

### EstadisticasArchivo

```c
typedef struct {
    bool existe;                  // true si el archivo existe
    long tamaño;                 // Tamaño del archivo en bytes
    int numero_lineas;           // Número de líneas en el archivo
    int numero_palabras;         // Número de palabras en el archivo
    int numero_caracteres;       // Número de caracteres (sin espacios)
    time_t fecha_modificacion;   // Fecha de última modificación
} EstadisticasArchivo;
```

### ConfiguracionLectura

```c
typedef struct {
    bool mostrar_numeros_linea;   // Mostrar números de línea
    bool ignorar_lineas_vacias;   // Omitir líneas vacías
    bool convertir_a_mayusculas;  // Convertir a mayúsculas
    bool mostrar_estadisticas;    // Mostrar estadísticas al final
    int max_lineas_mostrar;       // Máximo de líneas a mostrar (-1 = todas)
} ConfiguracionLectura;
```

## Tests

El proyecto incluye una suite completa de tests usando Criterion:

```bash
# Ejecutar todos los tests
./test_leer_archivo

# Ejecutar con salida detallada
./test_leer_archivo --verbose

# Ejecutar tests específicos
make test_basicos    # Tests de lectura básica
make test_avanzados  # Tests de funciones avanzadas
```

### Casos de Prueba

- ✅ Lectura básica de archivo
- ✅ Lectura de archivo inexistente
- ✅ Manejo de parámetros NULL
- ✅ Lectura de archivo vacío
- ✅ Lectura con números de línea
- ✅ Lectura con configuración personalizada
- ✅ Carga de archivo en memoria
- ✅ Obtención de estadísticas
- ✅ Búsqueda de palabras (sensible/insensible)
- ✅ Conteo de frecuencia de palabras
- ✅ Filtrado de líneas por patrón
- ✅ Lectura de rangos de líneas
- ✅ Comparación de archivos
- ✅ Manejo de memoria y liberación
- ✅ Configuraciones especiales (ignorar vacías, mayúsculas)

## Conceptos Educativos

### 1. Lectura de Archivos en C

```c
FILE* archivo = fopen("archivo.txt", "r");  // Abrir para lectura
if (archivo == NULL) {
    perror("Error al abrir archivo");
    return;
}

char linea[256];
while (fgets(linea, sizeof(linea), archivo) != NULL) {
    printf("%s", linea);  // Procesar línea
}

fclose(archivo);  // Cerrar archivo
```

### 2. Modos de Apertura para Lectura

- `"r"` - Leer (archivo debe existir)
- `"r+"` - Leer y escribir (archivo debe existir)
- `"rb"` - Leer en modo binario

### 3. Funciones de Lectura

```c
// Leer línea completa
fgets(buffer, tamaño, archivo);

// Leer carácter individual
int c = fgetc(archivo);

// Leer bloque de datos
fread(buffer, tamaño_elemento, cantidad, archivo);

// Leer con formato
fscanf(archivo, "%d %s", &numero, cadena);
```

### 4. Control del Flujo de Lectura

```c
// Verificar final de archivo
if (feof(archivo)) {
    printf("Fin de archivo alcanzado\n");
}

// Verificar errores de lectura
if (ferror(archivo)) {
    printf("Error durante la lectura\n");
}

// Posicionarse en el archivo
fseek(archivo, 0, SEEK_SET);  // Ir al inicio
ftell(archivo);               // Obtener posición actual
```

### 5. Procesamiento de Texto

```c
// Dividir línea en palabras
char* palabra = strtok(linea, " \t\n");
while (palabra != NULL) {
    // Procesar palabra
    palabra = strtok(NULL, " \t\n");
}

// Búsqueda en texto
if (strstr(linea, "patrón") != NULL) {
    printf("Patrón encontrado\n");
}
```

### 6. Información de Archivos

```c
#include <sys/stat.h>

struct stat info;
if (stat("archivo.txt", &info) == 0) {
    printf("Tamaño: %ld bytes\n", info.st_size);
    printf("Última modificación: %s", ctime(&info.st_mtime));
}
```

## Aplicaciones Prácticas

1. **Análisis de logs** - Procesar archivos de registro
2. **Procesamiento de datos** - Leer archivos CSV, TSV
3. **Análisis de texto** - Contar palabras, buscar patrones
4. **Validación de archivos** - Verificar formato y contenido
5. **Herramientas de texto** - Implementar `grep`, `wc`, `head`, `tail`
6. **Conversión de formatos** - Leer un formato y escribir otro
7. **Backup y sincronización** - Comparar archivos para diferencias

## Funciones Interactivas

El ejercicio incluye un menú interactivo completo:

```bash
=== Menú de Lectura de Archivos ===
1. Lectura básica
2. Lectura con números de línea
3. Lectura configurada
4. Obtener estadísticas
5. Buscar palabra
6. Filtrar líneas
7. Leer rango de líneas
8. Cargar en memoria
0. Salir
```

## Notas de Implementación

- **Seguridad**: Validación exhaustiva de parámetros y manejo de errores
- **Robustez**: Control de EOF, errores de E/O y memoria insuficiente
- **Eficiencia**: Uso óptimo de buffers y lectura por chunks
- **Flexibilidad**: Configuración personalizable para diferentes necesidades
- **Pedagogía**: Código bien documentado con ejemplos educativos

## Recursos Adicionales

- [Lectura de archivos en C - Tutorial](https://www.cprogramming.com/tutorial/cfileio.html)
- [Funciones de E/O estándar](https://en.cppreference.com/w/c/io)
- [Manejo de archivos POSIX](https://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html)
- [Análisis de texto en C](https://www.geeksforgeeks.org/c-program-count-words-text-file/)