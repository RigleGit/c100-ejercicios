# 🤝 Guía de Contribución - C100 Ejercicios

¡Gracias por tu interés en contribuir al proyecto **C100 Ejercicios**! Este documento te guiará a través del proceso de contribución.

## 📋 Tabla de Contenidos

- [🎯 Tipos de Contribución](#-tipos-de-contribución)
- [🚀 Configuración Inicial](#-configuración-inicial)
- [📝 Estándares de Código](#-estándares-de-código)
- [🧪 Testing](#-testing)
- [📚 Documentación](#-documentación)
- [🔄 Proceso de PR](#-proceso-de-pr)
- [🐛 Reportar Issues](#-reportar-issues)
- [🎓 Contribuciones Educativas](#-contribuciones-educativas)

## 🎯 Tipos de Contribución

### ✨ Nuevos Ejercicios
- Agregar ejercicios a bloques existentes
- Crear nuevos bloques temáticos
- Ejercicios de dificultad progresiva

### 🐛 Corrección de Bugs
- Errores de compilación
- Tests fallidos
- Problemas de compatibilidad

### 📚 Documentación
- Mejorar README.md
- Documentar ejercicios individuales
- Guías de uso y ejemplos

### 🛠️ Herramientas
- Scripts de automatización
- Configuraciones Docker
- Mejoras de testing

### 🔧 Infraestructura
- Configuraciones CI/CD
- Optimizaciones de build
- Compatibilidad multiplataforma

## 🚀 Configuración Inicial

### 1. Fork y Clone

```bash
# Fork en GitHub, luego:
git clone https://github.com/TU_USERNAME/c100-ejercicios.git
cd c100-ejercicios
```

### 2. Configurar Entorno

#### Opción A: Docker (Recomendado)
```bash
# Verificar que todo funciona
./tools/docker_build.sh test
```

#### Opción B: Local
```bash
# Instalar dependencias
sudo apt install build-essential cmake libcriterion-dev

# Verificar entorno
./tools/test_everything_fresh.sh
```

### 3. Crear Branch
```bash
git checkout -b feature/nuevo-ejercicio-nombre
# o
git checkout -b fix/problema-descripcion
```

## 📝 Estándares de Código

### 🎨 Estilo de Código

#### Formato C
```c
// Usar LLVM/Google style
// Configuración en .clang-format (si existe)

// Ejemplo de función bien formateada:
/**
 * @brief Calcula el factorial de un número
 * @param n Número entero positivo
 * @return Factorial de n, o -1 si error
 */
int calcular_factorial(int n) {
    if (n < 0) {
        return -1;  // Error: número negativo
    }
    
    if (n == 0 || n == 1) {
        return 1;
    }
    
    return n * calcular_factorial(n - 1);
}
```

#### Naming Conventions
- **Variables**: `snake_case`
- **Funciones**: `snake_case`
- **Constantes**: `UPPER_SNAKE_CASE`
- **Tipos**: `snake_case_t`

```c
// ✅ Bien
int numero_estudiantes;
void calcular_promedio(void);
const int MAX_INTENTOS = 3;
typedef struct persona_s persona_t;

// ❌ Mal
int numEstudiantes;
void CalcularPromedio(void);
const int maxIntentos = 3;
```

### 📁 Estructura de Ejercicios

```
XXX-nombre-ejercicio/
├── README.md              # Descripción y objetivos
├── CMakeLists.txt         # Configuración de build
├── src/
│   ├── main.c            # Programa principal
│   └── nombre_ejercicio.c # Lógica (si es complejo)
├── include/              # Headers (si es necesario)
│   └── nombre_ejercicio.h
├── tests/
│   └── test_nombre.c     # Tests con Criterion
└── examples/             # Ejemplos adicionales (opcional)
    └── ejemplo.c
```

### 📖 Documentación de Ejercicios

#### README.md Template
```markdown
# XXX - Nombre del Ejercicio

## 🎯 Objetivo
Breve descripción de qué debe aprender el estudiante.

## 📚 Conceptos
- Concepto 1
- Concepto 2

## 🔧 Implementación
Explicación de la implementación.

## 🧪 Tests
Descripción de qué prueban los tests.

## 💡 Hints
Pistas para resolver el ejercicio (opcional).

## 🔗 Referencias
- Enlaces útiles
- Documentación relevante
```

## 🧪 Testing

### Tests Obligatorios

Cada ejercicio **DEBE** incluir:

1. **Test de compilación** - Verifica que compila sin errores
2. **Tests unitarios** - Valida funcionalidad básica
3. **Test de parámetros inválidos** - Manejo de errores
4. **Test de memoria** - Sin leaks (si usa malloc)

### Ejemplo de Test Criterion

```c
#include <criterion/criterion.h>
#include "../include/nombre_ejercicio.h"

TestSuite(nombre_ejercicio, .description = "Tests para XXX");

Test(nombre_ejercicio, test_caso_basico) {
    // Arrange
    int entrada = 5;
    int esperado = 120;
    
    // Act
    int resultado = calcular_factorial(entrada);
    
    // Assert
    cr_assert_eq(resultado, esperado, 
                 "factorial(5) debe ser 120, pero fue %d", resultado);
}

Test(nombre_ejercicio, test_parametro_invalido) {
    int resultado = calcular_factorial(-1);
    cr_assert_eq(resultado, -1, "factorial(-1) debe retornar -1");
}
```

### Ejecutar Tests

```bash
# Test específico
cd XXX-ejercicio
cmake -S . -B build && cmake --build build
ctest --test-dir build --verbose

# Todos los tests
./tools/test_everything_fresh.sh

# Bloque específico
./tools/quick_test.sh X
```

## 📚 Documentación

### Comentarios en Código

```c
/**
 * @file main.c
 * @brief Programa principal del ejercicio XXX
 * @description Explicación detallada de qué hace el programa
 * @version 1.0
 * @date 2024
 * @author Contributor Name
 */

#include <stdio.h>

/**
 * @brief Función principal del programa
 * @description Demuestra el uso de [concepto específico]
 * @return 0 si éxito, código de error si falla
 */
int main(void) {
    // Comentario explicativo de la lógica
    printf("Hola, mundo!\n");
    return 0;
}
```

### CMakeLists.txt Template

```cmake
cmake_minimum_required(VERSION 3.10)
project(XXX-nombre-ejercicio)
set(CMAKE_C_STANDARD 99)

# Configuración básica
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Ejecutable principal
add_executable(main src/main.c)

# Tests con Criterion
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_nombre tests/test_nombre.c)
    target_link_libraries(test_nombre ${CRITERION_LIBRARIES})
    target_include_directories(test_nombre PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_nombre PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_nombre PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    add_test(NAME nombre_tests COMMAND test_nombre)
endif()
```

## 🔄 Proceso de PR

### 1. Preparación

```bash
# Actualizar desde upstream
git fetch upstream
git rebase upstream/main

# Verificar que todo funciona
./tools/test_everything_fresh.sh
```

### 2. Commits

#### Formato de Commit Messages
```
tipo(alcance): descripción breve

Descripción más detallada si es necesario.

- Cambio específico 1
- Cambio específico 2

Fixes #123
```

#### Tipos de Commit
- `feat`: Nueva funcionalidad
- `fix`: Corrección de bug
- `docs`: Documentación
- `test`: Tests
- `refactor`: Refactoring
- `chore`: Tareas de mantenimiento

#### Ejemplos
```bash
feat(ejercicio): agregar 101-algoritmos-ordenacion

- Implementa bubble sort, quick sort, merge sort
- Incluye tests comparativos de performance
- Documentación con complejidad algorítmica

Fixes #45

---

fix(docker): corregir problema con Criterion en Alpine

- Actualizar Dockerfile para usar libcriterion-dev
- Agregar pkg-config al contenedor
- Verificar compatibilidad con arm64

Fixes #78
```

### 3. Testing Previo al PR

```bash
# Testing completo
./tools/test_everything_fresh.sh

# Docker testing
./tools/docker_build.sh test

# Verificar estilo (si disponible)
clang-format -i src/*.c

# Verificar memory leaks (opcional)
valgrind --leak-check=full ./build/main
```

### 4. Crear PR

1. Push a tu fork
2. Crear PR en GitHub
3. Completar template de PR
4. Esperar review

## 🐛 Reportar Issues

### Información Requerida

Incluye siempre:

```bash
# Información del sistema
uname -a
gcc --version
cmake --version
pkg-config --exists criterion && echo "Criterion OK"

# Output de debug
./tools/test_everything_fresh.sh > debug.log 2>&1
```

### Template de Issue

Usa el [template de issue](.github/ISSUE_TEMPLATE.md) que incluye:

- Tipo de problema
- Pasos para reproducir
- Entorno de desarrollo
- Output de debug
- Comportamiento esperado vs actual

## 🎓 Contribuciones Educativas

### Nuevos Ejercicios

Al crear un nuevo ejercicio, considera:

#### Valor Educativo
- **Concepto objetivo**: ¿Qué específicamente debe aprender?
- **Dificultad apropiada**: ¿Está en el nivel correcto del bloque?
- **Progresión**: ¿Se basa en ejercicios anteriores?
- **Aplicación práctica**: ¿Tiene uso en el mundo real?

#### Estructura Pedagógica
```markdown
1. **Motivación**: ¿Por qué es importante este concepto?
2. **Explicación**: Conceptos teóricos necesarios
3. **Ejemplo**: Implementación simple
4. **Ejercicio**: Tarea para el estudiante
5. **Extensiones**: Ideas para profundizar
```

#### Ejercicios de Calidad
- **Instrucciones claras** y precisas
- **Ejemplos de entrada/salida**
- **Tests exhaustivos** pero no abrumadores
- **Hints graduales** para diferentes niveles
- **Referencias** a documentación oficial

### Secuencia de Ejercicios

Al agregar a un bloque existente:

1. **Revisar progresión** actual del bloque
2. **Identificar gaps** en conceptos
3. **Proponer secuencia** lógica
4. **Validar con community** (via issue/discussion)

### Niveles de Dificultad

- **🌱 Básico**: Sintaxis y conceptos fundamentales
- **🔥 Intermedio**: Algoritmos y estructuras de datos
- **🚀 Avanzado**: Sistemas, optimización, proyectos

## 🏆 Reconocimiento

### Contributors

Todos los contributors aparecen en:
- README.md principal
- CONTRIBUTORS.md (si se crea)
- Release notes

### Tipos de Reconocimiento

- **🥇 Core Contributor**: >10 PRs significativos
- **🏅 Exercise Creator**: Nuevos ejercicios de calidad
- **🛠️ Tools Developer**: Herramientas y automatización
- **📚 Documentation**: Mejoras de documentación
- **🐛 Bug Hunter**: Reportes de calidad y fixes

## 💬 Comunicación

### Canales

- **GitHub Issues**: Bugs y feature requests
- **GitHub Discussions**: Preguntas y ideas
- **PR Comments**: Review y feedback técnico

### Etiquetas

Use etiquetas apropiadas:
- `good first issue`: Para newcomers
- `help wanted`: Necesita colaboración
- `documentation`: Relacionado con docs
- `enhancement`: Mejoras
- `bug`: Errores
- `educational`: Valor educativo específico

---

## 🎉 ¡Gracias por Contribuir!

Tu contribución hace que **C100 Ejercicios** sea mejor para toda la comunidad de estudiantes y educadores de C.

**¿Preguntas?** 
- Abre un issue con la etiqueta `question`
- Inicia una discussion en GitHub

---

*¡Feliz coding! 🚀*
