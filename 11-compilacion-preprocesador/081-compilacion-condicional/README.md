# Ejercicio 081: Compilación Condicional

## Descripción
Este ejercicio demuestra el uso de directivas de preprocesador para compilación condicional en C, incluyendo `#ifdef`, `#ifndef`, `#if`, `#else`, `#elif`, y `#endif`. Aprenderás cómo crear código que se comporta diferente dependiendo de las macros definidas durante la compilación.

## Conceptos Clave
- **Compilación condicional**: Incluir o excluir código basado en condiciones de preprocesador
- **Directivas de preprocesador**: `#ifdef`, `#ifndef`, `#if`, `#else`, `#elif`, `#endif`
- **Macros de depuración**: `DEBUG`, `VERBOSE`, `RELEASE`
- **Configuraciones de compilación**: Debug vs Release
- **Características específicas de plataforma**: `_WIN32`, `__linux__`, `__APPLE__`

## Archivos
- **`include/compilacion_condicional.h`**: Header con macros condicionales
- **`src/compilacion_condicional.c`**: Implementación con diferentes configuraciones
- **`tests/test_compilacion_condicional.c`**: Tests comprehensivos

## Compilación

### Modo Debug (con información de depuración)
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./test_compilacion_condicional
```

### Modo Release (optimizado)
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./test_compilacion_condicional
```

### Con macro DEBUG personalizada
```bash
mkdir build && cd build
cmake -DDEBUG=1 ..
make
./test_compilacion_condicional
```

### Con modo verboso
```bash
mkdir build && cd build
cmake -DVERBOSE=1 ..
make
./test_compilacion_condicional
```

### Con características experimentales
```bash
mkdir build && cd build
cmake -DEXPERIMENTAL=1 ..
make
./test_compilacion_condicional
```

## Explicación del Código

### Directivas Básicas
```c
#ifdef DEBUG
    // Código solo incluido si DEBUG está definido
    printf("Modo debug activado\n");
#endif

#ifndef RELEASE
    // Código incluido si RELEASE NO está definido
    printf("No es modo release\n");
#endif
```

### Directivas con Condiciones
```c
#if defined(DEBUG) && defined(VERBOSE)
    // Código para debug verboso
#elif defined(DEBUG)
    // Código solo para debug
#else
    // Código para release
#endif
```

### Detección de Plataforma
```c
#ifdef _WIN32
    // Código específico para Windows
#elif defined(__linux__)
    // Código específico para Linux
#elif defined(__APPLE__)
    // Código específico para macOS
#endif
```

## Casos de Uso Prácticos

1. **Depuración**: Incluir prints de debug solo en compilación de desarrollo
2. **Optimización**: Diferentes algoritmos para debug vs release
3. **Características**: Habilitar/deshabilitar funcionalidades experimentales
4. **Plataforma**: Código específico para diferentes sistemas operativos
5. **Configuración**: Diferentes configuraciones de logging

## Advertencias y Mejores Prácticas

1. **Evitar lógica compleja**: No uses demasiadas condiciones anidadas
2. **Documentar macros**: Siempre documenta qué hacen las macros condicionales
3. **Testing**: Testa todas las combinaciones de compilación posibles
4. **Consistencia**: Mantén consistencia en nombres de macros
5. **Performance**: Ten cuidado con el impacto en rendimiento del código de debug

## Ejemplos de Salida

### Modo Debug
```
=== MODO DEBUG ACTIVADO ===
Función procesando: 42
DEBUG: Validando entrada...
DEBUG: Procesando valor 42
DEBUG: Validación completada
Resultado: 1764
```

### Modo Release
```
Resultado: 1764
```

### Modo Verboso
```
=== MODO DEBUG ACTIVADO ===
=== MODO VERBOSO ACTIVADO ===
Función procesando: 42
DEBUG: Validando entrada...
VERBOSE: Entrada válida, procediendo...
DEBUG: Procesando valor 42
VERBOSE: Calculando cuadrado...
VERBOSE: 42 * 42 = 1764
DEBUG: Validación completada
Resultado: 1764
```

## Recursos Adicionales
- [Directivas de Preprocesador en C](https://en.cppreference.com/w/c/preprocessor)
- [CMake Build Types](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html)
- [Compilación Condicional](https://gcc.gnu.org/onlinedocs/cpp/Conditional-Syntax.html)
