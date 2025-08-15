
# Devcontainer para C100 Ejercicios

Objetivo: proporcionar un devcontainer portable y reproducible para desarrollar y ejecutar los ejercicios C en Linux, macOS y Windows usando VS Code Remote - Containers.

Qué incluye esta configuración
- Compiladores y herramientas básicas (gcc, g++, cmake) dentro del contenedor.
- Extensiones recomendadas para C/C++ y testing en `devcontainer.json`.
- Ajustes para depuración con ptrace y clang-format.

Notas de portabilidad
- Este devcontainer está pensado para usarse con Docker o Podman en el host. En macOS/Windows usar Docker Desktop. En Linux puedes usar Docker Engine o Podman.
- Por compatibilidad con Docker Desktop se evita usar `--network=host` en `runArgs`.
  - Si necesitas `--network=host` en Linux, añádelo manualmente en tu comando `docker run` o en tu configuración local de `devcontainer.json`.

Cómo usar
1. Instala Docker Desktop (macOS/Windows) o Docker/Podman (Linux).
2. Abre el proyecto en VS Code.
3. Desde la paleta: "Dev Containers: Reopen in Container" y el contenedor se construirá.

Checks rápidos (host)
- macOS/Windows: instala Docker Desktop y verifica que VS Code detecte Docker.
- Linux: instala Docker Engine o Podman y comprueba `docker ps` o `podman ps`.

Problemas comunes
- "permission denied" al montar: revisa permisos del directorio de proyecto.
- `--network=host` no funciona en macOS/Windows: usa `forwardPorts` para exponer puertos en el host.

Si quieres que haga variantes separadas del `devcontainer.json` para Windows/Linux/macOS (por ejemplo, añadiendo `runArgs` específicos por SO), puedo generarlas y documentar cómo distribuir cada una.

---

## Prueba rápida del devcontainer (opción A: dejar configuración actual)

Siguientes métodos para comprobar que el devcontainer funciona correctamente.

### 1) Probar desde VS Code (recomendado)

- Abre el repositorio en VS Code.
- Abre la paleta (Cmd/Ctrl+Shift+P) y ejecuta: "Dev Containers: Reopen in Container".
- Espera a que VS Code construya la imagen y abra el contenedor. En la esquina inferior izquierda verás el icono verde indicando que estás dentro del contenedor.
- Abre un terminal integrado (Terminal → New Terminal). Desde ahí puedes ejecutar los scripts de prueba del repo.

Ejemplo dentro del contenedor (en el terminal integrado):

```bash
cd /workspace
./tools/test_everything_fresh.sh
```

Si `tools/test_everything_fresh.sh` no existe o quieres ejecutar una prueba concreta, ve a un ejercicio y corre su Makefile (ejemplo):

```bash
cd /workspace/09-estructuras-datos/066-pila-lista-enlazada
make check
# o
make test
```

### 2) Probar desde línea de comandos (sin VS Code)

- Construir la imagen desde el root del repo:

```bash
docker build -f Dockerfile -t c100-ejercicios:dev ..
```

- Ejecutar un contenedor y montar el workspace actual (Linux/macOS):

```bash
docker run --rm -it \
  -v "$(pwd):/workspace:cached" \
  -w /workspace \
  --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
  c100-ejercicios:dev /bin/bash
```

- Para habilitar host networking solo en Linux (si lo necesitas):

```bash
docker run --rm -it --network=host \
  -v "$(pwd):/workspace:cached" -w /workspace \
  --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
  c100-ejercicios:dev /bin/bash
```

Dentro del contenedor CLI puedes ejecutar los mismos comandos de prueba que en VS Code (por ejemplo `./tools/test_everything_fresh.sh`).

### 3) Verificar que Criterion está disponible

Dentro del contenedor, comprueba la versión de Criterion con pkg-config:

```bash
pkg-config --modversion criterion || echo "Criterion no detectado"
```

Si `pkg-config` o Criterion no están instalados en tu imagen, el `Dockerfile` del repo instala `libcriterion-dev` en Ubuntu; si quieres puedo adaptar la imagen para instalar una versión específica o para sistemas distintos.

## Notas y recomendaciones

- La configuración por defecto evita `--network=host` para ser segura en Docker Desktop (macOS/Windows). Si desarrollas y pruebas servicios TCP que deben responder en el host, usa la opción `--network=host` en Linux solamente o configura `forwardPorts` y `portMappings` en tu `devcontainer.json` local.
- Si prefieres que la imagen use el usuario `vscode` en lugar de `developer`, puedo actualizar `Dockerfile` y `devcontainer.json` para seguir la convención oficial de Dev Containers.

Si quieres, procedo ahora a cualquiera de estos pasos: (A) dejar la configuración actual y crear una breve checklist para compartir con alumnos; (B) migrar a usuario `vscode`; (C) construir y ejecutar la imagen aquí para verificar (necesitaría permiso para ejecutar docker). Dime cuál prefieres.
