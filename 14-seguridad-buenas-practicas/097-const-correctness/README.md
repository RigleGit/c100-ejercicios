# Ejercicio 097: Const-Correctness en C

## Descripción

Este ejercicio demuestra la importancia y aplicación correcta de **const-correctness** en C, una práctica fundamental para escribir código seguro, claro y eficiente. A través de ejemplos comparativos y ejercicios interactivos, aprenderás cuándo y cómo usar `const` para mejorar la calidad de tu código.

## Conceptos Clave

### ¿Qué es const-correctness?

Const-correctness es el uso apropiado de la palabra clave `const` para:
- **Clarificar la intención**: Indicar qué datos no deben modificarse
- **Prevenir errores**: Detectar modificaciones accidentales en tiempo de compilación
- **Mejorar la seguridad**: Proteger datos importantes de cambios no deseados
- **Optimizar el código**: Permitir que el compilador aplique optimizaciones

### Tipos de const en C

1. **Puntero a datos constantes**: `const int* ptr`
   - Los datos apuntados no pueden modificarse
   - El puntero sí puede reasignarse

2. **Puntero constante**: `int* const ptr`
   - El puntero no puede reasignarse
   - Los datos sí pueden modificarse

3. **Puntero constante a datos constantes**: `const int* const ptr`
   - Ni el puntero ni los datos pueden modificarse

## Estructura del Proyecto

```
097-const-correctness/
├── include/
│   └── const_correctness.h      # Declaraciones y documentación
├── src/
│   ├── const_correctness.c      # Implementación completa
│   └── main.c                   # Programa interactivo
├── tests/
│   └── test_const_correctness.c # Suite de pruebas Criterion
├── tools/
│   └── .gitkeep                 # Herramientas adicionales
├── CMakeLists.txt               # Configuración de compilación
├── README.md                    # Esta documentación
└── .gitignore                   # Archivos a ignorar
```

## Funciones Implementadas

### Funciones Incorrectas (sin const)
- `longitud_cadena_incorrecto()` - Calcula longitud sin const
- `imprimir_mensaje_incorrecto()` - Imprime sin const
- `buscar_caracter_incorrecto()` - Busca carácter sin const

### Funciones Correctas (con const)
- `longitud_cadena_correcto()` - Versión const-correct
- `imprimir_mensaje_correcto()` - Versión const-correct
- `buscar_caracter_correcto()` - Versión const-correct
- `comparar_cadenas_correcto()` - Compara cadenas
- `copiar_cadena_correcto()` - Copia segura de cadenas

### Funciones con Diferentes Tipos de const
- `sumar_array_const_data()` - Puntero a datos constantes
- `llenar_array_const_ptr()` - Puntero constante
- `encontrar_maximo_const_completo()` - Puntero constante a datos constantes

### Funciones con Estructuras
- `inicializar_configuracion()` - Modifica estructura
- `imprimir_configuracion()` - Solo lectura (const)
- `validar_configuracion()` - Solo lectura (const)
- `cambiar_estado_configuracion()` - Modifica campos específicos

## Compilación

### Requisitos
- CMake 3.10 o superior
- Compilador C (GCC, Clang)
- Criterion (para pruebas)

### Compilar el Proyecto

```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar el Programa Principal

```bash
./const_correctness
```

### Ejecutar las Pruebas

```bash
./test_const_correctness
```

## Uso del Programa

El programa ofrece un menú interactivo con las siguientes opciones:

1. **Demostrar problemas sin const-correctness**
   - Muestra los problemas del código sin const
   - Explica las limitaciones y riesgos

2. **Mostrar beneficios del código const-correct**
   - Demuestra las ventajas de usar const
   - Ejemplos de mejores prácticas

3. **Comparación lado a lado**
   - Compara versiones incorrectas vs correctas
   - Muestra diferencias prácticas

4. **Explorar diferentes tipos de const**
   - Demuestra `const int*`, `int* const`, `const int* const`
   - Ejemplos prácticos de cada tipo

5. **Análisis de const-correctness**
   - Analiza código y sugiere mejoras
   - Estadísticas de uso de const

6. **Ejercicios interactivos**
   - Preguntas sobre const-correctness
   - Práctica identificando dónde aplicar const

## Ejemplos de Uso

### Ejemplo 1: Función de Solo Lectura

```c
// ❌ INCORRECTO: No queda claro que no se modifica
int longitud_cadena_incorrecto(char* cadena) {
    // Implementation...
}

// ✅ CORRECTO: Claro que es solo lectura
size_t longitud_cadena_correcto(const char* cadena) {
    // Implementation...
}
```

### Ejemplo 2: Punteros y const

```c
int valor = 42;

// Puntero a datos constantes
const int* ptr1 = &valor;    // *ptr1 = 5; ← ERROR
                             // ptr1++;   ← OK

// Puntero constante
int* const ptr2 = &valor;    // *ptr2 = 5; ← OK
                             // ptr2++;   ← ERROR

// Puntero constante a datos constantes
const int* const ptr3 = &valor; // *ptr3 = 5; ← ERROR
                                // ptr3++;   ← ERROR
```

### Ejemplo 3: Estructuras con const

```c
typedef struct {
    const int id;           // Campo inmutable
    char nombre[50];        // Campo modificable
    const double factor;    // Campo inmutable
    bool activo;           // Campo modificable
} configuracion_t;

// Solo lectura - usar const
void imprimir_configuracion(const configuracion_t* config);
bool validar_configuracion(const configuracion_t* config);

// Modificación - no usar const
bool cambiar_estado_configuracion(configuracion_t* config, bool nuevo_estado);
```

## Buenas Prácticas

### 1. Usar const para Parámetros de Solo Lectura

```c
// ✅ BUENO: Claro que no se modifica
void procesar_datos(const char* datos, const int* valores, size_t tamaño);

// ❌ MALO: No queda clara la intención
void procesar_datos(char* datos, int* valores, size_t tamaño);
```

### 2. const con Valores de Retorno

```c
// ✅ BUENO: Retorna puntero a datos constantes
const char* obtener_nombre_constante(void);

// Para strings literales que no deben modificarse
const char* obtener_mensaje_error(int codigo) {
    return "Error crítico";  // String literal const
}
```

### 3. Variables Locales const

```c
void calcular_area(double radio) {
    const double PI = 3.14159;  // ✅ Valor que no cambia
    const double area = PI * radio * radio;  // ✅ Resultado inmutable
    
    printf("Área: %.2f\n", area);
}
```

### 4. Arrays const

```c
// ✅ Array de valores constantes
const int DIAS_MES[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// ✅ Función que no modifica el array
int sumar_elementos(const int* array, size_t tamaño);
```

## Beneficios de const-correctness

### 1. **Seguridad de Tipos**
- Previene modificaciones accidentales
- Detecta errores en tiempo de compilación
- Reduce bugs relacionados con mutación no deseada

### 2. **Claridad del Código**
- Documenta la intención del programador
- Hace explícito qué datos se modifican
- Mejora la legibilidad y mantenimiento

### 3. **Optimizaciones del Compilador**
- Permite optimizaciones más agresivas
- Datos const pueden ir en memoria de solo lectura
- Mejor análisis de aliasing

### 4. **Flexibilidad**
- Funciones const aceptan más tipos de datos
- Compatible con string literals
- Facilita el uso de datos inmutables

## Errores Comunes

### 1. **No usar const para parámetros de solo lectura**

```c
// ❌ PROBLEMA
void imprimir_texto(char* texto) {  // Debería ser const char*
    printf("%s\n", texto);
}

// ✅ SOLUCIÓN
void imprimir_texto(const char* texto) {
    printf("%s\n", texto);
}
```

### 2. **Confundir tipos de const**

```c
const char* str1 = "hello";     // Datos constantes
char* const str2 = buffer;      // Puntero constante
const char* const str3 = "hi";  // Ambos constantes
```

### 3. **Modificar datos const (cast incorrecto)**

```c
const int valor = 42;
int* ptr = (int*)&valor;  // ❌ Cast peligroso
*ptr = 100;               // ❌ Comportamiento indefinido
```

## Ejercicios Propuestos

1. **Identificar dónde aplicar const**
   - Revisar código existente
   - Agregar const donde sea apropiado
   - Verificar que el código sigue compilando

2. **Convertir funciones a const-correct**
   - Tomar funciones sin const
   - Agregar const apropiadamente
   - Probar con diferentes tipos de datos

3. **Usar diferentes tipos de const**
   - Implementar funciones con `const T*`
   - Implementar funciones con `T* const`
   - Implementar funciones con `const T* const`

## Archivos de Prueba

Las pruebas incluyen:

- **Funciones básicas**: Longitud, búsqueda, comparación
- **Diferentes tipos de const**: Datos const, punteros const
- **Estructuras**: Validación, modificación, impresión
- **Casos límite**: NULL, cadenas vacías, arrays grandes
- **Integración**: Flujos completos de uso
- **Rendimiento**: Pruebas con datos grandes

Para ejecutar todas las pruebas:

```bash
cd build
./test_const_correctness
```

## Recursos Adicionales

- **C11 Standard**: Especificación oficial de const
- **Effective C**: Mejores prácticas de programación en C
- **MISRA C**: Estándares de seguridad que requieren const-correctness
- **Static Analysis Tools**: Herramientas que detectan violaciones de const

## Notas Importantes

1. **const es una garantía del programador al compilador**
2. **const no significa inmutable en runtime** (se puede hacer cast)
3. **const ayuda al compilador a optimizar**
4. **const mejora la autodocumentación del código**
5. **const es especialmente importante en APIs públicas**