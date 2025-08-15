# Ejercicio 001: Hola Mundo

## Enunciado

Escribe un programa que imprima el mensaje "Hola Mundo".

## Descripción

Este ejercicio es un punto de partida clásico y esencial para iniciarse en cualquier lenguaje de programación. En el caso de C, te permite conocer la estructura básica de un programa, cómo se gestiona la salida de datos y cómo se organiza el flujo de ejecución. Aunque es simple, es una buena oportunidad para familiarizarse con elementos fundamentales del lenguaje.

## Conceptos Clave

- **Función `main()`**: Punto de entrada del programa
- **Función `printf()`**: Para imprimir texto en la consola
- **Directiva `#include`**: Para incluir bibliotecas estándar
- **Valor de retorno**: `return 0` indica ejecución exitosa

## Estructura del Proyecto

```
001-hola-mundo/
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
./hola_mundo

# Ejecutar tests
make test
# o directamente:
./test_hola_mundo
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -o hola_mundo src/main.c

# Ejecutar
./hola_mundo

# Compilar y ejecutar tests
gcc -o test_hola_mundo test/test_main.c
./test_hola_mundo
```

## Salida Esperada

```
Hola Mundo
```

## Notas de Aprendizaje

1. **`#include <stdio.h>`**: Incluye la biblioteca estándar de entrada/salida que contiene la función `printf()`
2. **`int main()`**: Función principal donde comienza la ejecución del programa
3. **`printf("Hola Mundo\n")`**: Imprime el texto seguido de un salto de línea (`\n`)
4. **`return 0`**: Indica que el programa terminó exitosamente

## Extensiones Posibles

- Modificar el mensaje para incluir tu nombre
- Agregar más líneas de salida
- Experimentar con diferentes caracteres de escape (`\n`, `\t`, etc.)
- Usar variables para almacenar el mensaje antes de imprimirlo
