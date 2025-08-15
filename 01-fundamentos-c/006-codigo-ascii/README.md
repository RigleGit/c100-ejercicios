# Ejercicio 006: Código ASCII

## Enunciado

Escribe un programa que lea un carácter introducido por el usuario y muestre su valor numérico según la tabla ASCII.

## Descripción

Este ejercicio te permite practicar la lectura de caracteres con `%c` y entender cómo los caracteres se representan internamente como números enteros. Es un paso importante para comprender cómo funciona la codificación de texto en C, y te introduce de manera natural a la conversión entre tipos `char` e `int`.

## Tabla ASCII - Caracteres Comunes

### Caracteres de Control (0-31)
| Carácter | ASCII | Descripción |
|----------|-------|-------------|
| NUL | 0 | Carácter nulo |
| TAB | 9 | Tabulación |
| LF | 10 | Salto de línea |
| CR | 13 | Retorno de carro |
| ESC | 27 | Escape |

### Caracteres Imprimibles (32-126)
| Carácter | ASCII | Carácter | ASCII | Carácter | ASCII |
|----------|-------|----------|-------|----------|-------|
| (espacio) | 32 | '0' | 48 | '@' | 64 |
| '!' | 33 | '1' | 49 | 'A' | 65 |
| '"' | 34 | '2' | 50 | 'B' | 66 |
| '#' | 35 | ... | ... | ... | ... |
| ... | ... | '9' | 57 | 'Z' | 90 |
| '/' | 47 | ':' | 58 | '[' | 91 |
| ... | ... | ... | ... | 'a' | 97 |
| ... | ... | '?' | 63 | 'z' | 122 |

## Conceptos Clave

- **Tipo `char`**: Almacena un solo carácter (1 byte)
- **Especificador `%c`**: Para leer/escribir caracteres
- **Casting `(int)`**: Conversión explícita de char a int
- **Espacio en `scanf`**: ` %c` evita problemas con caracteres residuales
- **Codificación ASCII**: Estándar de representación de caracteres
- **Representación interna**: Los caracteres son números enteros pequeños

## Estructura del Proyecto

```
006-codigo-ascii/
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
./codigo_ascii

# Ejecutar tests
make test
# o directamente:
./test_codigo_ascii
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -o codigo_ascii src/main.c

# Ejecutar
./codigo_ascii

# Compilar y ejecutar tests
gcc -o test_codigo_ascii test/test_main.c
./test_codigo_ascii
```

## Ejemplos de Ejecución

```
Introduce un carácter: A
El código ASCII de 'A' es: 65
```

```
Introduce un carácter: a
El código ASCII de 'a' es: 97
```

```
Introduce un carácter: 0
El código ASCII de '0' es: 48
```

```
Introduce un carácter: @
El código ASCII de '@' es: 64
```

## Casos de Prueba

Los tests verifican caracteres importantes del ASCII:
- ✅ **Letra mayúscula**: 'A' → 65
- ✅ **Letra minúscula**: 'a' → 97
- ✅ **Dígito**: '0' → 48, '9' → 57
- ✅ **Espacio**: ' ' → 32
- ✅ **Símbolo**: '@' → 64
- ✅ **Estructura correcta**: Variables `char`, casting `(int)`, formato ` %c`

## Notas de Aprendizaje

1. **`char caracter;`**: Declara una variable para almacenar un carácter
2. **`scanf(" %c", &caracter);`**: El espacio antes de `%c` es crucial
3. **`(int)caracter`**: Convierte explícitamente el char a int
4. **`%c` vs `%d`**: Muestra el carácter vs su valor numérico
5. **Representación interna**: Los caracteres son números de 0-127 (ASCII básico)

## ¿Por qué el Espacio en `scanf`?

```c
scanf(" %c", &caracter);  // ✅ Correcto
scanf("%c", &caracter);   // ❌ Puede fallar
```

El espacio antes de `%c` le dice a `scanf` que ignore espacios en blanco, tabulaciones y saltos de línea que puedan quedar en el buffer de entrada.

## Diferencias entre Tipos de Caracteres

### Letras Mayúsculas vs Minúsculas
```
'A' = 65, 'B' = 66, ..., 'Z' = 90
'a' = 97, 'b' = 98, ..., 'z' = 122
Diferencia: minúscula = mayúscula + 32
```

### Dígitos como Caracteres
```
'0' = 48, '1' = 49, ..., '9' = 57
Para convertir a número: digito - '0'
Ejemplo: '5' - '0' = 57 - 48 = 9
```

## Aplicaciones Prácticas

- **Validación de entrada**: Verificar tipos de caracteres
- **Conversión de mayúsculas/minúsculas**: Usando diferencias ASCII
- **Cifrado simple**: Desplazamiento de caracteres
- **Análisis de texto**: Conteo de tipos de caracteres
- **Protocolos de comunicación**: Interpretación de comandos

## Rangos ASCII Importantes

| Rango | Descripción | Ejemplos |
|-------|-------------|----------|
| 0-31 | Caracteres de control | TAB, LF, CR |
| 32-47 | Símbolos y espacio | !"#$%&'()*+,-./ |
| 48-57 | Dígitos | 0123456789 |
| 58-64 | Símbolos | :;<=>?@ |
| 65-90 | Letras mayúsculas | ABCDEFGHIJKLMNOPQRSTUVWXYZ |
| 91-96 | Símbolos | [\]^_` |
| 97-122 | Letras minúsculas | abcdefghijklmnopqrstuvwxyz |
| 123-126 | Símbolos | {}\|~ |

## Posibles Errores Comunes

1. **Olvidar el espacio**: `scanf("%c", ...)` sin espacio inicial
2. **No hacer casting**: Mostrar directamente sin `(int)`
3. **Confundir carácter con número**: '5' ≠ 5
4. **Buffer de entrada**: No limpiar entrada previa
5. **Caracteres especiales**: No considerar caracteres no imprimibles

## Extensiones Posibles

- Mostrar si es letra, dígito o símbolo
- Convertir entre mayúsculas y minúsculas
- Mostrar el carácter siguiente y anterior
- Crear tabla ASCII completa
- Implementar cifrado César simple
- Validar rangos específicos de caracteres
- Analizar frecuencia de caracteres en texto
