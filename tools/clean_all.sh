#!/usr/bin/env bash
# Solo elimina carpetas generadas; por defecto muestra una simulación.
set -euo pipefail
raiz="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
aplicar=false
for opcion in "$@"; do
    case "$opcion" in
        --apply) aplicar=true ;;
        --hard) echo "--hard ya no se admite; usa --apply para eliminar solo build." >&2; exit 1 ;;
        *) echo "Uso: $0 [--apply]" >&2; exit 1 ;;
    esac
done
shopt -s nullglob
total=0
for carpeta in "$raiz"/[0-9][0-9]-*/[0-9][0-9][0-9]-*/build "$raiz"/alternativas/*/build; do
    [[ -d "$carpeta" || -L "$carpeta" ]] || continue
    if "$aplicar"; then
        rm -rf -- "$carpeta"
        printf 'Eliminado: %s\n' "$carpeta"
    else
        printf 'Se eliminaría: %s\n' "$carpeta"
    fi
    total=$((total + 1))
done
printf 'Carpetas build: %d\n' "$total"
if ! "$aplicar"; then
    echo "Simulación. Añade --apply para ejecutar."
fi
