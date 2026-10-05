# Probar con Docker

Docker proporciona Ubuntu 22.04, GCC/Clang, CMake, Make y Python 3.
Criterion no es una dependencia de esta batería.

Desde la raíz:

```bash
./tools/docker_build.sh build
./tools/docker_build.sh test
./tools/docker_build.sh test-block 12
```

test ejecuta los 100 ejercicios desde cero. test-block ejecuta el bloque indicado.
Los scripts propagan los fallos y no dan por válido un ejercicio sin pruebas.

El contenedor de desarrollo monta el repositorio en /workspace:

```bash
./tools/docker_build.sh run
```

Las carpetas build contienen cachés dependientes del sistema. Si alternas una
compilación local y otra en Docker, ejecuta el comando test para empezar desde cero.

El reporte dentro del contenedor se genera con:

```bash
./tools/docker_test_runner.sh report
```

Devuelve un código distinto de cero cuando falla la batería y guarda el log en
/tmp/c100_test_report.txt. Puedes cambiar la ruta con C100_REPORT_PATH.

clean de docker_build elimina únicamente los contenedores e imagen nombrados
por este proyecto. No ejecuta una limpieza global de volúmenes Docker.
