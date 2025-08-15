# Ejercicio 010: Volumen de una Esfera

## Enunciado

Escribe un programa que calcule el volumen de una esfera a partir del valor de su radio. Utiliza la fórmula: V = (4/3) × π × r³

## Descripción

Este ejercicio refuerza el uso de constantes simbólicas, operaciones aritméticas combinadas y el manejo de variables de tipo `float` o `double`. Es similar al ejercicio del área del círculo, pero aquí se introduce una fórmula con un operador de potencia y una división no entera, ideal para afianzar la precedencia de operadores y la precisión numérica.

## Fórmula Matemática

El **volumen de una esfera** se calcula con la fórmula:

```
V = (4/3) × π × r³
```

Donde:
- **V** = Volumen de la esfera
- **π** = Pi (aproximadamente 3.1416)
- **r** = Radio de la esfera

### Derivación de la Fórmula

La fórmula del volumen de una esfera se deriva del cálculo integral, pero conceptualmente:
1. **Una esfera** es una superficie tridimensional
2. **El volumen** mide el espacio interior contenido
3. **La potencia cúbica** refleja las tres dimensiones espaciales

## Conceptos Clave

- **Variables `double`**: Precisión necesaria para cálculos volumétricos
- **Constantes simbólicas**: `#define PI 3.14159265359` con precisión mejorada
- **Función `pow()`**: Calcular potencias (r³)
- **División decimal**: `4.0 / 3.0` en lugar de `4 / 3`
- **Precedencia de operadores**: Paréntesis para claridad
- **Especificador `%lf`**: Para variables `double`
- **Formato de salida**: `%.6lf` para mostrar seis decimales en volumen, `%.2lf` para radio
- **Validación de entrada**: Verificar que el radio no sea negativo

## Estructura del Proyecto

```
010-volumen-esfera/
├── CMakeLists.txt          # Configuración de compilación
├── README.md               # Este archivo
├── src/
│   └── main.c             # Código fuente principal
└── test/
    └── test_main.c        # Tests del programa
```
- Validar entrada de datos

## Explicación
El volumen de una esfera se calcula con la fórmula matemática:
**V = (4/3) × π × r³**

Donde:
- V = volumen
- π = pi (aproximadamente 3.14159...)
- r = radio de la esfera

El programa usa `double` para mayor precisión en los cálculos y la función `pow()` de `math.h` para calcular r³.

## Ejemplo de Ejecución
```
Introduce el radio de la esfera: 3.0
El volumen de la esfera con radio 3.00 es: 113.097336 unidades cúbicas
```

```
Introduce el radio de la esfera: 1.5
El volumen de la esfera con radio 1.50 es: 14.137167 unidades cúbicas
```

```
Introduce el radio de la esfera: -2
Error: El radio no puede ser negativo
```

## Conceptos Clave
- **Función `pow()`**: Calcula potencias (base^exponente)
- **Constante PI**: Valor matemático fundamental π ≈ 3.14159...
- **Precisión double**: Mayor precisión que `float` para cálculos exactos
- **Validación de entrada**: Verificar que los datos sean válidos
- **Librería math.h**: Funciones matemáticas estándar

## Compilación y Ejecución
```bash
# Usando CMake
mkdir build && cd build
cmake ..
make
./volumen_esfera

# Compilación directa (importante: enlazar con -lm)
gcc -o volumen_esfera src/main.c -lm
./volumen_esfera
```

**Nota importante**: Es necesario enlazar con la librería matemática usando `-lm`.

## Pruebas
```bash
# Ejecutar tests automáticos
cd build
make test
# o
ctest -V
```

## Errores Comunes
1. **Olvidar enlazar con -lm**: `undefined reference to 'pow'`
2. **No validar entrada**: Aceptar radios negativos sin validación
3. **Pérdida de precisión**: Usar `float` en lugar de `double`
4. **PI impreciso**: Usar valores como 3.14 en lugar de una constante precisa
5. **División entera**: Escribir `4/3` en lugar de `4.0/3.0`

## Extensiones Posibles
1. Calcular también el área de la superficie: A = 4πr²
2. Comparar volúmenes de múltiples esferas
3. Calcular el radio dado el volumen (operación inversa)
4. Conversión entre diferentes unidades de medida
5. Crear función que calcule volumen de diferentes figuras geométricas

## Fórmulas Relacionadas
- **Área superficie esfera**: A = 4πr²
- **Diámetro**: d = 2r
- **Circunferencia**: C = 2πr
- **Radio desde volumen**: r = ∛(3V/4π)

## Variantes del Problema
- Volumen de un cubo: V = a³
- Volumen de un cilindro: V = πr²h
- Volumen de un cono: V = (1/3)πr²h
- Área de un círculo: A = πr²
