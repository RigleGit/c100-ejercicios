# 🆘 Soporte - C100 Ejercicios

¿Necesitas ayuda con **C100 Ejercicios**? Esta guía te llevará a la solución más rápida.

## 🚀 Solución Rápida

### 1. 🔄 Intenta Primero

```bash
# Limpia todo y vuelve a probar
./tools/clean_all.sh
./tools/test_everything_fresh.sh

# Si tienes Docker
./tools/docker_build.sh test
```

### 2. 📋 Revisa FAQ

Consulta las [**Preguntas Frecuentes**](#-preguntas-frecuentes) abajo antes de reportar.

### 3. 🔍 Busca en Issues

[Buscar issues existentes](../../issues) - probablemente alguien ya tuvo el mismo problema.

## 📞 Canales de Soporte

### 🎯 Para Diferentes Tipos de Problemas

| Problema | Canal | Tiempo de Respuesta |
|----------|-------|-------------------|
| 🐛 **Bug/Error** | [GitHub Issues](../../issues/new) | 1-3 días |
| ❓ **Pregunta General** | [GitHub Discussions](../../discussions) | 1-2 días |
| 🚀 **Feature Request** | [GitHub Issues](../../issues/new) | 1 semana |
| 🎓 **Uso Educativo** | [GitHub Discussions](../../discussions) | 2-3 días |
| 🐳 **Docker/DevContainer** | [GitHub Issues](../../issues/new) | 1-3 días |

### 📋 Información Requerida

Para **cualquier** consulta de soporte, incluye:

```bash
# Información del sistema
echo "OS: $(uname -a)"
echo "GCC: $(gcc --version | head -n1)"
echo "CMake: $(cmake --version | head -n1)"
echo "Criterion: $(pkg-config --exists criterion && echo 'OK' || echo 'NO')"

# Debug del proyecto
./tools/test_everything_fresh.sh > support_debug.log 2>&1
```

Adjunta el archivo `support_debug.log` a tu consulta.

## ❓ Preguntas Frecuentes

### 🔧 Problemas de Compilación

<details>
<summary><strong>Error: "gcc: command not found"</strong></summary>

**Solución:**
```bash
# Ubuntu/Debian
sudo apt update && sudo apt install build-essential

# macOS
xcode-select --install

# O usa Docker
./tools/docker_build.sh run
```
</details>

<details>
<summary><strong>Error: "CMake not found"</strong></summary>

**Solución:**
```bash
# Ubuntu/Debian
sudo apt install cmake

# macOS
brew install cmake

# O usa Docker
./tools/docker_build.sh test
```
</details>

<details>
<summary><strong>Error: "criterion not found"</strong></summary>

**Solución:**
```bash
# Ubuntu/Debian
sudo apt install libcriterion-dev

# macOS
brew install criterion

# Verificar
pkg-config --exists criterion && echo "OK"

# Alternativa: Docker
./tools/docker_build.sh test
```
</details>

### 🧪 Problemas de Testing

<details>
<summary><strong>Tests fallan pero compila bien</strong></summary>

**Diagnóstico:**
```bash
# Test específico con debug
cd XX-bloque/XXX-ejercicio
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --verbose --output-on-failure
```

**Posibles causas:**
- Tests muy estrictos
- Diferencias en librerías
- Problemas de precisión float
</details>

<details>
<summary><strong>"timeout: command not found"</strong></summary>

**Solución:**
```bash
# Usa scripts sin timeout
./tools/quick_test.sh [BLOQUE]

# En lugar de test_all.sh (obsoleto)
./tools/test_everything_fresh.sh
```
</details>

### 🐳 Problemas Docker

<details>
<summary><strong>Docker no está instalado</strong></summary>

**Instalación:**
```bash
# Ubuntu/Debian
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
sudo usermod -aG docker $USER
# Reiniciar sesión

# macOS
brew install --cask docker

# Windows
# Instalar Docker Desktop desde docker.com
```
</details>

<details>
<summary><strong>Error: "permission denied" con Docker</strong></summary>

**Solución:**
```bash
# Agregar usuario al grupo docker
sudo usermod -aG docker $USER

# Reiniciar sesión o reiniciar
logout
# o
sudo reboot
```
</details>

<details>
<summary><strong>DevContainer no funciona en VSCode</strong></summary>

**Solución:**
1. Instalar extensión "Dev Containers"
2. `Ctrl+Shift+P` → "Dev Containers: Rebuild Container"
3. Si persiste: `Ctrl+Shift+P` → "Dev Containers: Rebuild Without Cache"
</details>

### 💻 Problemas por SO

<details>
<summary><strong>macOS: Error con OpenMP o threading</strong></summary>

**Solución:**
```bash
# Instalar libomp
brew install libomp

# O usar Docker para ejercicios avanzados
./tools/docker_build.sh test-block 12
```
</details>

<details>
<summary><strong>Windows: WSL vs Native</strong></summary>

**Recomendación:**
- ✅ **Usar WSL2** para compatibilidad completa
- ✅ **Usar Docker Desktop** en Windows
- ❌ **Evitar** compilación nativa Windows (complejo)

```bash
# En WSL2
sudo apt update && sudo apt install build-essential cmake
```
</details>

<details>
<summary><strong>Alpine Linux: Problemas con glibc</strong></summary>

**Solución:**
```bash
# Usar imagen Ubuntu en Docker en lugar de Alpine
# O instalar glibc compatibility
apk add gcompat
```
</details>

## 🎓 Soporte Educativo

### Para Profesores

- **📚 Uso en cursos**: Totalmente libre, sin restricciones
- **🔧 Adaptaciones**: Fork y modifica según necesites
- **📊 Evaluación**: Scripts automáticos disponibles
- **🎯 Progresión**: Sugerencias de secuencia incluidas

**Contacto para partnerships educativos**: Abre un [GitHub Discussion](../../discussions) con el tag `education`.

### Para Estudiantes

- **❓ Dudas conceptuales**: [GitHub Discussions](../../discussions)
- **🐛 Problemas técnicos**: [GitHub Issues](../../issues)
- **💡 Mejoras**: [Pull Requests](../../pulls) bienvenidos
- **🤝 Colaboración**: ¡Contribuye al proyecto!

## 🔧 Autodiagnóstico

### Script de Diagnóstico

Ejecuta este script para autodiagnóstico completo:

```bash
#!/bin/bash
echo "🔍 C100 EJERCICIOS - DIAGNÓSTICO AUTOMÁTICO"
echo "=========================================="
echo ""

echo "📍 UBICACIÓN:"
echo "PWD: $(pwd)"
echo "Git repo: $(git remote -v 2>/dev/null | head -n1 || echo 'No git repo')"
echo ""

echo "💻 SISTEMA:"
echo "OS: $(uname -a)"
echo "Shell: $SHELL"
echo ""

echo "🔧 HERRAMIENTAS:"
echo "GCC: $(gcc --version 2>/dev/null | head -n1 || echo 'NO INSTALADO')"
echo "Clang: $(clang --version 2>/dev/null | head -n1 || echo 'NO INSTALADO')"
echo "CMake: $(cmake --version 2>/dev/null | head -n1 || echo 'NO INSTALADO')"
echo "Make: $(make --version 2>/dev/null | head -n1 || echo 'NO INSTALADO')"
echo "Pkg-config: $(pkg-config --version 2>/dev/null || echo 'NO INSTALADO')"
echo ""

echo "📦 LIBRERÍAS:"
echo "Criterion: $(pkg-config --exists criterion && echo 'OK' || echo 'NO DISPONIBLE')"
echo "Math lib: $(ldconfig -p 2>/dev/null | grep libm | head -n1 || echo 'Verificar manualmente')"
echo ""

echo "🐳 DOCKER:"
echo "Docker: $(docker --version 2>/dev/null || echo 'NO INSTALADO')"
echo "Docker running: $(docker info >/dev/null 2>&1 && echo 'SÍ' || echo 'NO')"
echo ""

echo "📁 ESTRUCTURA PROYECTO:"
echo "tools/ exists: $([ -d tools ] && echo 'SÍ' || echo 'NO')"
echo "Dockerfile exists: $([ -f Dockerfile ] && echo 'SÍ' || echo 'NO')"
echo "test_everything_fresh.sh: $([ -f tools/test_everything_fresh.sh ] && echo 'SÍ' || echo 'NO')"
echo ""

echo "🧪 TEST RÁPIDO:"
if [ -f tools/quick_test.sh ]; then
    echo "Ejecutando test del bloque 1..."
    ./tools/quick_test.sh 1 2>&1 | tail -n 5
else
    echo "Script de test no encontrado"
fi

echo ""
echo "✅ Diagnóstico completado"
echo "📋 Copia esta información si necesitas soporte"
```

Guarda esto como `diagnostico.sh`, hazlo ejecutable y ejecútalo:

```bash
chmod +x diagnostico.sh
./diagnostico.sh > mi_diagnostico.txt
```

## 📈 Escalación de Soporte

### Nivel 1: Self-Service
1. Revisar FAQ
2. Ejecutar diagnóstico
3. Probar con Docker

### Nivel 2: Community Support  
1. Buscar en issues existentes
2. Hacer pregunta en Discussions
3. Incluir información de diagnóstico

### Nivel 3: Direct Support
1. Crear issue detallado
2. Incluir reproductor mínimo
3. Adjuntar logs completos

## 🕒 Tiempos de Respuesta

| Prioridad | Descripción | Tiempo |
|-----------|-------------|---------|
| 🔴 **Alta** | Proyecto no funciona | 24-48h |
| 🟡 **Media** | Ejercicio específico falla | 2-3 días |
| 🟢 **Baja** | Mejoras y features | 1-2 semanas |

## 💡 Consejos Pro

### Para Desarrollo
- **Usa Docker** para evitar problemas de entorno
- **Empieza con bloques básicos** (1-3) antes de avanzados
- **Copia estructura** de ejercicios existentes
- **Prueba frecuentemente** con `quick_test.sh`

### Para Teaching
- **DevContainer** para estudiantes con diferentes SOs
- **Scripts automáticos** para evaluación masiva
- **Docker** para demos en clase
- **GitHub Codespaces** para acceso remoto

### Para Contributing
- **Lee CONTRIBUTING.md** primero
- **Usa template de PR** completo
- **Prueba en múltiples entornos** antes de PR
- **Documenta bien** los cambios

---

## 🙋 ¿No Encuentras Solución?

Si después de revisar esta guía aún tienes problemas:

1. **📝 Crea un issue** con [template completo](../../issues/new)
2. **📎 Adjunta** output de diagnóstico
3. **🔍 Describe** pasos exactos para reproducir
4. **⏰ Ten paciencia** - respondemos tan pronto como podemos

---

## 🤝 Comunidad

Recuerda que **C100 Ejercicios** es un proyecto comunitario. ¡Tu participación hace la diferencia!

- **🌟 Star** el proyecto si te ayuda
- **🐛 Reporta** bugs que encuentres  
- **💡 Sugiere** mejoras
- **🤝 Ayuda** a otros en Discussions

---

*¡Gracias por usar C100 Ejercicios! 🚀*
