# Ejercicio 045: Recorrer Array con Punteros

## 📋 Descripción

Este ejercicio introduce una técnica fundamental en C: el uso de punteros para recorrer estructuras de datos como arrays. Demuestra cómo la aritmética de punteros permite un control más directo y eficiente sobre la memoria, y cómo `array[i]` es equivalente a `*(array + i)`.

## 🎯 Objetivo

Implementar y demostrar diferentes métodos para recorrer arrays utilizando aritmética de punteros en lugar de índices tradicionales, estableciendo los fundamentos para programación de bajo nivel y manipulación directa de memoria.

## 📝 Enunciado

**Suma de Elementos usando Aritmética de Punteros:**

Escribir un programa que sume todos los elementos de un array de enteros utilizando aritmética de punteros en lugar de índices. El programa debe:

1. Declarar un array de enteros con valores iniciales
2. Usar un puntero para recorrer el array
3. Sumar todos los elementos usando desreferenciación de punteros
4. Mostrar el resultado final

**Ejemplo de código base:**
```c
int valores[] = {3, 7, 2, 8, 4};
int suma = 0;
int *ptr;

for (ptr = valores; ptr < valores + 5; ptr++) {
    suma += *ptr;
}

printf("La suma de los elementos es: %d\n", suma);
```

**Salida esperada:**
```
La suma de los elementos es: 24
```

## 🔧 Funciones Implementadas

### Funciones Principales

1. **`suma_array_punteros(int *array, size_t tamaño)`**
   - Suma básica usando aritmética de punteros
   - Implementación eficiente y directa

2. **`suma_array_debug(int *array, size_t tamaño)`**
   - Suma con información detallada de debug
   - Muestra direcciones de memoria y offsets
   - Educativo para entender la aritmética de punteros

3. **`suma_array_estilos(int *array, size_t tamaño)`**
   - Demuestra tres estilos diferentes de recorrido
   - Incremento de puntero, offset y comparación

### Funciones de Demostración

4. **`comparar_metodos(int *array, size_t tamaño)`**
   - Compara recorrido por índices vs punteros
   - Muestra equivalencias importantes

5. **`demo_aritmetica_punteros(int *array, size_t tamaño)`**
   - Demuestra conceptos de aritmética de punteros
   - Explica diferencias de direcciones y tamaños

6. **`suma_array_reverso(int *array, size_t tamaño)`**
   - Recorre el array desde el final hacia el inicio
   - Demuestra flexibilidad de los punteros

### Funciones de Utilidad

7. **`encontrar_max_min_punteros(int *array, size_t tamaño, int *max, int *min)`**
   - Encuentra valores máximo y mínimo usando punteros

8. **`contar_con_condicion(int *array, size_t tamaño, int (*condicion)(int))`**
   - Cuenta elementos que cumplen una condición
   - Demuestra punteros a funciones

9. **`copiar_array_punteros(int *origen, int *destino, size_t tamaño)`**
   - Copia arrays usando solo punteros

### Funciones de Condición

10. **`es_par(int num)`**, **`es_positivo(int num)`**, **`es_mayor_que_cinco(int num)`**
    - Funciones de condición para demostrar punteros a funciones

## 🏗️ Estructura del Proyecto

```
045-recorrer-array-punteros/
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

### Tests de Suma Básica
- ✅ Array básico del enunciado
- ✅ Array de un elemento
- ✅ Array vacío
- ✅ Array de dos elementos
- ✅ Arrays con valores positivos, negativos y mixtos
- ✅ Array con ceros
- ✅ Array grande (1-100)
- ✅ Valores extremos

### Tests de Equivalencia
- ✅ Suma debug vs suma básica
- ✅ Diferentes estilos de recorrido
- ✅ Suma normal vs suma reversa

### Tests de Búsqueda
- ✅ Máximo y mínimo en diferentes arrays
- ✅ Array de un elemento
- ✅ Todos los elementos iguales
- ✅ Arrays con negativos y valores extremos

### Tests de Condiciones
- ✅ Funciones es_par, es_positivo, es_mayor_que_cinco
- ✅ Conteo con diferentes condiciones
- ✅ Arrays donde ninguno/todos cumplen condición

### Tests de Copia
- ✅ Copia básica de arrays
- ✅ Arrays grandes
- ✅ Independencia entre original y copia

### Tests Avanzados
- ✅ Casos especiales (overflow controlado)
- ✅ Secuencias aritméticas y Fibonacci
- ✅ Patrones alternantes
- ✅ Tests de rendimiento
- ✅ Integración completa

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
=== RECORRER ARRAY CON PUNTEROS ===

🔹 EJEMPLO BÁSICO:
Array: 3 7 2 8 4 

Recorrido con punteros:
  *ptr = 3 (dirección: 0x7fff5fbff6a0)
  *ptr = 7 (dirección: 0x7fff5fbff6a4)
  *ptr = 2 (dirección: 0x7fff5fbff6a8)
  *ptr = 8 (dirección: 0x7fff5fbff6ac)
  *ptr = 4 (dirección: 0x7fff5fbff6b0)

La suma de los elementos es: 24

🔹 ANÁLISIS DETALLADO:
=== Recorrido detallado del array ===
Dirección base del array: 0x7fff5fbff690
Tamaño del array: 5 elementos
Tamaño de cada elemento: 4 bytes

Posición 0:
  Dirección: 0x7fff5fbff690
  Valor: 10
  Offset desde base: 0 bytes
  array[0] == *(array + 0) == 10
  Suma parcial: 10

Posición 1:
  Dirección: 0x7fff5fbff694
  Valor: 20
  Offset desde base: 4 bytes
  array[1] == *(array + 1) == 20
  Suma parcial: 30

🔹 DIFERENTES ESTILOS DE ARITMÉTICA DE PUNTEROS:

Estilo 1 - Incremento del puntero:
  *ptr1 = 10
  *ptr1 = 20
  *ptr1 = 30
  *ptr1 = 40
  *ptr1 = 50
  Suma: 150

Estilo 2 - Puntero con offset:
  *(array + 0) = 10
  *(array + 1) = 20
  *(array + 2) = 30
  *(array + 3) = 40
  *(array + 4) = 50
  Suma: 150

Estilo 3 - Comparación de punteros:
  *ptr3 = 10 (dirección: 0x7fff5fbff690)
  *ptr3 = 20 (dirección: 0x7fff5fbff694)
  *ptr3 = 30 (dirección: 0x7fff5fbff698)
  *ptr3 = 40 (dirección: 0x7fff5fbff69c)
  *ptr3 = 50 (dirección: 0x7fff5fbff6a0)
  Suma: 150

Verificación - Todas las sumas son iguales: ✓ SÍ
```

## 🔍 Conceptos Clave

### 1. **Equivalencia Fundamental**
```c
array[i] ≡ *(array + i)
&array[i] ≡ (array + i)
array ≡ &array[0]
```

### 2. **Aritmética de Punteros**
- `ptr++` avanza `sizeof(tipo)` bytes
- `ptr + n` avanza `n * sizeof(tipo)` bytes
- La diferencia entre punteros da número de elementos

### 3. **Estilos de Recorrido**

**Estilo 1: Incremento directo**
```c
for (ptr = array; ptr < array + tamaño; ptr++) {
    suma += *ptr;
}
```

**Estilo 2: Offset**
```c
for (i = 0; i < tamaño; i++) {
    suma += *(array + i);
}
```

**Estilo 3: Punteros de inicio y fin**
```c
int *end = array + tamaño;
while (ptr < end) {
    suma += *ptr++;
}
```

### 4. **Direcciones de Memoria**
- Las direcciones están separadas por `sizeof(tipo)` bytes
- En sistemas de 64 bits, `int` típicamente ocupa 4 bytes
- La aritmética es manejada automáticamente por el compilador

### 5. **Punteros a Funciones**
```c
int (*condicion)(int) = es_par;
int resultado = condicion(numero);
```

### 6. **Recorrido Bidireccional**
- Forward: `ptr++` desde inicio
- Backward: `ptr--` desde final
- Ambos son igualmente eficientes

## ⚠️ Consideraciones Importantes

### Seguridad
- **Verificar límites**: Nunca acceder fuera del array
- **Punteros válidos**: Asegurar que apuntan a memoria válida
- **Tamaño correcto**: Usar `sizeof(array)/sizeof(array[0])` para arrays automáticos

### Rendimiento
- Aritmética de punteros es típicamente más eficiente
- El compilador optimiza muchas diferencias
- Importante para código crítico en rendimiento

### Portabilidad
- `sizeof(int)` puede variar entre sistemas
- Usar tipos específicos (`int32_t`) cuando sea necesario
- Las direcciones de memoria varían entre ejecuciones

### Estilo de Código
- Balance entre eficiencia y legibilidad
- Punteros para código de bajo nivel
- Índices para código legible y educativo

## 🎓 Aplicaciones Prácticas

1. **Algoritmos de Ordenamiento**
   - Quick sort, merge sort
   - Manipulación eficiente de subarrays

2. **Procesamiento de Strings**
   - Búsqueda de patrones
   - Manipulación carácter por carácter

3. **Estructuras de Datos**
   - Listas enlazadas
   - Implementación de stacks y queues

4. **Programación de Sistemas**
   - Drivers de dispositivos
   - Manipulación directa de memoria

5. **Optimización de Rendimiento**
   - Loops críticos
   - Procesamiento de grandes volúmenes de datos

6. **Interfaces con Hardware**
   - Memory-mapped I/O
   - Buffers de comunicación

## 🔗 Relación con Otros Conceptos

### Arrays y Punteros
- Array name decays to pointer
- Paso por referencia automático
- No hay diferencia real en tiempo de ejecución

### Memoria y Cache
- Acceso secuencial mejora cache hit
- Localidad de referencia
- Predicción de branch

### Tipos de Datos
- Aritmética funciona con cualquier tipo
- `void*` para funciones genéricas
- Cast necesario para aritmética en `void*`

## 📚 Referencias

- **Kernighan & Ritchie**: "The C Programming Language", Capítulo 5
- **C11 Standard**: ISO/IEC 9899:2011, Sección 6.5.6 (Additive operators)
- **Criterion Documentation**: Testing framework para C
- **CMake Documentation**: Build system
