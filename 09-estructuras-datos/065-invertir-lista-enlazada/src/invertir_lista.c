#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/invertir_lista.h"

#ifndef EXCLUDE_MAIN
int main(void) {
    printf("=== Ejercicio 065: Invertir Lista Enlazada ===\n\n");
    
    // Demostración básica según el enunciado
    printf("1. Implementación básica (según enunciado):\n");
    Nodo* lista_simple = NULL;
    
    printf("Creando lista: 1 -> 2 -> 3 -> 4\n");
    insertar_final_simple(&lista_simple, 1);
    insertar_final_simple(&lista_simple, 2);
    insertar_final_simple(&lista_simple, 3);
    insertar_final_simple(&lista_simple, 4);
    
    printf("Lista original: ");
    imprimir_lista_simple(lista_simple);
    
    printf("Invirtiendo lista...\n");
    if (invertir_lista_simple(&lista_simple)) {
        printf("Lista invertida: ");
        imprimir_lista_simple(lista_simple);
    } else {
        printf("Error al invertir la lista.\n");
    }
    
    // Demostración con estructura Lista completa
    printf("\n2. Implementación completa con estructura Lista:\n");
    Lista* lista = crear_lista();
    if (!lista) {
        printf("Error al crear la lista\n");
        return 1;
    }
    
    // Crear lista de prueba más grande
    printf("Creando lista: 10 -> 20 -> 30 -> 40 -> 50 -> 60\n");
    int valores[] = {10, 20, 30, 40, 50, 60};
    for (int i = 0; i < 6; i++) {
        insertar_final(lista, valores[i]);
    }
    imprimir_lista_detallada(lista, "Lista original");
    
    // Invertir lista completa
    printf("Invirtiendo lista completa...\n");
    if (invertir_lista(lista)) {
        imprimir_lista_detallada(lista, "Lista completamente invertida");
    }
    
    // Invertir de nuevo para restaurar
    printf("Invirtiendo de nuevo para restaurar orden original...\n");
    invertir_lista(lista);
    imprimir_lista_detallada(lista, "Lista restaurada");
    
    // Demostrar inversión de sublista
    printf("\n3. Inversión de sublista:\n");
    printf("Invirtiendo sublista desde posición 1 hasta 4 (valores 20,30,40,50)...\n");
    if (invertir_sublista(lista, 1, 4)) {
        imprimir_lista_detallada(lista, "Después de invertir sublista [1,4]");
    }
    
    // Demostrar inversión recursiva
    printf("\n4. Inversión recursiva:\n");
    Lista* lista_recursiva = crear_lista();
    printf("Creando nueva lista: 1 -> 2 -> 3 -> 4 -> 5\n");
    for (int i = 1; i <= 5; i++) {
        insertar_final(lista_recursiva, i);
    }
    imprimir_lista_detallada(lista_recursiva, "Antes de inversión recursiva");
    
    printf("Aplicando inversión recursiva...\n");
    if (invertir_lista_recursivo(lista_recursiva)) {
        imprimir_lista_detallada(lista_recursiva, "Después de inversión recursiva");
    }
    
    // Demostrar inversión por grupos
    printf("\n5. Inversión por grupos de k nodos:\n");
    Lista* lista_grupos = crear_lista();
    printf("Creando lista: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8\n");
    for (int i = 1; i <= 8; i++) {
        insertar_final(lista_grupos, i);
    }
    imprimir_lista_detallada(lista_grupos, "Lista original");
    
    printf("Invirtiendo por grupos de 3 nodos...\n");
    if (invertir_por_grupos(lista_grupos, 3)) {
        imprimir_lista_detallada(lista_grupos, "Después de invertir por grupos de 3");
    }
    
    // Demostrar creación de copia invertida
    printf("\n6. Crear copia invertida:\n");
    Lista* lista_original = crear_lista();
    printf("Creando lista original: A -> B -> C -> D (usando valores 65,66,67,68)\n");
    for (int i = 65; i <= 68; i++) {
        insertar_final(lista_original, i);
    }
    imprimir_lista_detallada(lista_original, "Lista original");
    
    Lista* lista_copia_inv = crear_copia_invertida(lista_original);
    if (lista_copia_inv) {
        imprimir_lista_detallada(lista_copia_inv, "Copia invertida");
        imprimir_lista_detallada(lista_original, "Original (sin modificar)");
    }
    
    // Verificar integridad
    printf("\n7. Validación de integridad:\n");
    printf("¿Lista original válida? %s\n", validar_integridad_lista(lista_original) ? "Sí" : "No");
    printf("¿Copia invertida válida? %s\n", validar_integridad_lista(lista_copia_inv) ? "Sí" : "No");
    
    // Demostrar casos especiales
    printf("\n8. Casos especiales:\n");
    demostrar_casos_especiales_inversion();
    
    // Visualizar proceso paso a paso
    printf("\n9. Visualización paso a paso:\n");
    Lista* lista_visual = crear_lista();
    for (int i = 1; i <= 4; i++) {
        insertar_final(lista_visual, i);
    }
    visualizar_proceso_inversion(lista_visual);
    
    // Análisis de rendimiento
    printf("\n10. Análisis de rendimiento:\n");
    analizar_rendimiento_inversion();
    
    // Cleanup
    destruir_lista_simple(&lista_simple);
    destruir_lista(lista);
    destruir_lista(lista_recursiva);
    destruir_lista(lista_grupos);
    destruir_lista(lista_original);
    destruir_lista(lista_copia_inv);
    destruir_lista(lista_visual);
    
    printf("\n=== Fin del ejercicio ===\n");
    return 0;
}
#endif // EXCLUDE_MAIN

// === IMPLEMENTACIÓN DE FUNCIONES ===

Lista* crear_lista(void) {
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    if (!lista) return NULL;
    
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->longitud = 0;
    
    return lista;
}

Nodo* crear_nodo(int valor) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return NULL;
    
    nuevo->valor = valor;
    nuevo->siguiente = NULL;
    
    return nuevo;
}

void destruir_lista(Lista* lista) {
    if (!lista) return;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        Nodo* temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    
    free(lista);
}

void destruir_nodo(Nodo* nodo) {
    if (nodo) {
        free(nodo);
    }
}

bool insertar_final(Lista* lista, int valor) {
    if (!lista) return false;
    
    Nodo* nuevo = crear_nodo(valor);
    if (!nuevo) return false;
    
    if (lista->cabeza == NULL) {
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    } else {
        lista->cola->siguiente = nuevo;
        lista->cola = nuevo;
    }
    
    lista->longitud++;
    return true;
}

bool insertar_inicio(Lista* lista, int valor) {
    if (!lista) return false;
    
    Nodo* nuevo = crear_nodo(valor);
    if (!nuevo) return false;
    
    nuevo->siguiente = lista->cabeza;
    lista->cabeza = nuevo;
    
    if (lista->cola == NULL) {
        lista->cola = nuevo;
    }
    
    lista->longitud++;
    return true;
}

// === FUNCIONES DE INVERSIÓN (PRINCIPAL) ===

bool invertir_lista(Lista* lista) {
    if (!lista || !lista->cabeza) return true; // Lista vacía o con un elemento
    
    Nodo* prev = NULL;
    Nodo* actual = lista->cabeza;
    Nodo* siguiente = NULL;
    
    // Guardar la cola original como nueva cabeza
    lista->cola = lista->cabeza;
    
    while (actual != NULL) {
        siguiente = actual->siguiente;  // Guardar siguiente nodo
        actual->siguiente = prev;       // Invertir enlace
        prev = actual;                  // Mover prev y actual
        actual = siguiente;
    }
    
    lista->cabeza = prev; // Nueva cabeza
    return true;
}

bool invertir_sublista(Lista* lista, size_t inicio, size_t fin) {
    if (!lista || inicio > fin || fin >= lista->longitud) return false;
    
    // Casos especiales
    if (inicio == fin) return true; // Un solo elemento
    if (inicio == 0 && fin == lista->longitud - 1) {
        return invertir_lista(lista); // Lista completa
    }
    
    // Encontrar nodos antes del inicio y después del fin
    Nodo* prev_inicio = NULL;
    Nodo* nodo_inicio = lista->cabeza;
    
    // Navegar hasta el nodo de inicio
    for (size_t i = 0; i < inicio; i++) {
        prev_inicio = nodo_inicio;
        nodo_inicio = nodo_inicio->siguiente;
    }
    
    // Invertir la sublista
    Nodo* prev = NULL;
    Nodo* actual = nodo_inicio;
    Nodo* siguiente = NULL;
    
    for (size_t i = 0; i <= fin - inicio; i++) {
        siguiente = actual->siguiente;
        actual->siguiente = prev;
        prev = actual;
        actual = siguiente;
    }
    
    // Reconectar la sublista invertida
    if (prev_inicio) {
        prev_inicio->siguiente = prev;
    } else {
        lista->cabeza = prev; // Se invirtió desde el inicio
    }
    
    nodo_inicio->siguiente = actual;
    
    // Actualizar cola si es necesario
    if (fin == lista->longitud - 1) {
        lista->cola = nodo_inicio;
    }
    
    return true;
}

bool invertir_lista_recursivo(Lista* lista) {
    if (!lista || !lista->cabeza) return true;
    
    // Guardar la cola original
    lista->cola = lista->cabeza;
    
    // Llamar función auxiliar recursiva
    lista->cabeza = invertir_recursivo_auxiliar(lista->cabeza);
    
    return true;
}

Nodo* invertir_recursivo_auxiliar(Nodo* nodo_actual) {
    // Caso base: nodo NULL o último nodo
    if (!nodo_actual || !nodo_actual->siguiente) {
        return nodo_actual;
    }
    
    // Recursivamente invertir el resto de la lista
    Nodo* nueva_cabeza = invertir_recursivo_auxiliar(nodo_actual->siguiente);
    
    // Invertir los enlaces del nodo actual
    nodo_actual->siguiente->siguiente = nodo_actual;
    nodo_actual->siguiente = NULL;
    
    return nueva_cabeza;
}

bool invertir_por_grupos(Lista* lista, size_t k) {
    if (!lista || k <= 1 || lista->longitud < k) return false;
    
    Nodo* actual = lista->cabeza;
    Nodo* prev_grupo = NULL;
    Nodo* nueva_cabeza = NULL;
    
    while (actual) {
        // Verificar si hay suficientes nodos para un grupo completo
        Nodo* temp = actual;
        size_t contador = 0;
        while (temp && contador < k) {
            temp = temp->siguiente;
            contador++;
        }
        
        if (contador == k) {
            // Invertir este grupo
            Nodo* inicio_grupo = actual;
            Nodo* nuevo_inicio = invertir_grupo_k_auxiliar(actual, k);
            
            if (!nueva_cabeza) {
                nueva_cabeza = nuevo_inicio;
            }
            
            if (prev_grupo) {
                prev_grupo->siguiente = nuevo_inicio;
            }
            
            inicio_grupo->siguiente = temp;
            prev_grupo = inicio_grupo;
            actual = temp;
        } else {
            // No hay suficientes nodos para otro grupo completo
            break;
        }
    }
    
    if (nueva_cabeza) {
        lista->cabeza = nueva_cabeza;
        
        // Actualizar cola
        Nodo* temp = lista->cabeza;
        while (temp->siguiente) {
            temp = temp->siguiente;
        }
        lista->cola = temp;
    }
    
    return true;
}

Nodo* invertir_grupo_k_auxiliar(Nodo* cabeza, size_t k) {
    Nodo* prev = NULL;
    Nodo* actual = cabeza;
    Nodo* siguiente = NULL;
    
    for (size_t i = 0; i < k && actual; i++) {
        siguiente = actual->siguiente;
        actual->siguiente = prev;
        prev = actual;
        actual = siguiente;
    }
    
    return prev;
}

// === FUNCIONES DE VISUALIZACIÓN ===

void imprimir_lista(const Lista* lista) {
    if (!lista || esta_vacia(lista)) {
        printf("[]");
        return;
    }
    
    printf("[");
    Nodo* actual = lista->cabeza;
    while (actual) {
        printf("%d", actual->valor);
        if (actual->siguiente) {
            printf(" -> ");
        }
        actual = actual->siguiente;
    }
    printf("]");
}

void imprimir_lista_detallada(const Lista* lista, const char* titulo) {
    if (titulo) {
        printf("%s:\n", titulo);
    }
    printf("  Lista: ");
    imprimir_lista(lista);
    printf("\n  Longitud: %zu\n", obtener_longitud(lista));
    if (!esta_vacia(lista)) {
        printf("  Cabeza: %d, Cola: %d\n", lista->cabeza->valor, lista->cola->valor);
    }
    printf("\n");
}

void imprimir_debug_lista(const Lista* lista) {
    if (!lista) {
        printf("Lista: NULL\n");
        return;
    }
    
    printf("=== DEBUG Lista ===\n");
    printf("Dirección lista: %p\n", (void*)lista);
    printf("Cabeza: %p\n", (void*)lista->cabeza);
    printf("Cola: %p\n", (void*)lista->cola);
    printf("Longitud: %zu\n", lista->longitud);
    
    if (lista->cabeza) {
        printf("Nodos:\n");
        Nodo* actual = lista->cabeza;
        size_t i = 0;
        while (actual) {
            printf("  [%zu] %p: valor=%d, siguiente=%p\n", 
                   i, (void*)actual, actual->valor, (void*)actual->siguiente);
            actual = actual->siguiente;
            i++;
        }
    }
    printf("==================\n");
}

// === FUNCIONES VERSIÓN SIMPLE ===

bool invertir_lista_simple(Nodo** cabeza) {
    if (!cabeza || !*cabeza) return true; // Lista vacía o con un elemento
    
    Nodo* prev = NULL;
    Nodo* actual = *cabeza;
    Nodo* siguiente = NULL;
    
    while (actual != NULL) {
        siguiente = actual->siguiente;  // Guardar siguiente nodo
        actual->siguiente = prev;       // Invertir enlace
        prev = actual;                  // Mover prev y actual
        actual = siguiente;
    }
    
    *cabeza = prev; // Nueva cabeza
    return true;
}

bool insertar_final_simple(Nodo** cabeza, int valor) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return false;
    
    nuevo->valor = valor;
    nuevo->siguiente = NULL;
    
    if (*cabeza == NULL) {
        *cabeza = nuevo;
    } else {
        Nodo* temp = *cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
    
    return true;
}

void imprimir_lista_simple(const Nodo* cabeza) {
    while (cabeza != NULL) {
        printf("%d -> ", cabeza->valor);
        cabeza = cabeza->siguiente;
    }
    printf("NULL\n");
}

void destruir_lista_simple(Nodo** cabeza) {
    if (!cabeza) return;
    
    while (*cabeza) {
        Nodo* temp = *cabeza;
        *cabeza = (*cabeza)->siguiente;
        free(temp);
    }
}

// === FUNCIONES DE CONSULTA Y VALIDACIÓN ===

bool esta_vacia(const Lista* lista) {
    return !lista || lista->cabeza == NULL;
}

size_t obtener_longitud(const Lista* lista) {
    return lista ? lista->longitud : 0;
}

Nodo* obtener_en_posicion(const Lista* lista, size_t posicion) {
    if (!lista || posicion >= lista->longitud) return NULL;
    
    Nodo* actual = lista->cabeza;
    for (size_t i = 0; i < posicion; i++) {
        actual = actual->siguiente;
    }
    
    return actual;
}

bool validar_integridad_lista(const Lista* lista) {
    if (!lista) return false;
    
    // Lista vacía
    if (lista->longitud == 0) {
        return lista->cabeza == NULL && lista->cola == NULL;
    }
    
    // Lista no vacía debe tener cabeza
    if (!lista->cabeza) return false;
    
    // Contar nodos manualmente
    size_t contador = 0;
    Nodo* actual = lista->cabeza;
    Nodo* ultimo = NULL;
    
    while (actual) {
        contador++;
        ultimo = actual;
        actual = actual->siguiente;
    }
    
    // Verificar longitud
    if (contador != lista->longitud) return false;
    
    // Verificar cola
    if (lista->cola != ultimo) return false;
    
    return true;
}

bool listas_iguales(const Lista* lista1, const Lista* lista2) {
    if (!lista1 || !lista2) return lista1 == lista2;
    
    if (lista1->longitud != lista2->longitud) return false;
    
    Nodo* nodo1 = lista1->cabeza;
    Nodo* nodo2 = lista2->cabeza;
    
    while (nodo1 && nodo2) {
        if (nodo1->valor != nodo2->valor) return false;
        nodo1 = nodo1->siguiente;
        nodo2 = nodo2->siguiente;
    }
    
    return nodo1 == NULL && nodo2 == NULL;
}

// === FUNCIONES EDUCATIVAS ===

void demostrar_casos_especiales_inversion(void) {
    printf("Demostrando casos especiales de inversión:\n");
    
    // Lista vacía
    Lista* lista_vacia = crear_lista();
    printf("- Invertir lista vacía: %s\n", 
           invertir_lista(lista_vacia) ? "Éxito" : "Falló");
    printf("  Lista después: ");
    imprimir_lista(lista_vacia);
    printf("\n");
    
    // Lista con un solo elemento
    Lista* lista_uno = crear_lista();
    insertar_final(lista_uno, 42);
    printf("- Lista con un elemento: ");
    imprimir_lista(lista_uno);
    printf("\n  Invirtiendo: %s\n", 
           invertir_lista(lista_uno) ? "Éxito" : "Falló");
    printf("  Lista después: ");
    imprimir_lista(lista_uno);
    printf("\n");
    
    // Lista con dos elementos
    Lista* lista_dos = crear_lista();
    insertar_final(lista_dos, 1);
    insertar_final(lista_dos, 2);
    printf("- Lista con dos elementos: ");
    imprimir_lista(lista_dos);
    printf("\n  Invirtiendo: %s\n", 
           invertir_lista(lista_dos) ? "Éxito" : "Falló");
    printf("  Lista después: ");
    imprimir_lista(lista_dos);
    printf("\n");
    
    destruir_lista(lista_vacia);
    destruir_lista(lista_uno);
    destruir_lista(lista_dos);
}

void analizar_rendimiento_inversion(void) {
    printf("Análisis de rendimiento de inversión:\n");
    printf("- invertir_lista(): O(n) - Un solo recorrido\n");
    printf("- invertir_lista_recursivo(): O(n) tiempo, O(n) espacio (call stack)\n");
    printf("- invertir_sublista(): O(n) - Recorrido hasta posición + inversión\n");
    printf("- invertir_por_grupos(): O(n) - Múltiples inversiones locales\n");
    printf("- crear_copia_invertida(): O(n) tiempo, O(n) espacio adicional\n\n");
    
    printf("Ventajas de cada método:\n");
    printf("- Iterativo: Eficiente en memoria, fácil de entender\n");
    printf("- Recursivo: Elegante, pero usa stack adicional\n");
    printf("- Por grupos: Útil para algoritmos específicos\n");
    printf("- Copia invertida: Preserva lista original\n");
}

void visualizar_proceso_inversion(const Lista* lista) {
    if (!lista || esta_vacia(lista)) {
        printf("Lista vacía - nada que visualizar\n");
        return;
    }
    
    printf("Visualización paso a paso de la inversión:\n");
    printf("Lista original: ");
    imprimir_lista(lista);
    printf("\n\n");
    
    // Crear copia para no modificar la original
    Lista* copia = crear_lista();
    Nodo* actual = lista->cabeza;
    while (actual) {
        insertar_final(copia, actual->valor);
        actual = actual->siguiente;
    }
    
    printf("Proceso de inversión:\n");
    
    Nodo* prev = NULL;
    actual = copia->cabeza;
    Nodo* siguiente = NULL;
    int paso = 1;
    
    printf("Inicial: prev=NULL, actual=%d, siguiente=?\n", 
           actual ? actual->valor : -1);
    
    while (actual != NULL) {
        siguiente = actual->siguiente;
        printf("\nPaso %d:\n", paso);
        printf("  - Guardar siguiente: %d\n", 
               siguiente ? siguiente->valor : -1);
        printf("  - actual->siguiente = prev (%s)\n", 
               prev ? "valor" : "NULL");
        actual->siguiente = prev;
        printf("  - Mover: prev=%d, actual=%d\n", 
               actual->valor, siguiente ? siguiente->valor : -1);
        prev = actual;
        actual = siguiente;
        paso++;
    }
    
    printf("\nResultado final: cabeza apunta a %d\n", prev->valor);
    copia->cabeza = prev;
    
    // Actualizar cola
    actual = copia->cabeza;
    while (actual->siguiente) {
        actual = actual->siguiente;
    }
    copia->cola = actual;
    
    printf("Lista invertida: ");
    imprimir_lista(copia);
    printf("\n");
    
    destruir_lista(copia);
}

// === FUNCIONES AVANZADAS ===

Lista* crear_copia_invertida(const Lista* lista_original) {
    if (!lista_original) return NULL;
    
    Lista* nueva_lista = crear_lista();
    if (!nueva_lista) return NULL;
    
    // Insertar cada elemento al inicio para invertir automáticamente
    Nodo* actual = lista_original->cabeza;
    while (actual) {
        if (!insertar_inicio(nueva_lista, actual->valor)) {
            destruir_lista(nueva_lista);
            return NULL;
        }
        actual = actual->siguiente;
    }
    
    return nueva_lista;
}

bool invertir_excepto_valor(Lista* lista, int mantener_valor) {
    if (!lista || esta_vacia(lista)) return true;
    
    // Estrategia: extraer elementos que no son el valor a mantener,
    // invertir esa lista, y luego reinsertarlos en sus posiciones originales
    
    // Crear lista temporal con elementos a invertir
    Lista* temp = crear_lista();
    size_t* posiciones = malloc(lista->longitud * sizeof(size_t));
    if (!temp || !posiciones) {
        destruir_lista(temp);
        free(posiciones);
        return false;
    }
    
    // Extraer elementos y recordar posiciones
    Nodo* actual = lista->cabeza;
    size_t pos = 0;
    size_t count_extraidos = 0;
    
    while (actual) {
        if (actual->valor != mantener_valor) {
            insertar_final(temp, actual->valor);
            posiciones[count_extraidos] = pos;
            count_extraidos++;
        }
        actual = actual->siguiente;
        pos++;
    }
    
    // Invertir lista temporal
    invertir_lista(temp);
    
    // Reemplazar valores en posiciones originales
    Nodo* temp_actual = temp->cabeza;
    for (size_t i = 0; i < count_extraidos; i++) {
        Nodo* nodo_destino = obtener_en_posicion(lista, posiciones[i]);
        if (nodo_destino && temp_actual) {
            nodo_destino->valor = temp_actual->valor;
            temp_actual = temp_actual->siguiente;
        }
    }
    
    destruir_lista(temp);
    free(posiciones);
    return true;
}

bool invertir_alternativo(Lista* lista, size_t k) {
    if (!lista || k <= 1) return false;
    
    size_t grupo = 0;
    size_t inicio = 0;
    
    while (inicio < lista->longitud) {
        size_t fin = inicio + k - 1;
        if (fin >= lista->longitud) {
            fin = lista->longitud - 1;
        }
        
        // Invertir solo grupos impares (1, 3, 5, ...)
        if (grupo % 2 == 1) {
            invertir_sublista(lista, inicio, fin);
        }
        
        inicio += k;
        grupo++;
    }
    
    return true;
}
