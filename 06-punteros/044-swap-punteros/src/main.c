#include <stdio.h>
#include <stdlib.h>

/**
 * Intercambia los valores de dos enteros usando punteros
 * @param a Puntero al primer entero
 * @param b Puntero al segundo entero
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Intercambia valores sin variable temporal (usando XOR)
 * NOTA: Solo funciona si a != b (diferentes direcciones)
 * @param a Puntero al primer entero
 * @param b Puntero al segundo entero
 */
void swap_xor(int *a, int *b) {
    if (a != b) {  // Verificar que no son la misma dirección
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

/**
 * Intercambia valores usando aritmética (puede causar overflow)
 * @param a Puntero al primer entero
 * @param b Puntero al segundo entero
 */
void swap_aritmetico(int *a, int *b) {
    if (a != b) {
        *a = *a + *b;
        *b = *a - *b;
        *a = *a - *b;
    }
}

/**
 * Versión genérica de swap para cualquier tipo
 * @param a Puntero al primer elemento
 * @param b Puntero al segundo elemento
 * @param tamaño Tamaño en bytes del tipo de dato
 */
void swap_generico(void *a, void *b, size_t tamaño) {
    unsigned char *ptr_a = (unsigned char *)a;
    unsigned char *ptr_b = (unsigned char *)b;
    
    for (size_t i = 0; i < tamaño; i++) {
        unsigned char temp = ptr_a[i];
        ptr_a[i] = ptr_b[i];
        ptr_b[i] = temp;
    }
}

/**
 * Intercambia dos floats
 * @param a Puntero al primer float
 * @param b Puntero al segundo float
 */
void swap_float(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Intercambia dos caracteres
 * @param a Puntero al primer carácter
 * @param b Puntero al segundo carácter
 */
void swap_char(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Intercambia dos punteros (las direcciones, no los valores apuntados)
 * @param a Puntero al primer puntero
 * @param b Puntero al segundo puntero
 */
void swap_punteros(int **a, int **b) {
    int *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Demuestra el intercambio paso a paso con información detallada
 * @param a Puntero al primer entero
 * @param b Puntero al segundo entero
 */
void swap_con_debug(int *a, int *b) {
    printf("=== Intercambio Detallado ===\n");
    printf("Antes del intercambio:\n");
    printf("  *a = %d (dirección: %p)\n", *a, (void *)a);
    printf("  *b = %d (dirección: %p)\n", *b, (void *)b);
    
    printf("\nPaso 1: int temp = *a;\n");
    int temp = *a;
    printf("  temp = %d\n", temp);
    
    printf("\nPaso 2: *a = *b;\n");
    *a = *b;
    printf("  *a = %d\n", *a);
    
    printf("\nPaso 3: *b = temp;\n");
    *b = temp;
    printf("  *b = %d\n", *b);
    
    printf("\nDespués del intercambio:\n");
    printf("  *a = %d (dirección: %p)\n", *a, (void *)a);
    printf("  *b = %d (dirección: %p)\n", *b, (void *)b);
    printf("========================\n\n");
}

/**
 * Función incorrecta que intenta intercambiar por valor (no funciona)
 * @param a Primer valor (copia)
 * @param b Segundo valor (copia)
 */
void swap_incorrecto(int a, int b) {
    printf("Intento de intercambio incorrecto (por valor):\n");
    printf("  Antes en función: a = %d, b = %d\n", a, b);
    
    int temp = a;
    a = b;
    b = temp;
    
    printf("  Después en función: a = %d, b = %d\n", a, b);
    printf("  (Los valores originales NO cambian)\n\n");
}

/**
 * Valida punteros antes del intercambio
 * @param a Puntero al primer entero
 * @param b Puntero al segundo entero
 * @return 1 si los punteros son válidos, 0 si no
 */
int swap_seguro(int *a, int *b) {
    if (a == NULL || b == NULL) {
        printf("Error: Uno o ambos punteros son NULL\n");
        return 0;
    }
    
    printf("Intercambio seguro: %d <-> %d\n", *a, *b);
    swap(a, b);
    printf("Resultado: %d <-> %d\n", *a, *b);
    return 1;
}

/**
 * Intercambia elementos de un array usando índices
 * @param array Array de enteros
 * @param i Primer índice
 * @param j Segundo índice
 */
void swap_array_elementos(int array[], int i, int j) {
    printf("Intercambiando array[%d] = %d con array[%d] = %d\n", 
           i, array[i], j, array[j]);
    swap(&array[i], &array[j]);
    printf("Resultado: array[%d] = %d, array[%d] = %d\n", 
           i, array[i], j, array[j]);
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== INTERCAMBIO DE VALORES CON PUNTEROS ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("🔹 EJEMPLO BÁSICO:\n");
    int x = 5, y = 10;

    printf("Antes del intercambio:\n");
    printf("x = %d, y = %d\n", x, y);

    swap(&x, &y);

    printf("Después del intercambio:\n");
    printf("x = %d, y = %d\n\n", x, y);

    // Demostración detallada
    printf("🔹 INTERCAMBIO DETALLADO:\n");
    int a = 15, b = 25;
    swap_con_debug(&a, &b);

    // Comparación con método incorrecto
    printf("🔹 COMPARACIÓN: MÉTODO INCORRECTO\n");
    int p = 100, q = 200;
    printf("Valores originales: p = %d, q = %d\n", p, q);
    swap_incorrecto(p, q);  // Esto NO funciona
    printf("Valores después de función incorrecta: p = %d, q = %d\n\n", p, q);

    // Diferentes métodos de intercambio
    printf("🔹 MÉTODOS ALTERNATIVOS:\n");
    
    // Método XOR
    int m = 50, n = 75;
    printf("Método XOR - Antes: m = %d, n = %d\n", m, n);
    swap_xor(&m, &n);
    printf("Método XOR - Después: m = %d, n = %d\n\n", m, n);

    // Método aritmético
    int r = 30, s = 40;
    printf("Método aritmético - Antes: r = %d, s = %d\n", r, s);
    swap_aritmetico(&r, &s);
    printf("Método aritmético - Después: r = %d, s = %d\n\n", r, s);

    // Intercambio con diferentes tipos
    printf("🔹 INTERCAMBIO CON DIFERENTES TIPOS:\n");
    
    float f1 = 3.14f, f2 = 2.71f;
    printf("Floats - Antes: f1 = %.2f, f2 = %.2f\n", f1, f2);
    swap_float(&f1, &f2);
    printf("Floats - Después: f1 = %.2f, f2 = %.2f\n\n", f1, f2);

    char c1 = 'A', c2 = 'Z';
    printf("Chars - Antes: c1 = '%c', c2 = '%c'\n", c1, c2);
    swap_char(&c1, &c2);
    printf("Chars - Después: c1 = '%c', c2 = '%c'\n\n", c1, c2);

    // Intercambio genérico
    printf("🔹 INTERCAMBIO GENÉRICO:\n");
    int gen1 = 111, gen2 = 222;
    printf("Genérico (int) - Antes: gen1 = %d, gen2 = %d\n", gen1, gen2);
    swap_generico(&gen1, &gen2, sizeof(int));
    printf("Genérico (int) - Después: gen1 = %d, gen2 = %d\n\n", gen1, gen2);

    // Intercambio de punteros
    printf("🔹 INTERCAMBIO DE PUNTEROS:\n");
    int val1 = 555, val2 = 777;
    int *ptr1 = &val1, *ptr2 = &val2;
    
    printf("Antes del intercambio de punteros:\n");
    printf("  ptr1 apunta a: %d (dirección: %p)\n", *ptr1, (void *)ptr1);
    printf("  ptr2 apunta a: %d (dirección: %p)\n", *ptr2, (void *)ptr2);
    
    swap_punteros(&ptr1, &ptr2);
    
    printf("Después del intercambio de punteros:\n");
    printf("  ptr1 apunta a: %d (dirección: %p)\n", *ptr1, (void *)ptr1);
    printf("  ptr2 apunta a: %d (dirección: %p)\n", *ptr2, (void *)ptr2);
    printf("\n");

    // Intercambio seguro
    printf("🔹 INTERCAMBIO SEGURO:\n");
    int safe1 = 88, safe2 = 99;
    int *ptr_nulo = NULL;
    
    printf("Intercambio válido:\n");
    swap_seguro(&safe1, &safe2);
    
    printf("Intento con puntero nulo:\n");
    swap_seguro(&safe1, ptr_nulo);
    printf("\n");

    // Intercambio en arrays
    printf("🔹 INTERCAMBIO EN ARRAYS:\n");
    int array[] = {10, 20, 30, 40, 50};
    printf("Array original: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    swap_array_elementos(array, 0, 4);  // Intercambiar primer y último
    swap_array_elementos(array, 1, 3);  // Intercambiar segundo y cuarto
    
    printf("Array después de intercambios: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");

    // Casos especiales
    printf("🔹 CASOS ESPECIALES:\n");
    
    // Intercambio consigo mismo
    int auto_swap = 42;
    printf("Intercambio consigo mismo - Antes: %d\n", auto_swap);
    swap(&auto_swap, &auto_swap);
    printf("Intercambio consigo mismo - Después: %d\n", auto_swap);
    
    // Valores iguales
    int igual1 = 123, igual2 = 123;
    printf("Valores iguales - Antes: igual1 = %d, igual2 = %d\n", igual1, igual2);
    swap(&igual1, &igual2);
    printf("Valores iguales - Después: igual1 = %d, igual2 = %d\n", igual1, igual2);

    return 0;
}
#endif
