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

## Ejemplo de Ejecución

```
Introduce la temperatura en grados Celsius: 25.5
25.5 grados Celsius equivalen a 77.9 grados Fahrenheit
```

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

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/conversion_temperatura
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
