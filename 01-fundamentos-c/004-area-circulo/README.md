# Ejercicio 004: Área del Círculo

## Enunciado

Escribe un programa que calcule el área de un círculo a partir del valor de su radio.

## Descripción

Este ejercicio refuerza el uso de variables de tipo `double`, introduce el uso de la librería `<math.h>` para realizar cálculos con potencias y promueve buenas prácticas al declarar constantes simbólicas en lugar de usar valores "mágicos". En este caso, el valor de π debe declararse de forma explícita como constante, facilitando la legibilidad, mantenimiento y reutilización del código.

## Fórmula Matemática

El área de un círculo se calcula con la fórmula:

```
A = π × r²
```

Donde:
- **A** = Área del círculo
- **π** = Pi (aproximadamente 3.1416)
- **r** = Radio del círculo

## Conceptos Clave

- **Variables `double`**: Mayor precisión que `float` para cálculos matemáticos
- **Librería `<math.h>`**: Funciones matemáticas como `pow()`
- **Constantes simbólicas**: `#define PI 3.1416` vs valores "mágicos"
- **Especificador `%lf`**: Para leer variables `double` con `scanf`
- **Formato de salida**: `%.2f` para mostrar dos decimales
- **Función `pow()`**: Calcular potencias de forma explícita
- **Enlazado de librerías**: `-lm` para la librería matemática

## Estructura del Proyecto

```
004-area-circulo/
├── CMakeLists.txt          # Configuración de compilación
├── README.md               # Este archivo
├── src/
│   └── main.c             # Código fuente principal
└── test/
    └── test_main.c        # Tests del programa
```

## Compilación y Ejecución

### Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar el programa
./area_circulo

# Ejecutar tests
make test
# o directamente:
./test_area_circulo
```

### Compilación Manual

```bash
# Compilar el programa principal (nota el -lm para la librería matemática)
gcc -o area_circulo src/main.c -lm

# Ejecutar
./area_circulo

# Compilar y ejecutar tests
gcc -o test_area_circulo test/test_main.c -lm
./test_area_circulo
```

## Ejemplo de Ejecución

```
Introduce el radio del círculo: 2.5
El área del círculo es 19.64
```

## Casos de Prueba

Los tests verifican cálculos matemáticos precisos:
- ✅ **Círculo unitario**: Radio 1 → Área ≈ π (3.14)
- ✅ **Radio entero**: Radio 2 → Área ≈ 12.57
- ✅ **Radio decimal**: Radio 2.5 → Área ≈ 19.64
- ✅ **Radio pequeño**: Radio 0.1 → Área ≈ 0.03
- ✅ **Estructura del código**: Variables `double`, librería `math.h`, constante π
- ✅ **Formato correcto**: `%.2f` y `%lf`

## Notas de Aprendizaje

1. **`#include <math.h>`**: Necesario para la función `pow()`
2. **`#define PI 3.1416`**: Constante simbólica mejor que valor literal
3. **`double radio, area;`**: Variables de doble precisión
4. **`scanf("%lf", &radio)`**: Especificador `%lf` para `double`
5. **`pow(radio, 2)`**: Función para calcular r²
6. **`%.2f`**: Formato de salida con exactamente dos decimales
7. **Enlace `-lm`**: Necesario para usar funciones matemáticas

## Tabla de Referencia - Áreas Comunes

| Radio | Área (π ≈ 3.1416) | Uso Común |
|-------|-------------------|-----------|
| 0.5 | 0.79 | Moneda pequeña |
| 1.0 | 3.14 | Círculo unitario |
| 2.0 | 12.57 | Plato pequeño |
| 5.0 | 78.54 | Pizza personal |
| 10.0 | 314.16 | Mesa redonda |

## Alternativas de Implementación

### Usando `const` en lugar de `#define`
```c
const double PI = 3.1416;
```

### Usando π más preciso
```c
#define PI 3.141592653589793
```

### Sin función `pow()` (más eficiente)
```c
area = PI * radio * radio;
```

## Posibles Errores Comunes

1. **Olvidar `-lm`**: El enlazador no encuentra funciones de `math.h`
2. **Usar `%f` con `double`**: Debería usar `%lf` en `scanf`
3. **Valores mágicos**: Usar `3.1416` directamente en lugar de constante
4. **Precisión insuficiente**: Usar `float` en lugar de `double`
5. **No incluir `math.h`**: Error de compilación con `pow()`

## Extensiones Posibles

- Calcular circunferencia además del área
- Validar que el radio sea positivo
- Usar `M_PI` de `math.h` si está disponible
- Implementar calculadora de figuras geométricas
- Comparar precisión entre `float` y `double`
- Agregar cálculo de sector circular
- Implementar conversión entre unidades (cm², m², etc.)
