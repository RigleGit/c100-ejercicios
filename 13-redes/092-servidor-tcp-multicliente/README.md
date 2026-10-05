# Ejercicio 092: Servidor TCP Multicliente

## Enunciado

- **Programación de redes**: Sockets TCP, bind, listen, accept
- **Concurrencia con threads**: pthread_create, pthread_detach
- **Arquitectura servidor multicliente**: Patrón thread-per-client
- **Gestión de recursos**: Prevención de memory leaks y resource leaks
- **Sincronización**: Manejo thread-safe de recursos compartidos
- **Manejo de errores**: Error handling robusto en entornos concurrentes

## Comportamiento y límites

El programa acepta eco PUERTO o chat PUERTO y llama al servidor real. Chat difunde a los demás clientes. Las pruebas de registro y transferencia no acreditan ausencia de carreras en todos los caminos de cierre.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/servidor_tcp_multicliente
```

## Pruebas de esta revisión

Dos clientes, límite rechazado, broadcast que excluye al autor y transferencia.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

## Dos terminales

```bash
./build/servidor_tcp_multicliente eco 9092
```

```bash
printf "hola\n" | nc 127.0.0.1 9092
```

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
