#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/**
 * Demuestra conceptos básicos de punteros
 * @param valor Puntero al valor a demostrar
 * @param nombre Nombre descriptivo de la variable
 */
void demostrar_puntero_basico(int *valor, const char *nombre) {
    printf("=== Análisis de Puntero: %s ===\n", nombre);
    printf("Dirección de la variable %s: %p\n", nombre, (void *)valor);
    printf("Valor de la variable %s: %d\n", nombre, *valor);
    printf("Tamaño del puntero: %zu bytes\n", sizeof(valor));
    printf("Tamaño del valor apuntado: %zu bytes\n", sizeof(*valor));
    printf("\n");
}

/**
 * Modifica un valor a través de un puntero
 * @param puntero Puntero al valor a modificar
 * @param nuevo_valor Nuevo valor a asignar
 */
void modificar_via_puntero(int *puntero, int nuevo_valor) {
    printf("Modificando valor de %d a %d via puntero...\n", *puntero, nuevo_valor);
    *puntero = nuevo_valor;
    printf("Modificación completada. Nuevo valor: %d\n", *puntero);
}

/**
 * Compara direcciones y valores de diferentes variables
 * @param var1 Puntero a primera variable
 * @param var2 Puntero a segunda variable
 * @param nombre1 Nombre de la primera variable
 * @param nombre2 Nombre de la segunda variable
 */
void comparar_punteros(int *var1, int *var2, const char *nombre1, const char *nombre2) {
    printf("=== Comparación de Punteros ===\n");
    printf("%s: dirección %p, valor %d\n", nombre1, (void *)var1, *var1);
    printf("%s: dirección %p, valor %d\n", nombre2, (void *)var2, *var2);
    
    /* Solo se pueden restar u ordenar punteros del mismo array. */
    printf("Misma dirección: %s\n", var1 == var2 ? "sí" : "no");
    printf("\n");
}

/**
 * Demuestra aritmética básica de punteros
 * @param base_ptr Puntero base
 * @param tamaño Número de elementos a mostrar
 */
void demostrar_aritmetica_punteros(int *base_ptr, int tamaño) {
    printf("=== Aritmética de Punteros ===\n");
    printf("Puntero base: %p\n", (void *)base_ptr);
    
    for (int i = 0; i < tamaño; i++) {
        int *ptr_actual = base_ptr + i;
        printf("base + %d = %p (diferencia: %td bytes)\n", 
               i, (void *)ptr_actual, (ptr_actual - base_ptr) * (ptrdiff_t)sizeof(*base_ptr));
    }
    printf("\n");
}

/**
 * Demuestra diferentes tipos de punteros
 */
void demostrar_tipos_punteros(void) {
    printf("=== Tamaños de Diferentes Tipos de Punteros ===\n");
    
    int entero = 42;
    float flotante = 3.14f;
    double doble = 2.71828;
    char caracter = 'A';
    
    int *ptr_int = &entero;
    float *ptr_float = &flotante;
    double *ptr_double = &doble;
    char *ptr_char = &caracter;
    void *ptr_void = &entero;
    
    printf("int*:    %zu bytes, apunta a: %p, valor: %d\n", 
           sizeof(ptr_int), (void *)ptr_int, *ptr_int);
    printf("float*:  %zu bytes, apunta a: %p, valor: %.2f\n", 
           sizeof(ptr_float), (void *)ptr_float, *ptr_float);
    printf("double*: %zu bytes, apunta a: %p, valor: %.5f\n", 
           sizeof(ptr_double), (void *)ptr_double, *ptr_double);
    printf("char*:   %zu bytes, apunta a: %p, valor: '%c'\n", 
           sizeof(ptr_char), (void *)ptr_char, *ptr_char);
    printf("void*:   %zu bytes, apunta a: %p (no se puede desreferenciar)\n", 
           sizeof(ptr_void), ptr_void);
    printf("\n");
}

/**
 * Función que intercambia valores usando punteros
 * @param a Puntero al primer valor
 * @param b Puntero al segundo valor
 */
void intercambiar(int *a, int *b) {
    printf("Intercambiando valores: *a=%d, *b=%d\n", *a, *b);
    int temp = *a;
    *a = *b;
    *b = temp;
    printf("Después del intercambio: *a=%d, *b=%d\n", *a, *b);
}

/**
 * Demuestra el concepto de punteros nulos y validación
 * @param ptr Puntero a validar
 * @return 1 si el puntero es válido, 0 si es nulo
 */
int validar_puntero(int *ptr) {
    if (ptr == NULL) {
        printf("⚠️  PUNTERO NULO detectado - no es seguro desreferenciar\n");
        return 0;
    } else {
        printf("✅ Puntero válido: %p, valor: %d\n", (void *)ptr, *ptr);
        return 1;
    }
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== INTRODUCCIÓN A PUNTEROS EN C ===\n\n");
    
    // Ejemplo básico del enunciado
    int numero = 10;
    int *puntero;

    puntero = &numero;

    printf("🔹 EJEMPLO BÁSICO:\n");
    printf("Antes de modificar:\n");
    printf("Valor de numero: %d\n", numero);
    printf("Dirección de numero: %p\n", (void *)&numero);
    printf("Valor apuntado por puntero: %d\n", *puntero);
    printf("Dirección almacenada en puntero: %p\n", (void *)puntero);

    // Modificamos el valor a través del puntero
    *puntero = 25;

    printf("\nDespués de modificar:\n");
    printf("Valor de numero: %d\n", numero);
    printf("Valor apuntado por puntero: %d\n", *puntero);
    printf("\n");

    // Demostraciones adicionales para aprendizaje
    printf("🔹 ANÁLISIS DETALLADO:\n");
    demostrar_puntero_basico(&numero, "numero");

    // Crear más variables para demostraciones
    int otra_variable = 100;
    int tercera_variable = 200;

    printf("🔹 COMPARACIÓN DE VARIABLES:\n");
    comparar_punteros(&numero, &otra_variable, "numero", "otra_variable");

    // Demostrar diferentes asignaciones de punteros
    printf("🔹 REASIGNACIÓN DE PUNTEROS:\n");
    printf("Puntero apunta inicialmente a numero (%d)\n", *puntero);
    
    puntero = &otra_variable;
    printf("Ahora puntero apunta a otra_variable (%d)\n", *puntero);
    
    puntero = &tercera_variable;
    printf("Ahora puntero apunta a tercera_variable (%d)\n", *puntero);
    printf("\n");

    // Demostrar modificación a través de punteros
    printf("🔹 MODIFICACIÓN VIA PUNTEROS:\n");
    modificar_via_puntero(&numero, 50);
    modificar_via_puntero(&otra_variable, 150);
    printf("\n");

    // Demostrar intercambio usando punteros
    printf("🔹 INTERCAMBIO DE VALORES:\n");
    printf("Antes del intercambio: numero=%d, otra_variable=%d\n", numero, otra_variable);
    intercambiar(&numero, &otra_variable);
    printf("Después del intercambio: numero=%d, otra_variable=%d\n", numero, otra_variable);
    printf("\n");

    // Demostrar diferentes tipos de punteros
    demostrar_tipos_punteros();

    // Demostrar aritmética de punteros con array local
    printf("🔹 ARITMÉTICA DE PUNTEROS:\n");
    int array_local[] = {10, 20, 30, 40, 50};
    demostrar_aritmetica_punteros(array_local, 5);

    // Demostrar validación de punteros
    printf("🔹 VALIDACIÓN DE PUNTEROS:\n");
    int *puntero_valido = &numero;
    int *puntero_nulo = NULL;
    
    printf("Validando puntero válido:\n");
    validar_puntero(puntero_valido);
    
    printf("Validando puntero nulo:\n");
    validar_puntero(puntero_nulo);
    printf("\n");

    // Resumen final
    printf("🔹 RESUMEN FINAL:\n");
    printf("Variables finales:\n");
    printf("  numero = %d (dirección: %p)\n", numero, (void *)&numero);
    printf("  otra_variable = %d (dirección: %p)\n", otra_variable, (void *)&otra_variable);
    printf("  tercera_variable = %d (dirección: %p)\n", tercera_variable, (void *)&tercera_variable);
    printf("  puntero apunta a: %p (valor apuntado: %d)\n", (void *)puntero, *puntero);

    return 0;
}
#endif
