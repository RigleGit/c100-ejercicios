# Dockerfile para C100 Ejercicios - Entorno completo de desarrollo C
FROM ubuntu:22.04

# Evitar prompts interactivos durante la instalación
ENV DEBIAN_FRONTEND=noninteractive

# Instalar herramientas esenciales de desarrollo
RUN apt-get update && apt-get install -y \
    # Compiladores y herramientas de build
    build-essential \
    gcc \
    clang \
    cmake \
    make \
    pkg-config \
    \
    # Librerías de desarrollo
    libc6-dev \
    libpthread-stubs0-dev \
    \
    # Herramientas de debugging y análisis
    gdb \
    valgrind \
    cppcheck \
    clang-format \
    \
    # Pruebas CLI con la biblioteca estándar de Python
    python3 \
    \
    # Herramientas de sistema y utilidades
    git \
    vim \
    nano \
    curl \
    wget \
    tree \
    htop \
    \
    # Herramientas de red para ejercicios de redes
    netcat-openbsd \
    telnet \
    \
    # (libm se incluye con glibc; no se requiere paquete separado)
    \
    # Limpiar cache de apt
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Crear usuario no-root para desarrollo
RUN useradd -m -s /bin/bash developer && \
    echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Configurar entorno de trabajo
WORKDIR /workspace

# Copiar herramientas del proyecto
COPY tools/ /workspace/tools/
RUN chmod +x /workspace/tools/*.sh

# Crear script de entrada para el contenedor
COPY <<EOF /entrypoint.sh
#!/bin/bash
set -e

echo "🐳 Entorno Docker C100-Ejercicios iniciado"
echo "================================================"
echo "📁 Directorio de trabajo: /workspace"
echo "🔧 Herramientas disponibles en: /workspace/tools/"
echo "📋 Comandos útiles:"
echo "  - tools/test_everything_fresh.sh  # Probar todos los ejercicios"
echo "  - tools/quick_test.sh [BLOQUE]    # Probar bloque específico"
echo "  - tools/clean_all.sh              # Limpiar archivos compilados"
echo "================================================"
echo ""

# Ejecutar el comando como el usuario actual (ya configurado con USER developer)
exec "\$@"
EOF

RUN chmod +x /entrypoint.sh

# Configurar variables de entorno
ENV CC=gcc
ENV CXX=g++
ENV CMAKE_BUILD_TYPE=Debug

# Puerto por defecto para ejercicios de red (se puede cambiar)
EXPOSE 8080-8090

# Usuario por defecto
USER developer

# Punto de entrada
ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash"]

# Labels para metadatos
LABEL org.opencontainers.image.title="C100 Ejercicios"
LABEL org.opencontainers.image.description="Entorno completo para desarrollo y testing de ejercicios C"
LABEL org.opencontainers.image.version="1.0"
LABEL org.opencontainers.image.authors="C100 Ejercicios Project"
