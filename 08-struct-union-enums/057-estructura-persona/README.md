# Ejercicio 057: Estructura Persona

## Descripción

Este ejercicio introduce los conceptos fundamentales de las estructuras (`struct`) en C mediante la implementación de una estructura `Persona`. Aprenderás a agrupar datos relacionados, usar `typedef` para simplificar declaraciones, y realizar operaciones comunes sobre estructuras.

## Conceptos Clave

### Estructuras en C
- **Agrupación de datos**: Combina variables de diferentes tipos en una entidad
- **Modelado del mundo real**: Representa objetos con múltiples atributos
- **Acceso con operador punto**: `estructura.campo` para acceder a miembros
- **Paso por referencia**: Usar punteros para eficiencia con estructuras grandes

### typedef
- **Simplificación de sintaxis**: `typedef struct {...} Nombre;`
- **Mejor legibilidad**: `Persona p;` vs `struct Persona p;`
- **Abstracción de tipos**: Oculta detalles de implementación
- **Facilidad de mantenimiento**: Cambios centralizados en la definición

## Algoritmos Implementados

### Definición de la Estructura
```c
typedef struct {
    char nombre[MAX_NOMBRE];    // Cadena de caracteres
    int edad;                   // Número entero
} Persona;
```

### Operaciones Básicas
```c
// Crear persona
Persona crear_persona(const char *nombre, int edad) {
    Persona p;
    strncpy(p.nombre, nombre, MAX_NOMBRE - 1);
    p.nombre[MAX_NOMBRE - 1] = '\0';
    p.edad = edad;
    return p;
}

// Validar persona
bool validar_persona(const Persona *p) {
    return p != NULL && strlen(p->nombre) > 0 && 
           p->edad >= 0 && p->edad <= 150;
}
```

### Operaciones con Arrays
```c
// Ordenamiento por edad (burbuja optimizado)
void ordenar_por_edad(Persona personas[], int cantidad) {
    for (int i = 0; i < cantidad - 1; i++) {
        bool intercambio = false;
        for (int j = 0; j < cantidad - 1 - i; j++) {
            if (personas[j].edad > personas[j + 1].edad) {
                Persona temp = personas[j];
                personas[j] = personas[j + 1];
                personas[j + 1] = temp;
                intercambio = true;
            }
        }
        if (!intercambio) break;
    }
}
```

## Funcionalidades

### Básicas
- ✅ Definición de estructura con `typedef`
- ✅ Creación e inicialización de personas
- ✅ Mostrar datos en formato simple y detallado
- ✅ Validación de datos de entrada
- ✅ Copia de estructuras

### Intermedias
- ✅ Comparaciones entre personas
- ✅ Búsqueda en arrays por nombre
- ✅ Categorización por edad
- ✅ Funciones para encontrar mayor/menor

### Avanzadas
- ✅ Ordenamiento por edad y nombre
- ✅ Estadísticas (promedio de edad)
- ✅ Conteo por rangos de edad
- ✅ Validación de arrays completos
- ✅ Interfaz interactiva completa

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make

# Ejecutar programa principal
./estructura_persona

# Ejecutar tests
make run_tests

# Tests específicos
make run_tests_basic    # Tests básicos
make run_tests_arrays   # Tests de arrays
```

### Usando GCC directamente
```bash
gcc -std=c99 -Wall -Wextra -Iinclude src/*.c -o estructura_persona
./estructura_persona
```

## Uso del Programa

### Programa Principal
El programa demuestra múltiples aspectos de las estructuras:

```
=== Ejercicio 057: Estructura Persona ===

--- Ejemplo Básico ---
Nombre: Ana López
Edad: 28

--- Ejemplos Avanzados ---
Nombre: Carlos Ruiz, Edad: 35

--- Crear Nueva Persona ---
Introduce el nombre: María García
Introduce la edad: 32

Datos ingresados:
╔══════════════════════════════════════╗
║            DATOS PERSONALES          ║
╠══════════════════════════════════════╣
║ Nombre: María García                 ║
║ Edad:   32                           ║
║ Categoría: Adulto                    ║
║ Estado: Válido                       ║
╚══════════════════════════════════════╝
```

### Ejemplos de Uso

#### Comparaciones
```
Comparando edades:
Ana López (28 años)
María García (28 años)
→ Tienen la misma edad

Persona mayor entre María García y José Fernández: José Fernández (45 años)
```

#### Array de Personas
```
Lista de personas:
 1. Laura Martín (32 años)
 2. Pedro Sánchez (27 años)
 3. Elena Jiménez (41 años)
 4. David López (29 años)

Persona de mayor edad: Elena Jiménez (41 años)
Edad promedio del grupo: 32.2 años
```

#### Lista Ordenada
```
--- Lista Ordenada por Edad ---
 1. Pedro Sánchez (27 años)
 2. David López (29 años)
 3. Laura Martín (32 años)
 4. Elena Jiménez (41 años)
```

## Tests

El proyecto incluye una suite exhaustiva de tests con Criterion:

```bash
# Ejecutar todos los tests
make run_tests

# Tests con salida detallada
make run_tests_verbose

# Tests específicos
make run_tests_basic     # Creación y validación
make run_tests_arrays    # Operaciones con arrays
```

### Tests Implementados
- ✅ Creación de personas con datos válidos/inválidos
- ✅ Validación con diferentes casos límite
- ✅ Comparaciones entre personas
- ✅ Operaciones de copia
- ✅ Categorización por edad
- ✅ Búsquedas en arrays
- ✅ Ordenamiento por edad y nombre
- ✅ Estadísticas y conteos
- ✅ Tests parametrizados para validación
- ✅ Tests de integración completos

## Análisis de Complejidad

### Temporal
- **Crear persona**: O(1)
- **Validar persona**: O(1)
- **Buscar en array**: O(n)
- **Ordenar por edad**: O(n²) burbuja, O(n) si ya ordenado
- **Encontrar mayor/menor**: O(n)
- **Calcular promedio**: O(n)

### Espacial
- **Estructura Persona**: sizeof(char[MAX_NOMBRE]) + sizeof(int)
- **Array de n personas**: n × sizeof(Persona)
- **Operaciones**: O(1) adicional (ordenamiento in-place)

## Consideraciones de Diseño

### Ventajas de las Estructuras
- ✅ **Organización lógica**: Datos relacionados agrupados
- ✅ **Legibilidad**: Código más expresivo y mantenible
- ✅ **Reutilización**: Funciones genéricas para el tipo
- ✅ **Abstracción**: Oculta detalles de implementación

### Decisiones de Implementación
- **Array fijo vs puntero**: Array para simplicidad, evita malloc/free
- **Validación defensiva**: Verificar punteros NULL y rangos
- **Funciones const**: Parámetros de solo lectura cuando sea posible
- **Manejo de errores**: Valores de retorno claros (-1, NULL, false)

### Categorías de Edad Implementadas
```c
const char* obtener_categoria_edad(int edad) {
    if (edad < 0) return "Inválida";
    if (edad < 3) return "Bebé";
    if (edad < 12) return "Niño/a";
    if (edad < 18) return "Adolescente";
    if (edad < 30) return "Joven";
    if (edad < 60) return "Adulto";
    if (edad < 80) return "Adulto mayor";
    return "Anciano/a";
}
```

## Aplicaciones Prácticas

### Sistemas de Información
- Bases de datos de empleados
- Registros de estudiantes
- Sistemas de pacientes médicos
- Directorios de contactos

### Videojuegos
- Personajes con stats (nombre, nivel, vida)
- Inventarios de objetos
- Sistemas de puntuación
- Perfiles de jugadores

### Aplicaciones Científicas
- Datos experimentales agrupados
- Coordenadas geográficas
- Mediciones con metadatos
- Registros temporales

## Patrones Avanzados

### Estructuras Anidadas
```c
typedef struct {
    int dia, mes, año;
} Fecha;

typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
    Fecha nacimiento;
} PersonaCompleta;
```

### Arrays Flexibles
```c
typedef struct {
    int num_personas;
    Persona personas[];  // Array flexible
} GrupoPersonas;
```

### Punteros a Funciones en Estructuras
```c
typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
    void (*mostrar)(const Persona*);
} PersonaConComportamiento;
```

## Errores Comunes y Soluciones

### 1. Acceso a Miembros Incorrectos
```c
// ❌ Mal - confundir . y ->
Persona *p = &persona;
printf("%s\n", p.nombre);  // Error!

// ✅ Bien - usar -> para punteros
printf("%s\n", p->nombre);
// O equivalentemente:
printf("%s\n", (*p).nombre);
```

### 2. Copia Superficial vs Profunda
```c
// ❌ Mal - si la estructura contiene punteros
Persona p1 = p2;  // Solo copia punteros, no contenido

// ✅ Bien - función específica de copia
copiar_persona(&p1, &p2);
```

### 3. Inicialización Incompleta
```c
// ❌ Mal - dejar campos sin inicializar
Persona p;
strcpy(p.nombre, "Juan");
// p.edad contiene basura

// ✅ Bien - inicializar todos los campos
Persona p = {"Juan", 0};
// O usar función constructora
Persona p = crear_persona("Juan", 25);
```

### 4. Validación Insuficiente
```c
// ❌ Mal - no validar entrada
strcpy(p.nombre, nombre_usuario);  // Puede causar overflow

// ✅ Bien - copia segura con validación
strncpy(p.nombre, nombre_usuario, MAX_NOMBRE - 1);
p.nombre[MAX_NOMBRE - 1] = '\0';
```

## Optimizaciones

### 1. Alineación de Memoria
```c
// Considerar orden de campos para mejor alineación
typedef struct {
    int edad;           // 4 bytes
    char nombre[100];   // 100 bytes
    // Total: 104 bytes (bien alineado)
} PersonaOptimizada;
```

### 2. Comparaciones Eficientes
```c
// Para ordenamiento más eficiente
int comparar_personas_edad(const void *a, const void *b) {
    const Persona *p1 = (const Persona*)a;
    const Persona *p2 = (const Persona*)b;
    return p1->edad - p2->edad;
}

// Usar con qsort
qsort(personas, cantidad, sizeof(Persona), comparar_personas_edad);
```

### 3. Cache-Friendly Access
```c
// Procesar arrays de estructuras secuencialmente
for (int i = 0; i < cantidad; i++) {
    procesar_persona(&personas[i]);  // Mejor localidad
}
```

## Recursos Adicionales

- [Structures in C](https://en.cppreference.com/w/c/language/struct)
- [typedef in C](https://en.cppreference.com/w/c/language/typedef)
- [Memory Layout of Structures](https://stackoverflow.com/questions/119123/why-isnt-sizeof-for-a-struct-equal-to-the-sum-of-sizeof-of-each-member)
- [Struct Best Practices](https://stackoverflow.com/questions/1003119/when-should-one-use-a-struct-instead-of-a-class-in-c)
