# Ejercicio 082: Reestructurar Programa en Varios Archivos

## Descripción
Este ejercicio enseña los conceptos fundamentales de modularización en C, incluyendo la creación de bibliotecas, uso de headers con include guards, compilación separada y enlazado de objetos. Es esencial para entender cómo organizar proyectos grandes y crear código reutilizable.

## Conceptos Clave
- **Modularización**: Dividir código en módulos reutilizables
- **Include guards**: Prevenir inclusiones múltiples con `#ifndef`, `#define`, `#endif`
- **Compilación separada**: Compilar archivos .c independientemente
- **Enlazado**: Unir objetos compilados para formar el ejecutable
- **Bibliotecas estáticas**: Archivos .a que se incluyen en el ejecutable
- **Headers**: Archivos .h con declaraciones de funciones y constantes
- **API pública**: Interfaz que expone una biblioteca

## Estructura del Proyecto
```
082-reestructurar-archivos/
├── include/
│   ├── mimath.h              # Header con declaraciones y constantes
│   └── .gitkeep
├── lib/
│   └── mimath.c              # Implementación de la biblioteca
├── src/
│   └── main.c                # Programa principal
├── tests/
│   └── test_mimath.c         # Tests comprehensivos
├── CMakeLists.txt            # Configuración de build
├── README.md                 # Esta documentación
└── .gitignore                # Exclusiones de git
```

## Archivos

### 1. Header Principal (`include/mimath.h`)
Contiene:
- **Include guards** para prevenir inclusiones múltiples
- **Declaraciones de funciones** matemáticas
- **Constantes matemáticas** (PI, E, etc.)
- **Documentación** completa de la API

### 2. Biblioteca (`lib/mimath.c`)
Implementa:
- **Operaciones básicas**: suma, resta, multiplicación, división, módulo
- **Operaciones avanzadas**: potencia, raíz cuadrada, factorial, MCD, MCM
- **Validaciones**: números primos, pares, perfectos
- **Utilidades**: valor absoluto, máximo, mínimo, intercambio
- **Estadísticas**: operaciones con arrays

### 3. Programa Principal (`src/main.c`)
Demuestra:
- **Uso de la biblioteca** importada
- **Todas las funcionalidades** implementadas
- **Manejo de casos especiales** y errores
- **Presentación organizada** de resultados

## Compilación

### Método 1: CMake (Recomendado)
```bash
# Crear directorio de build
mkdir build && cd build

# Configurar proyecto
cmake ..

# Compilar todo
make

# Ejecutar programa principal
./reestructurar_archivos

# Ejecutar tests (si Criterion está instalado)
make check
```

### Método 2: Compilación Manual (Educativa)
```bash
# 1. Compilar biblioteca a objeto
gcc -c lib/mimath.c -Iinclude -o mimath.o

# 2. Compilar programa principal a objeto
gcc -c src/main.c -Iinclude -o main.o

# 3. Enlazar objetos para crear ejecutable
gcc main.o mimath.o -o programa

# 4. Ejecutar
./programa

# 5. Limpiar objetos temporales
rm *.o
```

### Método 3: Demostración Automática
```bash
# CMake incluye un target para demostrar compilación manual
cd build
make compilacion_manual
./programa_manual
```

### Compilación con Diferentes Configuraciones
```bash
# Debug (con información de depuración)
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release (optimizado)
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Con biblioteca dinámica
cmake -DBUILD_SHARED_LIBS=ON ..
make
```

## Include Guards Explicados

El header usa include guards para prevenir inclusiones múltiples:

```c
#ifndef MIMATH_H    // Si MIMATH_H no está definido
#define MIMATH_H    // Definir MIMATH_H

// ... contenido del header ...

#endif /* MIMATH_H */  // Fin de la protección
```

**¿Por qué son necesarios?**
- Previenen **errores de redefinición** cuando un header se incluye múltiples veces
- Son **esenciales** en proyectos con múltiples archivos
- **Mejoran** los tiempos de compilación
- **Estándar de la industria** para headers en C

## Funcionalidades de la Biblioteca

### Operaciones Básicas
- `suma(a, b)` - Suma dos números
- `resta(a, b)` - Resta dos números  
- `multiplicacion(a, b)` - Multiplica dos números
- `division(a, b)` - División con manejo de división por cero
- `modulo(a, b)` - Módulo con manejo de casos especiales

### Operaciones Avanzadas
- `potencia(base, exponente)` - Exponenciación
- `raiz_cuadrada(numero)` - Raíz cuadrada entera
- `factorial(n)` - Factorial con validación
- `mcd(a, b)` - Máximo común divisor (Algoritmo de Euclides)
- `mcm(a, b)` - Mínimo común múltiplo

### Funciones de Validación
- `es_primo(numero)` - Verificar si es primo
- `es_par(numero)` - Verificar si es par
- `es_perfecto(numero)` - Verificar si es número perfecto

### Utilidades
- `valor_absoluto(numero)` - Valor absoluto
- `maximo(a, b)` / `minimo(a, b)` - Extremos
- `intercambiar(&a, &b)` - Intercambio por referencia

### Estadísticas de Arrays
- `suma_array(arr, tamaño)` - Suma de elementos
- `promedio_array(arr, tamaño)` - Promedio
- `maximo_array(arr, tamaño)` - Valor máximo
- `minimo_array(arr, tamaño)` - Valor mínimo

## Tests y Validación

Los tests cubren:
- **Todas las funciones** de la biblioteca
- **Casos límite** y valores extremos
- **Manejo de errores** (división por cero, valores inválidos)
- **Casos especiales** (arrays vacíos, punteros nulos)
- **Consistencia matemática** (propiedades conmutativas, etc.)
- **Integración** entre funciones

Para ejecutar tests:
```bash
cd build
make test_mimath
./test_mimath
```

## Ventajas de la Modularización

1. **Reutilización**: La biblioteca puede usarse en múltiples proyectos
2. **Mantenimiento**: Cambios en implementación no afectan código cliente
3. **Compilación**: Solo recompilar archivos modificados
4. **Organización**: Código más limpio y estructurado
5. **Testing**: Probar módulos independientemente
6. **Colaboración**: Equipos pueden trabajar en módulos separados

## Mejores Prácticas Implementadas

1. **Include guards** en todos los headers
2. **Documentación** completa de funciones
3. **Validación** de parámetros de entrada
4. **Manejo de errores** robusto
5. **Nombres descriptivos** para funciones y variables
6. **Separación clara** entre interfaz e implementación
7. **Tests exhaustivos** para todas las funcionalidades

## Ejemplo de Salida

```
🧮 BIBLIOTECA MATEMÁTICA MIMATH
Demostración de funcionalidades
================================

=== OPERACIONES BÁSICAS ===
Números: a = 15, b = 4
Suma: 15 + 4 = 19
Resta: 15 - 4 = 11
Multiplicación: 15 * 4 = 60
División: 15 / 4 = 3
Módulo: 15 % 4 = 3

=== OPERACIONES AVANZADAS ===
Potencia: 3^4 = 81
Raíz cuadrada de 25 = 5
Factorial de 6 = 720
MCD de 48 y 18 = 6
MCM de 48 y 18 = 144

[... más demostraciones ...]

=== COMPILACIÓN MODULAR EXITOSA ===
✅ Biblioteca mimath.c compilada como módulo separado
✅ Header mimath.h protegido con include guards
✅ Enlazado exitoso de objetos separados
✅ Demostración completa de modularización en C
```

## Herramientas Adicionales

### Análisis Estático
```bash
make cppcheck  # Análisis con cppcheck
```

### Formateo de Código
```bash
make format    # Formatear con clang-format
```

### Instalación del Sistema
```bash
make install   # Instalar biblioteca y ejecutable
```

## Aplicaciones Prácticas

Este patrón de modularización es esencial en:
- **Sistemas operativos** (módulos del kernel)
- **Bibliotecas gráficas** (OpenGL, DirectX)
- **Motores de juegos** (componentes separados)
- **Aplicaciones empresariales** (módulos de negocio)
- **Bibliotecas matemáticas** (BLAS, LAPACK)

## Recursos Adicionales
- [Modular Programming in C](https://en.wikipedia.org/wiki/Modular_programming)
- [C Static Libraries](https://www.tutorialspoint.com/c_standard_library/)
- [Include Guards Best Practices](https://google.github.io/styleguide/cppguide.html#The__define_Guard)
