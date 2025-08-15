# Ejercicio 009: Intercambio de Variables

## Enunciado
Escribe un programa que lea dos números enteros e intercambie sus valores utilizando una variable auxiliar. Luego, muestra los valores intercambiados.

## Objetivo
- Aprender el concepto de intercambio de variables usando variable auxiliar
- Comprender la importancia del orden en las asignaciones
- Prácticar el uso de variables temporales

## Explicación
Este ejercicio es fundamental en programación. Para intercambiar dos variables sin perder información, necesitamos una variable auxiliar temporal que guarde temporalmente uno de los valores mientras realizamos el intercambio.

El algoritmo es:
1. `temp = a` - Guardamos el valor de `a` en la variable temporal
2. `a = b` - Asignamos el valor de `b` a `a`
3. `b = temp` - Asignamos el valor original de `a` (guardado en `temp`) a `b`

## Ejemplo de Ejecución
```
Introduce dos números enteros: 5 10
Antes del intercambio: a = 5, b = 10
Después del intercambio: a = 10, b = 5
```

## Conceptos Clave
- **Variable auxiliar**: Variable temporal para no perder datos
- **Asignación**: Operador `=` para dar valores a variables
- **Orden de operaciones**: La secuencia importa para no perder información
- **Entrada y salida**: `scanf()` y `printf()`

## Compilación y Ejecución
```bash
# Usando CMake
mkdir build && cd build
cmake ..
make
./intercambio_variables

# Compilación directa
gcc -o intercambio_variables src/main.c
./intercambio_variables
```

## Pruebas
```bash
# Ejecutar tests automáticos
cd build
make test
# o
ctest -V
```

## Errores Comunes
1. **Intercambio sin variable auxiliar**: `a = b; b = a;` → Ambas variables tendrán el mismo valor
2. **Orden incorrecto**: Cambiar el orden de las asignaciones puede perder datos
3. **No declarar la variable temporal**: Olvidar declarar `temp`

## Extensiones Posibles
1. Intercambiar con operaciones aritméticas: `a = a + b; b = a - b; a = a - b;`
2. Intercambiar con XOR (solo para enteros): `a ^= b; b ^= a; a ^= b;`
3. Intercambiar múltiples variables en cadena
4. Intercambiar elementos de un array
5. Función de intercambio usando punteros

## Variantes del Problema
- Intercambiar valores de tipo `float` o `double`
- Intercambiar caracteres
- Intercambiar cadenas de caracteres
- Intercambiar sin usar variable auxiliar
