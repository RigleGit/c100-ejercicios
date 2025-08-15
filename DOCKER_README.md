# 🐳 Entorno Docker para C100 Ejercicios

Este proyecto incluye un entorno Docker completo para desarrollo y testing de ejercicios C, garantizando consistencia y reproducibilidad en cualquier sistema.

## 🚀 Opciones de Uso

### 1. 📦 Docker Simple (Recomendado para testing)

```bash
# Construir y ejecutar
./tools/docker_build.sh run

# Solo testing
./tools/docker_build.sh test

# Testing de un bloque específico
./tools/docker_build.sh test-block 5
```

### 2. 🔧 DevContainer (Recomendado para desarrollo)

Si usas **VSCode** o **Cursor**:

1. Instala la extensión "Dev Containers"
2. Abre el proyecto
3. `Ctrl+Shift+P` → "Dev Containers: Reopen in Container"
4. ¡Listo! El entorno se configura automáticamente

### 3. 🌐 Docker Compose (Entorno completo con servicios)

```bash
# Levantar todos los servicios
./tools/docker_build.sh compose-up

# Conectar al contenedor principal
docker exec -it c100-ejercicios-dev /bin/bash

# Detener servicios
./tools/docker_build.sh compose-down
```

## 📋 Servicios Incluidos

### 🛠️ Contenedor Principal (`c100-dev`)
- **Propósito**: Desarrollo y testing principal
- **Herramientas**: GCC, CMake, Criterion, Valgrind, GDB
- **Puertos**: 8080-8089, 9999
- **Volumen**: Código fuente montado en `/workspace`

### 🌐 Servidor TCP de Prueba (`test-tcp-server`)
- **Propósito**: Servidor para ejercicios de red
- **Puerto**: 8090
- **Función**: Responde automáticamente a conexiones TCP

### 🔍 Analizador de Código (`code-analyzer`)
- **Propósito**: Análisis estático automático
- **Herramientas**: cppcheck, clang-format
- **Reportes**: Guardados en volumen persistente

## 🎯 Casos de Uso

### Para Estudiantes
```bash
# Entorno rápido para probar ejercicios
./tools/docker_build.sh test

# Desarrollo interactivo
./tools/docker_build.sh run
```

### Para Profesores/Evaluadores
```bash
# Evaluación automática completa
./tools/docker_build.sh test > resultados.txt

# Análisis de código
./tools/docker_build.sh compose-up
# Los reportes aparecen en el volumen analyzer-reports
```

### Para CI/CD
```bash
# En tu pipeline
docker build -t c100-test .
docker run --rm -v $(pwd):/workspace c100-test \
  bash -c "cd /workspace && ./tools/test_everything_fresh.sh"
```

## 🔧 Configuración Detallada

### Variables de Entorno
```bash
CC=gcc                    # Compilador C
CXX=g++                   # Compilador C++
CMAKE_BUILD_TYPE=Debug    # Tipo de build
DEBIAN_FRONTEND=noninteractive
```

### Herramientas Incluidas
- **Compiladores**: GCC, Clang
- **Build Systems**: CMake, Make
- **Testing**: Criterion
- **Debugging**: GDB, Valgrind
- **Análisis**: cppcheck, clang-format
- **Red**: netcat, telnet
- **Utilidades**: git, vim, nano, tree

### Capacidades Especiales
- `SYS_PTRACE`: Para debugging con GDB
- `seccomp=unconfined`: Para debugging avanzado
- Network host: Para ejercicios de red

## 📁 Estructura del Entorno

```
/workspace/               # Tu código (montado desde host)
├── tools/               # Scripts de automatización
│   ├── docker_build.sh     # Gestión de Docker
│   ├── docker_test_runner.sh  # Testing en contenedor
│   ├── test_everything_fresh.sh  # Suite completa
│   └── quick_test.sh       # Tests por bloque
├── build/               # Cache de compilación (volumen)
└── reports/             # Reportes de análisis (volumen)
```

## 🎮 Scripts Disponibles

### En el Host (fuera del contenedor)

#### `./tools/docker_build.sh`
```bash
build          # Construir imagen
run            # Ejecutar contenedor interactivo
test           # Ejecutar todos los tests
test-block N   # Ejecutar tests del bloque N
clean          # Limpiar contenedores
compose-up     # Levantar con docker-compose
compose-down   # Detener docker-compose
```

### En el Contenedor (dentro del contenedor)

#### `./tools/docker_test_runner.sh`
```bash
banner         # Mostrar información del sistema
check          # Verificar herramientas
clean          # Limpiar entorno
test-all       # Ejecutar todos los tests
test-block N   # Ejecutar tests del bloque N
report         # Generar reporte completo
interactive    # Modo interactivo
```

## 🔍 Ejemplos de Uso

### Desarrollo Diario
```bash
# Método 1: DevContainer (recomendado)
# Abrir en VSCode/Cursor y usar "Reopen in Container"

# Método 2: Docker manual
./tools/docker_build.sh run
# Dentro del contenedor:
./tools/quick_test.sh 5
make -C 05-vectores-cadenas-caracteres/031-invertir-array/build
```

### Testing Automatizado
```bash
# Test completo
./tools/docker_build.sh test

# Test específico
./tools/docker_build.sh test-block 12

# Con docker-compose (servicios adicionales)
./tools/docker_build.sh compose-up
docker exec -it c100-ejercicios-dev ./tools/docker_test_runner.sh report
```

### Debugging
```bash
./tools/docker_build.sh run
# Dentro del contenedor:
cd 06-punteros/043-introduccion-punteros
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
gdb build/main
```

### Análisis de Código
```bash
./tools/docker_build.sh compose-up
# El servicio code-analyzer genera reportes automáticamente
docker exec -it c100-analyzer cat /reports/cppcheck.xml
```

## 🚨 Troubleshooting

### Problema: "Docker no encontrado"
```bash
# Instalar Docker
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
```

### Problema: "Permisos denegados"
```bash
# Agregar usuario al grupo docker
sudo usermod -aG docker $USER
# Reiniciar sesión
```

### Problema: "Puerto ya en uso"
```bash
# Cambiar puertos en docker-compose.yml
ports:
  - "8180-8189:8080-8089"  # Usar rango diferente
```

### Problema: "Volumen lleno"
```bash
# Limpiar volúmenes
./tools/docker_build.sh clean
docker system prune -f
```

## 🎯 Ventajas del Entorno Docker

### ✅ **Consistencia**
- Mismo entorno en todos los sistemas
- Versiones específicas de herramientas
- Configuración reproducible

### ✅ **Aislamiento**
- No afecta el sistema host
- Fácil limpieza y reset
- Sin conflictos de dependencias

### ✅ **Portabilidad**
- Funciona en Windows, macOS, Linux
- Fácil distribución a estudiantes
- Compatible con CI/CD

### ✅ **Funcionalidad Completa**
- Todos los 100 ejercicios funcionan
- Testing automatizado
- Debugging y análisis incluidos

## 📚 Recursos Adicionales

- [Documentación Docker](https://docs.docker.com/)
- [Dev Containers](https://containers.dev/)
- [Docker Compose](https://docs.docker.com/compose/)

---

## 🎉 ¡Listo para usar!

Ahora tienes un entorno Docker completo y profesional para desarrollar y probar todos los ejercicios C. ¡Disfruta programando! 🚀
