# Ejercicio 054: Concatenar Cadenas Dinámico

## 📋 Descripción

Este ejercicio enseña la **concatenación de cadenas usando memoria dinámica**, combinando el uso de `malloc()`, funciones estándar de C (`strlen`, `strcpy`, `strcat`) y gestión segura de memoria para crear nuevas cadenas que contengan la unión de dos o más cadenas de entrada.

## 🎯 Objetivos de Aprendizaje

- **Gestión de memoria dinámica**: Cálculo preciso y reserva de memoria para cadenas
- **Funciones estándar de cadenas**: Uso correcto de `strlen()`, `strcpy()`, `strcat()`
- **Cálculo de tamaños**: Determinar memoria necesaria incluyendo terminador nulo
- **Concatenación segura**: Evitar desbordamientos de buffer
- **Entrada de usuario**: Lectura segura de cadenas desde teclado
- **Buenas prácticas**: Validación, manejo de errores y liberación de memoria

## 🚀 Funcionalidades

### Implementación Básica (Enunciado)
```c
// Leer dos cadenas y concatenarlas dinámicamente
char cadena1[100], cadena2[100];
size_t longitud_total = strlen(cadena1) + strlen(cadena2) + 1;
char *resultado = malloc(longitud_total * sizeof(char));
strcpy(resultado, cadena1);
strcat(resultado, cadena2);
```

### Funciones Avanzadas
- **Concatenación múltiple**: Unir arrays de cadenas
- **Concatenación con separador**: Insertar delimitadores
- **Concatenación con formato**: Similar a `sprintf` pero dinámico
- **Cadenas dinámicas**: Estructura que crece automáticamente
- **Manipulación de cadenas**: Invertir, cambiar caso, eliminar espacios

## 🔧 Estructura de Datos

```c
typedef struct {
    char *contenido;    // Puntero al contenido de la cadena
    size_t longitud;    // Longitud actual (sin incluir '\0')
    size_t capacidad;   // Capacidad total asignada
} CadenaDinamica;
```

### Conceptos Clave

- **Cálculo de memoria**: `strlen(cadena1) + strlen(cadena2) + 1`
- **Terminador nulo**: Siempre reservar espacio para `'\0'`
- **Funciones estándar**: Aprovechar bibliotecas optimizadas
- **Validación de entrada**: Verificar parámetros antes de usar

## 📊 Ejemplo de Uso

### Implementación Básica
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char cadena1[100], cadena2[100];
    
    printf("Introduce la primera cadena: ");
    fgets(cadena1, sizeof(cadena1), stdin);
    cadena1[strcspn(cadena1, "\n")] = '\0';  // Eliminar \n
    
    printf("Introduce la segunda cadena: ");
    fgets(cadena2, sizeof(cadena2), stdin);
    cadena2[strcspn(cadena2, "\n")] = '\0';  // Eliminar \n
    
    // Calcular memoria necesaria
    size_t longitud_total = strlen(cadena1) + strlen(cadena2) + 1;
    
    // Reservar memoria
    char *resultado = malloc(longitud_total * sizeof(char));
    if (resultado == NULL) {
        printf("Error al reservar memoria.\n");
        return 1;
    }
    
    // Concatenar
    strcpy(resultado, cadena1);
    strcat(resultado, cadena2);
    
    printf("Cadena concatenada: %s\n", resultado);
    
    // Liberar memoria
    free(resultado);
    return 0;
}
```

### Implementación Avanzada
```c
#include "concatenar_cadenas.h"

int main() {
    // Concatenación simple
    char *resultado = concatenarCadenas("Hola", " mundo");
    printf("Resultado: %s\n", resultado);
    liberarCadena(resultado);
    
    // Concatenación múltiple
    const char *cadenas[] = {"Hola", " ", "mundo", " ", "desde", " ", "C"};
    char *multiple = concatenarMultiples(cadenas, 7);
    printf("Múltiple: %s\n", multiple);
    liberarCadena(multiple);
    
    // Concatenación con separador
    char *email = concatenarConSeparador("usuario", "dominio.com", "@");
    printf("Email: %s\n", email);
    liberarCadena(email);
    
    // Cadena dinámica que crece automáticamente
    CadenaDinamica *cadena = crearCadenaDinamica(10);
    asignarContenido(cadena, "Inicio");
    concatenarACadena(cadena, " y más contenido");
    printf("Dinámica: %s\n", cadena->contenido);
    liberarCadenaDinamica(cadena);
    
    return 0;
}
```

## 🔍 Funciones Principales

### Concatenación Básica
```c
char* concatenarCadenas(const char *cadena1, const char *cadena2);
char* concatenarMultiples(const char **cadenas, size_t cantidad);
char* concatenarConSeparador(const char *c1, const char *c2, const char *sep);
char* concatenarConFormato(const char *formato, ...);
```

### Gestión de Cadenas Dinámicas
```c
CadenaDinamica* crearCadenaDinamica(size_t capacidad_inicial);
bool asignarContenido(CadenaDinamica *cadena, const char *contenido);
bool concatenarACadena(CadenaDinamica *cadena, const char *sufijo);
void liberarCadenaDinamica(CadenaDinamica *cadena);
```

### Manipulación de Cadenas
```c
char* duplicarCadena(const char *cadena);
char* subcadena(const char *cadena, size_t inicio, size_t longitud);
char* invertirCadena(const char *cadena);
char* convertirAMayusculas(const char *cadena);
char* convertirAMinusculas(const char *cadena);
char* eliminarEspacios(const char *cadena);
```

### Búsqueda y Análisis
```c
int buscarSubcadena(const char *cadena, const char *subcadena);
size_t contarOcurrencias(const char *cadena, char caracter);
bool empiezaCon(const char *cadena, const char *prefijo);
bool terminaCon(const char *cadena, const char *sufijo);
```

## 📈 Análisis de Complejidad

| Operación | Tiempo | Espacio | Notas |
|-----------|--------|---------|-------|
| `concatenarCadenas` | O(n + m) | O(n + m) | n, m = longitudes |
| `strcpy` | O(n) | O(1) | n = longitud fuente |
| `strcat` | O(n + m) | O(1) | Busca final + copia |
| `strlen` | O(n) | O(1) | Recorre hasta '\0' |
| `malloc` | O(1) | O(n) | Asignación de n bytes |

## 🚨 Consideraciones Importantes

### Gestión de Memoria
```c
// ✅ CORRECTO
size_t longitud = strlen(cadena1) + strlen(cadena2) + 1;  // +1 para '\0'
char *resultado = malloc(longitud);
if (!resultado) {
    // Manejar error
    return NULL;
}
strcpy(resultado, cadena1);
strcat(resultado, cadena2);
// ... usar resultado ...
free(resultado);
```

### Errores Comunes
```c
// ❌ INCORRECTO - olvidar espacio para '\0'
char *resultado = malloc(strlen(cadena1) + strlen(cadena2));

// ❌ INCORRECTO - no verificar malloc
char *resultado = malloc(longitud);
strcpy(resultado, cadena1);  // Puede causar crash

// ❌ INCORRECTO - usar strcat sin strcpy inicial
strcat(resultado, cadena1);  // Undefined behavior

// ❌ INCORRECTO - no liberar memoria
char *resultado = concatenarCadenas("a", "b");
// ... usar resultado pero no hacer free()
```

### Buenas Prácticas
- ✅ Siempre verificar retorno de `malloc()`
- ✅ Incluir espacio para terminador nulo
- ✅ Usar `strcpy()` antes de `strcat()`
- ✅ Liberar memoria con `free()`
- ✅ Validar parámetros de entrada
- ✅ Usar `const` para parámetros no modificados

## 🔄 Funciones Estándar Utilizadas

### strlen()
```c
size_t strlen(const char *s);
// Retorna: longitud de la cadena (sin incluir '\0')
// Complejidad: O(n)
```

### strcpy()
```c
char *strcpy(char *dest, const char *src);
// Copia src a dest (incluyendo '\0')
// Retorna: puntero a dest
// Requisito: dest debe tener espacio suficiente
```

### strcat()
```c
char *strcat(char *dest, const char *src);
// Concatena src al final de dest
// Retorna: puntero a dest
// Requisito: dest debe contener cadena válida y tener espacio
```

### strcspn()
```c
size_t strcspn(const char *s1, const char *s2);
// Longitud del prefijo de s1 que no contiene caracteres de s2
// Útil para eliminar '\n': s[strcspn(s, "\n")] = '\0';
```

## 🎯 Casos de Uso Reales

1. **Construcción de URLs**: Combinar protocolo, dominio y rutas
2. **Generación de mensajes**: Unir plantillas con datos dinámicos
3. **Procesamiento de archivos**: Construir nombres de archivos
4. **Interfaz de usuario**: Formatear textos para mostrar
5. **Logs y debugging**: Crear mensajes informativos
6. **Parsers**: Reconstruir tokens en cadenas completas

## 💡 Optimizaciones Avanzadas

### Pre-cálculo de Longitudes
```c
// Evitar múltiples llamadas a strlen()
size_t len1 = strlen(cadena1);
size_t len2 = strlen(cadena2);
size_t total = len1 + len2 + 1;

char *resultado = malloc(total);
memcpy(resultado, cadena1, len1);
memcpy(resultado + len1, cadena2, len2 + 1);  // +1 para '\0'
```

### Concatenación In-Place
```c
// Para cadenas dinámicas que pueden crecer
bool concatenarInPlace(CadenaDinamica *cadena, const char *sufijo) {
    size_t len_sufijo = strlen(sufijo);
    size_t nueva_longitud = cadena->longitud + len_sufijo;
    
    if (nueva_longitud + 1 > cadena->capacidad) {
        // Redimensionar
        size_t nueva_capacidad = (nueva_longitud + 1) * 2;
        char *nuevo = realloc(cadena->contenido, nueva_capacidad);
        if (!nuevo) return false;
        
        cadena->contenido = nuevo;
        cadena->capacidad = nueva_capacidad;
    }
    
    memcpy(cadena->contenido + cadena->longitud, sufijo, len_sufijo + 1);
    cadena->longitud = nueva_longitud;
    
    return true;
}
```

## 🔄 Extensiones Posibles

- **Concatenación con encoding**: UTF-8, Unicode
- **Concatenación thread-safe**: Para aplicaciones concurrentes
- **Pool de memoria**: Reutilizar buffers para evitar fragmentación
- **Streaming**: Concatenar cadenas muy grandes sin cargar todo en memoria
- **Compresión**: Concatenar y comprimir en tiempo real
- **Internacionalización**: Soporte para diferentes idiomas y locales

## 📚 Recursos Adicionales

- [String Functions - C Reference](https://en.cppreference.com/w/c/string/byte)
- [Dynamic Memory Allocation](https://www.cprogramming.com/tutorial/c/lesson6.html)
- [Secure String Handling](https://wiki.sei.cmu.edu/confluence/display/c/STR)

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/concatenar_cadenas_dinamico
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
