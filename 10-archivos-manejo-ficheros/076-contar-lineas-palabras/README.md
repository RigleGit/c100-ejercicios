# Ejercicio 076: Contar Líneas, Palabras y Caracteres

## Descripción

Este ejercicio implementa un programa en C que lee un archivo de texto y cuenta cuántas líneas, palabras y caracteres contiene, imitando el comportamiento básico de la herramienta Unix `wc` (word count).

## Enunciado

Escribe un programa en C que lea un archivo de texto (`entrada.txt`) y cuente cuántas líneas, palabras y caracteres contiene, imitando el comportamiento básico de la herramienta `wc`. Se deben considerar:

- Una palabra como una secuencia de caracteres separada por espacios o saltos de línea
- Una línea termina con el carácter `\n`

Este ejercicio aplica lectura de archivos, manejo de caracteres, y lógica de control para conteo.

## Conceptos Clave

### 1. Manejo de Archivos
```c
FILE *archivo = fopen("entrada.txt", "r");
if (archivo == NULL) {
    perror("Error al abrir archivo");
    return 1;
}
```

### 2. Lectura Carácter por Carácter
```c
char c;
while ((c = fgetc(archivo)) != EOF) {
    // Procesar carácter
}
```

### 3. Lógica de Conteo
- **Líneas**: Incrementar cuando se encuentra `\n`
- **Palabras**: Usar bandera para detectar transiciones espacio→carácter
- **Caracteres**: Contar cada carácter leído

### 4. Manejo de Estados
```c
int en_palabra = 0;
if (isspace(c)) {
    en_palabra = 0;
} else if (!en_palabra) {
    en_palabra = 1;
    palabras++;
}
```

## Implementación

### Estructura del Proyecto
```
076-contar-lineas-palabras/
├── include/
│   └── contar_lineas_palabras.h    # Declaraciones y estructuras
├── src/
│   └── contar_lineas_palabras.c    # Implementación
├── tests/
│   └── test_contar_lineas_palabras.c # Tests unitarios
├── CMakeLists.txt                   # Configuración de build
└── README.md                        # Este archivo
```

### Funciones Principales

#### 1. Versión Básica del Usuario
```c
int main_usuario_basico() {
    FILE *archivo = fopen("entrada.txt", "r");
    int caracteres = 0, palabras = 0, lineas = 0;
    int en_palabra = 0;
    char c;

    while ((c = fgetc(archivo)) != EOF) {
        caracteres++;
        if (c == '\n') lineas++;
        if (isspace(c)) {
            en_palabra = 0;
        } else if (!en_palabra) {
            en_palabra = 1;
            palabras++;
        }
    }
    // Mostrar resultados
}
```

#### 2. Versión Modular
```c
bool contar_archivo_basico(const char* nombre_archivo, EstadisticasArchivo* stats);
bool contar_archivo_completo(const char* nombre_archivo, EstadisticasArchivo* stats);
bool contar_palabras_robusto(const char* nombre_archivo, EstadisticasArchivo* stats);
```

### Estructura de Datos
```c
typedef struct {
    int lineas;       // Número de líneas
    int palabras;     // Número de palabras
    int caracteres;   // Número de caracteres
    int bytes;        // Número de bytes (para análisis completo)
} EstadisticasArchivo;
```

## Algoritmos Implementados

### 1. Algoritmo Básico
- Lectura carácter por carácter con `fgetc()`
- Estado simple para detectar palabras
- Conteo directo de líneas y caracteres

### 2. Algoritmo Robusto
- Lectura por líneas con `fgets()`
- Manejo mejorado de caracteres especiales
- Consideración de guiones y guiones bajos como parte de palabras

### 3. Análisis Completo
- Modo binario para conteo exacto de bytes
- Distinción entre caracteres válidos y bytes
- Validación de caracteres imprimibles

## Salida Esperada

### Formato Estándar
```
=== Estadísticas del archivo ===
Archivo: entrada.txt
Líneas:     3
Palabras:   6
Caracteres: 33
============================
```

### Formato wc
```
       3        6       33 entrada.txt
```

## Variantes y Extensiones

### 1. Comparación con wc Unix
```bash
# Nuestro programa
./contar_lineas_palabras

# Comando wc real
wc entrada.txt
```

### 2. Análisis de Múltiples Archivos
```c
for (int i = 1; i < argc; i++) {
    contar_archivo_basico(argv[i], &stats);
    imprimir_formato_wc(&stats, argv[i]);
}
```

### 3. Opciones de Línea de Comandos
- `-l`: Solo líneas
- `-w`: Solo palabras  
- `-c`: Solo caracteres
- `-m`: Solo bytes

## Conceptos Educativos

### 1. Máquinas de Estado
El conteo de palabras implementa una máquina de estado simple:
```
Estado: FUERA_PALABRA -> (carácter no-espacio) -> DENTRO_PALABRA
Estado: DENTRO_PALABRA -> (espacio) -> FUERA_PALABRA
```

### 2. Manejo de Archivos de Texto vs Binario
```c
fopen("archivo.txt", "r");  // Modo texto
fopen("archivo.txt", "rb"); // Modo binario
```

### 3. Funciones de Clasificación de Caracteres
```c
#include <ctype.h>
isspace(c);  // Espacios en blanco
isalnum(c);  // Alfanumérico
isprint(c);  // Imprimible
```

### 4. Optimización y Rendimiento
- Lectura por bloques vs carácter por carácter
- Uso de buffering del sistema
- Análisis de complejidad O(n)

## Aplicaciones Prácticas

1. **Análisis de documentos**: Estadísticas de texto
2. **Herramientas de desarrollo**: Métricas de código
3. **Procesamiento de logs**: Análisis de archivos de registro
4. **Validación de entrada**: Verificar límites de texto

## Referencias

- Manual de `wc`: `man wc`
- Estándar C99: ISO/IEC 9899:1999
- "The C Programming Language" - Kernighan & Ritchie
- POSIX.1-2008: Definiciones de archivos de texto

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/contar_lineas_palabras
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [test/test_contador.c](test/test_contador.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
