# Ejercicio 021: Función Máximo de Dos Números

## Enunciado

Escribe un programa en C que defina una función `int max(int a, int b)` que reciba dos enteros y devuelva el mayor de ambos. Usa esa función desde `main` para mostrar el resultado de comparar dos números introducidos por el usuario.

## Descripción

El programa debe:

1. Definir una función `max` que compare dos números enteros
2. La función debe retornar el mayor de los dos números
3. En `main`, solicitar dos números al usuario
4. Llamar a la función `max` con esos números
5. Mostrar el resultado al usuario

### Ejemplo de ejecución:

```
Introduce el primer número: 15
Introduce el segundo número: 8
El número mayor es: 15
```

```
Introduce el primer número: -5
Introduce el segundo número: -2
El número mayor es: -2
```

```
Introduce el primer número: 7
Introduce el segundo número: 7
El número mayor es: 7
```

## Conceptos clave

- **Funciones**: Bloques de código reutilizable que realizan una tarea específica
- **Modularidad**: Separación de la lógica en unidades independientes
- **Parámetros**: Valores que se pasan a una función
- **Valor de retorno**: Resultado que devuelve una función
- **Declaración vs Definición**: Diferencia entre prototipo y implementación
- **Encapsulación**: Ocultar la implementación detrás de una interfaz

## Anatomía de una función en C

### Sintaxis completa:
```c
tipo_retorno nombre_funcion(tipo_param1 param1, tipo_param2 param2) {
    // Cuerpo de la función
    return valor;
}
```

### Ejemplo detallado:
```c
int max(int a, int b) {
//│   │   │     │   └── Parámetro 2
//│   │   │     └────── Parámetro 1  
//│   │   └──────────── Tipo de parámetros
//│   └──────────────── Nombre de la función
//└──────────────────── Tipo de retorno

    if (a > b) {
        return a;     // Retorna el primer parámetro
    } else {
        return b;     // Retorna el segundo parámetro
    }
}
```

## Partes de una función

| Componente | Descripción | Ejemplo |
|------------|-------------|---------|
| **Tipo de retorno** | Tipo de dato que devuelve | `int`, `float`, `void` |
| **Nombre** | Identificador de la función | `max`, `calcular`, `imprimir` |
| **Parámetros** | Datos de entrada | `(int a, int b)` |
| **Cuerpo** | Implementación de la lógica | `{ ... }` |
| **Return** | Valor devuelto | `return a;` |

## Ventajas de usar funciones

### 1. **Reutilización de código**:
```c
int resultado1 = max(10, 5);    // Uso 1
int resultado2 = max(-3, -7);   // Uso 2
int resultado3 = max(x, y);     // Uso 3
```

### 2. **Legibilidad mejorada**:
```c
// Sin función (menos claro)
if (temperatura1 > temperatura2) {
    temp_maxima = temperatura1;
} else {
    temp_maxima = temperatura2;
}

// Con función (más claro)
temp_maxima = max(temperatura1, temperatura2);
```

### 3. **Facilidad de mantenimiento**:
```c
// Si necesitas cambiar la lógica, solo modificas un lugar
int max(int a, int b) {
    // Nueva implementación mejorada
    return (a > b) ? a : b;  // Operador ternario
}
```

### 4. **Testeo independiente**:
```c
// Puedes probar la función por separado
assert(max(5, 3) == 5);
assert(max(-1, -2) == -1);
```

## Comparación: Con y sin funciones

### ❌ Sin funciones (código repetitivo):
```c
int main() {
    int x, y, z;
    
    // Encontrar máximo de x, y
    int max_xy;
    if (x > y) {
        max_xy = x;
    } else {
        max_xy = y;
    }
    
    // Encontrar máximo de max_xy, z
    int max_xyz;
    if (max_xy > z) {
        max_xyz = max_xy;
    } else {
        max_xyz = z;
    }
    
    return 0;
}
```

### ✅ Con funciones (código limpio):
```c
int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    int x, y, z;
    int max_xyz = max(max(x, y), z);
    return 0;
}
```

## Variaciones y extensiones

### 1. **Función máximo con operador ternario**:
```c
int max(int a, int b) {
    return (a > b) ? a : b;
}
```

### 2. **Función máximo de tres números**:
```c
int max_tres(int a, int b, int c) {
    return max(max(a, b), c);
}
```

### 3. **Función máximo genérica con macro**:
```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
```

### 4. **Función que también devuelve el mínimo**:
```c
void max_min(int a, int b, int* maximo, int* minimo) {
    if (a > b) {
        *maximo = a;
        *minimo = b;
    } else {
        *maximo = b;
        *minimo = a;
    }
}
```

## Prototipos de función

### ¿Cuándo usar prototipos?

```c
// Prototipo (declaración)
int max(int a, int b);

int main() {
    int resultado = max(10, 5);  // Uso antes de la definición
    return 0;
}

// Definición (implementación)
int max(int a, int b) {
    return (a > b) ? a : b;
}
```

### Reglas de prototipos:
- Necesarios cuando la función se usa antes de definirse
- Deben coincidir exactamente con la definición
- Permiten organizar mejor el código

## Propiedades matemáticas de max

| Propiedad | Fórmula | Ejemplo |
|-----------|---------|---------|
| **Conmutativa** | max(a, b) = max(b, a) | max(5, 3) = max(3, 5) = 5 |
| **Asociativa** | max(max(a, b), c) = max(a, max(b, c)) | max(max(1, 2), 3) = max(1, max(2, 3)) |
| **Idempotente** | max(a, a) = a | max(7, 7) = 7 |
| **Identidad** | max(a, -∞) = a | max(a, INT_MIN) ≈ a |

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./funcion_maximo_dos

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Casos básicos (números positivos, iguales)
- ✅ Números negativos
- ✅ Mezcla de positivos y negativos
- ✅ Números grandes y valores extremos
- ✅ Propiedades matemáticas (conmutativa, idempotente)
- ✅ Combinaciones con función min
- ✅ Extensión a tres números
- ✅ Secuencias de números
- ✅ Coherencia en diferentes órdenes

## Posibles extensiones

1. **Máximo de N números**: Función que acepte un array
2. **Máximo con diferentes tipos**: float, double, long
3. **Máximo con condiciones**: Ignorar números negativos
4. **Estadísticas completas**: Máximo, mínimo, promedio
5. **Versión recursiva**: Implementación con recursión
6. **Máximo de cadenas**: Comparación lexicográfica
7. **Interfaz mejorada**: Menú para múltiples comparaciones
8. **Máximo con metadatos**: Devolver también la posición

## Ejemplo de extensión - Máximo de array:

```c
int max_array(int arr[], int size) {
    if (size <= 0) return INT_MIN;  // Caso de error
    
    int maximo = arr[0];
    for (int i = 1; i < size; i++) {
        maximo = max(maximo, arr[i]);
    }
    return maximo;
}

// Uso:
int numeros[] = {3, 7, 1, 9, 2};
int size = sizeof(numeros) / sizeof(numeros[0]);
int resultado = max_array(numeros, size);  // resultado = 9
```

## Buenas prácticas

1. **Nombres descriptivos**: `max` es claro y conciso
2. **Funciones pequeñas**: Una responsabilidad por función
3. **Parámetros inmutables**: No modificar los parámetros de entrada
4. **Manejo de casos especiales**: Considerar valores extremos
5. **Documentación**: Comentarios claros sobre el propósito
6. **Pruebas exhaustivas**: Verificar todos los casos posibles

## Notas de implementación

- Primera introducción al concepto de funciones en C
- Ejemplo perfecto de modularidad y reutilización
- Base para funciones más complejas
- Demuestra separación de responsabilidades
- Las pruebas cubren propiedades matemáticas importantes
- Excelente ejercicio para entender parámetros y valores de retorno
