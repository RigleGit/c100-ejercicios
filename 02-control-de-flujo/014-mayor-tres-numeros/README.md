# Ejercicio 014: Mayor de Tres Números

## Enunciado

Escribe un programa que lea tres números enteros introducidos por el usuario y determine cuál de ellos es el mayor.

## Descripción

Este ejercicio es ideal para reforzar **estructuras condicionales compuestas y encadenadas**, al obligarte a comparar múltiples valores y tomar decisiones basadas en más de una condición. También es útil para empezar a pensar en **estructuras en cascada**, donde una decisión depende de otra anterior.

## Conceptos Matemáticos

Para determinar el mayor de tres números, necesitamos:
- **Comparaciones múltiples**: Cada número debe compararse con los otros dos
- **Operadores lógicos**: Combinar condiciones con AND (`&&`)
- **Precedencia**: El orden de evaluación de las condiciones es importante

### Algoritmo de Comparación

Para tres números `a`, `b`, `c`:
1. Si `a >= b` Y `a >= c` → `a` es el mayor
2. Si no, si `b >= a` Y `b >= c` → `b` es el mayor  
3. Si no → `c` es el mayor

## Conceptos Clave

- **Estructuras condicionales compuestas**: `if-else if-else`
- **Operador lógico AND (`&&`)**: Combina múltiples condiciones
- **Operador mayor o igual (`>=`)**: Maneja casos de igualdad
- **Entrada múltiple**: `scanf` con múltiples especificadores
- **Lógica exhaustiva**: Todas las posibilidades están cubiertas
- **Evaluación en cortocircuito**: Optimización del operador `&&`

## Estructura del Proyecto

```
014-mayor-tres-numeros/
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

### Enfoque de Comparación Exhaustiva

```c
if (a >= b && a >= c) {
    // 'a' es mayor o igual que ambos
} else if (b >= a && b >= c) {
    // 'b' es mayor que 'a' Y mayor o igual que 'c'
} else {
    // 'c' debe ser el mayor (única opción restante)
}
```

### ¿Por qué usar `>=` en lugar de `>`?

- **Manejo de igualdades**: Si varios números son iguales, se selecciona el primero en orden de prioridad
- **Casos de empate**: `a >= b` permite que `a` sea seleccionado cuando `a == b`

## Ejemplo de Ejecución

```
Introduce tres números enteros separados por espacios: 10 5 3
El mayor es: 10
```

```
Introduce tres números enteros separados por espacios: 3 15 7
El mayor es: 15
```

```
Introduce tres números enteros separados por espacios: 4 8 20
El mayor es: 20
```

```
Introduce tres números enteros separados por espacios: 5 5 5
El mayor es: 5
```

```
Introduce tres números enteros separados por espacios: -10 -5 -15
El mayor es: -5
```

## Tabla de Casos

| Caso | a | b | c | Condición Verdadera | Mayor |
|------|---|---|---|-------------------|-------|
| 1 | 10 | 5 | 3 | `a >= b && a >= c` | 10 |
| 2 | 3 | 15 | 7 | `b >= a && b >= c` | 15 |
| 3 | 4 | 8 | 20 | `else` | 20 |
| 4 | 5 | 5 | 5 | `a >= b && a >= c` | 5 |
| 5 | 12 | 12 | 8 | `a >= b && a >= c` | 12 |
| 6 | -10 | -5 | -15 | `b >= a && b >= c` | -5 |

## Conceptos Clave Detallados

### 1. Operador Lógico AND (`&&`)
- **Función**: Ambas condiciones deben ser verdaderas
- **Cortocircuito**: Si la primera es falsa, no evalúa la segunda
- **Sintaxis**: `condicion1 && condicion2`

### 2. Entrada Múltiple con scanf
```c
scanf("%d %d %d", &a, &b, &c);
```
- Lee tres enteros separados por espacios
- Los espacios en la cadena de formato permiten separadores flexibles

### 3. Lógica Exhaustiva
- **Primera condición**: `a` es el mayor
- **Segunda condición**: `b` es el mayor (sabemos que `a` no lo es)
- **Else**: Solo queda que `c` sea el mayor

## Compilación y Ejecución

```bash
# Usando CMake (recomendado)
mkdir build && cd build
cmake ..
make
./mayor_tres_numeros

# Compilación directa
gcc -o mayor_tres_numeros src/main.c
./mayor_tres_numeros
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
- Cada número siendo el mayor
- Números iguales (todos y parciales)
- Números negativos
- Números mixtos (positivos, negativos, cero)
- Números grandes y casos límite
- Formato correcto de entrada y salida

## Errores Comunes

1. **Usar `>` en lugar de `>=`**: No maneja correctamente las igualdades
2. **Condiciones incompletas**: No comparar con todos los números
3. **Lógica incorrecta**: Usar OR (`||`) en lugar de AND (`&&`)
4. **Orden de condiciones**: Poner condiciones menos específicas primero
5. **Formato de entrada**: No manejar correctamente múltiples números

## Casos Especiales

### Números Iguales
- **Todos iguales**: `5 5 5` → El primero (`a`) es seleccionado
- **Dos iguales**: `12 12 8` → El primero de los iguales es seleccionado

### Números Negativos
- **Todos negativos**: `-10 -5 -15` → `-5` es el mayor (menos negativo)
- **Mixtos**: `-3 0 5` → `5` es el mayor

## Extensiones Posibles

1. **Validación de entrada**: Verificar que se introducen exactamente tres números
2. **Encontrar mínimo también**: Mostrar tanto el mayor como el menor
3. **Ordenamiento completo**: Mostrar los tres números ordenados
4. **Múltiples conjuntos**: Procesar varios grupos de tres números
5. **Estadísticas**: Calcular promedio, mediana, etc.
6. **Generalización**: Encontrar el mayor de N números

## Algoritmos Alternativos

### Enfoque con Variables Auxiliares
```c
int mayor = a;
if (b > mayor) mayor = b;
if (c > mayor) mayor = c;
```

### Enfoque con Función max
```c
int mayor = max(a, max(b, c));
```

### Enfoque de Ordenamiento
```c
// Ordenar y tomar el último elemento
```

## Complejidad

- **Tiempo**: O(1) - Número constante de comparaciones
- **Espacio**: O(1) - Espacio constante
- **Comparaciones**: Máximo 4 comparaciones necesarias

## Variantes del Problema

- **Menor de tres números**: Cambiar operadores de comparación
- **Segundo mayor**: Encontrar el segundo número más grande
- **Mediano**: Encontrar el número del medio
- **Mayor de cuatro números**: Extender la lógica
- **Array de números**: Generalizar para N números
