# Ejercicio 059: Union para Reinterpretar Datos

## Descripción

Este ejercicio explora el uso de **uniones (unions)** en C para reinterpretar datos en memoria, permitiendo observar cómo se almacenan internamente los valores y conceptos fundamentales como el endianness.

## Objetivos de Aprendizaje

- Comprensión profunda de las uniones en C
- Exploración de la representación interna de datos
- Conceptos de endianness (little-endian vs big-endian)
- Reinterpretación de tipos de datos
- Manipulación de datos a nivel de bits y bytes
- Análisis de la representación IEEE 754 para números flotantes

## Funcionalidad

### Función Principal (Enunciado)
```c
union Datos {
    int entero;
    char bytes[4];
};
```

**Propósito**: Definir una unión que permita acceder a un entero de 32 bits tanto como valor completo como array de 4 bytes individuales.

### Características Implementadas

#### 🔍 **Análisis de Datos**
- Visualización de bytes individuales de enteros
- Detección automática de endianness del sistema
- Representación binaria completa
- Análisis hexadecimal detallado

#### 🧮 **Tipos de Datos Soportados**
- Enteros con y sin signo (32 bits)
- Números de punto flotante (IEEE 754)
- Arrays de bytes y words de 16 bits
- Direcciones de memoria (punteros)

#### ⚡ **Operaciones Avanzadas**
- Intercambio de endianness
- Manipulación de bits individuales
- Reinterpretación entre tipos
- Análisis de representaciones especiales (NaN, infinito)

#### 📊 **Análisis IEEE 754**
- Descomposición de signo, exponente y mantisa
- Visualización de casos especiales
- Comparación de representaciones

## Uniones Definidas

### Union Datos (Principal)
```c
union Datos {
    int32_t entero;     // Entero de 32 bits con signo
    char bytes[4];      // Array de 4 bytes
};
```

### Union DatosExtendidos
```c
union DatosExtendidos {
    int32_t entero;         // Entero con signo
    uint32_t sin_signo;     // Entero sin signo
    float flotante;         // Número flotante
    char bytes[4];          // Array de bytes
    uint16_t words[2];      // Array de words de 16 bits
    uint8_t octetos[4];     // Bytes sin signo
};
```

### Union FloatBits (IEEE 754)
```c
union FloatBits {
    float valor;            // Valor flotante
    uint32_t bits;          // Representación completa
    struct {
        uint32_t mantissa : 23;  // Mantisa (23 bits)
        uint32_t exponente : 8;  // Exponente (8 bits)
        uint32_t signo : 1;      // Bit de signo (1 bit)
    } ieee754;              // Estructura de campos de bits
};
```

## Uso del Programa

### Ejemplo Básico (Enunciado)
```bash
./union_reinterpretar
```

**Salida esperada:**
```
=== Ejercicio 059: Union para Reinterpretar Datos ===
Ejemplo básico del enunciado:

Valor entero: 0x12345678
Bytes individuales:
Byte 0: 0x78
Byte 1: 0x56
Byte 2: 0x34
Byte 3: 0x12
```

### Programa Interactivo
Para acceder al menú completo, editar `src/main.c` y activar `programa_interactivo()`.

## Conceptos Técnicos Demonstrados

### 1. Endianness
El programa detecta automáticamente si el sistema usa:
- **Little-Endian**: Byte menos significativo primero (x86, ARM)
- **Big-Endian**: Byte más significativo primero (algunos RISC)

```c
// En little-endian: 0x12345678 se almacena como [0x78, 0x56, 0x34, 0x12]
// En big-endian:    0x12345678 se almacena como [0x12, 0x34, 0x56, 0x78]
```

### 2. Representación IEEE 754
```c
union FloatBits fb;
fb.valor = 3.14159f;

// Acceso a componentes:
// fb.ieee754.signo     - Bit de signo (0 = positivo, 1 = negativo)
// fb.ieee754.exponente - Exponente sesgado (bias = 127)
// fb.ieee754.mantissa  - Mantisa/fracción (23 bits)
```

### 3. Reinterpretación de Tipos
```c
union DatosExtendidos datos;
datos.entero = -1;
printf("Como int32_t:  %d\n", datos.entero);      // -1
printf("Como uint32_t: %u\n", datos.sin_signo);   // 4294967295
printf("Como float:    %f\n", datos.flotante);    // Valor impredecible
```

## Ejemplos Prácticos

### 1. Análisis de Endianness
```c
void ejemplo_endianness() {
    union Datos d;
    d.entero = 0x12345678;
    
    printf("Valor: 0x%08X\n", d.entero);
    for (int i = 0; i < 4; i++) {
        printf("Byte[%d]: 0x%02X\n", i, (unsigned char)d.bytes[i]);
    }
    
    if (es_little_endian()) {
        printf("Sistema: Little-Endian\n");
    } else {
        printf("Sistema: Big-Endian\n");
    }
}
```

### 2. Análisis de Float
```c
void ejemplo_float() {
    union FloatBits fb;
    fb.valor = 3.14159f;
    
    printf("Float: %f\n", fb.valor);
    printf("Bits: 0x%08X\n", fb.bits);
    printf("Signo: %u\n", fb.ieee754.signo);
    printf("Exponente: %u\n", fb.ieee754.exponente);
    printf("Mantisa: %u\n", fb.ieee754.mantissa);
}
```

### 3. Manipulación de Bits
```c
void ejemplo_manipulacion() {
    union DatosExtendidos datos;
    datos.entero = 0x12345678;
    
    printf("Original: 0x%08X\n", datos.entero);
    
    // Modificar byte individual
    datos.octetos[0] = 0xFF;
    printf("Modificado: 0x%08X\n", datos.entero);
    
    // Trabajar con words de 16 bits
    datos.words[0] = 0xDEAD;
    printf("Con word: 0x%08X\n", datos.entero);
}
```

## Aplicaciones Prácticas

### 1. Protocolos de Red
```c
union {
    struct {
        uint16_t puerto_origen;
        uint16_t puerto_destino;
    } header;
    uint32_t datos_completos;
} protocolo_tcp;
```

### 2. Gráficos y Color
```c
union {
    struct {
        uint8_t r, g, b, a;
    } componentes;
    uint32_t valor_rgba;
} color;
```

### 3. Sistemas Embebidos
```c
union {
    struct {
        uint8_t comando : 4;
        uint8_t flags : 4;
    } control;
    uint8_t byte_control;
} registro_control;
```

## Consideraciones de Portabilidad

⚠️ **Importantes**:
- El orden de bytes depende de la arquitectura del sistema
- Las uniones con tipos flotantes pueden tener comportamiento indefinido
- El padding de estructuras puede afectar el tamaño de las uniones
- Algunos compiladores pueden optimizar de forma inesperada

✅ **Buenas Prácticas**:
- Verificar endianness antes de interpretar datos
- Usar tipos de tamaño fijo (`int32_t`, `uint16_t`)
- Validar datos antes de la reinterpretación
- Documentar las asunciones sobre el layout de memoria

## Extensiones Posibles

- **Análisis de doubles (64 bits)**: Soporte para IEEE 754 de doble precisión
- **Vectores SIMD**: Análisis de tipos como `__m128i`
- **Diferentes arquitecturas**: Comparación entre ARM, x86, RISC-V
- **Serialización**: Conversión para almacenamiento/transmisión
- **Herramientas de debug**: Visualizador hexadecimal interactivo

## Notas Técnicas

- **Tamaño de unión**: Igual al miembro más grande
- **Alineación**: Determinada por el miembro con mayor requerimiento
- **Inicialización**: Solo el primer miembro puede inicializarse en C99
- **Acceso concurrente**: No definido si se lee un miembro diferente al último escrito
- **Compiladores**: Compatible con GCC, Clang, MSVC

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/union_reinterpretar
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
