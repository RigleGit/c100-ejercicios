#!/bin/bash
# Si el script se ejecuta con /bin/sh u otra shell, relanzarlo con /usr/bin/bash si está disponible.
if [ -z "${BASH_VERSION:-}" ]; then
  if [ -x /usr/bin/bash ]; then
    exec /usr/bin/bash "$0" "$@"
  fi
fi

set -uo pipefail
IFS=$'\n\t'

# Asegurar que el script opere desde la raíz del repositorio (independiente del cwd)
# Esto permite ejecutar el script como `tools/clean_all.sh` desde la raíz o desde otra carpeta.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Habilitar nullglob para que los patrones de globs vacíos no se queden literales
# y dotglob para incluir carpetas que empiecen por punto únicamente cuando se necesite
shopt -s nullglob

# Uso:
#   ./cleanall.sh               -> DRY-RUN (no borra; muestra qué borraría)
#   ./cleanall.sh --apply       -> Aplica los borrados seguros
#   ./cleanall.sh --apply --hard-> Incluye borrado agresivo de ejecutables en subcarpetas
#
# Revisa SIEMPRE la salida del dry-run antes de --apply.

APPLY=false
HARD=false
for arg in "$@"; do
  case "$arg" in
    --apply) APPLY=true ;;
    --hard)  HARD=true  ;;
    *) echo "Uso: $0 [--apply] [--hard]"; exit 1 ;;
  esac
done

echo "=== Limpieza de artefactos de compilación y testing ==="
$APPLY || echo "[DRY-RUN] No se borrará nada. Añade --apply para ejecutar."

# Directorios a excluir SIEMPRE del recorrido
EXCLUDES=(
  ".git"
  ".devcontainer"
  ".github"
  ".vscode"
  "node_modules"
  "vendor"
  "external"
  "third_party"
  "venv"
  "tools"
)

# find con exclusiones (NO añade -print)
do_find() {
  # do_find <dir> <find-args...>
  local dir="$1"; shift
  # Simplificamos: solo excluimos las carpetas más importantes
  find "$dir" -name ".git" -prune -o -name ".devcontainer" -prune -o -name "tools" -prune -o "$@" 2>/dev/null
}

# Borra (o simula) paths recibidos por stdin terminados en NUL
do_delete() {
  local n=0
  local path
  # Usar un temporal para evitar problemas con set -e y pipelines vacíos
  local temp_file=$(mktemp)
  cat > "$temp_file"
  
  if [ -s "$temp_file" ]; then
    while IFS= read -r -d '' path; do
      if $APPLY; then
        rm -rf -- "$path"
      fi
      printf '    %s %s\n' "$( $APPLY && echo 'DELETED' || echo 'WOULD DELETE')" "$path"
      n=$((n+1))
    done < "$temp_file"
  fi
  
  rm -f "$temp_file"
  total_deleted=$((total_deleted + n))
}

total_deleted=0
total_dirs=0

clean_directory() {
  local dir="$1"
  [ -d "$dir" ] || return 0
  echo "Carpeta: $dir"
  total_dirs=$((total_dirs+1))

  # 1) Directorios típicos de build / test infra
  do_find "$dir" \( -name "build" -o -name "CMakeFiles" -o -name "Testing" -o -name ".cache" -o -name ".pytest_cache" -o -name "bin" -o -name "obj" -o -name "debug" -o -name "release" \) -type d -prune -print0 | do_delete

  # 2) Archivos CMake sueltos y de build
  do_find "$dir" \( -name "CMakeCache.txt" -o -name "cmake_install.cmake" -o -name "Makefile" -o -name "compile_commands.json" -o -name "CTestTestfile.cmake" \) -type f -print0 | do_delete

  # 3) Objetos, bibliotecas y archivos compilados
  do_find "$dir" \( -name "*.o" -o -name "*.obj" -o -name "*.a" -o -name "*.lib" -o -name "*.so" -o -name "*.dll" -o -name "*.dylib" -o -name "*.exe" \) -type f -print0 | do_delete

  # 4) Coverage / perfiles / debug
  do_find "$dir" \( -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" -o -name "*.profraw" -o -name "*.profdata" -o -name ".coverage" -o -name "*.pdb" -o -name "*.ilk" \) -type f -print0 | do_delete

  # 5) .dSYM (macOS) y otros directorios de debug
  do_find "$dir" \( -name "*.dSYM" -o -name "*.app" \) -type d -prune -print0 | do_delete

  # 6) Ejecutables (seguro: solo nivel 1 de la carpeta)
  if ! $HARD; then
    do_find "$dir" -maxdepth 1 -type f -perm -111 \
      ! -name "*.sh" ! -name "*.py" ! -name "*.pl" ! -name "*.rb" ! -name "*.cmd" ! -name "*.bat" ! -name "*.exe" -print0 | do_delete
  else
    # Agresivo: recursivo
    do_find "$dir" -type f -perm -111 \
      ! -name "*.sh" ! -name "*.py" ! -name "*.pl" ! -name "*.rb" ! -name "*.cmd" ! -name "*.bat" ! -name "*.exe" -print0 | do_delete
  fi

  # 7) Ejecutables comunes por nombre (por si no tienen +x)
  do_find "$dir" \( -name "a.out" -o -name "main" -o -name "programa" -o -name "test_*" -o -name "*_test" -o -name "*.test" -o -name "debug_*" \) -type f -print0 | do_delete

  # 8) Ficheros de prueba y temporales
  do_find "$dir" \( -name "test_*.txt" -o -name "demo_*.txt" -o -name "*_output.txt" -o -name "test_output.txt" -o -name "*.tmp" -o -name "*.temp" -o -name "core" -o -name "core.*" \) -type f -print0 | do_delete

  # 9) Archivos de respaldo y editor
  do_find "$dir" \( -name "*~" -o -name "*.bak" -o -name "*.orig" -o -name "*.swp" -o -name "*.swo" -o -name ".*.swp" -o -name ".*.swo" \) -type f -print0 | do_delete

  # 10) Logs y archivos de error
  do_find "$dir" \( -name "*.log" -o -name "error.txt" -o -name "errors.txt" -o -name "warnings.txt" \) -type f -print0 | do_delete

  # 11) Basura macOS y otros SO
  do_find "$dir" \( -name ".DS_Store" -o -name "Thumbs.db" -o -name "desktop.ini" \) -type f -print0 | do_delete
}

# === Recorrido de bloques y ejercicios ===
# Limpia el propio bloque y cada subcarpeta (ejercicio) sin depender de CMakeLists.txt
# Usamos find para listar directorios de primer nivel (más robusto en distintos shells/entornos)
while IFS= read -r -d '' block_path; do
  # block_path tiene la forma './00-plantilla'
  # Normalizar a formato sin prefijo './' y con slash final
  block=${block_path#./}
  [ -z "$block" ] && continue
  # Añadir slash al final para mantener compatibilidad con clean_directory
  block_dir="$block/"

  # Excluir la carpeta tools explícitamente
  if [ "${block_dir}" = "tools/" ]; then
    continue
  fi

  if [ -d "$block_dir" ]; then
  echo "Procesando bloque: $block_dir"
    clean_directory "$block_dir"

    # Limpiar cada ejercicio dentro del bloque
    # Usar glob local y comprobar existencia
    for exercise in "$block_dir"*/; do
      [ -d "$exercise" ] && clean_directory "$exercise"
    done
  fi
done < <(find . -maxdepth 1 -mindepth 1 -type d -print0)

echo "Limpieza en la raíz del repo"
clean_directory "."

echo ""
echo "=== RESUMEN ==="
echo "   Directorios procesados: $total_dirs"
echo "   Elementos eliminados: $total_deleted"
echo "   Modo: $( $APPLY && echo 'APLICADO' || echo 'DRY-RUN (simulación)')"
