# Ejercicio 035: Ordenamiento Burbuja

## Descripción
Este programa implementa el algoritmo de ordenamiento burbuja (bubble sort) para ordenar un array de números enteros de menor a mayor. Es uno de los algoritmos de ordenamiento más simples de entender e implementar, aunque no es eficiente para grandes volúmenes de datos.

## Enunciado
Escribir un programa que:
1. Solicite al usuario la cantidad de números a ingresar (máximo 100)
2. Lea esos números y los almacene en un array
3. Implemente el algoritmo de ordenamiento burbuja
4. Muestre el array ordenado de menor a mayor

## Funcionalidades
- **Entrada de datos**: Lectura de array de números enteros
- **Ordenamiento burbuja**: Implementación del algoritmo clásico
- **Intercambio de elementos**: Técnica de swap para reordenar
- **Optimización**: Reducción del rango en cada iteración
- **Salida formateada**: Muestra del array ordenado

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
./bin/ordenar_array_burbuja

# O directamente con gcc
gcc -o ordenar_array_burbuja src/main.c
./ordenar_array_burbuja
```

### Ejecutar Tests
```bash
# Desde el directorio build
make run_tests

# O ejecutar directamente
./test_035-ordenar-array-burbuja
```

## Ejemplo de Uso
```
¿Cuántos números quieres ordenar (máximo 100)? 5
Introduce el número 1: 64
Introduce el número 2: 25
Introduce el número 3: 12
Introduce el número 4: 22
Introduce el número 5: 11
Array ordenado:
11 12 22 25 64
```

## Conceptos Demostrados
- **Algoritmos de ordenamiento**: Implementación de bubble sort
- **Bucles anidados**: Estructura de doble loop para comparaciones
- **Intercambio de variables**: Técnica de swap con variable temporal
- **Optimización básica**: Reducción del rango de comparación
- **Análisis algorítmico**: Comprensión de complejidad temporal

## Estructura del Proyecto
```
035-ordenar-array-burbuja/
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

### Implementación Core
```c
// Algoritmo de ordenamiento burbuja
for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++) {
        if (numeros[j] > numeros[j + 1]) {
            // Intercambio (swap)
            temp = numeros[j];
            numeros[j] = numeros[j + 1];
            numeros[j + 1] = temp;
        }
    }
}
```

### Funcionamiento del Algoritmo
1. **Comparación adyacente**: Compara elementos consecutivos
2. **Intercambio condicional**: Swap si están en orden incorrecto
3. **Burbujeo**: El elemento mayor "burbujea" hacia el final
4. **Reducción progresiva**: Cada pasada reduce el rango a revisar
5. **Terminación**: Cuando no hay más intercambios necesarios

### Características Algorítmicas
- **Complejidad temporal**: O(n²) en todos los casos
- **Complejidad espacial**: O(1) - ordenamiento in-place
- **Estabilidad**: Estable (mantiene orden relativo de elementos iguales)
- **Adaptabilidad**: No adaptativo (mismo tiempo para cualquier entrada)

## Análisis de Complejidad

### Mejor Caso: O(n²)
- Array ya ordenado: [1, 2, 3, 4, 5]
- Aún realiza todas las comparaciones
- Sin intercambios, pero tiempo constante

### Caso Promedio: O(n²)
- Array parcialmente ordenado
- Aproximadamente n²/4 intercambios

### Peor Caso: O(n²)
- Array en orden inverso: [5, 4, 3, 2, 1]
- Máximo número de comparaciones e intercambios
- n²/2 intercambios

### Optimización Implementada
```c
for (int j = 0; j < n - 1 - i; j++)
//                      ^^^^
//              Reducción progresiva
```
- Cada iteración externa coloca un elemento en su posición final
- Reduce comparaciones innecesarias en elementos ya ordenados

## Casos de Prueba
Los tests incluyen verificaciones para:
- **Arrays básicos**: Desordenado, ya ordenado, orden inverso
- **Casos límite**: Un elemento, dos elementos, todos iguales
- **Datos especiales**: Números negativos, ceros, duplicados
- **Casos extremos**: Números grandes, secuencias aleatorias
- **Validación**: Cantidades inválidas (0, >100)

## Casos Especiales Importantes

### Array Ya Ordenado
```
Entrada: [1, 2, 3, 4]
Resultado: [1, 2, 3, 4]
Comportamiento: Sin intercambios, pero O(n²) comparaciones
```

### Array en Orden Inverso (Peor Caso)
```
Entrada: [4, 3, 2, 1]
Resultado: [1, 2, 3, 4]
Comportamiento: Máximo número de intercambios
```

### Elementos Duplicados
```
Entrada: [3, 1, 3, 2, 1, 2]
Resultado: [1, 1, 2, 2, 3, 3]
Comportamiento: Ordenamiento estable preserva orden relativo
```

## Ventajas y Desventajas

### ✅ Ventajas
- **Simplicidad**: Muy fácil de entender e implementar
- **In-place**: No requiere memoria adicional
- **Estable**: Mantiene orden relativo de elementos iguales
- **Detección**: Puede detectar si el array ya está ordenado

### ❌ Desventajas
- **Ineficiencia**: O(n²) es lento para datasets grandes
- **No adaptativo**: Mismo tiempo sin importar el estado inicial
- **Muchos intercambios**: Más movimientos que otros algoritmos

## Comparación con Otros Algoritmos

| Algoritmo | Mejor Caso | Caso Promedio | Peor Caso | Espacio |
|-----------|------------|---------------|-----------|---------|
| Burbuja   | O(n²)      | O(n²)         | O(n²)     | O(1)    |
| Selección | O(n²)      | O(n²)         | O(n²)     | O(1)    |
| Inserción | O(n)       | O(n²)         | O(n²)     | O(1)    |
| Quick Sort| O(n log n) | O(n log n)    | O(n²)     | O(log n)|
| Merge Sort| O(n log n) | O(n log n)    | O(n log n)| O(n)    |

## Optimizaciones Posibles

### 1. Detección Temprana
```c
int intercambio_realizado = 0;
// Si no hay intercambios en una pasada, está ordenado
```

### 2. Burbuja Bidireccional (Cocktail Sort)
```c
// Alterna entre ordenar hacia arriba y hacia abajo
// Mejora ligeramente el rendimiento en algunos casos
```

### 3. Optimización de Límites
```c
// Recordar la última posición donde se hizo intercambio
// Reducir aún más el rango de comparación
```

## Aplicaciones Educativas
- **Introducción a algoritmos**: Primer contacto con ordenamiento
- **Análisis de complejidad**: Ejemplo clásico de O(n²)
- **Técnicas de intercambio**: Fundamento para otros algoritmos
- **Optimización**: Base para entender mejoras algorítmicas

## Valor Histórico y Educativo
Aunque bubble sort no es práctico para uso real, es invaluable para:
- Enseñar conceptos fundamentales de ordenamiento
- Demostrar la importancia de la eficiencia algorítmica
- Introducir el análisis de complejidad temporal
- Preparar para algoritmos más sofisticados

## Mejoras y Extensiones Posibles
- Implementar versión optimizada con detección temprana
- Agregar contador de intercambios y comparaciones
- Visualización paso a paso del proceso de ordenamiento
- Soporte para ordenamiento descendente
- Implementación genérica para diferentes tipos de datos
