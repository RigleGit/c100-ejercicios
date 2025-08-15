# Ejercicio 017: Suma de Dígitos

## Enunciado

Escribe un programa en C que lea un número entero positivo y calcule la suma de todos sus dígitos.

## Descripción

El programa debe:

1. Solicitar al usuario que introduzca un número entero positivo
2. Calcular la suma de todos los dígitos que componen el número
3. Mostrar el resultado de la suma
4. Validar que el número sea positivo

### Ejemplo de ejecución:

```
Introduce un número entero positivo: 1234
La suma de los dígitos es: 10
```

```
Introduce un número entero positivo: 567
La suma de los dígitos es: 18
```

```
Introduce un número entero positivo: -15
El número debe ser positivo.
```

## Conceptos clave

- **Operador módulo (%)**: Para extraer el último dígito de un número
- **División entera (/)**: Para eliminar el último dígito de un número
- **Bucles while**: Para repetir el proceso hasta que no queden dígitos
- **Validación de entrada**: Verificar que el número sea positivo

## Algoritmo

1. Leer el número del usuario
2. Verificar que sea positivo
3. Mientras el número sea mayor que 0:
   - Extraer el último dígito usando `numero % 10`
   - Sumarlo al acumulador
   - Eliminar el último dígito usando `numero / 10`
4. Mostrar la suma total

## Tabla de referencia - Operaciones con dígitos

| Número | Operación | Resultado | Explicación |
|--------|-----------|-----------|-------------|
| 1234 | 1234 % 10 | 4 | Último dígito |
| 1234 | 1234 / 10 | 123 | Sin último dígito |
| 123 | 123 % 10 | 3 | Último dígito |
| 123 | 123 / 10 | 12 | Sin último dígito |
| 12 | 12 % 10 | 2 | Último dígito |
| 12 | 12 / 10 | 1 | Sin último dígito |
| 1 | 1 % 10 | 1 | Último dígito |
| 1 | 1 / 10 | 0 | Sin último dígito |

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./suma_digitos

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Números de un solo dígito (0, 5, 9)
- ✅ Números de dos dígitos (12, 99, 10)
- ✅ Números de tres dígitos (123, 456, 999)
- ✅ Números más grandes (1234, 9876, 12345)
- ✅ Casos especiales (100, 1000, 2020)

## Posibles extensiones

1. **Versión recursiva**: Implementar la suma de dígitos usando recursión
2. **Múltiples números**: Procesar varios números en una sola ejecución
3. **Análisis de dígitos**: Mostrar también el dígito más frecuente
4. **Números negativos**: Manejar números negativos ignorando el signo
5. **Base diferente**: Calcular suma de dígitos en otras bases numéricas

## Notas de implementación

- Se usa validación para asegurar números positivos
- El algoritmo es eficiente con complejidad O(log n)
- Las pruebas cubren casos típicos y extremos
- El código está bien comentado para fines educativos
