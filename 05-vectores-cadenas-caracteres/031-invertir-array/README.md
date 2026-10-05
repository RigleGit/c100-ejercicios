# Ejercicio 031: Invertir Array

## Descripción
Este programa solicita al usuario que ingrese una cantidad de números enteros y luego los muestra en orden inverso. Es un ejercicio fundamental para practicar el manejo de arrays y recorrido de elementos.

## Enunciado
Escribir un programa que:
1. Solicite al usuario la cantidad de números a ingresar (máximo 100)
2. Lea esos números y los almacene en un array
3. Muestre los números en orden inverso

## Funcionalidades
- **Entrada de datos**: Lectura de cantidad de números y valores
- **Validación**: Verifica que la cantidad esté en el rango válido (1-100)
- **Inversión**: Recorre el array desde el último elemento hasta el primero
- **Salida formateada**: Muestra los números separados por espacios

## Ejemplo de Uso
```
Ingrese la cantidad de números: 5
Ingrese el número 1: 10
Ingrese el número 2: 20
Ingrese el número 3: 30
Ingrese el número 4: 40
Ingrese el número 5: 50
Los números en orden inverso son:
50 40 30 20 10
```

## Conceptos Demostrados
- **Arrays**: Declaración y manejo de arrays de enteros
- **Bucles**: Uso de for para entrada y salida de datos
- **Validación de entrada**: Verificación de rangos válidos
- **Recorrido inverso**: Técnica para acceder a elementos en orden reverso
- **Entrada/Salida**: Uso de scanf y printf para interacción con usuario

## Notas Técnicas
- El programa maneja hasta 100 números como máximo
- Utiliza un array estático para almacenar los valores
- La validación de entrada es importante para evitar desbordamientos
- El recorrido inverso se hace mediante índices decrecientes

## Mejoras Posibles
- Permitir arrays de tamaño dinámico
- Agregar más validaciones de entrada
- Implementar inversión in-place para arrays grandes
- Soporte para números de punto flotante

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/invertir_array
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
