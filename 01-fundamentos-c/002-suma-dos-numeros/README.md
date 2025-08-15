# Ejercicio 002: Suma de Dos Números

## Enunciado

Escribe un programa que pida al usuario dos números enteros, los sume y muestre el resultado en pantalla.

## Descripción

Este ejercicio tiene como objetivo reforzar el uso de entrada y salida estándar en C, introduciendo una nueva función: `scanf`, que permite leer datos proporcionados por el usuario. Es también una forma práctica de comenzar a trabajar con variables, operaciones aritméticas y el flujo básico de interacción entre el usuario y el programa. Aunque la operación es sencilla, el ejercicio introduce el concepto de almacenar datos en memoria y manipularlos mediante expresiones, dos pilares fundamentales en programación.

## Conceptos Clave

- **Variables enteras**: Declaración y uso de variables `int`
- **Función `scanf()`**: Para leer entrada del usuario
- **Especificadores de formato**: `%d` para enteros
- **Operador de dirección**: `&` para pasar la dirección de las variables a `scanf`
- **Operaciones aritméticas**: Suma de números enteros
- **Interpolación en `printf()`**: Mostrar valores de variables en cadenas

## Estructura del Proyecto

```
002-suma-dos-numeros/
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
./suma_dos_numeros

# Ejecutar tests
make test
# o directamente:
./test_suma_dos_numeros
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -o suma_dos_numeros src/main.c

# Ejecutar
./suma_dos_numeros

# Compilar y ejecutar tests
gcc -o test_suma_dos_numeros test/test_main.c
./test_suma_dos_numeros
```

## Ejemplo de Ejecución

```
Introduce el primer número: 15
Introduce el segundo número: 25
La suma de 15 y 25 es 40
```

## Notas de Aprendizaje

1. **Declaración de variables**: `int a, b, suma;` declara tres variables enteras
2. **`scanf("%d", &a)`**: Lee un entero del usuario y lo almacena en la variable `a`
3. **Operador `&`**: Proporciona la dirección de memoria de la variable a `scanf`
4. **Aritmética básica**: `suma = a + b` realiza la operación matemática
5. **Formato de salida**: `printf("La suma de %d y %d es %d\n", a, b, suma)` muestra los valores

## Casos de Prueba

Los tests verifican:
- ✅ Suma de números positivos (5 + 10 = 15)
- ✅ Suma con números negativos (-3 + 7 = 4)
- ✅ Suma de ceros (0 + 0 = 0)
- ✅ Estructura correcta del código

## Posibles Errores Comunes

1. **Olvidar el operador `&`** en `scanf`: `scanf("%d", a)` ❌ vs `scanf("%d", &a)` ✅
2. **Especificador incorrecto**: `scanf("%s", &a)` ❌ para enteros
3. **No inicializar variables**: Aunque no es necesario aquí, es buena práctica
4. **Buffer overflow**: `scanf` puede ser vulnerable, en proyectos reales considerar alternativas

## Extensiones Posibles

- Validar que la entrada sean números válidos
- Agregar más operaciones (resta, multiplicación, división)
- Manejar números decimales usando `float` y `%f`
- Crear una calculadora básica con menú de opciones
- Implementar verificación de overflow en la suma
