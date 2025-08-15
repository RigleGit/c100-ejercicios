# tools — Lista y uso actual

Scripts disponibles:

- `clean_all.sh` — Limpia archivos de compilación y binarios generados.
- `docker_build.sh` — Ayuda a construir/ejecutar la imagen Docker del proyecto (build/run/test/clean).
- `docker_test_runner.sh` — Runner auxiliar para ejecutar tests dentro de contenedores.
- `quick_test.sh` — Ejecuta tests rápidos por bloque (acepta número de bloque como argumento opcional).
- `prueba_everything_fresh.sh` — Script maestro para ejecutar la suite completa desde cero (limpieza + tests).

Uso rápido:

```bash
# Ejecutar el script maestro
./tools/prueba_everything_fresh.sh

# Ejecutar tests rápidos por bloque
./tools/quick_test.sh 5

# Limpiar proyecto
./tools/clean_all.sh
```
