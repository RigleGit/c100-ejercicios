# Ejercicio 026: Paridad con Bits

## Objetivo
Determinar si un número entero es par o impar utilizando operaciones a nivel de bit en lugar del operador módulo (%). Este ejercicio introduce las operaciones binarias fundamentales y su aplicación práctica.

## Problema
Implementa un programa que:
- Use el operador AND bit a bit (&) para verificar paridad
- Examine el bit menos significativo (LSB) del número
- Compare la eficiencia con el método tradicional del módulo
- Demuestre conceptos de manipulación binaria

## Código de Ejemplo

### Entrada
```
Número: 42   → Par (LSB = 0)
Número: 37   → Impar (LSB = 1) 
Número: -8   → Par (LSB = 0)
Número: -15  → Impar (LSB = 1)
```

### Salida Esperada
```
=== Ejercicio 026: Paridad con Bits ===

Demostración básica:
Número	Binario		LSB	Paridad
------	-------		---	-------
0	00000000	0	PAR
1	00000001	1	IMPAR
2	00000010	0	PAR
3	00000011	1	IMPAR
4	00000100	0	PAR
5	00000101	1	IMPAR

=== Números Negativos ===
-1 es IMPAR (LSB: 1)
-2 es PAR (LSB: 0)
-3 es IMPAR (LSB: 1)
-4 es PAR (LSB: 0)

=== Comparación de Métodos ===
Método		Operación	Complejidad	Velocidad
------		---------	-----------	---------
Módulo		n % 2		División	Más lenta
Bits		n & 1		AND		Más rápida

Introduce un número entero: 42
42 es par.
```

## Conceptos Clave

### 1. **Operadores Bit a Bit**
- **AND (&)**: Operación lógica bit por bit
- **Bit menos significativo (LSB)**: El bit más a la derecha
- **Representación binaria**: Base 2 de los números

### 2. **Fundamento Matemático**
```
En binario, el LSB determina la paridad:
• Si LSB = 0 → número par
• Si LSB = 1 → número impar

Ejemplos:
12 = 1100₂ → LSB = 0 → PAR
13 = 1101₂ → LSB = 1 → IMPAR
```

### 3. **Operación AND con 1**
```
Cualquier número & 1 = LSB del número

Ejemplos:
42 & 1 = 101010₂ & 000001₂ = 0 → PAR
37 & 1 = 100101₂ & 000001₂ = 1 → IMPAR
```

## Análisis Técnico

### Representación Binaria
```
Número  | Binario    | LSB | Paridad
--------|------------|-----|--------
0       | 00000000   | 0   | PAR
1       | 00000001   | 1   | IMPAR
2       | 00000010   | 0   | PAR
3       | 00000011   | 1   | IMPAR
4       | 00000100   | 0   | PAR
5       | 00000101   | 1   | IMPAR
...     | ...        | ... | ...
```

### Números Negativos (Complemento a Dos)
```
Número  | Binario (32-bit)              | LSB | Paridad
--------|-------------------------------|-----|--------
-1      | 11111111111111111111111111111 | 1   | IMPAR
-2      | 11111111111111111111111111110 | 0   | PAR
-3      | 11111111111111111111111111101 | 1   | IMPAR
-4      | 11111111111111111111111111100 | 0   | PAR
```

### Comparación de Métodos

| Aspecto | Módulo (`n % 2`) | Bits (`n & 1`) |
|---------|------------------|----------------|
| **Operación** | División/Resto | AND bit a bit |
| **Complejidad** | O(1)* | O(1) |
| **Velocidad** | Más lenta | Más rápida |
| **Instrucciones** | División | Una instrucción |
| **Portabilidad** | Universal | Universal |

*\*El módulo puede requerir división, que es más costosa*

## Implementaciones

### 1. **Función Básica**
```c
int es_par_bits(int numero) {
    return (numero & 1) == 0;
}

int es_impar_bits(int numero) {
    return (numero & 1) == 1;
}
```

### 2. **Comparación con Módulo**
```c
void comparar_metodos(int numero) {
    // Método tradicional
    int par_modulo = (numero % 2) == 0;
    
    // Método con bits
    int par_bits = (numero & 1) == 0;
    
    printf("Número: %d\n", numero);
    printf("Módulo: %s\n", par_modulo ? "PAR" : "IMPAR");
    printf("Bits: %s\n", par_bits ? "PAR" : "IMPAR");
}
```

### 3. **Visualización Binaria**
```c
void mostrar_binario(int numero, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (numero >> i) & 1);
    }
}
```

## Aplicaciones Prácticas

### 1. **Algoritmos de Ordenamiento**
```c
void separar_pares_impares(int arr[], int n) {
    int izq = 0, der = n - 1;
    
    while (izq < der) {
        // Buscar impar desde la izquierda
        while (izq < der && (arr[izq] & 1) == 0) izq++;
        
        // Buscar par desde la derecha  
        while (izq < der && (arr[der] & 1) == 1) der--;
        
        // Intercambiar
        if (izq < der) {
            int temp = arr[izq];
            arr[izq] = arr[der];
            arr[der] = temp;
        }
    }
}
```

### 2. **Criptografía Simple**
```c
char cifrar_paridad(char c) {
    // XOR con paridad del código ASCII
    return c ^ ((c & 1) ? 0x55 : 0xAA);
}
```

### 3. **Algoritmos de Hash**
```c
unsigned int hash_simple(int numero) {
    // Usar paridad como parte del hash
    unsigned int hash = numero;
    hash ^= (hash & 1) ? 0x9E3779B9 : 0x6A09E667;
    return hash;
}
```

### 4. **Optimización de Loops**
```c
void procesar_array_optimizado(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (arr[i] & 1) {
            // Procesar números impares
            procesar_impar(arr[i]);
        } else {
            // Procesar números pares
            procesar_par(arr[i]);
        }
    }
}
```

## Casos de Prueba

### Casos Básicos
```c
assert((0 & 1) == 0);   // 0 es par
assert((1 & 1) == 1);   // 1 es impar
assert((2 & 1) == 0);   // 2 es par
assert((3 & 1) == 1);   // 3 es impar
```

### Casos con Negativos
```c
assert((-2 & 1) == 0);  // -2 es par
assert((-1 & 1) == 1);  // -1 es impar
assert((-4 & 1) == 0);  // -4 es par
assert((-3 & 1) == 1);  // -3 es impar
```

### Casos Extremos
```c
assert((INT_MAX & 1) == 1);  // INT_MAX es impar
assert((INT_MIN & 1) == 0);  // INT_MIN es par
```

## Extensiones Sugeridas

### 1. **Análisis de Patrones**
```c
void analizar_patron_paridad(int inicio, int fin) {
    printf("Patrón de paridad del %d al %d:\n", inicio, fin);
    for (int i = inicio; i <= fin; i++) {
        printf("%d", (i & 1));
        if ((i - inicio + 1) % 8 == 0) printf(" ");
    }
    printf("\n");
}
```

### 2. **Contadores de Paridad**
```c
typedef struct {
    int pares;
    int impares;
} ContadorParidad;

ContadorParidad contar_paridad_array(int arr[], int n) {
    ContadorParidad contador = {0, 0};
    for (int i = 0; i < n; i++) {
        if (arr[i] & 1) {
            contador.impares++;
        } else {
            contador.pares++;
        }
    }
    return contador;
}
```

### 3. **Generador de Secuencias**
```c
void generar_secuencia_paridad(int n) {
    printf("Primeros %d números:\n", n);
    printf("Pares: ");
    for (int i = 0, count = 0; count < n/2; i++) {
        if ((i & 1) == 0) {
            printf("%d ", i);
            count++;
        }
    }
    
    printf("\nImpares: ");
    for (int i = 0, count = 0; count < n/2; i++) {
        if (i & 1) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");
}
```

### 4. **Análisis de Rendimiento**
```c
#include <time.h>

void comparar_rendimiento(int iteraciones) {
    clock_t inicio, fin;
    
    // Test con módulo
    inicio = clock();
    for (int i = 0; i < iteraciones; i++) {
        volatile int resultado = (i % 2) == 0;
    }
    fin = clock();
    double tiempo_modulo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    
    // Test con bits
    inicio = clock();
    for (int i = 0; i < iteraciones; i++) {
        volatile int resultado = (i & 1) == 0;
    }
    fin = clock();
    double tiempo_bits = (double)(fin - inicio) / CLOCKS_PER_SEC;
    
    printf("Rendimiento (%d iteraciones):\n", iteraciones);
    printf("Módulo: %.6f segundos\n", tiempo_modulo);
    printf("Bits: %.6f segundos\n", tiempo_bits);
    printf("Mejora: %.2fx más rápido\n", tiempo_modulo / tiempo_bits);
}
```

## Notas de Implementación

### Eficiencia
- **AND bit a bit** es una operación primitiva muy rápida
- **Módulo** puede requerir división, que es más costosa
- **Compiladores modernos** optimizan `n % 2` a `n & 1` automáticamente

### Portabilidad
- Funciona en todos los sistemas de complemento a dos
- Independiente del tamaño del entero
- Compatible con signed/unsigned

### Casos Especiales
- **Números negativos**: El LSB sigue determinando la paridad
- **Cero**: Considerado par (LSB = 0)
- **Overflow**: No afecta la operación AND

## Compilación y Ejecución

```bash
# Compilar el programa principal
gcc -o paridad_bits src/main.c -std=c99 -Wall -Wextra

# Ejecutar
./paridad_bits

# Compilar y ejecutar tests (si tienes Criterion)
gcc -o test_paridad test/test_main.c -lcriterion
./test_paridad

# Con CMake
mkdir build && cd build
cmake ..
make
./paridad_bits
make test  # Ejecutar pruebas automáticas
```

## Referencias

- **K&R**: "The C Programming Language" - Operadores bit a bit
- **Hacker's Delight**: Henry S. Warren Jr. - Trucos con bits
- **Intel Manual**: Instruction set reference - AND operation
- **IEEE Standards**: Representación de enteros en complemento a dos
