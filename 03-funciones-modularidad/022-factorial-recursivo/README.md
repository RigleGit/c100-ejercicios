# Ejercicio 022: Factorial Recursivo

## Enunciado

Escribe una función recursiva `long factorial(int n)` que calcule el factorial de un número entero no negativo. Usa esta función desde `main` para leer un número y mostrar su factorial.

## Descripción

El programa debe:

1. Definir una función recursiva `factorial` que calcule n!
2. Implementar el caso base (n = 0 o n = 1)
3. Implementar el caso recursivo (n × factorial(n-1))
4. En `main`, solicitar un número al usuario
5. Validar que el número no sea negativo
6. Mostrar el resultado del factorial

### Ejemplo de ejecución:

```
Introduce un número entero no negativo: 5
El factorial de 5 es: 120
```

```
Introduce un número entero no negativo: 0
El factorial de 0 es: 1
```

```
Introduce un número entero no negativo: -3
El factorial no está definido para números negativos.
```

## Conceptos clave

- **Recursividad**: Técnica donde una función se llama a sí misma
- **Caso base**: Condición que detiene la recursión
- **Caso recursivo**: Llamada de la función a sí misma con parámetros modificados
- **Pila de llamadas**: Estructura que almacena las llamadas pendientes
- **Factorial matemático**: n! = n × (n-1) × (n-2) × ... × 2 × 1

## ¿Qué es la recursividad?

La **recursividad** es una técnica de programación donde una función se define en términos de sí misma. Para que funcione correctamente, debe tener:

1. **Caso base**: Una condición que detiene la recursión
2. **Caso recursivo**: La función se llama a sí misma con un problema más pequeño

### Anatomía de una función recursiva:

```c
tipo_retorno funcion_recursiva(parametros) {
    // CASO BASE - Condición de parada
    if (condicion_parada) {
        return valor_base;
    }
    
    // CASO RECURSIVO - Llamada a sí misma
    return operacion(funcion_recursiva(parametros_modificados));
}
```

## El factorial matemático

### Definición matemática:
```
n! = n × (n-1) × (n-2) × ... × 2 × 1

Casos especiales:
0! = 1  (por definición)
1! = 1
```

### Definición recursiva:
```
factorial(n) = {
    1                           si n = 0 o n = 1  (caso base)
    n × factorial(n-1)          si n > 1          (caso recursivo)
}
```

### Ejemplos paso a paso:

#### factorial(5):
```
factorial(5) = 5 × factorial(4)
             = 5 × (4 × factorial(3))
             = 5 × (4 × (3 × factorial(2)))
             = 5 × (4 × (3 × (2 × factorial(1))))
             = 5 × (4 × (3 × (2 × 1)))
             = 5 × (4 × (3 × 2))
             = 5 × (4 × 6)
             = 5 × 24
             = 120
```

## Flujo de ejecución y pila de llamadas

### Visualización del stack:

```
Llamada inicial: factorial(4)

Stack frame 1: factorial(4)
├── n = 4
├── return 4 * factorial(3)
│
├── Stack frame 2: factorial(3)
│   ├── n = 3
│   ├── return 3 * factorial(2)
│   │
│   ├── Stack frame 3: factorial(2)
│   │   ├── n = 2
│   │   ├── return 2 * factorial(1)
│   │   │
│   │   └── Stack frame 4: factorial(1)
│   │       ├── n = 1
│   │       └── return 1  ← CASO BASE
│   │
│   └── return 2 * 1 = 2
│
└── return 3 * 2 = 6

return 4 * 6 = 24
```

### Tabla de ejecución:

| Llamada | Parámetro | Acción | Resultado parcial |
|---------|-----------|--------|------------------|
| 1 | factorial(4) | 4 × factorial(3) | Pendiente |
| 2 | factorial(3) | 3 × factorial(2) | Pendiente |
| 3 | factorial(2) | 2 × factorial(1) | Pendiente |
| 4 | factorial(1) | return 1 | 1 |
| 3 | factorial(2) | 2 × 1 | 2 |
| 2 | factorial(3) | 3 × 2 | 6 |
| 1 | factorial(4) | 4 × 6 | 24 |

## Comparación: Recursivo vs Iterativo

### ✅ Versión recursiva (elegante):
```c
long factorial_recursivo(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial_recursivo(n - 1);
}
```

### ✅ Versión iterativa (eficiente):
```c
long factorial_iterativo(int n) {
    long resultado = 1;
    for (int i = 2; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}
```

### Comparación de características:

| Aspecto | Recursivo | Iterativo |
|---------|-----------|-----------|
| **Legibilidad** | Muy clara | Clara |
| **Memoria** | O(n) stack | O(1) |
| **Velocidad** | Más lenta | Más rápida |
| **Elegancia** | Natural | Mecánica |
| **Desbordamiento** | Stack overflow | Menos probable |

## Optimización: Tail Recursion

### Recursión de cola optimizada:
```c
long factorial_tail_recursivo(int n, long acumulador) {
    if (n == 0 || n == 1) {
        return acumulador;
    }
    return factorial_tail_recursivo(n - 1, n * acumulador);
}

long factorial_tail(int n) {
    return factorial_tail_recursivo(n, 1);
}
```

### Ventajas de tail recursion:
- **Optimizable**: Algunos compiladores la convierten en iteración
- **Memoria constante**: O(1) en lugar de O(n)
- **Sin stack overflow**: Para números grandes

## Análisis de complejidad

### Factorial recursivo clásico:
- **Tiempo**: O(n) - n llamadas recursivas
- **Espacio**: O(n) - n frames en el stack
- **Llamadas**: n+1 llamadas para factorial(n)

### Tabla de llamadas:

| n | Llamadas recursivas | Memoria stack |
|---|-------------------|---------------|
| 0 | 1 | 1 frame |
| 1 | 1 | 1 frame |
| 5 | 6 | 6 frames |
| 10 | 11 | 11 frames |
| 100 | 101 | 101 frames |

## Casos especiales y límites

### Valores especiales:
```c
factorial(0) = 1    // Por definición matemática
factorial(1) = 1    // Caso base
factorial(2) = 2    // Primer caso no trivial
factorial(5) = 120  // Ejemplo común
factorial(10) = 3,628,800  // Fácil de verificar
```

### Límites de long en C:
```
factorial(20) = 2,432,902,008,176,640,000  // Último que cabe en long
factorial(21) = ???  // Overflow en sistemas de 64 bits
```

### Manejo de casos límite:
```c
long factorial_seguro(int n) {
    if (n < 0) return -1;           // Error: negativo
    if (n > 20) return -1;          // Error: muy grande
    if (n == 0 || n == 1) return 1; // Caso base
    return n * factorial_seguro(n - 1);
}
```

## Aplicaciones de la recursividad

### 1. **Secuencia de Fibonacci**:
```c
long fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}
```

### 2. **Potencia recursiva**:
```c
long potencia(int base, int exp) {
    if (exp == 0) return 1;
    return base * potencia(base, exp-1);
}
```

### 3. **Suma de dígitos**:
```c
int suma_digitos(int n) {
    if (n == 0) return 0;
    return (n % 10) + suma_digitos(n / 10);
}
```

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./factorial_recursivo

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Casos básicos (0!, 1!, 2!, 3!, 4!, 5!)
- ✅ Casos intermedios (6! hasta 12!)
- ✅ Comparación recursivo vs iterativo
- ✅ Versión con optimización de cola
- ✅ Conteo de llamadas recursivas
- ✅ Propiedades matemáticas (n! = n × (n-1)!)
- ✅ Valores conocidos y factoriales famosos
- ✅ Crecimiento exponencial del factorial
- ✅ Casos límite y consistencia
- ✅ Relaciones entre factoriales consecutivos

## Posibles extensiones

1. **Factorial con memoización**: Cachear resultados para eficiencia
2. **Factorial de números grandes**: Usar arrays para números > 20!
3. **Factorial doble**: Calcular n!! = n × (n-2) × (n-4) × ...
4. **Gamma function**: Extensión del factorial a números reales
5. **Subfactorial**: Calcular desarreglos (!n)
6. **Factorial modular**: Calcular n! mod m para evitar overflow
7. **Benchmarking**: Comparar rendimiento recursivo vs iterativo
8. **Visualización**: Mostrar el árbol de llamadas recursivas

## Ejemplo de extensión - Factorial con memoización:

```c
#include <string.h>

#define MAX_N 100
long memo[MAX_N + 1];
int inicializado = 0;

long factorial_memo(int n) {
    if (!inicializado) {
        memset(memo, -1, sizeof(memo));
        memo[0] = memo[1] = 1;
        inicializado = 1;
    }
    
    if (n < 0 || n > MAX_N) return -1;
    if (memo[n] != -1) return memo[n];
    
    memo[n] = n * factorial_memo(n - 1);
    return memo[n];
}
```

## Buenas prácticas para recursividad

1. **Siempre definir caso base**: Evitar recursión infinita
2. **Verificar convergencia**: Asegurar que el problema se reduce
3. **Considerar memoria**: Stack overflow para n muy grandes
4. **Validar entrada**: Manejar casos especiales y errores
5. **Documentar el algoritmo**: Explicar caso base y recursivo
6. **Comparar con iterativo**: Evaluar si recursión es la mejor opción

## Notas de implementación

- Introduce el concepto fundamental de recursividad
- Excelente ejemplo de divide y vencerás
- Demuestra la elegancia de la programación recursiva
- Las pruebas cubren aspectos matemáticos y técnicos
- Base sólida para algoritmos recursivos más complejos
- Enseña a pensar en términos de casos base y recursivos
