# Ejercicio 007: Distancia Euclídea

## Enunciado

Escribe un programa que lea las coordenadas de dos puntos en el plano cartesiano y calcule la distancia euclídea entre ellos.

## Descripción

Este ejercicio te permite aplicar fórmulas matemáticas reales en un contexto de programación. Practicarás el uso de variables tipo `float` o `double`, el cálculo de potencias y raíces cuadradas usando la biblioteca `<math.h>`, y entenderás cómo resolver problemas geométricos simples en C.

## Fórmula Matemática

La **distancia euclídea** entre dos puntos P₁(x₁, y₁) y P₂(x₂, y₂) se calcula usando el teorema de Pitágoras:

```
d = √[(x₂ - x₁)² + (y₂ - y₁)²]
```

### Derivación de la Fórmula

1. **Diferencias en coordenadas**: Δx = x₂ - x₁, Δy = y₂ - y₁
2. **Teorema de Pitágoras**: d² = Δx² + Δy²
3. **Distancia final**: d = √(Δx² + Δy²)

## Conceptos Clave

- **Variables `double`**: Precisión necesaria para cálculos geométricos
- **Librería `<math.h>`**: Funciones `sqrt()` y `pow()`
- **Especificador `%lf`**: Para leer variables `double`
- **Formato de salida**: `%.2lf` para mostrar dos decimales
- **Teorema de Pitágoras**: Base matemática del cálculo
- **Geometría analítica**: Aplicación práctica de matemáticas
- **Enlazado `-lm`**: Requerido para funciones matemáticas

## Estructura del Proyecto

```
007-distancia-euclidiana/
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
./distancia_euclidiana

# Ejecutar tests
make test
# o directamente:
./test_distancia_euclidiana
```

### Compilación Manual

```bash
# Compilar el programa principal (nota el -lm para la librería matemática)
gcc -o distancia_euclidiana src/main.c -lm

# Ejecutar
./distancia_euclidiana

# Compilar y ejecutar tests
gcc -o test_distancia_euclidiana test/test_main.c -lm
./test_distancia_euclidiana
```

## Ejemplos de Ejecución

### Ejemplo 1: Triángulo 3-4-5
```
Introduce las coordenadas del primer punto (x1 y1): 0 0
Introduce las coordenadas del segundo punto (x2 y2): 3 4
La distancia entre los dos puntos es: 5.00
```

### Ejemplo 2: Distancia horizontal
```
Introduce las coordenadas del primer punto (x1 y1): 1 2
Introduce las coordenadas del segundo punto (x2 y2): 6 2
La distancia entre los dos puntos es: 5.00
```

### Ejemplo 3: Coordenadas negativas
```
Introduce las coordenadas del primer punto (x1 y1): -2 -1
Introduce las coordenadas del segundo punto (x2 y2): 1 3
La distancia entre los dos puntos es: 5.00
```

## Casos de Prueba

Los tests verifican diferentes escenarios geométricos:
- ✅ **Puntos idénticos**: (2,3) a (2,3) → 0.00
- ✅ **Distancia horizontal**: (1,2) a (6,2) → 5.00
- ✅ **Distancia vertical**: (3,1) a (3,8) → 7.00
- ✅ **Triángulo clásico**: (0,0) a (3,4) → 5.00
- ✅ **Números decimales**: (1.5,2.5) a (4.5,6.5) → 5.00
- ✅ **Coordenadas negativas**: (-2,-1) a (1,3) → 5.00
- ✅ **Estructura correcta**: Variables `double`, funciones `sqrt()` y `pow()`

## Notas de Aprendizaje

1. **`double x1, y1, x2, y2, distancia;`**: Variables de doble precisión
2. **`scanf("%lf %lf", &x1, &y1);`**: Leer dos doubles en una línea
3. **`pow(x2 - x1, 2)`**: Calcular el cuadrado de la diferencia
4. **`sqrt(...)`**: Calcular la raíz cuadrada del resultado
5. **`%.2lf`**: Mostrar resultado con exactamente dos decimales

## Desglose del Algoritmo

### Paso 1: Leer Coordenadas
```c
scanf("%lf %lf", &x1, &y1);  // Primer punto
scanf("%lf %lf", &x2, &y2);  // Segundo punto
```

### Paso 2: Calcular Diferencias
```c
// Implícito en la fórmula:
// dx = x2 - x1
// dy = y2 - y1
```

### Paso 3: Aplicar Fórmula
```c
distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
```

## Casos Especiales

### Distancia Cero
- **Condición**: x₁ = x₂ y y₁ = y₂
- **Resultado**: d = 0
- **Significado**: Los puntos son idénticos

### Distancia Horizontal
- **Condición**: y₁ = y₂
- **Fórmula simplificada**: d = |x₂ - x₁|
- **Ejemplo**: (1,3) a (5,3) → d = 4

### Distancia Vertical
- **Condición**: x₁ = x₂
- **Fórmula simplificada**: d = |y₂ - y₁|
- **Ejemplo**: (2,1) a (2,6) → d = 5

## Triángulos Pitagóricos Famosos

| Lados | Ejemplo de Puntos | Distancia |
|-------|-------------------|-----------|
| 3-4-5 | (0,0) a (3,4) | 5.00 |
| 5-12-13 | (0,0) a (5,12) | 13.00 |
| 8-15-17 | (0,0) a (8,15) | 17.00 |
| 7-24-25 | (0,0) a (7,24) | 25.00 |

## Aplicaciones Prácticas

- **Sistemas de navegación**: Calcular distancias en mapas
- **Gráficos por computadora**: Distancias entre objetos
- **Robótica**: Planificación de rutas
- **Física**: Cálculo de desplazamientos
- **Juegos**: Detección de colisiones y rangos
- **Estadística**: Distancias en espacios multidimensionales

## Optimizaciones Posibles

### Sin función `pow()` (más eficiente)
```c
double dx = x2 - x1;
double dy = y2 - y1;
distancia = sqrt(dx * dx + dy * dy);
```

### Para comparaciones (evitar `sqrt()`)
```c
// Si solo necesitas comparar distancias:
double distancia_cuadrada = dx * dx + dy * dy;
```

## Posibles Errores Comunes

1. **Olvidar `-lm`**: Error de enlazado con funciones matemáticas
2. **Usar `%f` con `double`**: Debería usar `%lf` en `scanf`
3. **Precisión insuficiente**: Usar `float` en lugar de `double`
4. **Orden de operaciones**: Paréntesis incorrectos en la fórmula
5. **No incluir `math.h`**: Error de compilación

## Extensiones Posibles

- Calcular el punto medio entre los dos puntos
- Determinar si tres puntos forman un triángulo
- Calcular el área del triángulo formado con el origen
- Implementar distancia de Manhattan (|x₂-x₁| + |y₂-y₁|)
- Extender a distancia en 3D (añadir coordenada z)
- Crear calculadora de perímetros y áreas
- Implementar diferentes métricas de distancia

