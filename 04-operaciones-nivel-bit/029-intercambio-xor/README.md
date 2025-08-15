# Ejercicio 029: Intercambio de Variables con XOR

## Objetivo
Implementar el intercambio de variables sin usar memoria adicional utilizando las propiedades matemáticas del operador XOR.

## Descripción
Este ejercicio explora el elegante método de intercambio de variables usando operaciones XOR exclusivamente. Se analiza la base matemática, se compara con otros métodos y se exploran casos especiales y limitaciones.

## Conceptos Clave

### Operador XOR (Exclusive OR)
- **Símbolo**: `^` en C
- **Función**: Retorna 1 si los bits son diferentes, 0 si son iguales
- **Tabla de verdad**:
  ```
  A | B | A^B
  --|---|----
  0 | 0 |  0
  0 | 1 |  1
  1 | 0 |  1
  1 | 1 |  0
  ```

### Propiedades Matemáticas del XOR
1. **Conmutativa**: `a ^ b = b ^ a`
2. **Asociativa**: `(a ^ b) ^ c = a ^ (b ^ c)`
3. **Elemento neutro**: `a ^ 0 = a`
4. **Autoinverso**: `a ^ a = 0`
5. **Doble XOR**: `(a ^ b) ^ b = a`

### El Algoritmo de Intercambio XOR
```c
void intercambio_xor(int *a, int *b) {
    if (a == b) return;  // Protección esencial
    
    *a = *a ^ *b;  // Paso 1: a contiene a^b
    *b = *a ^ *b;  // Paso 2: b = (a^b)^b = a (valor original de a)
    *a = *a ^ *b;  // Paso 3: a = (a^b)^a = b (valor original de b)
}
```

## Análisis Matemático

### ¿Por qué funciona?
1. **Paso 1**: `a = a ⊕ b`
   - Ahora `a` contiene la combinación XOR de ambos valores originales

2. **Paso 2**: `b = (a ⊕ b) ⊕ b = a ⊕ b ⊕ b = a ⊕ 0 = a`
   - La propiedad autoinversa cancela `b ⊕ b = 0`
   - Ahora `b` contiene el valor original de `a`

3. **Paso 3**: `a = (a ⊕ b) ⊕ a = a ⊕ b ⊕ a = b ⊕ 0 = b`
   - Similar a paso 2, ahora `a` contiene el valor original de `b`

### Ejemplo Numérico
Intercambiar `a = 25` y `b = 58`:

```
Inicial:    a = 25 = 00011001₂
            b = 58 = 00111010₂

Paso 1:     a = 25 ^ 58 = 00100011₂ = 35
            b = 58       = 00111010₂ = 58

Paso 2:     a = 35       = 00100011₂ = 35
            b = 35 ^ 58  = 00011001₂ = 25 (¡valor original de a!)

Paso 3:     a = 35 ^ 25  = 00111010₂ = 58 (¡valor original de b!)
            b = 25       = 00011001₂ = 25

Resultado:  a = 58, b = 25 ✓
```

## Comparación de Métodos

| Método | Memoria Extra | Operaciones | Limitaciones |
|--------|---------------|-------------|--------------|
| **Temporal** | 1 variable | 3 asignaciones | Ninguna |
| **XOR** | 0 variables | 3 XOR | Misma dirección → 0 |
| **Aritmético** | 0 variables | 2 sumas, 1 resta | Overflow posible |
| **Multiplicativo** | 0 variables | 1 mult, 2 div | No funciona con 0 |

### Ventajas del XOR
- ✅ **Sin memoria adicional**
- ✅ **Sin riesgo de overflow**
- ✅ **Funciona con cualquier valor entero**
- ✅ **Operaciones bit a bit muy eficientes**
- ✅ **Elegante y matemáticamente hermoso**

### Desventajas del XOR
- ❌ **Falla si las variables tienen la misma dirección**
- ❌ **Menos legible que el método temporal**
- ❌ **No funciona con tipos de punto flotante**

## Casos Especiales

### 1. Misma Dirección de Memoria
```c
int x = 42;
intercambio_xor(&x, &x);  // ¡x se convierte en 0!
```
**Problema**: `x ^ x = 0` siempre.

### 2. Valores Iguales (Direcciones Diferentes)
```c
int a = 25, b = 25;
intercambio_xor(&a, &b);  // Funciona correctamente
// Resultado: a = 25, b = 25
```

### 3. Overflow en Método Aritmético
```c
int a = 2000000000, b = 2000000000;
// a + b = 4000000000 > INT_MAX (overflow)
```

## Aplicaciones Prácticas

### 1. Algoritmos de Ordenamiento
- **Bubble Sort in-place**: Intercambio eficiente
- **Selection Sort**: Minimizar uso de memoria

### 2. Criptografía
- **Cifrado de flujo**: XOR con clave
- **One-time pad**: Seguridad perfecta

### 3. Estructuras de Datos
- **Listas enlazadas XOR**: Punteros comprimidos
- **Algoritmos de búsqueda**: Optimización de memoria

### 4. Programación de Sistemas
- **Drivers de dispositivos**: Intercambio de registros
- **Kernels**: Operaciones atómicas eficientes

## Análisis de Complejidad

| Aspecto | Complejidad |
|---------|-------------|
| **Tiempo** | O(1) - Constante |
| **Espacio** | O(1) - Sin memoria adicional |
| **Operaciones** | 3 XOR + verificación de seguridad |

## Código de Ejemplo

```c
#include <stdio.h>

void intercambio_xor(int *a, int *b) {
    if (a == b) return;  // Protección esencial
    
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

int main() {
    int x = 10, y = 20;
    printf("Antes: x=%d, y=%d\n", x, y);
    
    intercambio_xor(&x, &y);
    
    printf("Después: x=%d, y=%d\n", x, y);
    return 0;
}
```

## Extensiones Sugeridas

### 1. **Análisis de Rendimiento**
- Comparar tiempos de ejecución entre métodos
- Medir uso de registros de CPU
- Analizar optimizaciones del compilador

### 2. **Implementación Genérica**
```c
#define SWAP_XOR(a, b) do { \
    if (&(a) != &(b)) { \
        (a) ^= (b); \
        (b) ^= (a); \
        (a) ^= (b); \
    } \
} while(0)
```

### 3. **Intercambio Múltiple**
- Intercambio circular de 3+ variables
- Permutaciones usando solo XOR

### 4. **Aplicaciones Avanzadas**
- Implementar lista enlazada XOR
- Cifrado XOR con múltiples claves
- Optimizaciones en algoritmos de ordenamiento

## Ejercicios Relacionados
- **026**: Paridad con Bits
- **027**: Contar Bits en 1
- **028**: Potencia de 2 con Bits
- **030**: Empaquetar Bytes (próximo)

## Recursos Adicionales
- [XOR Linked List](https://en.wikipedia.org/wiki/XOR_linked_list)
- [Bitwise Operations in C](https://en.wikipedia.org/wiki/Bitwise_operation)
- [XOR Cipher](https://en.wikipedia.org/wiki/XOR_cipher)
