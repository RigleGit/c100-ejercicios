# Ejercicio 080: Macros con Argumentos

## 📋 Descripción

Este ejercicio demuestra el uso de **macros con argumentos** en C mediante la directiva `#define`. Las macros con parámetros son más poderosas que las constantes simples, pero introducen complejidades importantes como la **evaluación múltiple** de argumentos y **efectos secundarios**.

## 🎯 Objetivo

Aprender a:
- Crear macros con argumentos usando `#define`
- Entender la evaluación múltiple de argumentos
- Reconocer y evitar efectos secundarios peligrosos
- Usar paréntesis correctamente en macros
- Comparar macros vs funciones
- Aplicar macros de forma segura y efectiva

## ⚠️ Concepto Clave: Evaluación Múltiple

**La característica más importante de las macros con argumentos es que cada argumento se evalúa tantas veces como aparece en la definición de la macro.**

```c
#define CUADRADO(x) ((x) * (x))

int contador = 0;
int incrementar() { return ++contador; }

CUADRADO(incrementar()); // incrementar() se llama DOS veces!
// Se expande a: ((incrementar()) * (incrementar()))
// Resultado: 1 * 2 = 2, no 1 * 1 = 1
```

## 📚 Conceptos Fundamentales

### 1. Sustitución Textual
```c
#define CUADRADO(x) ((x) * (x))

CUADRADO(3 + 1)
// Se expande a: ((3 + 1) * (3 + 1))
// Resultado: 4 * 4 = 16
```

### 2. Importancia de los Paréntesis
```c
// ❌ INCORRECTO: Sin paréntesis
#define CUADRADO_MALO(x) x * x
CUADRADO_MALO(3 + 1) → 3 + 1 * 3 + 1 = 7 ❌

// ✅ CORRECTO: Con paréntesis
#define CUADRADO(x) ((x) * (x))
CUADRADO(3 + 1) → ((3 + 1) * (3 + 1)) = 16 ✅
```

### 3. Efectos Secundarios Peligrosos
```c
int i = 5;
CUADRADO(++i);  // ++i se evalúa DOS veces
// Resultado: (6 * 7) = 42, no (6 * 6) = 36
// i termina siendo 7, no 6
```

### 4. Diferencias con Funciones

| Aspecto | Macros | Funciones |
|---------|--------|-----------|
| **Evaluación** | Múltiple | Una sola vez |
| **Tipos** | Sin verificación | Verificación estricta |
| **Rendimiento** | Sin overhead | Overhead de llamada |
| **Depuración** | Difícil | Fácil |
| **Sustitución** | Tiempo de compilación | Tiempo de ejecución |

## 🏗️ Estructura del Proyecto

```
080-macros-con-argumentos/
├── include/
│   └── macros_con_argumentos.h    # Definiciones de macros
├── src/
│   └── macros_con_argumentos.c    # Implementación y demos
├── tests/
│   └── test_macros_con_argumentos.c # Tests exhaustivos
├── CMakeLists.txt                 # Configuración de build
└── README.md                      # Este archivo
```

## 🛠️ Compilación

### Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar proyecto
cmake ..

# Compilar
make

# Ejecutar tests
make test
```

### Usando GCC directamente

```bash
# Compilar el ejemplo básico del enunciado
gcc -Wall -Wextra -std=c99 -o main_basico src/macros_con_argumentos.c -Iinclude -DCOMPILAR_MAIN_BASICO -lm

# Compilar tests (requiere Criterion)
gcc -Wall -Wextra -std=c99 -o test tests/test_macros_con_argumentos.c src/macros_con_argumentos.c -Iinclude -lcriterion -lm
```

## ▶️ Ejecución

### Demos Disponibles

1. **Ejemplo básico del enunciado:**
   ```bash
   make demo_enunciado
   # o directamente:
   ./main_basico
   ```

2. **Demostración completa:**
   ```bash
   make demo_completo
   # o directamente:
   ./demo_completo
   ```

3. **Demostrar problemas comunes:**
   ```bash
   make demo_problemas
   ```

### Tests Específicos
```bash
# Ejecutar tests con detalles
make test_verbose

# Tests específicos
make test_basicos      # Macros básicas
make test_efectos      # Efectos secundarios
make test_problematicas # Macros problemáticas
make test_educativos   # Tests educativos
```

### Análisis Avanzado
```bash
# Ver expansión del preprocesador
make mostrar_expansion

# Generar código ensamblador comparativo
make demo_optimizacion

# Análisis detallado de macros
make analizar_macros

# Informe educativo
make informe_educativo
```

## 🧪 Casos de Prueba

### Tests Implementados

1. **Macros Básicas:**
   - Verificación de `CUADRADO`, `CUBO`, `CUARTA_POTENCIA`
   - Pruebas con expresiones aritméticas
   - Valores extremos y casos límite

2. **Macros Problemáticas:**
   - Demostración de macros sin paréntesis
   - Problemas de precedencia de operadores
   - Comparación con versiones correctas

3. **Efectos Secundarios:**
   - Evaluación múltiple de funciones
   - Problemas con incremento/decremento
   - Funciones con efectos laterales

4. **Macros de Utilidad:**
   - Tamaño de arrays (`LONGITUD_ARRAY`)
   - Intercambio de variables (`INTERCAMBIAR`)
   - Macros condicionales (`ES_PAR`, `ES_IMPAR`)

5. **Tests Educativos:**
   - Diferencias entre macros y funciones
   - Demostración de expansión textual
   - Verificación de tipos genéricos

## 📋 Ejemplos Prácticos

### Ejemplo Básico (del enunciado)
```c
#include "macros_con_argumentos.h"

int sumarUno() {
    printf("sumarUno() llamado\n");
    return 1;
}

int main() {
    int a = 5;
    printf("CUADRADO(a) = %d\n", CUADRADO(a));         // 25
    printf("CUADRADO(3 + 1) = %d\n", CUADRADO(3 + 1)); // 16
    printf("CUADRADO(sumarUno()) = %d\n", CUADRADO(sumarUno())); // ¡sumarUno() se llama DOS veces!
    
    return 0;
}
```

**Salida esperada:**
```
CUADRADO(a) = 25
CUADRADO(3 + 1) = 16
sumarUno() llamado
sumarUno() llamado
CUADRADO(sumarUno()) = 2
```

### Problema con Incremento
```c
int i = 5;
printf("i antes: %d\n", i);           // 5
printf("CUADRADO(++i): %d\n", CUADRADO(++i)); // 42 (6*7), no 36 (6*6)
printf("i después: %d\n", i);         // 7, no 6
```

### Uso Seguro
```c
// ✅ Seguro: Sin efectos secundarios
int x = 5;
printf("CUADRADO(x) = %d\n", CUADRADO(x)); // 25

// ✅ Seguro: Expresiones sin efectos secundarios
printf("CUADRADO(2 + 3) = %d\n", CUADRADO(2 + 3)); // 25

// ❌ Peligroso: Con efectos secundarios
printf("CUADRADO(++x) = %d\n", CUADRADO(++x)); // ¡x se incrementa dos veces!
```

### Macros Avanzadas
```c
// Macro para máximo con protección
#define MAX_SEGURO(a, b) ({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    (_a > _b) ? _a : _b; \
})

// Uso
int resultado = MAX_SEGURO(sumarUno(), sumarUno());
// sumarUno() se llama solo dos veces, no cuatro
```

## 🎯 Objetivos de Aprendizaje

Al completar este ejercicio, habrás aprendido:

1. **Mecánica de las Macros:**
   - Cómo funcionan las macros con argumentos
   - Por qué ocurre la evaluación múltiple
   - Cuándo usar macros vs funciones

2. **Problemas Comunes:**
   - Efectos secundarios por evaluación múltiple
   - Problemas de precedencia sin paréntesis
   - Dificultades en la depuración

3. **Buenas Prácticas:**
   - Siempre usar paréntesis alrededor de argumentos
   - Evitar expresiones con efectos secundarios
   - Usar nombres en MAYÚSCULAS para macros
   - Considerar alternativas más seguras

4. **Casos de Uso Apropiados:**
   - Código genérico (funciona con cualquier tipo)
   - Optimizaciones de rendimiento
   - Constantes calculadas en tiempo de compilación

## ⚠️ Precauciones Importantes

### Problemas a Evitar

1. **Evaluación Múltiple:**
   ```c
   // ❌ PELIGROSO
   CUADRADO(++i)
   CUADRADO(funcion_costosa())
   CUADRADO(rand())
   ```

2. **Falta de Paréntesis:**
   ```c
   // ❌ INCORRECTO
   #define SUMA(a,b) a + b
   SUMA(2,3) * 4 → 2 + 3 * 4 = 14 ❌
   
   // ✅ CORRECTO
   #define SUMA(a,b) ((a) + (b))
   SUMA(2,3) * 4 → ((2) + (3)) * 4 = 20 ✅
   ```

3. **Efectos en Depuración:**
   ```c
   // Difícil de depurar porque la macro se expande
   if (CUADRADO(calcular_valor()) > 100) {
       // ¿Qué valor se calculó realmente?
   }
   ```

### Alternativas Más Seguras

1. **Funciones inline (C99+):**
   ```c
   static inline int cuadrado(int x) {
       return x * x;
   }
   ```

2. **Expresiones Compuestas (GCC):**
   ```c
   #define CUADRADO_SEGURO(x) ({ \
       typeof(x) _x = (x); \
       (_x) * (_x); \
   })
   ```

3. **Funciones Template (C++):**
   ```cpp
   template<typename T>
   constexpr T cuadrado(T x) {
       return x * x;
   }
   ```

## 🔍 Análisis del Preprocesador

Para entender mejor cómo funcionan las macros:

```bash
# Ver la expansión del preprocesador
gcc -E archivo.c -I include

# Ver solo las expansiones de CUADRADO
gcc -E archivo.c -I include | grep -A 2 -B 2 "CUADRADO"
```

**Ejemplo de expansión:**
```c
// Código original:
CUADRADO(3 + 1)

// Después del preprocesador:
((3 + 1) * (3 + 1))
```

## 📊 Comparación de Rendimiento

Las macros pueden ser más rápidas que funciones porque:
- No hay overhead de llamada a función
- No hay push/pop del stack
- Pueden optimizarse mejor por el compilador

Pero:
- Pueden aumentar el tamaño del código (expansión múltiple)
- Dificultan la depuración
- No verifican tipos

## 🤝 Conclusiones

Las macros con argumentos son una herramienta poderosa pero peligrosa:

**✅ Usar cuando:**
- Necesitas código genérico (sin tipos específicos)
- El rendimiento es crítico
- Trabajas con constantes en tiempo de compilación

**❌ Evitar cuando:**
- Los argumentos tienen efectos secundarios
- El código puede ser complejo de depurar
- Una función simple sería suficiente

**🛡️ Siempre recordar:**
- Usar paréntesis alrededor de todos los argumentos
- Usar paréntesis alrededor de toda la expresión
- Evitar argumentos con efectos secundarios
- Testear exhaustivamente

---

## 📚 Referencias

- Kernighan & Ritchie: "The C Programming Language"
- ISO C Standard: Preprocessing directives
- GCC Manual: Macro definitions with parameters
- "Expert C Programming" by Peter van der Linden

---

## 🤝 Contribución

Si encuentras errores o tienes sugerencias de mejora, por favor crea un issue o envía un pull request.
