# Ejercicio 024: Función MCD (Máximo Común Divisor)

## Enunciado

Escribe una función `int mcd(int a, int b)` que calcule el Máximo Común Divisor de dos números enteros positivos utilizando el algoritmo de Euclides, y luego úsala desde `main` para mostrar el resultado de dos valores introducidos por el usuario.

## Descripción

El programa debe:

1. Implementar el algoritmo de Euclides para calcular el MCD
2. Usar recursividad para aplicar el algoritmo
3. En `main`, solicitar dos números al usuario
4. Calcular y mostrar el MCD de los números ingresados
5. Demostrar la eficiencia del algoritmo clásico

### Ejemplo de ejecución:

```
Introduce el primer número: 48
Introduce el segundo número: 18
El MCD de 48 y 18 es: 6
```

```
Introduce el primer número: 17
Introduce el segundo número: 13
El MCD de 17 y 13 es: 1
```

```
Introduce el primer número: 100
Introduce el segundo número: 25
El MCD de 100 y 25 es: 25
```

## Conceptos clave

- **Algoritmo de Euclides**: Método eficiente para calcular el MCD
- **Recursividad aplicada**: Uso natural de recursión en algoritmo matemático
- **Máximo Común Divisor**: Mayor número que divide exactamente a dos números
- **Eficiencia algorítmica**: Complejidad logarítmica O(log min(a,b))
- **Matemáticas discretas**: Aplicación de teoría de números
- **Casos base**: Condición de parada en recursión (b = 0)

## ¿Qué es el Máximo Común Divisor (MCD)?

El **MCD** de dos números enteros es el mayor número entero positivo que divide exactamente a ambos números.

### Definición matemática:
```
mcd(a, b) = max{d ∈ ℕ : d|a ∧ d|b}
```

### Ejemplos:
- **mcd(12, 18) = 6**: Los divisores de 12 son {1,2,3,4,6,12}, los de 18 son {1,2,3,6,9,18}
- **mcd(17, 13) = 1**: 17 y 13 son números primos (coprimos)
- **mcd(48, 18) = 6**: 48 = 6×8, 18 = 6×3

## El Algoritmo de Euclides

### Historia:
Descrito por Euclides en "Los Elementos" (~300 a.C.), es uno de los algoritmos más antiguos conocidos que aún se usa.

### Principio fundamental:
```
mcd(a, b) = mcd(b, a mod b)
```

### Demostración:
Si **d** divide a **a** y **b**, entonces también divide a **a - kb** para cualquier entero **k**.
Como **a mod b = a - kb** (donde k = ⌊a/b⌋), entonces **d** también divide a **a mod b**.

## Implementaciones del Algoritmo

### 1. Versión recursiva (elegante):
```c
int mcd(int a, int b) {
    if (b == 0)           // Caso base
        return a;
    return mcd(b, a % b); // Caso recursivo
}
```

### 2. Versión iterativa (eficiente en memoria):
```c
int mcd_iterativo(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
```

### 3. Versión con manejo de negativos:
```c
int mcd_absoluto(int a, int b) {
    a = (a < 0) ? -a : a;  // Valor absoluto
    b = (b < 0) ? -b : b;
    return mcd(a, b);
}
```

## Traza del Algoritmo

### Ejemplo: mcd(48, 18)

| Paso | a | b | a mod b | Operación |
|------|---|---|---------|-----------|
| 1 | 48 | 18 | 12 | 48 = 18×2 + 12 |
| 2 | 18 | 12 | 6 | 18 = 12×1 + 6 |
| 3 | 12 | 6 | 0 | 12 = 6×2 + 0 |
| 4 | 6 | 0 | - | **Resultado: 6** |

### Llamadas recursivas:
```
mcd(48, 18)
├── mcd(18, 12)
│   ├── mcd(12, 6)
│   │   ├── mcd(6, 0)
│   │   │   └── return 6
│   │   └── return 6
│   └── return 6
└── return 6
```

## Análisis de Complejidad

### Complejidad temporal:
- **Peor caso**: O(log min(a, b))
- **Caso promedio**: O(log min(a, b))
- **Mejor caso**: O(1) cuando b = 0

### Complejidad espacial:
- **Recursivo**: O(log min(a, b)) - por la pila de llamadas
- **Iterativo**: O(1) - memoria constante

### ¿Por qué es tan eficiente?

El algoritmo reduce el problema a la mitad (o más) en cada paso:
- Si **b > a/2**, entonces **a mod b < a/2**
- Si **b ≤ a/2**, entonces **b < a/2**

Por tanto, en máximo 2 pasos, el mayor de los números se reduce a la mitad.

## Propiedades Matemáticas del MCD

### 1. **Conmutativa**:
```
mcd(a, b) = mcd(b, a)
```

### 2. **Asociativa**:
```
mcd(mcd(a, b), c) = mcd(a, mcd(b, c))
```

### 3. **Distributiva**:
```
mcd(ka, kb) = k × mcd(a, b)
```

### 4. **Relación con MCM**:
```
mcd(a, b) × mcm(a, b) = a × b
```

### 5. **Identidad**:
```
mcd(a, 0) = a
mcd(a, 1) = 1
```

## Aplicaciones del MCD

### 1. **Simplificación de fracciones**:
```c
void simplificar_fraccion(int numerador, int denominador) {
    int gcd = mcd(numerador, denominador);
    printf("%d/%d = %d/%d\n", 
           numerador, denominador,
           numerador/gcd, denominador/gcd);
}
```

### 2. **Cálculo del MCM (Mínimo Común Múltiplo)**:
```c
int mcm(int a, int b) {
    return (a * b) / mcd(a, b);
}
```

### 3. **Verificar números coprimos**:
```c
int son_coprimos(int a, int b) {
    return mcd(a, b) == 1;
}
```

### 4. **Algoritmo extendido de Euclides**:
```c
// Encuentra x, y tal que ax + by = mcd(a, b)
int mcd_extendido(int a, int b, int* x, int* y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    
    int x1, y1;
    int gcd = mcd_extendido(b, a % b, &x1, &y1);
    
    *x = y1;
    *y = x1 - (a / b) * y1;
    
    return gcd;
}
```

## Casos Especiales

### 1. **Números coprimos**:
```c
mcd(7, 11) = 1    // Números primos
mcd(15, 28) = 1   // Sin factores comunes
```

### 2. **Uno divide al otro**:
```c
mcd(12, 3) = 3    // 3 divide a 12
mcd(100, 25) = 25 // 25 divide a 100
```

### 3. **Números iguales**:
```c
mcd(7, 7) = 7     // El número mismo
```

### 4. **Con cero**:
```c
mcd(a, 0) = a     // Por definición
mcd(0, b) = b     // Por definición
```

## Optimizaciones y Variantes

### 1. **MCD de múltiples números**:
```c
int mcd_multiple(int arr[], int n) {
    int resultado = arr[0];
    for (int i = 1; i < n; i++) {
        resultado = mcd(resultado, arr[i]);
    }
    return resultado;
}
```

### 2. **MCD binario (algoritmo de Stein)**:
```c
int mcd_binario(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    
    // Ambos son pares
    if ((a & 1) == 0 && (b & 1) == 0) {
        return mcd_binario(a >> 1, b >> 1) << 1;
    }
    
    // a es par, b es impar
    if ((a & 1) == 0) {
        return mcd_binario(a >> 1, b);
    }
    
    // a es impar, b es par
    if ((b & 1) == 0) {
        return mcd_binario(a, b >> 1);
    }
    
    // Ambos son impares
    if (a > b) {
        return mcd_binario((a - b) >> 1, b);
    } else {
        return mcd_binario((b - a) >> 1, a);
    }
}
```

## Relación con la Secuencia de Fibonacci

### Caso peor del algoritmo:
Los números consecutivos de Fibonacci proporcionan el **peor caso** para el algoritmo de Euclides:

```c
// Fibonacci: 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144...
mcd(89, 55) // Requiere más pasos que cualquier otro par de números de tamaño similar
```

### Teorema de Lamé:
El número de pasos del algoritmo de Euclides para mcd(a,b) es a lo sumo **5 veces** el número de dígitos del menor de los dos números.

## Aplicaciones en Criptografía

### RSA y números coprimos:
```c
// En RSA, e y φ(n) deben ser coprimos
if (mcd(e, phi_n) == 1) {
    printf("e es válido para RSA\n");
}
```

### Generación de claves:
```c
// Encontrar e tal que mcd(e, φ(n)) = 1
int encontrar_e(int phi_n) {
    for (int e = 3; e < phi_n; e += 2) {
        if (mcd(e, phi_n) == 1) {
            return e;
        }
    }
    return -1; // Error
}
```

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./funcion_mcd

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Casos básicos (12,18), (48,18), (100,25)
- ✅ Casos especiales (con 0, con 1, números iguales)
- ✅ Números primos y coprimos
- ✅ Comparación recursivo vs iterativo
- ✅ Manejo de números negativos
- ✅ Propiedades matemáticas (conmutativa, divisibilidad)
- ✅ Cálculo de MCM usando MCD
- ✅ Algoritmo extendido de Euclides
- ✅ Conteo de pasos del algoritmo
- ✅ Verificación de números coprimos
- ✅ Casos con números grandes
- ✅ Secuencia de Fibonacci (caso peor)
- ✅ Casos límite y eficiencia
- ✅ Consistencia del algoritmo

## Posibles extensiones

1. **Calculadora de fracciones**: Simplificación automática usando MCD
2. **MCD de múltiples números**: Extensión a arrays de números
3. **Visualización del algoritmo**: Mostrar todos los pasos
4. **Análisis de rendimiento**: Comparar diferentes implementaciones
5. **Aplicaciones criptográficas**: Generador de claves RSA
6. **MCD modular**: Para aritmética modular
7. **Interfaz gráfica**: Visualización del proceso paso a paso
8. **MCD de polinomios**: Extensión a estructuras algebraicas

## Ejemplo de extensión - Calculadora de fracciones:

```c
typedef struct {
    int numerador;
    int denominador;
} Fraccion;

Fraccion simplificar(Fraccion f) {
    int gcd = mcd_absoluto(f.numerador, f.denominador);
    f.numerador /= gcd;
    f.denominador /= gcd;
    
    // Mover signo al numerador
    if (f.denominador < 0) {
        f.numerador = -f.numerador;
        f.denominador = -f.denominador;
    }
    
    return f;
}

Fraccion sumar_fracciones(Fraccion a, Fraccion b) {
    Fraccion resultado;
    resultado.numerador = a.numerador * b.denominador + b.numerador * a.denominador;
    resultado.denominador = a.denominador * b.denominador;
    return simplificar(resultado);
}
```

## Notas de implementación

- Implementación clásica del algoritmo más famoso de la matemática
- Excelente ejemplo de recursividad con propósito matemático
- Demuestra la elegancia y eficiencia de algoritmos antiguos
- Base fundamental para muchas aplicaciones en ciencias de la computación
- Las pruebas cubren tanto aspectos teóricos como prácticos
- Algoritmo esencial en criptografía, álgebra y teoría de números
