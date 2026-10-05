# Ejercicio 081: Compilación Condicional

## Comportamiento y límites

La demostración normal compila sin DEBUG. CTest construye también una variante DEBUG y verifica su salida real. Las aserciones de pruebas permanecen activas en Release.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/main_sin_debug
```

## Pruebas de esta revisión

Rangos, memoria y salida de DEBUG frente a Release.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
