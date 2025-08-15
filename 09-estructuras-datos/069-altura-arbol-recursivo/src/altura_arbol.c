/*
 * Ejercicio 069: Altura de Árbol Binario Recursivo
 * 
 * Implementación completa de funciones para calcular la altura
 * de un árbol binario usando recursión y métodos auxiliares.
 */

#include "../include/altura_arbol.h"

/* ================================
 * FUNCIONES BÁSICAS DEL ÁRBOL
 * ================================ */

ArbolBinario* arbol_crear(void) {
    ArbolBinario* arbol = malloc(sizeof(ArbolBinario));
    if (!arbol) {
        fprintf(stderr, "Error: No se pudo crear el árbol\n");
        return NULL;
    }
    
    arbol->raiz = NULL;
    arbol->tamanio = 0;
    return arbol;
}

void arbol_destruir(ArbolBinario* arbol) {
    if (!arbol) return;
    
    liberar_subarbol(arbol->raiz);
    free(arbol);
}

NodoArbol* nodo_crear(int valor) {
    NodoArbol* nodo = malloc(sizeof(NodoArbol));
    if (!nodo) {
        fprintf(stderr, "Error: No se pudo crear el nodo\n");
        return NULL;
    }
    
    nodo->valor = valor;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

bool arbol_insertar(ArbolBinario* arbol, int valor) {
    if (!arbol) return false;
    
    int tamanio_anterior = arbol->tamanio;
    arbol->raiz = insertar_recursivo(arbol->raiz, valor);
    
    // Verificar si se insertó realmente
    if (arbol->tamanio > tamanio_anterior) {
        return true;
    }
    
    return false;
}

NodoArbol* insertar_recursivo(NodoArbol* raiz, int valor) {
    // Caso base: posición encontrada
    if (raiz == NULL) {
        return nodo_crear(valor);
    }
    
    // Insertar en subárbol izquierdo
    if (valor < raiz->valor) {
        raiz->izq = insertar_recursivo(raiz->izq, valor);
    }
    // Insertar en subárbol derecho
    else if (valor > raiz->valor) {
        raiz->der = insertar_recursivo(raiz->der, valor);
    }
    // Valor duplicado - no insertar
    
    return raiz;
}

/* ================================
 * FUNCIONES DE ALTURA - CORE
 * ================================ */

int arbol_altura(ArbolBinario* arbol) {
    if (!arbol || !arbol->raiz) {
        return 0;
    }
    
    return altura_recursiva(arbol->raiz);
}

int altura_recursiva(NodoArbol* raiz) {
    // Caso base: árbol vacío
    if (raiz == NULL) {
        return 0;
    }
    
    // Calcular altura de subárboles recursivamente
    int altura_izq = altura_recursiva(raiz->izq);
    int altura_der = altura_recursiva(raiz->der);
    
    // Retornar la altura máxima + 1 (el nodo actual)
    return 1 + (altura_izq > altura_der ? altura_izq : altura_der);
}

int altura_iterativa(NodoArbol* raiz) {
    if (!raiz) return 0;
    
    // Usamos dos arrays para simular una cola por niveles
    NodoArbol* cola[1000];  // Tamaño fijo para simplificar
    int frente = 0, final = 0;
    int altura = 0;
    
    // Inicializar con la raíz
    cola[final++] = raiz;
    
    while (frente < final) {
        int nodos_en_nivel = final - frente;
        altura++;
        
        // Procesar todos los nodos del nivel actual
        for (int i = 0; i < nodos_en_nivel; i++) {
            NodoArbol* actual = cola[frente++];
            
            // Agregar hijos al siguiente nivel
            if (actual->izq) cola[final++] = actual->izq;
            if (actual->der) cola[final++] = actual->der;
        }
    }
    
    return altura;
}

int altura_nodo(ArbolBinario* arbol, int valor) {
    if (!arbol) return -1;
    
    NodoArbol* nodo = buscar_nodo(arbol->raiz, valor);
    if (!nodo) return -1;
    
    return altura_recursiva(nodo);
}

/* ================================
 * FUNCIONES AUXILIARES DE ALTURA
 * ================================ */

int encontrar_profundidad(NodoArbol* raiz, int valor, int profundidad) {
    if (!raiz) return -1;
    
    if (raiz->valor == valor) {
        return profundidad;
    }
    
    // Buscar en subárbol izquierdo
    int prof_izq = encontrar_profundidad(raiz->izq, valor, profundidad + 1);
    if (prof_izq != -1) return prof_izq;
    
    // Buscar en subárbol derecho
    return encontrar_profundidad(raiz->der, valor, profundidad + 1);
}

int factor_balance(NodoArbol* raiz) {
    if (!raiz) return 0;
    
    int altura_izq = raiz->izq ? altura_recursiva(raiz->izq) : 0;
    int altura_der = raiz->der ? altura_recursiva(raiz->der) : 0;
    
    return altura_izq - altura_der;
}

bool es_balanceado(NodoArbol* raiz) {
    if (!raiz) return true;
    
    // Verificar factor de balance del nodo actual
    int balance = factor_balance(raiz);
    if (balance < -1 || balance > 1) {
        return false;
    }
    
    // Verificar recursivamente subárboles
    return es_balanceado(raiz->izq) && es_balanceado(raiz->der);
}

int altura_maxima_teorica(int n) {
    // En el peor caso (árbol degenerado), la altura es n
    return n;
}

int altura_minima_teorica(int n) {
    // En el mejor caso (árbol balanceado), altura = ceil(log2(n+1))
    if (n == 0) return 0;
    
    int altura = 0;
    int nodos = 1;
    
    while (nodos <= n) {
        altura++;
        nodos *= 2;
    }
    
    return altura;
}

/* ================================
 * FUNCIONES DE ANÁLISIS
 * ================================ */

int contar_nodos_nivel(NodoArbol* raiz, int nivel) {
    if (!raiz) return 0;
    
    if (nivel == 0) {
        return 1;
    }
    
    return contar_nodos_nivel(raiz->izq, nivel - 1) + 
           contar_nodos_nivel(raiz->der, nivel - 1);
}

int nivel_mas_poblado(ArbolBinario* arbol) {
    if (!arbol || !arbol->raiz) return -1;
    
    int altura = arbol_altura(arbol);
    int max_nodos = 0;
    int nivel_max = 0;
    
    for (int i = 0; i < altura; i++) {
        int nodos_nivel = contar_nodos_nivel(arbol->raiz, i);
        if (nodos_nivel > max_nodos) {
            max_nodos = nodos_nivel;
            nivel_max = i;
        }
    }
    
    return nivel_max;
}

int contar_hojas(NodoArbol* raiz) {
    if (!raiz) return 0;
    
    // Si es hoja
    if (!raiz->izq && !raiz->der) {
        return 1;
    }
    
    return contar_hojas(raiz->izq) + contar_hojas(raiz->der);
}

int contar_nodos_internos(NodoArbol* raiz) {
    if (!raiz || (!raiz->izq && !raiz->der)) {
        return 0;  // NULL o hoja
    }
    
    return 1 + contar_nodos_internos(raiz->izq) + contar_nodos_internos(raiz->der);
}

/* ================================
 * FUNCIONES DE VISUALIZACIÓN
 * ================================ */

void imprimir_arbol_con_altura(ArbolBinario* arbol) {
    if (!arbol || !arbol->raiz) {
        printf("Árbol vacío\n");
        return;
    }
    
    printf("Estructura del árbol (valor:altura):\n");
    imprimir_recursivo(arbol->raiz, 0);
    printf("\n");
}

void imprimir_recursivo(NodoArbol* raiz, int nivel) {
    if (!raiz) return;
    
    // Imprimir subárbol derecho primero (para visualización)
    imprimir_recursivo(raiz->der, nivel + 1);
    
    // Imprimir indentación
    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }
    
    // Imprimir valor y altura del subárbol
    printf("%d:%d\n", raiz->valor, altura_recursiva(raiz));
    
    // Imprimir subárbol izquierdo
    imprimir_recursivo(raiz->izq, nivel + 1);
}

void mostrar_estadisticas_altura(ArbolBinario* arbol) {
    if (!arbol) {
        printf("Error: Árbol es NULL\n");
        return;
    }
    
    printf("\n=== ESTADÍSTICAS DE ALTURA ===\n");
    printf("Tamaño del árbol: %d nodos\n", arbol->tamanio);
    
    if (!arbol->raiz) {
        printf("Árbol vacío\n");
        printf("==============================\n");
        return;
    }
    
    int altura = arbol_altura(arbol);
    int hojas = contar_hojas(arbol->raiz);
    int nodos_internos = contar_nodos_internos(arbol->raiz);
    int altura_min_teorica = altura_minima_teorica(arbol->tamanio);
    int altura_max_teorica = altura_maxima_teorica(arbol->tamanio);
    
    printf("Altura actual: %d\n", altura);
    printf("Altura mínima teórica: %d\n", altura_min_teorica);
    printf("Altura máxima teórica: %d\n", altura_max_teorica);
    printf("Número de hojas: %d\n", hojas);
    printf("Nodos internos: %d\n", nodos_internos);
    printf("Árbol balanceado: %s\n", es_balanceado(arbol->raiz) ? "Sí" : "No");
    printf("Nivel más poblado: %d\n", nivel_mas_poblado(arbol));
    
    // Mostrar distribución por niveles
    printf("\nDistribución por niveles:\n");
    for (int i = 0; i < altura; i++) {
        int nodos = contar_nodos_nivel(arbol->raiz, i);
        printf("  Nivel %d: %d nodos\n", i, nodos);
    }
    
    printf("==============================\n");
}

void imprimir_nivel(NodoArbol* raiz, int nivel) {
    if (!raiz) return;
    
    if (nivel == 0) {
        printf("%d ", raiz->valor);
    } else {
        imprimir_nivel(raiz->izq, nivel - 1);
        imprimir_nivel(raiz->der, nivel - 1);
    }
}

void imprimir_por_niveles(ArbolBinario* arbol) {
    if (!arbol || !arbol->raiz) {
        printf("Árbol vacío\n");
        return;
    }
    
    int altura = arbol_altura(arbol);
    printf("Recorrido por niveles:\n");
    
    for (int i = 0; i < altura; i++) {
        printf("Nivel %d: ", i);
        imprimir_nivel(arbol->raiz, i);
        printf("\n");
    }
}

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

void demo_calculo_altura(ArbolBinario* arbol) {
    if (!arbol) return;
    
    printf("\n=== DEMOSTRACIÓN DE CÁLCULO DE ALTURA ===\n");
    
    int altura_rec = arbol_altura(arbol);
    int altura_iter = altura_iterativa(arbol->raiz);
    
    printf("Altura (recursiva): %d\n", altura_rec);
    printf("Altura (iterativa): %d\n", altura_iter);
    printf("Métodos coinciden: %s\n", (altura_rec == altura_iter) ? "Sí" : "No");
    
    // Mostrar altura de algunos nodos específicos
    printf("\nAltura de nodos específicos:\n");
    if (arbol->raiz) {
        printf("  Nodo raíz (%d): %d\n", arbol->raiz->valor, altura_recursiva(arbol->raiz));
        
        if (arbol->raiz->izq) {
            printf("  Hijo izquierdo (%d): %d\n", 
                   arbol->raiz->izq->valor, altura_recursiva(arbol->raiz->izq));
        }
        
        if (arbol->raiz->der) {
            printf("  Hijo derecho (%d): %d\n", 
                   arbol->raiz->der->valor, altura_recursiva(arbol->raiz->der));
        }
    }
    
    printf("==========================================\n");
}

void comparar_algoritmos_altura(ArbolBinario* arbol) {
    if (!arbol) return;
    
    printf("\n=== COMPARACIÓN DE ALGORITMOS ===\n");
    
    // Simular medición de tiempo (conceptual)
    printf("Comparando eficiencia:\n");
    printf("Método recursivo:\n");
    printf("  - Complejidad: O(n)\n");
    printf("  - Espacio: O(h) por recursión\n");
    printf("  - Elegante y fácil de entender\n");
    
    printf("\nMétodo iterativo:\n");
    printf("  - Complejidad: O(n)\n");
    printf("  - Espacio: O(w) donde w es ancho máximo\n");
    printf("  - Evita stack overflow en árboles muy profundos\n");
    
    printf("==================================\n");
}

void demo_casos_especiales(void) {
    printf("\n=== CASOS ESPECIALES DE ALTURA ===\n");
    
    // Árbol vacío
    ArbolBinario* vacio = arbol_crear();
    printf("Árbol vacío - Altura: %d\n", arbol_altura(vacio));
    
    // Árbol con un solo nodo
    ArbolBinario* uno = arbol_crear();
    arbol_insertar(uno, 42);
    printf("Un nodo - Altura: %d\n", arbol_altura(uno));
    
    // Árbol degenerado (como lista)
    ArbolBinario* degenerado = arbol_crear();
    for (int i = 1; i <= 5; i++) {
        arbol_insertar(degenerado, i);
    }
    printf("Árbol degenerado (5 nodos) - Altura: %d\n", arbol_altura(degenerado));
    
    // Árbol balanceado
    ArbolBinario* balanceado = arbol_crear();
    int valores[] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < 7; i++) {
        arbol_insertar(balanceado, valores[i]);
    }
    printf("Árbol balanceado (7 nodos) - Altura: %d\n", arbol_altura(balanceado));
    
    // Limpiar memoria
    arbol_destruir(vacio);
    arbol_destruir(uno);
    arbol_destruir(degenerado);
    arbol_destruir(balanceado);
    
    printf("==================================\n");
}

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

void liberar_subarbol(NodoArbol* raiz) {
    if (!raiz) return;
    
    liberar_subarbol(raiz->izq);
    liberar_subarbol(raiz->der);
    free(raiz);
}

NodoArbol* buscar_nodo(NodoArbol* raiz, int valor) {
    if (!raiz) return NULL;
    
    if (raiz->valor == valor) {
        return raiz;
    }
    
    if (valor < raiz->valor) {
        return buscar_nodo(raiz->izq, valor);
    } else {
        return buscar_nodo(raiz->der, valor);
    }
}

bool arbol_vacio(ArbolBinario* arbol) {
    return !arbol || !arbol->raiz;
}

int arbol_tamanio(ArbolBinario* arbol) {
    return arbol ? arbol->tamanio : 0;
}

/* ================================
 * FUNCIÓN PRINCIPAL DE DEMOSTRACIÓN
 * ================================ */

#ifndef EXCLUDE_MAIN
int main(void) {
    printf("=== EJERCICIO 069: ALTURA DE ÁRBOL BINARIO RECURSIVO ===\n");
    
    // Crear árbol y insertar valores del enunciado
    ArbolBinario* arbol = arbol_crear();
    if (!arbol) {
        fprintf(stderr, "Error al crear el árbol\n");
        return 1;
    }
    
    // Valores del enunciado
    int valores[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    int n = sizeof(valores) / sizeof(valores[0]);
    
    printf("\nInsertando valores: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", valores[i]);
        arbol_insertar(arbol, valores[i]);
        arbol->tamanio++;  // Actualizar tamaño manualmente
    }
    printf("\n");
    
    // Mostrar resultado principal
    printf("\nLa altura del árbol es: %d\n", arbol_altura(arbol));
    
    // Demostraciones adicionales
    imprimir_arbol_con_altura(arbol);
    demo_calculo_altura(arbol);
    mostrar_estadisticas_altura(arbol);
    imprimir_por_niveles(arbol);
    comparar_algoritmos_altura(arbol);
    demo_casos_especiales();
    
    // Ejemplo interactivo
    printf("\n=== EJEMPLO INTERACTIVO ===\n");
    printf("Probando con diferentes estructuras:\n");
    
}
#endif // EXCLUDE_MAIN
