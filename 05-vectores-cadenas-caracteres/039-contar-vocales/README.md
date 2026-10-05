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

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/contar_vocales
```

## Pruebas de esta revisión

Entradas y salidas observables del programa.

Código: [pruebas CLI](../../tools/tests/test_cli_exercises.py). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
