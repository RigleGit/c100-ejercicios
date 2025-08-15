# Ejercicio 062: Estructura con Campos de Bits (Bit Fields)

## Descripción

Este ejercicio demuestra el uso de **campos de bits (bit fields)** en C, una característica que permite especificar exactamente cuántos bits debe usar cada campo de una estructura. Esto es especialmente útil para optimizar el uso de memoria y para trabajar con registros de hardware o protocolos de comunicación donde el control preciso de bits es crucial.

## Conceptos Clave

### ¿Qué son los Bit Fields?

Los bit fields son una característica de C que permite:
- **Control preciso de memoria**: Especificar exactamente cuántos bits usa cada campo
- **Optimización de espacio**: Empaquetar múltiples valores booleanos en pocos bytes
- **Mapeo de hardware**: Representar registros de hardware donde cada bit tiene significado específico
- **Protocolos de red**: Implementar cabeceras de protocolos con precisión de bits

### Sintaxis

```c
struct {
    unsigned int campo1 : 1;    // 1 bit (0-1)
    unsigned int campo2 : 4;    // 4 bits (0-15)
    unsigned int campo3 : 2;    // 2 bits (0-3)
} estructura_bits;
```

### Ventajas y Limitaciones

**Ventajas:**
- Ahorro significativo de memoria
- Representación natural de estados binarios
- Ideal para flags y configuraciones
- Mapeo directo de especificaciones de hardware

**Limitaciones:**
- No se puede tomar la dirección de un bit field (`&campo` no está permitido)
- Comportamiento de padding dependiente del compilador
- Orden de bits puede variar entre arquitecturas
- Valores se truncan automáticamente al rango del campo

## Estructura de Archivos

```
062-estructura-campos-bits/
├── src/
│   └── bitfields_estado.c      # Implementación principal
├── include/
│   └── bitfields_estado.h      # Declaraciones y definiciones
├── tests/
│   └── test_bitfields_estado.c # Suite de tests con Criterion
├── CMakeLists.txt              # Configuración de compilación
└── README.md                   # Este archivo
```

## Implementación

### Estructura Principal

```c
typedef struct {
    unsigned int activo : 1;        // Estado activo/inactivo (0 o 1)
    unsigned int conectado : 1;     // Estado de conexión (0 o 1)
    unsigned int error : 1;         // Indica si hay error (0 o 1)
    unsigned int modo_debug : 1;    // Modo depuración (0 o 1)
    unsigned int reservado : 4;     // Bits reservados (0 a 15)
} Estado;
```

**Total: 8 bits** que se almacenan en una variable de tipo `unsigned int` (típicamente 4 bytes, con 24 bits de padding).

### Comparación de Memoria

```c
// Estructura con bit fields: ~4 bytes
typedef struct {
    unsigned int activo : 1;
    unsigned int conectado : 1;
    unsigned int error : 1;
    unsigned int modo_debug : 1;
    unsigned int reservado : 4;
} Estado;

// Estructura equivalente sin bit fields: 20 bytes
typedef struct {
    unsigned int activo;        // 4 bytes
    unsigned int conectado;     // 4 bytes
    unsigned int error;         // 4 bytes
    unsigned int modo_debug;    // 4 bytes
    unsigned int reservado;     // 4 bytes
} EstadoSinBitFields;

// Ahorro: ~80% de memoria
```

### Funcionalidades Implementadas

#### 1. Configuración Básica
- `configurar_estado_inicial()`: Establece valores por defecto
- `activar_modo_debug()` / `desactivar_modo_debug()`: Control del modo debug
- `reportar_error()` / `limpiar_error()`: Gestión de errores
- `conectar()` / `desconectar()`: Control de conexión

#### 2. Consultas de Estado
- `esta_activo()`, `esta_conectado()`, `hay_error()`, `esta_en_debug()`
- `esta_operativo()`: Verifica si el sistema está completamente funcional

#### 3. Manipulación con Máscaras
```c
#define ESTADO_ACTIVO     0x01  // Bit 0
#define ESTADO_CONECTADO  0x02  // Bit 1
#define ESTADO_ERROR      0x04  // Bit 2
#define ESTADO_DEBUG      0x08  // Bit 3

// Configurar múltiples campos simultáneamente
unsigned int mascara = ESTADO_ACTIVO | ESTADO_CONECTADO;
configurar_con_mascara(&estado, mascara);
```

#### 4. Utilidades
- `copiar_estado()`: Copia todos los campos
- `comparar_estados()`: Compara dos estructuras
- `estado_a_string()`: Representación textual del estado

## Compilación y Ejecución

### Requisitos
- CMake 3.15 o superior
- Compilador C (gcc, clang, msvc)
- Criterion (para tests) - opcional

### Compilar

```bash
mkdir build && cd build
cmake ..
make
```

### Ejecutar

```bash
# Programa principal
./bitfields_estado

# Tests (si Criterion está instalado)
make test
# o para salida detallada:
make test_verbose
```

### Instalar Criterion (para tests)

```bash
# Ubuntu/Debian
sudo apt-get install libcriterion-dev

# macOS
brew install criterion

# Arch Linux
sudo pacman -S criterion
```

## Ejemplos de Uso

### Ejemplo 1: Configuración Básica

```c
Estado sistema = {0};  // Inicializar todo a 0

// Configurar sistema operativo
sistema.activo = 1;
sistema.conectado = 1;
sistema.error = 0;
sistema.modo_debug = 0;

printf("Sistema: %s\n", estado_a_string(&sistema));
// Salida: [ACTIVO|CONECTADO]
```

### Ejemplo 2: Gestión de Errores

```c
Estado sistema = {
    .activo = 1,
    .conectado = 1
};

// Simular error
reportar_error(&sistema);
// Ahora: activo=1, conectado=0, error=1

if (!esta_operativo(&sistema)) {
    printf("Sistema no operativo: %s\n", estado_a_string(&sistema));
    // Salida: Sistema no operativo: [ACTIVO|DESCONECTADO|ERROR]
}
```

### Ejemplo 3: Uso de Máscaras

```c
Estado sistema = {0};

// Configurar con máscara
unsigned int config = ESTADO_ACTIVO | ESTADO_DEBUG;
configurar_con_mascara(&sistema, config);

// Convertir estado actual a máscara
unsigned int estado_actual = estado_a_mascara(&sistema);
printf("Máscara actual: 0x%02X\n", estado_actual);
```

### Ejemplo 4: Límites de Campos

```c
Estado e = {0};

// Campo de 1 bit: solo acepta 0 o 1
e.activo = 0;  // ✓ Válido
e.activo = 1;  // ✓ Válido
e.activo = 2;  // Se trunca a 0 (2 & 0x1 = 0)

// Campo de 4 bits: acepta 0-15
e.reservado = 15;  // ✓ Válido (valor máximo)
e.reservado = 16;  // Se trunca a 0 (16 & 0xF = 0)
e.reservado = 17;  // Se trunca a 1 (17 & 0xF = 1)
```

## Casos de Uso Reales

### 1. Registros de Hardware
```c
typedef struct {
    unsigned int enable : 1;
    unsigned int interrupt : 1;
    unsigned int priority : 3;
    unsigned int channel : 4;
    unsigned int reserved : 23;
} HardwareRegister;
```

### 2. Flags de Configuración
```c
typedef struct {
    unsigned int verbose : 1;
    unsigned int debug : 1;
    unsigned int force : 1;
    unsigned int recursive : 1;
    unsigned int compression_level : 4;
} ConfigFlags;
```

### 3. Protocolos de Red
```c
typedef struct {
    unsigned int version : 4;
    unsigned int header_length : 4;
    unsigned int type_of_service : 8;
    unsigned int total_length : 16;
} IPHeader;
```

## Características Avanzadas

### Análisis de Memoria
El programa incluye análisis detallado del uso de memoria:

```c
printf("Tamaño Estado (con bit fields): %zu bytes\n", sizeof(Estado));
printf("Tamaño EstadoSinBitFields: %zu bytes\n", sizeof(EstadoSinBitFields));
printf("Reducción: %.1f%%\n", 
       ((float)(sizeof(EstadoSinBitFields) - sizeof(Estado)) / sizeof(EstadoSinBitFields)) * 100);
```

### Demostración de Límites
Función educativa que muestra cómo se comportan los campos con valores fuera de rango:

```c
void demostrar_limites(void) {
    Estado e = {0};
    
    // Campo de 4 bits: mostrar truncamiento
    for (int i = 0; i <= 20; i++) {
        e.reservado = i;
        printf("%d -> %u\n", i, e.reservado);
    }
}
```

## Tests Implementados

La suite de tests cubre:

### Tests Básicos
- ✅ Inicialización correcta
- ✅ Asignación individual de campos
- ✅ Límites y truncamiento
- ✅ Tamaño de estructura

### Tests de Funcionalidad
- ✅ Todas las funciones de configuración
- ✅ Todas las funciones de consulta
- ✅ Manipulación con máscaras
- ✅ Funciones de utilidad

### Tests de Casos Límite
- ✅ Inicialización designada
- ✅ Valores máximos
- ✅ Overflow silencioso
- ✅ Comportamiento de truncamiento

## Conceptos Educativos

### 1. Eficiencia de Memoria
Los bit fields demuestran cómo optimizar el uso de memoria cuando tenemos muchos valores pequeños.

### 2. Representación Binaria
Ayuda a entender cómo se almacenan los datos a nivel de bits en la memoria.

### 3. Máscaras de Bits
Introduce el concepto de manipular bits individuales mediante operaciones lógicas.

### 4. Portabilidad
Enseña sobre las consideraciones de portabilidad al trabajar con representaciones específicas de datos.

## Ejercicios Propuestos

1. **Extensión**: Añadir más campos (prioridad: 2 bits, versión: 3 bits)
2. **Persistencia**: Implementar funciones para guardar/cargar estado desde archivo
3. **Comunicación**: Usar la estructura para simular un protocolo de comunicación
4. **Hardware**: Simular el control de un dispositivo con múltiples registros
5. **Optimización**: Comparar rendimiento vs. estructuras normales en bucles intensivos

## Notas Importantes

- ⚠️ **Portabilidad**: El orden de bits puede variar entre compiladores/arquitecturas
- ⚠️ **Alineación**: El compilador puede añadir padding para alineación
- ⚠️ **Direcciones**: No se puede usar `&` en campos de bits
- ⚠️ **Atomicidad**: Accesos concurrentes pueden ser problemáticos

## Referencias

- ISO C11 Standard, Section 6.7.2.1 (Structure and union specifiers)
- "The C Programming Language" - Kernighan & Ritchie, Chapter 6
- GCC Manual: Bit Fields
- "Expert C Programming" - Peter van der Linden
