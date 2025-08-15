#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/eliminar_nodo.h"

#ifndef EXCLUDE_MAIN
int main(void) {
    printf("=== Ejercicio 064: Eliminar Nodo de Lista Enlazada ===\n\n");
    
    // Demostración básica según el enunciado
    printf("1. Implementación básica (según enunciado):\n");
    Nodo* lista_simple = NULL;
    
    printf("Creando lista: 10 -> 20 -> 30 -> 40\n");
    insertar_final_simple(&lista_simple, 10);
    insertar_final_simple(&lista_simple, 20);
    insertar_final_simple(&lista_simple, 30);
    insertar_final_simple(&lista_simple, 40);
    
    printf("Lista original: ");
    imprimir_lista_simple(lista_simple);
    
    printf("\nEliminando nodo con valor 30...\n");
    if (eliminar_nodo_simple(&lista_simple, 30)) {
        printf("Nodo eliminado exitosamente.\n");
    }
    printf("Lista después: ");
    imprimir_lista_simple(lista_simple);
    
    printf("\nEliminando nodo con valor 10 (cabeza)...\n");
    if (eliminar_nodo_simple(&lista_simple, 10)) {
        printf("Cabeza eliminada exitosamente.\n");
    }
    printf("Lista después: ");
    imprimir_lista_simple(lista_simple);
    
    printf("\nIntentando eliminar valor 99 (no existe)...\n");
    if (!eliminar_nodo_simple(&lista_simple, 99)) {
        printf("Valor no encontrado en la lista.\n");
    }
    printf("Lista después: ");
    imprimir_lista_simple(lista_simple);
    
    // Demostración con estructura Lista completa
    printf("\n2. Implementación completa con estructura Lista:\n");
    Lista* lista = crear_lista();
    if (!lista) {
        printf("Error al crear la lista\n");
        return 1;
    }
    
    // Crear lista de prueba
    printf("Creando lista: 1 -> 2 -> 3 -> 4 -> 5\n");
    for (int i = 1; i <= 5; i++) {
        insertar_final(lista, i);
    }
    imprimir_lista_detallada(lista, "Lista inicial");
    
    // Eliminar nodo del medio
    printf("Eliminando nodo con valor 3...\n");
    if (eliminar_nodo(lista, 3)) {
        printf("Nodo eliminado exitosamente.\n");
        imprimir_lista_detallada(lista, "Después de eliminar 3");
    }
    
    // Eliminar primer nodo
    printf("Eliminando primer nodo...\n");
    if (eliminar_primero(lista)) {
        printf("Primer nodo eliminado.\n");
        imprimir_lista_detallada(lista, "Después de eliminar primero");
    }
    
    // Eliminar último nodo
    printf("Eliminando último nodo...\n");
    if (eliminar_ultimo(lista)) {
        printf("Último nodo eliminado.\n");
        imprimir_lista_detallada(lista, "Después de eliminar último");
    }
    
    // Eliminar por posición
    printf("Eliminando nodo en posición 0...\n");
    if (eliminar_en_posicion(lista, 0)) {
        printf("Nodo en posición 0 eliminado.\n");
        imprimir_lista_detallada(lista, "Después de eliminar posición 0");
    }
    
    // Demostrar eliminación múltiple
    printf("\n3. Eliminación múltiple:\n");
    Lista* lista_multi = crear_lista();
    int valores[] = {1, 2, 1, 3, 1, 4, 1};
    int num_valores = sizeof(valores) / sizeof(valores[0]);
    
    printf("Creando lista con duplicados: ");
    for (int i = 0; i < num_valores; i++) {
        insertar_final(lista_multi, valores[i]);
    }
    imprimir_lista(lista_multi);
    printf("\n");
    
    printf("Eliminando todas las ocurrencias del valor 1...\n");
    size_t eliminados = eliminar_todos(lista_multi, 1);
    printf("Se eliminaron %zu nodos.\n", eliminados);
    imprimir_lista_detallada(lista_multi, "Después de eliminar todos los 1");
    
    // Demostrar eliminación de duplicados
    printf("\n4. Eliminación de duplicados:\n");
    Lista* lista_dup = crear_lista();
    int valores_dup[] = {1, 2, 2, 3, 3, 3, 4, 4, 5};
    int num_dup = sizeof(valores_dup) / sizeof(valores_dup[0]);
    
    printf("Creando lista con duplicados: ");
    for (int i = 0; i < num_dup; i++) {
        insertar_final(lista_dup, valores_dup[i]);
    }
    imprimir_lista(lista_dup);
    printf("\n");
    
    printf("Eliminando duplicados...\n");
    size_t duplicados_eliminados = eliminar_duplicados(lista_dup);
    printf("Se eliminaron %zu duplicados.\n", duplicados_eliminados);
    imprimir_lista_detallada(lista_dup, "Después de eliminar duplicados");
    
    // Demostrar eliminación por rango
    printf("\n5. Eliminación por rango:\n");
    Lista* lista_rango = crear_lista();
    printf("Creando lista: 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7\n");
    for (int i = 0; i < 8; i++) {
        insertar_final(lista_rango, i);
    }
    imprimir_lista_detallada(lista_rango, "Lista inicial");
    
    printf("Eliminando rango [2, 5]...\n");
    size_t rango_eliminados = eliminar_rango(lista_rango, 2, 5);
    printf("Se eliminaron %zu nodos del rango.\n", rango_eliminados);
    imprimir_lista_detallada(lista_rango, "Después de eliminar rango [2,5]");
    
    // Validación de integridad
    printf("\n6. Validación de integridad:\n");
    printf("¿Lista válida? %s\n", validar_integridad_lista(lista_rango) ? "Sí" : "No");
    printf("Longitud reportada: %zu\n", obtener_longitud(lista_rango));
    printf("¿Lista vacía? %s\n", esta_vacia(lista_rango) ? "Sí" : "No");
    
    // Demostrar casos especiales
    printf("\n7. Casos especiales:\n");
    demostrar_casos_especiales();
    
    // Análisis de rendimiento
    printf("\n8. Análisis de rendimiento:\n");
    analizar_rendimiento_eliminacion();
    
    // Cleanup
    destruir_lista_simple(&lista_simple);
    destruir_lista(lista);
    destruir_lista(lista_multi);
    destruir_lista(lista_dup);
    destruir_lista(lista_rango);
    
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
    
    vaciar_lista(lista);
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

bool insertar_en_posicion(Lista* lista, size_t posicion, int valor) {
    if (!lista) return false;
    
    if (posicion == 0) {
        return insertar_inicio(lista, valor);
    }
    
    if (posicion >= lista->longitud) {
        return insertar_final(lista, valor);
    }
    
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

// === FUNCIONES DE ELIMINACIÓN (PRINCIPAL) ===

bool eliminar_nodo(Lista* lista, int valor) {
    if (!lista || !lista->cabeza) return false;
    
    // Caso especial: eliminar la cabeza
    if (lista->cabeza->valor == valor) {
        return eliminar_primero(lista);
    }
    
    Nodo* anterior = lista->cabeza;
    Nodo* actual = lista->cabeza->siguiente;
    
    while (actual) {
        if (actual->valor == valor) {
            anterior->siguiente = actual->siguiente;
            
            // Actualizar cola si es necesario
            if (actual == lista->cola) {
                lista->cola = anterior;
            }
            
            destruir_nodo(actual);
            lista->longitud--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    return false; // Valor no encontrado
}

bool eliminar_en_posicion(Lista* lista, size_t posicion) {
    if (!lista || posicion >= lista->longitud) return false;
    
    if (posicion == 0) {
        return eliminar_primero(lista);
    }
    
    Nodo* anterior = lista->cabeza;
    for (size_t i = 0; i < posicion - 1; i++) {
        anterior = anterior->siguiente;
    }
    
    Nodo* nodo_eliminar = anterior->siguiente;
    anterior->siguiente = nodo_eliminar->siguiente;
    
    // Actualizar cola si es necesario
    if (nodo_eliminar == lista->cola) {
        lista->cola = anterior;
    }
    
    destruir_nodo(nodo_eliminar);
    lista->longitud--;
    return true;
}

bool eliminar_primero(Lista* lista) {
    if (!lista || !lista->cabeza) return false;
    
    Nodo* nodo_eliminar = lista->cabeza;
    lista->cabeza = lista->cabeza->siguiente;
    
    // Si era el único nodo
    if (lista->cola == nodo_eliminar) {
        lista->cola = NULL;
    }
    
    destruir_nodo(nodo_eliminar);
    lista->longitud--;
    return true;
}

bool eliminar_ultimo(Lista* lista) {
    if (!lista || !lista->cabeza) return false;
    
    // Solo un nodo
    if (lista->cabeza == lista->cola) {
        destruir_nodo(lista->cabeza);
        lista->cabeza = NULL;
        lista->cola = NULL;
        lista->longitud = 0;
        return true;
    }
    
    // Encontrar el penúltimo nodo
    Nodo* anterior = lista->cabeza;
    while (anterior->siguiente != lista->cola) {
        anterior = anterior->siguiente;
    }
    
    destruir_nodo(lista->cola);
    lista->cola = anterior;
    anterior->siguiente = NULL;
    lista->longitud--;
    return true;
}

size_t eliminar_todos(Lista* lista, int valor) {
    if (!lista) return 0;
    
    size_t eliminados = 0;
    
    // Eliminar de la cabeza
    while (lista->cabeza && lista->cabeza->valor == valor) {
        eliminar_primero(lista);
        eliminados++;
    }
    
    if (!lista->cabeza) return eliminados;
    
    // Eliminar del resto de la lista
    Nodo* anterior = lista->cabeza;
    Nodo* actual = lista->cabeza->siguiente;
    
    while (actual) {
        if (actual->valor == valor) {
            anterior->siguiente = actual->siguiente;
            
            if (actual == lista->cola) {
                lista->cola = anterior;
            }
            
            Nodo* temp = actual;
            actual = actual->siguiente;
            destruir_nodo(temp);
            lista->longitud--;
            eliminados++;
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
    
    return eliminados;
}

void vaciar_lista(Lista* lista) {
    if (!lista) return;
    
    while (lista->cabeza) {
        eliminar_primero(lista);
    }
}

// === FUNCIONES DE BÚSQUEDA Y CONSULTA ===

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

bool esta_vacia(const Lista* lista) {
    return !lista || lista->cabeza == NULL;
}

size_t obtener_longitud(const Lista* lista) {
    return lista ? lista->longitud : 0;
}

bool contiene_valor(const Lista* lista, int valor) {
    return buscar_valor(lista, valor) != NULL;
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

bool eliminar_nodo_simple(Nodo** cabeza, int valor) {
    if (!cabeza || !*cabeza) return false;
    
    Nodo* actual = *cabeza;
    Nodo* anterior = NULL;
    
    while (actual) {
        if (actual->valor == valor) {
            if (anterior == NULL) {
                // El nodo a eliminar es la cabeza
                *cabeza = actual->siguiente;
            } else {
                // Reenlazar para saltarse el nodo actual
                anterior->siguiente = actual->siguiente;
            }
            free(actual);
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    return false; // Valor no encontrado
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

// === FUNCIONES EDUCATIVAS ===

void demostrar_casos_especiales(void) {
    printf("Demostrando casos especiales de eliminación:\n");
    
    // Lista vacía
    Lista* lista_vacia = crear_lista();
    printf("- Eliminar de lista vacía: %s\n", 
           eliminar_nodo(lista_vacia, 1) ? "Éxito" : "Falló (esperado)");
    
    // Lista con un solo elemento
    Lista* lista_uno = crear_lista();
    insertar_final(lista_uno, 42);
    printf("- Lista con un elemento: ");
    imprimir_lista(lista_uno);
    printf("\n  Eliminando único elemento: %s\n", 
           eliminar_nodo(lista_uno, 42) ? "Éxito" : "Falló");
    printf("  Lista después: ");
    imprimir_lista(lista_uno);
    printf("\n");
    
    // Eliminar cabeza
    Lista* lista_cabeza = crear_lista();
    insertar_final(lista_cabeza, 1);
    insertar_final(lista_cabeza, 2);
    insertar_final(lista_cabeza, 3);
    printf("- Lista para eliminar cabeza: ");
    imprimir_lista(lista_cabeza);
    printf("\n  Eliminando cabeza (1): %s\n", 
           eliminar_nodo(lista_cabeza, 1) ? "Éxito" : "Falló");
    printf("  Lista después: ");
    imprimir_lista(lista_cabeza);
    printf("\n");
    
    // Eliminar cola
    printf("  Eliminando cola (3): %s\n", 
           eliminar_nodo(lista_cabeza, 3) ? "Éxito" : "Falló");
    printf("  Lista después: ");
    imprimir_lista(lista_cabeza);
    printf("\n");
    
    destruir_lista(lista_vacia);
    destruir_lista(lista_uno);
    destruir_lista(lista_cabeza);
}

void analizar_rendimiento_eliminacion(void) {
    printf("Análisis de rendimiento de eliminación:\n");
    printf("- eliminar_primero(): O(1) - Acceso directo\n");
    printf("- eliminar_ultimo(): O(n) - Requiere recorrido\n");
    printf("- eliminar_nodo(valor): O(n) - Búsqueda lineal\n");
    printf("- eliminar_en_posicion(pos): O(n) - Recorrido hasta posición\n");
    printf("- eliminar_todos(valor): O(n) - Un solo recorrido\n");
    printf("- vaciar_lista(): O(n) - Eliminar todos los nodos\n\n");
    
    printf("Optimizaciones implementadas:\n");
    printf("- Puntero a cola para detección rápida del último nodo\n");
    printf("- Contador de longitud para validación O(1)\n");
    printf("- Casos especiales optimizados (cabeza, cola)\n");
    printf("- Eliminación múltiple en un solo recorrido\n");
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

// === FUNCIONES AVANZADAS ===

size_t eliminar_con_predicado(Lista* lista, bool (*predicado)(int valor)) {
    if (!lista || !predicado) return 0;
    
    size_t eliminados = 0;
    
    // Eliminar de la cabeza
    while (lista->cabeza && predicado(lista->cabeza->valor)) {
        eliminar_primero(lista);
        eliminados++;
    }
    
    if (!lista->cabeza) return eliminados;
    
    // Eliminar del resto
    Nodo* anterior = lista->cabeza;
    Nodo* actual = lista->cabeza->siguiente;
    
    while (actual) {
        if (predicado(actual->valor)) {
            anterior->siguiente = actual->siguiente;
            
            if (actual == lista->cola) {
                lista->cola = anterior;
            }
            
            Nodo* temp = actual;
            actual = actual->siguiente;
            destruir_nodo(temp);
            lista->longitud--;
            eliminados++;
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
    
    return eliminados;
}

size_t eliminar_duplicados(Lista* lista) {
    if (!lista || !lista->cabeza) return 0;
    
    size_t eliminados = 0;
    Nodo* actual = lista->cabeza;
    
    while (actual && actual->siguiente) {
        if (actual->valor == actual->siguiente->valor) {
            Nodo* duplicado = actual->siguiente;
            actual->siguiente = duplicado->siguiente;
            
            if (duplicado == lista->cola) {
                lista->cola = actual;
            }
            
            destruir_nodo(duplicado);
            lista->longitud--;
            eliminados++;
        } else {
            actual = actual->siguiente;
        }
    }
    
    return eliminados;
}

size_t eliminar_rango(Lista* lista, size_t inicio, size_t fin) {
    if (!lista || inicio > fin || inicio >= lista->longitud) return 0;
    
    // Ajustar fin al último índice válido
    if (fin >= lista->longitud) {
        fin = lista->longitud - 1;
    }
    
    size_t eliminados = 0;
    for (size_t i = fin; i >= inicio && i != SIZE_MAX; i--) {
        if (eliminar_en_posicion(lista, i)) {
            eliminados++;
        }
    }
    
    return eliminados;
}
