# Ejercicio 023: Función esPrimo

## Enunciado

Escribe una función `bool esPrimo(int n)` que determine si un número entero mayor que 1 es primo. Luego, en `main`, utiliza esta función para listar todos los números primos hasta un valor N introducido por el usuario.

## Descripción

El programa debe:

1. Definir una función `esPrimo` que determine si un número es primo
2. Implementar optimizaciones para mejorar el rendimiento
3. En `main`, solicitar un límite N al usuario
4. Usar la función para listar todos los primos hasta N
5. Mostrar los resultados de forma clara

### Ejemplo de ejecución:

```
Introduce un número entero positivo: 30
Números primos hasta 30:
2 3 5 7 11 13 17 19 23 29 
```

```
Introduce un número entero positivo: 10
Números primos hasta 10:
2 3 5 7 
```

```
Introduce un número entero positivo: 100
Números primos hasta 100:
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 
```

## Conceptos clave

- **Modularización**: Separar la lógica de verificación de primalidad
- **Función booleana**: Retornar true/false según la condición
- **Optimización algorítmica**: Mejorar eficiencia con matemáticas
- **Reutilización**: Usar la misma función múltiples veces
- **Encapsulación**: Ocultar la complejidad detrás de una interfaz simple
- **stdbool.h**: Biblioteca para tipos booleanos en C

## Definición matemática de número primo

Un **número primo** es un número natural mayor que 1 que tiene exactamente dos divisores positivos: 1 y él mismo.

### Ejemplos:
- **Primos**: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31...
- **No primos**: 1 (por definición), 4, 6, 8, 9, 10, 12, 14, 15, 16...

### Casos especiales:
- **1**: No es primo por definición matemática
- **2**: Único número primo par
- **Números negativos**: No pueden ser primos

## Evolución del algoritmo esPrimo

### 1. Versión básica (ineficiente):
```c
bool esPrimo_basico(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    
    for (int i = 2; i < n; i++) {  // Verifica TODOS los números
        if (n % i == 0) return false;
    }
    
    return true;
}
```
**Complejidad**: O(n) - Muy lenta para números grandes

### 2. Optimización √n:
```c
bool esPrimo_sqrt(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    
    for (int i = 2; i <= sqrt(n); i++) {  // Solo hasta √n
        if (n % i == 0) return false;
    }
    
    return true;
}
```
**Complejidad**: O(√n) - Mucho más rápida

### 3. Versión optimizada (final):
```c
bool esPrimo(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;  // Elimina pares

    for (int i = 3; i <= sqrt(n); i += 2) {  // Solo impares
        if (n % i == 0) return false;
    }

    return true;
}
```
**Complejidad**: O(√n/2) - Aproximadamente 2x más rápida

## ¿Por qué la optimización √n funciona?

### Demostración matemática:

Si un número **n** tiene un divisor **d > √n**, entonces:
- n = d × k, donde k es otro divisor
- Como d > √n, entonces k < √n
- Por tanto, ya habríamos encontrado k en nuestra búsqueda

### Ejemplo con n = 35:
- √35 ≈ 5.9
- Divisores de 35: 1, 5, 7, 35
- Solo necesitamos verificar hasta 5: encontramos que 5 divide a 35
- No necesitamos verificar 6, 7, 8... hasta 34

### Tabla de comparación de eficiencia:

| Número | Algoritmo básico | Algoritmo √n | Mejora |
|--------|-----------------|-------------|--------|
| 100 | 98 verificaciones | 10 verificaciones | 9.8x |
| 1000 | 998 verificaciones | 31 verificaciones | 32x |
| 10000 | 9998 verificaciones | 100 verificaciones | 100x |

## Optimizaciones adicionales implementadas

### 1. **Manejo de casos especiales**:
```c
if (n <= 1) return false;  // 0, 1, negativos
if (n == 2) return true;   // Único primo par
```

### 2. **Eliminación de números pares**:
```c
if (n % 2 == 0) return false;  // Todos los pares > 2
```

### 3. **Verificación solo de impares**:
```c
for (int i = 3; i <= sqrt(n); i += 2)  // 3, 5, 7, 9, 11...
```

### 4. **Uso de sqrt() de math.h**:
- Cálculo preciso de la raíz cuadrada
- Evita errores de precisión con enteros

## Análisis de rendimiento

### Comparación de operaciones para verificar si 997 es primo:

| Algoritmo | Operaciones | Resultado |
|-----------|-------------|-----------|
| **Básico** | 995 divisiones | Muy lento |
| **√n** | 31 divisiones | Rápido |
| **√n + optimizaciones** | 15 divisiones | Muy rápido |

### Rendimiento en listado de primos hasta 1000:
- **Básico**: ~500,000 operaciones
- **Optimizado**: ~5,000 operaciones
- **Mejora**: 100x más rápido

## Uso de stdbool.h

### Antes de C99:
```c
// Sin stdbool.h (menos claro)
int esPrimo(int n) {
    // ...
    return 1;  // ¿Qué significa 1?
}

if (esPrimo(n)) {  // ¿Qué tipo devuelve?
    // ...
}
```

### Con stdbool.h (C99+):
```c
// Con stdbool.h (más claro)
bool esPrimo(int n) {
    // ...
    return true;  // Claramente booleano
}

if (esPrimo(n)) {  // Claramente booleano
    // ...
}
```

### Ventajas de bool:
- **Claridad semántica**: true/false vs 1/0
- **Mejor documentación**: El tipo indica el propósito
- **Menos errores**: Evita confusiones con enteros

## Aplicaciones de la función esPrimo

### 1. **Listado de primos en rango**:
```c
for (int i = 2; i <= limite; i++) {
    if (esPrimo(i)) {
        printf("%d ", i);
    }
}
```

### 2. **Conteo de primos**:
```c
int contar_primos(int limite) {
    int contador = 0;
    for (int i = 2; i <= limite; i++) {
        if (esPrimo(i)) contador++;
    }
    return contador;
}
```

### 3. **Factorización prima**:
```c
void factorizar(int n) {
    for (int i = 2; i <= n; i++) {
        if (esPrimo(i) && n % i == 0) {
            printf("%d ", i);
            n /= i;
            i--; // Verificar el mismo primo otra vez
        }
    }
}
```

### 4. **Primos gemelos**:
```c
void encontrar_primos_gemelos(int limite) {
    for (int i = 3; i <= limite - 2; i++) {
        if (esPrimo(i) && esPrimo(i + 2)) {
            printf("(%d, %d) ", i, i + 2);
        }
    }
}
```

## Extensiones del algoritmo

### 1. **Criba de Eratóstenes** (más eficiente para rangos):
```c
void criba_eratostenes(int limite) {
    bool es_primo[limite + 1];
    memset(es_primo, true, sizeof(es_primo));
    es_primo[0] = es_primo[1] = false;
    
    for (int i = 2; i * i <= limite; i++) {
        if (es_primo[i]) {
            for (int j = i * i; j <= limite; j += i) {
                es_primo[j] = false;
            }
        }
    }
    
    for (int i = 2; i <= limite; i++) {
        if (es_primo[i]) printf("%d ", i);
    }
}
```

### 2. **Test de primalidad probabilístico** (Miller-Rabin):
```c
// Para números muy grandes (implementación compleja)
bool es_primo_miller_rabin(long long n, int k);
```

### 3. **Optimización 6k±1**:
```c
bool esPrimo_6k(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}
```

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./funcion_esprimo

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Primos pequeños conocidos (2, 3, 5, 7, 11, 13, 17, 19, 23)
- ✅ Números no primos pequeños (1, 4, 6, 8, 9, 10, 12, 14, 15)
- ✅ Casos especiales y límites (0, -1, -5, 1, 2)
- ✅ Primos más grandes (29, 31, 37, 41, 43, 47, 53, 59, 61, 67)
- ✅ Compuestos grandes con factores conocidos
- ✅ Comparación entre diferentes implementaciones
- ✅ Conteo de primos en rangos específicos
- ✅ Secuencias de primos verificadas
- ✅ Primos gemelos
- ✅ Cuadrados de primos (que no son primos)
- ✅ Optimización de números pares
- ✅ Números con factores pequeños
- ✅ Primos grandes conocidos (97, 101, 103, 107, 109, 113)
- ✅ Consistencia y rendimiento

## Posibles extensiones

1. **Interfaz mejorada**: Mostrar tiempo de ejecución y estadísticas
2. **Factorización completa**: Descomponer números en factores primos
3. **Primos especiales**: Mersenne, Fermat, Sophie Germain
4. **Visualización**: Gráficos de distribución de primos
5. **Optimizaciones avanzadas**: Wheel factorization, segmented sieve
6. **Números grandes**: Soporte para long long o bibliotecas de precisión arbitraria
7. **Test de primalidad**: Implementar Miller-Rabin o AKS
8. **Aplicaciones criptográficas**: Generación de claves RSA

## Ejemplo de extensión - Factorización prima:

```c
void factorizar_numero(int n) {
    printf("Factorización de %d: ", n);
    
    for (int i = 2; i <= n; i++) {
        if (esPrimo(i)) {
            while (n % i == 0) {
                printf("%d ", i);
                n /= i;
            }
        }
        if (n == 1) break;
    }
    printf("\n");
}

// Uso:
factorizar_numero(60);  // Salida: Factorización de 60: 2 2 3 5
```

## Notas de implementación

- Excelente ejemplo de modularización efectiva
- Demuestra la importancia de la optimización algorítmica
- Introduce tipos booleanos y su uso apropiado
- Combina matemáticas con programación eficiente
- Las pruebas cubren casos matemáticos importantes
- Base sólida para algoritmos de teoría de números más avanzados
- Enseña a separar lógica compleja en funciones reutilizables
