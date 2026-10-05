# Ejercicio 097: Const-Correctness en C

## Comportamiento y límites

La búsqueda devuelve el índice real, contando espacios. Los campos const se inicializan al declarar configuracion_t. inicializar_configuracion solo cambia nombre/activo si id y factor coinciden con los valores inmutables; en caso contrario devuelve false sin cambios.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/const_correctness
```

## Pruebas de esta revisión

Índices con espacios, entrada const intacta, truncamiento y rechazo de cambios a const.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
