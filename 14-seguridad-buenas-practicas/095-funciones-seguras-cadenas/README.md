# Ejercicio 095: Funciones Seguras de Cadenas

## Descripción

Este ejercicio demuestra la importancia de usar funciones seguras para la manipulación de cadenas en C, comparando las funciones inseguras tradicionales (`strcpy`, `strcat`, `sprintf`) con sus alternativas seguras (`strncpy`, `strncat`, `snprintf`).

## ¿Por qué es importante la seguridad en el manejo de cadenas?

Las funciones tradicionales de manipulación de cadenas en C son propensas a vulnerabilidades de seguridad, especialmente **desbordamiento de buffer (buffer overflow)**. Estos problemas pueden llevar a:

- **Corrupción de memoria**: Sobrescribir datos importantes
- **Vulnerabilidades de seguridad**: Posibles ataques de ejecución de código arbitrario
- **Comportamiento indefinido**: Crashes del programa
- **Dificultad de depuración**: Errores difíciles de rastrear

### Funciones Inseguras vs Seguras

| Función Insegura | Problema | Alternativa Segura | Beneficio |
|------------------|----------|-------------------|-----------|
| `strcpy()` | No verifica límites del buffer | `strncpy()` | Limita caracteres copiados |
| `strcat()` | No verifica límites del buffer | `strncat()` | Limita caracteres concatenados |
| `sprintf()` | Puede desbordar el buffer | `snprintf()` | Limita el tamaño del buffer |
| `gets()` | Extremadamente inseguro | `fgets()` | Especifica límite de lectura |

## Estructura del Proyecto

```
095-funciones-seguras-cadenas/
├── include/
│   └── funciones_seguras_cadenas.h  # Declaraciones de funciones
├── src/
│   ├── funciones_seguras_cadenas.c  # Implementación de funciones seguras
│   └── main.c                       # Programa principal con ejemplos
├── tests/
│   └── test_funciones_seguras_cadenas.c  # Suite de tests con Criterion
├── tools/                           # Herramientas auxiliares (vacío por ahora)
├── CMakeLists.txt                   # Configuración de compilación
├── README.md                        # Este archivo
└── .gitignore                       # Archivos a ignorar por Git
```

## Funciones Implementadas

### 1. `strncpy_seguro()`
```c
int strncpy_seguro(char* destino, const char* origen, size_t tamaño_buffer);
```
- **Propósito**: Copia una cadena de forma segura
- **Garantías**: Siempre null-termina la cadena destino
- **Retorno**: 0 si exitoso, -1 si se truncó

### 2. `strncat_seguro()`
```c
int strncat_seguro(char* destino, const char* origen, size_t tamaño_buffer);
```
- **Propósito**: Concatena cadenas de forma segura
- **Garantías**: No desborda el buffer, mantiene null-termination
- **Retorno**: 0 si exitoso, -1 si se truncó

### 3. `snprintf_seguro()`
```c
int snprintf_seguro(char* buffer, size_t tamaño, const char* formato, ...);
```
- **Propósito**: Formateo seguro de cadenas
- **Garantías**: Nunca desborda el buffer
- **Retorno**: 0 si exitoso, -1 si se truncó

### 4. `demostracion_completa()`
```c
int demostracion_completa(char* resultado, size_t tamaño, const char* nombre, int edad);
```
- **Propósito**: Ejemplo que combina todas las funciones seguras
- **Demuestra**: Uso práctico de las funciones en un escenario real

## Compilación

### Opción 1: Usando CMake (Recomendado)
```bash
# Desde el directorio del ejercicio
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./funciones_seguras_cadenas

# Ejecutar tests
./test_funciones_seguras_cadenas
```

### Opción 2: Compilación Manual
```bash
# Programa principal
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/*.c -o funciones_seguras_cadenas

# Tests (requiere Criterion)
gcc -std=c99 -Wall -Wextra -Iinclude src/funciones_seguras_cadenas.c tests/test_funciones_seguras_cadenas.c -lcriterion -o test_funciones_seguras_cadenas
```

## Uso

### Programa Principal
```bash
./funciones_seguras_cadenas
```

El programa principal muestra:
1. **Ejemplos de vulnerabilidades** con funciones inseguras
2. **Implementaciones seguras** de las mismas operaciones
3. **Comparaciones lado a lado** del comportamiento
4. **Casos extremos** y manejo de errores

### Tests
```bash
./test_funciones_seguras_cadenas
```

Los tests verifican:
- ✅ Funcionamiento correcto en casos normales
- ✅ Manejo adecuado de truncamiento
- ✅ Preservación de null-termination
- ✅ Robustez con entradas inválidas
- ✅ Casos extremos (buffers pequeños, cadenas vacías)

## Ejemplos de Uso

### Ejemplo Básico
```c
#include "funciones_seguras_cadenas.h"

int main() {
    char buffer[50];
    
    // Copia segura
    if (strncpy_seguro(buffer, "Hola mundo", sizeof(buffer)) == 0) {
        printf("Copia exitosa: %s\n", buffer);
    }
    
    // Concatenación segura
    if (strncat_seguro(buffer, "!", sizeof(buffer)) == 0) {
        printf("Concatenación exitosa: %s\n", buffer);
    }
    
    // Formateo seguro
    char resultado[100];
    if (snprintf_seguro(resultado, sizeof(resultado), 
                       "Buffer: %s (longitud: %zu)", buffer, strlen(buffer)) == 0) {
        printf("Formateo exitoso: %s\n", resultado);
    }
    
    return 0;
}
```

### Manejo de Errores
```c
char pequeño[5];
const char* cadena_larga = "Esta cadena es demasiado larga";

if (strncpy_seguro(pequeño, cadena_larga, sizeof(pequeño)) == -1) {
    printf("Advertencia: La cadena fue truncada\n");
    printf("Resultado truncado: %s\n", pequeño);
}
```

## Conceptos Clave Demostrados

### 1. **Buffer Overflow Prevention**
- Las funciones siempre respetan los límites del buffer
- Nunca escriben más allá del tamaño especificado

### 2. **Null Termination Guarantee**
- Todas las funciones garantizan que las cadenas estén null-terminadas
- Esto previene lecturas fuera de los límites

### 3. **Error Reporting**
- Las funciones retornan códigos de error claros
- Permiten al programa manejar situaciones de truncamiento

### 4. **Defensive Programming**
- Validación de parámetros de entrada
- Manejo graceful de casos extremos

## Lecciones de Seguridad

1. **Nunca uses `strcpy`, `strcat`, `sprintf`** en código de producción
2. **Siempre especifica límites** de buffer en operaciones de cadenas
3. **Verifica códigos de retorno** para detectar truncamientos
4. **Usa herramientas de análisis estático** para detectar vulnerabilidades
5. **Considera usar bibliotecas seguras** como `strlcpy` si están disponibles

## Dependencias

- **Compiler**: GCC o Clang con soporte C99
- **CMake**: 3.10 o superior (para compilación con CMake)
- **Criterion**: Para ejecutar los tests (opcional)

## Recursos Adicionales

- [OWASP C/C++ Security Guide](https://owasp.org/www-pdf-archive/OWASP_C_Guide_v2.0_(English).pdf)
- [CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c)
- [Secure Coding in C and C++](https://www.amazon.com/Secure-Coding-2nd-Software-Engineering/dp/0321822137)
