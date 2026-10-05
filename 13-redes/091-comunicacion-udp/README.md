# Ejercicio 091: Comunicación UDP

## Comportamiento y límites

El programa acepta emitir HOST PUERTO MENSAJE y recibir PUERTO. Cada recepción obtiene un datagrama; UDP no garantiza entrega ni orden y la API comunica timeouts.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/comunicacion_udp
```

## Pruebas de esta revisión

Loopback en puerto efímero, contenido, estadísticas, timeout y NULL.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

## Dos terminales

```bash
./build/comunicacion_udp recibir 9091
```

```bash
./build/comunicacion_udp emitir 127.0.0.1 9091 "hola"
```

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
