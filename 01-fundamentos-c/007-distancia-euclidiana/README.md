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

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/distancia_euclidiana
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
