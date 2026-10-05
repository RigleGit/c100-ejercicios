# Ejercicio 082: Reestructurar Programa en Varios Archivos

## Comportamiento y límites

La raíz entera usa búsqueda binaria y compara medio con numero/medio. Devuelve el suelo de la raíz para entradas no negativas y -1 para negativas. Las otras funciones matemáticas tienen dominios limitados; para aritmética comprobada consulta el 096.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/reestructurar_archivos
```

## Pruebas de esta revisión

Raíces exactas/no exactas, cero, negativos y módulo.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/) y [lib/](lib/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
