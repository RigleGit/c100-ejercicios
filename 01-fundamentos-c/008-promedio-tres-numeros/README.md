# Ejercicio 008: Promedio de Tres Números

## Enunciado

Escribe un programa que lea tres números reales ingresados por el usuario y calcule su promedio.

## Descripción

Este ejercicio refuerza el manejo de entrada múltiple, operaciones con números reales y precisión en divisiones. Es ideal para practicar cómo evitar errores comunes al mezclar enteros y flotantes, y cómo presentar resultados con formato adecuado.

## Fórmula Matemática

El **promedio aritmético** (o media) de tres números se calcula como:

```
Promedio = (num1 + num2 + num3) / 3
```

### Propiedades del Promedio
- **Punto de equilibrio**: El promedio representa el valor central
- **Sensible a valores extremos**: Un valor muy grande o pequeño afecta significativamente
- **Unidades**: El promedio mantiene las mismas unidades que los números originales

## Conceptos Clave

- **Variables `float`**: Números con punto decimal
- **Entrada múltiple**: Leer varios valores en una sola línea
- **Especificador `%f`**: Para leer y escribir números flotantes
- **División con decimales**: Asegurar precisión en el resultado
- **Formato de salida**: `%.2f` para mostrar exactamente dos decimales
- **Paréntesis en operaciones**: Garantizar orden correcto de cálculo

## Estructura del Proyecto

```
008-promedio-tres-numeros/
├── CMakeLists.txt          # Configuración de compilación
├── README.md               # Este archivo
├── src/
│   └── main.c             # Código fuente principal
└── test/
    └── test_main.c        # Tests del programa
```

## Compilación y Ejecución

### Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar el programa
./promedio_tres_numeros

# Ejecutar tests
make test
# o directamente:
./test_promedio_tres_numeros
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -o promedio_tres_numeros src/main.c

# Ejecutar
./promedio_tres_numeros

# Compilar y ejecutar tests
gcc -o test_promedio_tres_numeros test/test_main.c
./test_promedio_tres_numeros
```

## Ejemplos de Ejecución

### Ejemplo 1: Números enteros
```
Introduce tres números (pueden tener decimales): 3 6 9
El promedio es: 6.00
```

### Ejemplo 2: Números decimales
```
Introduce tres números (pueden tener decimales): 2.5 3.5 4.0
El promedio es: 3.33
```

### Ejemplo 3: Números mixtos con negativos
```
Introduce tres números (pueden tener decimales): -2 0 5
El promedio es: 1.00
```

### Ejemplo 4: Números iguales
```
Introduce tres números (pueden tener decimales): 7.5 7.5 7.5
El promedio es: 7.50
```

## Casos de Prueba

Los tests verifican diferentes escenarios estadísticos:
- ✅ **Números enteros**: 3, 6, 9 → 6.00
- ✅ **Números decimales**: 2.5, 3.5, 4.0 → 3.33
- ✅ **Con negativos**: -2, 0, 5 → 1.00
- ✅ **Números iguales**: 7.5, 7.5, 7.5 → 7.50
- ✅ **Con ceros**: 0, 0, 6 → 2.00
- ✅ **Precisión decimal**: 1, 2, 3 → 2.00
- ✅ **Estructura correcta**: Variables `float`, división por 3, formato `%.2f`

## Notas de Aprendizaje

1. **`float num1, num2, num3, promedio;`**: Declara variables de punto flotante
2. **`scanf("%f %f %f", &num1, &num2, &num3);`**: Lee tres números en una línea
3. **`(num1 + num2 + num3) / 3`**: Paréntesis aseguran suma antes de división
4. **`%.2f`**: Formatea salida con exactamente dos decimales
5. **División automática**: Al usar variables `float`, la división es decimal

## Desglose del Algoritmo

### Paso 1: Declarar Variables
```c
float num1, num2, num3, promedio;
```

### Paso 2: Leer Entrada
```c
scanf("%f %f %f", &num1, &num2, &num3);
```

### Paso 3: Calcular Promedio
```c
promedio = (num1 + num2 + num3) / 3;
```

### Paso 4: Mostrar Resultado
```c
printf("El promedio es: %.2f\n", promedio);
```

## Precisión en División

### ¿Por qué usar `float`?
```c
// Con variables float:
float a = 1, b = 2, c = 3;
float promedio = (a + b + c) / 3;  // Resultado: 2.00

// Si usáramos int (incorrecto para este ejercicio):
int a = 1, b = 2, c = 3;
float promedio = (a + b + c) / 3;  // Resultado: 2.00 (por suerte)
int a = 1, b = 1, c = 2;
float promedio = (a + b + c) / 3;  // Resultado: 1.00 (debería ser 1.33)
```

## Casos Especiales

### Promedio de Números Iguales
- **Entrada**: a, a, a
- **Resultado**: a
- **Ejemplo**: 5.5, 5.5, 5.5 → 5.50

### Promedio con Cero
- **Entrada**: 0, b, c
- **Resultado**: (b + c) / 3
- **Ejemplo**: 0, 3, 6 → 3.00

### Promedio de Negativos
- **Entrada**: -a, -b, -c
- **Resultado**: -(a + b + c) / 3
- **Ejemplo**: -1, -2, -3 → -2.00

## Aplicaciones Prácticas

- **Calificaciones académicas**: Promedio de notas
- **Análisis financiero**: Promedio de precios o ganancias
- **Meteorología**: Temperatura promedio
- **Deportes**: Estadísticas de rendimiento
- **Control de calidad**: Medidas promedio en manufactura
- **Análisis de datos**: Tendencia central

## Tabla de Ejemplos Comunes

| Números | Suma | Promedio | Descripción |
|---------|------|----------|-------------|
| 0, 0, 0 | 0 | 0.00 | Caso trivial |
| 1, 2, 3 | 6 | 2.00 | Secuencia simple |
| 10, 20, 30 | 60 | 20.00 | Múltiplos de 10 |
| 2.5, 2.5, 2.5 | 7.5 | 2.50 | Números iguales |
| -5, 0, 5 | 0 | 0.00 | Simétrico alrededor de 0 |

## Variaciones del Ejercicio

### Promedio Ponderado
```c
// Si tuviéramos pesos w1, w2, w3:
promedio_ponderado = (num1*w1 + num2*w2 + num3*w3) / (w1 + w2 + w3);
```

### Promedio de N Números
```c
// Para número variable de entradas:
float suma = 0;
int n = 3;
for(int i = 0; i < n; i++) {
    scanf("%f", &numero);
    suma += numero;
}
promedio = suma / n;
```

## Posibles Errores Comunes

1. **División entera**: Usar variables `int` puede truncar el resultado
2. **Orden de operaciones**: No usar paréntesis puede dar resultados incorrectos
3. **Formato incorrecto**: Usar `%d` en lugar de `%f` para flotantes
4. **Precisión insuficiente**: No mostrar decimales adecuados
5. **Entrada malformada**: No manejar errores de `scanf`

## Extensiones Posibles

- Validar que la entrada sean números válidos
- Calcular también la mediana y la moda
- Mostrar cuál número está más cerca del promedio
- Calcular la desviación estándar
- Permitir número variable de entradas
- Implementar promedio ponderado
- Crear calculadora estadística completa

## Conceptos Estadísticos Relacionados

### Medidas de Tendencia Central
- **Media (promedio)**: ∑x / n
- **Mediana**: Valor central ordenado
- **Moda**: Valor más frecuente

### Propiedades Matemáticas
- **Linealidad**: promedio(a*x + b) = a*promedio(x) + b
- **Aditividad**: promedio(x + y) = promedio(x) + promedio(y)

