# Ejercicio 046: Copiar Cadena con Punteros

## Enunciado

Implementar diferentes métodos para copiar cadenas de caracteres utilizando punteros, demostrando el manejo correcto de arrays de caracteres terminados en `'\0'` y la importancia de la gestión segura de memoria.

## Comportamiento y límites

copiarCadena_overlap usa memmove incluyendo el terminador. El destino debe disponer de capacidad suficiente. La variante limitada recibe esa capacidad; invertir una cadena vacía no mueve el puntero antes del buffer.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/copiar_cadena_punteros
```

## Pruebas de esta revisión

Copia, truncamiento, solapamiento y terminación.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
