# Ejercicio 045: Recorrer Array con Punteros

## Enunciado

Implementar y demostrar diferentes métodos para recorrer arrays utilizando aritmética de punteros en lugar de índices tradicionales, estableciendo los fundamentos para programación de bajo nivel y manipulación directa de memoria.

## Comportamiento y límites

El recorrido inverso parte de una posición después del último elemento y decrementa antes de acceder. No forma un puntero anterior al inicio; longitud cero devuelve suma cero.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/recorrer_array_punteros
```

## Pruebas de esta revisión

Recorridos, sumas, máximo y mínimo.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
