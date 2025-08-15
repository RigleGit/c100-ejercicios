#!/bin/bash
# docker_test_runner.sh - Script optimizado para testing dentro del contenedor Docker

set -e

# Variables de entorno para el contenedor
export TERM=xterm-256color
export CC=gcc
export CXX=g++

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log_info() {
    echo -e "${BLUE}[DOCKER]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Banner de inicio
show_banner() {
    echo "════════════════════════════════════════════════════════════"
    echo "🐳 C100 EJERCICIOS - ENTORNO DOCKER"
    echo "════════════════════════════════════════════════════════════"
    echo "📍 Directorio: $(pwd)"
    echo "🔧 Compilador: $(gcc --version | head -n1)"
    echo "📦 CMake: $(cmake --version | head -n1)"
    echo "🧪 Criterion: $(pkg-config --modversion criterion 2>/dev/null || echo 'Disponible')"
    echo "════════════════════════════════════════════════════════════"
    echo ""
}

# Verificar herramientas esenciales
check_tools() {
    log_info "Verificando herramientas esenciales..."
    
    local tools=("gcc" "cmake" "make" "pkg-config")
    local missing=()
    
    for tool in "${tools[@]}"; do
        if ! command -v "$tool" >/dev/null 2>&1; then
            missing+=("$tool")
        fi
    done
    
    if [ ${#missing[@]} -ne 0 ]; then
        log_error "Herramientas faltantes: ${missing[*]}"
        return 1
    fi
    
    # Verificar Criterion
    if ! pkg-config --exists criterion; then
        log_error "Criterion no está disponible"
        return 1
    fi
    
    log_success "Todas las herramientas están disponibles"
    return 0
}

# Limpiar entorno antes de testing
clean_environment() {
    log_info "Limpiando entorno de testing..."
    
    # Usar el script existente
    if [ -f "./tools/clean_all.sh" ]; then
        ./tools/clean_all.sh
    else
        # Limpieza manual
        find . -name "build" -type d -exec rm -rf {} + 2>/dev/null || true
        find . -name "*.o" -delete 2>/dev/null || true
        find . -name "a.out" -delete 2>/dev/null || true
        find . -name "test_*" -type f -executable -delete 2>/dev/null || true
    fi
    
    log_success "Entorno limpio"
}

# Ejecutar tests de un bloque específico
test_block() {
    local block=$1
    log_info "Ejecutando tests del bloque $block..."
    
    if [ -f "./tools/quick_test.sh" ]; then
        ./tools/quick_test.sh "$block"
    else
        log_error "Script quick_test.sh no encontrado"
        return 1
    fi
}

# Ejecutar todos los tests
test_all() {
    log_info "Ejecutando suite completa de tests..."
    
    if [ -f "./tools/test_everything_fresh.sh" ]; then
        ./tools/test_everything_fresh.sh
    else
        log_error "Script test_everything_fresh.sh no encontrado"
        return 1
    fi
}

# Generar reporte de tests
generate_report() {
    log_info "Generando reporte de tests..."
    
    local report_file="/tmp/c100_test_report.txt"
    
    {
        echo "════════════════════════════════════════════════════════════"
        echo "C100 EJERCICIOS - REPORTE DE TESTS"
        echo "════════════════════════════════════════════════════════════"
        echo "Fecha: $(date)"
        echo "Host: $(hostname)"
        echo "Usuario: $(whoami)"
        echo "Directorio: $(pwd)"
        echo ""
        echo "INFORMACIÓN DEL SISTEMA:"
        echo "OS: $(cat /etc/os-release | grep PRETTY_NAME | cut -d= -f2 | tr -d '\"')"
        echo "Kernel: $(uname -r)"
        echo "Arquitectura: $(uname -m)"
        echo ""
        echo "HERRAMIENTAS:"
        echo "GCC: $(gcc --version | head -n1)"
        echo "CMake: $(cmake --version | head -n1)"
        echo "Make: $(make --version | head -n1)"
        echo ""
        echo "RESULTADO DE TESTS:"
    } > "$report_file"
    
    # Ejecutar tests y capturar resultado
    if test_all >> "$report_file" 2>&1; then
        echo "✅ TODOS LOS TESTS PASARON" >> "$report_file"
        log_success "Reporte generado: $report_file"
    else
        echo "❌ ALGUNOS TESTS FALLARON" >> "$report_file"
        log_error "Tests fallaron. Reporte: $report_file"
    fi
    
    echo "════════════════════════════════════════════════════════════" >> "$report_file"
    
    # Mostrar resumen
    echo ""
    echo "📋 RESUMEN DEL REPORTE:"
    tail -n 20 "$report_file"
}

# Función de ayuda
show_help() {
    echo "🐳 Docker Test Runner para C100 Ejercicios"
    echo "==========================================="
    echo ""
    echo "Uso: $0 [COMANDO]"
    echo ""
    echo "COMANDOS:"
    echo "  banner         Mostrar banner informativo"
    echo "  check          Verificar herramientas"
    echo "  clean          Limpiar entorno"
    echo "  test-all       Ejecutar todos los tests"
    echo "  test-block N   Ejecutar tests del bloque N"
    echo "  report         Generar reporte completo"
    echo "  interactive    Modo interactivo"
    echo ""
}

# Modo interactivo
interactive_mode() {
    show_banner
    
    while true; do
        echo ""
        echo "🐳 MENÚ INTERACTIVO:"
        echo "1) Verificar herramientas"
        echo "2) Limpiar entorno"
        echo "3) Ejecutar tests de un bloque"
        echo "4) Ejecutar todos los tests"
        echo "5) Generar reporte completo"
        echo "6) Abrir shell de desarrollo"
        echo "0) Salir"
        echo ""
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1) check_tools ;;
            2) clean_environment ;;
            3) 
                read -p "Número de bloque (1-15): " block
                test_block "$block"
                ;;
            4) test_all ;;
            5) generate_report ;;
            6) 
                echo "💡 Abriendo shell. Escribe 'exit' para volver al menú."
                /bin/bash
                ;;
            0) 
                log_info "¡Hasta luego!"
                exit 0
                ;;
            *) 
                log_error "Opción no válida"
                ;;
        esac
    done
}

# Función principal
main() {
    case "${1:-interactive}" in
        "banner")
            show_banner
            ;;
        "check")
            check_tools
            ;;
        "clean")
            clean_environment
            ;;
        "test-all")
            clean_environment
            test_all
            ;;
        "test-block")
            if [ -z "$2" ]; then
                log_error "Debe especificar el número de bloque"
                exit 1
            fi
            clean_environment
            test_block "$2"
            ;;
        "report")
            clean_environment
            generate_report
            ;;
        "interactive")
            interactive_mode
            ;;
        "help"|"--help"|"-h")
            show_help
            ;;
        *)
            log_error "Comando desconocido: $1"
            show_help
            exit 1
            ;;
    esac
}

# Ejecutar función principal
main "$@"
