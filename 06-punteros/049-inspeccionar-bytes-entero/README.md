# Ejercicio 049: Inspeccionar Bytes de Entero

## Comportamiento y límites

Extraer los 32 bits usa UINT32_MAX, sin desplazar por 32. La validación de inicio y longitud evita una suma que podría desbordar.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/inspeccionar_bytes_entero
```

## Pruebas de esta revisión

Bytes, endianess y extracción de anchura completa.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
