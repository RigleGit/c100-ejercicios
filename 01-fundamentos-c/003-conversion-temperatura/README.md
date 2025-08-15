# Ejercicio 003: Conversión de Temperatura

## Enunciado

Escribe un programa que convierta una temperatura dada en grados Celsius a grados Fahrenheit.

## Descripción

Este ejercicio es un clásico en la enseñanza de programación y sirve como una excelente oportunidad para practicar con variables de tipo decimal (`float` o `double`) y operaciones aritméticas con fórmulas. También refuerza la lectura de datos mediante `scanf` y la salida formateada con `printf`, incluyendo control sobre los decimales mostrados. Además, introduce al estudiante a los tipos de datos con punto flotante, necesarios cuando trabajamos con medidas reales.

## Fórmula de Conversión

La fórmula para convertir de Celsius a Fahrenheit es:

```
°F = (°C × 9/5) + 32
```

## Conceptos Clave

- **Variables de punto flotante**: Uso de `float` para números decimales
- **Especificador `%f`**: Para leer y escribir números decimales
- **Formato de salida**: `%.1f` para mostrar un decimal
- **Operaciones aritméticas**: Multiplicación, división y suma en una fórmula
- **Precedencia de operadores**: Uso de paréntesis para controlar el orden
- **Datos con precisión**: Manejo de medidas reales que requieren decimales

## Estructura del Proyecto

```
003-conversion-temperatura/
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
./conversion_temperatura

# Ejecutar tests
make test
# o directamente:
./test_conversion_temperatura
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -o conversion_temperatura src/main.c -lm

# Ejecutar
./conversion_temperatura

# Compilar y ejecutar tests
gcc -o test_conversion_temperatura test/test_main.c -lm
./test_conversion_temperatura
```

## Ejemplo de Ejecución

```
Introduce la temperatura en grados Celsius: 25.5
25.5 grados Celsius equivalen a 77.9 grados Fahrenheit
```

## Casos de Prueba

Los tests verifican conversiones importantes:
- ✅ **Punto de congelación**: 0°C = 32°F
- ✅ **Punto de ebullición**: 100°C = 212°F
- ✅ **Temperatura negativa**: -40°C = -40°F (punto de coincidencia)
- ✅ **Números decimales**: 25.5°C = 77.9°F
- ✅ **Formato de salida**: Verificación de `%.1f`
- ✅ **Estructura del código**: Variables `float`, fórmula correcta

## Notas de Aprendizaje

1. **`float celsius, fahrenheit;`**: Declara variables de punto flotante
2. **`scanf("%f", &celsius)`**: Lee un número decimal del usuario
3. **`(celsius * 9 / 5) + 32`**: Aplica la fórmula de conversión
4. **`%.1f`**: Formatea la salida para mostrar exactamente un decimal
5. **Precedencia**: Los paréntesis aseguran el orden correcto de operaciones

## Puntos de Referencia Importantes

| Celsius | Fahrenheit | Descripción |
|---------|------------|-------------|
| -273.15°C | -459.67°F | Cero absoluto |
| -40°C | -40°F | Punto donde las escalas coinciden |
| 0°C | 32°F | Punto de congelación del agua |
| 20°C | 68°F | Temperatura ambiente típica |
| 37°C | 98.6°F | Temperatura corporal normal |
| 100°C | 212°F | Punto de ebullición del agua |

## Posibles Errores Comunes

1. **Olvidar paréntesis**: `celsius * 9 / 5 + 32` puede dar resultados incorrectos por precedencia
2. **División entera**: Usar `9/5` en lugar de `9.0/5.0` puede truncar el resultado
3. **Formato incorrecto**: Usar `%d` en lugar de `%f` para números decimales
4. **Precisión**: No considerar la precisión de `float` vs `double`

## Extensiones Posibles

- Implementar conversión bidireccional (Fahrenheit a Celsius)
- Agregar conversión a Kelvin
- Crear un menú para elegir tipo de conversión
- Validar entrada (temperaturas por debajo del cero absoluto)
- Mostrar tabla de conversiones para un rango de temperaturas
- Implementar con mayor precisión usando `double`
