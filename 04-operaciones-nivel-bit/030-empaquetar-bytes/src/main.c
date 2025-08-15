#include <stdio.h>
#include <stdint.h>

// Función para empaquetar dos bytes en un unsigned short usando desplazamiento y OR
unsigned short empaquetar_bytes(unsigned char byte1, unsigned char byte2) {
    // byte2 se coloca en los 8 bits más significativos (MSB)
    // byte1 se coloca en los 8 bits menos significativos (LSB)
    return ((unsigned short)byte2 << 8) | byte1;
}

// Función para desempaquetar un unsigned short en dos bytes
void desempaquetar_bytes(unsigned short valor, unsigned char *byte1, unsigned char *byte2) {
    *byte1 = (unsigned char)(valor & 0xFF);         // Byte menos significativo
    *byte2 = (unsigned char)((valor >> 8) & 0xFF);  // Byte más significativo
}

// Función para mostrar la representación binaria de un valor
void mostrar_binario(unsigned short valor) {
    printf("Binario: ");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (valor >> i) & 1);
        if (i == 8) printf(" ");  // Separar bytes para mejor visualización
    }
    printf("\n");
}

// Función para mostrar información detallada del empaquetado
void mostrar_detalles_empaquetado(unsigned char byte1, unsigned char byte2, unsigned short resultado) {
    printf("\n--- Detalles del Empaquetado ---\n");
    printf("Byte 1 (LSB): %3d (0x%02X)\n", byte1, byte1);
    printf("Byte 2 (MSB): %3d (0x%02X)\n", byte2, byte2);
    printf("Resultado:    %5d (0x%04X)\n", resultado, resultado);
    mostrar_binario(resultado);
    
    // Verificación del desempaquetado
    unsigned char test_byte1, test_byte2;
    desempaquetar_bytes(resultado, &test_byte1, &test_byte2);
    printf("Verificación: Byte1=%d, Byte2=%d\n", test_byte1, test_byte2);
}

// (Eliminado) Implementación alternativa con unión no utilizada en este ejercicio

int main(void) {
    printf("=== EJERCICIO 030: EMPAQUETAR BYTES ===\n");
    printf("Programa que lee dos bytes y los combina en un unsigned short\n\n");
    
    unsigned char byte1, byte2;
    int input1, input2;
    
    // Leer primer byte
    printf("Ingrese el primer byte (0-255): ");
    if (scanf("%d", &input1) != 1) {
        printf("Error: Entrada inválida\n");
        return 1;
    }
    
    // Validar rango
    if (input1 < 0 || input1 > 255) {
        printf("Error: El valor debe estar entre 0 y 255\n");
        return 1;
    }
    byte1 = (unsigned char)input1;
    
    // Leer segundo byte
    printf("Ingrese el segundo byte (0-255): ");
    if (scanf("%d", &input2) != 1) {
        printf("Error: Entrada inválida\n");
        return 1;
    }
    
    // Validar rango
    if (input2 < 0 || input2 > 255) {
        printf("Error: El valor debe estar entre 0 y 255\n");
        return 1;
    }
    byte2 = (unsigned char)input2;
    
    // Empaquetar los dos bytes
    unsigned short resultado = empaquetar_bytes(byte1, byte2);
    
    // Mostrar resultados
    printf("\n--- RESULTADOS ---\n");
    printf("Primer byte:  %d (0x%02X)\n", byte1, byte1);
    printf("Segundo byte: %d (0x%02X)\n", byte2, byte2);
    printf("Resultado empaquetado:\n");
    printf("  Decimal:      %d\n", resultado);
    printf("  Hexadecimal:  0x%04X\n", resultado);
    
    // Mostrar detalles del empaquetado
    mostrar_detalles_empaquetado(byte1, byte2, resultado);
    
    // Demostración con valores ejemplo
    printf("\n--- EJEMPLOS ADICIONALES ---\n");
    
    // Ejemplo 1: 0x12 y 0x34
    unsigned short ej1 = empaquetar_bytes(0x12, 0x34);
    printf("Ejemplo 1: empaquetar(0x12, 0x34) = 0x%04X = %d\n", ej1, ej1);
    
    // Ejemplo 2: 255 y 0
    unsigned short ej2 = empaquetar_bytes(255, 0);
    printf("Ejemplo 2: empaquetar(255, 0) = 0x%04X = %d\n", ej2, ej2);
    
    // Ejemplo 3: 0 y 255
    unsigned short ej3 = empaquetar_bytes(0, 255);
    printf("Ejemplo 3: empaquetar(0, 255) = 0x%04X = %d\n", ej3, ej3);
    
    // Ejemplo 4: ambos bytes máximos
    unsigned short ej4 = empaquetar_bytes(255, 255);
    printf("Ejemplo 4: empaquetar(255, 255) = 0x%04X = %d\n", ej4, ej4);
    
    return 0;
}

// (Eliminada) Demostración no utilizada que dependía de funciones no definidas