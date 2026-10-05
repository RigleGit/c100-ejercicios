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

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/array_estructuras
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
