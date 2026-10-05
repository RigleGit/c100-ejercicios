# Ejercicio 088: Patrón Productor-Consumidor

## Comportamiento y límites

El ejecutable inicia un productor y un consumidor reales. Transfiere los valores 1 a 1000 y comprueba su orden FIFO. El módulo usa mutex y condiciones; consumir puede devolver error si vence la espera.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/productor_consumidor
```

## Pruebas de esta revisión

FIFO con capacidad 3 y 1000 elementos entre dos hilos reales.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
