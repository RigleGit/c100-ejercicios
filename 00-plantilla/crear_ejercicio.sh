#!/bin/bash

# Script para crear un nuevo ejercicio basado en la plantilla
# Uso: ./crear_ejercicio.sh categoria numero nombre descripcion

set -e  # Salir en caso de error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para mostrar ayuda
mostrar_ayuda() {
    echo -e "${BLUE}=== GENERADOR DE EJERCICIOS ===${NC}"
    echo "Uso: $0 <categoria> <numero> <nombre> <descripcion>"
    echo ""
    echo "Parámetros:"
    echo "  categoria    : Nombre de la categoría (ej: fundamentos-c)"
    echo "  numero       : Número del ejercicio con formato XXX (ej: 011)"
    echo "  nombre       : Nombre del ejercicio (ej: area-circulo)"
    echo "  descripcion  : Descripción breve entre comillas"
    echo ""
    echo "Ejemplo:"
    echo "  $0 fundamentos-c 011 area-circulo \"Cálculo del área de un círculo\""
    echo ""
    echo "Estructura creada:"
    echo "  XX-categoria/XXX-nombre/include/nombre.h"
    echo "  XX-categoria/XXX-nombre/src/nombre.c"
    echo "  XX-categoria/XXX-nombre/src/main.c"
    echo "  XX-categoria/XXX-nombre/tests/test_nombre.c"
    echo "  XX-categoria/XXX-nombre/README.md"
    echo "  XX-categoria/XXX-nombre/CMakeLists.txt"
    echo "  XX-categoria/XXX-nombre/Makefile"
    echo "  + directorios tools/, ejemplos/, etc."
}

# Función para logging
log() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
}

# Verificar argumentos
if [ $# -ne 4 ]; then
    error "Número incorrecto de argumentos"
    echo ""
    mostrar_ayuda
    exit 1
fi

# Asignar parámetros
CATEGORIA="$1"
NUMERO="$2"
NOMBRE="$3"
DESCRIPCION="$4"

# Validar formato del número
if ! [[ "$NUMERO" =~ ^[0-9]{3}$ ]]; then
    error "El número debe tener formato XXX (3 dígitos)"
    exit 1
fi

# Validar que no contenga caracteres especiales
if ! [[ "$NOMBRE" =~ ^[a-z0-9-]+$ ]]; then
    error "El nombre solo debe contener letras minúsculas, números y guiones"
    exit 1
fi

# Calcular paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLANTILLA_DIR="$SCRIPT_DIR"
CATEGORIA_DIR="../$CATEGORIA"
EJERCICIO_DIR="$CATEGORIA_DIR/$NUMERO-$NOMBRE"

# Verificar que existe la plantilla
if [ ! -d "$PLANTILLA_DIR" ]; then
    error "No se encontró el directorio de plantilla: $PLANTILLA_DIR"
    exit 1
fi

# Verificar si ya existe el ejercicio
if [ -d "$EJERCICIO_DIR" ]; then
    error "El ejercicio ya existe: $EJERCICIO_DIR"
    exit 1
fi

# Crear directorio de categoría si no existe
if [ ! -d "$CATEGORIA_DIR" ]; then
    log "Creando directorio de categoría: $CATEGORIA_DIR"
    mkdir -p "$CATEGORIA_DIR"
fi

log "Creando nuevo ejercicio: $EJERCICIO_DIR"

# Copiar plantilla
log "Copiando plantilla..."
cp -r "$PLANTILLA_DIR" "$EJERCICIO_DIR"

# Eliminar archivos de instrucciones de la copia
rm -f "$EJERCICIO_DIR/INSTRUCCIONES.md"
rm -f "$EJERCICIO_DIR/crear_ejercicio.sh"

cd "$EJERCICIO_DIR"

# Generar nombres derivados
NOMBRE_UNDERSCORE=$(echo "$NOMBRE" | tr '-' '_')
NOMBRE_MAYUS=$(echo "$NOMBRE_UNDERSCORE" | tr 'a-z' 'A-Z')
NOMBRE_CAMEL=$(echo "$NOMBRE" | sed 's/-\([a-z]\)/\U\1/g' | sed 's/^./\U&/')
NOMBRE_FUNCION="${NOMBRE_UNDERSCORE}"

log "Renombrando archivos..."

# Renombrar archivos
mv "include/nombre_ejercicio.h" "include/${NOMBRE_UNDERSCORE}.h"
mv "src/nombre_ejercicio.c" "src/${NOMBRE_UNDERSCORE}.c"
mv "tests/test_nombre_ejercicio.c" "tests/test_${NOMBRE_UNDERSCORE}.c"

log "Reemplazando marcadores en archivos..."

# Función para reemplazar en archivos
reemplazar_en_archivos() {
    local buscar="$1"
    local reemplazar="$2"
    
    find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.md" -o -name "CMakeLists.txt" -o -name "Makefile" \) \
        -exec sed -i '' "s|${buscar}|${reemplazar}|g" {} \;
}

# Reemplazos principales
reemplazar_en_archivos "nombre_ejercicio" "$NOMBRE_UNDERSCORE"
reemplazar_en_archivos "NOMBRE_EJERCICIO_H" "${NOMBRE_MAYUS}_H"
reemplazar_en_archivos "\[NOMBRE_PROYECTO\]" "$NOMBRE_CAMEL"
reemplazar_en_archivos "\[NOMBRE DEL EJERCICIO\]" "$DESCRIPCION"
reemplazar_en_archivos "\[DESCRIPCIÓN BREVE\]" "$DESCRIPCION"
reemplazar_en_archivos "\[FECHA\]" "$(date '+%d de %B de %Y')"
reemplazar_en_archivos "\[CATEGORÍA\]" "$(echo $CATEGORIA | tr '-' ' ' | sed 's/\b\w/\u&/g')"
reemplazar_en_archivos "Ejercicio XXX:" "Ejercicio $NUMERO:"
reemplazar_en_archivos "xxx-nombre-ejercicio" "$NUMERO-$NOMBRE"

# Reemplazos específicos para README
sed -i '' "s|# Ejercicio XXX:|# Ejercicio $NUMERO:|g" README.md

# Función para solicitar información adicional
solicitar_info() {
    echo ""
    echo -e "${BLUE}=== CONFIGURACIÓN ADICIONAL ===${NC}"
    echo "Puedes personalizar estos valores ahora o editarlos después:"
    echo ""
    
    read -p "Nivel de dificultad (1-5) [3]: " DIFICULTAD
    DIFICULTAD=${DIFICULTAD:-3}
    
    read -p "Tiempo estimado en minutos [30]: " TIEMPO
    TIEMPO=${TIEMPO:-30}
    
    read -p "Complejidad temporal (ej: n, n^2, log n) [1]: " COMP_TEMPORAL
    COMP_TEMPORAL=${COMP_TEMPORAL:-1}
    
    read -p "Complejidad espacial (ej: 1, n, log n) [1]: " COMP_ESPACIAL
    COMP_ESPACIAL=${COMP_ESPACIAL:-1}
    
    # Aplicar configuración adicional
    reemplazar_en_archivos "\[TIEMPO\]" "$TIEMPO"
    reemplazar_en_archivos "\[COMPLEJIDAD TEMPORAL\]" "$COMP_TEMPORAL"
    reemplazar_en_archivos "\[COMPLEJIDAD ESPACIAL\]" "$COMP_ESPACIAL"
    
    # Generar estrellas para dificultad
    ESTRELLAS=""
    for i in $(seq 1 5); do
        if [ $i -le $DIFICULTAD ]; then
            ESTRELLAS="${ESTRELLAS}⭐"
        else
            ESTRELLAS="${ESTRELLAS}☆"
        fi
    done
    
    reemplazar_en_archivos "⭐⭐⭐☆☆ (\[NIVEL\]/5)" "$ESTRELLAS ($DIFICULTAD/5)"
}

# Solicitar información adicional (opcional)
echo ""
read -p "¿Deseas configurar información adicional ahora? (y/N): " CONFIG_ADICIONAL

if [[ $CONFIG_ADICIONAL =~ ^[Yy]$ ]]; then
    solicitar_info
fi

# Crear archivo de ejemplo básico
log "Creando archivos de ejemplo..."

cat > "ejemplos/datos_basicos.txt" << EOF
# Datos de ejemplo para $DESCRIPCION
# Formato: [describir formato aquí]
5 3.14
10 2.71
15 1.41
EOF

cat > "ejemplos/datos_avanzados.txt" << EOF
# Casos más complejos para $DESCRIPCION
# [Agregar casos específicos del ejercicio]
100 1.414
1000 2.718
10000 3.141
EOF

cat > "ejemplos/casos_error.txt" << EOF
# Casos que deben generar errores
# [Agregar casos de error específicos]
-1 0
0 -1
abc def
EOF

# Verificar compilación
log "Verificando que el código compile..."

if command -v gcc &> /dev/null; then
    if gcc -std=c99 -Wall -Wextra -Iinclude src/*.c -o test_compile -lm 2>/dev/null; then
        log "Compilación exitosa ✓"
        rm -f test_compile
    else
        warning "Hay errores de compilación. Revisa el código generado."
    fi
else
    warning "gcc no encontrado. No se pudo verificar compilación."
fi

# Mostrar resumen
echo ""
echo -e "${GREEN}=== EJERCICIO CREADO EXITOSAMENTE ===${NC}"
echo "Ubicación: $EJERCICIO_DIR"
echo ""
echo "Archivos generados:"
echo "  📁 include/${NOMBRE_UNDERSCORE}.h"
echo "  📁 src/${NOMBRE_UNDERSCORE}.c"
echo "  📁 src/main.c"
echo "  📁 tests/test_${NOMBRE_UNDERSCORE}.c"
echo "  📄 README.md"
echo "  ⚙️  CMakeLists.txt"
echo "  ⚙️  Makefile"
echo "  📂 ejemplos/ (con archivos de datos)"
echo "  📂 tools/ (directorio preparado)"
echo ""
echo "Próximos pasos:"
echo "1. cd $EJERCICIO_DIR"
echo "2. Editar src/${NOMBRE_UNDERSCORE}.c - Implementar lógica específica"
echo "3. Editar tests/test_${NOMBRE_UNDERSCORE}.c - Agregar tests específicos"
echo "4. Actualizar README.md con detalles del ejercicio"
echo "5. make - Compilar y probar"
echo ""
echo "Comandos útiles:"
echo "  make           # Compilar"
echo "  make test      # Ejecutar tests"
echo "  make demo      # Ejecutar demostración"
echo "  make help      # Ver todas las opciones"
echo ""
echo -e "${BLUE}¡Listo para programar! 🚀${NC}"
