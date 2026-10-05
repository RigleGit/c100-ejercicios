# Aprende C paso a paso: 100 ejercicios resueltos

Repositorio de código y ejercicios que acompaña a la edición revisada del libro.
Hay 100 ejercicios numerados, en 15 bloques, desde el primer programa hasta archivos,
hilos, sockets, una agenda de contactos y un evaluador RPN.

Empieza por el [índice de ejercicios](docs/EJERCICIOS.md). Cada entrada enlaza el código,
los comandos de compilación y las pruebas de su carpeta.

## Empezar

Necesitas un compilador C, CMake 3.16 o posterior, Make, Bash y Python 3.
Los ejercicios de redes y concurrencia usan APIs POSIX: Linux, macOS o Windows con WSL.

```bash
git clone https://github.com/RigleGit/c100-ejercicios.git
cd c100-ejercicios
./tools/quick_test.sh 1 1
./01-fundamentos-c/001-hola-mundo/build/hola_mundo
```

En Ubuntu/Debian, las dependencias mínimas son build-essential, cmake y python3.
En macOS, instala las herramientas de línea de comandos de Xcode, CMake y Python 3.
Las pruebas no necesitan Criterion.

## Cómo practicar

1. Lee el enunciado y escribe tu propia solución antes de consultar src/.
2. Prueba un caso normal, un límite y una entrada inválida dentro del dominio indicado.
3. Compara tu resultado con la solución y explica por qué funciona.
4. Ejecuta CTest; un resultado correcto en algunos casos no prueba todos los caminos.

Las soluciones y ampliaciones sirven para aprender. En particular, las demostraciones
de carreras de datos del 086 tienen comportamiento no determinista y no se usan como
patrón para código sincronizado.

## Compilar y probar

Desde la raíz:

```bash
./tools/quick_test.sh             # Todos los ejercicios
./tools/quick_test.sh 5           # Un bloque
./tools/quick_test.sh 12 85        # Un ejercicio
./tools/test_everything_fresh.sh  # Borra solo build y prueba desde cero
python3 -m unittest discover -s tools/tests -p 'test_*.py'
```

Desde la carpeta de un ejercicio:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

El nombre del ejecutable está en el [índice](docs/EJERCICIOS.md).
El lanzador falla si la selección no existe, si no hay pruebas, si falla una prueba
o si se agota su tiempo. Mantiene los logs en build/.

## Docker

```bash
./tools/docker_build.sh test
./tools/docker_build.sh test-block 12
```

Consulta la [guía Docker](DOCKER_README.md). Para borrar compilaciones locales,
usa ./tools/clean_all.sh para ver una simulación y añade --apply para borrar solo
las carpetas build. La limpieza conserva fuentes, fixtures y backups.

## Correspondencia con el libro

La edición revisada usa:

| Número | Tema |
|---|---|
| 050 | Punteros void |
| 070 | Lista doblemente enlazada |
| 085 | Suma paralela |
| 086 | Condiciones de carrera |
| 087 | Sincronización con mutex |
| 094 | Entrada segura, implementación revisada |

La versión anterior del 094 se conserva en [alternativas/](alternativas/README.md)
y queda fuera de los 100 ejercicios. Los nombres de 085–087 han cambiado respecto
al repositorio anterior: usa el índice de la edición correspondiente.

## Estado y novedades

El [informe de verificación](docs/VERIFICACION.md) documenta el entorno, los resultados
y el alcance de las pruebas. Una batería que pasa no garantiza ausencia de todos los
errores posibles.

Los recursos del libro están disponibles sin registro:
[repositorio público](https://github.com/RigleGit/c100-ejercicios).
Para recibir avisos de cambios, usa Watch en GitHub y selecciona las notificaciones
que quieras. Las futuras ediciones y sus recursos se anunciarán aquí; la suscripción
a una futura newsletter será opcional y no será necesaria para acceder al código.

Para comunicar un fallo, abre un [issue](https://github.com/RigleGit/c100-ejercicios/issues)
con la entrada, la salida esperada, el ejercicio y la versión del compilador.

## Contribuir y licencia

Consulta [CONTRIBUTING.md](CONTRIBUTING.md).
El código se distribuye bajo la [licencia MIT](LICENSE).
