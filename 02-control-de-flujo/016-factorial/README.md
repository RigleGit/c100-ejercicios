# Ejercicio 016: Factorial

## Enunciado

Escribe un programa que calcule el factorial de un número entero positivo introducido por el usuario, usando un bucle `for`.

## Descripción

Este ejercicio sirve para introducir los **bucles controlados por contador**, en este caso, el cálculo de un factorial `n! = 1 × 2 × ... × n`, que es un clásico por su sencillez conceptual y su utilidad para practicar **multiplicación acumulativa**. Además, refuerza la necesidad de **inicializar correctamente variables** y gestionar adecuadamente los límites del bucle.

## Conceptos Matemáticos

El **factorial** de un número entero no negativo `n` se define como:

```
n! = n × (n-1) × (n-2) × ... × 2 × 1
```

**Casos especiales:**
- `0! = 1` (por definición matemática)
- `1! = 1`
- Para `n < 0`: El factorial no está definido

**Ejemplos:**
- `5! = 5 × 4 × 3 × 2 × 1 = 120`
- `10! = 3,628,800`

## Conceptos Clave

- **Bucle `for`**: Estructura de control con contador
- **Multiplicación acumulativa**: Variable que acumula el producto
- **Tipo `unsigned long long`**: Para manejar números grandes
- **Inicialización correcta**: Variable factorial inicializada en 1
- **Validación de entrada**: Verificar que el número no sea negativo
- **Límites del bucle**: De 1 hasta n (inclusive)
- **Especificador `%llu`**: Para números de tipo unsigned long long

## Estructura del Proyecto

```
016-factorial/
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

### Pasos del Algoritmo

1. **Entrada**: Leer número entero del usuario
2. **Validación**: Verificar que no sea negativo
3. **Inicialización**: `factorial = 1`
4. **Bucle**: Para `i` desde 1 hasta `n`, multiplicar `factorial *= i`
5. **Salida**: Mostrar el resultado

### Estructura del Bucle For

```c
for (int i = 1; i <= n; ++i) {
    factorial *= i;
}
```

- **Inicialización**: `i = 1` (primer número a multiplicar)
- **Condición**: `i <= n` (incluir el número n)
- **Incremento**: `++i` (siguiente número)
- **Cuerpo**: `factorial *= i` (multiplicación acumulativa)

## Ejemplo de Ejecución

```
Introduce un número entero positivo: 5
El factorial de 5 es 120
```

```
Introduce un número entero positivo: 0
El factorial de 0 es 1
```

```
Introduce un número entero positivo: 10
El factorial de 10 es 3628800
```

```
Introduce un número entero positivo: -3
El factorial no está definido para números negativos.
```

## Tabla de Factoriales

| n | n! | Cálculo |
|---|----|---------| 
| 0 | 1 | Por definición |
| 1 | 1 | 1 |
| 2 | 2 | 1 × 2 |
| 3 | 6 | 1 × 2 × 3 |
| 4 | 24 | 1 × 2 × 3 × 4 |
| 5 | 120 | 1 × 2 × 3 × 4 × 5 |
| 6 | 720 | 1 × 2 × 3 × 4 × 5 × 6 |
| 7 | 5,040 | 1 × 2 × 3 × 4 × 5 × 6 × 7 |
| 8 | 40,320 | 1 × 2 × 3 × 4 × 5 × 6 × 7 × 8 |
| 9 | 362,880 | ... |
| 10 | 3,628,800 | ... |

## Conceptos Clave Detallados

### 1. Bucle For
- **Sintaxis**: `for (inicialización; condición; incremento)`
- **Ventaja**: Código compacto para bucles con contador
- **Control**: Número conocido de iteraciones

### 2. Multiplicación Acumulativa
```c
unsigned long long factorial = 1;  // Inicialización
for (int i = 1; i <= n; ++i) {
    factorial *= i;  // Equivalente a: factorial = factorial * i
}
```

### 3. Tipo de Datos
- **`unsigned long long`**: 64 bits, rango 0 a 18,446,744,073,709,551,615
- **Necesario**: Los factoriales crecen muy rápidamente
- **Especificador**: `%llu` en printf

### 4. Caso Especial: 0!
- **Matemáticamente**: 0! = 1 por definición
- **En el código**: El bucle no se ejecuta cuando n=0, factorial permanece como 1

## Compilación y Ejecución

```bash
# Usando CMake (recomendado)
mkdir build && cd build
cmake ..
make
./factorial

# Compilación directa
gcc -o factorial src/main.c
./factorial
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
- Factorial de 0 (caso especial)
- Factoriales pequeños (1, 2, 3, 4, 5)
- Factoriales medianos (6, 7, 8, 9, 10)
- Factoriales grandes (12, 20)
- Números negativos (manejo de errores)
- Formato correcto de entrada y salida
- Estructura correcta del código (for, unsigned long long, etc.)

## Crecimiento Exponencial

Los factoriales crecen extremadamente rápido:

| n | n! | Dígitos |
|---|----|---------| 
| 10 | 3,628,800 | 7 |
| 15 | 1,307,674,368,000 | 13 |
| 20 | 2,432,902,008,176,640,000 | 19 |
| 25 | 15,511,210,043,330,985,984,000,000 | 26 |

**Límite práctico**: Con `unsigned long long`, podemos calcular hasta aproximadamente 20! antes de desbordamiento.

## Errores Comunes

1. **Inicialización incorrecta**: `factorial = 0` en lugar de `factorial = 1`
2. **Límites del bucle**: `i < n` en lugar de `i <= n`
3. **Tipo de datos insuficiente**: Usar `int` en lugar de `unsigned long long`
4. **No validar entrada**: No verificar números negativos
5. **Especificador incorrecto**: `%d` en lugar de `%llu`
6. **Desbordamiento**: No considerar límites del tipo de datos

## Trace de Ejecución (n = 5)

| Iteración | i | factorial antes | factorial después |
|-----------|---|-----------------|-------------------|
| Inicial | - | 1 | 1 |
| 1 | 1 | 1 | 1 × 1 = 1 |
| 2 | 2 | 1 | 1 × 2 = 2 |
| 3 | 3 | 2 | 2 × 3 = 6 |
| 4 | 4 | 6 | 6 × 4 = 24 |
| 5 | 5 | 24 | 24 × 5 = 120 |
| Final | - | 120 | 120 |

## Extensiones Posibles

1. **Tabla de factoriales**: Mostrar factoriales de 1 a n
2. **Validación robusta**: Manejar entrada no numérica
3. **Detección de desbordamiento**: Avisar cuando el resultado es demasiado grande
4. **Factorial con double**: Mayor rango usando punto flotante
5. **Comparación de métodos**: Iterativo vs recursivo
6. **Aproximación de Stirling**: Para números muy grandes

## Variantes del Problema

- **Factorial recursivo**: Implementación usando recursión
- **Subfactorial**: Número de desarreglos
- **Factorial doble**: n!! = n × (n-2) × (n-4) × ...
- **Gamma function**: Generalización para números reales
- **Combinaciones**: Aplicar factorial en cálculo de C(n,k)

## Aplicaciones Prácticas

- **Combinatoria**: Cálculo de permutaciones y combinaciones
- **Probabilidad**: Distribuciones estadísticas
- **Matemáticas**: Series de Taylor, análisis matemático
- **Ciencias de la computación**: Análisis de algoritmos
- **Criptografía**: Algunos algoritmos criptográficos
