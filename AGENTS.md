# Repository Guidelines

## Project Structure & Module Organization
- Topics live under numbered folders (e.g., `01-fundamentos-c`, `12-concurrencia`, `15-proyectos-finales`).
- Each exercise resides in `NNN-nombre-ejercicio/` and typically contains: `src/`, `include/`, `test/` (or `tests/` in the template), `CMakeLists.txt`, `README.md`, and sometimes `ejemplos/` and `tools/`. Some final projects also include a `Makefile`.
- The scaffold lives in `00-plantilla/` with `crear_ejercicio.sh` to generate a new exercise.

## Build, Test, and Development Commands
- CMake (per exercise):
  - `cd 01-fundamentos-c/001-hola-mundo`
  - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
  - `cmake --build build`
  - Run program: `./build/hola_mundo`
  - Run tests: `ctest --test-dir build -V`
- Make (only if `Makefile` exists, mainly in template/final projects): `make`, `make test`, `make demo`.

## Coding Style & Naming Conventions
- Language: C99 with strict warnings (`-Wall -Wextra -Werror -pedantic`). Indentation: 4 spaces. Encoding: UTF-8.
- Directories: `NNN-nombre-ejercicio`. Files: lower_snake_case for `.c/.h` (e.g., `test/test_nombre.c`). Keep `main.c` minimal; place logic in separate modules under `src/` with headers in `include/`.
- Comments and docs may be in Spanish; prefer concise, actionable explanations.

## Testing Guidelines
- Tests are small C programs using `assert` and are run via CTest. Place them in `test/` (or `tests/`) and name `test_<tema>.c`.
- Ensure tests can locate the built binary; CTest runs from `build/`, so spawning `./<ejecutable>` works.
- Minimal CMake pattern:
  - `enable_testing()`
  - `add_executable(test_<name> test/test_<name>.c)`
  - `add_test(NAME <Name> COMMAND test_<name>)`

## Commit & Pull Request Guidelines
- Use Conventional Commits: `feat:`, `fix:`, `docs:`, `test:`, `refactor:`, `chore:`. Scope with folder when useful (e.g., `feat(03-funciones-modularidad/024-funcion-mcd): ...`).
- Keep commits focused (ideally one exercise per commit). In PRs, include: clear description, paths touched, rationale, and evidence of local build/test (`ctest` output or notes).

## Security & Configuration Tips
- Favor safe C practices (input validation, bounds checks, no unchecked I/O). See `14-seguridad-buenas-practicas/` for patterns.
- Debug builds: `-DCMAKE_BUILD_TYPE=Debug`. To enable sanitizers: `CFLAGS="-fsanitize=address,undefined -g -O0" cmake -S . -B build`.

