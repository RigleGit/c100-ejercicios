# Ejercicio 100: Evaluador de Notación Polaca Reversa (RPN)

## Descripción

Este ejercicio implementa un evaluador completo de expresiones en Notación Polaca Reversa (RPN) usando una pila (stack) implementada como lista enlazada. El evaluador soporta operaciones aritméticas básicas, potencias, módulo, y funciones avanzadas.

## Características

### Operaciones Soportadas
- **Aritméticas básicas**: `+`, `-`, `*`, `/`
- **Potencia**: `^` (ejemplo: `2 3 ^` = 8)
- **Módulo**: `%` (ejemplo: `7 3 %` = 1)
- **Funciones**: `abs`, `sqrt`, `sin`, `cos`, `tan`, `log`, `ln`
- **Constantes**: `pi`, `e`

### Funcionalidades
- ✅ Evaluación interactiva desde consola
- ✅ Procesamiento de archivos con expresiones
- ✅ Estadísticas de operaciones realizadas
- ✅ Historial de cálculos
- ✅ Manejo robusto de errores
- ✅ Validación de entrada
- ✅ Soporte para números decimales

## Notación Polaca Reversa (RPN)

En RPN, los operadores siguen a sus operandos:
- Infijo: `(3 + 4) * 5`
- RPN: `3 4 + 5 *`

### Ejemplos de Uso

```
Entrada: 3 4 +
Resultado: 7

Entrada: 15 7 1 1 + - /
Resultado: 3

Entrada: 2 3 ^
Resultado: 8

Entrada: 9 sqrt
Resultado: 3

Entrada: pi 2 /
Resultado: 1.570796
```

## Compilación

### Usando CMake (Recomendado)

```bash
mkdir build
cd build
cmake ..
make
```

### Usando Make Directo

```bash
make
```

### Compilación Manual

```bash
gcc -std=c99 -Wall -Wextra -Iinclude src/*.c -o evaluador_rpn -lm
```

## Uso

### Modo Interactivo

```bash
./evaluador_rpn
```

### Procesamiento de Archivo

```bash
./evaluador_rpn archivo_expresiones.txt
```

### Ejecutar Tests

```bash
make test
# o
./test_evaluador_rpn
```

## Estructura del Proyecto

```
100-evaluador-rpn/
├── include/
│   └── evaluador_rpn.h      # Definiciones y prototipos
├── src/
│   ├── evaluador_rpn.c      # Implementación del evaluador
│   ├── funciones_adicionales.c  # Funciones matemáticas adicionales
│   └── main.c               # Programa principal
├── tests/
│   ├── test_evaluador_rpn.c # Tests unitarios
│   └── test_data/           # Datos de prueba
├── ejemplos/
│   ├── expresiones_basicas.txt
│   ├── expresiones_avanzadas.txt
│   └── casos_error.txt
├── tools/
│   ├── generador_tests.c    # Generador de casos de prueba
│   └── benchmark.c          # Pruebas de rendimiento
├── CMakeLists.txt           # Configuración CMake
├── Makefile                 # Makefile alternativo
├── README.md                # Este archivo
└── .gitignore              # Archivos a ignorar
```

## Algoritmo de Evaluación

1. **Inicializar** pila vacía
2. **Para cada token** en la expresión:
   - Si es **número**: empujar a la pila
   - Si es **operador**: 
     - Extraer operandos necesarios de la pila
     - Realizar operación
     - Empujar resultado a la pila
3. **Resultado final**: único elemento en la pila

## Gestión de Memoria

- ✅ **Liberación automática**: Toda la memoria se libera correctamente
- ✅ **Detección de fugas**: No hay memory leaks
- ✅ **Validación de punteros**: Verificación antes de uso
- ✅ **Manejo de errores**: Liberación en casos de error

## Manejo de Errores

### Errores Detectados
- División por cero
- Pila vacía al extraer operandos
- Operandos insuficientes
- Desbordamiento de números
- Tokens inválidos
- Errores de dominio matemático (sqrt de negativos, log de cero, etc.)

### Códigos de Error
```c
typedef enum {
    RPN_EXITO = 0,
    RPN_ERROR_DIVISION_CERO = -1,
    RPN_ERROR_PILA_VACIA = -2,
    RPN_ERROR_OPERANDOS_INSUFICIENTES = -3,
    RPN_ERROR_TOKEN_INVALIDO = -4,
    RPN_ERROR_MEMORIA = -5,
    RPN_ERROR_DESBORDAMIENTO = -6,
    RPN_ERROR_DOMINIO = -7
} rpn_resultado_t;
```

## Tests Incluidos

### Tests Unitarios
- ✅ Operaciones básicas (+, -, *, /)
- ✅ Potencias y módulo
- ✅ Funciones matemáticas
- ✅ Manejo de constantes
- ✅ Casos de error
- ✅ Expresiones complejas
- ✅ Validación de memoria

### Tests de Integración
- ✅ Procesamiento de archivos
- ✅ Modo interactivo simulado
- ✅ Estadísticas y historial
- ✅ Rendimiento con expresiones largas

## Archivos de Ejemplo

### `ejemplos/expresiones_basicas.txt`
```
3 4 +
10 2 /
5 3 - 2 *
```

### `ejemplos/expresiones_avanzadas.txt`
```
2 3 ^ 4 +
pi 2 / sin
e ln 1 +
9 sqrt 3 /
```

## Optimizaciones Implementadas

1. **Pila eficiente**: Lista enlazada simple con O(1) para push/pop
2. **Parser optimizado**: Tokenización en una sola pasada
3. **Validación temprana**: Verificación antes de cálculos costosos
4. **Reutilización de memoria**: Pool de nodos para operaciones frecuentes

## Casos de Uso Educativos

### Básico
```bash
# Suma simple
echo "2 3 +" | ./evaluador_rpn

# Expresión compuesta
echo "3 4 + 2 *" | ./evaluador_rpn
```

### Intermedio
```bash
# Usar archivo
./evaluador_rpn ejemplos/expresiones_basicas.txt

# Funciones matemáticas
echo "9 sqrt 3 /" | ./evaluador_rpn
```

### Avanzado
```bash
# Benchmark de rendimiento
./tools/benchmark

# Generación de tests
./tools/generador_tests 1000 > test_generado.txt
```

## Extensiones Posibles

1. **Más operadores**: `factorial`, `gcd`, `lcm`
2. **Variables**: Soporte para `x`, `y`, `z`
3. **Funciones personalizadas**: Definición de funciones por usuario
4. **Modo gráfico**: Visualización de la pila
5. **Precisión arbitraria**: Usando bibliotecas como GMP

## Referencias

- [Notación Polaca Reversa - Wikipedia](https://es.wikipedia.org/wiki/Notaci%C3%B3n_polaca_inversa)
- [Algoritmos de Evaluación de Expresiones](https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
- [Estructuras de Datos: Pilas](https://es.wikipedia.org/wiki/Pila_(inform%C3%A1tica))
