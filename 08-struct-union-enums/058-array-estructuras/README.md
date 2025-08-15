# Ejercicio 058: Array de Estructuras

## Descripción

Este ejercicio demuestra el uso de **arrays de estructuras** en C, implementando un sistema de gestión de personas con funcionalidades de filtrado, búsqueda, ordenamiento y estadísticas.

## Objetivos de Aprendizaje

- Manejo de arrays de estructuras
- Implementación de algoritmos de filtrado sobre datos estructurados
- Operaciones de búsqueda y ordenamiento en colecciones
- Gestión de memoria para estructuras de datos complejas
- Funciones que trabajan con arrays de tipos definidos por el usuario

## Funcionalidad

### Función Principal (Enunciado)
```c
void imprimir_mayores_de(Persona personas[], int cantidad, int edad_limite);
```

**Propósito**: Filtrar e imprimir todas las personas cuya edad sea mayor que un límite especificado.

### Características Implementadas

#### 🔍 **Filtrado y Búsqueda**
- Filtrar por edad mínima
- Filtrar por rango de edad
- Filtrar por inicial del nombre
- Filtrar por década de edad
- Buscar persona por nombre exacto

#### 📊 **Ordenamiento**
- Ordenar por edad (menor a mayor)
- Ordenar por nombre (alfabéticamente)
- Algoritmo de burbuja optimizado

#### 📈 **Estadísticas**
- Edad promedio, mínima y máxima
- Longitud promedio de nombres
- Distribución por décadas
- Contadores y análisis detallado

#### ✏️ **Gestión de Datos**
- Agregar nuevas personas
- Eliminar personas por índice
- Validación de datos de entrada
- Carga de datos de ejemplo

## Estructura del Proyecto

```
058-array-estructuras/
├── src/
│   ├── array_estructuras.c     # Implementación principal
│   └── main.c                  # Programa demostración
├── include/
│   └── array_estructuras.h     # Definiciones y prototipos
├── tests/
│   └── test_array_estructuras.c # Tests unitarios
├── test_simple.c               # Tests simples (sin Criterion)
├── debug_test.c               # Test de debug para verificación
├── CMakeLists.txt              # Configuración de build
├── Makefile                    # Makefile para compilación
├── README.md                   # Este archivo
└── .gitignore                  # Exclusiones de Git
```

## Estructuras de Datos

### Estructura Persona
```c
typedef struct {
    char nombre[MAX_NOMBRE];    // Nombre completo (hasta 100 caracteres)
    int edad;                   // Edad en años (0-150)
} Persona;
```

### Estructura ListaPersonas
```c
typedef struct {
    Persona personas[MAX_PERSONAS]; // Array de personas (5 elementos)
    int cantidad;                    // Número actual de personas
} ListaPersonas;
```

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make

# Ejecutar programa
./array_estructuras

# Ejecutar tests (requiere Criterion)
make run_tests

# O directamente:
./test_array_estructuras
```

### Compilación Manual
```bash
# Compilar programa principal
gcc -std=c99 -Wall -Wextra -Iinclude -o array_estructuras src/*.c

# Compilar tests (requiere Criterion)
gcc -std=c99 -Wall -Wextra -Iinclude -o test_array_estructuras src/array_estructuras.c tests/test_array_estructuras.c -lcriterion
```

## Uso del Programa

### Modo Demostración
El programa incluye datos de ejemplo que muestran todas las funcionalidades:

```bash
./array_estructuras
```

### Modo Interactivo
Para usar el modo interactivo completo, editar `src/main.c` y descomentar la llamada a `programa_interactivo()`.

### Ejemplos de Uso

#### Filtrado por Edad
```c
Persona personas[] = {
    {"Ana García", 25},
    {"Pedro López", 35},
    {"María Rodríguez", 42}
};

imprimir_mayores_de(personas, 3, 30);
```

**Salida:**
```
Personas mayores de 30 años:
- Pedro López (35 años)
- María Rodríguez (42 años)
Total: 2 persona(s) encontrada(s).
```

#### Ordenamiento por Edad
```c
ListaPersonas lista;
cargar_datos_ejemplo(&lista);
ordenar_por_edad(&lista);
mostrar_lista_completa(&lista);
```

## Compilación y Ejecución

### Usando Makefile (Recomendado)
```bash
# Compilar programa principal
make

# Ejecutar tests completos
make test

# Ejecutar tests simples (sin Criterion)
make test-simple

# Ejecutar con datos de ejemplo
make run-example

# Ver opciones disponibles
make help

# Limpiar archivos compilados
make clean
```

### Compilación Manual
```bash
# Programa principal
gcc -Wall -Wextra -std=c99 -Iinclude -o array_estructuras src/array_estructuras.c src/main.c

# Tests con Criterion (macOS)
gcc -Wall -Wextra -std=c99 -I/opt/homebrew/include -Iinclude -L/opt/homebrew/lib -o test_array_estructuras src/array_estructuras.c tests/test_array_estructuras.c -lcriterion

# Tests simples
gcc -Wall -Wextra -std=c99 -Iinclude -o test_simple src/array_estructuras.c test_simple.c
```

## Tests

El proyecto incluye tests comprehensivos usando **Criterion**:

### Estado de Tests
✅ **Todos los tests pasan** - 19/19 tests exitosos

### Categorías de Tests
- ✅ **Funciones básicas**: Inicialización, agregar/eliminar personas
- ✅ **Filtrado**: Todos los tipos de filtros implementados  
- ✅ **Ordenamiento**: Por edad y por nombre
- ✅ **Búsqueda**: Por nombre exacto y contadores
- ✅ **Funciones auxiliares**: Validación, copia, datos de ejemplo
- ✅ **Casos límite**: Lista llena, vacía, nombres especiales

### Ejecutar Tests
```bash
# Con Makefile
make test

# Directamente
./test_array_estructuras

# Tests específicos
./test_array_estructuras --filter test_basicas
./test_array_estructuras --filter test_filtrado

# Formato TAP para CI/CD
./test_array_estructuras --tap
```

## Conceptos Técnicos

### Arrays de Estructuras
- **Declaración**: `Persona personas[MAX_PERSONAS]`
- **Acceso**: `personas[i].nombre`, `personas[i].edad`
- **Paso a funciones**: Por referencia al primer elemento

### Algoritmos Implementados
- **Búsqueda lineal**: O(n) para encontrar elementos
- **Ordenamiento burbuja**: O(n²) con optimización de parada temprana
- **Filtrado**: O(n) con criterios múltiples

### Gestión de Memoria
- **Estática**: Arrays de tamaño fijo
- **Validación**: Límites de edad y longitud de nombres
- **Seguridad**: Verificación de índices y punteros nulos

## Casos de Uso Educativos

### 1. Gestión de Estudiantes
```c
Persona estudiantes[] = {
    {"Juan Pérez", 20},
    {"Ana López", 19},
    {"Carlos García", 21}
};

// Encontrar estudiantes mayores de edad
imprimir_mayores_de(estudiantes, 3, 18);
```

### 2. Análisis Demográfico
```c
// Distribución por décadas
filtrar_por_decada(poblacion, total, 20);  // 20-29 años
filtrar_por_decada(poblacion, total, 30);  // 30-39 años
```

### 3. Sistema de Empleados
```c
// Buscar empleado específico
int indice = buscar_persona_por_nombre(&empleados, "María García");
if (indice >= 0) {
    printf("Empleado encontrado en posición %d\n", indice);
}
```

## Extensiones Posibles

- **Persistencia**: Guardar/cargar datos desde archivos
- **Más campos**: Añadir dirección, teléfono, ID único
- **Filtros complejos**: Combinación de criterios múltiples
- **Ordenamiento avanzado**: QuickSort, MergeSort
- **Interfaz gráfica**: GUI con GTK o similar

## Buenas Prácticas Demostradas

✅ **Encapsulación**: Estructura `ListaPersonas` que agrupa datos y metadatos  
✅ **Validación**: Verificación exhaustiva de entrada de datos  
✅ **Modularidad**: Funciones especializadas con responsabilidades claras  
✅ **Reutilización**: Funciones genéricas que operan sobre arrays  
✅ **Documentación**: Comentarios detallados y documentación API  
✅ **Testing**: Suite completa de tests unitarios  
✅ **Manejo de errores**: Códigos de retorno y validación de parámetros  

## Notas Técnicas

- **Tamaño máximo**: 5 personas por lista (configurable con `MAX_PERSONAS`)
- **Nombres**: Hasta 100 caracteres (configurable con `MAX_NOMBRE`)
- **Edades válidas**: 0-150 años
- **Compilador**: Compatible con C99 y posteriores
- **Plataforma**: Multiplataforma (Linux, macOS, Windows)
