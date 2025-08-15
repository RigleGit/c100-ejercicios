#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/**
 * Función básica del enunciado: gestión básica de array dinámico
 * Implementación directa del ejercicio original
 */
void gestionBasicaArray(void) {
    printf("=== IMPLEMENTACIÓN BÁSICA DEL ENUNCIADO ===\n");
    
    int *arr;
    int n, i, suma = 0;
    double promedio;

    printf("Introduce el número de elementos: ");
    scanf("%d", &n);

    arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Error: no se pudo reservar memoria.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        printf("Introduce el elemento %d: ", i + 1);
        scanf("%d", &arr[i]);
        suma += arr[i];
    }

    promedio = (double)suma / n;

    printf("Suma: %d\n", suma);
    printf("Promedio: %.2f\n", promedio);

    free(arr);
    arr = NULL;
    
    printf("Memoria liberada correctamente\n");
    printf("============================================\n\n");
}

/**
 * Función avanzada para crear un array dinámico con verificaciones
 * @param tamaño Número de elementos del array
 * @return Puntero al array creado o NULL si hay error
 */
int* crearArray(int tamaño) {
    if (tamaño <= 0) {
        printf("Error: Tamaño inválido (%d). Debe ser mayor que 0\n", tamaño);
        return NULL;
    }
    
    if (tamaño > INT_MAX / sizeof(int)) {
        printf("Error: Tamaño demasiado grande, posible overflow\n");
        return NULL;
    }
    
    printf("=== Creando array dinámico ===\n");
    printf("Número de elementos: %d\n", tamaño);
    printf("Memoria solicitada: %zu bytes\n", tamaño * sizeof(int));
    
    int *array = (int *)malloc(tamaño * sizeof(int));
    
    if (array == NULL) {
        printf("Error: No se pudo reservar memoria para el array\n");
        printf("errno: %d (%s)\n", errno, strerror(errno));
        return NULL;
    }
    
    printf("Array creado exitosamente\n");
    printf("Dirección base: %p\n", (void*)array);
    printf("Dirección final: %p\n", (void*)(array + tamaño - 1));
    printf("===============================\n\n");
    
    return array;
}

/**
 * Función para llenar un array dinámico con valores del usuario
 * @param array Puntero al array
 * @param tamaño Número de elementos
 * @return 1 si exitoso, 0 si error
 */
int llenarArray(int *array, int tamaño) {
    if (array == NULL) {
        printf("Error: Array nulo\n");
        return 0;
    }
    
    printf("=== Llenando array ===\n");
    printf("Introduce %d valores:\n", tamaño);
    
    for (int i = 0; i < tamaño; i++) {
        printf("Elemento %d: ", i + 1);
        
        if (scanf("%d", &array[i]) != 1) {
            printf("Error: Entrada inválida\n");
            // Limpiar buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return 0;
        }
        
        printf("  array[%d] = %d (dirección: %p)\n", 
               i, array[i], (void*)&array[i]);
    }
    
    printf("Array llenado exitosamente\n");
    printf("======================\n\n");
    
    return 1;
}

/**
 * Función para llenar un array con valores automáticos (para testing)
 * @param array Puntero al array
 * @param tamaño Número de elementos
 * @param patron Patrón de llenado (0=secuencial, 1=aleatorio, 2=fibonacci)
 */
void llenarArrayAutomatico(int *array, int tamaño, int patron) {
    if (array == NULL) return;
    
    printf("=== Llenando array automáticamente ===\n");
    printf("Patrón: ");
    
    switch (patron) {
        case 0: // Secuencial
            printf("Secuencial (1, 2, 3, ...)\n");
            for (int i = 0; i < tamaño; i++) {
                array[i] = i + 1;
            }
            break;
            
        case 1: // Números pares
            printf("Números pares (2, 4, 6, ...)\n");
            for (int i = 0; i < tamaño; i++) {
                array[i] = (i + 1) * 2;
            }
            break;
            
        case 2: // Potencias de 2
            printf("Potencias de 2 (1, 2, 4, 8, ...)\n");
            for (int i = 0; i < tamaño; i++) {
                array[i] = 1 << i; // 2^i
            }
            break;
            
        case 3: // Fibonacci
            printf("Fibonacci (1, 1, 2, 3, 5, ...)\n");
            if (tamaño >= 1) array[0] = 1;
            if (tamaño >= 2) array[1] = 1;
            for (int i = 2; i < tamaño; i++) {
                array[i] = array[i-1] + array[i-2];
            }
            break;
            
        default:
            printf("Todos ceros\n");
            for (int i = 0; i < tamaño; i++) {
                array[i] = 0;
            }
    }
    
    printf("Array llenado con patrón %d\n", patron);
    printf("======================================\n\n");
}

/**
 * Función para mostrar un array dinámico
 * @param array Puntero al array
 * @param tamaño Número de elementos
 * @param nombre Nombre descriptivo del array
 */
void mostrarArray(int *array, int tamaño, const char *nombre) {
    if (array == NULL) {
        printf("=== Array %s ===\n", nombre);
        printf("Estado: PUNTERO NULO\n");
        printf("No se puede mostrar contenido\n");
        printf("========================\n\n");
        return;
    }
    
    printf("=== Array %s ===\n", nombre);
    printf("Tamaño: %d elementos\n", tamaño);
    printf("Memoria total: %zu bytes\n", tamaño * sizeof(int));
    printf("Dirección base: %p\n", (void*)array);
    
    printf("\nContenido:\n");
    printf("Índice  |  Valor  | Dirección\n");
    printf("--------|---------|----------\n");
    
    for (int i = 0; i < tamaño; i++) {
        printf("%6d  | %7d | %p\n", i, array[i], (void*)&array[i]);
    }
    
    printf("\nRepresentación lineal: [");
    for (int i = 0; i < tamaño; i++) {
        printf("%d", array[i]);
        if (i < tamaño - 1) printf(", ");
    }
    printf("]\n");
    
    printf("========================\n\n");
}

/**
 * Función para calcular estadísticas de un array
 * @param array Puntero al array
 * @param tamaño Número de elementos
 * @return Estructura con las estadísticas (simulada con prints)
 */
void calcularEstadisticas(int *array, int tamaño) {
    if (array == NULL || tamaño <= 0) {
        printf("=== Error en estadísticas ===\n");
        printf("Array nulo o tamaño inválido\n");
        printf("=============================\n\n");
        return;
    }
    
    printf("=== Calculando estadísticas ===\n");
    
    // Suma y promedio
    long long suma = 0; // Usar long long para evitar overflow
    for (int i = 0; i < tamaño; i++) {
        suma += array[i];
        printf("Suma parcial después de array[%d]: %lld\n", i, suma);
    }
    
    double promedio = (double)suma / tamaño;
    
    // Mínimo y máximo
    int minimo = array[0];
    int maximo = array[0];
    int pos_min = 0, pos_max = 0;
    
    for (int i = 1; i < tamaño; i++) {
        if (array[i] < minimo) {
            minimo = array[i];
            pos_min = i;
        }
        if (array[i] > maximo) {
            maximo = array[i];
            pos_max = i;
        }
    }
    
    // Contar valores positivos, negativos y ceros
    int positivos = 0, negativos = 0, ceros = 0;
    for (int i = 0; i < tamaño; i++) {
        if (array[i] > 0) positivos++;
        else if (array[i] < 0) negativos++;
        else ceros++;
    }
    
    printf("\nResultados:\n");
    printf("- Suma total: %lld\n", suma);
    printf("- Promedio: %.2f\n", promedio);
    printf("- Mínimo: %d (posición %d)\n", minimo, pos_min);
    printf("- Máximo: %d (posición %d)\n", maximo, pos_max);
    printf("- Rango: %d\n", maximo - minimo);
    printf("- Valores positivos: %d\n", positivos);
    printf("- Valores negativos: %d\n", negativos);
    printf("- Ceros: %d\n", ceros);
    
    printf("===============================\n\n");
}

/**
 * Función para buscar un valor en el array
 * @param array Puntero al array
 * @param tamaño Número de elementos
 * @param valor Valor a buscar
 * @return Índice del primer elemento encontrado, -1 si no se encuentra
 */
int buscarValor(int *array, int tamaño, int valor) {
    if (array == NULL) return -1;
    
    printf("=== Buscando valor %d ===\n", valor);
    
    for (int i = 0; i < tamaño; i++) {
        printf("Comparando array[%d] = %d con %d... ", i, array[i], valor);
        if (array[i] == valor) {
            printf("¡ENCONTRADO!\n");
            printf("Posición: %d\n", i);
            printf("Dirección: %p\n", (void*)&array[i]);
            printf("========================\n\n");
            return i;
        }
        printf("no coincide\n");
    }
    
    printf("Valor %d no encontrado en el array\n", valor);
    printf("========================\n\n");
    return -1;
}

/**
 * Función para modificar un elemento del array
 * @param array Puntero al array
 * @param tamaño Número de elementos
 * @param indice Índice del elemento a modificar
 * @param nuevo_valor Nuevo valor
 * @return 1 si exitoso, 0 si error
 */
int modificarElemento(int *array, int tamaño, int indice, int nuevo_valor) {
    if (array == NULL) {
        printf("Error: Array nulo\n");
        return 0;
    }
    
    if (indice < 0 || indice >= tamaño) {
        printf("Error: Índice %d fuera de rango [0, %d]\n", indice, tamaño - 1);
        return 0;
    }
    
    printf("=== Modificando elemento ===\n");
    printf("Posición: %d\n", indice);
    printf("Valor anterior: %d\n", array[indice]);
    printf("Nuevo valor: %d\n", nuevo_valor);
    printf("Dirección: %p\n", (void*)&array[indice]);
    
    array[indice] = nuevo_valor;
    
    printf("Modificación exitosa\n");
    printf("Valor actual: %d\n", array[indice]);
    printf("============================\n\n");
    
    return 1;
}

/**
 * Función para liberar un array dinámico de forma segura
 * @param array Puntero al puntero del array
 * @param tamaño Tamaño para logging
 */
void liberarArray(int **array, int tamaño) {
    if (array == NULL || *array == NULL) {
        printf("=== Liberación de array ===\n");
        printf("No hay memoria que liberar (puntero ya es NULL)\n");
        printf("===========================\n\n");
        return;
    }
    
    printf("=== Liberando array ===\n");
    printf("Dirección a liberar: %p\n", (void*)*array);
    printf("Tamaño: %d elementos\n", tamaño);
    printf("Memoria liberada: %zu bytes\n", tamaño * sizeof(int));
    
    free(*array);
    *array = NULL;
    
    printf("Array liberado exitosamente\n");
    printf("Puntero establecido a NULL\n");
    printf("=======================\n\n");
}

/**
 * Función para demostrar diferentes operaciones con arrays dinámicos
 */
void demostrarOperacionesAvanzadas(void) {
    printf("=== OPERACIONES AVANZADAS CON ARRAYS ===\n");
    
    const int TAMAÑO = 6;
    int *array = crearArray(TAMAÑO);
    if (array == NULL) {
        printf("Error creando array para demostración\n");
        return;
    }
    
    // Llenar con patrón fibonacci
    llenarArrayAutomatico(array, TAMAÑO, 3);
    mostrarArray(array, TAMAÑO, "fibonacci");
    
    // Calcular estadísticas
    calcularEstadisticas(array, TAMAÑO);
    
    // Buscar valores
    buscarValor(array, TAMAÑO, 5);  // Debería encontrarlo
    buscarValor(array, TAMAÑO, 99); // No debería encontrarlo
    
    // Modificar algunos elementos
    modificarElemento(array, TAMAÑO, 2, 100);
    modificarElemento(array, TAMAÑO, 4, 200);
    
    // Mostrar array modificado
    mostrarArray(array, TAMAÑO, "modificado");
    
    // Recalcular estadísticas
    printf("Estadísticas después de modificaciones:\n");
    calcularEstadisticas(array, TAMAÑO);
    
    // Liberar memoria
    liberarArray(&array, TAMAÑO);
    
    printf("=======================================\n\n");
}

/**
 * Función para demostrar manejo de errores
 */
void demostrarManejoErrores(void) {
    printf("=== MANEJO DE ERRORES ===\n");
    
    // 1. Tamaño inválido
    printf("1. Creando array con tamaño inválido:\n");
    int *array1 = crearArray(-5);
    if (array1 == NULL) {
        printf("✓ Error manejado correctamente\n\n");
    }
    
    // 2. Acceso fuera de límites
    printf("2. Acceso fuera de límites:\n");
    int *array2 = crearArray(3);
    if (array2 != NULL) {
        llenarArrayAutomatico(array2, 3, 0);
        
        // Intentar modificar índice inválido
        modificarElemento(array2, 3, 5, 999);  // Fuera de rango
        modificarElemento(array2, 3, -1, 999); // Índice negativo
        
        liberarArray(&array2, 3);
    }
    
    // 3. Operaciones con puntero nulo
    printf("3. Operaciones con puntero nulo:\n");
    int *array3 = NULL;
    mostrarArray(array3, 5, "nulo");
    calcularEstadisticas(array3, 5);
    buscarValor(array3, 5, 10);
    modificarElemento(array3, 5, 0, 100);
    liberarArray(&array3, 5);
    
    printf("=========================\n\n");
}

/**
 * Función para analizar el uso de memoria con diferentes tamaños
 */
void analizarUsoMemoria(void) {
    printf("=== ANÁLISIS DE USO DE MEMORIA ===\n");
    
    int tamaños[] = {10, 100, 1000, 10000, 100000};
    int num_tamaños = sizeof(tamaños) / sizeof(tamaños[0]);
    
    printf("Análisis de memoria para diferentes tamaños:\n");
    printf("Tamaño   | Memoria (bytes) | Memoria (KB) | Memoria (MB)\n");
    printf("---------|-----------------|--------------|-------------\n");
    
    for (int i = 0; i < num_tamaños; i++) {
        size_t bytes = tamaños[i] * sizeof(int);
        double kb = bytes / 1024.0;
        double mb = kb / 1024.0;
        
        printf("%8d | %14zu | %11.2f | %10.2f\n", 
               tamaños[i], bytes, kb, mb);
    }
    
    printf("\nPrueba práctica con array grande:\n");
    const int TAMAÑO_GRANDE = 1000000; // 1 millón de enteros
    
    printf("Intentando crear array de %d enteros...\n", TAMAÑO_GRANDE);
    
    int *array_grande = crearArray(TAMAÑO_GRANDE);
    if (array_grande != NULL) {
        printf("✓ Array grande creado exitosamente\n");
        printf("Memoria usada: %.2f MB\n", 
               (TAMAÑO_GRANDE * sizeof(int)) / (1024.0 * 1024.0));
        
        // Llenar con un patrón simple
        printf("Llenando array...\n");
        for (int i = 0; i < TAMAÑO_GRANDE; i++) {
            array_grande[i] = i % 1000; // Valores de 0-999
        }
        
        printf("Array llenado. Calculando suma...\n");
        long long suma = 0;
        for (int i = 0; i < TAMAÑO_GRANDE; i++) {
            suma += array_grande[i];
        }
        
        printf("Suma total: %lld\n", suma);
        printf("Promedio: %.2f\n", (double)suma / TAMAÑO_GRANDE);
        
        liberarArray(&array_grande, TAMAÑO_GRANDE);
    } else {
        printf("✗ No se pudo crear array grande\n");
    }
    
    printf("==================================\n\n");
}

/**
 * Función principal que demuestra todos los conceptos
 */
int main(void) {
    printf("=== EJERCICIO 052: ARRAY DINÁMICO ===\n\n");
    
    // 0. Implementación básica del enunciado
    // Comentada para evitar interacción durante tests automáticos
    // gestionBasicaArray();
    
    printf("NOTA: La función interactiva está comentada para permitir tests automáticos\n");
    printf("Para probar manualmente, descomenta gestionBasicaArray() en main()\n\n");
    
    // 1. Demostración avanzada con arrays automáticos
    printf("1. GESTIÓN AVANZADA DE ARRAYS DINÁMICOS\n");
    printf("---------------------------------------\n");
    
    // Crear array pequeño para demostración
    const int TAMAÑO_DEMO = 5;
    int *mi_array = crearArray(TAMAÑO_DEMO);
    if (mi_array == NULL) {
        printf("Error: No se pudo crear el array de demostración\n");
        return 1;
    }
    
    // Llenar con patrón secuencial
    llenarArrayAutomatico(mi_array, TAMAÑO_DEMO, 0);
    mostrarArray(mi_array, TAMAÑO_DEMO, "demo");
    
    // 2. Cálculo de estadísticas
    printf("2. CÁLCULO DE ESTADÍSTICAS\n");
    printf("--------------------------\n");
    calcularEstadisticas(mi_array, TAMAÑO_DEMO);
    
    // 3. Operaciones de búsqueda y modificación
    printf("3. BÚSQUEDA Y MODIFICACIÓN\n");
    printf("--------------------------\n");
    buscarValor(mi_array, TAMAÑO_DEMO, 3);
    modificarElemento(mi_array, TAMAÑO_DEMO, 2, 99);
    mostrarArray(mi_array, TAMAÑO_DEMO, "modificado");
    
    // 4. Liberar array demo
    printf("4. LIBERACIÓN DE MEMORIA\n");
    printf("------------------------\n");
    liberarArray(&mi_array, TAMAÑO_DEMO);
    
    // 5. Operaciones avanzadas
    printf("5. OPERACIONES AVANZADAS\n");
    printf("------------------------\n");
    demostrarOperacionesAvanzadas();
    
    // 6. Manejo de errores
    printf("6. MANEJO DE ERRORES\n");
    printf("--------------------\n");
    demostrarManejoErrores();
    
    // 7. Análisis de memoria
    printf("7. ANÁLISIS DE MEMORIA\n");
    printf("----------------------\n");
    analizarUsoMemoria();
    
    printf("=== FIN DEL EJERCICIO ===\n");
    
    return 0;
}
