# Ejercicio 012: Máximo de Dos Números

## Enunciado

Escribe un programa que lea dos números introducidos por el usuario y muestre cuál de los dos es mayor (o si son iguales).

## Descripción

Este ejercicio es ideal para afianzar el uso de **estructuras condicionales** `if-else` y practicar la **comparación entre valores**. Ayuda a entender cómo los programas pueden comportarse de forma distinta según la entrada que reciben, una habilidad básica para cualquier lenguaje de programación.

## Conceptos Clave

- **Estructuras condicionales**: `if`, `else if`, `else`
- **Operadores de comparación**: `>`, `<`, `==`
- **Control de flujo**: Toma de decisiones basada en condiciones
- **Entrada de múltiples valores**: Usar `scanf()` varias veces
- **Lógica de comparación**: Casos mutuamente excluyentes

## Explicación del Algoritmo

El programa utiliza una **cadena de decisiones** para comparar dos números:

1. **Primera condición**: `if (a > b)` - Si el primer número es mayor
2. **Segunda condición**: `else if (b > a)` - Si el segundo número es mayor
3. **Caso por defecto**: `else` - Si ninguna de las anteriores es cierta (son iguales)

Esta estructura garantiza que **solo una** de las tres ramas se ejecute.

## Ejemplo de Ejecución

```
Introduce el primer número: 15
Introduce el segundo número: 8
El mayor es: 15
```

```
Introduce el primer número: 3
Introduce el segundo número: 12
El mayor es: 12
```

```
Introduce el primer número: 7
Introduce el segundo número: 7
Ambos números son iguales.
```

```
Introduce el primer número: -5
Introduce el segundo número: -10
El mayor es: -5
```

## Conceptos Avanzados

### Operadores de Comparación
- `a > b`: Mayor que
- `a < b`: Menor que  
- `a == b`: Igual a
- `a >= b`: Mayor o igual que
- `a <= b`: Menor o igual que
- `a != b`: Diferente de

### Precedencia de Operadores
Los operadores de comparación tienen menor precedencia que los aritméticos:
```c
if (a + 1 > b * 2)  // Primero se calculan a+1 y b*2, luego se comparan
```

### Estructura Condicional Anidada
Una alternativa sería usar `if` anidados:
```c
if (a > b) {
    printf("El mayor es: %d\n", a);
} else {
    if (b > a) {
        printf("El mayor es: %d\n", b);
    } else {
        printf("Ambos números son iguales.\n");
    }
}
```

## Errores Comunes

1. **Usar `=` en lugar de `==`**: `if (a = b)` asigna en lugar de comparar
2. **No contemplar el caso de igualdad**: Olvidar el `else` final
3. **Lógica incorrecta**: Usar `&&` o `||` innecesariamente
4. **Formato de entrada**: No manejar correctamente múltiples `scanf()`

## Extensiones Posibles

1. **Máximo de tres números**: Extender la lógica a tres valores
2. **Máximo de N números**: Usar un bucle para comparar múltiples valores
3. **Mostrar ambos números**: Indicar cuál es mayor y cuál menor
4. **Validación de entrada**: Verificar que la entrada sean números válidos
5. **Diferencia entre números**: Mostrar también la diferencia absoluta
6. **Interfaz mejorada**: Menú para repetir la comparación

## Variantes del Problema

- **Mínimo de dos números**: Cambiar `>` por `<`
- **Ordenar dos números**: Mostrar en orden ascendente o descendente
- **Comparar caracteres**: Usar el mismo concepto con `char`
- **Comparar decimales**: Usar `float` o `double`

## Algoritmos Relacionados

- **Ordenamiento burbuja**: Extensión para múltiples elementos
- **Búsqueda del máximo**: En arrays o listas
- **Algoritmos de selección**: Encontrar el k-ésimo elemento mayor

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/maximo_dos
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
