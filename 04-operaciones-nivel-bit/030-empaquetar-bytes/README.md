# Ejercicio 030: Empaquetar Bytes

## Descripción
Este ejercicio demuestra cómo combinar dos bytes individuales en un único valor de 16 bits (`unsigned short`) utilizando operaciones de desplazamiento de bits y OR. Esta técnica es fundamental en programación de sistemas, protocolos de red y optimización de memoria.

## Objetivo
Aprender a:
- Empaquetar dos bytes en un `unsigned short` usando desplazamiento y OR
- Desempaquetar un `unsigned short` para recuperar los bytes originales
- Comprender la representación binaria del empaquetado
- Mostrar resultados en formato hexadecimal y decimal
- Verificar la integridad del proceso de empaquetado/desempaquetado

## Conceptos Clave

### Empaquetado de Bytes
El empaquetado combina múltiples valores pequeños en un tipo de dato más grande:
```c
unsigned short resultado = ((unsigned short)byte2 << 8) | byte1;
```

### Operaciones Utilizadas
1. **Desplazamiento izquierdo (`<<`)**: Mueve byte2 a los 8 bits superiores
2. **OR binario (`|`)**: Combina los bytes sin conflictos
3. **Cast (`(unsigned short)`)**: Asegura el tipo correcto para el desplazamiento

### Desempaquetado
Para extraer los bytes originales:
```c
unsigned char byte1 = valor & 0xFF;           // Bits 0-7
unsigned char byte2 = (valor >> 8) & 0xFF;    // Bits 8-15
```

## Ejemplo de Funcionamiento

### Entrada
```
Primer byte: 86 (0x56)
Segundo byte: 18 (0x12)
```

### Proceso de Empaquetado
```
byte1 = 0x56 = 01010110 (binario)
byte2 = 0x12 = 00010010 (binario)

byte2 << 8 = 0x1200 = 0001001000000000 (binario)
byte1      = 0x0056 = 0000000001010110 (binario)
                     ________________
OR result  = 0x1256 = 0001001001010110 (binario)
```

### Salida
```
Resultado empaquetado:
  Decimal:      4694
  Hexadecimal:  0x1256
  Binario:      00010010 01010110
```

## Estructura del Código

### Funciones Principales
- `empaquetar_bytes()`: Combina dos bytes en un unsigned short
- `desempaquetar_bytes()`: Extrae los bytes originales
- `mostrar_binario()`: Visualiza la representación binaria
- `mostrar_detalles_empaquetado()`: Muestra información detallada del proceso

### Programa Principal
El programa:
1. Solicita dos valores de byte (0-255)
2. Valida que estén en el rango correcto
3. Realiza el empaquetado
4. Muestra los resultados en múltiples formatos
5. Verifica la integridad con desempaquetado
6. Presenta ejemplos adicionales

## Casos de Prueba

### Casos Básicos
- `empaquetar_bytes(0x12, 0x34)` → `0x3412` = 13330
- `empaquetar_bytes(255, 0)` → `0x00FF` = 255  
- `empaquetar_bytes(0, 255)` → `0xFF00` = 65280
- `empaquetar_bytes(255, 255)` → `0xFFFF` = 65535

### Casos Límite
- Valores mínimos: `(0, 0)` → `0x0000`
- Valores máximos: `(255, 255)` → `0xFFFF`
- Un byte máximo: `(255, 0)` y `(0, 255)`

## Aplicaciones Prácticas

### Protocolos de Red
- Empaquetado de puertos TCP/UDP (16 bits)
- Headers de protocolos
- Direcciones y flags de control

### Optimización de Memoria
- Almacenar múltiples valores pequeños eficientemente
- Reducir el uso de memoria en estructuras
- Acelerar transferencias de datos

### Comunicación con Hardware
- Registros de 16 bits en microcontroladores
- Comandos y parámetros empaquetados
- Interfaz con dispositivos externos

## Análisis de Rendimiento

### Ventajas del Empaquetado
- **Eficiencia de memoria**: 2 bytes → 1 short (sin overhead)
- **Velocidad**: Operaciones de bits son muy rápidas
- **Atomicidad**: Una sola operación para leer/escribir ambos bytes

### Consideraciones
- **Endianness**: El orden de bytes puede variar entre arquitecturas
- **Alineación**: Los shorts pueden requerir alineación específica
- **Portabilidad**: Verificar el tamaño de tipos en diferentes sistemas

## Compilación y Ejecución

### Compilar el programa principal
```bash
gcc -o main src/main.c -Wall -Wextra -std=c99
```

### Compilar y ejecutar tests
```bash
gcc -o test_main test/test_main.c -Wall -Wextra -std=c99
./test_main
```

### Usando CMake
```bash
mkdir build && cd build
cmake ..
make
./ejercicio_030
./test_ejercicio_030
```

## Salida Esperada

```
=== EJERCICIO 030: EMPAQUETAR BYTES ===
Programa que lee dos bytes y los combina en un unsigned short

Ingrese el primer byte (0-255): 86
Ingrese el segundo byte (0-255): 18

--- RESULTADOS ---
Primer byte:  86 (0x56)
Segundo byte: 18 (0x12)
Resultado empaquetado:
  Decimal:      4694
  Hexadecimal:  0x1256

--- Detalles del Empaquetado ---
Byte 1 (LSB): 86 (0x56)
Byte 2 (MSB): 18 (0x12)
Resultado:    4694 (0x1256)
Binario: 00010010 01010110
Verificación: Byte1=86, Byte2=18

--- EJEMPLOS ADICIONALES ---
Ejemplo 1: empaquetar(0x12, 0x34) = 0x3412 = 13330
Ejemplo 2: empaquetar(255, 0) = 0x00FF = 255
Ejemplo 3: empaquetar(0, 255) = 0xFF00 = 65280
Ejemplo 4: empaquetar(255, 255) = 0xFFFF = 65535
```

## Conceptos Avanzados

### Extensiones Posibles
- Empaquetado de 4 bytes en un `uint32_t`
- Empaquetado de flags booleanos en bits individuales
- Uso de `union` para reinterpretación de datos
- Campos de bits para estructuras optimizadas

### Técnicas Relacionadas
- Serialización de datos
- Compresión básica
- Protocolos binarios
- Interfaz con hardware de bajo nivel

## Notas de Implementación

### Seguridad de Tipos
- Validación de rangos de entrada (0-255)
- Uso de casts explícitos para claridad
- Verificación de integridad del round-trip

### Portabilidad
- Uso de tipos estándar (`unsigned char`, `unsigned short`)
- Independiente del endianness del sistema
- Compatible con estándares C99 y posteriores

### Eficiencia
- Operaciones de bits de costo constante O(1)
- Sin asignaciones dinámicas de memoria
- Código optimizable por el compilador
