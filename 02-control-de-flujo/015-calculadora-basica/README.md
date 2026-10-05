# Ejercicio 015: Calculadora Básica

## Enunciado

Escribe un programa que lea dos números y un operador (+, -, *, /) introducidos por el usuario, y realice la operación correspondiente mostrando el resultado. Usa `switch` para seleccionar la operación.

## Descripción

Este ejercicio tiene como objetivo practicar **estructuras condicionales con `switch`**, una alternativa clara y estructurada al uso de múltiples `if/else` cuando se trabaja con múltiples casos concretos. Además, refuerza el uso de **entrada de caracteres**, **operaciones aritméticas** y **control de errores básicos**.

## Conceptos Matemáticos

La calculadora implementa las cuatro operaciones aritméticas fundamentales:
- **Suma (+)**: `a + b`
- **Resta (-)**: `a - b`
- **Multiplicación (*)**: `a * b`
- **División (/)**: `a / b` (con validación de división por cero)

## Conceptos Clave

- **Estructura `switch-case`**: Control de flujo para múltiples opciones discretas
- **Variables `double`**: Precisión de punto flotante para cálculos
- **Variable `char`**: Almacenamiento del operador
- **Entrada formateada**: `scanf` con múltiples especificadores
- **Sentencia `break`**: Evita la ejecución en cascada
- **Caso `default`**: Manejo de opciones no válidas
- **Validación de entrada**: Control de errores (división por cero)

## Explicación del Algoritmo

### Flujo del Programa

1. **Entrada**: Leer dos números y un operador en formato `num1 op num2`
2. **Evaluación**: Usar `switch` para determinar la operación
3. **Cálculo**: Realizar la operación correspondiente
4. **Validación**: Verificar división por cero
5. **Salida**: Mostrar el resultado o mensaje de error

### Estructura Switch

```c
switch (operador) {
    case '+':
        // Suma
        break;
    case '-':
        // Resta
        break;
    case '*':
        // Multiplicación
        break;
    case '/':
        // División (con validación)
        break;
    default:
        // Operador no válido
}
```

## Ejemplo de Ejecución

```
Introduce una operación (ejemplo: 4.5 * 2): 15.5 + 8.3
Resultado: 23.80
```

```
Introduce una operación (ejemplo: 4.5 * 2): 20.0 - 7.5
Resultado: 12.50
```

```
Introduce una operación (ejemplo: 4.5 * 2): 6.0 * 4.5
Resultado: 27.00
```

```
Introduce una operación (ejemplo: 4.5 * 2): 18.0 / 3.0
Resultado: 6.00
```

```
Introduce una operación (ejemplo: 4.5 * 2): 10.0 / 0.0
Error: división por cero.
```

```
Introduce una operación (ejemplo: 4.5 * 2): 5.0 % 2.0
Operador no válido.
```

## Conceptos Clave Detallados

### 1. Estructura Switch-Case
- **Ventajas sobre if-else**: Más clara para múltiples opciones discretas
- **Evaluación**: Compara la variable con cada `case`
- **Break**: Necesario para evitar "fall-through" (ejecución en cascada)
- **Default**: Captura casos no contemplados

### 2. Entrada Formateada
```c
scanf("%lf %c %lf", &num1, &operador, &num2);
```
- **%lf**: Lee números de tipo `double`
- **%c**: Lee un carácter (el operador)
- **Espacios**: Permiten separadores flexibles

### 3. Manejo de Errores
- **División por cero**: Verificación antes de dividir
- **Operadores inválidos**: Caso `default` en `switch`
- **Formato de salida**: Consistencia en mensajes de error

### 4. Tipos de Datos
- **double**: Mayor precisión que `float`
- **char**: Un byte para el operador
- **Formato %.2lf**: Dos decimales en la salida

## Tabla de Operaciones

| Operación | Símbolo | Ejemplo | Resultado |
|-----------|---------|---------|-----------|
| Suma | + | 5.5 + 3.2 | 8.70 |
| Resta | - | 10.0 - 3.5 | 6.50 |
| Multiplicación | * | 4.0 * 2.5 | 10.00 |
| División | / | 15.0 / 3.0 | 5.00 |
| División por cero | / | 10.0 / 0.0 | Error |
| Operador inválido | % | 5.0 % 2.0 | Error |

## Errores Comunes

1. **Olvidar `break`**: Causa ejecución en cascada (fall-through)
2. **No validar división por cero**: Puede causar comportamiento indefinido
3. **Formato incorrecto en scanf**: No manejar espacios correctamente
4. **Tipo de datos**: Usar `float` en lugar de `double`
5. **No incluir `default`**: No manejar operadores inválidos
6. **Comparación de punto flotante**: Usar `!= 0` en lugar de comparación exacta

## Extensiones Posibles

1. **Más operaciones**: Potencia (^), módulo (%), raíz cuadrada
2. **Validación de entrada**: Verificar que se introducen números válidos
3. **Calculadora interactiva**: Bucle para múltiples operaciones
4. **Historial**: Guardar operaciones anteriores
5. **Funciones avanzadas**: Seno, coseno, logaritmo
6. **Interfaz mejorada**: Menú de opciones y formateo

## Comparación: Switch vs If-Else

### Con Switch (preferido para este caso)
```c
switch (operador) {
    case '+': /* suma */ break;
    case '-': /* resta */ break;
    case '*': /* mult */ break;
    case '/': /* div */ break;
    default: /* error */ break;
}
```

### Con If-Else (alternativa)
```c
if (operador == '+') {
    /* suma */
} else if (operador == '-') {
    /* resta */
} else if (operador == '*') {
    /* mult */
} else if (operador == '/') {
    /* div */
} else {
    /* error */
}
```

## Variantes del Problema

- **Calculadora científica**: Operaciones trigonométricas y logarítmicas
- **Calculadora de enteros**: Solo números enteros sin decimales
- **Calculadora RPN**: Notación polaca inversa
- **Calculadora con memoria**: Funciones de memoria (M+, M-, MR, MC)
- **Evaluador de expresiones**: Análisis de expresiones complejas

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/calculadora_basica
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
