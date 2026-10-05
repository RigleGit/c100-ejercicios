# Ejercicio 093: Cliente HTTP

## Enunciado

- **Protocolo HTTP**: Comprensión del protocolo HTTP/1.0 y HTTP/1.1
- **Sockets TCP**: Uso de getaddrinfo, connect, send, recv
- **Parsing de URLs**: Descomposición de URLs en sus componentes
- **Gestión de buffers**: Manejo eficiente de datos de red
- **Análisis de respuestas**: Parsing de headers y contenido HTTP
- **Estadísticas de red**: Medición de tiempos y throughput

## Comportamiento y límites

respuesta.contenido contiene solo el cuerpo; bytes_recibidos incluye cabeceras y cuerpo. Se rechazan respuestas sin cabeceras completas. Es HTTP didáctico: no implementa TLS ni decodifica chunked. La recepción con cuerpo termina al cerrar el peer o vencer el timeout.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/cliente_http
```

## Pruebas de esta revisión

URL, petición HTTP/1.1, cabeceras y cuerpo por socket real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
