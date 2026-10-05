# Ejercicio 096: Operaciones Aritméticas Seguras

## Comportamiento y límites

La multiplicación comprueba los cuatro casos de signo antes de operar, incluyendo resultados válidos iguales a INT_MIN. Los rangos de sumandos se limitan al dominio de int sin restas que desborden.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/operaciones_aritmeticas_seguras
```

## Pruebas de esta revisión

Límites de suma/resta, división por cero, INT_MIN/-1, INT_MIN/2*2 y rangos.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
