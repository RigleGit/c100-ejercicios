# Ejercicio 094: Entrada Segura de Datos

## Descripción

Este ejercicio demuestra la importancia crítica de usar funciones seguras para la entrada de datos en C, contrastando las funciones inseguras como `gets()` con alternativas seguras como `fgets()`. Se enfoca en prevenir vulnerabilidades de **buffer overflow**, una de las causas más comunes de exploits de seguridad.

## ¿Por qué es crítica la entrada segura?

La entrada insegura de datos es una de las principales fuentes de vulnerabilidades en software. Los problemas más comunes incluyen:

- **Buffer Overflow**: Escribir más datos de los que un buffer puede contener
- **Corrupción de memoria**: Sobrescribir datos importantes en memoria
- **Ejecución de código arbitrario**: Permitir a atacantes ejecutar código malicioso
- **Denial of Service**: Crashes del programa por datos malformados

### Funciones Peligrosas vs Seguras

| Función Peligrosa | ¿Por qué es peligrosa? | Alternativa Segura | Beneficio |
|-------------------|------------------------|-------------------|-----------|
| `gets(buffer)` | No verifica límites | `fgets(buffer, size, stdin)` | Especifica límite máximo |
| `scanf("%s", buf)` | Sin límite de caracteres | `scanf("%49s", buf)` | Limita caracteres leídos |
| `getchar()` en bucle | Sin verificación de límites | `fgets()` con verificación | Control total del tamaño |

## Estructura del Proyecto

```
094-entrada-segura/
├── include/
│   └── entrada_segura.h           # Declaraciones y tipos
├── src/
│   ├── entrada_segura.c           # Implementación de funciones
│   └── main.c                     # Programa principal interactivo
├── tests/
│   └── test_entrada_segura.c      # Suite de tests con Criterion
├── tools/                         # Herramientas auxiliares
├── CMakeLists.txt                 # Configuración de compilación
├── README.md                      # Este archivo
└── .gitignore                     # Archivos a ignorar por Git
```

## Funciones Implementadas

### Funciones Inseguras (Solo para demostración)

#### 1. `entrada_insegura_gets()`
```c
char* entrada_insegura_gets(char* buffer);
```
- **⚠️ PELIGRO**: Simula el comportamiento peligroso de `gets()`
- **Problema**: No verifica límites del buffer
- **Uso**: Solo para demostración educativa

#### 2. `entrada_insegura_scanf()`
```c
int entrada_insegura_scanf(char* buffer);
```
- **⚠️ PELIGRO**: Usa `scanf("%s")` sin límites
- **Problema**: Puede desbordar cualquier buffer
- **Uso**: Solo para demostración educativa

### Funciones Seguras

#### 1. `entrada_segura_fgets()`
```c
resultado_entrada_t entrada_segura_fgets(char* buffer, size_t tamaño, FILE* archivo);
```
- **Propósito**: Wrapper seguro de `fgets()`
- **Garantías**: Nunca excede límites del buffer
- **Retorna**: Estado de la operación (exitosa, truncada, error, EOF)

#### 2. `entrada_segura_linea_limpia()`
```c
resultado_entrada_t entrada_segura_linea_limpia(char* buffer, size_t tamaño, FILE* archivo);
```
- **Propósito**: Lee línea y elimina `\n` final automáticamente
- **Ventajas**: Maneja truncamiento y limpia el stream
- **Uso típico**: Entrada de texto limpio sin caracteres de control

#### 3. `entrada_segura_validada()`
```c
resultado_entrada_t entrada_segura_validada(char* buffer, size_t tamaño, 
                                           FILE* archivo, estadisticas_entrada_t* stats);
```
- **Propósito**: Entrada con validación completa y estadísticas
- **Características**: Recopila métricas detalladas de la operación
- **Uso avanzado**: Debugging y análisis de entrada

## Tipos de Datos

### `resultado_entrada_t`
```c
typedef enum {
    ENTRADA_EXITOSA = 0,    // Lectura exitosa completa
    ENTRADA_TRUNCADA = 1,   // Entrada fue truncada por límites
    ENTRADA_ERROR = -1,     // Error en la operación
    ENTRADA_EOF = -2        // Fin de archivo alcanzado
} resultado_entrada_t;
```

### `estadisticas_entrada_t`
```c
typedef struct {
    size_t caracteres_leidos;    // Número de caracteres leídos
    size_t lineas_procesadas;    // Número de líneas procesadas
    bool truncamiento_ocurrido;  // Si ocurrió truncamiento
    int codigo_ultimo_error;     // Código del último error
} estadisticas_entrada_t;
```

## Compilación

### Opción 1: Usando CMake (Recomendado)
```bash
# Desde el directorio del ejercicio
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./entrada_segura

# Ejecutar tests
./test_entrada_segura
```

### Opción 2: Compilación Manual
```bash
# Programa principal
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/*.c -o entrada_segura

# Tests (requiere Criterion)
gcc -std=c99 -Wall -Wextra -Iinclude src/entrada_segura.c tests/test_entrada_segura.c -lcriterion -o test_entrada_segura
```

## Uso del Programa

### Menú Principal
```bash
./entrada_segura
```

El programa ofrece las siguientes opciones:

1. **🔴 Demostrar vulnerabilidad de gets()** - Muestra por qué `gets()` es peligroso
2. **🟢 Demostrar seguridad de fgets()** - Muestra cómo `fgets()` previene problemas
3. **⚖️ Comparación lado a lado** - Contrasta ambos enfoques directamente
4. **🎓 Demostración completa educativa** - Tutorial completo paso a paso
5. **🧪 Pruebas interactivas** - Permite probar las funciones manualmente
6. **📚 Información teórica** - Explicación detallada de conceptos

### Tests Automatizados
```bash
./test_entrada_segura
```

Los tests verifican:
- ✅ Funcionamiento correcto con entrada normal
- ✅ Manejo adecuado de truncamiento
- ✅ Preservación de null-termination
- ✅ Robustez con parámetros inválidos
- ✅ Manejo de casos extremos (EOF, líneas vacías, caracteres especiales)

## Ejemplos de Uso

### Ejemplo Básico: Entrada Segura
```c
#include "entrada_segura.h"

int main() {
    char buffer[100];
    
    printf("Ingrese su nombre: ");
    resultado_entrada_t resultado = entrada_segura_linea_limpia(buffer, sizeof(buffer), stdin);
    
    switch (resultado) {
        case ENTRADA_EXITOSA:
            printf("Hola, %s!\n", buffer);
            break;
        case ENTRADA_TRUNCADA:
            printf("Nombre demasiado largo, truncado a: %s\n", buffer);
            break;
        case ENTRADA_ERROR:
            printf("Error al leer entrada\n");
            break;
    }
    
    return 0;
}
```

### Ejemplo Avanzado: Con Estadísticas
```c
#include "entrada_segura.h"

int main() {
    char buffer[50];
    estadisticas_entrada_t stats;
    
    inicializar_estadisticas(&stats);
    
    printf("Ingrese texto: ");
    resultado_entrada_t resultado = entrada_segura_validada(buffer, sizeof(buffer), stdin, &stats);
    
    printf("Resultado: %s\n", resultado_a_string(resultado));
    printf("Texto leído: \"%s\"\n", buffer);
    imprimir_estadisticas(&stats);
    
    return 0;
}
```

## Conceptos Clave Demostrados

### 1. **Buffer Overflow Prevention**
- Las funciones seguras siempre verifican límites
- Nunca escriben más allá del tamaño especificado
- Garantizan null-termination correcta

### 2. **Error Handling Robusto**
- Códigos de retorno claros y descriptivos
- Manejo graceful de casos extremos
- Información detallada sobre el estado de la operación

### 3. **Validación de Entrada**
- Verificación de parámetros antes del uso
- Detección y reporte de condiciones anómalas
- Estadísticas para análisis y debugging

### 4. **Defensive Programming**
- Asunción de que la entrada puede ser maliciosa
- Validación en múltiples niveles
- Fallback seguro en caso de error

## Lecciones de Seguridad

### ❌ NUNCA Hacer:
```c
char buffer[50];
gets(buffer);                    // PELIGROSO: sin límites
scanf("%s", buffer);            // PELIGROSO: sin límites
char* p = buffer;
while ((c = getchar()) != '\n') // PELIGROSO: sin verificación
    *p++ = c;
```

### ✅ SIEMPRE Hacer:
```c
char buffer[50];
fgets(buffer, sizeof(buffer), stdin);           // SEGURO: con límite
scanf("%49s", buffer);                          // SEGURO: límite especificado
entrada_segura_linea_limpia(buffer, sizeof(buffer), stdin); // MEJOR: wrapper seguro
```

## Casos de Estudio Históricos

### Morris Worm (1988)
- **Problema**: `gets()` en el demonio `fingerd`
- **Explotación**: Buffer overflow para ejecutar código arbitrario
- **Impacto**: 6,000 computadoras infectadas (10% de Internet)
- **Lección**: La importancia de validar entrada

### Code Red Worm (2001)
- **Problema**: Buffer overflow en IIS
- **Explotación**: URL excesivamente larga
- **Impacto**: 359,000 servidores comprometidos
- **Lección**: Validar toda entrada externa

## Mejores Prácticas

1. **🛡️ Principio de Defensa en Profundidad**
   - Validar en múltiples niveles
   - Asumir que la entrada es hostil
   - Usar múltiples mecanismos de protección

2. **📏 Siempre Especificar Límites**
   - Usar funciones que requieren tamaño de buffer
   - Verificar límites antes de escribir
   - Documentar tamaños de buffer en código

3. **🔍 Validar y Sanitizar**
   - Verificar formato de entrada
   - Eliminar o escapar caracteres peligrosos
   - Usar listas blancas en lugar de listas negras

4. **📊 Monitorear y Loggear**
   - Registrar intentos de entrada anómala
   - Implementar alertas para patrones sospechosos
   - Usar estadísticas para detectar ataques

## Herramientas de Análisis

### Compilador
```bash
gcc -Wall -Wextra -Werror -fstack-protector-strong
```

### Análisis Estático
```bash
clang-tidy *.c
cppcheck *.c
```

### Análisis Dinámico
```bash
valgrind --tool=memcheck ./entrada_segura
```

### Sanitizers
```bash
gcc -fsanitize=address -fsanitize=undefined
```

## Dependencias

- **Compiler**: GCC o Clang con soporte C99
- **CMake**: 3.10 o superior (para compilación con CMake)
- **Criterion**: Para ejecutar los tests (opcional)

## Recursos Adicionales

- [OWASP Input Validation](https://owasp.org/www-community/vulnerabilities/Input_Validation)
- [CERT C Secure Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/STR31-C.+Guarantee+that+storage+for+strings+has+sufficient+space+for+character+data+and+the+null+terminator)
- [CWE-120: Buffer Copy without Checking Size](https://cwe.mitre.org/data/definitions/120.html)
- [Secure Programming HOWTO](https://dwheeler.com/secure-programs/)