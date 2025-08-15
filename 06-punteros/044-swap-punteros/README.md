# Ejercicio 044: Swap con Punteros

## 📋 Descripción

Este ejercicio explora el intercambio de valores entre variables utilizando punteros en C. Es fundamental para entender cómo los punteros permiten modificar valores de variables desde funciones externas, un concepto clave en la programación en C.

## 🎯 Objetivo

Implementar y demostrar diferentes métodos para intercambiar valores de variables utilizando punteros, mostrando la diferencia entre paso por valor y paso por referencia.

## 📝 Enunciado

**Intercambio de Valores con Punteros:**

Crear una función que intercambie los valores de dos variables enteras utilizando punteros. La función debe:

1. Recibir como parámetros dos punteros a enteros
2. Intercambiar los valores apuntados por estos punteros
3. Los cambios deben ser visibles desde la función que llama

**Ejemplo:**
```c
int x = 5, y = 10;
printf("Antes: x = %d, y = %d\n", x, y);
swap(&x, &y);
printf("Después: x = %d, y = %d\n", x, y);
```

**Salida esperada:**
```
Antes: x = 5, y = 10
Después: x = 10, y = 5
```

## 🔧 Funciones Implementadas

### Funciones Principales

1. **`swap(int *a, int *b)`**
   - Intercambio básico usando variable temporal
   - Método estándar y más legible

2. **`swap_xor(int *a, int *b)`**
   - Intercambio usando operador XOR
   - No requiere variable temporal
   - Incluye verificación de seguridad

3. **`swap_aritmetico(int *a, int *b)`**
   - Intercambio usando aritmética
   - Puede causar overflow en casos extremos

4. **`swap_generico(void *a, void *b, size_t tamaño)`**
   - Función genérica para cualquier tipo de dato
   - Utiliza punteros void y manipulación byte a byte

### Funciones Especializadas

5. **`swap_float(float *a, float *b)`**
   - Intercambio específico para números flotantes

6. **`swap_char(char *a, char *b)`**
   - Intercambio específico para caracteres

7. **`swap_punteros(int **a, int **b)`**
   - Intercambio de punteros (direcciones)
   - Demuestra punteros a punteros

### Funciones Auxiliares y Educativas

8. **`swap_con_debug(int *a, int *b)`**
   - Intercambio con información detallada paso a paso
   - Muestra direcciones de memoria

9. **`swap_incorrecto(int a, int b)`**
   - Ejemplo de método incorrecto (paso por valor)
   - Demuestra por qué no funciona

10. **`swap_seguro(int *a, int *b)`**
    - Intercambio con validación de punteros nulos
    - Práctica de programación defensiva

11. **`swap_array_elementos(int array[], int i, int j)`**
    - Intercambio de elementos específicos en un array

## 🏗️ Estructura del Proyecto

```
044-swap-punteros/
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
- ✅ Intercambio de valores diferentes
- ✅ Valores positivos y negativos
- ✅ Intercambio con cero
- ✅ Valores iguales
- ✅ Valores extremos (INT_MAX, INT_MIN)

### Tests de Métodos Alternativos
- ✅ Método XOR con diferentes valores
- ✅ Método aritmético
- ✅ Intercambio consigo mismo (misma dirección)

### Tests de Tipos Específicos
- ✅ Intercambio de floats
- ✅ Intercambio de caracteres
- ✅ Función genérica con diferentes tipos

### Tests de Punteros
- ✅ Intercambio de punteros
- ✅ Punteros al mismo valor
- ✅ Punteros a arrays

### Tests de Seguridad
- ✅ Punteros nulos (primer parámetro)
- ✅ Punteros nulos (segundo parámetro)
- ✅ Ambos punteros nulos

### Tests de Arrays
- ✅ Intercambio de elementos por índice
- ✅ Índices adyacentes
- ✅ Mismo índice
- ✅ Array de dos elementos

### Tests Avanzados
- ✅ Intercambios múltiples
- ✅ Intercambio en bucles
- ✅ Intercambios anidados
- ✅ Tests de rendimiento
- ✅ Comparación entre métodos

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
=== INTERCAMBIO DE VALORES CON PUNTEROS ===

🔹 EJEMPLO BÁSICO:
Antes del intercambio:
x = 5, y = 10
Después del intercambio:
x = 10, y = 5

🔹 INTERCAMBIO DETALLADO:
=== Intercambio Detallado ===
Antes del intercambio:
  *a = 15 (dirección: 0x7fff5fbff6ac)
  *b = 25 (dirección: 0x7fff5fbff6a8)

Paso 1: int temp = *a;
  temp = 15

Paso 2: *a = *b;
  *a = 25

Paso 3: *b = temp;
  *b = 15

Después del intercambio:
  *a = 25 (dirección: 0x7fff5fbff6ac)
  *b = 15 (dirección: 0x7fff5fbff6a8)
========================

🔹 MÉTODOS ALTERNATIVOS:
Método XOR - Antes: m = 50, n = 75
Método XOR - Después: m = 75, n = 50

Método aritmético - Antes: r = 30, s = 40
Método aritmético - Después: r = 40, s = 30

🔹 INTERCAMBIO CON DIFERENTES TIPOS:
Floats - Antes: f1 = 3.14, f2 = 2.71
Floats - Después: f1 = 2.71, f2 = 3.14

🔹 INTERCAMBIO SEGURO:
Intercambio válido:
Intercambio seguro: 88 <-> 99
Resultado: 99 <-> 88
Intento con puntero nulo:
Error: Uno o ambos punteros son NULL
```

## 🔍 Conceptos Clave

### 1. **Paso por Referencia vs Paso por Valor**
- Los punteros permiten modificar valores originales
- Paso por valor solo modifica copias locales

### 2. **Desreferenciación de Punteros**
- Usar `*` para acceder al valor apuntado
- Usar `&` para obtener la dirección de una variable

### 3. **Algoritmo de Intercambio Clásico**
```c
int temp = *a;    // Guardar valor de a
*a = *b;          // a toma el valor de b
*b = temp;        // b toma el valor original de a
```

### 4. **Métodos Alternativos**
- **XOR**: `a^=b; b^=a; a^=b;` (sin variable temporal)
- **Aritmético**: `a+=b; b=a-b; a-=b;` (riesgo de overflow)

### 5. **Punteros a Punteros**
- `int **ptr` permite modificar la dirección apuntada
- Útil para funciones que modifican punteros

### 6. **Programación Defensiva**
- Siempre verificar punteros nulos
- Manejar casos especiales (misma dirección)

## ⚠️ Consideraciones Importantes

### Seguridad
- **Verificar punteros nulos** antes de desreferenciar
- **Evitar intercambio consigo mismo** en métodos XOR/aritmético
- **Considerar overflow** en método aritmético

### Rendimiento
- Método clásico es el más eficiente y legible
- XOR puede ser útil en contextos específicos
- Función genérica tiene overhead pero es flexible

### Portabilidad
- Todos los métodos son estándar C11
- Addresses de memoria pueden variar entre ejecuciones
- Comportamiento consistente en diferentes plataformas

## 🎓 Aplicaciones Prácticas

1. **Algoritmos de Ordenamiento**
   - Bubble sort, selection sort, quick sort
   - Intercambio de elementos en arrays

2. **Manipulación de Estructuras de Datos**
   - Listas enlazadas
   - Árboles binarios
   - Intercambio de nodos

3. **Optimización de Memoria**
   - Intercambio de buffers
   - Rotación de variables temporales

4. **Interfaces de Usuario**
   - Intercambio de estados
   - Alternancia de configuraciones

## 📚 Referencias

- **Kernighan & Ritchie**: "The C Programming Language"
- **C11 Standard**: ISO/IEC 9899:2011
- **Criterion Documentation**: Testing framework
- **CMake Documentation**: Build system
