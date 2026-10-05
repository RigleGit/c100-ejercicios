# Ejercicio 013: Clasificación Numérica

## Enunciado

Escribe un programa que lea un número entero introducido por el usuario y determine si es positivo, negativo o cero.

## Descripción

Este ejercicio refuerza el uso de **estructuras condicionales encadenadas** (`if`, `else if`, `else`), permitiéndote manejar múltiples condiciones posibles de forma clara. También es una forma de introducir la idea de **flujos de control con múltiples ramas**, que será clave para tomar decisiones más complejas más adelante.

## Conceptos Matemáticos

Los números enteros se clasifican en tres categorías:
- **Positivos**: números mayores que cero (1, 2, 3, ...)
- **Negativos**: números menores que cero (-1, -2, -3, ...)
- **Cero**: el número neutro (0)

Esta clasificación es fundamental en matemáticas y programación.

## Conceptos Clave

- **Estructuras condicionales encadenadas**: `if-else if-else`
- **Operadores de comparación**: `>`, `<`, `==`
- **Flujo de control múltiple**: Tres posibles caminos de ejecución
- **Evaluación secuencial**: Las condiciones se evalúan en orden
- **Exclusividad**: Solo una rama se ejecuta
- **Casos mutuamente excluyentes**: Un número no puede ser positivo y negativo a la vez

## Explicación del Algoritmo

1. **Entrada**: Leer número entero del usuario
2. **Decisión múltiple**:
   - Si `numero > 0` → es positivo
   - Si no, si `numero < 0` → es negativo
   - Si no → es cero
3. **Salida**: Mostrar la clasificación correspondiente

```c
if (numero > 0) {
    // Caso positivo
} else if (numero < 0) {
    // Caso negativo
} else {
    // Caso cero (la única opción restante)
}
```

## Ejemplo de Ejecución

```
Introduce un número entero: 15
El número es positivo.
```

```
Introduce un número entero: -25
El número es negativo.
```

```
Introduce un número entero: 0
El número es cero.
```

```
Introduce un número entero: 1
El número es positivo.
```

```
Introduce un número entero: -1
El número es negativo.
```

## Conceptos Clave Detallados

### 1. Estructura If-Else If-Else
- **Evaluación secuencial**: Se evalúa de arriba hacia abajo
- **Primera coincidencia**: Se ejecuta la primera condición verdadera
- **Exclusividad**: Solo una rama se ejecuta
- **Else final**: Captura todos los casos no contemplados

### 2. Operadores de Comparación
- **Mayor que (`>`)**: Verifica si el número es positivo
- **Menor que (`<`)**: Verifica si el número es negativo
- **Implícito igual (`==`)**: El `else` captura el caso de cero

### 3. Lógica de Clasificación
```
número > 0  → POSITIVO
número < 0  → NEGATIVO
resto       → CERO
```

## Errores Comunes

1. **Orden incorrecto**: Poner condiciones menos específicas primero
2. **Condiciones redundantes**: Usar `== 0` cuando `else` es suficiente
3. **Operadores incorrectos**: Confundir `>` con `>=` o `<` con `<=`
4. **Falta de else**: No manejar todos los casos posibles
5. **Estructura incorrecta**: Usar `if` múltiples en lugar de `else if`

## Extensiones Posibles

1. **Clasificación extendida**: Agregar "muy grande", "muy pequeño"
2. **Múltiples números**: Procesar varios números en bucle
3. **Estadísticas**: Contar cuántos de cada tipo se introducen
4. **Validación de entrada**: Verificar que se introduce un número válido
5. **Menú interactivo**: Permitir múltiples operaciones
6. **Rangos personalizados**: Definir rangos para positivo/negativo

## Tabla de Decisiones

| Condición | numero > 0 | numero < 0 | Resultado |
|-----------|------------|------------|-----------|
| Caso 1 | Verdadero | - | Positivo |
| Caso 2 | Falso | Verdadero | Negativo |
| Caso 3 | Falso | Falso | Cero |

## Variantes del Problema

- **Clasificación de notas**: A, B, C, D, F según rangos
- **Categorías de edad**: Niño, adulto, senior
- **Temperatura**: Frío, templado, caliente
- **Velocidad**: Lento, normal, rápido
- **Tamaño**: Pequeño, mediano, grande

## Conceptos Relacionados

- **Lógica booleana**: Verdadero/falso en condiciones
- **Teoría de conjuntos**: Números positivos, negativos, cero
- **Flujo de control**: Cómo el programa toma decisiones
- **Algoritmos de clasificación**: Categorizar datos según criterios

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/clasificacion_numerica
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
