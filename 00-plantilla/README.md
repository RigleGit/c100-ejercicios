# Ejercicio XXX: [NOMBRE DEL EJERCICIO]

## Descripción

[DESCRIPCIÓN DETALLADA DEL EJERCICIO]

Este ejercicio forma parte de la serie **100 Ejercicios de C** y está diseñado para enseñar y practicar los siguientes conceptos:

- ✅ [CONCEPTO 1]
- ✅ [CONCEPTO 2]
- ✅ [CONCEPTO 3]
- ✅ [CONCEPTO 4]

## Enunciado

[ENUNCIADO COMPLETO DEL EJERCICIO]

### Ejemplo de Entrada y Salida

```
Entrada: [EJEMPLO DE ENTRADA]
Salida: [EJEMPLO DE SALIDA]

Entrada: [OTRO EJEMPLO]
Salida: [OTRA SALIDA]
```

## Conceptos Clave

### [CONCEPTO PRINCIPAL 1]
[EXPLICACIÓN DEL CONCEPTO]

```c
// Ejemplo de código relevante
tipo_dato funcion_ejemplo(parametros) {
    // Implementación
    return resultado;
}
```

### [CONCEPTO PRINCIPAL 2]
[EXPLICACIÓN DEL SEGUNDO CONCEPTO]

### [CONCEPTO PRINCIPAL 3]
[EXPLICACIÓN DEL TERCER CONCEPTO]

## Estructura del Proyecto

```
xxx-nombre-ejercicio/
├── include/
│   └── nombre_ejercicio.h      # Definiciones y prototipos
├── src/
│   ├── nombre_ejercicio.c      # Implementación principal
│   └── main.c                  # Programa principal interactivo
├── tests/
│   └── test_nombre_ejercicio.c # Tests unitarios completos
├── tools/                      # Herramientas adicionales
│   ├── benchmark.c             # Pruebas de rendimiento
│   ├── demo.c                  # Demostración adicional
│   └── generador_datos.c       # Generador de datos de prueba
├── ejemplos/                   # Archivos de ejemplo
│   ├── datos_basicos.txt       # Casos básicos
│   ├── datos_avanzados.txt     # Casos complejos
│   └── casos_error.txt         # Casos de error
├── CMakeLists.txt              # Configuración CMake
├── Makefile                    # Makefile alternativo
├── README.md                   # Este archivo
└── .gitignore                  # Archivos a ignorar
```

## Compilación y Ejecución

### Método 1: Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build
cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar
./nombre_ejercicio
```

### Método 2: Usando Makefile

```bash
# Compilar
make

# Compilar en modo debug
make debug

# Ejecutar
make run
```

### Método 3: Compilación Manual

```bash
# Compilación básica
gcc -std=c99 -Wall -Wextra -Iinclude src/*.c -o nombre_ejercicio -lm

# Con optimización
gcc -std=c99 -Wall -Wextra -O3 -Iinclude src/*.c -o nombre_ejercicio -lm

# Modo debug
gcc -std=c99 -Wall -Wextra -g -DDEBUG -Iinclude src/*.c -o nombre_ejercicio -lm
```

## Uso del Programa

### Modo Interactivo

```bash
./nombre_ejercicio
```

El programa mostrará un menú interactivo con las siguientes opciones:
1. Calcular resultado con parámetros personalizados
2. Ejecutar demostración con ejemplos
3. Ejecutar casos de prueba automáticos
4. Mostrar información del ejercicio
5. Salir

### Procesamiento de Archivos

```bash
./nombre_ejercicio archivo_datos.txt
```

Formato del archivo:
```
parametro1 parametro2
parametro1 parametro2
...
```

### Opciones de Línea de Comandos

```bash
./nombre_ejercicio --help      # Mostrar ayuda
./nombre_ejercicio --demo      # Ejecutar demostración
./nombre_ejercicio --test      # Ejecutar tests automáticos
```

## Algoritmo

### Descripción del Algoritmo

[DESCRIPCIÓN PASO A PASO DEL ALGORITMO IMPLEMENTADO]

1. **Paso 1**: [DESCRIPCIÓN]
2. **Paso 2**: [DESCRIPCIÓN]
3. **Paso 3**: [DESCRIPCIÓN]
4. **Paso 4**: [DESCRIPCIÓN]

### Complejidad

- **Temporal**: O([COMPLEJIDAD TEMPORAL])
- **Espacial**: O([COMPLEJIDAD ESPACIAL])

### Pseudocódigo

```
ALGORITMO nombre_algoritmo
ENTRADA: [parámetros de entrada]
SALIDA: [valor de salida]

INICIO
    [pseudocódigo paso a paso]
FIN
```

## Testing

### Ejecutar Tests

```bash
# Usando CMake
cd build
make test

# Usando Makefile
make test

# Con salida detallada
make test-verbose

# Tests de memoria
make test-memory
```

### Tests Incluidos

- ✅ **Funcionalidad básica**: Casos típicos de uso
- ✅ **Validación de entrada**: Parámetros inválidos
- ✅ **Casos límite**: Valores extremos
- ✅ **Manejo de errores**: Situaciones de error
- ✅ **Rendimiento**: Pruebas de velocidad
- ✅ **Memoria**: Detección de memory leaks

### Cobertura de Código

```bash
# Generar reporte de cobertura
make coverage

# Ver resultados
open coverage/index.html  # En macOS
```

## Herramientas Adicionales

### Benchmark de Rendimiento

```bash
make benchmark
```

Ejecuta pruebas de rendimiento con diferentes tamaños de entrada y muestra estadísticas de tiempo de ejecución.

### Análisis Estático

```bash
make static-analysis
```

Ejecuta análisis estático del código usando `cppcheck` para detectar posibles problemas.

### Formateo de Código

```bash
make format          # Formatear código
make format-check    # Verificar formato
```

### Generador de Datos de Prueba

```bash
./tools/generador_datos 100 > datos_generados.txt
```

## Archivos de Ejemplo

### `ejemplos/datos_basicos.txt`
```
5 3.14
10 2.71
15 1.41
```

### `ejemplos/datos_avanzados.txt`
```
[EJEMPLOS MÁS COMPLEJOS]
```

### `ejemplos/casos_error.txt`
```
[CASOS QUE DEBEN GENERAR ERRORES]
```

## Manejo de Errores

El programa maneja los siguientes tipos de errores:

| Código | Descripción | Acción |
|--------|-------------|---------|
| `RESULTADO_EXITO` | Operación exitosa | Continuar |
| `RESULTADO_ERROR_PARAMETRO` | Parámetros inválidos | Mostrar error y solicitar nuevos datos |
| `RESULTADO_ERROR_MEMORIA` | Fallo en asignación de memoria | Terminar programa |
| `RESULTADO_ERROR_ENTRADA` | Error en entrada de datos | Solicitar entrada válida |
| `RESULTADO_ERROR_CALCULO` | Error en cálculo | Mostrar error específico |

## Optimizaciones

### Optimizaciones Implementadas

1. **[OPTIMIZACIÓN 1]**: [DESCRIPCIÓN]
2. **[OPTIMIZACIÓN 2]**: [DESCRIPCIÓN]
3. **[OPTIMIZACIÓN 3]**: [DESCRIPCIÓN]

### Optimizaciones Posibles

- **[OPTIMIZACIÓN FUTURA 1]**: [DESCRIPCIÓN]
- **[OPTIMIZACIÓN FUTURA 2]**: [DESCRIPCIÓN]

## Casos de Uso Educativos

### Nivel Principiante

```bash
# Ejecutar con datos simples
echo "5 3" | ./nombre_ejercicio
```

### Nivel Intermedio

```bash
# Procesar archivo de datos
./nombre_ejercicio ejemplos/datos_basicos.txt
```

### Nivel Avanzado

```bash
# Análisis de rendimiento
./tools/benchmark

# Análisis de memoria
make test-memory
```

## Extensiones Posibles

1. **[EXTENSIÓN 1]**: [DESCRIPCIÓN]
2. **[EXTENSIÓN 2]**: [DESCRIPCIÓN]
3. **[EXTENSIÓN 3]**: [DESCRIPCIÓN]

## Problemas Comunes y Soluciones

### Error: "Segmentation fault"
**Causa**: Acceso a memoria no válida
**Solución**: Verificar punteros y límites de arrays

### Error: "Division by zero"
**Causa**: División por cero
**Solución**: Validar denominadores antes de la división

### Error: "Memory leak detected"
**Causa**: Memoria no liberada
**Solución**: Asegurar que todo `malloc()` tenga su correspondiente `free()`

## Referencias

- [Referencia 1](URL) - [Descripción]
- [Referencia 2](URL) - [Descripción]
- [Estándar C99](https://www.iso.org/standard/29237.html)
- [GNU C Library](https://www.gnu.org/software/libc/)

## Información del Ejercicio

- **Categoría**: [CATEGORÍA DEL EJERCICIO]
- **Dificultad**: ⭐⭐⭐☆☆ ([NIVEL]/5)
- **Tiempo estimado**: [TIEMPO] minutos
- **Conceptos previos**: [LISTA DE CONCEPTOS REQUERIDOS]
- **Ejercicios relacionados**: [NÚMEROS DE EJERCICIOS RELACIONADOS]

## Licencia

Este ejercicio es parte del proyecto **100 Ejercicios de C** y está disponible bajo la licencia MIT.
