# Ejercicio 075: Copiar archivo de texto

## Descripción

Ejercicio para practicar operaciones de E/S de archivos en C: abrir, leer, escribir y cerrar ficheros. El objetivo es implementar una función que copie el contenido de un archivo de texto a otro manejando correctamente errores y recursos.

## Objetivos de aprendizaje

- Uso básico de la API de ficheros en C (`fopen`, `fread`, `fwrite`, `fclose`).
- Manejo de errores y comprobaciones de retorno.
- Lectura/escritura en bloques para eficiencia.
- Manejo de rutas y permisos.

## Estructura del proyecto

```
075-copiar-archivo-texto/
├── include/
cd build
# Ejercicio 075: Copiar archivo de texto

## Descripción

Implementación ampliada de utilidades para copiar archivos de texto y binarios con varios métodos y funcionalidades auxiliares: copia línea por línea, carácter por carácter, copia por bloques con buffer configurable, copia con progreso, verificación (tamaño/contenido/checksum), backups y copia en lote. El `main` incluido crea un archivo de ejemplo y ejecuta demos automáticas.

## Objetivos de aprendizaje

- Manejo de E/S en C: `fopen`, `fread`, `fwrite`, `fclose`.
- Diferentes estrategias de copia y su impacto en rendimiento.
- Verificación de integridad de ficheros (por tamaño, por contenido y checksum).
- Implementar progreso y medición de velocidad de transferencia.
- Buenas prácticas: comprobación de errores, manejo de recursos y backups.

## Conceptos clave

- Métodos de copia: línea por línea, carácter por carácter y por bloques.
- Verificación: comparar tamaños, comparar contenido por bloques o calcular checksum.
- Copia con callback para mostrar progreso en tiempo real.
- Copia en lote y creación/restauración de backups automáticos.

## Estructura del proyecto

```
075-copiar-archivo-texto/
├── include/
│   └── copiar_archivo.h         # Declaraciones públicas
├── src/
│   └── copiar_archivo.c         # Implementación completa (métodos, verificación, demos)
├── tests/
│   └── test_dummy.c             # Tests (CMake genera dummy si Criterion está disponible)
├── CMakeLists.txt
└── README.md
```

## Compilación

Requisitos: compilador C (gcc/clang) y CMake opcional.

### Usando CMake (recomendado)

```bash
mkdir -p build && cd build
cmake ..
make -j
```

### Compilación manual (ejemplos)

```bash
gcc -std=c11 -Wall -Wextra -O2 -pthread src/copiar_archivo.c -I include -o copiar_archivo

# Para compilar el ejecutable principal si hay un main separado
gcc -std=c11 -Wall -Wextra -O2 -pthread src/copiar_archivo.c src/main.c -I include -o copiar_archivo_main
```

> Nota: el `CMakeLists.txt` actual monta un test dummy si se detecta Criterion a través de pkg-config; los tests reales se ejecutan solo si Criterion está instalado en el sistema.

## Uso / Ejemplos

Tras compilar con CMake, desde la raíz del repo:

```bash
cd build
./hilos_basicos_main  # no aplica aquí, usar el binario generado para este ejercicio
```

Comandos útiles (desde la raíz o desde `build` si compilas con CMake):

```bash
# Copia básica: crea 'origen.txt' de ejemplo y ejecuta la copia básica
./build/copy_example || ./copiar_archivo_main

# Ejecutar demo de comparación de métodos (crea archivos temporales)
./build/copiar_archivo_main    # y seleccionar la opción correspondiente en el menú interactivo

# Ejecutar copia configurada con verificación de contenido (si el programa ofrece flags)
./build/copiar_archivo_main --demo verify
```

Ejemplo rápido (si compilaste manualmente como `copiar_archivo`):

```bash
# Crear un archivo de prueba
printf "Línea 1\nLínea 2\n" > origen.txt

# Copiar por bloques con el ejecutable manual
./copiar_archivo origen.txt copia.txt

# Verificar checksum (si implementado como función exportable o herramienta)
```

## API pública (resumen)

Las funciones principales expuestas en `include/copiar_archivo.h` (resumen):

- `ResultadoCopia copiar_archivo_basico(const char* origen, const char* destino)`
- `ResultadoCopia copiar_linea_por_linea(const char* origen, const char* destino)`
- `ResultadoCopia copiar_caracter_por_caracter(const char* origen, const char* destino)`
- `ResultadoCopia copiar_por_bloques(const char* origen, const char* destino, size_t tamaño_buffer)`
- `ResultadoCopia copiar_archivo_configurado(const char* origen, const char* destino, const ConfiguracionCopia* config)`
- `ResultadoCopia copiar_archivo_con_progreso(const char* origen, const char* destino, void (*callback)(const ProgresoCopia*))`
- `ResultadoCopia copiar_archivos_lote(const char** origenes, const char** destinos, int num, const ConfiguracionCopia* config)`
- `bool verificar_archivos_identicos(const char* a, const char* b, TipoVerificacion tipo)`
- `unsigned long calcular_checksum_archivo(const char* nombre)`

Tipos y estructuras principales (en `include`): `ResultadoCopia`, `ConfiguracionCopia`, `ProgresoCopia`, `TipoVerificacion`, `MetodoCopia`.

## Tests

Para ejecutar los tests con CTest (si compilaste con CMake):

```bash
cd build
ctest --output-on-failure
```

O ejecutar directamente el ejecutable de test si se generó:

```bash
./build/test_dummy
```

> Nota: CMake solo configura `test_dummy` si Criterion está disponible en el sistema.

## Ejemplos de uso avanzados (demos incluidos en `main`)

- Comparar rendimiento de métodos: `comparar_metodos_copia("origen.txt", "dest_base")` — el `main` ejecuta esto automáticamente como demo.
- Copia con verificación: `copiar_archivo_configurado(..., verificacion=VERIFICACION_CONTENIDO, crear_backup=true)` — crea backup antes de sobrescribir y verifica contenido.
- Copia con progreso: `copiar_archivo_con_progreso(..., callback_progreso_defecto)` — imprime porcentaje, velocidad y tiempo estimado.
- Copia en lote: `copiar_archivos_lote(...)` — procesa múltiples pares origen/destino con la misma configuración.

## Buenas prácticas y notas

- Comprobar siempre valores de retorno (`fopen`, `fread`, `fwrite`).
- Usar buffers razonables (4KB o 64KB según tamaño) para optimizar rendimiento.
- Al sobrescribir archivos importantes, habilitar backups antes de escribir.
- Para transferencias grandes, preferir copia por bloques en modo binario.

## Referencias

- Documentación estándar de C (man fopen, fread, fwrite)
- Tutoriales sobre E/S en C y manejo de ficheros

