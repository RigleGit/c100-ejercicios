# Ejercicio 034: Búsqueda Lineal

## Descripción
Este programa implementa el algoritmo de búsqueda lineal (búsqueda secuencial) para encontrar un valor específico dentro de un array de números enteros. El programa muestra la posición donde se encuentra el valor buscado o indica si no está presente.

## Enunciado
Escribir un programa que:
1. Solicite al usuario la cantidad de números a ingresar (máximo 100)
2. Lea esos números y los almacene en un array
3. Solicite al usuario el valor que desea buscar
4. Implemente búsqueda lineal para encontrar el valor
5. Muestre la posición (1-indexada) e índice (0-indexado) si lo encuentra
6. Muestre un mensaje apropiado si no lo encuentra

## Funcionalidades
- **Entrada de datos**: Lectura de array y valor a buscar
- **Búsqueda secuencial**: Recorrido elemento por elemento
- **Detección temprana**: Uso de `break` al encontrar el valor
- **Información detallada**: Muestra posición e índice
- **Manejo de casos**: Valor encontrado vs. no encontrado

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
./bin/busqueda_lineal

# O directamente con gcc
gcc -o busqueda_lineal src/main.c
./busqueda_lineal
```

### Ejecutar Tests
```bash
# Desde el directorio build
make run_tests

# O ejecutar directamente
./test_034-busqueda-lineal
```

## Ejemplo de Uso

### Caso: Valor Encontrado
```
¿Cuántos números vas a introducir (máximo 100)? 5
Introduce el número 1: 10
Introduce el número 2: 25
Introduce el número 3: 30
Introduce el número 4: 15
Introduce el número 5: 40
Introduce el valor que deseas buscar: 30
Valor encontrado en la posición 3 (índice 2).
```

### Caso: Valor No Encontrado
```
¿Cuántos números vas a introducir (máximo 100)? 3
Introduce el número 1: 5
Introduce el número 2: 10
Introduce el número 3: 15
Introduce el valor que deseas buscar: 99
Valor no encontrado en el array.
```

## Conceptos Demostrados
- **Algoritmo de búsqueda**: Implementación de búsqueda lineal
- **Recorrido secuencial**: Examen elemento por elemento
- **Control de flujo**: Uso de `break` para salida temprana
- **Variables de estado**: Flag `encontrado` para control lógico
- **Indexación**: Diferencia entre posición (1-based) e índice (0-based)
- **Manejo de casos**: Lógica para encontrado/no encontrado

## Estructura del Proyecto
```
034-busqueda-lineal/
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
- **Posiciones específicas**: Primer elemento, último, elemento medio
- **Casos especiales**: Array de un elemento, todos elementos iguales
- **Valores no encontrados**: Búsqueda de valores ausentes
- **Números especiales**: Negativos, cero, números grandes
- **Valores repetidos**: Primera ocurrencia de elementos duplicados
- **Validación**: Cantidades inválidas (0, >100)

## Análisis del Algoritmo

### Implementación Core
```c
for (int i = 0; i < n; i++) {
    if (numeros[i] == valor) {
        printf("Valor encontrado en la posición %d (índice %d).\n", i + 1, i);
        encontrado = 1;
        break;  // Salida temprana: importante para eficiencia
    }
}
```

### Características Algorítmicas
- **Complejidad temporal**: O(n) en el peor caso, O(1) en el mejor caso
- **Complejidad espacial**: O(1) - solo variables auxiliares
- **Tipo de búsqueda**: Secuencial, no requiere orden previo
- **Optimización**: Termina inmediatamente al encontrar el valor

### Ventajas de la Búsqueda Lineal
- ✅ **Simplicidad**: Fácil de entender e implementar
- ✅ **No requiere orden**: Funciona con datos no ordenados
- ✅ **Pequeños datasets**: Eficiente para arrays pequeños
- ✅ **Primera ocurrencia**: Encuentra la primera aparición del valor

### Limitaciones
- ❌ **Eficiencia**: O(n) puede ser lento para grandes datasets
- ❌ **No aprovecha orden**: No optimiza si los datos están ordenados

## Casos Especiales Importantes

### Primera Ocurrencia
Cuando hay valores repetidos, el algoritmo encuentra la **primera ocurrencia**:
```
Array: [1, 2, 3, 2, 4, 2]
Buscar: 2
Resultado: Posición 2 (índice 1) ← Primera aparición
```

### Array de Un Elemento
```c
// Caso límite que debe manejarse correctamente
Array: [42]
Buscar: 42 → Encontrado en posición 1
Buscar: 99 → No encontrado
```

### Importancia del `break`
Sin el `break`, el programa seguiría buscando innecesariamente:
```c
// Sin break: O(n) siempre
// Con break: O(1) mejor caso, O(n) peor caso
```

## Variaciones y Mejoras Posibles
- **Búsqueda de todas las ocurrencias**: Encontrar todas las posiciones
- **Búsqueda con contador**: Contar cuántas veces aparece el valor
- **Búsqueda binaria**: Para arrays ordenados (O(log n))
- **Búsqueda con predicado**: Búsqueda por condición, no igualdad exacta
- **Interfaz más robusta**: Múltiples búsquedas en una ejecución

## Aplicaciones Prácticas
- **Validación de datos**: Verificar si un valor existe
- **Sistemas de inventario**: Buscar productos por código
- **Bases de datos simples**: Búsqueda en listas no indexadas
- **Depuración**: Verificar contenido de estructuras de datos

## Relación con Otros Algoritmos
- **Predecesor de**: Búsqueda binaria, hash tables
- **Fundamento para**: Algoritmos de ordenamiento que incluyen búsqueda
- **Complementa a**: Algoritmos de inserción y eliminación en arrays
