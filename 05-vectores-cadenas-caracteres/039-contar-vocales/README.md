# Ejercicio 039: Contar Vocales en Cadena

## Descripción

Este programa implementa funciones para contar vocales (a, e, i, o, u) en cadenas de caracteres, sin distinguir entre mayúsculas y minúsculas. Proporciona tanto un conteo total como un análisis detallado de cada tipo de vocal.

## Objetivos de Aprendizaje

- Manipulación de cadenas de caracteres en C
- Recorrido de arrays de caracteres
- Uso de funciones auxiliares para modularizar código
- Comparación de caracteres y conversión de mayúsculas/minúsculas
- Uso de punteros para retornar múltiples valores
- Validación de entrada y manejo de casos especiales

## Conceptos Clave

### 1. Identificación de Vocales
```c
int es_vocal(char c) {
    // Convertir a minúscula si es mayúscula
    if (c >= 'A' && c <= 'Z') {
        c = c + ('a' - 'A');
    }
    
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}
```

### 2. Conteo Simple
```c
int contar_vocales(const char *cadena) {
    int contador = 0;
    int i = 0;
    
    while (cadena[i] != '\0') {
        if (es_vocal(cadena[i])) {
            contador++;
        }
        i++;
    }
    
    return contador;
}
```

### 3. Conteo Detallado con Punteros
```c
void contar_vocales_detallado(const char *cadena, int *a, int *e, int *i, int *o, int *u) {
    // Inicializar contadores
    *a = *e = *i = *o = *u = 0;
    
    // Procesar cada carácter...
}
```

## Características del Programa

### Funcionalidades Principales

1. **Conteo Total de Vocales**
   - Cuenta todas las vocales en una cadena
   - No distingue entre mayúsculas y minúsculas
   - Ignora caracteres no alfabéticos

2. **Análisis Detallado**
   - Cuenta cada tipo de vocal por separado
   - Muestra estadísticas completas
   - Usa punteros para retornar múltiples valores

3. **Interfaz Interactiva**
   - Menú de opciones
   - Entrada de cadenas personalizadas
   - Análisis de ejemplos predefinidos

### Casos de Uso

- **Cadenas simples**: "hola" → 2 vocales (o, a)
- **Mayúsculas/minúsculas**: "HOLA" → 2 vocales (O, A)
- **Frases completas**: "El gato come" → 5 vocales
- **Con números/símbolos**: "a1e2i3" → 3 vocales (a, e, i)

## Compilación y Ejecución

### Requisitos
- Compilador GCC o Clang
- CMake 3.10 o superior
- Criterion (para tests)

### Compilar el Programa Principal
```bash
mkdir build
cd build
cmake ..
make main
./main
```

### Ejecutar Tests
```bash
# Instalar Criterion (si no está instalado)
# macOS:
brew install criterion

# Ubuntu/Debian:
sudo apt-get install libcriterion-dev

# Compilar y ejecutar tests
make test_main
make run_tests

# Tests con salida detallada
make run_tests_detailed
```

### Targets Disponibles
- `main` - Programa principal
- `test_main` - Compilar tests
- `run_tests` - Ejecutar todos los tests
- `run_tests_detailed` - Tests con salida detallada
- `clean` - Limpiar archivos compilados
- `help_targets` - Mostrar ayuda

## Estructura del Proyecto

```
039-contar-vocales/
├── src/
│   └── main.c           # Implementación principal
├── test/
│   └── test_main.c      # Tests unitarios
├── include/
│   └── .gitkeep         # Directorio para headers
├── CMakeLists.txt       # Configuración de build
├── README.md           # Este archivo
└── .gitignore          # Archivos a ignorar
```

## Algoritmos Implementados

### 1. Conversión de Mayúsculas a Minúsculas
```c
if (c >= 'A' && c <= 'Z') {
    c = c + ('a' - 'A');
}
```

### 2. Recorrido de Cadenas
```c
int i = 0;
while (cadena[i] != '\0') {
    // Procesar cadena[i]
    i++;
}
```

### 3. Uso de Switch para Clasificación
```c
switch (c) {
    case 'a': (*a)++; break;
    case 'e': (*e)++; break;
    case 'i': (*i)++; break;
    case 'o': (*o)++; break;
    case 'u': (*u)++; break;
}
```

## Casos de Prueba

### Tests Unitarios Incluidos

1. **Función `es_vocal`**:
   - Vocales minúsculas (a, e, i, o, u)
   - Vocales mayúsculas (A, E, I, O, U)
   - Consonantes (b, z, M, X)
   - Caracteres especiales (números, símbolos)

2. **Función `contar_vocales`**:
   - Cadenas vacías y NULL
   - Solo vocales/consonantes
   - Frases completas
   - Casos extremos

3. **Función `contar_vocales_detallado`**:
   - Conteos individuales
   - Manejo de punteros NULL
   - Consistencia con conteo simple

### Ejemplos de Entrada y Salida

```
Entrada: "Hola Mundo"
Salida: 4 vocales (o, a, u, o)

Entrada: "PROGRAMACIÓN"  
Salida: 5 vocales (o, a, a, i, o)

Entrada: "aeiou"
Salida: 5 vocales (a, e, i, o, u)

Entrada: "12345"
Salida: 0 vocales
```

## Complejidades

- **Temporal**: O(n) donde n es la longitud de la cadena
- **Espacial**: O(1) - espacio constante

## Extensiones Posibles

1. **Soporte para vocales acentuadas** (á, é, í, ó, ú)
2. **Análisis de frecuencia completo** (todas las letras)
3. **Procesamiento de archivos de texto**
4. **Estadísticas avanzadas** (porcentajes, distribuciones)
5. **Soporte para múltiples idiomas**

## Notas Técnicas

- Usa aritmética de caracteres para conversión de mayúsculas
- Maneja punteros NULL de forma segura
- Implementa validación de entrada robusta
- Separa lógica de presentación
- Código modular y reutilizable

## Autor

Ejercicio 039 del proyecto "100 Ejercicios de Programación en C"

---

Este ejercicio demuestra técnicas fundamentales de procesamiento de cadenas y análisis de texto en C, proporcionando una base sólida para aplicaciones más complejas de manipulación de texto.
