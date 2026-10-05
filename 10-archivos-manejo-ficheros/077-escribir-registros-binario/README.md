# Ejercicio 077: Escribir Registros Binario

## Enunciado

Define una estructura `Producto` con campos como código (entero) y precio (float). Crea un array con varios productos, asígnales valores, y escribe todos los registros en un archivo binario llamado `productos.dat` usando `fwrite`. Asegúrate de usar `sizeof` correctamente para escribir el número adecuado de elementos.

Este ejercicio introduce el uso de archivos binarios y refuerza el manejo de estructuras y funciones de escritura en C.

## Comportamiento y límites

El escritor básico valida el lote antes de abrir con wb y comprueba fwrite/fclose. Producto se escribe en formato nativo: tamaño, padding y endianess no son portables entre máquinas.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/escribir_registros_binario
```

## Pruebas de esta revisión

Escritura leída con fread independiente, append y lote inválido sin borrar datos.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
