# Ejercicio 032: Suma y Promedio de Array

## Descripción
Este programa solicita al usuario que ingrese una cantidad de números enteros, los almacena en un array y calcula tanto la suma total como el promedio de todos los valores. Es un ejercicio fundamental para practicar operaciones básicas con arrays y acumuladores.

## Enunciado
Escribir un programa que:
1. Solicite al usuario la cantidad de números a ingresar (máximo 100)
2. Lea esos números y los almacene en un array
3. Calcule la suma total de todos los números
4. Calcule el promedio de los números (usando aritmética de punto flotante)
5. Muestre tanto la suma como el promedio

## Funcionalidades
- **Entrada de datos**: Lectura de cantidad de números y valores
- **Validación**: Verifica que la cantidad esté en el rango válido (1-100)
- **Acumulación**: Suma todos los valores del array
- **Cálculo de promedio**: Conversión a double para precisión decimal
- **Salida formateada**: Muestra suma como entero y promedio con 2 decimales

## Compilación y Ejecución

### Requisitos
- Compilador C (gcc, clang)
- CMake 3.10 o superior
- Criterion (para tests, opcional)

### Compilación con CMake
```bash
mkdir build
cd build
cmake ..
make
```

### Ejecución
```bash
# Desde el directorio build
./bin/suma_promedio_array

# O directamente con gcc
gcc -o suma_promedio_array src/main.c
./suma_promedio_array
```

### Ejecutar Tests
```bash
# Desde el directorio build
make run_tests

# O ejecutar directamente
./test_032-suma-promedio-array
```

## Ejemplo de Uso
```
¿Cuántos números vas a introducir (máximo 100)? 5
Introduce el número 1: 10
Introduce el número 2: 20
Introduce el número 3: 30
Introduce el número 4: 40
Introduce el número 5: 50
La suma es: 150
El promedio es: 30.00
```

## Conceptos Demostrados
- **Arrays**: Declaración y manejo de arrays de enteros
- **Acumuladores**: Variable para sumar elementos progresivamente
- **Conversión de tipos**: Cast de int a double para cálculo preciso
- **Aritmética de punto flotante**: Uso de double para promedios
- **Bucles**: Recorrido de arrays para operaciones matemáticas
- **Validación de entrada**: Verificación de rangos válidos

## Estructura del Proyecto
```
032-suma-promedio-array/
├── src/
│   └── main.c          # Código principal
├── test/
│   └── test_main.c     # Tests unitarios
├── include/            # Archivos de cabecera (si los hay)
├── CMakeLists.txt      # Configuración de CMake
├── .gitignore         # Archivos a ignorar por git
└── README.md          # Este archivo
```

## Casos de Prueba
Los tests incluyen verificaciones para:
- Arrays con números positivos y negativos
- Arrays de diferentes tamaños (1, 3, 5, 6 elementos)
- Casos donde la suma es cero
- Promedios con decimales (precisión a 2 decimales)
- Números grandes y pequeños
- Validación de cantidades inválidas (0, >100)
- Secuencias mixtas de números

## Aspectos Técnicos Importantes

### Precisión del Promedio
```c
promedio = (double)suma / n;
```
El cast a `double` es crucial para:
- Evitar división entera truncada
- Obtener resultados precisos con decimales
- Mantener la precisión en cálculos posteriores

### Manejo de Tipos
- **suma**: `int` - suficiente para la mayoría de casos
- **promedio**: `double` - necesario para precisión decimal
- **números**: `int` - entrada del usuario

### Formato de Salida
- Suma: `%d` (entero)
- Promedio: `%.2f` (2 decimales de precisión)

## Casos Especiales
- **Un solo número**: suma = número, promedio = número
- **Suma cero**: promedio = 0.00
- **Números negativos**: se manejan correctamente
- **Números grandes**: vigilar overflow en suma

## Mejoras Posibles
- Usar `long long` para sumas muy grandes
- Validar overflow en la suma
- Permitir números de punto flotante como entrada
- Calcular mediana y moda además del promedio
- Implementar entrada desde archivo
- Agregar estadísticas adicionales (desviación estándar, etc.)
