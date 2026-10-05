# Ejercicio 094: Entrada Segura de Datos

## Comportamiento y límites

Pasa la capacidad real del buffer. Tamaños menores que 2 se rechazan. La lectura limpia elimina LF y descarta el resto de una línea truncada. Las estadísticas indican bytes conservados en la última lectura y líneas realmente leídas. La demostración de gets usa lectura limitada, sin escribir fuera del buffer.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/entrada_segura
```

## Pruebas de esta revisión

Truncamiento, línea siguiente, tamaño 1, estadísticas y EOF; tres pruebas CLI con ASan/UBSan.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
