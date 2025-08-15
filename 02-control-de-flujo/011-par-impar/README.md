# Ejercicio 011: Par o Impar

## Enunciado

Escribe un programa que determine si un número entero introducido por el usuario es par o impar.

## Descripción

Este ejercicio es un clásico para introducir el uso de **estructuras condicionales**. Permite practicar el operador módulo (`%`) y entender cómo los programas pueden **tomar decisiones** en función de los datos que reciben. Es también una oportunidad para reforzar el control del flujo con `if`.

## Conceptos Matemáticos

Un número entero es:
- **Par**: si es divisible entre 2 (resto = 0)
- **Impar**: si no es divisible entre 2 (resto = 1)

El **operador módulo** (`%`) devuelve el resto de una división:
- `8 % 2 = 0` → 8 es par
- `7 % 2 = 1` → 7 es impar
- `0 % 2 = 0` → 0 es par
- `-4 % 2 = 0` → -4 es par

## Conceptos Clave

- **Estructura condicional `if-else`**: Control de flujo básico
- **Operador módulo (`%`)**: Obtiene el resto de una división
- **Expresión booleana**: `numero % 2 == 0` evalúa verdadero/falso
- **Variables `int`**: Números enteros con signo
- **Especificador `%d`**: Para enteros en scanf/printf
- **Casos especiales**: Cero y números negativos

## Estructura del Proyecto

```
011-par-impar/
├── CMakeLists.txt          # Configuración de compilación
├── README.md               # Este archivo
├── .gitignore             # Archivos a ignorar en git
├── include/               # Archivos de cabecera (vacío en este ejercicio)
├── src/
│   └── main.c             # Código fuente principal
└── test/
    └── test_main.c        # Tests del programa
```

## Explicación del Algoritmo

1. **Entrada**: Solicitar número entero al usuario
2. **Proceso**: Aplicar operador módulo `numero % 2`
3. **Decisión**: Si el resto es 0 → par, si no → impar
4. **Salida**: Mostrar el resultado

```c
if (numero % 2 == 0) {
    // Es par
} else {
    // Es impar
}
```

## Ejemplo de Ejecución

```
Introduce un número entero: 8
El número 8 es par.
```

```
Introduce un número entero: 7
El número 7 es impar.
```

```
Introduce un número entero: 0
El número 0 es par.
```

```
Introduce un número entero: -3
El número -3 es impar.
```

## Conceptos Clave Detallados

### 1. Operador Módulo (%)
- **Sintaxis**: `a % b` devuelve el resto de `a / b`
- **Aplicación**: `numero % 2` da 0 si es par, 1 si es impar
- **Casos especiales**: Funciona con números negativos

### 2. Estructura If-Else
- **Condicional simple**: Permite bifurcación del programa
- **Expresión booleana**: Se evalúa como verdadera o falsa
- **Flujo de control**: Solo una rama se ejecuta

### 3. Entrada y Salida
- **scanf("%d", &variable)**: Lee entero desde teclado
- **printf("formato", variable)**: Muestra resultado formateado

## Compilación y Ejecución

```bash
# Usando CMake (recomendado)
mkdir build && cd build
cmake ..
make
./par_impar

# Compilación directa
gcc -o par_impar src/main.c
./par_impar
```

## Pruebas

```bash
# Ejecutar tests automáticos
cd build
make test
# o
ctest -V
```

Los tests verifican:
- Números pares positivos
- Números impares positivos
- Número cero (caso especial)
- Números negativos (pares e impares)
- Números grandes
- Formato correcto de entrada y salida

## Errores Comunes

1. **Confundir operadores**: Usar `=` en lugar de `==` en la condición
2. **Olvidar el else**: Solo manejar un caso (par o impar)
3. **No validar entrada**: Asumir que siempre se introduce un número válido
4. **Mal uso del módulo**: No entender que `% 2` da 0 o 1
5. **Problema con negativos**: No probar que funciona con números negativos

## Extensiones Posibles

1. **Validación de entrada**: Verificar que se introduce un número válido
2. **Múltiples números**: Procesar varios números en un bucle
3. **Divisibilidad general**: Verificar divisibilidad por cualquier número
4. **Menú de opciones**: Elegir entre diferentes operaciones
5. **Historial**: Mantener registro de números procesados

## Tabla de Verdad

| Número | numero % 2 | Resultado |
|--------|------------|-----------|
| 0      | 0          | Par       |
| 1      | 1          | Impar     |
| 2      | 0          | Par       |
| 3      | 1          | Impar     |
| 4      | 0          | Par       |
| -1     | 1          | Impar     |
| -2     | 0          | Par       |

## Variantes del Problema

- **Múltiplo de 3**: `numero % 3 == 0`
- **Número primo**: Verificar divisibilidad por varios números
- **Clasificación numérica**: Par/impar + positivo/negativo/cero
- **Dígitos pares/impares**: Analizar dígitos individuales
