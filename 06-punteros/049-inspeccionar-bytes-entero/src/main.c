#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * Función básica del enunciado: inspección simple de bytes
 * Implementación directa del ejercicio original
 */
void inspeccionBasica(void) {
    int numero = 305419896; // 0x12345678 en hexadecimal
    unsigned char *ptr = (unsigned char*)&numero;

    printf("Bytes en memoria de %d:\n", numero);
    for (int i = 0; i < (int)sizeof(int); i++) {
        printf("Byte %d: 0x%02X\n", i, ptr[i]);
    }
    printf("============================================\n\n");
}

/**
 * Inspecciona los bytes de un entero de 32 bits
 * @param valor Entero a inspeccionar
 */
void inspeccionarBytes(int32_t valor) {
    printf("=== Inspección de bytes para valor: %d ===\n", valor);
    printf("Representación hexadecimal: 0x%08X\n", valor);
    printf("Representación binaria: ");
    
    // Mostrar en binario
    for (int i = 31; i >= 0; i--) {
        printf("%d", (valor >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" ");
    }
    printf("\n\n");
    
    // Acceso byte por byte usando puntero
    unsigned char *ptr = (unsigned char*)&valor;
    
    printf("Inspección byte por byte:\n");
    printf("Dirección del entero: %p\n", (void*)&valor);
    printf("Tamaño del entero: %zu bytes\n\n", sizeof(valor));
    
    for (int i = 0; i < sizeof(int32_t); i++) {
        printf("Byte %d:\n", i);
        printf("  Dirección: %p\n", (void*)(ptr + i));
        printf("  Valor decimal: %u\n", ptr[i]);
        printf("  Valor hexadecimal: 0x%02X\n", ptr[i]);
        printf("  Valor binario: ");
        
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (ptr[i] >> bit) & 1);
        }
        printf("\n\n");
    }
    
    // Detectar endianness
    if (ptr[0] == (valor & 0xFF)) {
        printf("Sistema: Little Endian (byte menos significativo primero)\n");
    } else {
        printf("Sistema: Big Endian (byte más significativo primero)\n");
    }
    printf("============================================\n\n");
}

/**
 * Modifica un byte específico de un entero
 * @param valor Puntero al entero a modificar
 * @param byte_pos Posición del byte (0-3)
 * @param nuevo_valor Nuevo valor para el byte
 */
void modificarByte(int32_t *valor, int byte_pos, unsigned char nuevo_valor) {
    if (byte_pos < 0 || byte_pos >= sizeof(int32_t)) {
        printf("Error: Posición de byte inválida (%d). Debe estar entre 0 y %zu\n", 
               byte_pos, sizeof(int32_t) - 1);
        return;
    }
    
    printf("=== Modificación de byte ===\n");
    printf("Valor original: %d (0x%08X)\n", *valor, *valor);
    
    unsigned char *ptr = (unsigned char*)valor;
    unsigned char valor_anterior = ptr[byte_pos];
    
    printf("Byte %d antes: 0x%02X (%u)\n", byte_pos, valor_anterior, valor_anterior);
    
    ptr[byte_pos] = nuevo_valor;
    
    printf("Byte %d después: 0x%02X (%u)\n", byte_pos, nuevo_valor, nuevo_valor);
    printf("Valor final: %d (0x%08X)\n", *valor, *valor);
    printf("============================\n\n");
}

/**
 * Construye un entero desde bytes individuales
 * @param b0 Byte 0 (menos significativo en little endian)
 * @param b1 Byte 1
 * @param b2 Byte 2
 * @param b3 Byte 3 (más significativo en little endian)
 * @return Entero construido
 */
int32_t construirDesdeBytes(unsigned char b0, unsigned char b1, 
                           unsigned char b2, unsigned char b3) {
    printf("=== Construcción desde bytes ===\n");
    printf("Byte 0: 0x%02X (%u)\n", b0, b0);
    printf("Byte 1: 0x%02X (%u)\n", b1, b1);
    printf("Byte 2: 0x%02X (%u)\n", b2, b2);
    printf("Byte 3: 0x%02X (%u)\n", b3, b3);
    
    int32_t resultado;
    unsigned char *ptr = (unsigned char*)&resultado;
    
    ptr[0] = b0;
    ptr[1] = b1;
    ptr[2] = b2;
    ptr[3] = b3;
    
    printf("Entero resultante: %d (0x%08X)\n", resultado, resultado);
    printf("===============================\n\n");
    
    return resultado;
}

/**
 * Intercambia el orden de bytes (cambio de endianness)
 * @param valor Valor a procesar
 * @return Valor con bytes intercambiados
 */
int32_t intercambiarBytes(int32_t valor) {
    printf("=== Intercambio de bytes ===\n");
    printf("Valor original: %d (0x%08X)\n", valor, valor);
    
    unsigned char *ptr_orig = (unsigned char*)&valor;
    int32_t resultado = 0;
    unsigned char *ptr_result = (unsigned char*)&resultado;
    
    // Intercambiar orden de bytes
    ptr_result[0] = ptr_orig[3];
    ptr_result[1] = ptr_orig[2];
    ptr_result[2] = ptr_orig[1];
    ptr_result[3] = ptr_orig[0];
    
    printf("Valor intercambiado: %d (0x%08X)\n", resultado, resultado);
    printf("============================\n\n");
    
    return resultado;
}

/**
 * Extrae un rango de bits de un entero
 * @param valor Valor del cual extraer bits
 * @param inicio Bit de inicio (0-31)
 * @param longitud Número de bits a extraer
 * @return Valor extraído
 */
uint32_t extraerBits(int32_t valor, int inicio, int longitud) {
    if (inicio < 0 || inicio >= 32 || longitud <= 0 || inicio + longitud > 32) {
        printf("Error: Parámetros de extracción inválidos\n");
        return 0;
    }
    
    printf("=== Extracción de bits ===\n");
    printf("Valor original: %d (0x%08X)\n", valor, valor);
    printf("Extraer %d bits desde posición %d\n", longitud, inicio);
    
    // Crear máscara
    uint32_t mascara = (1U << longitud) - 1;
    uint32_t resultado = ((uint32_t)valor >> inicio) & mascara;
    
    printf("Máscara: 0x%08X\n", mascara);
    printf("Resultado: %u (0x%08X)\n", resultado, resultado);
    printf("=========================\n\n");
    
    return resultado;
}

/**
 * Compara diferentes tipos de datos y sus representaciones en memoria
 */
void compararTiposDatos(void) {
    printf("=== Comparación de tipos de datos ===\n\n");
    
    // Diferentes tipos con el mismo valor
    int8_t  val8 = 42;
    int16_t val16 = 42;
    int32_t val32 = 42;
    int64_t val64 = 42;
    
    float   valf = 42.0f;
    double  vald = 42.0;
    
    printf("Mismo valor (42) en diferentes tipos:\n\n");
    
    printf("int8_t (1 byte):\n");
    printf("  Dirección: %p\n", (void*)&val8);
    printf("  Tamaño: %zu bytes\n", sizeof(val8));
    unsigned char *ptr8 = (unsigned char*)&val8;
    printf("  Bytes: ");
    for (size_t i = 0; i < sizeof(val8); i++) {
        printf("0x%02X ", ptr8[i]);
    }
    printf("\n\n");
    
    printf("int16_t (2 bytes):\n");
    printf("  Dirección: %p\n", (void*)&val16);
    printf("  Tamaño: %zu bytes\n", sizeof(val16));
    unsigned char *ptr16 = (unsigned char*)&val16;
    printf("  Bytes: ");
    for (size_t i = 0; i < sizeof(val16); i++) {
        printf("0x%02X ", ptr16[i]);
    }
    printf("\n\n");
    
    printf("int32_t (4 bytes):\n");
    printf("  Dirección: %p\n", (void*)&val32);
    printf("  Tamaño: %zu bytes\n", sizeof(val32));
    unsigned char *ptr32 = (unsigned char*)&val32;
    printf("  Bytes: ");
    for (size_t i = 0; i < sizeof(val32); i++) {
        printf("0x%02X ", ptr32[i]);
    }
    printf("\n\n");
    
    printf("int64_t (8 bytes):\n");
    printf("  Dirección: %p\n", (void*)&val64);
    printf("  Tamaño: %zu bytes\n", sizeof(val64));
    unsigned char *ptr64 = (unsigned char*)&val64;
    printf("  Bytes: ");
    for (size_t i = 0; i < sizeof(val64); i++) {
        printf("0x%02X ", ptr64[i]);
    }
    printf("\n\n");
    
    printf("float (4 bytes):\n");
    printf("  Dirección: %p\n", (void*)&valf);
    printf("  Tamaño: %zu bytes\n", sizeof(valf));
    unsigned char *ptrf = (unsigned char*)&valf;
    printf("  Bytes: ");
    for (size_t i = 0; i < sizeof(valf); i++) {
        printf("0x%02X ", ptrf[i]);
    }
    printf("\n\n");
    
    printf("double (8 bytes):\n");
    printf("  Dirección: %p\n", (void*)&vald);
    printf("  Tamaño: %zu bytes\n", sizeof(vald));
    unsigned char *ptrd = (unsigned char*)&vald;
    printf("  Bytes: ");
    for (size_t i = 0; i < sizeof(vald); i++) {
        printf("0x%02X ", ptrd[i]);
    }
    printf("\n\n");
    
    printf("====================================\n\n");
}

/**
 * Función principal que demuestra el uso de todas las funciones
 */
#ifndef UNIT_TESTING
int main(void) {
    printf("=== EJERCICIO 049: INSPECCIONAR BYTES DE ENTERO ===\n\n");
    
    // 1. Inspección básica
    printf("1. INSPECCIÓN BÁSICA DE BYTES\n");
    printf("------------------------------\n");
    int32_t numero = 0x12345678;
    inspeccionarBytes(numero);
    
    numero = -1;
    inspeccionarBytes(numero);
    
    numero = 256;
    inspeccionarBytes(numero);
    
    // 2. Modificación de bytes
    printf("2. MODIFICACIÓN DE BYTES\n");
    printf("------------------------\n");
    numero = 0x12345678;
    printf("Valor inicial:\n");
    inspeccionarBytes(numero);
    
    modificarByte(&numero, 0, 0xAB);
    modificarByte(&numero, 2, 0xCD);
    
    printf("Valor después de modificaciones:\n");
    inspeccionarBytes(numero);
    
    // 3. Construcción desde bytes
    printf("3. CONSTRUCCIÓN DESDE BYTES\n");
    printf("---------------------------\n");
    int32_t construido = construirDesdeBytes(0x78, 0x56, 0x34, 0x12);
    inspeccionarBytes(construido);
    
    // 4. Intercambio de bytes
    printf("4. INTERCAMBIO DE BYTES (ENDIANNESS)\n");
    printf("------------------------------------\n");
    numero = 0x12345678;
    int32_t intercambiado = intercambiarBytes(numero);
    
    printf("Comparación:\n");
    printf("Original:     ");
    inspeccionarBytes(numero);
    printf("Intercambiado: ");
    inspeccionarBytes(intercambiado);
    
    // 5. Extracción de bits
    printf("5. EXTRACCIÓN DE BITS\n");
    printf("---------------------\n");
    numero = 0x12345678;
    
    extraerBits(numero, 0, 8);   // Byte menos significativo
    extraerBits(numero, 8, 8);   // Segundo byte
    extraerBits(numero, 16, 8);  // Tercer byte
    extraerBits(numero, 24, 8);  // Byte más significativo
    extraerBits(numero, 4, 12);  // 12 bits desde posición 4
    
    // 6. Comparación de tipos
    printf("6. COMPARACIÓN DE TIPOS DE DATOS\n");
    printf("---------------------------------\n");
    compararTiposDatos();
    
    // 7. Casos especiales
    printf("7. CASOS ESPECIALES\n");
    printf("-------------------\n");
    
    printf("Valor máximo de int32_t:\n");
    inspeccionarBytes(INT32_MAX);
    
    printf("Valor mínimo de int32_t:\n");
    inspeccionarBytes(INT32_MIN);
    
    printf("Cero:\n");
    inspeccionarBytes(0);
    
    printf("Todos los bits en 1:\n");
    inspeccionarBytes(-1);
    
    return 0;
}
#endif
