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

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/minimo_maximo_array
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
