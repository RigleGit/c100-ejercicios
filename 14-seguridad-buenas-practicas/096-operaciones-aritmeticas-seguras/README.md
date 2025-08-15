# Ejercicio 096: Operaciones Aritméticas Seguras

## Descripción

Este ejercicio demuestra la implementación de operaciones aritméticas seguras con detección de **overflow/underflow**, un problema crítico de seguridad en programación C. Los overflow aritméticos pueden causar vulnerabilidades serias al permitir bypass de validaciones de seguridad y comportamiento indefinido.

## ¿Por qué son críticas las operaciones aritméticas seguras?

Los overflow/underflow aritméticos son una fuente común de vulnerabilidades en software, especialmente en:

- **Validaciones de tamaño**: `if (size + offset < buffer_size)` puede fallar si `size + offset` causa overflow
- **Cálculos de memoria**: `malloc(num_items * item_size)` puede allocar menos memoria de la esperada
- **Índices de arrays**: Overflow puede resultar en acceso fuera de límites
- **Operaciones financieras**: Errores en cálculos pueden tener consecuencias graves

### Problemas Principales

| Problema | Descripción | Consecuencia |
|----------|-------------|--------------|
| **Integer Overflow** | Resultado excede `INT_MAX` | Wraparound a valores negativos |
| **Integer Underflow** | Resultado menor que `INT_MIN` | Wraparound a valores positivos |
| **Comportamiento Indefinido** | C no define qué pasa en overflow | Resultados impredecibles |
| **Vulnerabilidades** | Bypass de validaciones | Explotación de seguridad |

## Estructura del Proyecto

```
096-operaciones-aritmeticas-seguras/
├── include/
│   └── operaciones_aritmeticas_seguras.h  # Declaraciones y tipos
├── src/
│   ├── operaciones_aritmeticas_seguras.c  # Implementación de funciones
│   └── main.c                             # Programa principal interactivo
├── tests/
│   └── test_operaciones_aritmeticas_seguras.c  # Suite de tests con Criterion
├── tools/                                 # Herramientas auxiliares
├── CMakeLists.txt                         # Configuración de compilación
├── README.md                              # Este archivo
└── .gitignore                             # Archivos a ignorar por Git
```

## Funciones Implementadas

### Operaciones Básicas Seguras

#### 1. `suma_segura()`
```c
resultado_aritmetica_t suma_segura(int a, int b, int* resultado);
```
- **Detección**: `b > 0 && a > INT_MAX - b` (overflow)
- **Detección**: `b < 0 && a < INT_MIN - b` (underflow)
- **Garantía**: Nunca produce overflow silencioso

#### 2. `resta_segura()`
```c
resultado_aritmetica_t resta_segura(int a, int b, int* resultado);
```
- **Detección**: `b < 0 && a > INT_MAX + b` (overflow)
- **Detección**: `b > 0 && a < INT_MIN + b` (underflow)
- **Uso**: `a - b` con verificación previa

#### 3. `multiplicacion_segura()`
```c
resultado_aritmetica_t multiplicacion_segura(int a, int b, int* resultado);
```
- **Casos especiales**: 0, 1, -1, `INT_MIN * -1`
- **Verificación**: División para detectar overflow
- **Manejo**: Signos diferentes y valores absolutos

#### 4. `division_segura()`
```c
resultado_aritmetica_t division_segura(int a, int b, int* resultado);
```
- **Verificación**: División por cero
- **Caso especial**: `INT_MIN / -1` produce overflow
- **Garantía**: Operación segura o error claro

#### 5. `modulo_seguro()`
```c
resultado_aritmetica_t modulo_seguro(int a, int b, int* resultado);
```
- **Verificación**: Módulo por cero
- **Caso especial**: `INT_MIN % -1` manejo correcto
- **Resultado**: Residuo matemáticamente correcto

### Operaciones para Otros Tipos

#### 1. `suma_segura_long()`, `suma_segura_uint()`, `suma_segura_size_t()`
- Versiones especializadas para diferentes tipos
- Manejo de límites específicos (`LLONG_MAX`, `UINT_MAX`, `SIZE_MAX`)
- Crítico para cálculos de memoria y arrays grandes

### Funciones de Validación

#### 1. `verificar_suma_segura()`, `verificar_multiplicacion_segura()`
- Verificación sin realizar la operación
- Útil para validaciones previas
- Overhead mínimo

#### 2. `calcular_rango_suma_segura()`
- Calcula rango seguro para un valor base
- Útil para validaciones de entrada
- Prevención proactiva

## Tipos de Datos

### `resultado_aritmetica_t`
```c
typedef enum {
    ARITMETICA_EXITOSA = 0,         // Operación exitosa
    ARITMETICA_OVERFLOW = 1,        // Overflow detectado
    ARITMETICA_UNDERFLOW = 2,       // Underflow detectado
    ARITMETICA_DIVISION_CERO = 3,   // División por cero
    ARITMETICA_ERROR_PARAMETRO = -1 // Error en parámetros
} resultado_aritmetica_t;
```

### `estadisticas_aritmetica_t`
```c
typedef struct {
    size_t operaciones_exitosas;    // Operaciones exitosas
    size_t overflows_detectados;    // Overflows detectados
    size_t underflows_detectados;   // Underflows detectados
    size_t divisiones_cero;         // Divisiones por cero
    size_t errores_parametro;       // Errores de parámetro
} estadisticas_aritmetica_t;
```

## Compilación

### Opción 1: Usando CMake (Recomendado)
```bash
# Desde el directorio del ejercicio
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./operaciones_aritmeticas_seguras

# Ejecutar tests
./test_operaciones_aritmeticas_seguras
```

### Opción 2: Compilación Manual
```bash
# Programa principal
gcc -std=c99 -Wall -Wextra -O2 -Iinclude src/*.c -o operaciones_aritmeticas_seguras

# Tests (requiere Criterion)
gcc -std=c99 -Wall -Wextra -Iinclude src/operaciones_aritmeticas_seguras.c tests/test_operaciones_aritmeticas_seguras.c -lcriterion -o test_operaciones_aritmeticas_seguras
```

## Uso del Programa

### Menú Principal
```bash
./operaciones_aritmeticas_seguras
```

El programa ofrece las siguientes opciones:

1. **🚨 Demostrar vulnerabilidades de overflow** - Muestra cómo los overflow causan problemas
2. **🛡️ Demostrar operaciones seguras** - Muestra la detección y prevención
3. **🧪 Ejecutar pruebas con casos límite** - Tests exhaustivos con valores extremos
4. **⚖️ Comparación: normal vs segura** - Contrasta ambos enfoques
5. **🎓 Demostración completa educativa** - Tutorial paso a paso
6. **🔧 Calculadora aritmética segura** - Calculadora interactiva con detección
7. **📊 Ver límites de tipos de datos** - Información de `INT_MAX`, `INT_MIN`, etc.
8. **🧮 Verificador de rangos seguros** - Herramienta para calcular rangos válidos

### Tests Automatizados
```bash
./test_operaciones_aritmeticas_seguras
```

Los tests verifican:
- ✅ Detección correcta de overflow/underflow
- ✅ Operaciones exitosas en casos válidos
- ✅ Manejo de casos especiales (`INT_MIN * -1`, división por cero)
- ✅ Robustez con parámetros inválidos
- ✅ Funcionamiento con diferentes tipos de datos

## Ejemplos de Uso

### Ejemplo Básico: Suma Segura
```c
#include "operaciones_aritmeticas_seguras.h"

int main() {
    int a = INT_MAX - 5;
    int b = 10;
    int resultado;
    
    resultado_aritmetica_t estado = suma_segura(a, b, &resultado);
    
    switch (estado) {
        case ARITMETICA_EXITOSA:
            printf("Resultado: %d\n", resultado);
            break;
        case ARITMETICA_OVERFLOW:
            printf("¡Overflow detectado! Operación cancelada por seguridad.\n");
            break;
        default:
            printf("Error: %s\n", resultado_aritmetica_a_string(estado));
    }
    
    return 0;
}
```

### Ejemplo Avanzado: Validación de Memoria
```c
#include "operaciones_aritmeticas_seguras.h"

bool allocar_memoria_seguro(size_t num_elementos, size_t tamaño_elemento) {
    size_t total_bytes;
    
    // Verificar que el cálculo no cause overflow
    if (suma_segura_size_t(num_elementos * tamaño_elemento, 0, &total_bytes) != ARITMETICA_EXITOSA) {
        printf("Error: Cálculo de memoria causaría overflow\n");
        return false;
    }
    
    // Solo ahora es seguro allocar
    void* ptr = malloc(total_bytes);
    if (ptr == NULL) {
        printf("Error: No se pudo allocar memoria\n");
        return false;
    }
    
    printf("Memoria allocada exitosamente: %zu bytes\n", total_bytes);
    free(ptr);
    return true;
}
```

### Ejemplo de Calculadora Segura
```c
#include "operaciones_aritmeticas_seguras.h"

void calculadora_interactiva() {
    int a, b, resultado;
    char op;
    
    printf("Ingrese operación (a op b): ");
    scanf("%d %c %d", &a, &op, &b);
    
    resultado_aritmetica_t estado;
    
    switch (op) {
        case '+':
            estado = suma_segura(a, b, &resultado);
            break;
        case '*':
            estado = multiplicacion_segura(a, b, &resultado);
            break;
        default:
            printf("Operador no soportado\n");
            return;
    }
    
    if (estado == ARITMETICA_EXITOSA) {
        printf("%d %c %d = %d\n", a, op, b, resultado);
    } else {
        printf("Error: %s\n", resultado_aritmetica_a_string(estado));
    }
}
```

## Casos de Estudio de Vulnerabilidades

### 1. Bypass de Validación de Tamaño
```c
// CÓDIGO VULNERABLE
unsigned int size = 0xFFFFFFF0;  // Muy grande
unsigned int offset = 0x20;      // 32 bytes
unsigned int buffer_size = 1000;

if (size + offset < buffer_size) {  // ¡OVERFLOW! Resultado es pequeño
    // ¡PELIGRO! Esta validación es burlada
    memcpy(buffer, data, size + offset);  // Buffer overflow
}

// CÓDIGO SEGURO
unsigned int total_size;
if (suma_segura_uint(size, offset, &total_size) == ARITMETICA_OVERFLOW) {
    printf("Error: Cálculo de tamaño causa overflow\n");
    return -1;
}

if (total_size >= buffer_size) {
    printf("Error: Tamaño excede buffer\n");
    return -1;
}

// Ahora es seguro copiar
memcpy(buffer, data, total_size);
```

### 2. Allocación de Memoria Insegura
```c
// CÓDIGO VULNERABLE
size_t num_items = 1000000;
size_t item_size = 5000;

void* ptr = malloc(num_items * item_size);  // ¡OVERFLOW! Alloca poco
// Programa cree que tiene mucha memoria, pero tiene poca

// CÓDIGO SEGURO
size_t total_size;
if (suma_segura_size_t(num_items * item_size, 0, &total_size) != ARITMETICA_EXITOSA) {
    printf("Error: Cálculo de memoria causa overflow\n");
    return NULL;
}

void* ptr = malloc(total_size);  // Allocación correcta o falla explícitamente
```

## Conceptos Clave Demostrados

### 1. **Detección Preventiva**
- Verificar límites antes de realizar operaciones
- Usar matemática para predecir overflow
- Fallar de forma segura y explícita

### 2. **Comportamiento Definido**
- Reemplazar comportamiento indefinido por errores claros
- Proporcionar feedback detallado sobre problemas
- Permitir manejo graceful de errores

### 3. **Validación en Múltiples Niveles**
- Verificar entrada en puntos de entrada
- Validar operaciones intermedias
- Confirmar resultados antes de usar

### 4. **Tipos Apropiados**
- Usar `unsigned` para tamaños y índices cuando es apropiado
- Considerar `size_t` para cálculos de memoria
- Entender límites de cada tipo de dato

## Mejores Prácticas

### ✅ Hacer:
```c
// Verificar antes de operar
if (suma_segura(a, b, &resultado) == ARITMETICA_EXITOSA) {
    // Usar resultado de forma segura
}

// Usar tipos apropiados
size_t buffer_size = 1000;  // Para tamaños
unsigned int count = 0;     // Para contadores no negativos

// Validar entrada
if (valor < 0 || valor > LIMITE_MAXIMO) {
    return ERROR_RANGO_INVALIDO;
}
```

### ❌ Evitar:
```c
// Aritmética sin verificación
int resultado = a + b;  // ¡Puede tener overflow silencioso!

// Asumir que no habrá overflow
size_t memoria = num_items * item_size;  // ¡Peligroso!

// Usar signed para tamaños
int buffer_size = -1000;  // ¡Ilógico y peligroso!
```

## Límites de Tipos de Datos

| Tipo | Mínimo | Máximo | Notas |
|------|--------|--------|-------|
| `int` | `-2,147,483,648` | `2,147,483,647` | Rango típico en sistemas de 32 bits |
| `unsigned int` | `0` | `4,294,967,295` | Solo valores positivos |
| `long long` | `-9,223,372,036,854,775,808` | `9,223,372,036,854,775,807` | Rango extendido |
| `size_t` | `0` | `18,446,744,073,709,551,615` | Específico para tamaños |

## Herramientas de Análisis

### Compilador
```bash
gcc -Wall -Wextra -Werror -ftrapv  # Detecta overflow en runtime
gcc -fsanitize=signed-integer-overflow  # UBSan para overflow
```

### Análisis Estático
```bash
clang-tidy --checks=-*,cert-*,misc-*,readability-* *.c
cppcheck --enable=all *.c
```

### Tests de Fuzzing
```bash
# Usar AFL o libFuzzer para probar con entradas aleatorias
afl-gcc -o programa programa.c
afl-fuzz -i inputs -o outputs ./programa
```

## Dependencias

- **Compiler**: GCC o Clang con soporte C99
- **CMake**: 3.10 o superior (para compilación con CMake)
- **Criterion**: Para ejecutar los tests (opcional)

## Recursos Adicionales

- [CERT INT32-C](https://wiki.sei.cmu.edu/confluence/display/c/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow)
- [CWE-190: Integer Overflow](https://cwe.mitre.org/data/definitions/190.html)
- [OWASP Integer Overflow](https://owasp.org/www-community/vulnerabilities/Integer_Overflow)
- [Secure Coding in C: Integer Security](https://wiki.sei.cmu.edu/confluence/display/c/INT+Integers)
