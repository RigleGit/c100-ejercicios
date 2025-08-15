# Ejercicio 042: Búsqueda de Subcadena

## Descripción
Este ejercicio implementa funciones para buscar subcadenas dentro de cadenas de texto sin utilizar la función `strstr()` de la biblioteca estándar. Es fundamental para entender algoritmos de búsqueda de patrones en texto y procesamiento de cadenas.

## Conceptos Clave

### 1. Búsqueda de Patrones
- **Búsqueda simple:** Comparación carácter por carácter
- **Búsqueda múltiple:** Encontrar todas las ocurrencias
- **Búsqueda insensible:** Ignorar mayúsculas y minúsculas
- **Búsqueda reversa:** Encontrar la última ocurrencia

### 2. Algoritmos de Búsqueda
- **Fuerza bruta:** Comparar en cada posición posible
- **Optimizaciones:** Terminación temprana, salto de caracteres
- **Casos especiales:** Subcadenas vacías, solapamientos

### 3. Análisis de Complejidad
- **Tiempo:** O(n * m) en el peor caso (n = longitud texto, m = longitud patrón)
- **Espacio:** O(1) para búsqueda simple, O(k) para múltiples ocurrencias
- **Optimizaciones posibles:** KMP, Boyer-Moore, Rabin-Karp

## Algoritmos Implementados

### 1. Búsqueda Básica (Primera Ocurrencia)
```
FUNCIÓN buscar_subcadena(cadena, subcadena):
    PARA i desde 0 hasta longitud(cadena):
        coincidir = VERDADERO
        PARA j desde 0 hasta longitud(subcadena):
            SI cadena[i+j] != subcadena[j]:
                coincidir = FALSO
                SALIR
        SI coincidir:
            RETORNAR i
    RETORNAR -1
```

### 2. Búsqueda de Todas las Ocurrencias
```
FUNCIÓN buscar_todas(cadena, subcadena, posiciones[]):
    count = 0
    inicio = 0
    MIENTRAS VERDADERO:
        pos = buscar_subcadena(cadena + inicio, subcadena)
        SI pos == -1: SALIR
        posiciones[count] = inicio + pos
        count++
        inicio = inicio + pos + 1
    RETORNAR count
```

### 3. Búsqueda Insensible a Mayúsculas
```
FUNCIÓN buscar_sin_caso(cadena, subcadena):
    PARA cada posición:
        PARA cada carácter:
            c1 = convertir_minuscula(cadena[i+j])
            c2 = convertir_minuscula(subcadena[j])
            SI c1 != c2: SALIR
        SI coincide completamente: RETORNAR i
    RETORNAR -1
```

### Complejidad
- **Búsqueda simple:** O(n * m)
- **Todas las ocurrencias:** O(n * m * k) donde k es el número de ocurrencias
- **Espacio:** O(1) para simple, O(k) para múltiples

## Código de Ejemplo

### Implementación Básica
```c
int buscar_subcadena(const char *cadena, const char *subcadena) {
    int i, j;
    
    if (subcadena[0] == '\0') {
        return 0;  // Subcadena vacía
    }
    
    for (i = 0; cadena[i] != '\0'; i++) {
        for (j = 0; subcadena[j] != '\0'; j++) {
            if (cadena[i + j] == '\0' || cadena[i + j] != subcadena[j]) {
                break;
            }
        }
        
        if (subcadena[j] == '\0') {
            return i;  // Encontrada
        }
    }
    
    return -1;  // No encontrada
}
```

### Búsqueda con Punteros
```c
int buscar_con_punteros(const char *texto, const char *patron) {
    const char *t = texto;
    const char *inicio = texto;
    
    while (*t) {
        const char *t_temp = t;
        const char *p = patron;
        
        while (*t_temp && *p && *t_temp == *p) {
            t_temp++;
            p++;
        }
        
        if (*p == '\0') {
            return t - inicio;  // Encontrado
        }
        
        t++;
    }
    
    return -1;  // No encontrado
}
```

### Algoritmo KMP (Avanzado)
```c
void construir_tabla_fallos(const char *patron, int *tabla, int m) {
    int longitud = 0;
    tabla[0] = 0;
    
    for (int i = 1; i < m; i++) {
        if (patron[i] == patron[longitud]) {
            longitud++;
            tabla[i] = longitud;
        } else {
            if (longitud != 0) {
                longitud = tabla[longitud - 1];
                i--;  // No incrementar i
            } else {
                tabla[i] = 0;
            }
        }
    }
}

int buscar_kmp(const char *texto, const char *patron) {
    int n = strlen(texto);
    int m = strlen(patron);
    int tabla[m];
    
    construir_tabla_fallos(patron, tabla, m);
    
    int i = 0, j = 0;
    while (i < n) {
        if (patron[j] == texto[i]) {
            i++;
            j++;
        }
        
        if (j == m) {
            return i - j;  // Encontrado
        } else if (i < n && patron[j] != texto[i]) {
            if (j != 0) {
                j = tabla[j - 1];
            } else {
                i++;
            }
        }
    }
    
    return -1;  // No encontrado
}
```

## Casos de Prueba

### Casos Básicos
- Al inicio: `"hola mundo"` + `"hola"` → posición 0
- Al final: `"hola mundo"` + `"mundo"` → posición 5
- En medio: `"hola mundo feliz"` + `"mundo"` → posición 5
- No encontrada: `"hola mundo"` + `"adiós"` → -1

### Casos Especiales
- Subcadena vacía: `"texto"` + `""` → posición 0
- Cadena vacía: `""` + `"texto"` → -1
- Iguales: `"test"` + `"test"` → posición 0
- Subcadena más larga: `"hi"` + `"hello"` → -1

### Casos Complejos
- Solapamiento: `"aaaa"` + `"aa"` → múltiples posiciones
- Repetición: `"abababab"` + `"abab"` → múltiples ocurrencias
- Mayúsculas: `"Hola"` + `"hola"` → -1 (exacta), 0 (sin caso)

### Casos de Rendimiento
- Peor caso: `"aaaaaaaaab"` + `"aaab"` → muchas comparaciones
- Mejor caso: `"abcdefgh"` + `"xyz"` → pocas comparaciones

## Compilación y Ejecución

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make
./busqueda_subcadena
```

### Compilación Manual
```bash
gcc -Wall -Wextra -std=c99 src/main.c -o busqueda_subcadena
./busqueda_subcadena
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
Introduce el texto principal: La programación en C es fundamental para programadores
Introduce la subcadena a buscar: programa
```

### Salida
```
=== Buscador de Subcadenas ===
Introduce el texto principal: La programación en C es fundamental para programadores
Introduce la subcadena a buscar: programa

=== Datos de Entrada ===
Texto principal: "La programación en C es fundamental para programadores" (longitud: 55)
Subcadena a buscar: "programa" (longitud: 8)

=== Resultados de Búsqueda ===
✓ Primera ocurrencia encontrada en posición 3
✓ Última ocurrencia encontrada en posición 47
📊 Total de ocurrencias: 2
📍 Posiciones encontradas: 3, 47
✓ Encontrada ignorando mayús/minús en posición 3

=== Análisis Detallado ===
Contexto de la primera ocurrencia:
Posición:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
Texto:     L  a     p  r  o  g  r  a  m  a  c  i  ó  n     e  n     C    
Match:              ^  ^  ^  ^  ^  ^  ^  ^                                

Cadena coincidente: "programa"
```

## Aspectos Pedagógicos

### Conceptos que Refuerza
1. **Bucles anidados y control de flujo**
2. **Comparación secuencial de caracteres**
3. **Manejo de índices y límites de arrays**
4. **Optimización de algoritmos (terminación temprana)**
5. **Gestión de múltiples resultados**

### Patrones de Programación
1. **Búsqueda exhaustiva:** Probar todas las posiciones
2. **Coincidencia parcial:** Manejar coincidencias incompletas
3. **Acumulación de resultados:** Coleccionar múltiples ocurrencias
4. **Validación de entrada:** Casos especiales y límites

### Errores Comunes
1. **Desbordamiento de buffer:** No verificar límites al comparar
2. **Bucle infinito:** Incremento incorrecto de índices
3. **Casos especiales:** No manejar subcadenas vacías
4. **Solapamiento:** No considerar ocurrencias superpuestas

### Variaciones Educativas
1. **Búsqueda con límites:** Solo los primeros N caracteres
2. **Búsqueda bidireccional:** Desde inicio y final simultáneamente
3. **Búsqueda aproximada:** Permitir algunas diferencias
4. **Búsqueda con wildcards:** Caracteres comodín como '*' o '?'

## Aplicaciones Prácticas

### En Sistemas Reales
- **Editores de texto:** Función "Buscar" y "Buscar y reemplazar"
- **Motores de búsqueda:** Indexación y búsqueda de contenido
- **Análisis de logs:** Búsqueda de patrones en archivos de registro
- **Bioinformática:** Búsqueda de secuencias en ADN/proteínas

### Optimizaciones Industriales
- **Algoritmo KMP:** Evita recomparaciones innecesarias
- **Boyer-Moore:** Búsqueda desde el final del patrón
- **Rabin-Karp:** Usa hashing para comparaciones rápidas
- **Aho-Corasick:** Búsqueda simultánea de múltiples patrones

## Algoritmos Avanzados

### Comparación de Rendimiento
| Algoritmo | Tiempo Promedio | Peor Caso | Espacio | Uso Típico |
|-----------|----------------|-----------|----------|------------|
| Fuerza Bruta | O(n*m) | O(n*m) | O(1) | Textos pequeños |
| KMP | O(n+m) | O(n+m) | O(m) | Textos largos |
| Boyer-Moore | O(n/m) | O(n*m) | O(σ) | Búsqueda general |
| Rabin-Karp | O(n+m) | O(n*m) | O(1) | Múltiples patrones |

### Implementaciones Especializadas
```c
// Búsqueda insensible a acentos
int buscar_sin_acentos(const char *texto, const char *patron);

// Búsqueda con expresiones regulares simples
int buscar_regex_simple(const char *texto, const char *patron);

// Búsqueda difusa (tolerante a errores)
int buscar_aproximada(const char *texto, const char *patron, int max_errores);
```

## Relación con Otros Ejercicios
- **Prerequisito de:** Reemplazo de texto, análisis sintáctico
- **Extiende:** Comparación de cadenas, manipulación de strings
- **Combina con:** Conteo de caracteres, validación de patrones

## Recursos Adicionales
- "Introduction to Algorithms" (CLRS) - Capítulo sobre String Matching
- "Programming Pearls" - Algoritmos de búsqueda eficientes
- Documentación de `strstr()` y funciones relacionadas en C
- Análisis de algoritmos de búsqueda en bioinformática
