# Ejercicio 025: Función Potencia

## Objetivo
Implementar una función que calcule la potencia de un número (base^exponente) sin usar la función `pow()` de la biblioteca `<math.h>`. La función debe manejar exponentes enteros positivos, negativos y cero, trabajando con números de tipo `double`.

## Problema
Crea una función `double potencia(double base, int exp)` que:
- Calcule base^exp sin usar bibliotecas externas
- Maneje correctamente exponentes positivos, negativos y cero
- Trabaje con números reales (tipo `double`)
- Sea implementada de forma iterativa y eficiente
- Incluya manejo de casos especiales

## Código de Ejemplo

### Entrada
```
Base: 2.0, Exponente: 3   → 8.0
Base: 4.0, Exponente: -2  → 0.25
Base: 5.0, Exponente: 0   → 1.0
Base: 1.5, Exponente: 2   → 2.25
```

### Salida Esperada
```
Introduce la base: 2.0
Introduce el exponente (entero): 3
2.00 elevado a 3 es: 8.00000

Introduce la base: 4.0
Introduce el exponente (entero): -2
4.00 elevado a -2 es: 0.25000

Introduce la base: 5.0
Introduce el exponente (entero): 0
5.00 elevado a 0 es: 1.00000
```

## Conceptos Clave

### 1. **Operaciones con Punto Flotante**
- **Tipo `double`**: Precisión doble para números reales
- **Multiplicación acumulativa**: Construcción del resultado por pasos
- **División para exponentes negativos**: Manejo de potencias inversas

### 2. **Casos Especiales de Potencias**
- **Exponente 0**: Cualquier número^0 = 1 (excepto 0^0 que es indefinido)
- **Exponente 1**: Cualquier número^1 = número
- **Exponente negativo**: base^(-n) = 1 / (base^n)
- **Base negativa**: Resultado depende de si el exponente es par o impar

### 3. **Eficiencia Algorítmica**
- **Iterativo**: O(|exponente|) - simple y directo
- **Recursivo**: O(|exponente|) con stack overhead
- **Exponenciación rápida**: O(log|exponente|) - optimal

## Análisis Matemático

### Propiedades de las Potencias
```
1. a^0 = 1 (para a ≠ 0)
2. a^1 = a
3. a^(-n) = 1 / a^n
4. a^m × a^n = a^(m+n)
5. (a^m)^n = a^(m×n)
6. (a×b)^n = a^n × b^n
```

### Casos Especiales
```
• Base = 0: 0^n = 0 (para n > 0), 0^0 indefinido
• Base = 1: 1^n = 1 (para cualquier n)
• Base = -1: (-1)^n = 1 si n es par, -1 si n es impar
• Exponente negativo: Requiere división por la potencia positiva
```

## Implementaciones Alternativas

### 1. **Versión Básica (Iterativa)**
```c
double potencia_basica(double base, int exp) {
    if (exp == 0) return 1.0;
    
    double resultado = 1.0;
    int exp_abs = exp < 0 ? -exp : exp;
    
    for (int i = 0; i < exp_abs; i++) {
        resultado *= base;
    }
    
    return exp < 0 ? 1.0 / resultado : resultado;
}
```

### 2. **Versión Recursiva**
```c
double potencia_recursiva(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp == 1) return base;
    if (exp == -1) return 1.0 / base;
    
    if (exp > 0) {
        return base * potencia_recursiva(base, exp - 1);
    } else {
        return (1.0 / base) * potencia_recursiva(base, exp + 1);
    }
}
```

### 3. **Exponenciación Rápida (Óptima)**
```c
double potencia_rapida(double base, int exp) {
    if (exp == 0) return 1.0;
    
    bool negativo = exp < 0;
    if (negativo) exp = -exp;
    
    double resultado = 1.0;
    double base_actual = base;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            resultado *= base_actual;
        }
        base_actual *= base_actual;
        exp /= 2;
    }
    
    return negativo ? 1.0 / resultado : resultado;
}
```

## Análisis de Complejidad

### Complejidad Temporal
- **Versión básica**: O(|exp|) - lineal
- **Versión recursiva**: O(|exp|) - lineal + overhead de stack
- **Exponenciación rápida**: O(log|exp|) - logarítmica

### Complejidad Espacial
- **Iterativa**: O(1) - espacio constante
- **Recursiva**: O(|exp|) - por la pila de llamadas
- **Exponenciación rápida**: O(1) - espacio constante

### Comparación de Eficiencia
```
Para exp = 1000:
• Básica: 1000 multiplicaciones
• Rápida: ~10 multiplicaciones (log₂(1000) ≈ 10)

Mejora: ~100x más rápida para exponentes grandes
```

## Casos de Prueba

### Casos Básicos
```c
assert(potencia(2.0, 3) == 8.0);      // Exponente positivo
assert(potencia(5.0, 0) == 1.0);      // Exponente cero
assert(potencia(4.0, -2) == 0.25);    // Exponente negativo
assert(potencia(-2.0, 3) == -8.0);    // Base negativa, exp impar
assert(potencia(-2.0, 2) == 4.0);     // Base negativa, exp par
```

### Casos con Decimales
```c
assert(fabs(potencia(1.5, 2) - 2.25) < 1e-10);      // 1.5²
assert(fabs(potencia(0.5, 3) - 0.125) < 1e-10);     // 0.5³
assert(fabs(potencia(2.5, -2) - 0.16) < 1e-10);     // 2.5⁻²
```

### Casos Especiales
```c
assert(potencia(1.0, 1000) == 1.0);   // Base 1
assert(potencia(10.0, 3) == 1000.0);  // Potencias de 10
assert(potencia(2.0, 10) == 1024.0);  // Potencias de 2
```

## Extensiones Sugeridas

### 1. **Potencias Fraccionarias**
```c
double raiz_n(double base, int n) {
    // Implementar raíz n-ésima usando método de Newton
    // base^(1/n)
}

double potencia_fraccionaria(double base, int num, int den) {
    // base^(num/den) = raiz_n(potencia(base, num), den)
}
```

### 2. **Manejo de Errores**
```c
typedef enum {
    POTENCIA_OK,
    POTENCIA_ERROR_CERO_EXPONENTE_NEGATIVO,
    POTENCIA_ERROR_OVERFLOW,
    POTENCIA_ERROR_UNDERFLOW
} ResultadoPotencia;

ResultadoPotencia potencia_segura(double base, int exp, double *resultado);
```

### 3. **Optimizaciones Específicas**
```c
// Para bases específicas
double potencia_de_2(int exp);     // Usando shifts binarios
double potencia_de_10(int exp);    // Usando tabla de lookup
double potencia_de_e(int exp);     // Para cálculos exponenciales
```

### 4. **Análisis de Precisión**
```c
void analizar_precision(double base, int exp_max) {
    // Comparar con pow() de math.h
    // Analizar error acumulativo
    // Mostrar límites de precisión
}
```

## Aplicaciones Prácticas

### 1. **Cálculos Científicos**
```c
// Ley del cuadrado inverso
double intensidad_luz(double distancia) {
    return 1.0 / potencia(distancia, 2);
}

// Crecimiento exponencial
double crecimiento_poblacion(double poblacion_inicial, double tasa, int tiempo) {
    return poblacion_inicial * potencia(1.0 + tasa, tiempo);
}
```

### 2. **Finanzas**
```c
// Interés compuesto
double interes_compuesto(double principal, double tasa, int periodos) {
    return principal * potencia(1.0 + tasa, periodos);
}

// Valor presente
double valor_presente(double valor_futuro, double tasa, int periodos) {
    return valor_futuro / potencia(1.0 + tasa, periodos);
}
```

### 3. **Computación Gráfica**
```c
// Transformaciones 2D/3D
typedef struct {
    double x, y;
} Punto2D;

Punto2D escalar_punto(Punto2D p, double factor) {
    return (Punto2D){p.x * factor, p.y * factor};
}

// Curvas de Bézier
double bezier_cuadratica(double t, double p0, double p1, double p2) {
    double u = 1.0 - t;
    return potencia(u, 2) * p0 + 2 * u * t * p1 + potencia(t, 2) * p2;
}
```

### 4. **Algoritmos Numéricos**
```c
// Serie de Taylor para e^x
double aproximar_exp(double x, int terminos) {
    double resultado = 1.0;
    double factorial = 1.0;
    
    for (int n = 1; n <= terminos; n++) {
        factorial *= n;
        resultado += potencia(x, n) / factorial;
    }
    
    return resultado;
}
```

## Consideraciones de Implementación

### Manejo de Overflow/Underflow
```c
#include <float.h>

double potencia_segura(double base, int exp) {
    if (exp == 0) return 1.0;
    
    // Verificar overflow potencial
    if (exp > 0 && base > 1.0) {
        double limite = DBL_MAX / base;
        for (int i = 1; i < exp; i++) {
            if (limite < base) {
                return INFINITY;  // Overflow
            }
            limite /= base;
        }
    }
    
    return potencia(base, exp);
}
```

### Optimización de Casos Comunes
```c
double potencia_optimizada(double base, int exp) {
    // Casos especiales rápidos
    if (exp == 0) return 1.0;
    if (exp == 1) return base;
    if (exp == 2) return base * base;
    if (base == 1.0) return 1.0;
    if (base == -1.0) return (exp % 2 == 0) ? 1.0 : -1.0;
    
    // Usar exponenciación rápida para casos generales
    return potencia_rapida(base, exp);
}
```

## Compilación y Ejecución

```bash
# Compilar el programa principal
gcc -o funcion_potencia src/main.c -std=c99 -Wall -Wextra

# Ejecutar
./funcion_potencia

# Compilar y ejecutar tests (si tienes Criterion)
gcc -o test_potencia test/test_main.c -lcriterion -lm
./test_potencia

# Con CMake
mkdir build && cd build
cmake ..
make
./funcion_potencia
make test  # Ejecutar pruebas automáticas
```

## Referencias

- **Knuth, Donald**: "The Art of Computer Programming", Vol. 2 - Algoritmos de exponenciación
- **Cormen et al.**: "Introduction to Algorithms" - Exponenciación rápida
- **IEEE 754**: Estándar para aritmética de punto flotante
- **Numerical Recipes**: Algoritmos numéricos en C - Manejo de precisión
