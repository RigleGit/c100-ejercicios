# Comunicar problemas

Abre un issue con el número del ejercicio, la entrada, el resultado esperado
y la salida obtenida. Añade el sistema operativo y estas versiones:

```bash
cc --version
cmake --version
python3 --version
```

Para reproducir un fallo usa el comando del README del ejercicio o:

```bash
./tools/quick_test.sh BLOQUE EJERCICIO
```

Adjunta build/compile.log si falla la compilación y la salida de CTest si falla
una prueba. El lanzador rechaza selecciones vacías y ejercicios sin pruebas.

Si has alternado compilaciones locales y Docker, empieza de cero con
./tools/test_everything_fresh.sh. Solo elimina las carpetas build.
