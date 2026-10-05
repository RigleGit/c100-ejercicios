# Ejercicio 043: Introducción a Punteros

## Comportamiento y límites

Se comparan por igualdad punteros a variables independientes. La resta y el orden relacional se reservan a punteros del mismo array.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/introduccion_punteros
```

## Pruebas de esta revisión

Modificación/intercambio mediante punteros y NULL.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
