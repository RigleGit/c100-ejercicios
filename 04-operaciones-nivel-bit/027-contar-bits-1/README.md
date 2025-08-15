# Ejercicio 027: Contar Bits 1

## Objetivo
Contar cuántos bits están establecidos a 1 en la representación binaria de un número entero positivo. Este ejercicio introduce el análisis bit a bit y diferentes algoritmos de optimización para manipulación binaria.

## Problema
Implementa un programa que:
- Cuente los bits establecidos a 1 en un número
- Use operaciones de desplazamiento (>>) y máscaras (&)
- Demuestre diferentes algoritmos de conteo
- Compare eficiencia entre métodos básicos y optimizados
- Visualice la representación binaria del número

## Código de Ejemplo

### Entrada
```
Número: 42    → Binario: 101010 → 3 bits en 1
Número: 255   → Binario: 11111111 → 8 bits en 1
Número: 1023  → Binario: 1111111111 → 10 bits en 1
```

### Salida Esperada
```
=== Ejercicio 027: Contar Bits 1 ===

Demostración básica:
Número	Binario		Bits 1
------	-------		------
0	0000000000000000	0
1	0000000000000001	1
3	0000000000000011	2
7	0000000000000111	3
15	0000000000001111	4
31	0000000000011111	5
85	0000000001010101	4
170	0000000010101010	4
255	0000000011111111	8

=== Comparación de Métodos ===
Número	Básico	Kernighan	Tabla	Paralelo
------	------	---------	-----	--------
42	3	3		3	3
255	8	8		8	8
1023	10	10		10	10

Introduce un número entero positivo: 42
Número de bits a 1: 3
```

## Conceptos Clave

### 1. **Operaciones Bit a Bit Fundamentales**
- **Desplazamiento derecha (>>)**: Divide por potencias de 2
- **Máscara AND (&1)**: Extrae el bit menos significativo
- **Análisis secuencial**: Examina cada bit individualmente

### 2. **Algoritmos de Conteo**
- **Método básico**: Shift y máscara O(log n)
- **Algoritmo de Kernighan**: Elimina bits eficientemente O(popcount)
- **Tabla de lookup**: Precálculo para optimización O(1)
- **Población paralela**: Conteo en paralelo usando máscaras

### 3. **Peso de Hamming**
- **Definición**: Número de bits establecidos a 1
- **Aplicaciones**: Códigos de corrección de errores, criptografía
- **Propiedades**: Fundamental en teoría de la información

## Análisis de Algoritmos

### 1. **Método Básico (Shift y Máscara)**
```c
int contar_bits_basico(unsigned int numero) {
    int cuenta = 0;
    while (numero != 0) {
        if (numero & 1) {  // Verificar LSB
            cuenta++;
        }
        numero >>= 1;  // Desplazar derecha
    }
    return cuenta;
}
```
- **Complejidad**: O(log n) o O(32) para enteros de 32 bits
- **Ventaja**: Simple y fácil de entender
- **Desventaja**: Siempre examina todos los bits

### 2. **Algoritmo de Brian Kernighan**
```c
int contar_bits_kernighan(unsigned int numero) {
    int cuenta = 0;
    while (numero) {
        numero &= numero - 1;  // Elimina bit 1 más a la derecha
        cuenta++;
    }
    return cuenta;
}
```
- **Complejidad**: O(popcount) - solo iteraciones por bits establecidos
- **Ventaja**: Más eficiente para números "sparse" (pocos bits en 1)
- **Truco**: `n & (n-1)` siempre elimina el bit 1 más a la derecha

### 3. **Tabla de Lookup**
```c
// Tabla precalculada para bytes (0-255)
static const int tabla_bits[256] = {0, 1, 1, 2, 1, 2, 2, 3, ...};

int contar_bits_tabla(unsigned int numero) {
    return tabla_bits[numero & 0xFF] +           // Byte 0
           tabla_bits[(numero >> 8) & 0xFF] +    // Byte 1
           tabla_bits[(numero >> 16) & 0xFF] +   // Byte 2
           tabla_bits[(numero >> 24) & 0xFF];    // Byte 3
}
```
- **Complejidad**: O(1) con preprocesamiento O(256)
- **Ventaja**: Muy rápido para múltiples consultas
- **Desventaja**: Usa memoria adicional

### 4. **Población Paralela**
```c
int contar_bits_paralelo(unsigned int numero) {
    numero = numero - ((numero >> 1) & 0x55555555);
    numero = (numero & 0x33333333) + ((numero >> 2) & 0x33333333);
    numero = (numero + (numero >> 4)) & 0x0F0F0F0F;
    numero = numero + (numero >> 8);
    numero = numero + (numero >> 16);
    return numero & 0x3F;
}
```
- **Complejidad**: O(1) - número fijo de operaciones
- **Ventaja**: Extremadamente rápido, sin branches
- **Técnica**: Suma bits en paralelo usando máscaras binarias

## Análisis de Rendimiento

### Comparación de Métodos

| Método | Complejidad | Memoria | Ventajas | Desventajas |
|--------|-------------|---------|----------|-------------|
| **Básico** | O(log n) | O(1) | Simple, claro | Examina todos los bits |
| **Kernighan** | O(popcount) | O(1) | Eficiente para sparse | Depende de datos |
| **Tabla** | O(1) | O(256) | Muy rápido | Usa memoria extra |
| **Paralelo** | O(1) | O(1) | Óptimo | Complejo de entender |

### Casos de Uso Óptimos
```
• Básico: Educación, casos simples
• Kernighan: Números con pocos bits establecidos
• Tabla: Múltiples consultas, sistemas con memoria
• Paralelo: Rendimiento crítico, loops intensivos
```

## Aplicaciones Prácticas

### 1. **Códigos de Corrección de Errores**
```c
int paridad_hamming(unsigned int palabra) {
    int bits = contar_bits_basico(palabra);
    return bits % 2;  // Paridad par/impar
}

int distancia_hamming(unsigned int a, unsigned int b) {
    return contar_bits_basico(a ^ b);  // Bits diferentes
}
```

### 2. **Criptografía y Hash**
```c
unsigned int hash_popcount(unsigned int key) {
    int bits = contar_bits_basico(key);
    return (key << bits) ^ (key >> (32 - bits));
}

int avalanche_test(unsigned int original, unsigned int modificado) {
    int bits_diferentes = contar_bits_basico(original ^ modificado);
    return bits_diferentes;  // Debe ser ~50% para buen hash
}
```

### 3. **Algoritmos de Conjuntos (Bit Sets)**
```c
typedef unsigned int BitSet;

int cardinalidad(BitSet conjunto) {
    return contar_bits_basico(conjunto);
}

BitSet interseccion(BitSet a, BitSet b) {
    return a & b;
}

int jaccard_index(BitSet a, BitSet b) {
    int intersec = contar_bits_basico(a & b);
    int union_bits = contar_bits_basico(a | b);
    return (union_bits > 0) ? (intersec * 100) / union_bits : 0;
}
```

### 4. **Optimización de Algoritmos**
```c
// Verificar si un número es potencia de 2
bool es_potencia_de_2(unsigned int n) {
    return n != 0 && contar_bits_basico(n) == 1;
    // O alternativamente: return n != 0 && (n & (n-1)) == 0;
}

// Encontrar la siguiente potencia de 2
unsigned int siguiente_potencia_2(unsigned int n) {
    if (es_potencia_de_2(n)) return n;
    
    // Establecer todos los bits a la derecha del bit más significativo
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}
```

## Propiedades Matemáticas

### Números Especiales
```c
// Potencias de 2: Siempre 1 bit
2^0 = 1    → 1 bit
2^1 = 2    → 1 bit  
2^2 = 4    → 1 bit
2^n        → 1 bit

// Números de Mersenne (2^n - 1): n bits
2^1 - 1 = 1     → 1 bit
2^2 - 1 = 3     → 2 bits
2^3 - 1 = 7     → 3 bits
2^n - 1         → n bits
```

### Identidades Útiles
```c
// popcount(a XOR b) = distancia de Hamming
// popcount(a AND b) ≤ min(popcount(a), popcount(b))
// popcount(a OR b) = popcount(a) + popcount(b) - popcount(a AND b)
// popcount(NOT a) = 32 - popcount(a)  // Para enteros de 32 bits
```

## Casos de Prueba

### Casos Básicos
```c
assert(contar_bits(0) == 0);      // Sin bits
assert(contar_bits(1) == 1);      // Un bit
assert(contar_bits(3) == 2);      // 11₂
assert(contar_bits(7) == 3);      // 111₂
assert(contar_bits(255) == 8);    // 11111111₂
```

### Potencias de 2
```c
for (int i = 0; i < 32; i++) {
    unsigned int potencia = 1U << i;
    assert(contar_bits(potencia) == 1);
}
```

### Números de Mersenne
```c
for (int i = 1; i <= 16; i++) {
    unsigned int mersenne = (1U << i) - 1;
    assert(contar_bits(mersenne) == i);
}
```

### Consistencia entre Métodos
```c
unsigned int test_cases[] = {42, 255, 1023, 65535, 1234567, UINT_MAX};
for (int i = 0; i < num_cases; i++) {
    unsigned int n = test_cases[i];
    assert(contar_bits_basico(n) == contar_bits_kernighan(n));
    assert(contar_bits_basico(n) == contar_bits_tabla(n));
    assert(contar_bits_basico(n) == contar_bits_paralelo(n));
}
```

## Extensiones Sugeridas

### 1. **Análisis de Patrones**
```c
void analizar_patron_bits(int inicio, int fin) {
    printf("Análisis de patrones del %d al %d:\n", inicio, fin);
    for (int i = inicio; i <= fin; i++) {
        int bits = contar_bits_basico(i);
        printf("%d: %d bits\n", i, bits);
    }
}
```

### 2. **Funciones de Utilidad**
```c
// Bit más significativo
int bit_mas_significativo(unsigned int n) {
    if (n == 0) return -1;
    int pos = 0;
    while (n >>= 1) pos++;
    return pos;
}

// Bit menos significativo establecido
int bit_menos_significativo_set(unsigned int n) {
    if (n == 0) return -1;
    return n & -n;  // Aísla el LSB
}

// Rotar bits
unsigned int rotar_izquierda(unsigned int n, int bits) {
    return (n << bits) | (n >> (32 - bits));
}
```

### 3. **Análisis Estadístico**
```c
void estadisticas_bits(unsigned int inicio, unsigned int fin) {
    int histograma[33] = {0};  // 0 a 32 bits
    
    for (unsigned int i = inicio; i <= fin; i++) {
        int bits = contar_bits_basico(i);
        histograma[bits]++;
    }
    
    printf("Distribución de bits:\n");
    for (int i = 0; i <= 32; i++) {
        if (histograma[i] > 0) {
            printf("%d bits: %d números\n", i, histograma[i]);
        }
    }
}
```

### 4. **Benchmarks de Rendimiento**
```c
#include <time.h>

void benchmark_metodos(int iteraciones) {
    unsigned int numeros[1000];
    for (int i = 0; i < 1000; i++) {
        numeros[i] = rand();
    }
    
    clock_t inicio = clock();
    for (int iter = 0; iter < iteraciones; iter++) {
        for (int i = 0; i < 1000; i++) {
            volatile int resultado = contar_bits_basico(numeros[i]);
        }
    }
    clock_t fin = clock();
    
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Método básico: %.6f segundos\n", tiempo);
    
    // Repetir para otros métodos...
}
```

## Optimizaciones del Compilador

### Funciones Intrínsecas
```c
#ifdef __GNUC__
    // GCC builtin
    #define contar_bits_builtin(x) __builtin_popcount(x)
#elif defined(_MSC_VER)
    // MSVC intrinsic
    #include <intrin.h>
    #define contar_bits_builtin(x) __popcnt(x)
#endif

// Comparar con implementación manual
void comparar_con_builtin(unsigned int numero) {
    int manual = contar_bits_basico(numero);
    int builtin = contar_bits_builtin(numero);
    
    printf("Manual: %d, Builtin: %d\n", manual, builtin);
    assert(manual == builtin);
}
```

## Compilación y Ejecución

```bash
# Compilar el programa principal
gcc -o contar_bits src/main.c -std=c99 -Wall -Wextra -O3

# Ejecutar
./contar_bits

# Compilar con optimizaciones específicas
gcc -o contar_bits_opt src/main.c -mpopcnt -O3

# Compilar y ejecutar tests
gcc -o test_bits test/test_main.c -lcriterion
./test_bits

# Con CMake
mkdir build && cd build
cmake ..
make
./contar_bits_1
make test  # Ejecutar pruebas automáticas
```

## Referencias

- **Hacker's Delight**: Henry S. Warren Jr. - Cap. 5 "Counting Bits"
- **Knuth**: "The Art of Computer Programming" Vol. 4A - Combinatorial algorithms
- **Intel Manual**: Instrucción POPCNT - Population count
- **Wikipedia**: Hamming weight y algoritmos de bit counting
- **Warren**: "Hacker's Delight" - Bit manipulation techniques
