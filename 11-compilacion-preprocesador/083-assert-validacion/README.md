# Ejercicio 083: Assert para Validación de Precondiciones

## Descripción
Este ejercicio enseña el uso de la macro `assert()` para validar precondiciones durante el desarrollo. Assert es una herramienta fundamental de debugging que ayuda a detectar errores de programación verificando que ciertas condiciones se cumplan en puntos críticos del código.

## Conceptos Clave
- **Assert**: Macro para verificar precondiciones durante desarrollo
- **Precondiciones**: Condiciones que deben ser verdaderas al inicio de una función
- **NDEBUG**: Macro que desactiva assert en builds de producción
- **Debugging vs. Manejo de errores**: Diferencia entre validación de desarrollo y errores de usuario
- **Compilación condicional**: Comportamiento diferente entre Debug y Release

## ¿Qué es Assert?

### Definición
`assert(expresion)` es una macro que:
1. **Evalúa una expresión** booleana
2. **Si es falsa**, imprime un mensaje de error y termina el programa con `abort()`
3. **Si es verdadera**, no hace nada y continúa la ejecución
4. **Se desactiva completamente** cuando se define `NDEBUG`

### Sintaxis
```c
#include <assert.h>

assert(condicion && "Mensaje opcional de error");
```

### Cuándo Usar Assert
✅ **SÍ usar para**:
- Verificar precondiciones de funciones
- Validar postcondiciones
- Verificar invariantes de estructuras de datos
- Detectar bugs durante desarrollo
- Documentar asunciones del código

❌ **NO usar para**:
- Validar entrada de usuario
- Manejar errores esperados
- Condiciones que pueden fallar en producción
- Efectos secundarios importantes

## Estructura del Proyecto
```
083-assert-validacion/
├── include/
│   ├── assert_validacion.h      # Declaraciones con documentación
│   └── .gitkeep
├── src/
│   ├── assert_validacion.c      # Implementación con assert
│   └── main.c                   # Programa interactivo
├── tests/
│   └── test_assert_validacion.c # Tests para ambas configuraciones
├── CMakeLists.txt               # Build con múltiples configuraciones
├── README.md                    # Esta documentación
└── .gitignore                   # Exclusiones
```

## Compilación y Ejecución

### Método 1: CMake con Configuraciones
```bash
# Crear directorio de build
mkdir build && cd build

# Assert ACTIVO (Debug)
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./demo_assert

# Assert DESACTIVADO (Release)
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./demo_assert
```

### Método 2: Forzar Estado de Assert
```bash
# Forzar assert activo
cmake -DFORCE_ASSERT_ON=ON ..
make

# Forzar assert desactivado
cmake -DFORCE_ASSERT_OFF=ON ..
make
```

### Método 3: Compilación Manual
```bash
# Con assert activo
gcc -I include src/assert_validacion.c src/main.c -lm -o programa_debug

# Con assert desactivado
gcc -DNDEBUG -I include src/assert_validacion.c src/main.c -lm -o programa_release
```

### Demostraciones Específicas
```bash
# Generar ambas versiones para comparar
make comparar_assert

# Verificar estado actual de assert
make verificar_assert

# Ejecutar ejemplo simple del enunciado
make ejemplo_simple
./ejemplo_simple
```

## Funcionalidades Implementadas

### Funciones Matemáticas con Assert
```c
// Raíz cuadrada - solo números no negativos
double raiz_cuadrada(double x) {
    assert(x >= 0.0 && "El argumento no puede ser negativo");
    return sqrt(x);
}

// División - divisor no puede ser cero
double division_segura(double dividendo, double divisor) {
    assert(fabs(divisor) > DBL_EPSILON && "El divisor no puede ser cero");
    return dividendo / divisor;
}
```

### Funciones de Array con Assert
```c
// Promedio - array válido y tamaño positivo
double promedio_array(const double array[], int tamaño) {
    assert(array != NULL && "El array no puede ser NULL");
    assert(tamaño > 0 && "El tamaño debe ser mayor que cero");
    // ... implementación
}
```

### Alternativas Seguras (Sin Assert)
```c
// Versión que retorna éxito/error en lugar de usar assert
bool raiz_cuadrada_segura(double x, double *resultado) {
    if (x < 0.0 || resultado == NULL) {
        return false;  // Error, pero sin terminar programa
    }
    *resultado = sqrt(x);
    return true;  // Éxito
}
```

## Comportamiento de Assert

### Con Assert Activo (Debug)
```bash
$ ./programa_debug
Raíz de 25.00: 5.00
Assertion failed: (x >= 0.0), function raiz_cuadrada, file assert_validacion.c, line 23.
Abort trap: 6
```

### Con Assert Desactivado (Release)
```bash
$ ./programa_release
Raíz de 25.00: 5.00
Raíz de -9.00: nan    # Se ejecuta pero produce NaN
```

## Programa Interactivo

El programa principal ofrece un menú interactivo para:

1. **Demostrar casos válidos** - Funciones que pasan todas las verificaciones
2. **Mostrar casos inválidos** - Explicar qué causaría assert (sin ejecutar)
3. **Probar alternativas seguras** - Versiones sin assert que manejan errores
4. **Verificar estado de assert** - Mostrar si NDEBUG está definido
5. **Ejecutar casos específicos** - Probar funciones individuales

### Ejemplo de Uso Interactivo
```
🛡️  DEMOSTRACIÓN DE ASSERT EN C
Validación de precondiciones durante el desarrollo
================================================

🟢 NDEBUG NO está definido - Assert está ACTIVADO
   Las validaciones con assert se ejecutarán
   Esto es típico en builds de Debug

Selecciona una opción:
1. Demostrar casos válidos (safe)
2. Mostrar casos inválidos (info)
3. Demostrar alternativas seguras
4. Mostrar estado de assert
5. Ejecutar caso específico
0. Salir

Opción: 1

=== CASOS VÁLIDOS (Assert no se activa) ===
Raíz cuadrada de 25.0: 5.00
Raíz cuadrada de 0.0: 0.00
...
✅ Todos los casos válidos ejecutados sin problemas
```

## Tests Comprehensivos

### Tests para Casos Válidos
- Funciones matemáticas con parámetros correctos
- Operaciones con arrays válidos
- Funciones de cadenas con punteros no nulos
- Verificación de resultados correctos

### Tests Condicionales
```c
Test(assert_validacion, test_comportamiento_con_ndebug) {
    if (es_assert_activo()) {
        // Si assert está activo, no probar casos inválidos
        cr_assert(true, "Assert activo - casos inválidos omitidos");
    } else {
        // Si assert está desactivado, probar casos inválidos
        cr_assert_float_eq(raiz_cuadrada(-5.0), sqrt(-5.0), 0.0001);
    }
}
```

### Ejecutar Tests
```bash
# Tests estándar
make check

# Tests con ambas configuraciones
make check_todos

# Solo tests con assert activo
make test_assert_activo

# Solo tests con assert desactivado  
make test_assert_desactivado
```

## Mejores Prácticas

### 1. Mensajes Descriptivos
```c
// ✅ Bueno - mensaje claro
assert(ptr != NULL && "Puntero no puede ser NULL");

// ❌ Malo - sin mensaje
assert(ptr != NULL);
```

### 2. Condiciones Simples
```c
// ✅ Bueno - condición clara
assert(tamaño > 0 && "Tamaño debe ser positivo");

// ❌ Malo - condición compleja
assert((ptr != NULL && tamaño > 0 && tamaño < MAX_SIZE && ptr[0] != 0) && "Múltiples condiciones");
```

### 3. Sin Efectos Secundarios
```c
// ❌ Malo - efecto secundario en assert
assert(++contador > 0);  // contador se incrementa solo en Debug

// ✅ Bueno - sin efectos secundarios
++contador;
assert(contador > 0);
```

### 4. Separar Assert de Manejo de Errores
```c
// Para errores de programación (bugs)
double raiz_cuadrada(double x) {
    assert(x >= 0);  // Precondición: programador debe asegurar x >= 0
    return sqrt(x);
}

// Para errores de usuario (entrada inválida)
bool raiz_cuadrada_usuario(double x, double *resultado) {
    if (x < 0) {
        return false;  // Error de usuario, no bug
    }
    *resultado = sqrt(x);
    return true;
}
```

## Configuraciones de Build

### Debug (Desarrollo)
- Assert **ACTIVO**
- Información de debugging
- Sin optimizaciones
- Para detectar bugs

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Release (Producción)
- Assert **DESACTIVADO** (NDEBUG definido)
- Optimizaciones activadas
- Sin información de debugging
- Para rendimiento máximo

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Herramientas Adicionales

### Verificación de Estado
```bash
make verificar_assert  # Mostrar si assert está activo
```

### Comparación de Versiones
```bash
make comparar_assert   # Generar versiones Debug y Release
```

### Análisis Estático
```bash
make cppcheck          # Análisis con cppcheck
```

## Casos de Uso Reales

### 1. Validación de Estructuras de Datos
```c
void insertar_lista(lista_t *lista, int valor) {
    assert(lista != NULL);
    assert(lista->tamaño >= 0);
    assert(lista->tamaño < lista->capacidad);
    // ... insertar elemento
}
```

### 2. Algoritmos con Precondiciones
```c
int busqueda_binaria(int arr[], int n, int x) {
    assert(arr != NULL);
    assert(n > 0);
    // Assert: array debe estar ordenado (difícil de verificar eficientemente)
    // ... implementación
}
```

### 3. Invariantes de Clase
```c
typedef struct {
    double *datos;
    int tamaño;
    int capacidad;
} vector_t;

void verificar_invariantes(const vector_t *v) {
    assert(v != NULL);
    assert(v->tamaño >= 0);
    assert(v->tamaño <= v->capacidad);
    assert(v->capacidad > 0);
    assert(v->datos != NULL);
}
```

## Alternativas a Assert

### 1. Validación Manual
```c
if (condicion_invalida) {
    fprintf(stderr, "Error: condición inválida\n");
    return ERROR_CODE;
}
```

### 2. Macros Personalizadas
```c
#define REQUIRE(cond, msg) \
    do { if (!(cond)) { \
        fprintf(stderr, "Precondición fallida: %s\n", msg); \
        exit(EXIT_FAILURE); \
    } } while(0)
```

### 3. Logging Condicional
```c
#ifdef DEBUG
    if (condicion_sospechosa) {
        printf("DEBUG: Condición sospechosa detectada\n");
    }
#endif
```

## Resumen Educativo

### ✅ Assert es útil para:
- **Detectar bugs** durante desarrollo
- **Documentar precondiciones** en el código
- **Verificar invariantes** de estructuras de datos
- **Validar asunciones** del programador

### ❌ Assert NO es para:
- **Validar entrada de usuario** (usar validación manual)
- **Manejar errores esperados** (usar códigos de retorno)
- **Código de producción crítico** (se desactiva con NDEBUG)
- **Condiciones que pueden fallar** normalmente

### 🎯 Regla de oro:
> **Assert verifica bugs del programador, no errores del usuario**

## Recursos Adicionales
- [C Assert Macro](https://en.cppreference.com/w/c/error/assert)
- [Defensive Programming](https://en.wikipedia.org/wiki/Defensive_programming)
- [Design by Contract](https://en.wikipedia.org/wiki/Design_by_contract)
