/*
 * Ejercicio 027: Contar Bits 1
 * Cuenta cuántos bits están establecidos a 1 en la representación binaria
 * de un número entero positivo.
 * 
 * Conceptos: Análisis bit a bit, desplazamiento (>>), máscaras (&), bucles
 * Dificultad: Intermedio
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * Cuenta los bits establecidos a 1 usando método básico (shift y máscara)
 * @param numero El número a analizar
 * @return Cantidad de bits establecidos a 1
 */
int contar_bits_basico(unsigned int numero) {
    int cuenta = 0;
    
    while (numero != 0) {
        if (numero & 1) {  // Verificar si el LSB es 1
            cuenta++;
        }
        numero >>= 1;  // Desplazar una posición a la derecha
    }
    
    return cuenta;
}

/**
 * Cuenta los bits usando el truco de Brian Kernighan
 * Más eficiente: elimina el bit menos significativo en cada iteración
 * @param numero El número a analizar
 * @return Cantidad de bits establecidos a 1
 */
int contar_bits_kernighan(unsigned int numero) {
    int cuenta = 0;
    
    while (numero) {
        numero &= numero - 1;  // Elimina el bit 1 más a la derecha
        cuenta++;
    }
    
    return cuenta;
}

/**
 * Cuenta bits usando tabla de lookup para optimización
 * @param numero El número a analizar
 * @return Cantidad de bits establecidos a 1
 */
int contar_bits_tabla(unsigned int numero) {
    // Tabla para contar bits en un byte (0-255)
    static const int tabla_bits[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
    };
    
    return tabla_bits[numero & 0xFF] +
           tabla_bits[(numero >> 8) & 0xFF] +
           tabla_bits[(numero >> 16) & 0xFF] +
           tabla_bits[(numero >> 24) & 0xFF];
}

/**
 * Cuenta bits usando algoritmo de población paralela
 * Muy eficiente para números de 32 bits
 * @param numero El número a analizar
 * @return Cantidad de bits establecidos a 1
 */
int contar_bits_paralelo(unsigned int numero) {
    // Contar bits en paralelo usando máscaras
    numero = numero - ((numero >> 1) & 0x55555555);
    numero = (numero & 0x33333333) + ((numero >> 2) & 0x33333333);
    numero = (numero + (numero >> 4)) & 0x0F0F0F0F;
    numero = numero + (numero >> 8);
    numero = numero + (numero >> 16);
    return numero & 0x3F;
}

/**
 * Muestra la representación binaria de un número con bits marcados
 * @param numero El número a mostrar
 * @param bits_mostrar Cantidad de bits a mostrar (máximo 32)
 */
void mostrar_binario_marcado(unsigned int numero, int bits_mostrar) {
    printf("Binario (%d bits): ", bits_mostrar);
    
    for (int i = bits_mostrar - 1; i >= 0; i--) {
        int bit = (numero >> i) & 1;
        if (bit) {
            printf("\033[1;32m%d\033[0m", bit);  // Verde para bits 1
        } else {
            printf("\033[1;30m%d\033[0m", bit);  // Gris para bits 0
        }
        
        if (i % 4 == 0 && i > 0) {
            printf(" ");  // Espacio cada 4 bits
        }
    }
    printf("\n");
}

/**
 * Analiza un número mostrando diferentes representaciones
 * @param numero El número a analizar
 */
void analizar_numero_completo(unsigned int numero) {
    printf("\n=== Análisis completo del número %u ===\n", numero);
    
    // Mostrar en diferentes bases
    printf("Decimal: %u\n", numero);
    printf("Hexadecimal: 0x%X\n", numero);
    printf("Octal: 0%o\n", numero);
    
    // Mostrar representación binaria
    mostrar_binario_marcado(numero, 32);
    
    // Contar bits con diferentes métodos
    int bits_basico = contar_bits_basico(numero);
    int bits_kernighan = contar_bits_kernighan(numero);
    int bits_tabla = contar_bits_tabla(numero);
    int bits_paralelo = contar_bits_paralelo(numero);
    
    printf("\nConteo de bits establecidos a 1:\n");
    printf("Método básico: %d\n", bits_basico);
    printf("Método Kernighan: %d\n", bits_kernighan);
    printf("Método tabla: %d\n", bits_tabla);
    printf("Método paralelo: %d\n", bits_paralelo);
    
    // Verificar consistencia
    if (bits_basico == bits_kernighan && bits_kernighan == bits_tabla && 
        bits_tabla == bits_paralelo) {
        printf("✓ Todos los métodos son consistentes\n");
    } else {
        printf("✗ Error: Inconsistencia entre métodos\n");
    }
    
    // Información adicional
    printf("\nInformación adicional:\n");
    printf("Peso de Hamming: %d\n", bits_basico);
    printf("Paridad: %s\n", (bits_basico % 2 == 0) ? "PAR" : "IMPAR");
    printf("Es potencia de 2: %s\n", (bits_basico == 1) ? "SÍ" : "NO");
}

/**
 * Demuestra el algoritmo de Kernighan paso a paso
 * @param numero El número a analizar
 */
void demo_kernighan(unsigned int numero) {
    printf("\n=== Demostración del algoritmo de Kernighan ===\n");
    printf("Número inicial: %u\n", numero);
    
    unsigned int temp = numero;
    int paso = 1;
    int cuenta = 0;
    
    while (temp) {
        printf("Paso %d:\n", paso);
        printf("  Antes: ");
        mostrar_binario_marcado(temp, 16);
        
        temp &= temp - 1;  // Eliminar el bit 1 más a la derecha
        cuenta++;
        
        printf("  Después: ");
        mostrar_binario_marcado(temp, 16);
        printf("  Bits contados hasta ahora: %d\n", cuenta);
        paso++;
    }
    
    printf("Total de bits 1: %d\n", cuenta);
}

/**
 * Analiza patrones en secuencias de números
 * @param inicio Inicio del rango
 * @param fin Fin del rango
 */
void analizar_patron_bits(unsigned int inicio, unsigned int fin) {
    printf("\n=== Análisis de patrones de bits del %u al %u ===\n", inicio, fin);
    printf("Número\tBinario\t\tBits 1\tParidad\n");
    printf("------\t-------\t\t------\t-------\n");
    
    for (unsigned int i = inicio; i <= fin && i <= inicio + 15; i++) {
        int bits = contar_bits_basico(i);
        const char* paridad = (bits % 2 == 0) ? "PAR" : "IMPAR";
        
        printf("%u\t", i);
        for (int j = 7; j >= 0; j--) {
            printf("%d", (i >> j) & 1);
        }
        printf("\t%d\t%s\n", bits, paridad);
    }
    
    if (fin > inicio + 15) {
        printf("... (mostrando solo primeros 16)\n");
    }
}

/**
 * Demuestra propiedades matemáticas relacionadas con bits
 */
void demo_propiedades_matematicas(void) {
    printf("\n=== Propiedades Matemáticas ===\n");
    
    // Potencias de 2 (siempre tienen exactamente 1 bit establecido)
    printf("Potencias de 2 (siempre 1 bit):\n");
    for (int i = 0; i < 16; i++) {
        unsigned int potencia = 1U << i;
        int bits = contar_bits_basico(potencia);
        printf("2^%d = %u\tbits: %d\n", i, potencia, bits);
    }
    
    // Números de Mersenne (2^n - 1, todos los bits en 1)
    printf("\nNúmeros de Mersenne (2^n - 1, todos bits en 1):\n");
    for (int i = 1; i <= 16; i++) {
        unsigned int mersenne = (1U << i) - 1;
        int bits = contar_bits_basico(mersenne);
        printf("2^%d - 1 = %u\tbits: %d\n", i, mersenne, bits);
    }
}

/**
 * Función principal que demuestra el conteo de bits
 */
int main(void) {
    printf("=== Ejercicio 027: Contar Bits 1 ===\n\n");
    
    // Casos de demostración
    unsigned int numeros_demo[] = {0, 1, 3, 7, 15, 31, 85, 170, 255, 1023, 12345};
    int num_demos = sizeof(numeros_demo) / sizeof(numeros_demo[0]);
    
    printf("Demostración básica:\n");
    printf("Número\tBinario\t\tBits 1\n");
    printf("------\t-------\t\t------\n");
    
    for (int i = 0; i < num_demos; i++) {
        unsigned int num = numeros_demo[i];
        int bits = contar_bits_basico(num);
        
        printf("%u\t", num);
        for (int j = 15; j >= 0; j--) {
            printf("%d", (num >> j) & 1);
        }
        printf("\t%d\n", bits);
    }
    
    // Comparación de métodos
    printf("\n=== Comparación de Métodos ===\n");
    printf("Número\tBásico\tKernighan\tTabla\tParalelo\n");
    printf("------\t------\t---------\t-----\t--------\n");
    
    unsigned int test_numbers[] = {42, 255, 1023, 65535, 1234567};
    int num_tests = sizeof(test_numbers) / sizeof(test_numbers[0]);
    
    for (int i = 0; i < num_tests; i++) {
        unsigned int num = test_numbers[i];
        printf("%u\t%d\t%d\t\t%d\t%d\n",
               num,
               contar_bits_basico(num),
               contar_bits_kernighan(num),
               contar_bits_tabla(num),
               contar_bits_paralelo(num));
    }
    
    // Demostrar propiedades matemáticas
    demo_propiedades_matematicas();
    
    // Analizar patrones
    analizar_patron_bits(0, 31);
    
    // Entrada del usuario
    printf("\n=== Prueba Interactiva ===\n");
    unsigned int numero;
    printf("Introduce un número entero positivo: ");
    scanf("%u", &numero);
    
    int cuenta = contar_bits_basico(numero);
    printf("Número de bits a 1: %d\n", cuenta);
    
    // Análisis completo del número introducido
    analizar_numero_completo(numero);
    
    // Demostración del algoritmo de Kernighan
    if (numero > 0 && numero <= 65535) {  // Limitar para no saturar la salida
        demo_kernighan(numero);
    }
    
    return 0;
}
