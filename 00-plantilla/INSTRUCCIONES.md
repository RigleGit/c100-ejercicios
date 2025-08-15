# INSTRUCCIONES PARA USAR LA PLANTILLA

Esta plantilla proporciona una estructura completa y profesional para crear nuevos ejercicios de C. Sigue estos pasos para crear un nuevo ejercicio:

## Paso 1: Copiar la Plantilla

```bash
# Copiar la plantilla al nuevo ejercicio
cp -r 00-plantilla XX-categoria/XXX-nombre-ejercicio/

# Ejemplo:
cp -r 00-plantilla 01-fundamentos-c/011-nuevo-ejercicio/
```

## Paso 2: Renombrar Archivos

Renombra los archivos para que coincidan con el nombre del ejercicio:

```bash
cd XX-categoria/XXX-nombre-ejercicio/

# Renombrar header
mv include/nombre_ejercicio.h include/nuevo_ejercicio.h

# Renombrar implementación
mv src/nombre_ejercicio.c src/nuevo_ejercicio.c

# Renombrar tests
mv tests/test_nombre_ejercicio.c tests/test_nuevo_ejercicio.c
```

## Paso 3: Buscar y Reemplazar Marcadores

Usa tu editor de texto o comandos para reemplazar los siguientes marcadores:

### Marcadores Principales

| Marcador | Ejemplo de Reemplazo |
|----------|---------------------|
| `[NOMBRE DEL EJERCICIO]` | `Cálculo del Área de un Círculo` |
| `[NOMBRE_PROYECTO]` | `AreaCirculo` |
| `[DESCRIPCIÓN BREVE]` | `Programa que calcula el área de un círculo dado su radio` |
| `[DESCRIPCIÓN DETALLADA]` | `Este ejercicio implementa el cálculo del área...` |
| `[FECHA]` | `6 de agosto de 2025` |
| `[CATEGORÍA]` | `Fundamentos de C` |
| `[NIVEL DE DIFICULTAD]` | `Principiante` |
| `[TIEMPO]` | `30` |
| `[COMPLEJIDAD TEMPORAL]` | `1` |
| `[COMPLEJIDAD ESPACIAL]` | `1` |

### Marcadores de Código

| Marcador | Reemplazar con |
|----------|----------------|
| `nombre_ejercicio` | `area_circulo` |
| `NOMBRE_EJERCICIO_H` | `AREA_CIRCULO_H` |
| `funcion_principal` | `calcular_area_circulo` |
| `funcion_auxiliar1` | `validar_radio` |
| `funcion_auxiliar2` | `formatear_resultado` |
| `estructura_principal_t` | `circulo_t` |

### Usando sed (Linux/macOS)

```bash
# Reemplazar en todos los archivos
find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.md" -o -name "CMakeLists.txt" -o -name "Makefile" \) -exec sed -i '' 's/nombre_ejercicio/area_circulo/g' {} \;

find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.md" -o -name "CMakeLists.txt" -o -name "Makefile" \) -exec sed -i '' 's/\[NOMBRE DEL EJERCICIO\]/Cálculo del Área de un Círculo/g' {} \;

# Continuar con otros marcadores...
```

## Paso 4: Implementar la Lógica Específica

### En `include/nuevo_ejercicio.h`:

1. Modificar la estructura `estructura_principal_t` según necesidades
2. Cambiar los prototipos de funciones
3. Ajustar las constantes
4. Actualizar la documentación

### En `src/nuevo_ejercicio.c`:

1. Implementar `funcion_principal` con la lógica del ejercicio
2. Implementar las funciones auxiliares
3. Ajustar validaciones según el tipo de ejercicio
4. Modificar manejo de errores específico

### En `src/main.c`:

1. Actualizar los ejemplos en `ejecutar_demostracion()`
2. Modificar los casos de prueba en `ejecutar_casos_prueba()`
3. Ajustar la solicitud de parámetros en el modo interactivo
4. Actualizar la ayuda y descripción

### En `tests/test_nuevo_ejercicio.c`:

1. Implementar tests específicos en `test_logica_especifica()`
2. Ajustar casos de prueba en todas las funciones test
3. Modificar validaciones según los tipos de datos
4. Actualizar valores esperados

## Paso 5: Configurar Build

### CMakeLists.txt:
```cmake
# Actualizar nombre del proyecto
project(AreaCirculo VERSION 1.0.0 LANGUAGES C)

# Actualizar nombres de archivos
set(LIB_SOURCES
    src/area_circulo.c
)

add_executable(area_circulo src/main.c)
add_executable(test_area_circulo tests/test_area_circulo.c)
```

### Makefile:
```makefile
# Actualizar TARGET
TARGET = $(BINDIR)/area_circulo
TEST_TARGET = $(BINDIR)/test_area_circulo

# Actualizar reglas de compilación
$(OBJDIR)/area_circulo.o: $(SRCDIR)/area_circulo.c $(INCDIR)/area_circulo.h | $(OBJDIR)
```

## Paso 6: Crear Archivos de Ejemplo

### `ejemplos/datos_basicos.txt`:
```
1.0
2.5
5.0
10.0
```

### `ejemplos/datos_avanzados.txt`:
```
0.1
100.0
1000.0
```

### `ejemplos/casos_error.txt`:
```
-1.0
0.0
abc
```

## Paso 7: Actualizar README.md

1. Completar la descripción del ejercicio
2. Actualizar ejemplos de entrada/salida
3. Describir el algoritmo implementado
4. Agregar información específica del ejercicio
5. Completar casos de uso educativos

## Paso 8: Verificar y Probar

```bash
# Compilar
make

# Ejecutar tests
make test

# Probar funcionamiento
make demo

# Verificar memoria
make test-memory

# Análisis estático
make static-analysis
```

## Tipos de Ejercicios y Adaptaciones

### Ejercicios Matemáticos
- Usar tipos `double` para cálculos
- Incluir `#include <math.h>`
- Validar rangos numéricos
- Considerar precisión decimal

### Ejercicios con Cadenas
- Incluir `#include <string.h>`
- Definir `#define EJERCICIO_CON_CADENAS`
- Usar funciones seguras (strncpy, strncat)
- Validar longitudes de cadenas

### Ejercicios con Arrays
- Definir `#define EJERCICIO_CON_ARRAYS`
- Implementar validación de límites
- Considerar memoria dinámica si es necesario
- Incluir tests de casos límite

### Ejercicios con Archivos
- Definir `#define EJERCICIO_CON_ARCHIVOS`
- Implementar manejo robusto de errores de E/O
- Validar permisos y existencia de archivos
- Incluir archivos de ejemplo

### Ejercicios con Estructuras de Datos
- Definir estructuras específicas
- Implementar inicialización y liberación
- Considerar operaciones de inserción/eliminación
- Incluir tests de memoria

## Herramientas Adicionales Opcionales

### `tools/benchmark.c`:
Para ejercicios que requieren análisis de rendimiento

### `tools/demo.c`:
Para demostraciones interactivas adicionales

### `tools/generador_datos.c`:
Para crear datos de prueba automáticamente

### `tools/visualizador.c`:
Para ejercicios que pueden beneficiarse de visualización

## Consejos Finales

1. **Mantén consistencia** en nombres y estilo
2. **Documenta bien** todas las funciones
3. **Incluye tests comprehensivos** para todos los casos
4. **Valida todas las entradas** de usuario
5. **Maneja errores gracefully**
6. **Optimiza solo cuando sea necesario**
7. **Escribe código legible** y mantenible

## Checklist de Completitud

- [ ] Todos los marcadores reemplazados
- [ ] Archivos renombrados correctamente
- [ ] Lógica específica implementada
- [ ] Tests actualizados y funcionando
- [ ] README.md completado
- [ ] Archivos de ejemplo creados
- [ ] Build configurado (CMake/Makefile)
- [ ] Programa compila sin warnings
- [ ] Tests pasan completamente
- [ ] Análisis de memoria limpio
- [ ] Documentación completa

¡Tu nuevo ejercicio está listo para usar!
