# Ejercicio 099: Agenda de Contactos

## Comportamiento y límites

La agenda guarda CSV con comillas duplicadas y admite LF dentro de campos. La importación valida todo antes de alterar la lista. Para archivos regulares, el guardado usa temporal y rename después de cerrar sin error. El menú comunica errores y cancela con EOF. La API ofrece CSV y backup; se retiraron JSON, binario y metadatos sin implementación.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/agenda_contactos
```

## Pruebas de esta revisión

CSV con comillas/LF, importación fallida, contenido inválido, backup y error de escritura; cinco pruebas CLI.

Código: [tests/test_persistencia.c](tests/test_persistencia.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
