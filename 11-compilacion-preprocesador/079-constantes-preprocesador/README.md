# Ejercicio 079: Constantes de Preprocesador

## 📋 Descripción

Este ejercicio demuestra el uso de **constantes del preprocesador** en C mediante la directiva `#define`. A diferencia de las variables `const`, las constantes de preprocesador realizan **sustitución textual** antes de la compilación, permitiendo su uso en contextos donde se requieren valores constantes conocidos en tiempo de compilación.

## 🎯 Objetivo

Aprender a:
- Usar `#define` para crear constantes simbólicas
- Entender la diferencia entre `#define` y `const`
- Crear macros simples (pseudo-funciones)
- Usar constantes condicionales con `#ifdef`/#ifndef`
- Comprender la sustitución textual del preprocesador

## 📚 Conceptos Clave

### 1. Sustitución Textual
```c
#define TAMANIO 5
int array[TAMANIO];  // Se convierte en: int array[5];
```

### 2. Diferencia entre `#define` y `const`
```c
#define TAMANIO_DEFINE 5      // Sustitución textual
const int TAMANIO_CONST = 5;  // Variable de solo lectura

int array1[TAMANIO_DEFINE];   // ✓ Válido
int array2[TAMANIO_CONST];    // ✗ Error en C89/C90
```

### 3. Tipos de Constantes

#### Constantes Numéricas
```c
#define PI 3.14159265359
#define MAX_BUFFER 1024
#define VERSION_MAYOR 1
```

#### Constantes de Cadena
```c
#define MSG_ERROR "Error en la operación"
#define FORMATO_FLOAT "%.2f"
```

#### Constantes Calculadas
```c
#define TAMANIO_DOBLE (TAMANIO * 2)
#define AREA_CIRCULO_UNITARIO (PI * 1 * 1)
```

### 4. Macros Simples (Pseudo-funciones)
```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SQUARE(x) ((x) * (x))
```

### 5. Constantes Condicionales
```c
#ifndef DEBUG
    #define DEBUG 0
#endif

#if DEBUG
    #define LOG(msg) printf("[DEBUG] %s\n", (msg))
#else
    #define LOG(msg)
#endif
```

## ▶️ Ejecución

### Ejecutables Disponibles

1. **Programa básico del enunciado:**
   ```bash
   ./main_basico
   ```

2. **Demostración completa:**
   ```bash
   ./demo_completo
   ```

3. **Demostración con debug:**
   ```bash
   ./demo_debug
   ```

### Tests
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/main_basico
```

### Targets Especiales
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/main_basico
```

## 📋 Ejemplo de Uso

### Código Básico (del enunciado)
```c
#include "constantes_preprocesador.h"

int main(void) {
    int numeros[TAMANIO];  // TAMANIO = 5

    // Llenar array
    for (int i = 0; i < TAMANIO; i++) {
        numeros[i] = (i + 1) * 10;
    }

    // Mostrar contenido
    for (int i = 0; i < TAMANIO; i++) {
        printf("numeros[%d] = %d\n", i, numeros[i]);
    }

    return 0;
}
```

### Uso Avanzado
```c
// Usar constantes matemáticas
double area = PI * radio * radio;

// Usar macros para comparaciones
int mayor = MAX(a, b);
int absoluto = ABS(numero);

// Usar constantes para validación
if (edad >= MIN_EDAD && edad <= MAX_EDAD) {
    printf("Edad válida\n");
}

// Usar máscaras de bits
unsigned char flags = BIT_0 | BIT_2 | BIT_5;
```

## 🎯 Objetivos de Aprendizaje

Al completar este ejercicio, habrás aprendido:

1. **Conceptos del Preprocesador:**
   - Cómo funciona la sustitución textual
   - Diferencias entre `#define` y `const`
   - Cuándo usar cada uno

2. **Buenas Prácticas:**
   - Nombrar constantes en MAYÚSCULAS
   - Usar paréntesis en macros
   - Evitar efectos secundarios en macros

3. **Aplicaciones Prácticas:**
   - Definir tamaños de arrays
   - Crear constantes matemáticas
   - Implementar configuración condicional

4. **Herramientas de Debug:**
   - Usar `-E` para ver expansión del preprocesador
   - Compilación condicional con `DEBUG`

## ⚠️ Consideraciones Importantes

### Ventajas de `#define`
- Sustitución textual permite uso en arrays
- No consume memoria en tiempo de ejecución
- Permite compilación condicional

### Desventajas de `#define`
- No tiene tipo (puede causar errores sutiles)
- No respeta scope (ámbito)
- Expansión puede ser inesperada

### Buenas Prácticas
```c
// ✓ Correcto: usar paréntesis
#define SQUARE(x) ((x) * (x))

// ✗ Incorrecto: sin paréntesis
#define SQUARE_MAL(x) x * x

// ✓ Correcto: evitar efectos secundarios
int valor = 5;
int resultado = SQUARE(valor);  // valor se evalúa una vez

// ✗ Problemático: con efectos secundarios
int resultado = SQUARE(++valor);  // valor se incrementa dos veces
```

## 🔍 Depuración

### Ver Expansión del Preprocesador
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/main_basico
```

### Compilar con Información de Debug
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/main_basico
```

## 📚 Referencias

- Kernighan & Ritchie: "The C Programming Language"
- ISO C Standard: Preprocessing directives
- GCC Manual: Preprocessor options

---

## 🤝 Contribución

Si encuentras errores o tienes sugerencias de mejora, por favor crea un issue o envía un pull request.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/main_basico
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
