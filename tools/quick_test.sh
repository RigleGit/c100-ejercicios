#!/bin/bash

# Script rápido para probar ejercicios específicos
# Uso: ./quick_test.sh [bloque] [ejercicio]

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log() {
    case $1 in
        "INFO")  echo -e "${BLUE}[INFO]${NC} $2" ;;
        "WARN")  echo -e "${YELLOW}[WARN]${NC} $2" ;;
        "ERROR") echo -e "${RED}[ERROR]${NC} $2" ;;
        "SUCCESS") echo -e "${GREEN}[SUCCESS]${NC} $2" ;;
    esac
}

test_exercise() {
    local dir=$1
    local name=$(basename "$dir")
    
    if [ ! -f "$dir/CMakeLists.txt" ]; then
        log "WARN" "$name: No tiene CMakeLists.txt"
        return 1
    fi
    
    cd "$dir"
    
    log "INFO" "$name: Compilando..."
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -Wno-dev > /dev/null 2>&1 || {
        log "ERROR" "$name: Error en cmake"
        cd - > /dev/null
        return 1
    }
    
    cmake --build build -j 4 > /dev/null 2>&1 || {
        log "ERROR" "$name: Error en build"
        cd - > /dev/null  
        return 1
    }
    
    # Buscar ejecutable de tests
    local test_found=false
    for test_exe in build/test_* build/tests/test_* build/bin/test_* test_runner build/test_runner; do
        if [ -x "$test_exe" ]; then
            log "INFO" "$name: Ejecutando tests..."
            "./$test_exe" > /dev/null 2>&1 && {
                log "SUCCESS" "$name: ✓ Tests OK"
                test_found=true
                break
            } || {
                log "WARN" "$name: Tests fallaron"
                test_found=true
                break
            }
        fi
    done
    
    if [ "$test_found" = false ]; then
        log "INFO" "$name: ✓ Compiló OK (sin tests)"
    fi
    
    cd - > /dev/null
    return 0
}

clean_all() {
    log "INFO" "Limpiando archivos de build..."
    find . -name "build" -type d -exec rm -rf {} + 2>/dev/null || true
    find . -name "*.o" -delete 2>/dev/null || true
    find . -name "a.out" -delete 2>/dev/null || true
    find . -name "test_runner" -delete 2>/dev/null || true
    log "SUCCESS" "Limpieza completada"
}

main() {
    if [ "$1" = "clean" ]; then
        clean_all
        exit 0
    fi
    
    if [ "$1" = "help" ] || [ "$1" = "--help" ]; then
        echo "Uso: $0 [bloque] [ejercicio] | clean | help"
        echo "Ejemplos:"
        echo "  $0                # Probar todos"
        echo "  $0 12             # Solo bloque 12"
        echo "  $0 12 088         # Solo ejercicio 088"
        echo "  $0 clean          # Limpiar builds"
        exit 0
    fi
    
    local count=0
    local success=0
    
    if [ -n "$2" ]; then
        # Ejercicio específico
        for dir in */; do
            for subdir in "$dir"*"$2"*/; do
                if [ -d "$subdir" ]; then
                    count=$((count + 1))
                    test_exercise "$subdir" && success=$((success + 1))
                fi
            done
        done
    elif [ -n "$1" ]; then
        # Bloque específico
        local pattern=$(printf "%02d-*" "$1")
        for dir in $pattern/; do
            if [ -d "$dir" ]; then
                for subdir in "$dir"*/; do
                    if [ -d "$subdir" ]; then
                        count=$((count + 1))
                        test_exercise "$subdir" && success=$((success + 1))
                    fi
                done
            fi
        done
    else
        # Todos los ejercicios
        for dir in [0-9][0-9]-*/; do
            if [ -d "$dir" ]; then
                for subdir in "$dir"*/; do
                    if [ -d "$subdir" ]; then
                        count=$((count + 1))
                        test_exercise "$subdir" && success=$((success + 1))
                    fi
                done
            fi
        done
    fi
    
    echo
    echo "=================================================="
    echo -e "Probados: $count | Exitosos: ${GREEN}$success${NC} | Fallidos: ${RED}$((count - success))${NC}"
    echo "=================================================="
    
    if [ $success -eq $count ]; then
        log "SUCCESS" "¡Todos los ejercicios OK!"
        exit 0
    else
        log "WARN" "Algunos ejercicios necesitan atención"
        exit 1
    fi
}

main "$@"

