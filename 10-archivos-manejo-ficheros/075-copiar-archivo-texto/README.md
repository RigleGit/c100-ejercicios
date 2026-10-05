# Ejercicio 075: Copiar archivo de texto

## Enunciado

- Uso básico de la API de ficheros en C (`fopen`, `fread`, `fwrite`, `fclose`).
- Manejo de errores y comprobaciones de retorno.
- Lectura/escritura en bloques para eficiencia.
- Manejo de rutas y permisos.

## Comportamiento y límites

Las copias rechazan origen y destino con el mismo dispositivo e inode. Se abre el origen antes de truncar el destino y se comprueban lectura y cierre. Un fallo posterior a abrir el destino puede dejarlo parcial: no son guardados transaccionales.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/copiar_archivo_texto
```

## Pruebas de esta revisión

Bytes exactos con NUL, vacío, bloque inválido, copia a sí misma y origen inexistente.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
