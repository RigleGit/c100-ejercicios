# Verificación del repositorio — 5 de octubre de 2026

Código de la rama local codex/reparar-ejercicios, comprobado hasta la revisión
300876c96e529c031b2fccacc518b936629b0bf1. La documentación y los resultados se guardan después en commits
propios. No se han subido commits.

## Resultados

| Entorno y configuración | Ejercicios configurados, compilados y probados | Pruebas CTest |
|---|---:|---:|
| macOS ARM64, Clang 17, Debug desde cero | 100/100 | 106 |
| Ubuntu 22.04 ARM64 en Docker, GCC 11.4, Debug desde cero | 100/100 | 107 |
| macOS ARM64, Clang 17, Release | 100/100 | 106 |
| Ubuntu 22.04 ARM64, GCC 11.4, Debug con UBSan | 100/100 | 107 |

Linux añade la comprobación de error de escritura mediante /dev/full.
Las cifras cuentan entradas de CTest; cada entrada puede contener varios casos y aserciones.

Las 16 pruebas unittest de herramientas e interfaces pasan en ambos entornos.
Incluyen fallos reales de CTest, timeout, ausencia de pruebas, conservación de fuentes
durante limpieza, propagación del fallo al generar un reporte, EOF de la agenda,
errores de importación y la demostración del 094 con ASan/UBSan.

También se compilaron por separado las 53 cabeceras públicas, con C99 y advertencias
estrictas. La imagen Docker nueva se construyó y ejecutó sin instalar Criterion.

Herramientas locales: CMake 4.4.3 y Python 3.14.7.
En Docker: CMake 3.22.1 y Python 3.10.12.

## Evidencia y reproducción

[Resultados macOS Release](verificacion/2026-10-05/macos-release.json),
[resultados Linux UBSan](verificacion/2026-10-05/linux-ubsan.json) y
[resúmenes de ejecución](verificacion/2026-10-05/) contienen los resultados por ejercicio.

```bash
./tools/test_everything_fresh.sh
python3 -m unittest discover -s tools/tests -p 'test_*.py'
C100_MODE=Release C100_OUTPUT=/tmp/c100-release python3 tools/verify_all.py
CFLAGS="-fsanitize=undefined -fno-sanitize-recover=all -g" C100_OUTPUT=/tmp/c100-ubsan python3 tools/verify_all.py
```

verify_all.py guarda results.json y logs de configuración, compilación y CTest.
Usa un directorio de salida nuevo al cambiar compilador, plataforma o flags:
CMake conserva esos valores en su caché.

## Correcciones principales

| Área | Corrección |
|---|---|
| Lanzadores | Ejecutar todos los tests, rechazar ausencia/selección vacía y transmitir fallos/timeouts |
| Limpieza | Borrar solo build; preservar fuentes, fixtures, copias y volúmenes Docker ajenos |
| 022/025/029/049/082/096 | Límites numéricos, exponentes extremos, máscaras completas y terminación |
| 038/043/045/046 | Bytes UTF-8, aritmética válida de punteros, recorrido inverso y copia solapada |
| 059/061/063/064/070 | Cabeceras autosuficientes y constantes/tipos portables |
| 069 | Tamaño e inserciones del árbol coherentes |
| 075/077 | No borrar archivos al rechazar origen, autocopia o lote inválido |
| 081/085–087 | DEBUG verificable y formatos int64_t/uint64_t portables |
| 088/091/092 | Ejecutables conectados a las implementaciones reales |
| 093/094/095/097 | Cuerpo HTTP, entrada limitada, estadísticas reales, formato de diagnóstico y const |
| 099 | CSV con comillas/LF, importación sin pérdida, escritura comprobada, backup y errores visibles |
| Documentación | Números alineados, un único 094, comandos y pruebas existentes, retirada de cifras ficticias |

Antes de la reparación había 101 carpetas para 100 números. Con Criterion instalado,
100 configuraciones CMake fallaban por referenciar archivos de test ausentes.
El lanzador podía devolver éxito pese a errores o a cero pruebas.

Los cambios quedan en commits locales separados por corrección o bloque.
La alternativa histórica del 094 está excluida de esta matriz.

## Alcance

Las pruebas validan contratos concretos, no todos los caminos de cada API.
Los sockets se prueban localmente, sin servicios externos. No se ha medido rendimiento
ni se ha verificado exhaustivamente la ausencia de fugas o carreras; las variantes
del 086 con varios hilos sin sincronización son demostraciones de comportamiento indefinido.
HTTP no incluye TLS ni decodificación chunked. Los binarios de registros son nativos.
La plantilla 00 y las herramientas históricas de alternativas no forman parte de los
100 ejercicios verificados.

El workflow de GitHub Actions queda preparado para Linux y macOS. No se ha ejecutado
en GitHub porque no se ha hecho push; los resultados de la tabla son ejecuciones locales.
