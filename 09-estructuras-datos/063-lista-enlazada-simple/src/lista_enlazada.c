#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lista_enlazada.h"

#ifndef EXCLUDE_MAIN
int main(void) {
    printf("=== Ejercicio 063: Lista Enlazada Simple ===\n\n");
    
    // Demostración básica según el enunciado
    printf("1. Implementación básica (según enunciado):\n");
    Nodo* lista_simple = NULL;
    
    printf("Insertando valores 10, 20, 30...\n");
    insertar_final_simple(&lista_simple, 10);
    insertar_final_simple(&lista_simple, 20);
    insertar_final_simple(&lista_simple, 30);
    
    printf("Lista resultante: ");
    imprimir_lista_simple(lista_simple);
    
    // Demostración con estructura Lista completa
    printf("\n2. Implementación completa con estructura Lista:\n");
    Lista* lista = crear_lista();
    if (!lista) {
        printf("Error al crear la lista\n");
        return 1;
    }
    
    // Insertar elementos
    printf("Insertando al final: 1, 2, 3\n");
    insertar_final(lista, 1);
    insertar_final(lista, 2);
    insertar_final(lista, 3);
    imprimir_lista_detallada(lista, "Después de insertar al final");
    
    printf("Insertando al inicio: 0\n");
    insertar_inicio(lista, 0);
    imprimir_lista_detallada(lista, "Después de insertar al inicio");
    
    printf("Insertando en posición 2: 15\n");
    insertar_en_posicion(lista, 2, 15);
    imprimir_lista_detallada(lista, "Después de insertar en posición 2");
    
    // Búsquedas
    printf("\n3. Operaciones de búsqueda:\n");
    int valor_buscar = 15;
    Nodo* encontrado = buscar_valor(lista, valor_buscar);
    if (encontrado) {
        printf("Valor %d encontrado en la lista\n", valor_buscar);
    }
    
    size_t posicion = obtener_posicion(lista, 15);
    if (posicion != SIZE_MAX) {
        printf("Valor 15 está en la posición %zu\n", posicion);
    }
    
    printf("¿La lista contiene el valor 3? %s\n", 
           contiene_valor(lista, 3) ? "Sí" : "No");
    printf("¿La lista contiene el valor 99? %s\n", 
           contiene_valor(lista, 99) ? "Sí" : "No");
    
    // Información de la lista
    printf("\n4. Información de la lista:\n");
    printf("Longitud: %zu elementos\n", obtener_longitud(lista));
    printf("¿Está vacía? %s\n", esta_vacia(lista) ? "Sí" : "No");
    
    int primero, ultimo;
    if (obtener_primero(lista, &primero)) {
        printf("Primer elemento: %d\n", primero);
    }
    if (obtener_ultimo(lista, &ultimo)) {
        printf("Último elemento: %d\n", ultimo);
    }
    
    // Copia de lista
    printf("\n5. Copia de lista:\n");
    Lista* copia = copiar_lista(lista);
    if (copia) {
        imprimir_lista_detallada(copia, "Lista copiada");
        printf("¿Las listas son iguales? %s\n", 
               comparar_listas(lista, copia) ? "Sí" : "No");
    }
    
    // Invertir lista
    printf("\n6. Inversión de lista:\n");
    imprimir_lista_detallada(lista, "Lista original");
    invertir_lista(lista);
    imprimir_lista_detallada(lista, "Lista invertida");
    
    // Lista ordenada
    printf("\n7. Lista ordenada:\n");
    Lista* lista_ordenada = crear_lista();
    printf("Insertando valores en orden: 5, 2, 8, 1, 9, 3\n");
    insertar_ordenado(lista_ordenada, 5);
    insertar_ordenado(lista_ordenada, 2);
    insertar_ordenado(lista_ordenada, 8);
    insertar_ordenado(lista_ordenada, 1);
    insertar_ordenado(lista_ordenada, 9);
    insertar_ordenado(lista_ordenada, 3);
    imprimir_lista_detallada(lista_ordenada, "Lista mantenida ordenada");
    
    // Análisis de memoria
    printf("\n8. Análisis de memoria:\n");
    printf("Tamaño de un nodo: %zu bytes\n", sizeof(Nodo));
    printf("Tamaño de la estructura Lista: %zu bytes\n", sizeof(Lista));
    printf("Memoria usada por la lista (aprox.): %zu bytes\n", 
           sizeof(Lista) + (obtener_longitud(lista) * sizeof(Nodo)));
    
    // Demostración de funciones educativas
    printf("\n9. Demostraciones educativas:\n");
    demostrar_operaciones_basicas();
    
    // Comparación de rendimiento
    printf("\n10. Análisis de rendimiento:\n");
    demostrar_rendimiento();
    
    // Cleanup
    destruir_lista_simple(&lista_simple);
    destruir_lista(lista);
    destruir_lista(copia);
    destruir_lista(lista_ordenada);
    
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
        Nodo* siguiente = actual->siguiente;
        destruir_nodo(actual);
        actual = siguiente;
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
        // Lista vacía
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    } else {
        // Añadir al final
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
        // Era lista vacía
        lista->cola = nuevo;
    }
    
    lista->longitud++;
    return true;
}

bool insertar_en_posicion(Lista* lista, size_t posicion, int valor) {
    if (!lista) return false;
    
    // Insertar al inicio
    if (posicion == 0) {
        return insertar_inicio(lista, valor);
    }
    
    // Insertar al final
    if (posicion >= lista->longitud) {
        return insertar_final(lista, valor);
    }
    
    // Insertar en posición intermedia
    Nodo* nuevo = crear_nodo(valor);
    if (!nuevo) return false;
    
    Nodo* anterior = lista->cabeza;
    for (size_t i = 0; i < posicion - 1; i++) {
        anterior = anterior->siguiente;
    }
    
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    lista->longitud++;
    
    return true;
}

bool insertar_ordenado(Lista* lista, int valor) {
    if (!lista) return false;
    
    // Lista vacía o valor menor al primero
    if (lista->cabeza == NULL || valor <= lista->cabeza->valor) {
        return insertar_inicio(lista, valor);
    }
    
    // Buscar posición correcta
    Nodo* actual = lista->cabeza;
    while (actual->siguiente && actual->siguiente->valor < valor) {
        actual = actual->siguiente;
    }
    
    // Insertar después de 'actual'
    Nodo* nuevo = crear_nodo(valor);
    if (!nuevo) return false;
    
    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    
    // Actualizar cola si es necesario
    if (nuevo->siguiente == NULL) {
        lista->cola = nuevo;
    }
    
    lista->longitud++;
    return true;
}

Nodo* buscar_valor(const Lista* lista, int valor) {
    if (!lista) return NULL;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        if (actual->valor == valor) {
            return actual;
        }
        actual = actual->siguiente;
    }
    
    return NULL;
}

Nodo* obtener_en_posicion(const Lista* lista, size_t posicion) {
    if (!lista || posicion >= lista->longitud) return NULL;
    
    Nodo* actual = lista->cabeza;
    for (size_t i = 0; i < posicion; i++) {
        actual = actual->siguiente;
    }
    
    return actual;
}

size_t obtener_posicion(const Lista* lista, int valor) {
    if (!lista) return SIZE_MAX;
    
    Nodo* actual = lista->cabeza;
    size_t posicion = 0;
    
    while (actual) {
        if (actual->valor == valor) {
            return posicion;
        }
        actual = actual->siguiente;
        posicion++;
    }
    
    return SIZE_MAX;
}

bool contiene_valor(const Lista* lista, int valor) {
    return buscar_valor(lista, valor) != NULL;
}

bool esta_vacia(const Lista* lista) {
    return !lista || lista->cabeza == NULL;
}

size_t obtener_longitud(const Lista* lista) {
    return lista ? lista->longitud : 0;
}

bool obtener_primero(const Lista* lista, int* valor) {
    if (!lista || !valor || esta_vacia(lista)) return false;
    
    *valor = lista->cabeza->valor;
    return true;
}

bool obtener_ultimo(const Lista* lista, int* valor) {
    if (!lista || !valor || esta_vacia(lista)) return false;
    
    *valor = lista->cola->valor;
    return true;
}

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
        int primero, ultimo;
        obtener_primero(lista, &primero);
        obtener_ultimo(lista, &ultimo);
        printf("  Primero: %d, Último: %d\n", primero, ultimo);
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

Lista* copiar_lista(const Lista* lista) {
    if (!lista) return NULL;
    
    Lista* nueva = crear_lista();
    if (!nueva) return NULL;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        if (!insertar_final(nueva, actual->valor)) {
            destruir_lista(nueva);
            return NULL;
        }
        actual = actual->siguiente;
    }
    
    return nueva;
}

void invertir_lista(Lista* lista) {
    if (!lista || !lista->cabeza) return;
    
    Nodo* anterior = NULL;
    Nodo* actual = lista->cabeza;
    Nodo* siguiente;
    
    // Guardar referencia a la cola original (que será la nueva cabeza)
    lista->cola = lista->cabeza;
    
    while (actual) {
        siguiente = actual->siguiente;
        actual->siguiente = anterior;
        anterior = actual;
        actual = siguiente;
    }
    
    lista->cabeza = anterior;
}

bool comparar_listas(const Lista* lista1, const Lista* lista2) {
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

// === FUNCIONES VERSIÓN SIMPLE ===

bool insertar_final_simple(Nodo** cabeza, int valor) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) {
        printf("Error al reservar memoria.\n");
        return false;
    }
    
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
    const Nodo* temp = cabeza;
    while (temp != NULL) {
        printf("%d -> ", temp->valor);
        temp = temp->siguiente;
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

// === FUNCIONES EDUCATIVAS ===

void demostrar_operaciones_basicas(void) {
    printf("Demostrando operaciones básicas:\n");
    
    Lista* demo = crear_lista();
    printf("- Lista creada (vacía): ");
    imprimir_lista(demo);
    printf("\n");
    
    printf("- Insertando 1, 2, 3 al final...\n");
    insertar_final(demo, 1);
    insertar_final(demo, 2);
    insertar_final(demo, 3);
    printf("  Resultado: ");
    imprimir_lista(demo);
    printf("\n");
    
    printf("- Insertando 0 al inicio...\n");
    insertar_inicio(demo, 0);
    printf("  Resultado: ");
    imprimir_lista(demo);
    printf("\n");
    
    printf("- Insertando 5 en posición 2...\n");
    insertar_en_posicion(demo, 2, 5);
    printf("  Resultado: ");
    imprimir_lista(demo);
    printf("\n");
    
    destruir_lista(demo);
}

void demostrar_rendimiento(void) {
    printf("Análisis de rendimiento:\n");
    
    Lista* lista = crear_lista();
    
    // Medir inserción al final
    printf("- Inserción al final: O(1) con puntero a cola\n");
    printf("- Búsqueda: O(n) - requiere recorrido secuencial\n");
    printf("- Inserción al inicio: O(1)\n");
    printf("- Inserción en posición: O(n) - requiere recorrido\n");
    
    // Ejemplo práctico
    printf("- Insertando 1000 elementos al final...\n");
    for (int i = 0; i < 1000; i++) {
        insertar_final(lista, i);
    }
    printf("  Lista con %zu elementos creada\n", obtener_longitud(lista));
    
    // Búsqueda en diferentes posiciones
    printf("- Buscando elemento 0 (inicio): ");
    if (buscar_valor(lista, 0)) printf("encontrado\n");
    printf("- Buscando elemento 500 (medio): ");
    if (buscar_valor(lista, 500)) printf("encontrado\n");
    printf("- Buscando elemento 999 (final): ");
    if (buscar_valor(lista, 999)) printf("encontrado\n");
    
    destruir_lista(lista);
}

void analizar_memoria(void) {
    printf("Análisis de uso de memoria:\n");
    printf("- Cada nodo: %zu bytes (int + puntero)\n", sizeof(Nodo));
    printf("- Estructura Lista: %zu bytes\n", sizeof(Lista));
    printf("- Overhead por elemento: mínimo\n");
    printf("- Fragmentación: posible, pero controlable\n");
}
