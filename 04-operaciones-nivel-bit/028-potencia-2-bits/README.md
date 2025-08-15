# Ejercicio 028 — Potencia de 2 con Bits

## Descripción

Funciones y demostraciones para detectar potencias de 2 usando operaciones a nivel de bits en C. El ejercicio muestra el clásico truco `n & (n-1) == 0` y varias implementaciones alternativas (contar bits, división, logaritmos), además de utilidades relacionadas: siguiente/anterior potencia de 2, cálculo del exponente y ejemplos prácticos.

## Objetivos

- Comprender operaciones bit a bit y optimizaciones para problemas numéricos.
- Implementar y comparar varias técnicas para determinar si un número es potencia de 2.
- Aplicar concepciones prácticas (alineación de memoria, máscaras) y demostrar su uso.

## Estructura del ejercicio

- `src/main.c` — Implementación principal con funciones y demo interactiva.
- `CMakeLists.txt` — Configuración para compilar el ejercicio.
- `test/` — Tests (si existen) para validar las funciones.

## API pública (firmas importantes)

```c
bool esPotenciaDe2(unsigned int n);
bool esPotenciaDe2_contandoBits(unsigned int n);
bool esPotenciaDe2_division(unsigned int n);
bool esPotenciaDe2_logaritmo(unsigned int n);

unsigned int siguientePotenciaDe2(unsigned int n);
unsigned int anteriorPotenciaDe2(unsigned int n);
int quePotenciaDe2(unsigned int n);

void generarPotenciasDe2(unsigned int limite);
void demostrarTruco(unsigned int n);
void analizarRango(unsigned int inicio, unsigned int fin);
void aplicacionesPracticas(void);
```

## Cómo compilar

Este ejercicio usa CMake. Desde la raíz del ejercicio:

```bash
mkdir -p build && cd build
cmake ..
make
```

El ejecutable resultante suele llamarse `potencia_2_bits` (o como esté definido en `CMakeLists.txt`) en el directorio `build/`.

## Cómo ejecutar

Después de compilar, ejecuta el binario:

```bash
# Desde el directorio build
./potencia_2_bits
```

El programa muestra una demostración automática (números predefinidos, comparación de métodos, análisis de rango, aplicaciones prácticas) y después solicita al usuario un número para realizar demostraciones interactivas (demostración del truco, potencias anterior/siguiente, etc.).

## Ejemplos de uso

- Comprobar si 16 es potencia de 2:

```c
// devuelve true (2^4)
bool r = esPotenciaDe2(16);
```

- Obtener la siguiente potencia de 2 mayor o igual a 1000:

```c
unsigned int p = siguientePotenciaDe2(1000); // 1024
```

## Notas y consideraciones

- La función `esPotenciaDe2` es O(1) y usa operaciones bit a bit; es la solución preferida para enteros no nulos.
- Las alternativas (contar bits, división) son educativas y sirven para comparar complejidad y robustez.
- `siguientePotenciaDe2` emplea un patrón de "llenado" de bits eficiente.
- Cuidado con 0 y con enteros máximos (UINT_MAX). El programa maneja el caso 0 como "no potencia de 2" por definición.

## Pruebas

Si hay un subdirectorio `test/` con tests (por ejemplo con Criterion), compílalos junto al proyecto y ejecuta el binario de tests en `build/`.

## Try it

Desde la raíz del repo puedes ejecutar sólo este ejercicio con:

```bash
cd 04-operaciones-nivel-bit/028-potencia-2-bits
mkdir -p build && cd build
cmake .. && make
./potencia_2_bits
```
