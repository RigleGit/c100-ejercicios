#!/bin/bash
# Compila y ejecuta todas las pruebas CTest de la selección.
set -eu
shopt -s nullglob
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_DIR"

if [[ ${1:-} == help || ${1:-} == --help ]]; then
    echo "Uso: $0 [bloque] [ejercicio] | clean"
    exit 0
fi
if [[ ${1:-} == clean ]]; then
    for folder in [0-9][0-9]-*/[0-9][0-9][0-9]-*/; do
        rm -rf "$folder/build"
    done
    exit 0
fi
if [[ $# -gt 2 ]]; then
    echo "Demasiados argumentos" >&2; exit 1
fi
block=""; exercise=""
if [[ -n ${1:-} ]]; then
    [[ $1 =~ ^[0-9]{1,2}$ ]] || { echo "Bloque inválido" >&2; exit 1; }
    printf -v block '%02d' "$((10#$1))"
fi
if [[ -n ${2:-} ]]; then
    [[ $2 =~ ^[0-9]{1,3}$ ]] || { echo "Ejercicio inválido" >&2; exit 1; }
    printf -v exercise '%03d' "$((10#$2))"
fi
timeout="${C100_TEST_TIMEOUT:-15}"
[[ $timeout =~ ^[1-9][0-9]*$ ]] || { echo "Timeout inválido" >&2; exit 1; }

test_exercise() (
    cd "$1"
    echo "=== $1 ==="
    if ! cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug > /tmp/c100-configure-$$.log 2>&1; then
        cat /tmp/c100-configure-$$.log; rm -f /tmp/c100-configure-$$.log; exit 1
    fi
    rm -f /tmp/c100-configure-$$.log
    if ! cmake --build build -j 4 > build/compile.log 2>&1; then
        cat build/compile.log; exit 1
    fi
    LC_ALL=C ctest --test-dir build -N > build/test-list.log
    if ! grep -Eq 'Total Tests: [1-9][0-9]*' build/test-list.log; then
        echo "Error: no hay pruebas registradas" >&2; exit 1
    fi
    ctest --test-dir build --output-on-failure --timeout "$timeout"
)

count=0; success=0
for folder in [0-9][0-9]-*/[0-9][0-9][0-9]-*/; do
    parent="${folder%%/*}"
    name="$(basename "$folder")"
    [[ $parent == 00-* ]] && continue
    [[ -z $block || $parent == "$block"-* ]] || continue
    [[ -z $exercise || $name == "$exercise"-* ]] || continue
    count=$((count + 1))
    if test_exercise "$folder"; then success=$((success + 1)); fi
done
echo "Probados: $count | Exitosos: $success | Fallidos: $((count - success))"
[[ $count -gt 0 && $success -eq $count ]]
