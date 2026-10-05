# Ejercicio 022: Factorial Recursivo

## Enunciado

Escribe una función recursiva `long factorial(int n)` que calcule el factorial de un número entero no negativo. Usa esta función desde `main` para leer un número y mostrar su factorial.

## Comportamiento y límites

factorial(n) devuelve -1 si n es negativo o el resultado no cabe en long. El límite se calcula con LONG_MAX antes de descender por la recursión. 0! vale 1.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/factorial_recursivo
```

## Pruebas de esta revisión

Cero, negativos y límite de long.

Código: [test/test_factorial.c](test/test_factorial.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
