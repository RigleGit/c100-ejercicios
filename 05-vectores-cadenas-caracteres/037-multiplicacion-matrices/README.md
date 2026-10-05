# Ejercicio 037: Multiplicación de Matrices

## Descripción
Este programa implementa la multiplicación de matrices, una operación fundamental en álgebra lineal. Multiplica una matriz A de tamaño 2×3 por una matriz B de tamaño 3×2, produciendo una matriz resultado de tamaño 2×2. Es un ejercicio avanzado que requiere comprensión profunda de bucles anidados y productos escalares.

## Enunciado
Escribir un programa que:
1. Declare dos matrices con dimensiones compatibles (A: 2×3, B: 3×2)
2. Inicialice las matrices con valores predeterminados
3. Implemente el algoritmo de multiplicación matricial
4. Almacene el resultado en una tercera matriz (2×2)
5. Muestre la matriz resultante

## Funcionalidades
- **Multiplicación matricial**: Implementación del algoritmo estándar
- **Dimensiones compatibles**: Manejo de matrices con dimensiones diferentes
- **Producto escalar**: Cálculo de productos punto entre filas y columnas
- **Triple bucle anidado**: Estructura algorítmica O(n³)
- **Inicialización de resultado**: Manejo correcto de acumuladores

## Ejemplo de Ejecución
```
Resultado de la multiplicación:
58	64	
139	154	
```

### Cálculo Detallado
```
Matriz A (2×3):        Matriz B (3×2):        Resultado (2×2):
[1  2  3]              [7   8 ]               [58   64 ]
[4  5  6]              [9   10]               [139  154]
                       [11  12]

Cálculos:
C[0][0] = 1×7 + 2×9 + 3×11 = 7 + 18 + 33 = 58
C[0][1] = 1×8 + 2×10 + 3×12 = 8 + 20 + 36 = 64
C[1][0] = 4×7 + 5×9 + 6×11 = 28 + 45 + 66 = 139
C[1][1] = 4×8 + 5×10 + 6×12 = 32 + 50 + 72 = 154
```

## Conceptos Demostrados
- **Multiplicación matricial**: Algoritmo fundamental del álgebra lineal
- **Producto escalar**: Suma de productos elemento por elemento
- **Bucles triple anidados**: Estructura algorítmica O(n³)
- **Compatibilidad dimensional**: Requisito columnas_A = filas_B
- **Acumulación**: Suma progresiva en cada posición del resultado
- **Inicialización**: Importancia de inicializar en cero

## Análisis del Algoritmo

### Algoritmo de Multiplicación Matricial
```c
// Algoritmo estándar O(n³)
for (int i = 0; i < FILAS_A; i++) {      // Para cada fila de A
    for (int j = 0; j < COLUMNAS_B; j++) {  // Para cada columna de B
        for (int k = 0; k < COLUMNAS_A; k++) { // Producto escalar
            resultado[i][j] += A[i][k] * B[k][j];
        }
    }
}
```

### Explicación Matemática
La multiplicación C = A × B se define como:
```
C[i][j] = Σ(k=0 to n-1) A[i][k] × B[k][j]
```

Donde:
- `i` recorre las filas de A (y del resultado)
- `j` recorre las columnas de B (y del resultado)  
- `k` recorre las columnas de A y las filas de B (dimensión común)

### Requisitos Dimensionales
Para multiplicar A × B:
- **Condición necesaria**: `columnas_A = filas_B`
- **Dimensión resultado**: `filas_A × columnas_B`
- **En nuestro caso**: (2×3) × (3×2) = (2×2)

### Complejidad Algorítmica
- **Temporal**: O(n³) para matrices n×n, O(p×q×r) para (p×q)×(q×r)
- **Espacial**: O(p×r) para almacenar el resultado
- **En nuestro caso**: O(2×3×2) = O(12) operaciones

## Aspectos Matemáticos Importantes

### Propiedades de la Multiplicación Matricial
- ✅ **Asociativa**: (AB)C = A(BC)
- ❌ **No conmutativa**: AB ≠ BA (en general)
- ✅ **Distributiva**: A(B+C) = AB + AC
- ✅ **Elemento neutro**: AI = IA = A (con identidad I)
- ✅ **Elemento absorvente**: A×0 = 0×A = 0

### Interpretación Geométrica
- **Filas de A**: Vectores que se proyectan sobre las columnas de B
- **Columnas de B**: Direcciones de proyección
- **Resultado**: Matriz de proyecciones escalares

### Aplicaciones Prácticas
- **Transformaciones gráficas**: Rotación, escala, traslación
- **Sistemas lineales**: Ax = b
- **Redes neuronales**: Propagación hacia adelante
- **Procesamiento de imágenes**: Convoluciones y filtros
- **Análisis de datos**: PCA, SVD

## Optimizaciones Posibles

### 1. Algoritmo de Strassen
- Complejidad: O(n^2.807)
- Más eficiente para matrices grandes
- Más complejo de implementar

### 2. Multiplicación por Bloques
```c
// Dividir matrices en submatrices
// Útil para matrices muy grandes que no caben en caché
```

### 3. Paralelización
```c
// Paralelizar bucles externos
#pragma omp parallel for
for (int i = 0; i < FILAS_A; i++) {
    // Cálculo independiente por fila
}
```

### 4. Optimizaciones de Caché
```c
// Reorganizar bucles para mejor localidad espacial
// Técnicas de tiling/blocking
```

## Verificación Manual de Cálculos

### Elemento C[0][0]:
```
C[0][0] = A[0][0]×B[0][0] + A[0][1]×B[1][0] + A[0][2]×B[2][0]
        = 1×7 + 2×9 + 3×11
        = 7 + 18 + 33
        = 58 ✓
```

### Elemento C[1][1]:
```
C[1][1] = A[1][0]×B[0][1] + A[1][1]×B[1][1] + A[1][2]×B[2][1]
        = 4×8 + 5×10 + 6×12
        = 32 + 50 + 72
        = 154 ✓
```

## Extensiones y Mejoras
- **Dimensiones variables**: Permitir tamaños dinámicos
- **Múltiples tipos de datos**: float, double, complex
- **Multiplicación in-place**: Para casos especiales
- **Verificación dimensional**: Validar compatibilidad
- **Entrada desde usuario**: Lectura interactiva de matrices
- **Multiplicación en cadena**: A×B×C×D optimizada

## Relación con Otros Conceptos
- **Predecesor**: Suma de matrices (ejercicio 036)
- **Fundamento para**: Sistemas lineales, eigenvalores
- **Conecta con**: Transformaciones geométricas, ML
- **Prepara para**: Algoritmos de álgebra lineal avanzada

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/multiplicacion_matrices
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
