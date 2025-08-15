#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "union_reinterpretar.h"

// Implementación de la función principal del enunciado
void mostrar_bytes_entero(int32_t valor) {
    union Datos d;
    d.entero = valor;
    
    printf("Valor entero: 0x%X (%d)\n", d.entero, d.entero);
    printf("Bytes individuales:\n");
    
    for (int i = 0; i < 4; i++) {
        printf("Byte %d: 0x%02X (%d)\n", i, 
               (unsigned char)d.bytes[i], (unsigned char)d.bytes[i]);
    }
}

union Datos crear_datos(int32_t valor) {
    union Datos datos;
    datos.entero = valor;
    return datos;
}

void imprimir_analisis_datos(union Datos datos) {
    printf("\n=== Análisis Detallado de Union Datos ===\n");
    printf("Valor entero: 0x%08X (%d)\n", datos.entero, datos.entero);
    printf("Tamaño de la unión: %zu bytes\n", sizeof(union Datos));
    
    printf("\nBytes individuales:\n");
    for (int i = 0; i < 4; i++) {
        char binario[9];
        byte_a_binario((uint8_t)datos.bytes[i], binario);
        printf("  Byte[%d]: 0x%02X = %3d = %s\n", 
               i, (unsigned char)datos.bytes[i], 
               (unsigned char)datos.bytes[i], binario);
    }
    
    // Mostrar interpretación como bytes con signo
    printf("\nBytes con signo:\n");
    for (int i = 0; i < 4; i++) {
        printf("  Byte[%d]: %4d\n", i, datos.bytes[i]);
    }
}

bool es_little_endian(void) {
    union {
        uint32_t entero;
        uint8_t bytes[4];
    } test = { .entero = 0x12345678 };
    
    return (test.bytes[0] == 0x78);
}

void mostrar_endianness(void) {
    printf("\n=== Análisis de Endianness ===\n");
    
    union Datos test;
    test.entero = 0x12345678;
    
    printf("Valor de prueba: 0x12345678\n");
    printf("Bytes en memoria: ");
    for (int i = 0; i < 4; i++) {
        printf("0x%02X ", (unsigned char)test.bytes[i]);
    }
    printf("\n");
    
    if (es_little_endian()) {
        printf("Sistema: Little-Endian (LSB primero)\n");
        printf("El byte menos significativo (0x78) se almacena primero\n");
    } else {
        printf("Sistema: Big-Endian (MSB primero)\n");
        printf("El byte más significativo (0x12) se almacena primero\n");
    }
}

void analizar_tipos_datos(union DatosExtendidos datos) {
    printf("\n=== Análisis de Tipos de Datos en Unión ===\n");
    printf("Tamaño de la unión: %zu bytes\n", sizeof(union DatosExtendidos));
    
    // Mostrar como diferentes tipos
    printf("\nInterpretaciones del mismo dato en memoria:\n");
    printf("  Como int32_t:  %d (0x%08X)\n", datos.entero, datos.entero);
    printf("  Como uint32_t: %u (0x%08X)\n", datos.sin_signo, datos.sin_signo);
    printf("  Como float:    %f\n", datos.flotante);
    
    printf("\nComo words de 16 bits:\n");
    for (int i = 0; i < 2; i++) {
        printf("  Word[%d]: 0x%04X (%u)\n", i, datos.words[i], datos.words[i]);
    }
    
    printf("\nComo bytes individuales:\n");
    for (int i = 0; i < 4; i++) {
        printf("  Byte[%d]: 0x%02X (%3u)\n", i, datos.octetos[i], datos.octetos[i]);
    }
}

int32_t intercambiar_endianness(int32_t valor) {
    union Datos original, intercambiado;
    original.entero = valor;
    
    // Intercambiar orden de bytes
    intercambiado.bytes[0] = original.bytes[3];
    intercambiado.bytes[1] = original.bytes[2];
    intercambiado.bytes[2] = original.bytes[1];
    intercambiado.bytes[3] = original.bytes[0];
    
    return intercambiado.entero;
}

void mostrar_representacion_binaria(int32_t valor) {
    union Datos datos;
    datos.entero = valor;
    
    printf("\n=== Representación Binaria ===\n");
    printf("Valor: %d (0x%08X)\n", valor, valor);
    
    // Mostrar bit por bit, byte por byte
    for (int byte = 3; byte >= 0; byte--) {
        char binario[9];
        byte_a_binario((uint8_t)datos.bytes[byte], binario);
        printf("Byte %d: %s (0x%02X)\n", byte, binario, 
               (unsigned char)datos.bytes[byte]);
    }
    
    // Mostrar como una cadena binaria completa
    printf("Completo: ");
    for (int byte = 3; byte >= 0; byte--) {
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (datos.bytes[byte] >> bit) & 1);
        }
        if (byte > 0) printf(" ");
    }
    printf("\n");
}

void analizar_float_ieee754(float valor) {
    union FloatBits fb;
    fb.valor = valor;
    
    printf("\n=== Análisis IEEE 754 de Float ===\n");
    printf("Valor float: %f\n", valor);
    printf("Representación en bits: 0x%08X\n", fb.bits);
    
    printf("\nComponentes IEEE 754:\n");
    printf("  Signo:     %u (bit 31)\n", fb.ieee754.signo);
    printf("  Exponente: %u (bits 30-23)\n", fb.ieee754.exponente);
    printf("  Mantisa:   %u (bits 22-0)\n", fb.ieee754.mantissa);
    
    // Mostrar interpretación
    if (fb.ieee754.signo) {
        printf("  Número negativo\n");
    } else {
        printf("  Número positivo\n");
    }
    
    int exponente_real = (int)fb.ieee754.exponente - 127;
    printf("  Exponente real: %d (bias de 127)\n", exponente_real);
    
    // Mostrar bytes
    printf("\nBytes del float:\n");
    union Datos float_bytes;
    float_bytes.entero = fb.bits;
    for (int i = 0; i < 4; i++) {
        printf("  Byte[%d]: 0x%02X\n", i, (unsigned char)float_bytes.bytes[i]);
    }
}

void demo_manipulacion_bits(int32_t valor) {
    union DatosExtendidos datos;
    datos.entero = valor;
    
    printf("\n=== Demostración de Manipulación de Bits ===\n");
    printf("Valor original: %d (0x%08X)\n", valor, valor);
    
    // Manipular bytes individuales
    printf("\nManipulando byte 0 (LSB):\n");
    datos.octetos[0] = 0xFF;
    printf("  Nuevo valor: %d (0x%08X)\n", datos.entero, datos.entero);
    
    // Resetear y manipular word
    datos.entero = valor;
    printf("\nManipulando word 0 (16 bits bajos):\n");
    datos.words[0] = 0xDEAD;
    printf("  Nuevo valor: %d (0x%08X)\n", datos.entero, datos.entero);
    
    // Resetear y trabajar con bits individuales
    datos.entero = valor;
    printf("\nInvertir todos los bits:\n");
    datos.entero = ~datos.entero;
    printf("  Valor invertido: %d (0x%08X)\n", datos.entero, datos.entero);
}

void comparar_signos(int32_t valor) {
    union DatosExtendidos datos;
    datos.entero = valor;
    
    printf("\n=== Comparación de Interpretaciones con/sin Signo ===\n");
    printf("Valor con signo (int32_t):    %d\n", datos.entero);
    printf("Valor sin signo (uint32_t):   %u\n", datos.sin_signo);
    printf("Diferencia: %s\n", 
           (datos.entero == (int32_t)datos.sin_signo) ? "Ninguna" : "Sí hay diferencia");
    
    // Caso especial: números negativos
    if (datos.entero < 0) {
        printf("Nota: El número es negativo en interpretación con signo\n");
        printf("      pero positivo en interpretación sin signo\n");
        printf("      Esto demuestra el bit de signo (MSB = 1)\n");
    }
}

void analizar_direccion_memoria(void *ptr) {
    if (!validar_puntero(ptr)) {
        printf("Puntero inválido\n");
        return;
    }
    
    union Puntero p;
    p.direccion = ptr;
    
    printf("\n=== Análisis de Dirección de Memoria ===\n");
    printf("Dirección: %p\n", ptr);
    printf("Valor numérico: %lu (0x%lX)\n", 
           (unsigned long)p.valor, (unsigned long)p.valor);
    
    printf("Bytes de la dirección:\n");
    for (size_t i = 0; i < sizeof(void*); i++) {
        printf("  Byte[%zu]: 0x%02X\n", i, p.bytes[i]);
    }
}

void demo_reinterpretacion_tipos(void) {
    printf("\n=== Demostración de Reinterpretación de Tipos ===\n");
    
    // Ejemplo 1: Entero a float
    union DatosExtendidos datos;
    datos.entero = 0x40490FDB;  // Aproximadamente π en IEEE 754
    
    printf("1. Reinterpretación entero -> float:\n");
    printf("   Como entero: %d (0x%08X)\n", datos.entero, datos.entero);
    printf("   Como float:  %f\n", datos.flotante);
    
    // Ejemplo 2: Float a entero
    datos.flotante = 3.14159f;
    printf("\n2. Reinterpretación float -> entero:\n");
    printf("   Como float:  %f\n", datos.flotante);
    printf("   Como entero: %d (0x%08X)\n", datos.entero, datos.entero);
    
    // Ejemplo 3: Análisis de NaN
    datos.flotante = 0.0f / 0.0f;  // NaN
    printf("\n3. Análisis de NaN:\n");
    printf("   Como float:  %f\n", datos.flotante);
    printf("   Como entero: %d (0x%08X)\n", datos.entero, datos.entero);
}

void ejemplos_practicos_uniones(void) {
    printf("\n=== Ejemplos Prácticos de Uniones ===\n");
    
    // Ejemplo 1: Parseo de protocolos de red
    printf("1. Simulación de header de protocolo de red:\n");
    union {
        struct {
            uint16_t puerto_origen;
            uint16_t puerto_destino;
        } header;
        uint32_t datos_completos;
    } protocolo;
    
    protocolo.header.puerto_origen = 80;    // HTTP
    protocolo.header.puerto_destino = 8080; // HTTP alternativo
    
    printf("   Puerto origen: %u\n", protocolo.header.puerto_origen);
    printf("   Puerto destino: %u\n", protocolo.header.puerto_destino);
    printf("   Datos completos: 0x%08X\n", protocolo.datos_completos);
    
    // Ejemplo 2: Conversión de colores
    printf("\n2. Representación de color RGBA:\n");
    union {
        struct {
            uint8_t r, g, b, a;
        } componentes;
        uint32_t valor;
    } color;
    
    color.valor = 0xFF00FF80;  // Magenta semi-transparente
    printf("   Color completo: 0x%08X\n", color.valor);
    printf("   R: %u, G: %u, B: %u, A: %u\n", 
           color.componentes.r, color.componentes.g, 
           color.componentes.b, color.componentes.a);
}

// Funciones de utilidad

void byte_a_binario(uint8_t byte, char *buffer) {
    for (int i = 7; i >= 0; i--) {
        buffer[7-i] = ((byte >> i) & 1) ? '1' : '0';
    }
    buffer[8] = '\0';
}

void entero_a_hex(int32_t valor, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "0x%08X", valor);
}

bool validar_puntero(void *ptr) {
    return ptr != NULL;
}

void imprimir_separador(const char *titulo) {
    printf("\n");
    for (int i = 0; i < 50; i++) {
        printf("=");
    }
    printf("\n");
    if (titulo) {
        printf(" %s \n", titulo);
        for (int i = 0; i < 50; i++) {
            printf("=");
        }
        printf("\n");
    }
}
