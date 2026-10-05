# Ejercicio 038: Longitud de Cadena

## Comportamiento y límites

Las funciones cuentan puntos de código de una entrada UTF-8 válida. No validan UTF-8 ni cuentan grafemas. La inspección muestra bytes en hexadecimal; el terminador se localiza en bytes.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/longitud_cadena
```

## Pruebas de esta revisión

ñ, á, línea vacía y salida UTF-8 válida.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
