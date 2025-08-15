# Ejercicio 020: Números Primos hasta N

## Enunciado

Escribe un programa en C que solicite un número entero positivo N y muestre por pantalla todos los números primos entre 2 y N.

## Descripción

El programa debe:

1. Solicitar al usuario un número entero positivo N
2. Encontrar todos los números primos desde 2 hasta N (inclusive)
3. Mostrar los números primos encontrados en una línea, separados por espacios
4. Un número primo es aquel que solo es divisible por 1 y por sí mismo

### Ejemplo de ejecución:

```
Introduce un número entero positivo: 20
2 3 5 7 11 13 17 19 
```

```
Introduce un número entero positivo: 10
2 3 5 7 
```

```
Introduce un número entero positivo: 30
2 3 5 7 11 13 17 19 23 29 
```

## Conceptos clave

- **Números primos**: Números naturales mayores que 1 que solo tienen dos divisores: 1 y ellos mismos
- **Bucles anidados**: Bucle externo para cada número, bucle interno para verificar divisibilidad
- **Optimización algorítmica**: Solo verificar divisores hasta √n
- **Lógica de divisibilidad**: Usar el operador módulo (%) para verificar divisiones exactas
- **Variables booleanas**: Usar flags para indicar si un número es primo
- **Break statement**: Salir del bucle cuando se encuentra un divisor

## Algoritmo

### Algoritmo básico:
1. Para cada número `num` desde 2 hasta N:
2. Asumir que `num` es primo (`esPrimo = 1`)
3. Para cada posible divisor `i` desde 2 hasta `√num`:
   - Si `num % i == 0`, entonces `num` no es primo
   - Marcar `esPrimo = 0` y salir del bucle (`break`)
4. Si `esPrimo` sigue siendo 1, imprimir `num`

### ¿Por qué solo hasta √n?

**Optimización clave**: Si un número n tiene un divisor mayor que √n, entonces necesariamente tiene otro divisor menor que √n.

**Demostración matemática**:
- Si n = a × b donde a > √n, entonces b < √n
- Por tanto, si no encontramos divisores hasta √n, no hay divisores mayores

| Número | √n | Divisores a verificar | Optimización |
|--------|----|--------------------|--------------|
| 25 | 5.0 | 2, 3, 4, 5 | En lugar de 2..24 |
| 49 | 7.0 | 2, 3, 4, 5, 6, 7 | En lugar de 2..48 |
| 100 | 10.0 | 2, 3, 4...10 | En lugar de 2..99 |

## Análisis de complejidad

### Sin optimización:
- **Cada número**: O(n) verificaciones
- **Total**: O(n²) operaciones

### Con optimización (√n):
- **Cada número**: O(√n) verificaciones  
- **Total**: O(n × √n) operaciones

### Ejemplo práctico para N = 100:
- **Sin optimización**: ~5,000 operaciones
- **Con optimización**: ~500 operaciones
- **Mejora**: 10x más rápido

## Tabla de los primeros primos

| Posición | Primo | Divisores probados | Optimización |
|----------|-------|-------------------|--------------|
| 1° | 2 | Ninguno (caso especial) | N/A |
| 2° | 3 | 2 | √3 ≈ 1.7 |
| 3° | 5 | 2 | √5 ≈ 2.2 |
| 4° | 7 | 2 | √7 ≈ 2.6 |
| 5° | 11 | 2, 3 | √11 ≈ 3.3 |
| 6° | 13 | 2, 3 | √13 ≈ 3.6 |
| 7° | 17 | 2, 3, 4 | √17 ≈ 4.1 |
| 8° | 19 | 2, 3, 4 | √19 ≈ 4.4 |

## Casos especiales

### Números no primos por definición:
- **0 y 1**: No son primos por definición matemática
- **Números pares > 2**: Siempre divisibles por 2

### Optimización adicional:
```c
if (num == 2) return 1;        // 2 es primo
if (num % 2 == 0) return 0;    // Pares > 2 no son primos
// Solo verificar divisores impares: 3, 5, 7, 9...
for (int i = 3; i * i <= num; i += 2)
```

## Algoritmos alternativos

### 1. Criba de Eratóstenes (más eficiente para rangos grandes):
```c
void criba_eratostenes(int N) {
    int es_primo[N+1];
    for (int i = 0; i <= N; i++) es_primo[i] = 1;
    
    es_primo[0] = es_primo[1] = 0;
    
    for (int i = 2; i * i <= N; i++) {
        if (es_primo[i]) {
            for (int j = i * i; j <= N; j += i) {
                es_primo[j] = 0;
            }
        }
    }
    
    for (int i = 2; i <= N; i++) {
        if (es_primo[i]) printf("%d ", i);
    }
}
```

### 2. Verificación optimizada con casos especiales:
```c
int es_primo_optimizado(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}
```

## Estadísticas de primos

| Rango | Cantidad de primos | Porcentaje |
|-------|-------------------|------------|
| 1-10 | 4 | 40% |
| 1-100 | 25 | 25% |
| 1-1000 | 168 | 16.8% |
| 1-10000 | 1229 | 12.3% |

**Teorema de los números primos**: La densidad de primos cerca de n es aproximadamente 1/ln(n).

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./numeros_primos_N

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Verificación individual de primalidad (números pequeños y grandes)
- ✅ Identificación correcta de números no primos
- ✅ Conteo de primos en diferentes rangos
- ✅ Secuencias específicas de primos (hasta 10, 20, etc.)
- ✅ Los primeros n números primos
- ✅ Casos extremos y optimizaciones
- ✅ Cuadrados perfectos (que nunca son primos excepto 4)

## Posibles extensiones

1. **Criba de Eratóstenes**: Implementar algoritmo más eficiente para rangos grandes
2. **Factorización**: Mostrar los factores de números no primos
3. **Primos gemelos**: Encontrar pares de primos que difieren en 2 (ej: 11,13)
4. **Estadísticas**: Contar primos y calcular porcentajes
5. **Validación de entrada**: Manejar números negativos o cero
6. **Primos de Mersenne**: Encontrar primos de la forma 2^p - 1
7. **Interfaz mejorada**: Mostrar primos en formato tabla
8. **Rendimiento**: Medir tiempo de ejecución para diferentes algoritmos

## Ejemplo de extensión - Primos gemelos:

```c
void encontrar_primos_gemelos(int N) {
    for (int i = 3; i <= N - 2; i++) {
        if (es_primo(i) && es_primo(i + 2)) {
            printf("(%d, %d) ", i, i + 2);
        }
    }
}
```

Salida para N=30: `(3, 5) (5, 7) (11, 13) (17, 19) (29, 31)`

## Notas de implementación

- La optimización √n es crucial para el rendimiento
- El uso de `break` evita verificaciones innecesarias
- Las pruebas cubren casos matemáticos importantes
- El algoritmo es didáctico y fácil de entender
- Excelente base para estudiar teoría de números
- Las extensiones permiten explorar conceptos avanzados
