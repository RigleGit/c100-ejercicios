# Contribuir a los ejercicios

Indica el ejercicio y un caso reproducible. Un buen informe incluye entrada,
salida esperada, salida actual, sistema operativo y versión del compilador.

## Desarrollo

Usa C99, UTF-8, cuatro espacios y las advertencias
-Wall -Wextra -Werror -pedantic. Los ejercicios POSIX requieren Linux/macOS o WSL.

La configuración compartida está en cmake/Exercise.cmake. Un ejercicio normal usa:

```cmake
cmake_minimum_required(VERSION 3.16)
project(nombre C)
include(../../cmake/Exercise.cmake)
c100_exercise(nombre src/main.c src/modulo.c)
```

El helper registra tests/test_*.c y test/test_*.c en CTest. Las pruebas llaman a la
implementación real: no copies el algoritmo dentro del test. Si la solución contiene
main, solo el objeto usado por las pruebas renombra ese símbolo. No hace falta
añadir una API artificial a la solución.

Los ejercicios iniciales comprueban su interfaz de consola con la biblioteca estándar
de Python en tools/tests/test_cli_exercises.py. Las pruebas C usan assert; se conservan
activas en Release. Un caso debe detectar un error de resultados, límites o ciclo
de vida. Evita probar el texto exacto de comentarios o detalles internos sin contrato.

## Antes del commit

```bash
./tools/quick_test.sh BLOQUE EJERCICIO
python3 -m unittest discover -s tools/tests -p 'test_*.py'
git diff --check
```

Para modificar una función defectuosa, reproduce primero el fallo con una prueba
del contrato y verifica después la corrección. No atribuyas a las pruebas cobertura
de memoria, rendimiento o todos los errores si esos caminos no se han comprobado.

Actualiza el README y el índice cuando cambien comandos, números o límites.
Usa commits con propósito concreto: fix, test, build o docs. Publica solo los cambios
que hayan sido revisados; los commits locales pueden verificarse antes de subirlos.
