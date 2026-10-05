# Ejercicio 061: Estructuras Dinámicas con malloc

## 📋 Descripción

Este ejercicio demuestra el uso avanzado de estructuras dinámicas en C, específicamente cómo gestionar memoria dinámica para estructuras complejas y arrays de estructuras. Implementa un sistema completo de gestión de personas con operaciones CRUD, filtrado, ordenamiento y estadísticas.

## 🎯 Objetivos de Aprendizaje

- **Gestión de memoria dinámica**: Uso correcto de `malloc()`, `realloc()` y `free()`
- **Estructuras complejas**: Definición y manejo de estructuras con miembros dinámicos
- **Arrays dinámicos**: Implementación de arrays redimensionables
- **Operaciones CRUD**: Create, Read, Update, Delete en estructuras de datos
- **Algoritmos de ordenamiento**: Implementación de ordenamiento en estructuras
- **Filtrado de datos**: Búsqueda y filtrado eficiente
- **Gestión de errores**: Manejo robusto de fallos de memoria
- **Optimización de memoria**: Estrategias de redimensionamiento automático

## 🏗️ Estructuras de Datos

### Estructura Persona
```c
typedef struct {
    char *nombre;        // Nombre dinámico
    char *email;         // Email dinámico
    int edad;            // Edad de la persona
    float salario;       // Salario de la persona
} Persona;
```

### Estructura ArrayPersonas
```c
typedef struct {
    Persona *personas;   // Array dinámico de personas
    size_t size;         // Número actual de personas
    size_t capacity;     // Capacidad máxima actual
} ArrayPersonas;
```

### Estructura EstadisticasPersonas
```c
typedef struct {
    float edad_promedio;
    float salario_promedio;
    int edad_minima;
    int edad_maxima;
    float salario_minimo;
    float salario_maximo;
    size_t total_personas;
} EstadisticasPersonas;
```

## ⚡ Funcionalidades

### 1. Gestión de Persona Individual
- ✅ **Crear persona**: Asignación dinámica de memoria para nombre y email
- ✅ **Liberar persona**: Liberación correcta de toda la memoria asociada
- ✅ **Validar persona**: Verificación de integridad de datos
- ✅ **Copiar persona**: Creación de copias independientes
- ✅ **Actualizar datos**: Modificación dinámica de nombre y email

### 2. Gestión de Array Dinámico
- ✅ **Crear array**: Inicialización con capacidad inicial
- ✅ **Agregar persona**: Inserción con redimensionamiento automático
- ✅ **Eliminar persona**: Por índice o por nombre
- ✅ **Buscar persona**: Búsqueda lineal por nombre
- ✅ **Redimensionar**: Crecimiento y reducción automática

### 3. Operaciones de Filtrado
- ✅ **Filtrar por edad**: Crear subconjuntos por rango de edad
- ✅ **Filtrar por salario**: Crear subconjuntos por rango salarial
- ✅ **Preservar datos originales**: Los filtros crean nuevos arrays

### 4. Algoritmos de Ordenamiento
- ✅ **Ordenar por nombre**: Ordenamiento alfabético
- ✅ **Ordenar por edad**: Ordenamiento numérico ascendente
- ✅ **Ordenar por salario**: Ordenamiento por valor monetario
- ✅ **Algoritmo burbuja**: Implementación pedagógica simple

### 5. Análisis Estadístico
- ✅ **Cálculo de promedios**: Edad y salario promedio
- ✅ **Valores extremos**: Mínimos y máximos
- ✅ **Conteo total**: Número de elementos
- ✅ **Generación de reportes**: Presentación formatada

### 6. Utilidades de Memoria
- ✅ **Medición de uso**: Cálculo exacto de bytes utilizados
- ✅ **Información detallada**: Desglose por componentes
- ✅ **Factor de utilización**: Eficiencia del array
- ✅ **Monitoreo en tiempo real**: Seguimiento dinámico

### 7. Importación/Exportación
- ✅ **Exportar a CSV**: Guardado de datos en formato estándar
- ✅ **Importar desde CSV**: Carga de datos externos
- ✅ **Manejo de errores**: Validación de formato y datos

## 🧮 Conceptos Técnicos

### Gestión de Memoria Dinámica

1. **Asignación Inicial**:
   ```c
   Persona *persona = malloc(sizeof(Persona));
   persona->nombre = malloc(strlen(nombre) + 1);
   ```

2. **Redimensionamiento**:
   ```c
   Persona *nuevo_ptr = realloc(array->personas, nueva_capacidad * sizeof(Persona));
   ```

3. **Liberación Correcta**:
   ```c
   free(persona->nombre);
   free(persona->email);
   free(persona);
   ```

### Estrategias de Redimensionamiento

- **Capacidad inicial**: 4 elementos
- **Factor de crecimiento**: 2x cuando se llena
- **Factor de reducción**: 0.5x cuando utilización < 25%
- **Capacidad mínima**: Nunca menor que la inicial

### Complejidad Algoritmica

| Operación | Complejidad | Notas |
|-----------|-------------|-------|
| Agregar | O(1) amortizado | O(n) en redimensionamiento |
| Buscar | O(n) | Búsqueda lineal |
| Eliminar | O(n) | Reordenamiento necesario |
| Ordenar | O(n²) | Algoritmo burbuja |
| Filtrar | O(n) | Una pasada por elemento |

## 📊 Ejemplos de Uso

### Ejemplo Básico
```c
#include "estructura_dinamica.h"

int main() {
    // Crear array de personas
    ArrayPersonas *empresa = crear_array_personas();
    
    // Agregar empleados
    agregar_persona(empresa, "Ana García", "ana@empresa.com", 28, 45000.0);
    agregar_persona(empresa, "Carlos López", "carlos@empresa.com", 35, 55000.0);
    
    // Mostrar información
    imprimir_array_personas(empresa);
    
    // Calcular estadísticas
    EstadisticasPersonas stats = calcular_estadisticas(empresa);
    imprimir_estadisticas(&stats);
    
    // Filtrar por edad
    ArrayPersonas *jovenes = filtrar_por_edad(empresa, 25, 30);
    imprimir_array_personas(jovenes);
    
    // Limpiar memoria
    liberar_array_personas(jovenes);
    liberar_array_personas(empresa);
    
    return 0;
}
```

### Ejemplo Avanzado con Todas las Funcionalidades
```c
void demo_completa() {
    ArrayPersonas *empresa = crear_array_personas();
    
    // Cargar datos masivos
    for (int i = 0; i < 1000; i++) {
        char nombre[50], email[100];
        snprintf(nombre, sizeof(nombre), "Empleado%d", i);
        snprintf(email, sizeof(email), "empleado%d@empresa.com", i);
        agregar_persona(empresa, nombre, email, 20 + (i % 45), 30000 + (i * 100));
    }
    
    // Análisis de memoria
    mostrar_informacion_memoria(empresa);
    
    // Filtrado múltiple
    ArrayPersonas *senior = filtrar_por_edad(empresa, 40, 65);
    ArrayPersonas *bien_pagados = filtrar_por_salario(senior, 60000, 100000);
    
    // Ordenamiento y análisis
    ordenar_por_salario(bien_pagados);
    EstadisticasPersonas stats = calcular_estadisticas(bien_pagados);
    imprimir_estadisticas(&stats);
    
    // Exportar resultados
    exportar_a_csv(bien_pagados, "empleados_senior_alto_salario.csv");
    
    // Limpieza
    liberar_array_personas(bien_pagados);
    liberar_array_personas(senior);
    liberar_array_personas(empresa);
}
```

## 🔍 Detalles de Implementación

### Gestión de Memoria Segura

1. **Verificación de malloc**: Todos los `malloc()` verifican retorno NULL
2. **Liberación ordenada**: Strings antes que estructuras
3. **Prevención de double-free**: Punteros a NULL después de `free()`
4. **Manejo de realloc**: Verificación de fallo sin perder datos originales

### Optimizaciones de Rendimiento

1. **Redimensionamiento inteligente**: Crecimiento exponencial para O(1) amortizado
2. **Reducción automática**: Libera memoria no utilizada
3. **Localidad de referencia**: Arrays contiguos para mejor cache
4. **Validaciones tempranas**: Evita operaciones costosas en datos inválidos

### Consideraciones de Diseño

1. **Separación de responsabilidades**: Funciones especializadas y cohesivas
2. **Manejo de errores robusto**: Todos los fallos manejados apropiadamente
3. **Interfaz consistente**: Patrones uniformes en toda la API
4. **Documentación exhaustiva**: Cada función completamente documentada

## 🎓 Valor Pedagógico

Este ejercicio enseña conceptos fundamentales de:

1. **Gestión manual de memoria**: Base para entender sistemas de bajo nivel
2. **Estructuras de datos dinámicas**: Fundamento de algoritmos avanzados
3. **Diseño de APIs**: Principios de interfaces limpias y usables
4. **Testing sistemático**: Metodología de validación exhaustiva
5. **Optimización práctica**: Balance entre simplicidad y eficiencia

### Extensiones Sugeridas

1. **Hash tables**: Implementar búsqueda O(1)
2. **Árboles balanceados**: Ordenamiento eficiente
3. **Serialización binaria**: Persistencia más eficiente
4. **Threading**: Operaciones concurrentes seguras
5. **Pool de memoria**: Gestión optimizada de allocaciones

## 📈 Casos de Uso Reales

- **Sistemas de gestión de empleados**: HR y nóminas
- **Bases de datos en memoria**: Sistemas embebidos
- **Análisis de datos**: Procesamiento estadístico
- **Sistemas de cache**: Gestión temporal de datos
- **Simulaciones**: Modelado de poblaciones

## ⚠️ Limitaciones Actuales

1. **Búsqueda lineal**: O(n) para búsquedas
2. **Ordenamiento simple**: O(n²) bubble sort
3. **Sin persistencia**: Solo datos en memoria
4. **Threading**: No thread-safe
5. **Validación básica**: Email/nombre sin formato estricto

Estas limitaciones son intencionadas para mantener el foco pedagógico en gestión de memoria dinámica.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/estructura_dinamica
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
