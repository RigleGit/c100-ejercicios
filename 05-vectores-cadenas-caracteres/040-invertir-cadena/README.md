# Ejercicio 040: Invertir Cadena

## Descripción
Este ejercicio implementa funciones para invertir cadenas de caracteres sin usar funciones de la biblioteca estándar como `strrev()`. Es fundamental para entender la manipulación de arrays y el concepto de índices en C.

## Conceptos Clave

### 1. Inversión de Cadenas
- **In-place:** Modifica la cadena original intercambiando caracteres
- **Por copia:** Crea una nueva cadena con los caracteres en orden inverso
- **Por impresión:** Muestra la cadena invertida sin modificar la original

### 2. Algoritmos de Intercambio
- Intercambio desde los extremos hacia el centro
- Uso de variable temporal para el intercambio
- Optimización: solo recorrer la mitad de la cadena

### 3. Manejo de Índices
- Acceso por posición: `cadena[i]`
- Cálculo de posición inversa: `cadena[longitud - 1 - i]`
- Iteración desde el final: `for (i = longitud - 1; i >= 0; i--)`

## Algoritmos Implementados

### 1. Inversión In-Place
```
PARA i desde 0 hasta longitud/2:
    temporal = cadena[i]
    cadena[i] = cadena[longitud - 1 - i]
    cadena[longitud - 1 - i] = temporal
```

### 2. Copia Invertida
```
PARA i desde 0 hasta longitud:
    destino[i] = origen[longitud - 1 - i]
destino[longitud] = '\0'
```

### 3. Impresión Invertida
```
PARA i desde longitud-1 hasta 0:
    imprimir(cadena[i])
```

### Complejidad
- **Tiempo:** O(n) donde n es la longitud de la cadena
- **Espacio:** O(1) para in-place, O(n) para copia

## Código de Ejemplo

### Implementación In-Place
```c
void invertir_cadena_inplace(char *cadena, int longitud) {
    for (int i = 0; i < longitud / 2; i++) {
        char temp = cadena[i];
        cadena[i] = cadena[longitud - 1 - i];
        cadena[longitud - 1 - i] = temp;
    }
}
```

### Implementación por Copia
```c
void copiar_cadena_invertida(const char *origen, char *destino, int longitud) {
    for (int i = 0; i < longitud; i++) {
        destino[i] = origen[longitud - 1 - i];
    }
    destino[longitud] = '\0';
}
```

### Implementación Recursiva (Avanzada)
```c
void invertir_recursivo(char *cadena, int inicio, int fin) {
    if (inicio >= fin) return;
    
    char temp = cadena[inicio];
    cadena[inicio] = cadena[fin];
    cadena[fin] = temp;
    
    invertir_recursivo(cadena, inicio + 1, fin - 1);
}
```

## Ejemplo de Uso

### Entrada
```
Introduce una cadena: Hola mundo
```

### Salida
```
=== Inversor de Cadenas ===
Introduce una cadena: Hola mundo

=== Resultados ===
Cadena original: "Hola mundo"
Longitud: 10 caracteres
Cadena invertida (método 1): "odnum aloH"
Cadena invertida (método 2): "odnum aloH"

=== Demostración de inversión in-place ===
Antes de invertir in-place: "Hola mundo"
Después de invertir in-place: "odnum aloH"

=== Análisis de caracteres ===
Posiciones de caracteres en la cadena invertida:
  [0]: 'o' (ASCII: 111)
  [1]: 'd' (ASCII: 100)
  [2]: 'n' (ASCII: 110)
  [3]: 'u' (ASCII: 117)
  [4]: 'm' (ASCII: 109)
  [5]: ' ' (ASCII: 32)
  [6]: 'a' (ASCII: 97)
  [7]: 'l' (ASCII: 111)
  [8]: 'o' (ASCII: 111)
  [9]: 'H' (ASCII: 72)
```

## Aspectos Pedagógicos

### Conceptos que Refuerza
1. **Manipulación de arrays de caracteres**
2. **Acceso por índice y aritmética de índices**
3. **Algoritmos de intercambio**
4. **Diferencia entre modificación y copia**
5. **Optimización de bucles (recorrer solo la mitad)**

### Patrones de Programación
1. **Intercambio de variables:** Uso de variable temporal
2. **Iteración simétrica:** Desde extremos hacia el centro
3. **Preservación de datos:** Copiar vs. modificar in-place
4. **Validación de entrada:** Manejo de casos especiales

### Errores Comunes
1. **Recorrer toda la cadena:** Causa doble inversión
2. **Olvidar el terminador nulo:** En funciones de copia
3. **Desbordamiento de índices:** No validar longitud
4. **Modificar const strings:** Intentar modificar literales

### Variaciones Educativas
1. **Con punteros:** Usar aritmética de punteros
2. **Recursiva:** Implementación recursiva del algoritmo
3. **Por palabras:** Invertir palabras en lugar de caracteres
4. **Con límites:** Invertir solo una parte de la cadena

## Aplicaciones Prácticas

### En Sistemas Reales
- **Algoritmos de cadenas:** Palíndromos, reversión de texto
- **Criptografía básica:** Transformaciones simples
- **Interfaz de usuario:** Efectos de texto, animaciones
- **Protocolos:** Reversión de números en little/big endian

### Optimizaciones Avanzadas
- **SIMD:** Uso de instrucciones vectoriales para cadenas largas
- **Cache-friendly:** Optimización para jerarquía de memoria
- **Parallelization:** División del trabajo en múltiples hilos
- **In-situ:** Minimización del uso de memoria adicional

## Extensiones Posibles

### Funcionalidades Adicionales
```c
// Invertir solo palabras (mantener espacios)
void invertir_palabras(char *cadena);

// Invertir con límites
void invertir_rango(char *cadena, int inicio, int fin);

// Invertir ignorando mayúsculas/minúsculas
void invertir_case_insensitive(char *cadena);

// Invertir solo caracteres alfabéticos
void invertir_solo_letras(char *cadena);
```

### Análisis de Rendimiento
- Comparación entre métodos in-place vs. copia
- Medición de tiempo para cadenas de diferentes tamaños
- Análisis de uso de memoria

## Relación con Otros Ejercicios
- **Prerequisito de:** Detección de palíndromos, rotación de cadenas
- **Combina con:** Búsqueda en cadenas, comparación de cadenas
- **Extiende:** Longitud de cadenas, manipulación básica de caracteres

## Recursos Adicionales
- Algoritmos de reversión en "Introduction to Algorithms" (CLRS)
- Optimizaciones de cadenas en sistemas de bajo nivel
- Patrones de manipulación de memoria en C

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/invertir_cadena
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
