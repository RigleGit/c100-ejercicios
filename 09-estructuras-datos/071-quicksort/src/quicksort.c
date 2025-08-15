/*
 * Ejercicio 071: Algoritmo Quicksort
 * 
 * Implementación completa del algoritmo Quicksort con múltiples variantes,
 * optimizaciones y análisis de rendimiento.
 */

#include "../include/quicksort.h"

/* ================================
 * FUNCIONES BÁSICAS
 * ================================ */

void quicksort_basico(int arr[], int bajo, int alto) {
    if (bajo < alto) {
        // Particionar el array y obtener la posición del pivot
        int pi = particion_basica(arr, bajo, alto);
        
        // Ordenar recursivamente los elementos antes y después del pivot
        quicksort_basico(arr, bajo, pi - 1);
        quicksort_basico(arr, pi + 1, alto);
    }
}

int particion_basica(int arr[], int bajo, int alto) {
    // Elegir el último elemento como pivot
    int pivot = arr[alto];
    
    // Índice del elemento más pequeño
    int i = (bajo - 1);
    
    for (int j = bajo; j <= alto - 1; j++) {
        // Si el elemento actual es menor o igual al pivot
        if (arr[j] <= pivot) {
            i++;
            intercambiar(arr, i, j);
        }
    }
    
    intercambiar(arr, i + 1, alto);
    return (i + 1);
}

void intercambiar(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/* ================================
 * VARIANTES DE PARTICIÓN
 * ================================ */

int particion_primer_elemento(int arr[], int bajo, int alto) {
    // Mover el primer elemento al final para usar la partición básica
    intercambiar(arr, bajo, alto);
    return particion_basica(arr, bajo, alto);
}

int particion_elemento_medio(int arr[], int bajo, int alto) {
    // Calcular posición media
    int medio = bajo + (alto - bajo) / 2;
    
    // Mover elemento medio al final
    intercambiar(arr, medio, alto);
    return particion_basica(arr, bajo, alto);
}

int particion_mediana_tres(int arr[], int bajo, int alto) {
    int medio = bajo + (alto - bajo) / 2;
    
    // Ordenar tres elementos: bajo, medio, alto
    if (arr[medio] < arr[bajo])
        intercambiar(arr, bajo, medio);
    if (arr[alto] < arr[bajo])
        intercambiar(arr, bajo, alto);
    if (arr[alto] < arr[medio])
        intercambiar(arr, medio, alto);
    
    // Colocar la mediana al final
    intercambiar(arr, medio, alto);
    return particion_basica(arr, bajo, alto);
}

int particion_aleatoria(int arr[], int bajo, int alto) {
    // Generar índice aleatorio
    int aleatorio = bajo + rand() % (alto - bajo + 1);
    
    // Mover elemento aleatorio al final
    intercambiar(arr, aleatorio, alto);
    return particion_basica(arr, bajo, alto);
}

int particion_hoare(int arr[], int bajo, int alto) {
    int pivot = arr[bajo];
    int i = bajo - 1;
    int j = alto + 1;
    
    while (true) {
        // Buscar elemento mayor al pivot desde la izquierda
        do {
            i++;
        } while (arr[i] < pivot);
        
        // Buscar elemento menor al pivot desde la derecha
        do {
            j--;
        } while (arr[j] > pivot);
        
        // Si los punteros se cruzan, terminar
        if (i >= j)
            return j;
        
        intercambiar(arr, i, j);
    }
}

/* ================================
 * QUICKSORT OPTIMIZADO
 * ================================ */

void quicksort_optimizado(int arr[], int bajo, int alto, TipoPivot tipo_pivot, EstadisticasOrdenamiento* stats) {
    if (bajo < alto) {
        if (stats) stats->llamadas_recursivas++;
        
        int pi;
        
        // Seleccionar estrategia de partición según el tipo de pivot
        switch (tipo_pivot) {
            case PIVOT_PRIMERO:
                pi = particion_primer_elemento(arr, bajo, alto);
                break;
            case PIVOT_ULTIMO:
                pi = particion_basica(arr, bajo, alto);
                break;
            case PIVOT_MEDIO:
                pi = particion_elemento_medio(arr, bajo, alto);
                break;
            case PIVOT_MEDIANA_DE_TRES:
                pi = particion_mediana_tres(arr, bajo, alto);
                break;
            case PIVOT_ALEATORIO:
                pi = particion_aleatoria(arr, bajo, alto);
                break;
            default:
                pi = particion_basica(arr, bajo, alto);
        }
        
        // Llamadas recursivas
        quicksort_optimizado(arr, bajo, pi - 1, tipo_pivot, stats);
        quicksort_optimizado(arr, pi + 1, alto, tipo_pivot, stats);
    }
}

void quicksort_hibrido(int arr[], int bajo, int alto, int umbral_insertion) {
    if (bajo < alto) {
        if (alto - bajo + 1 <= umbral_insertion) {
            // Usar insertion sort para arrays pequeños
            insertion_sort(arr, bajo, alto);
        } else {
            // Usar quicksort para arrays grandes
            int pi = particion_mediana_tres(arr, bajo, alto);
            quicksort_hibrido(arr, bajo, pi - 1, umbral_insertion);
            quicksort_hibrido(arr, pi + 1, alto, umbral_insertion);
        }
    }
}

void insertion_sort(int arr[], int bajo, int alto) {
    for (int i = bajo + 1; i <= alto; i++) {
        int clave = arr[i];
        int j = i - 1;
        
        // Mover elementos mayores una posición adelante
        while (j >= bajo && arr[j] > clave) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = clave;
    }
}

/* ================================
 * QUICKSORT ITERATIVO
 * ================================ */

void quicksort_iterativo(int arr[], int tamanio) {
    // Crear pila para almacenar rangos
    Pila* pila = pila_crear(tamanio);
    if (!pila) return;
    
    // Insertar rango inicial
    pila_push(pila, 0);
    pila_push(pila, tamanio - 1);
    
    // Procesar mientras haya rangos en la pila
    while (!pila_vacia(pila)) {
        int alto = pila_pop(pila);
        int bajo = pila_pop(pila);
        
        if (bajo < alto) {
            int pi = particion_basica(arr, bajo, alto);
            
            // Insertar sub-rangos en la pila
            pila_push(pila, bajo);
            pila_push(pila, pi - 1);
            
            pila_push(pila, pi + 1);
            pila_push(pila, alto);
        }
    }
    
    pila_destruir(pila);
}

/* ================================
 * IMPLEMENTACIÓN DE PILA
 * ================================ */

Pila* pila_crear(int capacidad) {
    Pila* pila = malloc(sizeof(Pila));
    if (!pila) return NULL;
    
    pila->datos = malloc(sizeof(int) * capacidad);
    if (!pila->datos) {
        free(pila);
        return NULL;
    }
    
    pila->cima = -1;
    pila->capacidad = capacidad;
    return pila;
}

void pila_destruir(Pila* pila) {
    if (pila) {
        free(pila->datos);
        free(pila);
    }
}

bool pila_vacia(Pila* pila) {
    return pila && pila->cima == -1;
}

bool pila_llena(Pila* pila) {
    return pila && pila->cima == pila->capacidad - 1;
}

void pila_push(Pila* pila, int valor) {
    if (pila && !pila_llena(pila)) {
        pila->datos[++pila->cima] = valor;
    }
}

int pila_pop(Pila* pila) {
    if (pila && !pila_vacia(pila)) {
        return pila->datos[pila->cima--];
    }
    return -1; // Valor de error
}

/* ================================
 * QUICKSORT DE TRES VÍAS
 * ================================ */

void quicksort_tres_vias(int arr[], int bajo, int alto) {
    if (bajo >= alto) return;
    
    int menor, mayor;
    particion_tres_vias(arr, bajo, alto, &menor, &mayor);
    
    quicksort_tres_vias(arr, bajo, menor - 1);
    quicksort_tres_vias(arr, mayor + 1, alto);
}

void particion_tres_vias(int arr[], int bajo, int alto, int* menor, int* mayor) {
    int pivot = arr[bajo];
    int i = bajo;
    int lt = bajo;      // límite de elementos menores
    int gt = alto;      // límite de elementos mayores
    
    while (i <= gt) {
        if (arr[i] < pivot) {
            intercambiar(arr, lt++, i++);
        } else if (arr[i] > pivot) {
            intercambiar(arr, i, gt--);
        } else {
            i++;
        }
    }
    
    *menor = lt;
    *mayor = gt;
}

/* ================================
 * UTILIDADES Y ANÁLISIS
 * ================================ */

void estadisticas_inicializar(EstadisticasOrdenamiento* stats) {
    if (stats) {
        stats->comparaciones = 0;
        stats->intercambios = 0;
        stats->llamadas_recursivas = 0;
        stats->tiempo_inicio = clock();
        stats->tiempo_fin = 0;
    }
}

void estadisticas_imprimir(EstadisticasOrdenamiento* stats, int tamanio_array) {
    if (!stats) return;
    
    double tiempo_cpu = ((double)(stats->tiempo_fin - stats->tiempo_inicio)) / CLOCKS_PER_SEC;
    
    printf("\n=== ESTADÍSTICAS DE ORDENAMIENTO ===\n");
    printf("Tamaño del array: %d\n", tamanio_array);
    printf("Comparaciones: %d\n", stats->comparaciones);
    printf("Intercambios: %d\n", stats->intercambios);
    printf("Llamadas recursivas: %d\n", stats->llamadas_recursivas);
    printf("Tiempo de CPU: %.6f segundos\n", tiempo_cpu);
    printf("=====================================\n");
}

bool esta_ordenado(int arr[], int tamanio) {
    for (int i = 1; i < tamanio; i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

void imprimir_array(int arr[], int tamanio, const char* titulo) {
    printf("%s: ", titulo);
    
    if (tamanio <= 20) {
        // Imprimir todo el array si es pequeño
        printf("[");
        for (int i = 0; i < tamanio; i++) {
            printf("%d", arr[i]);
            if (i < tamanio - 1) printf(", ");
        }
        printf("]\n");
    } else {
        // Imprimir solo los primeros y últimos elementos si es grande
        printf("[%d, %d, %d, ..., %d, %d, %d] (tamaño: %d)\n",
               arr[0], arr[1], arr[2],
               arr[tamanio-3], arr[tamanio-2], arr[tamanio-1],
               tamanio);
    }
}

void copiar_array(int origen[], int destino[], int tamanio) {
    for (int i = 0; i < tamanio; i++) {
        destino[i] = origen[i];
    }
}

void generar_array_aleatorio(int arr[], int tamanio, int min, int max) {
    for (int i = 0; i < tamanio; i++) {
        arr[i] = min + rand() % (max - min + 1);
    }
}

void generar_array_inverso(int arr[], int tamanio) {
    for (int i = 0; i < tamanio; i++) {
        arr[i] = tamanio - i;
    }
}

void generar_array_duplicados(int arr[], int tamanio, int num_valores_unicos) {
    for (int i = 0; i < tamanio; i++) {
        arr[i] = rand() % num_valores_unicos;
    }
}

/* ================================
 * COMPARACIÓN DE ALGORITMOS
 * ================================ */

void comparar_variantes_quicksort(int arr[], int tamanio) {
    int* arr_copia = malloc(sizeof(int) * tamanio);
    if (!arr_copia) return;
    
    printf("\n=== COMPARACIÓN DE VARIANTES DE QUICKSORT ===\n");
    printf("Tamaño del array: %d\n", tamanio);
    
    // Probar cada variante
    TipoPivot tipos[] = {PIVOT_ULTIMO, PIVOT_PRIMERO, PIVOT_MEDIO, 
                         PIVOT_MEDIANA_DE_TRES, PIVOT_ALEATORIO};
    const char* nombres[] = {"Último elemento", "Primer elemento", "Elemento medio",
                           "Mediana de tres", "Aleatorio"};
    
    for (int i = 0; i < 5; i++) {
        copiar_array(arr, arr_copia, tamanio);
        
        EstadisticasOrdenamiento stats;
        estadisticas_inicializar(&stats);
        
        clock_t inicio = clock();
        quicksort_optimizado(arr_copia, 0, tamanio - 1, tipos[i], &stats);
        clock_t fin = clock();
        
        stats.tiempo_fin = fin;
        
        printf("\nVariante: %s\n", nombres[i]);
        printf("Tiempo: %.6f segundos\n", ((double)(fin - inicio)) / CLOCKS_PER_SEC);
        printf("Llamadas recursivas: %d\n", stats.llamadas_recursivas);
        printf("Ordenado correctamente: %s\n", esta_ordenado(arr_copia, tamanio) ? "Sí" : "No");
    }
    
    // Probar versión híbrida
    copiar_array(arr, arr_copia, tamanio);
    clock_t inicio = clock();
    quicksort_hibrido(arr_copia, 0, tamanio - 1, 10);
    clock_t fin = clock();
    
    printf("\nVariante: Híbrido (umbral 10)\n");
    printf("Tiempo: %.6f segundos\n", ((double)(fin - inicio)) / CLOCKS_PER_SEC);
    printf("Ordenado correctamente: %s\n", esta_ordenado(arr_copia, tamanio) ? "Sí" : "No");
    
    // Probar versión iterativa
    copiar_array(arr, arr_copia, tamanio);
    inicio = clock();
    quicksort_iterativo(arr_copia, tamanio);
    fin = clock();
    
    printf("\nVariante: Iterativo\n");
    printf("Tiempo: %.6f segundos\n", ((double)(fin - inicio)) / CLOCKS_PER_SEC);
    printf("Ordenado correctamente: %s\n", esta_ordenado(arr_copia, tamanio) ? "Sí" : "No");
    
    free(arr_copia);
}

/* ================================
 * DEMOSTRACIÓN EDUCATIVA
 * ================================ */

void demo_quicksort_paso_a_paso(int arr[], int tamanio) {
    printf("\n=== DEMOSTRACIÓN PASO A PASO DE QUICKSORT ===\n");
    
    imprimir_array(arr, tamanio, "Array inicial");
    
    printf("\nEjecutando Quicksort con visualización...\n");
    quicksort_visualizado(arr, 0, tamanio - 1, 0);
    
    imprimir_array(arr, tamanio, "Array final");
    printf("Verificación: %s\n", esta_ordenado(arr, tamanio) ? "Ordenado correctamente" : "Error en ordenamiento");
}

void quicksort_visualizado(int arr[], int bajo, int alto, int nivel) {
    if (bajo < alto) {
        imprimir_indentacion(nivel);
        printf("Ordenando rango [%d, %d]: ", bajo, alto);
        
        // Mostrar sub-array actual
        printf("[");
        for (int i = bajo; i <= alto; i++) {
            printf("%d", arr[i]);
            if (i < alto) printf(", ");
        }
        printf("]\n");
        
        int pi = particion_basica(arr, bajo, alto);
        
        imprimir_indentacion(nivel);
        printf("Pivot: %d en posición %d\n", arr[pi], pi);
        
        quicksort_visualizado(arr, bajo, pi - 1, nivel + 1);
        quicksort_visualizado(arr, pi + 1, alto, nivel + 1);
    }
}

void imprimir_indentacion(int nivel) {
    for (int i = 0; i < nivel * 2; i++) {
        printf(" ");
    }
}

/* ================================
 * FUNCIÓN PRINCIPAL DE DEMOSTRACIÓN
 * ================================ */

void demo_completa_quicksort(void) {
    srand(time(NULL));
    
    printf("=== DEMOSTRACIÓN COMPLETA DE QUICKSORT ===\n");
    
    // 0. Ejemplo básico del usuario
    ejemplo_basico_quicksort();
    
    // 1. Ejemplo básico avanzado
    printf("\n1. EJEMPLO BÁSICO AVANZADO\n");
    int arr_basico[] = {64, 34, 25, 12, 22, 11, 90};
    int tamanio_basico = sizeof(arr_basico) / sizeof(arr_basico[0]);
    
    imprimir_array(arr_basico, tamanio_basico, "Array original");
    quicksort_basico(arr_basico, 0, tamanio_basico - 1);
    imprimir_array(arr_basico, tamanio_basico, "Array ordenado");
    
    // 2. Demostración paso a paso
    printf("\n2. DEMOSTRACIÓN PASO A PASO\n");
    int arr_demo[] = {8, 3, 5, 4, 7, 6, 1, 2};
    int tamanio_demo = sizeof(arr_demo) / sizeof(arr_demo[0]);
    demo_quicksort_paso_a_paso(arr_demo, tamanio_demo);
    
    // 3. Comparación de variantes con array aleatorio
    printf("\n3. COMPARACIÓN DE VARIANTES\n");
    const int tamanio_grande = 10000;
    int* arr_grande = malloc(sizeof(int) * tamanio_grande);
    if (arr_grande) {
        generar_array_aleatorio(arr_grande, tamanio_grande, 1, 1000);
        comparar_variantes_quicksort(arr_grande, tamanio_grande);
        free(arr_grande);
    }
    
    // 4. Prueba con diferentes tipos de datos
    printf("\n4. PRUEBAS CON DIFERENTES TIPOS DE ARRAYS\n");
    
    // Array ya ordenado
    int arr_ordenado[1000];
    for (int i = 0; i < 1000; i++) arr_ordenado[i] = i;
    clock_t inicio = clock();
    quicksort_optimizado(arr_ordenado, 0, 999, PIVOT_MEDIANA_DE_TRES, NULL);
    clock_t fin = clock();
    printf("Array ya ordenado (1000 elementos): %.6f segundos\n", 
           ((double)(fin - inicio)) / CLOCKS_PER_SEC);
    
    // Array inversamente ordenado
    generar_array_inverso(arr_ordenado, 1000);
    inicio = clock();
    quicksort_optimizado(arr_ordenado, 0, 999, PIVOT_MEDIANA_DE_TRES, NULL);
    fin = clock();
    printf("Array inversamente ordenado (1000 elementos): %.6f segundos\n", 
           ((double)(fin - inicio)) / CLOCKS_PER_SEC);
    
    // Array con muchos duplicados
    generar_array_duplicados(arr_ordenado, 1000, 10);
    inicio = clock();
    quicksort_tres_vias(arr_ordenado, 0, 999);
    fin = clock();
    printf("Array con duplicados usando 3-way quicksort (1000 elementos): %.6f segundos\n", 
           ((double)(fin - inicio)) / CLOCKS_PER_SEC);
    
    printf("\n=== FIN DE LA DEMOSTRACIÓN ===\n");
}

/* ================================
 * FUNCIÓN MAIN PARA PRUEBAS
 * ================================ */

#ifndef EXCLUDE_MAIN
int main(void) {
    demo_completa_quicksort();
    return 0;
}
#endif

/* ================================
 * IMPLEMENTACIÓN SIMPLE (VERSIÓN DEL USUARIO)
 * ================================ */

/**
 * @brief Versión simple de intercambio usando punteros
 * @param a Puntero al primer elemento
 * @param b Puntero al segundo elemento
 */
void intercambiar_simple(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Versión simple de partición
 * @param arr Array a particionar
 * @param bajo Índice inferior
 * @param alto Índice superior
 * @return Posición final del pivot
 */
int particionar_simple(int arr[], int bajo, int alto) {
    int pivote = arr[alto];
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (arr[j] <= pivote) {
            i++;
            intercambiar_simple(&arr[i], &arr[j]);
        }
    }
    intercambiar_simple(&arr[i + 1], &arr[alto]);
    return i + 1;
}

/**
 * @brief Versión simple de Quicksort
 * @param arr Array a ordenar
 * @param bajo Índice inferior
 * @param alto Índice superior
 */
void quicksort_simple(int arr[], int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionar_simple(arr, bajo, alto);
        quicksort_simple(arr, bajo, pi - 1);
        quicksort_simple(arr, pi + 1, alto);
    }
}

/**
 * @brief Imprime un array de forma simple
 * @param arr Array a imprimir
 * @param n Tamaño del array
 */
void imprimirArray_simple(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/**
 * @brief Ejemplo básico de uso de Quicksort
 */
void ejemplo_basico_quicksort(void) {
    printf("\n=== EJEMPLO BÁSICO DE QUICKSORT ===\n");
    
    int arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array original:\n");
    imprimirArray_simple(arr, n);

    quicksort_simple(arr, 0, n - 1);

    printf("Array ordenado:\n");
    imprimirArray_simple(arr, n);
    
    // Verificar que está ordenado
    bool ordenado = esta_ordenado(arr, n);
    printf("Verificación: %s\n", ordenado ? "✓ Correctamente ordenado" : "✗ Error en ordenamiento");
}
