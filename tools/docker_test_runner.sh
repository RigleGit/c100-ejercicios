#!/usr/bin/env bash
set -euo pipefail
raiz="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$raiz"
check_tools() {
    local herramienta
    for herramienta in cc cmake make python3; do
        command -v "$herramienta" >/dev/null || {
            echo "Falta: $herramienta" >&2; return 1;
        }
    done
}
report() {
    local archivo="${C100_REPORT_PATH:-/tmp/c100_test_report.txt}" estado=0
    { date; uname -s; cmake --version; } > "$archivo"
    ./tools/test_everything_fresh.sh >> "$archivo" 2>&1 || estado=$?
    tail -n 25 "$archivo"
    echo "Reporte: $archivo"
    return "$estado"
}
interactive() {
    local opcion bloque
    while read -r -p "1) comprobar 2) limpiar 3) bloque 4) todo 5) reporte 6) shell 0) salir: " opcion; do
        case "$opcion" in
            1) check_tools ;;
            2) ./tools/clean_all.sh --apply ;;
            3) read -r -p "Bloque: " bloque || return 0
               ./tools/quick_test.sh "$bloque" ;;
            4) ./tools/test_everything_fresh.sh ;;
            5) report ;;
            6) bash ;;
            0) return 0 ;;
            *) echo "Opción inválida" >&2 ;;
        esac
    done
}
case "${1:-interactive}" in
    check) check_tools ;;
    clean) ./tools/clean_all.sh --apply ;;
    test-all) ./tools/test_everything_fresh.sh ;;
    test-block) ./tools/quick_test.sh "${2:?Falta el bloque}" ;;
    report) report ;;
    interactive) interactive ;;
    banner) uname -s; cc --version; cmake --version ;;
    help|--help|-h) echo "Uso: $0 check|clean|test-all|test-block N|report|interactive" ;;
    *) echo "Comando desconocido: $1" >&2; exit 1 ;;
esac
