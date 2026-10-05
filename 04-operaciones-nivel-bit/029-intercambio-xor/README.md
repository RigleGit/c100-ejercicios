# Ejercicio 029: Intercambio de Variables con XOR

## Enunciado

Implementar el intercambio de variables sin usar memoria adicional utilizando las propiedades matemáticas del operador XOR.

## Comportamiento y límites

XOR no modifica un valor cuando ambos punteros coinciden. Las variantes aritméticas comprueban límites y recurren al intercambio temporal si la operación intermedia no cabe; también cubren ceros e INT_MIN.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/intercambio_xor
```

## Pruebas de esta revisión

Alias, extremos, alternativas aritméticas y consola.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
