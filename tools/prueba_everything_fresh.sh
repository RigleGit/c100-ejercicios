#!/bin/bash

# Script maestro para probar todo el proyecto desde cero
set -e

echo "🚀 INICIANDO PRUEBA COMPLETA DESDE CERO"
echo "======================================"
echo ""

# Obtener directorio del script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

cd "$PROJECT_DIR"

echo "📍 Directorio del proyecto: $PROJECT_DIR"
echo ""

# Paso 1: Limpieza completa
echo "🧹 PASO 1: Limpieza completa del proyecto"
echo "========================================="
if [ -f "tools/clean_all.sh" ]; then
    ./tools/clean_all.sh
else
    echo "❌ Error: No se encontró tools/clean_all.sh"
    exit 1
fi
echo ""

# Paso 2: Verificar herramientas necesarias
echo "🔧 PASO 2: Verificando herramientas necesarias"
echo "=============================================="

check_tool() {
    local tool="$1"
    local name="$2"
    if command -v "$tool" >/dev/null 2>&1; then
        echo "  ✅ $name: $(command -v "$tool")"
        return 0
    else
        echo "  ❌ $name: No encontrado"
        return 1
    fi
}

tools_ok=true
check_tool "cmake" "CMake" || tools_ok=false
check_tool "make" "Make" || tools_ok=false
check_tool "cc" "Compilador C" || tools_ok=false
check_tool "pkg-config" "pkg-config" || tools_ok=false

if command -v criterion >/dev/null 2>&1 || pkg-config --exists criterion 2>/dev/null; then
    echo "  ✅ Criterion: Disponible"
else
    echo "  ⚠️  Criterion: No detectado (algunos tests pueden usar fallbacks)"
fi

if [ "$tools_ok" = false ]; then
    echo ""
    echo "❌ Error: Faltan herramientas necesarias"
    exit 1
fi
echo ""

# Paso 3: Probar cada bloque
echo "🧪 PASO 3: Probando todos los bloques"
echo "===================================="

if [ ! -f "tools/quick_test.sh" ]; then
    echo "❌ Error: No se encontró tools/quick_test.sh"
    exit 1
fi

total_blocks=0
successful_blocks=0
failed_blocks=()

for block_num in {1..15}; do
    block_dir=""
    case $block_num in
        1) block_dir="01-fundamentos-c" ;;
        2) block_dir="02-control-de-flujo" ;;
        3) block_dir="03-funciones-modularidad" ;;
        4) block_dir="04-operaciones-nivel-bit" ;;
        5) block_dir="05-vectores-cadenas-caracteres" ;;
        6) block_dir="06-punteros" ;;
        7) block_dir="07-memoria-dinamica" ;;
        8) block_dir="08-struct-union-enums" ;;
        9) block_dir="09-estructuras-datos" ;;
        10) block_dir="10-archivos-manejo-ficheros" ;;
        11) block_dir="11-compilacion-preprocesador" ;;
        12) block_dir="12-concurrencia" ;;
        13) block_dir="13-redes" ;;
        14) block_dir="14-seguridad-buenas-practicas" ;;
        15) block_dir="15-proyectos-finales" ;;
    esac

    if [ -d "$block_dir" ]; then
        echo ""
        echo "📦 Probando Bloque $block_num ($block_dir)..."
        echo "----------------------------------------"

        total_blocks=$((total_blocks + 1))

        if ./tools/quick_test.sh $block_num; then
            echo "✅ Bloque $block_num: EXITOSO"
            successful_blocks=$((successful_blocks + 1))
        else
            echo "❌ Bloque $block_num: FALLÓ"
            failed_blocks+=("$block_num ($block_dir)")
        fi
    else
        echo "⏭️  Bloque $block_num: No existe ($block_dir)"
    fi
done

echo ""
echo "📊 RESUMEN FINAL"
echo "==============="
echo "Total de bloques probados: $total_blocks"
echo "Bloques exitosos: $successful_blocks"
echo "Bloques fallidos: $((total_blocks - successful_blocks))"

if [ ${#failed_blocks[@]} -eq 0 ]; then
    echo ""
    echo "🎉 ¡ÉXITO TOTAL! Todos los bloques funcionan correctamente"
    echo "✅ El proyecto está 100% funcional"
    echo ""
    echo "🚀 Proyecto listo para uso en producción/educación"
    exit 0
else
    echo ""
    echo "❌ BLOQUES CON PROBLEMAS:"
    for failed in "${failed_blocks[@]}"; do
        echo "   - $failed"
    done
    echo ""
    echo "⚠️  Verificar los bloques fallidos antes de usar en producción"
    exit 1
fi



