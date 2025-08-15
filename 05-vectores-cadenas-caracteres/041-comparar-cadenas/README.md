# Ejercicio 041: Comparar Cadenas

## Descripción
Este ejercicio implementa funciones para comparar cadenas de caracteres sin utilizar la función `strcmp()` de la biblioteca estándar. Es fundamental para entender cómo funcionan las comparaciones de cadenas en C y el concepto de comparación lexicográfica.

## Conceptos Clave

### 1. Comparación de Cadenas
- **Comparación exacta:** Carácter por carácter hasta encontrar diferencia o fin
- **Comparación lexicográfica:** Determina orden alfabético (como diccionario)
- **Comparación insensible a mayúsculas:** Ignora diferencias entre mayús/minús

### 2. Algoritmos de Comparación
- **Iteración simultánea:** Recorrer ambas cadenas al mismo tiempo
- **Terminación temprana:** Parar al encontrar primera diferencia
- **Validación de longitudes:** Verificar que ambas terminen simultáneamente

### 3. Casos Especiales
- **Cadenas vacías:** Una o ambas cadenas sin caracteres
- **Prefijos:** Una cadena es prefijo de otra
- **Misma longitud:** Verificar que no haya diferencias internas

## Algoritmos Implementados

### 1. Comparación de Igualdad
```
FUNCIÓN comparar_cadenas(cadena1, cadena2):
    i = 0
    MIENTRAS cadena1[i] != '\0' Y cadena2[i] != '\0':
        SI cadena1[i] != cadena2[i]:
            RETORNAR 0  // Son diferentes
        i++
    RETORNAR (cadena1[i] == '\0' Y cadena2[i] == '\0') ? 1 : 0
```

### 2. Comparación Lexicográfica
```
FUNCIÓN comparar_cadenas_ordenado(cadena1, cadena2):
    i = 0
    MIENTRAS cadena1[i] != '\0' Y cadena2[i] != '\0':
        SI cadena1[i] != cadena2[i]:
            RETORNAR cadena1[i] - cadena2[i]
        i++
    RETORNAR cadena1[i] - cadena2[i]  // Diferencia de longitudes
```

### 3. Comparación Sin Distinguir Mayúsculas
```
FUNCIÓN comparar_sin_caso(cadena1, cadena2):
    PARA cada carácter:
        c1 = convertir_a_minuscula(cadena1[i])
        c2 = convertir_a_minuscula(cadena2[i])
        SI c1 != c2: RETORNAR 0
    RETORNAR 1
```

### Complejidad
- **Tiempo:** O(min(n, m)) donde n y m son las longitudes de las cadenas
- **Espacio:** O(1) - solo variables auxiliares

## Código de Ejemplo

### Implementación Básica
```c
int comparar_cadenas(const char *cadena1, const char *cadena2) {
    int i = 0;
    
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        if (cadena1[i] != cadena2[i]) {
            return 0; // Son diferentes
        }
        i++;
    }
    
    return (cadena1[i] == '\0' && cadena2[i] == '\0') ? 1 : 0;
}
```

### Versión con Punteros
```c
int comparar_con_punteros(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*s1 == *s2);
}
```

### Comparación Robusta (como strcmp)
```c
int mi_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}
```

## Casos de Prueba

### Casos Básicos
- Cadenas iguales: `"hola"` vs `"hola"` → iguales
- Cadenas diferentes: `"hola"` vs `"mundo"` → diferentes
- Cadenas vacías: `""` vs `""` → iguales

### Casos de Longitud
- Misma longitud: `"test"` vs `"best"` → diferentes
- Diferente longitud: `"hola"` vs `"holanda"` → diferentes
- Una vacía: `""` vs `"hola"` → diferentes

### Casos de Mayúsculas
- Mismo caso: `"HOLA"` vs `"HOLA"` → iguales
- Diferente caso: `"Hola"` vs `"hola"` → diferentes (exacta), iguales (sin caso)
- Mixto: `"HoLa"` vs `"hOlA"` → diferentes (exacta), iguales (sin caso)

### Casos Especiales
- Con espacios: `"hola mundo"` vs `"hola mundo"` → iguales
- Caracteres especiales: `"!@#$"` vs `"!@#$"` → iguales
- Solo números: `"123"` vs `"123"` → iguales

## Compilación y Ejecución

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make
./comparar_cadenas
```

### Compilación Manual
```bash
gcc -Wall -Wextra -std=c99 src/main.c -o comparar_cadenas
./comparar_cadenas
```

### Ejecutar Tests
```bash
# Instalar Criterion primero (macOS)
brew install criterion

# Compilar y ejecutar tests
cd build
make run_tests
```

## Ejemplo de Uso

### Entrada
```
Introduce la primera cadena: Hola Mundo
Introduce la segunda cadena: hola mundo
```

### Salida
```
=== Comparador de Cadenas ===
Introduce la primera cadena: Hola Mundo
Introduce la segunda cadena: hola mundo

=== Resultados de Comparación ===
Cadena 1: "Hola Mundo" (longitud: 10)
Cadena 2: "hola mundo" (longitud: 10)

1. Comparación exacta: Las cadenas son DIFERENTES
2. Comparación ordenada: "Hola Mundo" es MENOR que "hola mundo"
3. Comparación sin caso: Las cadenas son IGUALES (ignorando mayús/minús)

=== Análisis de Diferencias ===
Primera diferencia en posición 0:
  Cadena 1[0]: 'H' (ASCII: 72)
  Cadena 2[0]: 'h' (ASCII: 104)
Diferencia de longitudes: 0

=== Comparación Carácter por Carácter ===
Pos  0: 'H'( 72) vs 'h'(104) -> ✗
Pos  1: 'o'(111) vs 'o'(111) -> ✓
Pos  2: 'l'(108) vs 'l'(108) -> ✓
Pos  3: 'a'( 97) vs 'a'( 97) -> ✓
Pos  4: ' '( 32) vs ' '( 32) -> ✓
Pos  5: 'M'( 77) vs 'm'(109) -> ✗
Pos  6: 'u'(117) vs 'u'(117) -> ✓
Pos  7: 'n'(110) vs 'n'(110) -> ✓
Pos  8: 'd'(100) vs 'd'(100) -> ✓
Pos  9: 'o'(111) vs 'o'(111) -> ✓
```

## Aspectos Pedagógicos

### Conceptos que Refuerza
1. **Recorrido simultáneo de arrays**
2. **Comparación de caracteres individuales**
3. **Manejo de terminadores nulos**
4. **Lógica de terminación temprana**
5. **Diferencia entre igualdad y orden lexicográfico**

### Patrones de Programación
1. **Bucle con múltiples condiciones:** `while (a && b && condition)`
2. **Retorno temprano:** Salir del bucle al encontrar diferencia
3. **Validación post-bucle:** Verificar estado final
4. **Comparación robusta:** Manejar todos los casos edge

### Errores Comunes
1. **No verificar ambos terminadores:** Solo verificar una cadena
2. **Comparación incorrecta de longitudes:** No detectar prefijos
3. **Desbordamiento de buffer:** No validar límites
4. **Sensibilidad a mayúsculas:** No distinguir tipos de comparación

### Variaciones Educativas
1. **Con límite de caracteres:** Comparar solo N caracteres
2. **Búsqueda de subcadenas:** Encontrar una dentro de otra
3. **Comparación de arrays numéricos:** Aplicar a otros tipos
4. **Implementación recursiva:** Versión recursiva del algoritmo

## Aplicaciones Prácticas

### En Sistemas Reales
- **Sistemas de archivos:** Comparación de nombres de archivo
- **Bases de datos:** Índices y búsquedas en texto
- **Interfaces de usuario:** Validación de entrada, autocompletado
- **Protocolos de red:** Comparación de headers y comandos

### Optimizaciones Avanzadas
- **Comparación por palabras:** Comparar 4/8 bytes a la vez
- **SIMD:** Instrucciones vectoriales para cadenas largas
- **Tablas de lookup:** Pre-computar conversiones de mayús/minús
- **Branch prediction:** Optimizar para casos comunes

## Relación con Funciones Estándar

### Equivalencias
```c
// strcmp() equivalente
int mi_strcmp(const char *s1, const char *s2);

// strcasecmp() equivalente  
int mi_strcasecmp(const char *s1, const char *s2);

// strncmp() equivalente
int mi_strncmp(const char *s1, const char *s2, size_t n);
```

### Diferencias con Estándar
- **Manejo de caracteres signed/unsigned**
- **Optimizaciones específicas del compilador**
- **Soporte para locales específicos**
- **Verificaciones adicionales de seguridad**

## Extensiones Posibles

### Funcionalidades Adicionales
```c
// Comparar ignorando espacios
int comparar_sin_espacios(const char *s1, const char *s2);

// Comparar solo los primeros N caracteres
int comparar_n_caracteres(const char *s1, const char *s2, int n);

// Encontrar la distancia de edición entre cadenas
int distancia_levenshtein(const char *s1, const char *s2);

// Comparación fuzzy (aproximada)
double similitud_cadenas(const char *s1, const char *s2);
```

## Relación con Otros Ejercicios
- **Prerequisito de:** Búsqueda en cadenas, ordenamiento de strings
- **Extiende:** Longitud de cadenas, manipulación básica
- **Combina con:** Inversión de cadenas, detección de palíndromos

## Recursos Adicionales
- Manual de C: Sección sobre string.h y funciones de comparación
- Algoritmos de comparación de strings en "Programming Pearls"
- Optimizaciones de rendimiento en sistemas de bajo nivel
