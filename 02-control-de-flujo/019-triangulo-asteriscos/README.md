# Ejercicio 019: Triángulo de Asteriscos

## Enunciado

Escribe un programa en C que, dado un número entero positivo n, imprima en pantalla un triángulo rectángulo formado por asteriscos (*) con n filas.

## Descripción

El programa debe:

1. Solicitar al usuario un número entero positivo n (número de filas)
2. Generar un triángulo rectángulo usando asteriscos
3. La primera fila tiene 1 asterisco, la segunda 2, y así sucesivamente
4. La fila n-ésima tiene n asteriscos

### Ejemplo de ejecución:

```
Introduce el número de filas: 5
*
**
***
****
*****
```

```
Introduce el número de filas: 3
*
**
***
```

```
Introduce el número de filas: 1
*
```

## Conceptos clave

- **Bucles anidados**: Un bucle dentro de otro para controlar filas y columnas
- **Bucle externo**: Controla el número de filas (1 a n)
- **Bucle interno**: Controla el número de asteriscos por fila (1 a i)
- **Patrones visuales**: Creación de formas geométricas con caracteres
- **Progresión aritmética**: Cada fila tiene un asterisco más que la anterior

## Algoritmo

1. Leer el número de filas n del usuario
2. Para cada fila i desde 1 hasta n:
   - Para cada posición j desde 1 hasta i:
     - Imprimir un asterisco
   - Imprimir un salto de línea
3. Fin del programa

## Análisis de bucles anidados

### Estructura básica:
```c
for (int i = 1; i <= n; i++) {      // Bucle externo: filas
    for (int j = 1; j <= i; j++) {  // Bucle interno: asteriscos
        printf("*");
    }
    printf("\n");                   // Nueva línea
}
```

### Ejecución paso a paso (n=4):

| Fila (i) | Asteriscos (j) | Iteraciones internas | Salida |
|----------|----------------|---------------------|--------|
| 1 | j = 1 | 1 vez | `*` |
| 2 | j = 1, 2 | 2 veces | `**` |
| 3 | j = 1, 2, 3 | 3 veces | `***` |
| 4 | j = 1, 2, 3, 4 | 4 veces | `****` |

### Complejidad:
- **Iteraciones totales**: 1 + 2 + 3 + ... + n = n*(n+1)/2
- **Complejidad temporal**: O(n²)
- **Complejidad espacial**: O(1)

## Variaciones del patrón

### 1. Triángulo rectángulo (actual):
```
*
**
***
****
```

### 2. Triángulo invertido:
```c
for (int i = n; i >= 1; i--) {
    for (int j = 1; j <= i; j++) {
        printf("*");
    }
    printf("\n");
}
```
Salida:
```
****
***
**
*
```

### 3. Triángulo isósceles:
```c
for (int i = 1; i <= n; i++) {
    // Espacios
    for (int j = 1; j <= n - i; j++) {
        printf(" ");
    }
    // Asteriscos
    for (int j = 1; j <= 2*i - 1; j++) {
        printf("*");
    }
    printf("\n");
}
```
Salida (n=4):
```
   *
  ***
 *****
*******
```

## Análisis matemático

### Fórmula del total de asteriscos:
Para un triángulo de n filas: **Total = n × (n + 1) ÷ 2**

| n | Asteriscos por fila | Total | Fórmula |
|---|-------------------|-------|---------|
| 1 | 1 | 1 | 1×2÷2 = 1 |
| 3 | 1,2,3 | 6 | 3×4÷2 = 6 |
| 5 | 1,2,3,4,5 | 15 | 5×6÷2 = 15 |
| 10 | 1,2,...,10 | 55 | 10×11÷2 = 55 |

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./triangulo_asteriscos

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Conteo correcto de asteriscos por fila
- ✅ Cálculo del total de asteriscos usando la fórmula
- ✅ Generación correcta de filas individuales
- ✅ Validación del patrón del triángulo
- ✅ Casos extremos (triángulos grandes)
- ✅ Progresión aritmética entre filas
- ✅ Verificación de la fórmula matemática

## Posibles extensiones

1. **Triángulos con diferentes caracteres**: Usar números, letras o símbolos
2. **Triángulos con colores**: Usar códigos ANSI para colores
3. **Pirámides**: Crear triángulos isósceles centrados
4. **Patrones más complejos**: Rombos, hexágonos, etc.
5. **Triángulos huecos**: Solo mostrar el contorno
6. **Animaciones**: Dibujar el triángulo línea por línea con pausas
7. **Validación de entrada**: Manejar números negativos o cero
8. **Menú interactivo**: Elegir diferentes tipos de patrones

## Ejemplo de extensión - Triángulo hueco:

```c
for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= i; j++) {
        if (j == 1 || j == i || i == n) {
            printf("*");
        } else {
            printf(" ");
        }
    }
    printf("\n");
}
```

Salida (n=5):
```
*
**
* *
*  *
*****
```

## Notas de implementación

- Los bucles anidados son fundamentales en programación
- Esta es la base para trabajar con matrices y arreglos 2D
- El patrón se puede extender a formas más complejas
- Excelente ejercicio para visualizar algoritmos
- Las pruebas verifican tanto la lógica como las matemáticas subyacentes
