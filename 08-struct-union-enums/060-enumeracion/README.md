# Ejercicio 060: Enumeración de Días de la Semana

## Descripción

Este ejercicio introduce el uso de **enumeraciones (enum)** en C para representar conjuntos finitos de constantes simbólicas, mejorando la legibilidad, mantenibilidad y expresividad del código.

## Objetivos de Aprendizaje

- Comprensión de enumeraciones en C
- Uso de constantes simbólicas en lugar de números mágicos
- Mejora de la legibilidad y mantenibilidad del código
- Implementación de máquinas de estado simples
- Conversiones entre tipos enum y otros tipos de datos
- Validación y manejo de errores con enumeraciones

## Funcionalidad

### Función Principal (Enunciado)
```c
enum DiaSemana { LUN = 1, MAR, MIE, JUE, VIE, SAB, DOM };
const char* nombreDia(enum DiaSemana dia);
```

**Propósito**: Convertir un número (1-7) introducido por el usuario al nombre correspondiente del día de la semana.

### Características Implementadas

#### 📅 **Días de la Semana**
- Enumeración con valores 1-7 (lunes a domingo)
- Conversión número ↔ día
- Nombres en español, inglés y abreviados
- Operaciones circulares (día siguiente/anterior)
- Clasificación (laboral/fin de semana)

#### 🗓️ **Meses y Estaciones**
- Enumeración completa de meses del año
- Cálculo de días por mes (incluyendo años bisiestos)
- Asociación automática con estaciones
- Transiciones estacionales

#### ⚙️ **Máquinas de Estado**
- Estados de máquina con valores personalizados
- Transiciones válidas entre estados
- Manejo de estados de error
- Validación de transiciones

#### 🔢 **Prioridades y Códigos de Error**
- Sistema de prioridades con valores ponderados
- Códigos de error descriptivos
- Comparación y validación de estados

## Estructura del Proyecto

```
060-enumeracion/
├── src/
│   ├── enumeracion.c           # Implementación principal
│   └── main.c                  # Programa demostración
├── include/
│   └── enumeracion.h           # Definiciones de enumeraciones
├── tests/
│   └── test_enumeracion.c      # Tests unitarios
├── CMakeLists.txt              # Configuración de build
├── README.md                   # Este archivo
└── .gitignore                  # Exclusiones de Git
```

## Enumeraciones Definidas

### DiaSemana (Principal)
```c
enum DiaSemana { 
    LUN = 1,    // Lunes (1)
    MAR,        // Martes (2)
    MIE,        // Miércoles (3)
    JUE,        // Jueves (4)
    VIE,        // Viernes (5)
    SAB,        // Sábado (6)
    DOM         // Domingo (7)
};
```

### Mes
```c
enum Mes {
    ENERO = 1, FEBRERO, MARZO, ABRIL, MAYO, JUNIO,
    JULIO, AGOSTO, SEPTIEMBRE, OCTUBRE, NOVIEMBRE, DICIEMBRE
};
```

### Estacion
```c
enum Estacion {
    PRIMAVERA,  // 0
    VERANO,     // 1
    OTONO,      // 2
    INVIERNO    // 3
};
```

### EstadoMaquina
```c
enum EstadoMaquina {
    APAGADO = 0,
    INICIANDO = 10,
    FUNCIONANDO = 20,
    PAUSADO = 30,
    ERROR = 99
};
```

### Prioridad
```c
enum Prioridad {
    BAJA = 1,
    MEDIA = 5,
    ALTA = 10,
    CRITICA = 20
};
```

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make

# Ejecutar programa básico
./enumeracion

# Ejecutar tests (requiere Criterion)
make run_tests
```

### Compilación Manual
```bash
# Compilar programa principal
gcc -std=c99 -Wall -Wextra -Iinclude -o enumeracion src/*.c

# Compilar tests (requiere Criterion)
gcc -std=c99 -Wall -Wextra -Iinclude -o test_enumeracion src/enumeracion.c tests/test_enumeracion.c -lcriterion
```

## Uso del Programa

### Ejemplo Básico (Enunciado)
```bash
./enumeracion
```

**Entrada/Salida esperada:**
```
=== Programa Simple - Días de la Semana ===
Número 1 -> Lunes
Número 3 -> Miércoles
Número 6 -> Sábado
Número 7 -> Domingo
Número 0 -> Día inválido
Número 8 -> Día inválido

Ciclo de la semana:
Lunes -> Martes
Martes -> Miércoles
Miércoles -> Jueves
Jueves -> Viernes
Viernes -> Sábado
Sábado -> Domingo
Domingo -> Lunes
```

### Programa Interactivo
Para acceder al menú completo, editar `src/main.c` y activar `programaInteractivoEnum()`.

## Ejemplos de Uso

### 1. Conversión Básica (Enunciado)
```c
#include "enumeracion.h"

int main() {
    int num;
    printf("Introduce un número del 1 al 7: ");
    scanf("%d", &num);
    
    enum DiaSemana dia = (enum DiaSemana) num;
    printf("Día seleccionado: %s\n", nombreDia(dia));
    
    return 0;
}
```

### 2. Operaciones con Días
```c
enum DiaSemana hoy = MIE;

printf("Hoy es: %s\n", nombreDia(hoy));
printf("Mañana es: %s\n", nombreDia(diaSiguiente(hoy)));
printf("Ayer fue: %s\n", nombreDia(diaAnterior(hoy)));
printf("¿Es fin de semana? %s\n", esFinDeSemana(hoy) ? "Sí" : "No");
printf("¿Es día laboral? %s\n", esDiaLaboral(hoy) ? "Sí" : "No");
```

### 3. Máquina de Estados
```c
enum EstadoMaquina estado = APAGADO;

while (estadoPermiteTransicion(estado)) {
    printf("Estado actual: %s\n", descripcionEstado(estado));
    estado = siguienteEstado(estado);
}
```

### 4. Análisis de Meses
```c
enum Mes mes = FEBRERO;
bool bisiesto = true;

printf("Mes: %s\n", nombreMes(mes));
printf("Días: %d\n", diasDelMes(mes, bisiesto));
printf("Estación: %s\n", nombreEstacion(estacionDelMes(mes)));
```

### 5. Sistema de Prioridades
```c
enum Prioridad tareas[] = {ALTA, BAJA, CRITICA, MEDIA};
int num_tareas = 4;

// Ordenar por prioridad (ejemplo conceptual)
for (int i = 0; i < num_tareas - 1; i++) {
    for (int j = 0; j < num_tareas - 1 - i; j++) {
        if (compararPrioridades(tareas[j], tareas[j + 1]) < 0) {
            // Intercambiar si j tiene menor prioridad que j+1
            enum Prioridad temp = tareas[j];
            tareas[j] = tareas[j + 1];
            tareas[j + 1] = temp;
        }
    }
}
```

## Tests

El proyecto incluye tests exhaustivos con **Criterion**:

### Categorías de Tests
- ✅ **Enum básico**: Valores, nombres, conversiones
- ✅ **Operaciones con días**: Siguiente, anterior, clasificaciones
- ✅ **Nombres y conversiones**: Multilenguaje, abreviaciones, parsing
- ✅ **Meses**: Nombres, días, estaciones
- ✅ **Estaciones**: Nombres, transiciones
- ✅ **Estados de máquina**: Descripciones, transiciones, validaciones
- ✅ **Prioridades y errores**: Comparaciones, validaciones
- ✅ **Funciones auxiliares**: Conteo, búsquedas
- ✅ **Casos límite**: Valores inválidos, consistencia circular

### Ejecutar Tests
```bash
# Con CMake
make run_tests

# Directamente
./test_enumeracion

# Tests específicos
./test_enumeracion --filter test_enum_basico
./test_enumeracion --filter test_operaciones_dias
```

## Conceptos Técnicos Demostrados

### 1. Enumeraciones Básicas
```c
// Valores automáticos (0, 1, 2, ...)
enum Color { ROJO, VERDE, AZUL };

// Valores personalizados
enum DiaSemana { LUN = 1, MAR, MIE, JUE, VIE, SAB, DOM };

// Valores no consecutivos
enum EstadoMaquina { APAGADO = 0, INICIANDO = 10, ERROR = 99 };
```

### 2. Ventajas de las Enumeraciones
- **Legibilidad**: `if (dia == VIE)` vs `if (dia == 5)`
- **Mantenibilidad**: Cambios centralizados en la definición
- **Detección de errores**: El compilador puede detectar inconsistencias
- **Documentación**: El código se autodocumenta

### 3. Conversiones y Validaciones
```c
// Conversión segura con validación
enum DiaSemana convertir_numero(int num) {
    if (num >= LUN && num <= DOM) {
        return (enum DiaSemana)num;
    }
    return (enum DiaSemana)0; // Valor inválido
}

// Validación
bool es_valido(enum DiaSemana dia) {
    return (dia >= LUN && dia <= DOM);
}
```

### 4. Switch con Enumeraciones
```c
// El compilador puede advertir sobre casos no manejados
const char* dia_a_string(enum DiaSemana dia) {
    switch (dia) {
        case LUN: return "Lunes";
        case MAR: return "Martes";
        // ... todos los casos
        default: return "Día inválido";
    }
}
```

## Aplicaciones Prácticas

### 1. Estados de Aplicación
```c
enum EstadoApp {
    INICIALIZANDO,
    MENU_PRINCIPAL,
    PROCESANDO,
    ESPERANDO_USUARIO,
    CERRANDO
};
```

### 2. Tipos de Archivo
```c
enum TipoArchivo {
    TEXTO = 1,
    IMAGEN = 2,
    VIDEO = 3,
    AUDIO = 4,
    DOCUMENTO = 5
};
```

### 3. Códigos de Respuesta HTTP
```c
enum CodigoHTTP {
    OK = 200,
    NOT_FOUND = 404,
    INTERNAL_ERROR = 500
};
```

### 4. Direcciones de Movimiento
```c
enum Direccion {
    NORTE,
    SUR,
    ESTE,
    OESTE
};
```

## Buenas Prácticas Demostradas

✅ **Nomenclatura**: Nombres descriptivos y consistentes  
✅ **Valores explícitos**: Asignar valores cuando sea importante  
✅ **Validación**: Verificar valores antes de usar  
✅ **Documentación**: Comentar el propósito de cada valor  
✅ **Funciones helper**: Crear funciones para conversiones comunes  
✅ **Casos por defecto**: Manejar valores inválidos en switch  
✅ **Modularidad**: Agrupar enums relacionados  

## Consideraciones Importantes

⚠️ **Limitaciones**:
- Los enums en C son básicamente enteros
- No hay verificación automática de tipos en tiempo de ejecución
- Los valores pueden estar fuera del rango definido

✅ **Soluciones**:
- Implementar funciones de validación
- Usar convenciones de nomenclatura claras
- Documentar los rangos válidos
- Crear funciones de conversión seguras

## Extensiones Posibles

- **Enums con strings**: Mapeo automático a cadenas
- **Serialización**: Guardar/cargar enums desde archivos
- **Internacionalización**: Soporte para múltiples idiomas
- **Validación avanzada**: Rangos y restricciones complejas
- **Metadatos**: Asociar información adicional a cada valor

## Notas Técnicas

- **Tamaño**: Los enums ocupan típicamente el tamaño de un `int`
- **Conversión**: Conversión implícita a enteros, explícita desde enteros
- **Ámbito**: Los nombres están en el mismo ámbito que la declaración
- **Compilación**: Compatible con C90 y posteriores
- **Portabilidad**: Funciona en todas las plataformas
