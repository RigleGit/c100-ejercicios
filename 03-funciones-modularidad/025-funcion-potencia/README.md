# Ejercicio 025: Función Potencia

## Enunciado

Implementar una función que calcule la potencia de un número (base^exponente) sin usar la función `pow()` de la biblioteca `<math.h>`. La función debe manejar exponentes enteros positivos, negativos y cero, trabajando con números de tipo `double`.

## Comportamiento y límites

La exponenciación binaria usa una magnitud unsigned para INT_MIN. Un dominio inválido o resultado infinito produce NAN. Se rechaza 0 elevado a un exponente negativo.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/funcion_potencia
```

## Pruebas de esta revisión

Exponentes positivos/negativos, cero, INT_MIN y overflow.

Código: [test/test_potencia.c](test/test_potencia.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
