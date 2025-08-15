# 🚀 C100 Ejercicios - Programación en C Completa

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](.) [![Tests](https://img.shields.io/badge/tests-100%25-success)](.) [![Docker](https://img.shields.io/badge/docker-ready-blue)](.) [![License](https://img.shields.io/badge/license-MIT-green)](.)

Un conjunto completo de **100 ejercicios de programación en C** organizados por dificultad y tema, con sistema de testing automatizado, entorno Docker y documentación exhaustiva.

## 📖 Sobre el Libro

Este repositorio es un **extra del libro "El Lenguaje C en 100 ejercicios explicados"**. Si este código te ha sido de ayuda y quieres apoyar el proyecto o acceder al contenido completo explicado paso a paso, puedes encontrar el libro aquí:

**📘 [El Lenguaje C en 100 ejercicios explicados](https://www.amazon.com/Rodrigo-Iglesias-ebook/dp/B0CRVJKX5H)**

¡Tu apoyo ayuda a mantener este proyecto actualizado y crear más recursos educativos! 🙏

## 📋 Tabla de Contenidos

- [📖 Sobre el Libro](#-sobre-el-libro)
- [🎯 Descripción](#-descripción)
- [📚 Estructura del Proyecto](#-estructura-del-proyecto)
- [🚀 Inicio Rápido](#-inicio-rápido)
- [🐳 Entorno Docker](#-entorno-docker)
- [🔧 Instalación Manual](#-instalación-manual)
- [🧪 Testing](#-testing)
- [📖 Bloques de Ejercicios](#-bloques-de-ejercicios)
- [🛠️ Herramientas](#-herramientas)
- [🎓 Para Educadores](#-para-educadores)
- [🤝 Contribución](#-contribución)
- [📄 Licencia](#-licencia)

## 🎯 Descripción

**C100 Ejercicios** es una colección educativa completa diseñada para aprender programación en C desde conceptos básicos hasta temas avanzados. Cada ejercicio incluye:

- ✅ **Código fuente completo** con comentarios explicativos
- ✅ **Tests automatizados** con Criterion
- ✅ **Documentación detallada** de cada concepto
- ✅ **Sistema de build** con CMake
- ✅ **Entorno containerizado** con Docker
- ✅ **Herramientas de análisis** y debugging

### 🎯 Objetivos Educativos

- **Fundamentos sólidos** en programación C
- **Buenas prácticas** de desarrollo
- **Testing y debugging** profesional
- **Gestión de memoria** correcta
- **Programación de sistemas** (redes, concurrencia)
- **Estructuras de datos** clásicas

## 📚 Estructura del Proyecto

```
c100-ejercicios/
├── 📁 01-fundamentos-c/           # Conceptos básicos de C
├── 📁 02-control-de-flujo/         # Condicionales y bucles
├── 📁 03-funciones-modularidad/    # Funciones y módulos
├── 📁 04-operaciones-nivel-bit/    # Manipulación de bits
├── 📁 05-vectores-cadenas-caracteres/ # Arrays y strings
├── 📁 06-punteros/                 # Punteros y referencias
├── 📁 07-memoria-dinamica/         # malloc, realloc, free
├── 📁 08-struct-union-enums/       # Estructuras de datos
├── 📁 09-estructuras-datos/        # Listas, árboles, algoritmos
├── 📁 10-archivos-manejo-ficheros/ # I/O de archivos
├── 📁 11-compilacion-preprocesador/ # Preprocesador y build
├── 📁 12-concurrencia/            # Threads y sincronización
├── 📁 13-redes/                   # Programación de sockets
├── 📁 14-seguridad-buenas-practicas/ # Código seguro
├── 📁 15-proyectos-finales/       # Proyectos integradores
├── 🛠️ tools/                      # Scripts de automatización
├── 🐳 Dockerfile                  # Entorno containerizado
├── 📖 DOCKER_README.md            # Guía de Docker
└── 📋 README.md                   # Esta documentación
```

### 📁 Estructura de Cada Ejercicio

```
001-hola-mundo/
├── 📄 README.md          # Descripción y objetivos
├── 🔧 CMakeLists.txt     # Configuración de build
├── 📁 src/              # Código fuente
│   ├── main.c           # Programa principal
│   └── *.c              # Módulos adicionales
├── 📁 include/          # Headers (si los hay)
├── 📁 tests/            # Tests automatizados
└── 📁 examples/         # Ejemplos adicionales
```

## 🚀 Inicio Rápido

### Opción 1: 🐳 Docker (Recomendado)

La forma más rápida de empezar es usando Docker:

```bash
# Clonar el repositorio
git clone <repo-url>
cd c100-ejercicios

# Probar todo el proyecto
./tools/docker_build.sh test

# Desarrollo interactivo
./tools/docker_build.sh run
```

### Opción 2: 🔧 DevContainer (VSCode/Cursor)

Si usas VSCode o Cursor:

1. Instala la extensión "Dev Containers"
2. Abre el proyecto
3. `Ctrl+Shift+P` → "Dev Containers: Reopen in Container"
4. ¡El entorno se configura automáticamente!

### Opción 3: 💻 Instalación Local

```bash
# Instalar dependencias (Ubuntu/Debian)
sudo apt update
sudo apt install build-essential cmake libcriterion-dev

# Clonar y probar
git clone <repo-url>
cd c100-ejercicios
./tools/prueba_everything_fresh.sh
```

## 🐳 Entorno Docker

El proyecto incluye múltiples opciones de Docker para diferentes necesidades:

### 🔧 Para Desarrollo

```bash
# DevContainer (integración IDE)
# Abrir en VSCode/Cursor → "Reopen in Container"

# O contenedor manual
./tools/docker_build.sh run
cd 01-fundamentos-c/001-hola-mundo
cmake -S . -B build && cmake --build build
./build/main
```

### 🧪 Para Testing

```bash
# Todos los tests
./tools/docker_build.sh test

# Bloque específico
./tools/docker_build.sh test-block 5

# Con servicios adicionales
./tools/docker_build.sh compose-up
docker exec -it c100-ejercicios-dev /bin/bash
```

### 🏭 Para CI/CD

```bash
# Pipeline de integración continua
docker build -t c100-test .
docker run --rm -v $(pwd):/workspace c100-test \
  bash -c "./tools/prueba_everything_fresh.sh"
```

📖 **Más detalles en [DOCKER_README.md](DOCKER_README.md)**

## 🔧 Instalación Manual

### Requisitos del Sistema

- **SO**: Linux (Ubuntu 20.04+) / macOS (10.15+) / Windows (WSL2)
- **Compilador**: GCC 7+ o Clang 8+
- **Build**: CMake 3.10+, Make
- **Testing**: Criterion 2.3+ (opcional pero recomendado)

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  pkg-config \
  libcriterion-dev \
  valgrind \
  gdb
```

### macOS

```bash
# Con Homebrew
brew install cmake criterion pkg-config

# Con MacPorts
sudo port install cmake criterion pkgconfig
```

### Verificación

```bash
# Verificar herramientas
gcc --version
cmake --version
pkg-config --exists criterion && echo "✅ Criterion OK"

# Probar proyecto
./tools/prueba_everything_fresh.sh
```

## 🧪 Testing

El proyecto incluye un sistema de testing robusto y multicapa:

### 🎯 Testing Completo

```bash
# Método 1: Script maestro (recomendado)
./tools/prueba_everything_fresh.sh

# Método 2: Docker
./tools/docker_build.sh test

# Método 3: Por bloques
for i in {1..15}; do
  ./tools/quick_test.sh $i
done
```

### 🔍 Testing Específico

```bash
# Un bloque específico
./tools/quick_test.sh 5

# Un ejercicio específico
cd 01-fundamentos-c/001-hola-mundo
cmake -S . -B build
cmake --build build
ctest --test-dir build --verbose
```

### 📊 Tipos de Tests

#### ✅ **Tests Unitarios**
- Validan lógica individual de cada función
- Ejecutan sin dependencias externas
- Ideales para desarrollo y CI/CD

#### ✅ **Tests de Integración**
- Prueban interacción entre módulos
- Incluyen I/O de archivos y red
- Algunos requieren servicios externos

#### ✅ **Tests de Memoria**
- Verifican gestión correcta de memoria
- Detectan leaks con Valgrind
- Validan bounds checking

#### ✅ **Tests de Rendimiento**
- Miden tiempo de ejecución
- Comparan algoritmos
- Detectan regresiones

### 🛠️ Herramientas de Testing

| Herramienta | Propósito | Comando |
|-------------|-----------|---------|
| **Criterion** | Framework de testing | `./build/test_*` |
| **CTest** | Ejecutor de tests | `ctest --test-dir build` |
| **Valgrind** | Análisis de memoria | `valgrind ./build/main` |
| **GDB** | Debugging | `gdb ./build/main` |
| **cppcheck** | Análisis estático | `cppcheck src/` |

## 📖 Bloques de Ejercicios

### 🌱 Nivel Principiante

#### **Bloque 1-3: Fundamentos (Ejercicios 1-25)**

<details>
<summary><strong>01 - Fundamentos de C</strong> (10 ejercicios)</summary>

- **001**: Hola Mundo - Primer programa
- **002**: Suma de dos números - Variables y operadores
- **003**: Conversión de temperatura - Fórmulas matemáticas
- **004**: Área de círculo - Constantes y π
- **005**: Formato de tiempo - Divisiones y módulo
- **006**: Código ASCII - Caracteres y números
- **007**: Distancia euclidiana - Funciones matemáticas
- **008**: Promedio de tres números - Entrada/salida
- **009**: Intercambio de variables - Algoritmos básicos
- **010**: Volumen de esfera - Matemáticas 3D

**Conceptos**: Variables, tipos de datos, operadores, E/S básica
</details>

<details>
<summary><strong>02 - Control de Flujo</strong> (10 ejercicios)</summary>

- **011**: Par o impar - Condicionales simples
- **012**: Máximo de dos números - if-else
- **013**: Clasificación numérica - if-else-if
- **014**: Mayor de tres números - Condiciones anidadas
- **015**: Calculadora básica - switch-case
- **016**: Factorial - Bucles while
- **017**: Suma de dígitos - Manipulación numérica
- **018**: Adivina el número - do-while y rand()
- **019**: Triángulo de asteriscos - Bucles anidados
- **020**: Números primos hasta N - Algoritmos matemáticos

**Conceptos**: if/else, switch, while, do-while, for, bucles anidados
</details>

<details>
<summary><strong>03 - Funciones y Modularidad</strong> (5 ejercicios)</summary>

- **021**: Función máximo de dos - Definición de funciones
- **022**: Factorial recursivo - Recursión
- **023**: Función es primo - Parámetros y retorno
- **024**: Función MCD - Algoritmo de Euclides
- **025**: Función potencia - Matemáticas iterativas

**Conceptos**: Funciones, parámetros, retorno, recursión, modularidad
</details>

### 🔥 Nivel Intermedio

#### **Bloque 4-9: Datos y Algoritmos (Ejercicios 26-72)**

<details>
<summary><strong>04 - Operaciones a Nivel de Bit</strong> (5 ejercicios)</summary>

- **026**: Paridad de bits - Operadores bit a bit
- **027**: Contar bits a 1 - Manipulación binaria
- **028**: Potencia de 2 con bits - Optimizaciones
- **029**: Intercambio XOR - Trucos bit a bit
- **030**: Empaquetar bytes - Estructuras de bits

**Conceptos**: AND, OR, XOR, NOT, shifts, máscaras de bits
</details>

<details>
<summary><strong>05 - Vectores y Cadenas</strong> (12 ejercicios)</summary>

- **031-035**: Operaciones con arrays - Invertir, buscar, ordenar
- **036-037**: Matrices 2D - Suma y multiplicación
- **038-042**: Manipulación de strings - Longitud, búsqueda, comparación

**Conceptos**: Arrays, matrices, strings, algoritmos de ordenación y búsqueda
</details>

<details>
<summary><strong>06 - Punteros</strong> (8 ejercicios)</summary>

- **043**: Introducción a punteros - Conceptos básicos
- **044**: Swap con punteros - Paso por referencia
- **045**: Recorrido con punteros - Aritmética de punteros
- **046**: Copia de strings - Punteros y memoria
- **047**: Puntero a función - Callbacks
- **048**: Búsqueda con punteros - Algoritmos
- **049**: Inspección de bytes - Representación interna
- **050**: Puntero void - Punteros genéricos

**Conceptos**: Punteros, referencias, aritmética, punteros a función, void*
</details>

<details>
<summary><strong>07 - Memoria Dinámica</strong> (6 ejercicios)</summary>

- **051**: Entero dinámico - malloc/free básico
- **052**: Array dinámico - Gestión de memoria
- **053**: Redimensionar array - realloc
- **054**: Concatenar strings - Memoria dinámica
- **055**: Matriz 2D dinámica - Punteros dobles
- **056**: Array de strings - Memoria compleja

**Conceptos**: malloc, calloc, realloc, free, gestión de memoria
</details>

<details>
<summary><strong>08 - Estructuras de Datos</strong> (6 ejercicios)</summary>

- **057**: Estructura persona - struct básico
- **058**: Array de estructuras - Datos complejos
- **059**: Union - Reinterpretación de datos
- **060**: Enumeraciones - Constantes nombradas
- **061**: Estructura dinámica - struct + malloc
- **062**: Campos de bits - Optimización de memoria

**Conceptos**: struct, union, enum, campos de bits, tipos de datos personalizados
</details>

<details>
<summary><strong>09 - Estructuras de Datos Avanzadas</strong> (10 ejercicios)</summary>

- **063-065**: Listas enlazadas - Inserción, eliminación, inversión
- **066-067**: Pilas y colas - Estructuras LIFO/FIFO
- **068-069**: Árboles binarios - BST, altura, recorridos
- **070**: Lista doblemente enlazada - Navegación bidireccional
- **071**: Quicksort - Algoritmo de ordenación
- **072**: Búsqueda binaria - Algoritmo de búsqueda

**Conceptos**: Listas, pilas, colas, árboles, algoritmos de ordenación y búsqueda
</details>

### 🚀 Nivel Avanzado

#### **Bloque 10-15: Sistemas y Proyectos (Ejercicios 73-100)**

<details>
<summary><strong>10 - Archivos y E/S</strong> (6 ejercicios)</summary>

- **073-076**: Manejo de archivos de texto
- **077-078**: Archivos binarios y registros

**Conceptos**: fopen, fread, fwrite, fseek, ftell, archivos de texto y binarios
</details>

<details>
<summary><strong>11 - Compilación y Preprocesador</strong> (5 ejercicios)</summary>

- **079-083**: Macros, compilación condicional, assert

**Conceptos**: #define, #ifdef, #include, compilación modular
</details>

<details>
<summary><strong>12 - Concurrencia</strong> (5 ejercicios)</summary>

- **084-088**: Threads, mutex, condiciones, productor-consumidor

**Conceptos**: pthread, sincronización, race conditions, semáforos
</details>

<details>
<summary><strong>13 - Redes</strong> (5 ejercicios)</summary>

- **089-093**: Sockets TCP/UDP, cliente/servidor

**Conceptos**: sockets, TCP, UDP, cliente-servidor, HTTP
</details>

<details>
<summary><strong>14 - Seguridad</strong> (5 ejercicios)</summary>

- **094-098**: Entrada segura, buffer overflow, const correctness

**Conceptos**: Seguridad, validación, buffer overflow, buenas prácticas
</details>

<details>
<summary><strong>15 - Proyectos Finales</strong> (2 ejercicios)</summary>

- **099**: Agenda de contactos - Proyecto integrador
- **100**: Evaluador RPN - Calculadora postfija

**Conceptos**: Integración de conceptos, proyectos reales
</details>

## 🛠️ Herramientas

El proyecto incluye un conjunto completo de herramientas de automatización:

### 📋 Scripts Principales

| Script | Propósito | Uso |
|--------|-----------|-----|
| `prueba_everything_fresh.sh` | Testing completo desde cero | `./tools/prueba_everything_fresh.sh` |
| `quick_test.sh` | Testing rápido por bloques | `./tools/quick_test.sh 5` |
| `clean_all.sh` | Limpieza completa | `./tools/clean_all.sh` |
| `docker_build.sh` | Gestión de Docker | `./tools/docker_build.sh run` |

### 🐳 Scripts Docker

| Script | Propósito | Uso |
|--------|-----------|-----|
| `docker_build.sh` | Gestión completa de Docker | `./tools/docker_build.sh test` |
| `docker_test_runner.sh` | Testing optimizado en contenedor | `./tools/docker_test_runner.sh interactive` |

### 🔧 Utilidades

- **Compilación automática** con detección de errores
- **Testing paralelo** para velocidad
- **Limpieza inteligente** de archivos generados
- **Reportes detallados** de resultados
- **Compatibilidad multiplataforma**

📖 **Documentación completa en [tools/README.md](tools/README.md)**

## 🎓 Para Educadores

### 📚 Uso en Aulas

```bash
# Configurar para estudiantes
git clone <repo-url>
cd c100-ejercicios

# Verificar entorno
./tools/docker_build.sh check

# Asignar ejercicios específicos
./tools/quick_test.sh 1  # Fundamentos
./tools/quick_test.sh 5  # Arrays y strings
```

### 📊 Evaluación Automática

```bash
# Evaluar todos los ejercicios
./tools/docker_build.sh test > evaluacion_estudiante.txt

# Evaluar bloque específico
./tools/docker_build.sh test-block 12 > concurrencia_eval.txt

# Análisis de código
./tools/docker_build.sh compose-up
# Reportes en volumen analyzer-reports
```

### 🎯 Progresión Sugerida

#### **Semana 1-2: Fundamentos** (Bloques 1-2)
- Sintaxis básica de C
- Control de flujo
- Entrada/salida

#### **Semana 3-4: Estructuras** (Bloques 3-4)
- Funciones y modularidad
- Operaciones de bits

#### **Semana 5-8: Datos** (Bloques 5-7)
- Arrays y strings
- Punteros
- Memoria dinámica

#### **Semana 9-12: Algoritmos** (Bloques 8-9)
- Estructuras de datos
- Algoritmos clásicos

#### **Semana 13-15: Sistemas** (Bloques 10-12)
- Archivos
- Preprocesador
- Concurrencia

#### **Semana 16: Proyectos** (Bloques 13-15)
- Redes
- Seguridad
- Proyectos finales

## 🤝 Contribución

¡Las contribuciones son bienvenidas! Este proyecto está diseñado para crecer y mejorar.

### 🐛 Reportar Issues

```bash
# Antes de reportar, ejecuta:
./tools/prueba_everything_fresh.sh > debug_output.txt

# Incluye en tu reporte:
# - Sistema operativo
# - Versión de compilador
# - Output completo de debug_output.txt
# - Pasos para reproducir
```

### ✨ Agregar Ejercicios

1. **Fork** el repositorio
2. **Crear** nuevo ejercicio siguiendo la estructura:
   ```bash
   mkdir XX-categoria/XXX-nombre-ejercicio
   # Seguir estructura estándar
   ```
3. **Probar** completamente:
   ```bash
   ./tools/quick_test.sh XX
   ```
4. **Crear PR** con descripción detallada

### 🔧 Mejorar Herramientas

- Scripts de automatización
- Configuraciones Docker
- Documentación
- Tests adicionales

### 📖 Estilo de Código

- **Estilo**: LLVM/Google C Style
- **Formato**: clang-format automático
- **Comentarios**: Doxygen style
- **Tests**: Criterion framework

## 🏆 Estado del Proyecto

### ✅ Completamente Funcional

- **📊 Testing**: 100% de ejercicios con tests automatizados
- **🐳 Docker**: Entorno completamente containerizado
- **🔧 Build**: CMake en todos los ejercicios
- **📚 Docs**: Documentación exhaustiva
- **🌐 Compatibilidad**: Linux, macOS, Windows (WSL)

### 📈 Estadísticas

| Métrica | Valor |
|---------|-------|
| **Ejercicios** | 100 |
| **Bloques** | 15 |
| **Tests automatizados** | ~300 |
| **Líneas de código** | ~50,000 |
| **Documentación** | ~200 páginas |

### 🎯 Roadmap

- [ ] **Más ejercicios avanzados** (GPU, paralelismo)
- [ ] **Interfaz web** para testing online
- [ ] **Integración CI/CD** con GitHub Actions
- [ ] **Métricas de calidad** automáticas
- [ ] **Traducción** a otros idiomas

## 📄 Licencia

Este proyecto está licenciado bajo la **MIT License** - ver [LICENSE](LICENSE) para detalles.

### 🎓 Uso Educativo

- ✅ **Libre uso** en aulas y cursos
- ✅ **Modificación** y adaptación permitida
- ✅ **Distribución** sin restricciones
- ✅ **Uso comercial** en contextos educativos

---

## 🌟 Reconocimientos

- **Criterion** - Framework de testing
- **CMake** - Sistema de build
- **Docker** - Containerización
- **Comunidad C** - Inspiración y feedback

---

## 📞 Soporte

### 💬 Canales de Ayuda

- **GitHub Issues**: Para bugs y features
- **GitHub Discussions**: Para preguntas generales
- **Docker Hub**: Para problemas de contenedores

### 🚨 FAQ

<details>
<summary><strong>❓ No compila en mi sistema</strong></summary>

```bash
# Prueba con Docker:
./tools/docker_build.sh test

# Si persiste, reporta issue con:
gcc --version
cmake --version
./tools/prueba_everything_fresh.sh > error.log 2>&1
```
</details>

<details>
<summary><strong>❓ Tests fallan</strong></summary>

```bash
# Limpia y vuelve a probar:
./tools/clean_all.sh
./tools/prueba_everything_fresh.sh

# Para un ejercicio específico:
cd XX-bloque/XXX-ejercicio
rm -rf build
cmake -S . -B build && cmake --build build
ctest --test-dir build --verbose
```
</details>

<details>
<summary><strong>❓ Docker no funciona</strong></summary>

```bash
# Verifica Docker:
docker --version
docker info

# Construye imagen:
./tools/docker_build.sh build

# Si falla, usa DevContainer en VSCode/Cursor
```
</details>

---

<div align="center">

## 🎉 ¡Disfruta programando en C!

**[⬆️ Volver al inicio](#-c100-ejercicios---programación-en-c-completa)**

---

*Hecho con ❤️ para la comunidad de programadores C*

</div>
