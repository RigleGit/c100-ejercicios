# Ejercicio 048 — Buscar carácter con punteros

## Descripción

Colección de funciones para buscar caracteres en cadenas usando punteros en C. Incluye variantes: primera ocurrencia, última, insensible a mayúsculas, búsqueda en letras, desde una posición, contar ocurrencias, encontrar todas las posiciones, buscar cualquiera/no en un conjunto, primeros dígitos/no-blancos, reemplazo in-situ, y utilidades de debug y comparación con `strchr`.

## Objetivos

- Practicar aritmética de punteros y manipulación de cadenas en C.
- Aprender a retornar punteros válidos desde funciones.
- Comparar implementaciones propias con funciones estándar (`strchr`).

## API pública (firmas importantes)

```c
char* buscarCaracter(char *str, char c);
char* buscarCaracter_debug(char *str, char c);
char* buscarUltimoCaracter(char *str, char c);
char* buscarCaracterInsensible(char *str, char c);
char* buscarCaracterEnLetras(char *str, char c);
char* buscarCaracterDesde(char *str, char c, int desde);
int contarCaracter(char *str, char c);
int encontrarTodasPosiciones(char *str, char c, int posiciones[], int max_posiciones);
char* buscarCualquierCaracter(char *str, const char *conjunto);
char* buscarCaracterNoEn(char *str, const char *conjunto);
char* buscarPrimerDigito(char *str);
char* buscarPrimerNoBlanco(char *str);
int reemplazarCaracter(char *str, char buscar, char reemplazar);

void compararImplementaciones(char *str, char c);
void demoBusquedas(char *str, char c);
void demoBusquedasAvanzadas(char *str);
```

## Cómo compilar

Desde el directorio del ejercicio:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/buscar_caracter_punteros
```

## Cómo ejecutar

Después de compilar, el ejecutable (p. ej. `buscar_caracter_punteros`) se encuentra en `build/`. Ejecuta desde `build/`:

```bash
./buscar_caracter_punteros
```

El programa muestra varias demostraciones automáticas y ejemplos interactivos.

## Ejemplos

- `buscarCaracter("Hola", 'o')` → puntero a la 'o' en "Hola".
- `contarCaracter("banana", 'a')` → 3.
- `reemplazarCaracter("Hola mundo", 'o', '0')` → modifica la cadena in-situ.

## Notas

- Las funciones devuelven `NULL` cuando no se encuentra el carácter o cuando los argumentos son inválidos.
- Algunas funciones (p. ej. `buscarCaracter_debug`) imprimen información de depuración detallada y deben usarse sólo para análisis.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/buscar_caracter_punteros
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
