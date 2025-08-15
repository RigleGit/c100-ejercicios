# Ejercicio 049: Inspeccionar Bytes de Entero

## Descripción

Este ejercicio demuestra cómo inspeccionar y manipular la representación en memoria de enteros a nivel de bytes. Comienza con una implementación básica del enunciado original y extiende a técnicas avanzadas de manipulación de memoria.

**Enunciado Original**: Escribir un programa que tome una variable `int` y utilice un puntero a `unsigned char` para recorrer e imprimir sus bytes en memoria en formato hexadecimal.

El ejercicio proporciona una comprensión profunda de:

- Representación binaria y hexadecimal de enteros
- Endianness (orden de bytes en memoria)
- Manipulación directa de bytes usando punteros
- Extracción y modificación de bits específicos
- Diferencias entre tipos de datos a nivel de memoria

## Implementación Básica

### Código del Enunciado Original

```c
void inspeccionBasica() {
    int numero = 305419896; // 0x12345678 en hexadecimal
    unsigned char *ptr = (unsigned char*)&numero;

    printf("Bytes en memoria de %d:\n", numero);
    for (int i = 0; i < sizeof(int); i++) {
        printf("Byte %d: 0x%02X\n", i, ptr[i]);
    }
}
```

### Conceptos Clave de la Implementación Básica

1. **Casting de Punteros**: `(unsigned char*)&numero` convierte la dirección del entero a un puntero de bytes
2. **Acceso a Bytes**: `ptr[i]` accede al byte i-ésimo de la representación en memoria
3. **Formato Hexadecimal**: `%02X` muestra cada byte como hexadecimal con 2 dígitos

### Salida Esperada (Little Endian)
```
Bytes en memoria de 305419896:
Byte 0: 0x78
Byte 1: 0x56
Byte 2: 0x34
Byte 3: 0x12
```

## Conceptos Avanzados

### 1. Representación en Memoria
Los enteros se almacenan en memoria como secuencias de bytes. Un `int32_t` ocupa 4 bytes (32 bits), y podemos acceder a cada byte individualmente usando punteros.

### 2. Endianness
- **Little Endian**: El byte menos significativo se almacena primero (x86, x64)
- **Big Endian**: El byte más significativo se almacena primero (algunos ARM, MIPS)

### 3. Conversión de Tipos
Usar punteros para reinterpretar la memoria como diferentes tipos de datos, permitiendo manipulación a nivel de bits.

## Funciones Implementadas

### Función Básica

```c
void inspeccionBasica(void)
```
Implementación directa del enunciado original para visualizar bytes de un entero.

### Funciones Avanzadas

```c
void inspeccionarBytes(int32_t valor)
```
Muestra la representación completa de un entero: binaria, hexadecimal, y byte por byte.

```c
void modificarByte(int32_t *valor, int byte_pos, unsigned char nuevo_valor)
```
Modifica un byte específico de un entero sin afectar los demás.

```c
int32_t construirDesdeBytes(unsigned char b0, b1, b2, b3)
```
Construye un entero a partir de 4 bytes individuales.

```c
int32_t intercambiarBytes(int32_t valor)
```
Intercambia el orden de los bytes (conversión entre endianness).

```c
uint32_t extraerBits(int32_t valor, int inicio, int longitud)
```
Extrae un rango específico de bits de un entero.

```c
void compararTiposDatos(void)
```
Compara la representación en memoria de diferentes tipos de datos.

## Características Técnicas

### Manipulación de Bytes
- Acceso directo a bytes individuales usando `unsigned char*`
- Modificación sin afectar otros bytes
- Construcción de enteros desde bytes

### Operaciones de Bits
- Extracción de rangos específicos de bits
- Uso de máscaras binarias para manipulación
- Operaciones de desplazamiento (`>>`, `<<`)

### Detección de Endianness
```c
unsigned char *ptr = (unsigned char*)&valor;
if (ptr[0] == (valor & 0xFF)) {
    // Little Endian
} else {
    // Big Endian
}
```

### Casos de Uso Educativos
- Análisis de protocolos de red
- Compresión y empaquetado de datos
- Optimización de algoritmos
- Debugging de bajo nivel

## Compilación y Ejecución

### Requisitos
- Compilador C (gcc, clang)
- CMake 3.16+
- Criterion (para tests)

### Instrucciones

```bash
# Crear directorio de construcción
mkdir build && cd build

# Configurar proyecto
cmake ..

# Compilar
make

# Ejecutar programa principal
./inspeccionar_bytes_entero

# Ejecutar tests
./test_inspeccionar_bytes_entero
```

## Ejemplo de Salida

```
=== INSPECCIÓN DE BYTES PARA VALOR: 305419896 ===
Representación hexadecimal: 0x12345678
Representación binaria: 00010010001101000101011001111000

Inspección byte por byte:
Dirección del entero: 0x7fff5fbff6ac
Tamaño del entero: 4 bytes

Byte 0:
  Dirección: 0x7fff5fbff6ac
  Valor decimal: 120
  Valor hexadecimal: 0x78
  Valor binario: 01111000

Byte 1:
  Dirección: 0x7fff5fbff6ad
  Valor decimal: 86
  Valor hexadecimal: 0x56
  Valor binario: 01010110

Sistema: Little Endian (byte menos significativo primero)
```

## Tests Incluidos

### Tests de Funcionalidad
- ✅ Inspección de valores básicos
- ✅ Detección correcta de endianness
- ✅ Modificación de bytes individuales
- ✅ Validación de índices inválidos

### Tests de Construcción
- ✅ Construcción desde bytes
- ✅ Valores extremos (0, -1)
- ✅ Integridad de datos

### Tests de Extracción
- ✅ Extracción de bytes completos
- ✅ Extracción de nibbles (4 bits)
- ✅ Manejo de parámetros inválidos

### Tests de Intercambio
- ✅ Intercambio básico de bytes
- ✅ Doble intercambio (reversibilidad)

### Tests de Integración
- ✅ Flujo completo de operaciones
- ✅ Comparación de tipos de datos

## Aplicaciones Prácticas

### 1. Análisis de Protocolos
```c
// Extraer campos de un header de red
uint32_t version = extraerBits(header, 28, 4);
uint32_t longitud = extraerBits(header, 16, 12);
```

### 2. Empaquetado de Datos
```c
// Empaquetar múltiples valores en un entero
int32_t empaquetado = construirDesdeBytes(flag, tipo, longitud, checksum);
```

### 3. Conversión de Endianness
```c
// Convertir para transmisión de red
int32_t network_order = intercambiarBytes(host_order);
```

### 4. Debugging de Memoria
```c
// Inspeccionar contenido de memoria sospechoso
inspeccionarBytes(valor_corrupto);
```

## Conceptos Avanzados

### Union para Reinterpretación
```c
union {
    int32_t entero;
    unsigned char bytes[4];
} conversor;
```

### Macros para Endianness
```c
#define SWAP_BYTES(x) ((x>>24) | ((x&0xFF0000)>>8) | ((x&0xFF00)<<8) | (x<<24))
```

### Optimizaciones del Compilador
El uso de `volatile` puede ser necesario para evitar optimizaciones no deseadas:
```c
volatile int32_t valor = 0x12345678;
```

## Consideraciones de Portabilidad

- **Endianness**: El código detecta automáticamente el endianness del sistema
- **Tamaños de tipos**: Usa tipos de tamaño fijo (`int32_t`) para consistencia
- **Alineación**: Ten en cuenta la alineación de memoria en diferentes arquitecturas

Este ejercicio proporciona una base sólida para entender la representación de datos en memoria y es fundamental para la programación de sistemas, redes, y optimización de rendimiento.
