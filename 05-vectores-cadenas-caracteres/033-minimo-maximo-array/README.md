# Ejercicio 033: Mínimo y Máximo de Array

## Descripción
Este programa solicita al usuario que ingrese una cantidad de números enteros, los almacena en un array y determina cuál es el valor mínimo y máximo del conjunto. Demuestra el análisis eficiente de datos mediante un único recorrido del array.

## Enunciado
Escribir un programa que:
1. Solicite al usuario la cantidad de números a ingresar (máximo 100)
2. Lea esos números y los almacene en un array
3. Durante la entrada, identifique simultáneamente el valor mínimo y máximo
4. Muestre ambos valores al final

## Funcionalidades
- **Entrada de datos**: Lectura de cantidad de números y valores
- **Validación**: Verifica que la cantidad esté en el rango válido (1-100)
- **Análisis en tiempo real**: Determina min/max durante la entrada de datos
- **Eficiencia**: Un solo recorrido para encontrar ambos extremos
- **Inicialización inteligente**: Usa el primer elemento como valor inicial

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
./bin/minimo_maximo_array

# O directamente con gcc
gcc -o minimo_maximo_array src/main.c
./minimo_maximo_array
```

### Ejecutar Tests
```bash
# Desde el directorio build
make run_tests

# O ejecutar directamente
./test_033-minimo-maximo-array
```

## Ejemplo de Uso
```
¿Cuántos números vas a introducir (máximo 100)? 5
Introduce el número 1: 15
Introduce el número 2: 8
Introduce el número 3: 23
Introduce el número 4: 4
Introduce el número 5: 12
Valor mínimo: 4
Valor máximo: 23
```

## Conceptos Demostrados
- **Arrays**: Almacenamiento y procesamiento de conjuntos de datos
- **Comparaciones condicionales**: Uso de if para comparar valores
- **Inicialización**: Técnica de usar el primer elemento como referencia
- **Análisis de datos**: Extracción de información estadística básica
- **Eficiencia algorítmica**: Procesamiento en una sola pasada
- **Validación de entrada**: Verificación de rangos válidos

## Estructura del Proyecto
```
033-minimo-maximo-array/
├── src/
│   └── main.c          # Código principal
├── test/
│   └── test_main.c     # Tests unitarios
├── include/            # Archivos de cabecera (si los hay)
├── CMakeLists.txt      # Configuración de CMake
├── .gitignore         # Archivos a ignorar por git
└── README.md          # Este archivo
```

## Casos de Prueba
Los tests incluyen verificaciones para:
- **Secuencias ordenadas**: Ascendente y descendente
- **Un solo elemento**: Min y max son el mismo valor
- **Números negativos**: Manejo correcto de valores negativos
- **Números mixtos**: Combinación de positivos, negativos y cero
- **Extremos en diferentes posiciones**: Inicio, medio, final
- **Valores iguales**: Array con todos los elementos idénticos
- **Números grandes**: Verificación con valores extremos
- **Validación**: Cantidades inválidas (0, >100)

## Aspectos Técnicos Importantes

### Algoritmo de Inicialización
```c
if (i == 0) {
    min = max = numeros[i];
} else {
    if (numeros[i] < min) min = numeros[i];
    if (numeros[i] > max) max = numeros[i];
}
```

**Ventajas de esta aproximación:**
- **Eficiencia**: Solo un recorrido del array
- **Simplicidad**: No requiere valores iniciales arbitrarios
- **Robustez**: Funciona con cualquier rango de valores
- **Claridad**: Lógica fácil de entender y mantener

### Consideraciones Especiales
- **Un elemento**: min = max = único valor
- **Números negativos**: Se manejan correctamente
- **Valores extremos**: INT_MIN y INT_MAX soportados
- **Inicialización segura**: No depende de constantes arbitrarias

### Alternativas de Implementación
```c
// Alternativa menos eficiente (dos pasadas)
// 1. Llenar array completo
// 2. Recorrer para encontrar min/max

// Alternativa con valores iniciales problemáticos
// min = INT_MAX; max = INT_MIN; // Requiere limits.h
```

## Casos Especiales
- **Array con un elemento**: min = max = elemento
- **Todos elementos iguales**: min = max = valor común
- **Números negativos**: mínimo más negativo, máximo menos negativo
- **Incluye cero**: se maneja como cualquier otro número

## Optimizaciones Posibles
- **Comparación simultánea**: Reducir número de comparaciones
- **Uso de estructuras**: Struct para retornar min/max
- **Búsqueda de índices**: Retornar posiciones además de valores
- **Múltiples estadísticas**: Calcular mediana, moda, etc.

## Aplicaciones Prácticas
- **Análisis de datos**: Rango de valores en datasets
- **Validación**: Verificar límites de entrada
- **Normalización**: Escalado de valores a rangos específicos
- **Estadísticas**: Base para cálculos más complejos

## Mejoras Posibles
- Encontrar las posiciones de min/max además de los valores
- Manejo de múltiples mínimos/máximos
- Soporte para números de punto flotante
- Entrada desde archivo
- Interfaz más robusta con manejo de errores avanzado
