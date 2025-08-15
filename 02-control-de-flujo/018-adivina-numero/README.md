# Ejercicio 018: Adivina el Número

## Enunciado

Escribe un programa en C que genere un número aleatorio entre 1 y 100, y que pida al usuario que lo adivine. Después de cada intento, el programa debe indicar si el número secreto es mayor o menor que el ingresado. El bucle se repite hasta que el usuario acierta.

## Descripción

El programa debe:

1. Generar un número aleatorio entre 1 y 100
2. Solicitar al usuario que adivine el número
3. Después de cada intento, dar una pista:
   - "El número es mayor" si el intento es menor que el secreto
   - "El número es menor" si el intento es mayor que el secreto
   - "¡Correcto!" cuando el usuario acierta
4. Repetir hasta que el usuario adivine correctamente

### Ejemplo de ejecución:

```
Adivina el número (entre 1 y 100):
Tu intento: 50
El número es mayor.
Tu intento: 75
El número es menor.
Tu intento: 63
El número es mayor.
Tu intento: 69
El número es menor.
Tu intento: 66
¡Correcto! Has adivinado el número.
```

## Conceptos clave

- **Bucle do...while**: Ejecuta al menos una vez y repite mientras la condición sea verdadera
- **Números aleatorios**: Uso de `rand()` y `srand()` para generar números pseudoaleatorios
- **Semilla aleatoria**: `srand(time(NULL))` para cambiar la secuencia en cada ejecución
- **Estructuras condicionales**: `if-else if-else` para dar pistas al jugador
- **Rango de números**: `rand() % 100 + 1` para obtener números entre 1 y 100

## Algoritmo

1. Inicializar el generador de números aleatorios con `srand(time(NULL))`
2. Generar un número secreto entre 1 y 100
3. Mostrar instrucciones al usuario
4. Repetir mientras el intento no sea igual al secreto:
   - Pedir un intento al usuario
   - Comparar con el número secreto
   - Dar la pista correspondiente
5. Felicitar al usuario cuando acierte

## Tabla de referencia - Generación de números aleatorios

| Expresión | Rango | Ejemplo |
|-----------|-------|---------|
| `rand() % 100` | 0 a 99 | 0, 45, 99 |
| `rand() % 100 + 1` | 1 a 100 | 1, 46, 100 |
| `rand() % 10` | 0 a 9 | 0, 5, 9 |
| `rand() % 6 + 1` | 1 a 6 | 1, 3, 6 (dado) |
| `rand() % (max-min+1) + min` | min a max | Fórmula general |

## ¿Por qué do...while?

El bucle `do...while` es perfecto para este ejercicio porque:

- **Garantiza al menos una ejecución**: Siempre necesitamos pedir al menos un intento
- **Condición al final**: Evaluamos si acertó después de procesar el intento
- **Flujo natural**: El usuario siempre hace un intento antes de verificar si ganó

### Comparación con while:

```c
// Con while (menos natural)
printf("Tu intento: ");
scanf("%d", &intento);
while (intento != secreto) {
    // dar pista
    printf("Tu intento: ");
    scanf("%d", &intento);
}

// Con do...while (más natural)
do {
    printf("Tu intento: ");
    scanf("%d", &intento);
    // dar pista
} while (intento != secreto);
```

## Compilación y ejecución

```bash
# Compilar el programa
mkdir build && cd build
cmake ..
make

# Ejecutar el programa
./adivina_numero

# Ejecutar las pruebas (si están disponibles)
make test
```

## Pruebas incluidas

El programa incluye pruebas automáticas que verifican:

- ✅ Evaluación correcta de intentos (menor, mayor, igual)
- ✅ Generación de números en el rango correcto (1-100)
- ✅ Casos límite (1, 100)
- ✅ Secuencia de juego simulada
- ✅ Rangos aleatorios personalizados

## Posibles extensiones

1. **Contador de intentos**: Mostrar cuántos intentos necesitó el usuario
2. **Dificultad variable**: Permitir elegir el rango (fácil: 1-50, difícil: 1-1000)
3. **Puntuación**: Sistema de puntos basado en los intentos
4. **Múltiples rondas**: Jugar varias partidas seguidas
5. **Estadísticas**: Guardar el mejor puntaje del jugador
6. **Límite de intentos**: Hacer el juego más desafiante
7. **Pistas mejoradas**: "Muy cerca", "Tibio", "Frío" según la proximidad

## Notas de implementación

- Se usa `srand(time(NULL))` para generar secuencias diferentes en cada ejecución
- La función `rand()` genera números pseudoaleatorios
- El operador módulo `%` se usa para limitar el rango
- Las pruebas usan semillas fijas para resultados reproducibles
- El código está optimizado para legibilidad educativa
