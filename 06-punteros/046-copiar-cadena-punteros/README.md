# Ejercicio 046: Copiar Cadena con Punteros

## 📋 Descripción

Este ejercicio explora la copia de cadenas de caracteres utilizando punteros en C, implementando una versión manual de la función `strcpy` de la biblioteca estándar. Es fundamental para entender el manejo de cadenas, la aritmética de punteros y la gestión de memoria.

## 🎯 Objetivo

Implementar diferentes métodos para copiar cadenas de caracteres utilizando punteros, demostrando el manejo correcto de arrays de caracteres terminados en `'\0'` y la importancia de la gestión segura de memoria.

## 📝 Enunciado

**Copia de Cadenas con Punteros:**

Crear una función que copie una cadena de caracteres (char *orig) en otra (char *dest) sin usar la función strcpy de la biblioteca estándar. La función debe:

1. Recibir como parámetros un puntero destino y un puntero origen
2. Copiar todos los caracteres desde origen hasta encontrar `'\0'`
3. Incluir el terminador nulo en la copia
4. Manejar casos especiales como cadenas vacías

**Ejemplo:**
```c
char origen[] = "Hola, mundo!";
char destino[50]; // Asegurarse de que hay suficiente espacio
copiarCadena(destino, origen);
printf("Cadena copiada: %s\n", destino);
```

**Salida esperada:**
```
Cadena copiada: Hola, mundo!
```

## 🔧 Funciones Implementadas

### Funciones Principales

1. **`copiarCadena(char *dest, const char *orig)`**
   - Implementación básica y clara del algoritmo
   - Copia carácter por carácter usando punteros

2. **`copiarCadena_optimizada(char *dest, const char *orig)`**
   - Versión optimizada con operaciones combinadas
   - Demuestra expresiones compactas de C

3. **`copiarCadena_retorno(char *dest, const char *orig)`**
   - Versión que retorna el puntero destino
   - Compatible con el comportamiento de `strcpy`

4. **`copiarCadena_segura(char *dest, const char *orig, size_t tamaño_max)`**
   - Versión segura con límite de buffer
   - Previene desbordamientos de buffer
   - Similar a `strncpy` pero más segura

### Funciones Especializadas

5. **`copiarCadena_overlap(char *dest, const char *orig)`**
   - Maneja casos de solapamiento de memoria
   - Similar a `memmove` para cadenas

6. **`copiarSubcadena(char *dest, const char *orig, size_t inicio, size_t longitud)`**
   - Copia una porción específica de la cadena
   - Útil para extraer subcadenas

7. **`concatenarCadena(char *dest, const char *orig)`**
   - Concatena cadenas usando punteros
   - Implementación manual de `strcat`

### Funciones Auxiliares

8. **`longitudCadena(const char *str)`**
   - Calcula longitud usando aritmética de punteros
   - Implementación manual de `strlen`

9. **`invertirCadena(char *str)`**
   - Invierte cadena in-place usando punteros
   - Demuestra manipulación bidireccional

10. **`copiarCadena_debug(char *dest, const char *orig)`**
    - Versión educativa con información paso a paso
    - Muestra direcciones de memoria y proceso

## 🏗️ Estructura del Proyecto

```
046-copiar-cadena-punteros/
├── src/
│   └── main.c              # Implementación principal
├── test/
│   └── test_main.c         # Suite de tests con Criterion
├── include/
│   └── .gitkeep           # Directorio para headers
├── CMakeLists.txt         # Configuración de build
├── README.md              # Este archivo
└── .gitignore            # Archivos a ignorar por git
```

## 🧪 Casos de Prueba

### Tests Básicos
- ✅ Cadena simple
- ✅ Cadena vacía
- ✅ Un carácter
- ✅ Cadena larga
- ✅ Cadenas con espacios
- ✅ Caracteres especiales
- ✅ Números y letras

### Tests de Funciones Optimizadas
- ✅ Versión optimizada vs básica
- ✅ Función con retorno
- ✅ Encadenamiento de funciones

### Tests de Seguridad
- ✅ Función segura con límites
- ✅ Truncamiento de cadenas largas
- ✅ Buffer de un carácter
- ✅ Tamaño cero
- ✅ Punteros nulos

### Tests de Funciones Auxiliares
- ✅ Copia de subcadenas
- ✅ Concatenación de cadenas
- ✅ Cálculo de longitud
- ✅ Inversión de cadenas

### Tests de Casos Especiales
- ✅ Caracteres Unicode básicos
- ✅ Saltos de línea y tabs
- ✅ Caracteres de control

### Tests de Rendimiento
- ✅ Cadenas muy largas
- ✅ Comparación entre métodos
- ✅ Múltiples operaciones

### Tests de Integración
- ✅ Flujo completo de operaciones
- ✅ Operaciones encadenadas
- ✅ Comparación con stdlib

## 🚀 Compilación y Ejecución

### Prerrequisitos
- CMake 3.15 o superior
- Compilador GCC o Clang
- Criterion (para tests)

### Instalación de Dependencias

**macOS:**
```bash
brew install cmake criterion
```

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install cmake libcriterion-dev
```

### Compilación

```bash
# Crear directorio build
mkdir build && cd build

# Configurar proyecto
cmake ..

# Compilar
make
```

### Ejecución

```bash
# Ejecutar programa principal
./main

# Ejecutar tests
make test_all
# o directamente
./test_runner
```

### Targets Disponibles

```bash
make main          # Compilar solo el ejecutable principal
make test_runner    # Compilar solo los tests
make run           # Compilar y ejecutar el programa principal
make test_all      # Ejecutar todos los tests
make clean_all     # Limpieza completa
```

## 📊 Ejemplo de Salida

```
=== COPIA DE CADENAS CON PUNTEROS ===

🔹 EJEMPLO BÁSICO:
Cadena original: "Hola, mundo!"
Cadena copiada: "Hola, mundo!"

🔹 COPIA DETALLADA:
=== Copia de Cadena Paso a Paso ===
Origen: "C Programming" (dirección: 0x7fff5fbff6a0)
Destino inicial: 0x7fff5fbff650

Paso 1: copiando 'C' de 0x7fff5fbff6a0 a 0x7fff5fbff650
Paso 2: copiando ' ' de 0x7fff5fbff6a1 a 0x7fff5fbff651
Paso 3: copiando 'P' de 0x7fff5fbff6a2 a 0x7fff5fbff652
...
Paso 13: copiando 'g' de 0x7fff5fbff6ac a 0x7fff5fbff65c
Paso 14: copiando terminador '\0'

Resultado: "C Programming"
Caracteres copiados: 13
===================================

🔹 VERSIÓN SEGURA CON LÍMITES:
Cadena original: "Esta es una cadena muy larga que podría desbordar"
Cadena truncada: "Esta es una cad"
Caracteres copiados: 19

🔹 FUNCIONES AUXILIARES:
Texto original: "Programación en C"
Subcadena (pos 13, len 5): "en C"
Concatenación: "Hola " + "mundo!" = "Hola mundo!"
Longitud de "Medir longitud": 14 caracteres
Original: "abcdef"
Invertida: "fedcba"

🔹 INFORMACIÓN TÉCNICA:
Tamaño de char*: 8 bytes
Tamaño de char: 1 byte
Dirección de array 'demo': 0x7fff5fbff6bc
Dirección de demo[0]: 0x7fff5fbff6bc
¿Son iguales? Sí
demo + 1 apunta a: 'e' (dirección: 0x7fff5fbff6bd)
*(demo + 2) = 'm'
```

## 🔍 Conceptos Clave

### 1. **Arrays vs Punteros en Cadenas**
- `char str[]` vs `char *str`
- Equivalencia entre `str[i]` y `*(str + i)`
- Aritmética de punteros con caracteres

### 2. **Terminador Nulo**
- Todas las cadenas en C terminan en `'\0'`
- Crítico para determinar el final de la cadena
- Debe incluirse en cualquier copia

### 3. **Algoritmo Básico de Copia**
```c
while (*orig != '\0') {
    *dest = *orig;
    dest++;
    orig++;
}
*dest = '\0';
```

### 4. **Versión Optimizada**
```c
while ((*dest++ = *orig++) != '\0') {
    // Cuerpo vacío
}
```

### 5. **Gestión Segura de Memoria**
- Verificar tamaño del buffer destino
- Validar punteros nulos
- Prevenir desbordamientos de buffer

### 6. **Const Correctness**
- Usar `const char *` para parámetros de solo lectura
- Evitar modificaciones accidentales del origen

## ⚠️ Consideraciones Importantes

### Seguridad
- **Buffer Overflow**: Verificar que destino sea suficientemente grande
- **Punteros Nulos**: Validar parámetros antes de usar
- **Límites**: Usar versiones seguras con límite de tamaño
- **Solapamiento**: Considerar casos donde origen y destino se solapan

### Rendimiento
- Versión optimizada es más eficiente pero menos legible
- Evitar verificaciones innecesarias en bucles críticos
- Considerar alineación de memoria para cadenas largas

### Portabilidad
- Código estándar C funciona en todas las plataformas
- Tamaño de punteros puede variar (32 vs 64 bits)
- Endianness no afecta operaciones de caracteres

## 🎓 Aplicaciones Prácticas

### 1. **Procesamiento de Texto**
- Parsers y lexers
- Manipulación de strings en compiladores
- Procesamiento de archivos de configuración

### 2. **Sistemas Embebidos**
- Donde no hay biblioteca estándar disponible
- Control preciso sobre memoria
- Optimización de rendimiento

### 3. **Seguridad**
- Implementaciones seguras de funciones de cadenas
- Validación de entrada de usuario
- Prevención de vulnerabilidades

### 4. **Educación**
- Entender cómo funcionan las funciones estándar
- Desarrollar intuición sobre punteros
- Base para estructuras de datos más complejas

## 🔧 Optimizaciones Avanzadas

### 1. **Copia por Palabras**
```c
// Para cadenas muy largas, copiar por bloques de sizeof(long)
while (remaining >= sizeof(long)) {
    *(long*)dest = *(long*)orig;
    dest += sizeof(long);
    orig += sizeof(long);
    remaining -= sizeof(long);
}
```

### 2. **Detección de Terminador Rápida**
```c
// Usar operaciones bit a bit para detectar '\0' rápidamente
long word = *(long*)orig;
if ((word - 0x0101010101010101L) & ~word & 0x8080808080808080L) {
    // Hay un byte cero en la palabra
}
```

### 3. **Prefetching**
```c
// En sistemas modernos, prefetch puede mejorar rendimiento
__builtin_prefetch(orig + 64, 0, 3);
```

## 📚 Comparación con Funciones Estándar

| Función Estándar | Nuestra Implementación | Ventajas |
|------------------|------------------------|----------|
| `strcpy` | `copiarCadena_retorno` | Educativa, control total |
| `strncpy` | `copiarCadena_segura` | Más segura, siempre termina en `\0` |
| `strlen` | `longitudCadena` | Demuestra aritmética de punteros |
| `strcat` | `concatenarCadena` | Implementación transparente |
| `memmove` | `copiarCadena_overlap` | Manejo explícito de solapamiento |

## 🔍 Debugging y Herramientas

### Valgrind (Linux/macOS)
```bash
valgrind --tool=memcheck --leak-check=full ./main
```

### AddressSanitizer
```bash
gcc -fsanitize=address -g src/main.c -o main
./main
```

### Static Analysis
```bash
clang-tidy src/main.c -- -I./include
```

## 🤝 Contribución

Para contribuir a este ejercicio:

1. Fork el repositorio
2. Crea una rama para tu feature
3. Añade tests para nuevas funcionalidades
4. Asegúrate de que todos los tests pasen
5. Verifica con herramientas de análisis estático
6. Envía un pull request

## 📄 Licencia

Este proyecto es parte del curso "100 Ejercicios de C" y está disponible bajo la licencia MIT.

## 📚 Referencias

- **Kernighan & Ritchie**: "The C Programming Language", Capítulo 5
- **C11 Standard**: ISO/IEC 9899:2011, Sección 7.24 (String handling)
- **CERT C Coding Standard**: STR31-C, STR32-C
- **Criterion Documentation**: Testing framework para C
