/*
 * Ejercicio 072: Algoritmo de Búsqueda Binaria
 * 
 * Implementación completa de búsqueda binaria y sus variantes
 * para arrays ordenados y casos especiales.
 */

#include "../include/busqueda_binaria.h"

/* ================================
 * BÚSQUEDA BINARIA BÁSICA
 * ================================ */

int busqueda_binaria_recursiva(int arr[], int izquierda, int derecha, int objetivo) {
    if (izquierda > derecha) {
        return -1; // Elemento no encontrado
    }
    
    int medio = izquierda + (derecha - izquierda) / 2;
    
    if (arr[medio] == objetivo) {
        return medio;
    }
    
    if (arr[medio] > objetivo) {
        return busqueda_binaria_recursiva(arr, izquierda, medio - 1, objetivo);
    } else {
        return busqueda_binaria_recursiva(arr, medio + 1, derecha, objetivo);
    }
}

int busqueda_binaria_iterativa(int arr[], int tamanio, int objetivo) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] == objetivo) {
            return medio;
        }
        
        if (arr[medio] < objetivo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return -1; // Elemento no encontrado
}

ResultadoBusqueda busqueda_binaria_detallada(int arr[], int tamanio, int objetivo) {
    ResultadoBusqueda resultado = {-1, 0, false};
    
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda <= derecha) {
        resultado.comparaciones++;
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] == objetivo) {
            resultado.posicion = medio;
            resultado.encontrado = true;
            return resultado;
        }
        
        if (arr[medio] < objetivo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return resultado;
}

/* ================================
 * VARIANTES DE BÚSQUEDA BINARIA
 * ================================ */

int busqueda_primera_ocurrencia(int arr[], int tamanio, int objetivo) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    int resultado = -1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] == objetivo) {
            resultado = medio;
            derecha = medio - 1; // Continuar buscando hacia la izquierda
        } else if (arr[medio] < objetivo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return resultado;
}

int busqueda_ultima_ocurrencia(int arr[], int tamanio, int objetivo) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    int resultado = -1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] == objetivo) {
            resultado = medio;
            izquierda = medio + 1; // Continuar buscando hacia la derecha
        } else if (arr[medio] < objetivo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return resultado;
}

ResultadoBusquedaMultiple busqueda_todas_ocurrencias(int arr[], int tamanio, int objetivo) {
    ResultadoBusquedaMultiple resultado = {-1, -1, 0, 0};
    
    resultado.primera_posicion = busqueda_primera_ocurrencia(arr, tamanio, objetivo);
    resultado.comparaciones += (int)(log2(tamanio) + 1); // Estimación
    
    if (resultado.primera_posicion != -1) {
        resultado.ultima_posicion = busqueda_ultima_ocurrencia(arr, tamanio, objetivo);
        resultado.comparaciones += (int)(log2(tamanio) + 1); // Estimación
        
        resultado.total_ocurrencias = resultado.ultima_posicion - resultado.primera_posicion + 1;
    }
    
    return resultado;
}

int busqueda_ceiling(int arr[], int tamanio, int objetivo) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    int resultado = -1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] >= objetivo) {
            resultado = medio;
            derecha = medio - 1; // Buscar un ceiling menor
        } else {
            izquierda = medio + 1;
        }
    }
    
    return resultado;
}

int busqueda_floor(int arr[], int tamanio, int objetivo) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    int resultado = -1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] <= objetivo) {
            resultado = medio;
            izquierda = medio + 1; // Buscar un floor mayor
        } else {
            derecha = medio - 1;
        }
    }
    
    return resultado;
}

/* ================================
 * BÚSQUEDA EN ARRAYS ESPECIALES
 * ================================ */

int busqueda_array_rotado(int arr[], int tamanio, int objetivo) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] == objetivo) {
            return medio;
        }
        
        // Determinar qué lado está ordenado
        if (arr[izquierda] <= arr[medio]) {
            // Lado izquierdo está ordenado
            if (objetivo >= arr[izquierda] && objetivo < arr[medio]) {
                derecha = medio - 1;
            } else {
                izquierda = medio + 1;
            }
        } else {
            // Lado derecho está ordenado
            if (objetivo > arr[medio] && objetivo <= arr[derecha]) {
                izquierda = medio + 1;
            } else {
                derecha = medio - 1;
            }
        }
    }
    
    return -1;
}

int encontrar_punto_rotacion(int arr[], int tamanio) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (arr[medio] > arr[derecha]) {
            izquierda = medio + 1;
        } else {
            derecha = medio;
        }
    }
    
    return izquierda;
}

int busqueda_tamanio_desconocido(int arr[], int objetivo) {
    // Primero encontrar un límite superior
    int limite = 1;
    while (arr[limite] < objetivo) {
        limite *= 2;
    }
    
    // Buscar en el rango [limite/2, limite]
    return busqueda_binaria_recursiva(arr, limite / 2, limite, objetivo);
}

bool busqueda_matriz_ordenada(int** matriz, int filas, int columnas, int objetivo, 
                             int* fila_encontrada, int* col_encontrada) {
    if (!matriz || filas <= 0 || columnas <= 0) {
        return false;
    }
    
    int fila = 0;
    int col = columnas - 1;
    
    while (fila < filas && col >= 0) {
        if (matriz[fila][col] == objetivo) {
            *fila_encontrada = fila;
            *col_encontrada = col;
            return true;
        } else if (matriz[fila][col] > objetivo) {
            col--;
        } else {
            fila++;
        }
    }
    
    return false;
}

/* ================================
 * BÚSQUEDA DE ELEMENTOS ESPECIALES
 * ================================ */

int encontrar_pico(int arr[], int tamanio) {
    if (tamanio == 1) return 0;
    
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        // Verificar si medio es un pico
        bool es_pico = true;
        
        if (medio > 0 && arr[medio] <= arr[medio - 1]) {
            es_pico = false;
        }
        if (medio < tamanio - 1 && arr[medio] <= arr[medio + 1]) {
            es_pico = false;
        }
        
        if (es_pico) {
            return medio;
        }
        
        // Moverse hacia el lado con mayor valor
        if (medio > 0 && arr[medio - 1] > arr[medio]) {
            derecha = medio - 1;
        } else {
            izquierda = medio + 1;
        }
    }
    
    return -1;
}

int encontrar_elemento_unico(int arr[], int tamanio) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        // Asegurar que medio sea par
        if (medio % 2 == 1) {
            medio--;
        }
        
        if (arr[medio] == arr[medio + 1]) {
            izquierda = medio + 2;
        } else {
            derecha = medio;
        }
    }
    
    return arr[izquierda];
}

int raiz_cuadrada_binaria(int numero) {
    if (numero < 0) return -1;
    if (numero == 0 || numero == 1) return numero;
    
    int izquierda = 1;
    int derecha = numero / 2;
    int resultado = 0;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        long long cuadrado = (long long)medio * medio;
        
        if (cuadrado == numero) {
            return medio;
        } else if (cuadrado < numero) {
            resultado = medio;
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return resultado;
}

int raiz_k_binaria(int numero, int k) {
    if (numero < 0 && k % 2 == 0) return -1; // Raíz par de número negativo
    if (numero == 0 || numero == 1 || k == 1) return numero;
    
    int izquierda = 1;
    int derecha = numero;
    int resultado = 0;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        // Calcular medio^k
        long long potencia = 1;
        for (int i = 0; i < k; i++) {
            potencia *= medio;
            if (potencia > numero) break;
        }
        
        if (potencia == numero) {
            return medio;
        } else if (potencia < numero) {
            resultado = medio;
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return resultado;
}

/* ================================
 * UTILIDADES Y ANÁLISIS
 * ================================ */

bool esta_ordenado_busqueda(int arr[], int tamanio) {
    for (int i = 1; i < tamanio; i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

void generar_array_ordenado(int arr[], int tamanio, int inicio, int incremento) {
    for (int i = 0; i < tamanio; i++) {
        arr[i] = inicio + i * incremento;
    }
}

void generar_array_con_duplicados(int arr[], int tamanio, int num_valores_unicos) {
    int valor_actual = 1;
    int repeticiones = tamanio / num_valores_unicos;
    int index = 0;
    
    for (int i = 0; i < num_valores_unicos && index < tamanio; i++) {
        for (int j = 0; j < repeticiones && index < tamanio; j++) {
            arr[index++] = valor_actual;
        }
        valor_actual += 2;
    }
    
    // Llenar elementos restantes
    while (index < tamanio) {
        arr[index++] = valor_actual;
    }
}

void rotar_array(int arr[], int tamanio, int posicion_rotacion) {
    if (posicion_rotacion <= 0 || posicion_rotacion >= tamanio) return;
    
    // Crear array temporal
    int* temp = malloc(sizeof(int) * tamanio);
    if (!temp) return;
    
    // Copiar elementos rotados
    for (int i = 0; i < tamanio; i++) {
        temp[i] = arr[(i + posicion_rotacion) % tamanio];
    }
    
    // Copiar de vuelta al array original
    for (int i = 0; i < tamanio; i++) {
        arr[i] = temp[i];
    }
    
    free(temp);
}

void imprimir_resultado_busqueda(ResultadoBusqueda resultado, int objetivo) {
    printf("Búsqueda del elemento %d:\n", objetivo);
    if (resultado.encontrado) {
        printf("  ✓ Encontrado en posición %d\n", resultado.posicion);
    } else {
        printf("  ✗ No encontrado\n");
    }
    printf("  Comparaciones realizadas: %d\n", resultado.comparaciones);
}

void imprimir_resultado_busqueda_multiple(ResultadoBusquedaMultiple resultado, int objetivo) {
    printf("Búsqueda múltiple del elemento %d:\n", objetivo);
    if (resultado.primera_posicion != -1) {
        printf("  ✓ Primera ocurrencia: posición %d\n", resultado.primera_posicion);
        printf("  ✓ Última ocurrencia: posición %d\n", resultado.ultima_posicion);
        printf("  ✓ Total de ocurrencias: %d\n", resultado.total_ocurrencias);
    } else {
        printf("  ✗ No encontrado\n");
    }
    printf("  Comparaciones realizadas: %d\n", resultado.comparaciones);
}

void imprimir_array_busqueda(int arr[], int tamanio, const char* titulo) {
    printf("%s: ", titulo);
    
    if (tamanio <= 20) {
        printf("[");
        for (int i = 0; i < tamanio; i++) {
            printf("%d", arr[i]);
            if (i < tamanio - 1) printf(", ");
        }
        printf("]\n");
    } else {
        printf("[%d, %d, %d, ..., %d, %d, %d] (tamaño: %d)\n",
               arr[0], arr[1], arr[2],
               arr[tamanio-3], arr[tamanio-2], arr[tamanio-1],
               tamanio);
    }
}

/* ================================
 * COMPARACIÓN Y ANÁLISIS
 * ================================ */

void comparar_busqueda_binaria_vs_lineal(int arr[], int tamanio, int objetivo) {
    printf("\n=== COMPARACIÓN: BÚSQUEDA BINARIA VS LINEAL ===\n");
    printf("Array de tamaño: %d\n", tamanio);
    printf("Elemento a buscar: %d\n\n", objetivo);
    
    // Búsqueda binaria
    clock_t inicio = clock();
    ResultadoBusqueda resultado_binaria = busqueda_binaria_detallada(arr, tamanio, objetivo);
    clock_t fin = clock();
    double tiempo_binaria = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    printf("BÚSQUEDA BINARIA:\n");
    printf("  Resultado: %s\n", resultado_binaria.encontrado ? "Encontrado" : "No encontrado");
    if (resultado_binaria.encontrado) {
        printf("  Posición: %d\n", resultado_binaria.posicion);
    }
    printf("  Comparaciones: %d\n", resultado_binaria.comparaciones);
    printf("  Tiempo: %.8f segundos\n", tiempo_binaria);
    
    // Búsqueda lineal
    inicio = clock();
    ResultadoBusqueda resultado_lineal = busqueda_lineal_detallada(arr, tamanio, objetivo);
    fin = clock();
    double tiempo_lineal = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    printf("\nBÚSQUEDA LINEAL:\n");
    printf("  Resultado: %s\n", resultado_lineal.encontrado ? "Encontrado" : "No encontrado");
    if (resultado_lineal.encontrado) {
        printf("  Posición: %d\n", resultado_lineal.posicion);
    }
    printf("  Comparaciones: %d\n", resultado_lineal.comparaciones);
    printf("  Tiempo: %.8f segundos\n", tiempo_lineal);
    
    // Comparación
    printf("\nCOMPARACIÓN:\n");
    printf("  Mejora en comparaciones: %.2fx\n", 
           (double)resultado_lineal.comparaciones / resultado_binaria.comparaciones);
    if (tiempo_lineal > 0) {
        printf("  Mejora en tiempo: %.2fx\n", tiempo_lineal / tiempo_binaria);
    }
    printf("  Complejidad teórica: O(log n) vs O(n)\n");
}

ResultadoBusqueda busqueda_lineal_detallada(int arr[], int tamanio, int objetivo) {
    ResultadoBusqueda resultado = {-1, 0, false};
    
    for (int i = 0; i < tamanio; i++) {
        resultado.comparaciones++;
        if (arr[i] == objetivo) {
            resultado.posicion = i;
            resultado.encontrado = true;
            break;
        }
    }
    
    return resultado;
}

void medir_rendimiento_busquedas(int arr[], int tamanio, int objetivo) {
    printf("\n=== ANÁLISIS DE RENDIMIENTO ===\n");
    
    const int NUM_REPETICIONES = 10000;
    
    // Medir búsqueda binaria iterativa
    clock_t inicio = clock();
    for (int i = 0; i < NUM_REPETICIONES; i++) {
        busqueda_binaria_iterativa(arr, tamanio, objetivo);
    }
    clock_t fin = clock();
    double tiempo_iterativa = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    // Medir búsqueda binaria recursiva
    inicio = clock();
    for (int i = 0; i < NUM_REPETICIONES; i++) {
        busqueda_binaria_recursiva(arr, 0, tamanio - 1, objetivo);
    }
    fin = clock();
    double tiempo_recursiva = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    
    printf("Rendimiento (%d repeticiones):\n", NUM_REPETICIONES);
    printf("  Búsqueda binaria iterativa: %.6f segundos\n", tiempo_iterativa);
    printf("  Búsqueda binaria recursiva: %.6f segundos\n", tiempo_recursiva);
    printf("  Diferencia: %.2fx\n", tiempo_recursiva / tiempo_iterativa);
}

/* ================================
 * DEMOSTRACIÓN EDUCATIVA
 * ================================ */

void demo_busqueda_binaria_paso_a_paso(int arr[], int tamanio, int objetivo) {
    printf("\n=== DEMOSTRACIÓN PASO A PASO ===\n");
    imprimir_array_busqueda(arr, tamanio, "Array ordenado");
    printf("Buscando elemento: %d\n\n", objetivo);
    
    int paso = 0;
    int resultado = busqueda_binaria_visualizada(arr, 0, tamanio - 1, objetivo, &paso);
    
    if (resultado != -1) {
        printf("\n✓ Elemento %d encontrado en posición %d\n", objetivo, resultado);
    } else {
        printf("\n✗ Elemento %d no encontrado\n", objetivo);
    }
    printf("Total de pasos: %d\n", paso);
}

int busqueda_binaria_visualizada(int arr[], int izquierda, int derecha, int objetivo, int* paso) {
    if (izquierda > derecha) {
        return -1;
    }
    
    (*paso)++;
    int medio = izquierda + (derecha - izquierda) / 2;
    
    mostrar_paso_busqueda(arr, izquierda, derecha, medio, objetivo, *paso);
    
    if (arr[medio] == objetivo) {
        return medio;
    }
    
    if (arr[medio] > objetivo) {
        printf("  → %d > %d, buscar en mitad izquierda\n", arr[medio], objetivo);
        return busqueda_binaria_visualizada(arr, izquierda, medio - 1, objetivo, paso);
    } else {
        printf("  → %d < %d, buscar en mitad derecha\n", arr[medio], objetivo);
        return busqueda_binaria_visualizada(arr, medio + 1, derecha, objetivo, paso);
    }
}

void mostrar_paso_busqueda(int arr[], int izquierda, int derecha, int medio, int objetivo, int paso) {
    printf("Paso %d:\n", paso);
    printf("  Rango: [%d, %d] (índices %d-%d)\n", 
           arr[izquierda], arr[derecha], izquierda, derecha);
    printf("  Medio: arr[%d] = %d\n", medio, arr[medio]);
    printf("  Comparando: %d vs %d\n", arr[medio], objetivo);
}

/* ================================
 * APLICACIONES PRÁCTICAS
 * ================================ */

int buscar_en_diccionario(char** palabras, int tamanio, const char* palabra_buscar) {
    int izquierda = 0;
    int derecha = tamanio - 1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        int comparacion = strcmp(palabras[medio], palabra_buscar);
        
        if (comparacion == 0) {
            return medio;
        } else if (comparacion < 0) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    
    return -1;
}

int encontrar_valor_mas_cercano(int arr[], int tamanio, int objetivo) {
    if (tamanio == 0) return -1;
    
    int posicion = busqueda_binaria_iterativa(arr, tamanio, objetivo);
    if (posicion != -1) {
        return posicion; // Encontrado exactamente
    }
    
    // Buscar el ceiling
    int ceiling = busqueda_ceiling(arr, tamanio, objetivo);
    if (ceiling == -1) {
        return tamanio - 1; // Todos los elementos son menores
    }
    
    if (ceiling == 0) {
        return 0; // Todos los elementos son mayores
    }
    
    // Comparar con el elemento anterior
    int floor_pos = ceiling - 1;
    
    if (abs(arr[ceiling] - objetivo) <= abs(arr[floor_pos] - objetivo)) {
        return ceiling;
    } else {
        return floor_pos;
    }
}

int busqueda_en_rango(int arr[], int tamanio, int min, int max, int* inicio_rango, int* fin_rango) {
    *inicio_rango = busqueda_ceiling(arr, tamanio, min);
    *fin_rango = busqueda_floor(arr, tamanio, max);
    
    if (*inicio_rango == -1 || *fin_rango == -1 || *inicio_rango > *fin_rango) {
        *inicio_rango = -1;
        *fin_rango = -1;
        return 0;
    }
    
    return *fin_rango - *inicio_rango + 1;
}

/* ================================
 * FUNCIONES SIMPLES (VERSIÓN BÁSICA)
 * ================================ */

/**
 * @brief Búsqueda binaria simple iterativa (versión del ejercicio básico)
 * @param arr Array ordenado a buscar
 * @param n Tamaño del array
 * @param clave Elemento a buscar
 * @return Índice del elemento o -1 si no se encuentra
 */
int busquedaBinaria(int arr[], int n, int clave) {
    int inicio = 0;
    int fin = n - 1;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;

        if (arr[medio] == clave)
            return medio;
        else if (arr[medio] < clave)
            inicio = medio + 1;
        else
            fin = medio - 1;
    }
    return -1;
}

/**
 * @brief Búsqueda binaria simple recursiva
 * @param arr Array ordenado a buscar
 * @param inicio Índice inicial
 * @param fin Índice final
 * @param clave Elemento a buscar
 * @return Índice del elemento o -1 si no se encuentra
 */
int busquedaBinariaRecursiva_simple(int arr[], int inicio, int fin, int clave) {
    if (inicio > fin)
        return -1;
    
    int medio = inicio + (fin - inicio) / 2;
    
    if (arr[medio] == clave)
        return medio;
    else if (arr[medio] < clave)
        return busquedaBinariaRecursiva_simple(arr, medio + 1, fin, clave);
    else
        return busquedaBinariaRecursiva_simple(arr, inicio, medio - 1, clave);
}

/**
 * @brief Función simple para imprimir array
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
 * @brief Ejemplo básico del ejercicio 072
 */
void ejemplo_basico_072(void) {
    printf("\n=== EJEMPLO BÁSICO EJERCICIO 072 ===\n");
    
    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    int n = sizeof(arr) / sizeof(arr[0]);
    int clave = 7;

    printf("Array:\n");
    imprimirArray_simple(arr, n);

    int indice = busquedaBinaria(arr, n, clave);
    if (indice != -1)
        printf("Elemento %d encontrado en la posición %d.\n", clave, indice);
    else
        printf("Elemento %d no encontrado.\n", clave);
    
    // Probar con elemento que no existe
    clave = 4;
    indice = busquedaBinaria(arr, n, clave);
    if (indice != -1)
        printf("Elemento %d encontrado en la posición %d.\n", clave, indice);
    else
        printf("Elemento %d no encontrado.\n", clave);
    
    // Probar versión recursiva
    printf("\nPrueba con versión recursiva:\n");
    clave = 11;
    indice = busquedaBinariaRecursiva_simple(arr, 0, n - 1, clave);
    if (indice != -1)
        printf("Elemento %d encontrado en la posición %d.\n", clave, indice);
    else
        printf("Elemento %d no encontrado.\n", clave);
}

/* ================================
 * FUNCIÓN PRINCIPAL DE DEMOSTRACIÓN
 * ================================ */

void demo_completa_busqueda_binaria(void) {
    printf("=== DEMOSTRACIÓN COMPLETA DE BÚSQUEDA BINARIA ===\n");
    
    // 0. Ejemplo básico del ejercicio
    ejemplo_basico_072();
    
    // 1. Ejemplo básico avanzado
    printf("\n1. BÚSQUEDA BINARIA AVANZADA\n");
    int arr_basico[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 67, 78};
    int tamanio_basico = sizeof(arr_basico) / sizeof(arr_basico[0]);
    
    imprimir_array_busqueda(arr_basico, tamanio_basico, "Array ordenado");
    
    int objetivo = 23;
    ResultadoBusqueda resultado = busqueda_binaria_detallada(arr_basico, tamanio_basico, objetivo);
    imprimir_resultado_busqueda(resultado, objetivo);
    
    // 2. Demostración paso a paso
    printf("\n2. DEMOSTRACIÓN PASO A PASO\n");
    demo_busqueda_binaria_paso_a_paso(arr_basico, tamanio_basico, 38);
    
    // 3. Búsqueda de múltiples ocurrencias
    printf("\n3. BÚSQUEDA CON DUPLICADOS\n");
    int arr_duplicados[] = {1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5, 6};
    int tamanio_duplicados = sizeof(arr_duplicados) / sizeof(arr_duplicados[0]);
    
    imprimir_array_busqueda(arr_duplicados, tamanio_duplicados, "Array con duplicados");
    
    ResultadoBusquedaMultiple resultado_multiple = busqueda_todas_ocurrencias(arr_duplicados, tamanio_duplicados, 5);
    imprimir_resultado_busqueda_multiple(resultado_multiple, 5);
    
    // 4. Búsqueda ceiling y floor
    printf("\n4. BÚSQUEDA CEILING Y FLOOR\n");
    int objetivo_cf = 15;
    int ceiling = busqueda_ceiling(arr_basico, tamanio_basico, objetivo_cf);
    int floor = busqueda_floor(arr_basico, tamanio_basico, objetivo_cf);
    
    printf("Para objetivo %d:\n", objetivo_cf);
    if (ceiling != -1) {
        printf("  Ceiling: arr[%d] = %d\n", ceiling, arr_basico[ceiling]);
    } else {
        printf("  Ceiling: No existe\n");
    }
    if (floor != -1) {
        printf("  Floor: arr[%d] = %d\n", floor, arr_basico[floor]);
    } else {
        printf("  Floor: No existe\n");
    }
    
    // 5. Array rotado
    printf("\n5. BÚSQUEDA EN ARRAY ROTADO\n");
    int arr_rotado[] = {4, 5, 6, 7, 0, 1, 2};
    int tamanio_rotado = sizeof(arr_rotado) / sizeof(arr_rotado[0]);
    
    imprimir_array_busqueda(arr_rotado, tamanio_rotado, "Array rotado");
    
    int objetivo_rotado = 0;
    int pos_rotado = busqueda_array_rotado(arr_rotado, tamanio_rotado, objetivo_rotado);
    printf("Elemento %d encontrado en posición: %d\n", objetivo_rotado, pos_rotado);
    
    int punto_rotacion = encontrar_punto_rotacion(arr_rotado, tamanio_rotado);
    printf("Punto de rotación (mínimo): índice %d, valor %d\n", 
           punto_rotacion, arr_rotado[punto_rotacion]);
    
    // 6. Búsqueda de elementos especiales
    printf("\n6. BÚSQUEDA DE ELEMENTOS ESPECIALES\n");
    
    // Pico
    int arr_pico[] = {1, 3, 20, 4, 1, 0};
    int tamanio_pico = sizeof(arr_pico) / sizeof(arr_pico[0]);
    int pico = encontrar_pico(arr_pico, tamanio_pico);
    printf("Pico encontrado en posición %d, valor: %d\n", pico, arr_pico[pico]);
    
    // Elemento único
    int arr_unico[] = {1, 1, 2, 3, 3, 4, 4, 8, 8};
    int tamanio_unico = sizeof(arr_unico) / sizeof(arr_unico[0]);
    int unico = encontrar_elemento_unico(arr_unico, tamanio_unico);
    printf("Elemento único: %d\n", unico);
    
    // Raíz cuadrada
    int numero = 25;
    int raiz = raiz_cuadrada_binaria(numero);
    printf("Raíz cuadrada de %d: %d\n", numero, raiz);
    
    // 7. Comparación de rendimiento
    printf("\n7. COMPARACIÓN DE RENDIMIENTO\n");
    const int tamanio_grande = 100000;
    int* arr_grande = malloc(sizeof(int) * tamanio_grande);
    if (arr_grande) {
        generar_array_ordenado(arr_grande, tamanio_grande, 1, 2);
        int objetivo_grande = arr_grande[tamanio_grande / 2];
        
        comparar_busqueda_binaria_vs_lineal(arr_grande, tamanio_grande, objetivo_grande);
        medir_rendimiento_busquedas(arr_grande, tamanio_grande, objetivo_grande);
        
        free(arr_grande);
    }
    
    // 8. Aplicaciones prácticas
    printf("\n8. APLICACIONES PRÁCTICAS\n");
    
    // Valor más cercano
    int objetivo_cercano = 14;
    int pos_cercano = encontrar_valor_mas_cercano(arr_basico, tamanio_basico, objetivo_cercano);
    printf("Valor más cercano a %d: arr[%d] = %d\n", 
           objetivo_cercano, pos_cercano, arr_basico[pos_cercano]);
    
    // Búsqueda en rango
    int min_rango = 10, max_rango = 50;
    int inicio_rango, fin_rango;
    int elementos_en_rango = busqueda_en_rango(arr_basico, tamanio_basico, 
                                              min_rango, max_rango, 
                                              &inicio_rango, &fin_rango);
    printf("Elementos en rango [%d, %d]: %d elementos (índices %d-%d)\n",
           min_rango, max_rango, elementos_en_rango, inicio_rango, fin_rango);
    
    printf("\n=== FIN DE LA DEMOSTRACIÓN ===\n");
}

/* ================================
 * FUNCIÓN MAIN PARA PRUEBAS
 * ================================ */

#ifndef EXCLUDE_MAIN
int main(void) {
    demo_completa_busqueda_binaria();
    return 0;
}
#endif
