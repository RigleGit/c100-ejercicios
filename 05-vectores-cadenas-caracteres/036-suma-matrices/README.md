# Ejercicio 036: Suma de Matrices

## Descripción
Este programa introduce el manejo de arrays bidimensionales (matrices) en C. Lee dos matrices de enteros de tamaño 3x3, calcula su suma elemento por elemento y almacena el resultado en una tercera matriz. Es fundamental para comprender estructuras de datos bidimensionales.

## Enunciado
Escribir un programa que:
1. Declare tres matrices de enteros de tamaño 3x3
2. Solicite al usuario los elementos de la primera matriz
3. Solicite al usuario los elementos de la segunda matriz
4. Calcule la suma elemento por elemento: C[i][j] = A[i][j] + B[i][j]
5. Muestre la matriz resultante de forma organizada

## Funcionalidades
- **Arrays bidimensionales**: Declaración y manejo de matrices
- **Entrada estructurada**: Lectura organizada de elementos matriciales
- **Operaciones matriciales**: Suma elemento por elemento
- **Bucles anidados**: Recorrido eficiente de estructuras 2D
- **Salida formateada**: Presentación tabular de resultados

## Compilación y Ejecución

### Requisitos
- Compilador C (gcc, clang)
- CMake 3.10 o superior
- Criterion (para tests, opcional)

### Compilación con CMake
```bash
mkdir build
cd build
cmake ..
make
```

### Ejecución
```bash
# Desde el directorio build
./bin/suma_matrices

# O directamente con gcc
gcc -o suma_matrices src/main.c
./suma_matrices
```

### Ejecutar Tests
```bash
# Desde el directorio build
make run_tests

# O ejecutar directamente
./test_036-suma-matrices
```

## Ejemplo de Uso
```
Introduce los elementos de la primera matriz (3x3):
A[0][0]: 1
A[0][1]: 2
A[0][2]: 3
A[1][0]: 4
A[1][1]: 5
A[1][2]: 6
A[2][0]: 7
A[2][1]: 8
A[2][2]: 9

Introduce los elementos de la segunda matriz (3x3):
B[0][0]: 9
B[0][1]: 8
B[0][2]: 7
B[1][0]: 6
B[1][1]: 5
B[1][2]: 4
B[2][0]: 3
B[2][1]: 2
B[2][2]: 1

Matriz resultante de la suma:
10	10	10	
10	10	10	
10	10	10	
```

## Conceptos Demostrados
- **Arrays bidimensionales**: Declaración con `int matriz[FILAS][COLUMNAS]`
- **Indexación 2D**: Acceso con `matriz[i][j]`
- **Bucles anidados**: Estructura for dentro de for para recorrido 2D
- **Operaciones matriciales**: Suma elemento por elemento
- **Constantes simbólicas**: Uso de `#define` para dimensiones
- **Formato de salida**: Presentación tabular con tabulaciones

## Estructura del Proyecto
```
036-suma-matrices/
├── src/
│   └── main.c          # Código principal
├── test/
│   └── test_main.c     # Tests unitarios
├── include/            # Archivos de cabecera (si los hay)
├── CMakeLists.txt      # Configuración de CMake
├── .gitignore         # Archivos a ignorar por git
└── README.md          # Este archivo
```

## Análisis del Algoritmo

### Estructura de Datos
```c
#define FILAS 3
#define COLUMNAS 3

int A[FILAS][COLUMNAS];      // Matriz A
int B[FILAS][COLUMNAS];      // Matriz B  
int resultado[FILAS][COLUMNAS]; // Matriz resultado
```

### Algoritmo de Suma Matricial
```c
// Suma elemento por elemento
for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
        resultado[i][j] = A[i][j] + B[i][j];
    }
}
```

### Complejidad
- **Temporal**: O(n×m) donde n=filas, m=columnas → O(9) = O(1) para 3x3
- **Espacial**: O(n×m) para almacenar las tres matrices

## Casos de Prueba
Los tests incluyen verificaciones para:
- **Suma básica**: Matrices con valores simples
- **Elemento neutro**: Suma con matriz de ceros
- **Matrices iguales**: Verificar duplicación de valores
- **Números negativos**: Manejo de valores negativos y anulación
- **Matriz identidad**: Casos especiales con patrones conocidos
- **Valores grandes**: Verificar que no hay overflow
- **Casos mixtos**: Combinaciones de positivos y negativos
- **Matrices especiales**: Diagonal, simétricas, etc.

## Conceptos Matemáticos

### Suma de Matrices
La suma de matrices A + B se define como:
```
C[i][j] = A[i][j] + B[i][j]
```

**Propiedades:**
- ✅ **Conmutativa**: A + B = B + A
- ✅ **Asociativa**: (A + B) + C = A + (B + C)
- ✅ **Elemento neutro**: A + 0 = A
- ✅ **Elemento opuesto**: A + (-A) = 0

### Requisitos para Suma Matricial
- Las matrices deben tener **las mismas dimensiones**
- Se opera **elemento por elemento**
- El resultado tiene las **mismas dimensiones** que las matrices originales

## Aspectos Técnicos Importantes

### Declaración de Arrays 2D
```c
// Declaración estática con dimensiones constantes
int matriz[FILAS][COLUMNAS];

// Acceso a elementos
matriz[fila][columna] = valor;
valor = matriz[fila][columna];
```

### Recorrido con Bucles Anidados
```c
// Patrón estándar para recorrer matrices
for (int i = 0; i < FILAS; i++) {      // Recorre filas
    for (int j = 0; j < COLUMNAS; j++) { // Recorre columnas
        // Procesar elemento matriz[i][j]
    }
}
```

### Formato de Salida Tabular
```c
// Usar tabulaciones para alineación
printf("%d\t", resultado[i][j]);
printf("\n"); // Nueva línea al final de cada fila
```

## Casos Especiales

### Matriz de Ceros (Elemento Neutro)
```
A + 0 = A
[1 2 3]   [0 0 0]   [1 2 3]
[4 5 6] + [0 0 0] = [4 5 6]
[7 8 9]   [0 0 0]   [7 8 9]
```

### Matrices Opuestas
```
A + (-A) = 0
[1 2 3]   [-1 -2 -3]   [0 0 0]
[4 5 6] + [-4 -5 -6] = [0 0 0]
[7 8 9]   [-7 -8 -9]   [0 0 0]
```

### Matriz Identidad
```
I + A (casos especiales con diagonal principal)
[1 0 0]   [a b c]   [1+a  b   c ]
[0 1 0] + [d e f] = [ d  1+e  f ]
[0 0 1]   [g h i]   [ g   h  1+i]
```

## Extensiones y Mejoras Posibles
- **Dimensiones variables**: Permitir matrices de diferentes tamaños
- **Múltiples operaciones**: Resta, multiplicación escalar
- **Validación**: Verificar compatibilidad de dimensiones
- **Entrada desde archivo**: Cargar matrices desde ficheros
- **Multiplicación matricial**: Operación más compleja
- **Determinante y transpuesta**: Operaciones matriciales avanzadas

## Aplicaciones Prácticas
- **Procesamiento de imágenes**: Suma de matrices de píxeles
- **Análisis numérico**: Operaciones en sistemas lineales
- **Gráficos por computadora**: Transformaciones matriciales
- **Estadística**: Operaciones con matrices de datos
- **Simulaciones**: Modelos matemáticos con matrices

## Progresión Educativa
Este ejercicio prepara para:
- **Multiplicación de matrices**: Operación más compleja
- **Arrays dinámicos 2D**: Manejo dinámico de memoria
- **Algoritmos matriciales**: Descomposición LU, etc.
- **Estructuras de datos avanzadas**: Grafos representados como matrices

## Valor Educativo
- **Transición 1D → 2D**: Evolución natural desde arrays simples
- **Bucles anidados**: Patrón fundamental en programación
- **Indexación múltiple**: Concepto clave para estructuras complejas
- **Operaciones matemáticas**: Conexión entre programación y matemáticas
