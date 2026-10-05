# Herramientas de verificación

| Comando desde la raíz | Efecto |
|---|---|
| ./tools/quick_test.sh | Compila y prueba los 100 ejercicios |
| ./tools/quick_test.sh 12 | Prueba un bloque |
| ./tools/quick_test.sh 12 85 | Prueba el ejercicio indicado del bloque |
| ./tools/test_everything_fresh.sh | Borra build y ejecuta toda la batería |
| ./tools/clean_all.sh | Simula la eliminación de carpetas build |
| ./tools/clean_all.sh --apply | Elimina solo carpetas build |
| ./tools/docker_build.sh test | Construye Docker y ejecuta la batería |
| ./tools/docker_test_runner.sh report | Guarda resultados y transmite fallos |
| python3 tools/verify_all.py | Verifica exactamente 001–100 y guarda JSON y logs en .verification |

quick_test.sh admite C100_TEST_TIMEOUT, en segundos enteros positivos (15 por defecto).
Falla por selección vacía, falta de pruebas, compilación, resultado de CTest o timeout.
build/compile.log y build/test-list.log ayudan a reproducir los fallos.

La antigua limpieza --hard se ha retirado porque podía eliminar fuentes y datos.
Los scripts pueden invocarse con rutas absolutas desde otra carpeta.

Las pruebas de herramientas usan unittest y proyectos CMake reales:

```bash
python3 -m unittest discover -s tools/tests -p 'test_*.py'
```

test_cli_exercises.py se llama desde CTest y recibe número y ruta del ejecutable.
No debe ejecutarse como script sin esos argumentos. El resto de test_*.py
son suites unittest. test_entrada_cli.py compila su demostración con ASan/UBSan.

verify_all.py admite C100_MODE=Debug o Release y C100_OUTPUT para aislar el directorio de compilación. Rechaza números ausentes o duplicados. Usa una salida nueva si cambias los flags CFLAGS.
