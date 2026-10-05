#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/**
 * Suma los elementos de un array usando aritmética de punteros
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 * @return La suma de todos los elementos
 */
int suma_array_punteros(int *array, size_t tamaño) {
    int suma = 0;
    int *ptr;
    
    // Recorrer array usando aritmética de punteros
    for (ptr = array; ptr < array + tamaño; ptr++) {
        suma += *ptr;
    }
    
    return suma;
}

/**
 * Suma array usando punteros con información de debug
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 * @return La suma de todos los elementos
 */
int suma_array_debug(int *array, size_t tamaño) {
    int suma = 0;
    int *ptr;
    size_t indice = 0;
    
    printf("=== Recorrido detallado del array ===\n");
    printf("Dirección base del array: %p\n", (void *)array);
    printf("Tamaño del array: %zu elementos\n", tamaño);
    printf("Tamaño de cada elemento: %zu bytes\n\n", sizeof(int));
    
    for (ptr = array; ptr < array + tamaño; ptr++) {
        printf("Posición %zu:\n", indice);
        printf("  Dirección: %p\n", (void *)ptr);
        printf("  Valor: %d\n", *ptr);
        printf("  Offset desde base: %ld bytes\n", (char *)ptr - (char *)array);
        printf("  array[%zu] == *(array + %zu) == %d\n", indice, indice, *ptr);
        
        suma += *ptr;
        printf("  Suma parcial: %d\n\n", suma);
        indice++;
    }
    
    printf("Suma total: %d\n", suma);
    printf("=====================================\n\n");
    
    return suma;
}

/**
 * Suma array usando diferentes estilos de aritmética de punteros
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 * @return La suma de todos los elementos
 */
int suma_array_estilos(int *array, size_t tamaño) {
    printf("🔹 DIFERENTES ESTILOS DE ARITMÉTICA DE PUNTEROS:\n\n");
    
    // Estilo 1: Incremento del puntero
    printf("Estilo 1 - Incremento del puntero:\n");
    int suma1 = 0;
    int *ptr1 = array;
    for (size_t i = 0; i < tamaño; i++) {
        printf("  *ptr1 = %d\n", *ptr1);
        suma1 += *ptr1++;  // Suma y luego incrementa el puntero
    }
    printf("  Suma: %d\n\n", suma1);
    
    // Estilo 2: Puntero con offset
    printf("Estilo 2 - Puntero con offset:\n");
    int suma2 = 0;
    for (size_t i = 0; i < tamaño; i++) {
        printf("  *(array + %zu) = %d\n", i, *(array + i));
        suma2 += *(array + i);
    }
    printf("  Suma: %d\n\n", suma2);
    
    // Estilo 3: Comparación de punteros
    printf("Estilo 3 - Comparación de punteros:\n");
    int suma3 = 0;
    int *ptr3 = array;
    int *end = array + tamaño;
    while (ptr3 < end) {
        printf("  *ptr3 = %d (dirección: %p)\n", *ptr3, (void *)ptr3);
        suma3 += *ptr3;
        ptr3++;
    }
    printf("  Suma: %d\n\n", suma3);
    
    // Verificar que todos dan el mismo resultado
    printf("Verificación - Todas las sumas son iguales: %s\n\n", 
           (suma1 == suma2 && suma2 == suma3) ? "✓ SÍ" : "✗ NO");
    
    return suma1;
}

/**
 * Compara métodos: índices vs punteros
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 */
void comparar_metodos(int *array, size_t tamaño) {
    printf("🔹 COMPARACIÓN: ÍNDICES vs PUNTEROS:\n\n");
    
    // Método tradicional con índices
    printf("Método con índices:\n");
    int suma_indices = 0;
    for (size_t i = 0; i < tamaño; i++) {
        printf("  array[%zu] = %d\n", i, array[i]);
        suma_indices += array[i];
    }
    printf("  Suma: %d\n\n", suma_indices);
    
    // Método con punteros
    printf("Método con punteros:\n");
    int suma_punteros = 0;
    int *ptr = array;
    for (size_t i = 0; i < tamaño; i++) {
        printf("  *(ptr + %zu) = %d\n", i, *(ptr + i));
        suma_punteros += *(ptr + i);
    }
    printf("  Suma: %d\n\n", suma_punteros);
    
    printf("Equivalencias importantes:\n");
    printf("  array[i] ≡ *(array + i)\n");
    printf("  &array[i] ≡ (array + i)\n");
    printf("  array ≡ &array[0]\n\n");
}

/**
 * Demuestra aritmética de punteros con diferentes tipos
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 */
void demo_aritmetica_punteros(int *array, size_t tamaño) {
    printf("🔹 DEMOSTRACIÓN DE ARITMÉTICA DE PUNTEROS:\n\n");
    
    int *inicio = array;
    int *fin = array + tamaño;
    
    printf("Información del array:\n");
    printf("  Dirección de inicio: %p\n", (void *)inicio);
    printf("  Dirección de fin: %p\n", (void *)fin);
    printf("  Diferencia en bytes: %ld\n", (char *)fin - (char *)inicio);
    printf("  Diferencia en elementos: %ld\n", fin - inicio);
    printf("  Tamaño de cada int: %zu bytes\n\n", sizeof(int));
    
    printf("Recorrido con diferentes operadores:\n");
    int *ptr = inicio;
    for (size_t i = 0; i < tamaño; i++) {
        printf("  Elemento %zu:\n", i);
        printf("    ptr = %p, *ptr = %d\n", (void *)ptr, *ptr);
        printf("    ptr++ -> avanza a %p\n", (void *)(ptr + 1));
        printf("    Equivalencias: array[%zu] = *(array+%zu) = %d\n", i, i, array[i]);
        ptr++;
        printf("\n");
    }
}

/**
 * Suma array recorriendo hacia atrás (desde el final)
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 * @return La suma de todos los elementos
 */
int suma_array_reverso(int *array, size_t tamaño) {
    int suma = 0;
    if (tamaño == 0) return 0;
    int *ptr = array + tamaño;  // Una posición después del último elemento
    
    printf("Recorrido hacia atrás:\n");
    for (size_t i = tamaño; i > 0; i--) {
        --ptr;
        printf("  *ptr = %d (posición %zu)\n", *ptr, i - 1);
        suma += *ptr;
    }
    printf("  Suma total: %d\n\n", suma);
    
    return suma;
}

/**
 * Encuentra el máximo y mínimo usando punteros
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 * @param max Puntero donde guardar el valor máximo
 * @param min Puntero donde guardar el valor mínimo
 */
void encontrar_max_min_punteros(int *array, size_t tamaño, int *max, int *min) {
    if (tamaño == 0) return;
    
    *max = *min = *array;  // Inicializar con el primer elemento
    
    int *ptr = array + 1;  // Comenzar desde el segundo elemento
    int *end = array + tamaño;
    
    while (ptr < end) {
        if (*ptr > *max) {
            *max = *ptr;
        }
        if (*ptr < *min) {
            *min = *ptr;
        }
        ptr++;
    }
}

/**
 * Cuenta elementos que cumplen una condición usando punteros
 * @param array Puntero al primer elemento del array
 * @param tamaño Número de elementos en el array
 * @param condicion Función que determina si un elemento cumple la condición
 * @return Número de elementos que cumplen la condición
 */
int contar_con_condicion(int *array, size_t tamaño, int (*condicion)(int)) {
    int contador = 0;
    int *ptr = array;
    int *end = array + tamaño;
    
    while (ptr < end) {
        if (condicion(*ptr)) {
            contador++;
        }
        ptr++;
    }
    
    return contador;
}

// Funciones auxiliares para condiciones
int es_par(int num) {
    return num % 2 == 0;
}

int es_positivo(int num) {
    return num > 0;
}

int es_mayor_que_cinco(int num) {
    return num > 5;
}

/**
 * Copia array usando punteros
 * @param origen Puntero al array origen
 * @param destino Puntero al array destino
 * @param tamaño Número de elementos a copiar
 */
void copiar_array_punteros(int *origen, int *destino, size_t tamaño) {
    int *ptr_origen = origen;
    int *ptr_destino = destino;
    int *end = origen + tamaño;
    
    printf("Copiando array usando punteros:\n");
    size_t i = 0;
    while (ptr_origen < end) {
        *ptr_destino = *ptr_origen;
        printf("  destino[%zu] = origen[%zu] = %d\n", i, i, *ptr_origen);
        ptr_origen++;
        ptr_destino++;
        i++;
    }
    printf("\n");
}

/**
 * Suma arrays de diferentes tipos usando punteros genéricos
 */
void demo_suma_tipos(void) {
    printf("🔹 SUMA DE DIFERENTES TIPOS CON PUNTEROS:\n\n");
    
    // Array de floats
    float floats[] = {1.5f, 2.3f, 3.7f, 4.1f};
    float suma_float = 0.0f;
    float *ptr_f = floats;
    size_t tamaño_f = sizeof(floats) / sizeof(floats[0]);
    
    printf("Suma de floats:\n");
    for (size_t i = 0; i < tamaño_f; i++) {
        printf("  *(ptr_f + %zu) = %.1f\n", i, *(ptr_f + i));
        suma_float += *(ptr_f + i);
    }
    printf("  Suma total: %.1f\n\n", suma_float);
    
    // Array de doubles
    double doubles[] = {10.25, 20.75, 30.5};
    double suma_double = 0.0;
    double *ptr_d = doubles;
    size_t tamaño_d = sizeof(doubles) / sizeof(doubles[0]);
    
    printf("Suma de doubles:\n");
    for (size_t i = 0; i < tamaño_d; i++) {
        printf("  *(ptr_d + %zu) = %.2f\n", i, *(ptr_d + i));
        suma_double += *(ptr_d + i);
    }
    printf("  Suma total: %.2f\n\n", suma_double);
}

#ifndef UNIT_TESTING
int main(void) {
    printf("=== RECORRER ARRAY CON PUNTEROS ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("🔹 EJEMPLO BÁSICO:\n");
    int valores[] = {3, 7, 2, 8, 4};
    int suma = 0;
    int *ptr;

    printf("Array: ");
    for (size_t i = 0; i < 5; i++) {
        printf("%d ", valores[i]);
    }
    printf("\n\n");

    printf("Recorrido con punteros:\n");
    for (ptr = valores; ptr < valores + 5; ptr++) {
        printf("  *ptr = %d (dirección: %p)\n", *ptr, (void *)ptr);
        suma += *ptr;
    }

    printf("\nLa suma de los elementos es: %d\n\n", suma);

    // Demostración detallada
    printf("🔹 ANÁLISIS DETALLADO:\n");
    int array_demo[] = {10, 20, 30, 40, 50};
    size_t tamaño = sizeof(array_demo) / sizeof(array_demo[0]);
    
    suma_array_debug(array_demo, tamaño);
    
    // Diferentes estilos de recorrido
    suma_array_estilos(array_demo, tamaño);
    
    // Comparación de métodos
    comparar_metodos(array_demo, tamaño);
    
    // Demostración de aritmética de punteros
    demo_aritmetica_punteros(array_demo, tamaño);
    
    // Recorrido hacia atrás
    printf("🔹 RECORRIDO INVERSO:\n");
    suma_array_reverso(array_demo, tamaño);
    
    // Encontrar máximo y mínimo
    printf("🔹 BÚSQUEDA DE MÁXIMO Y MÍNIMO:\n");
    int max, min;
    encontrar_max_min_punteros(array_demo, tamaño, &max, &min);
    printf("Máximo: %d, Mínimo: %d\n\n", max, min);
    
    // Conteo con condiciones
    printf("🔹 CONTEO CON CONDICIONES:\n");
    int array_mixto[] = {-2, 3, 6, -1, 8, 5, -7, 9};
    size_t tamaño_mixto = sizeof(array_mixto) / sizeof(array_mixto[0]);
    
    printf("Array: ");
    for (size_t i = 0; i < tamaño_mixto; i++) {
        printf("%d ", array_mixto[i]);
    }
    printf("\n");
    
    int pares = contar_con_condicion(array_mixto, tamaño_mixto, es_par);
    int positivos = contar_con_condicion(array_mixto, tamaño_mixto, es_positivo);
    int mayores_cinco = contar_con_condicion(array_mixto, tamaño_mixto, es_mayor_que_cinco);
    
    printf("Números pares: %d\n", pares);
    printf("Números positivos: %d\n", positivos);
    printf("Números > 5: %d\n\n", mayores_cinco);
    
    // Copia de arrays
    printf("🔹 COPIA DE ARRAYS:\n");
    int origen[] = {100, 200, 300};
    int destino[3];
    size_t tamaño_copia = sizeof(origen) / sizeof(origen[0]);
    
    copiar_array_punteros(origen, destino, tamaño_copia);
    
    printf("Array copiado: ");
    for (size_t i = 0; i < tamaño_copia; i++) {
        printf("%d ", destino[i]);
    }
    printf("\n\n");
    
    // Suma de diferentes tipos
    demo_suma_tipos();
    
    // Casos especiales
    printf("🔹 CASOS ESPECIALES:\n\n");
    
    // Array de un elemento
    int un_elemento[] = {42};
    printf("Array de un elemento: suma = %d\n", 
           suma_array_punteros(un_elemento, 1));
    
    // Array vacío (simulado)
    printf("Array vacío: suma = %d\n", 
           suma_array_punteros(NULL, 0));
    
    // Array con valores negativos
    int negativos[] = {-1, -2, -3, -4, -5};
    size_t tamaño_neg = sizeof(negativos) / sizeof(negativos[0]);
    printf("Array con negativos: suma = %d\n", 
           suma_array_punteros(negativos, tamaño_neg));
    
    // Array grande
    int grande[1000];
    for (int i = 0; i < 1000; i++) {
        grande[i] = i + 1;  // 1, 2, 3, ..., 1000
    }
    printf("Array grande (1-1000): suma = %d\n", 
           suma_array_punteros(grande, 1000));
    printf("(Fórmula: n*(n+1)/2 = 1000*1001/2 = %d)\n\n", 1000 * 1001 / 2);
    
    // Demostración final
    printf("🔹 CONCEPTOS CLAVE DEMOSTRADOS:\n");
    printf("✓ array[i] ≡ *(array + i)\n");
    printf("✓ Aritmética de punteros respeta el tamaño del tipo\n");
    printf("✓ ptr++ avanza sizeof(tipo) bytes\n");
    printf("✓ Comparación de punteros (ptr < end)\n");
    printf("✓ Punteros como iteradores\n");
    printf("✓ Recorrido bidireccional\n");
    printf("✓ Funciones que operan con punteros\n");

    return 0;
}
#endif
