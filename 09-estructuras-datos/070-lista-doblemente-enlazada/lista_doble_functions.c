/*
 * Ejercicio 070: Lista Doblemente Enlazada
 * 
 * Implementación completa de una lista doblemente enlazada con
 * operaciones bidireccionales y manejo cuidadoso de punteros.
 */

#include "../include/lista_doble.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ================================
 * FUNCIONES DE GESTIÓN DE LISTA
 * ================================ */

ListaDoble* lista_crear(void) {
    ListaDoble* lista = malloc(sizeof(ListaDoble));
    if (!lista) {
        fprintf(stderr, "Error: No se pudo crear la lista\n");
        return NULL;
    }
    
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->tamanio = 0;
    return lista;
}

void lista_destruir(ListaDoble* lista) {
    if (!lista) return;
    
    lista_limpiar(lista);
    free(lista);
}

Nodo* nodo_crear(int dato) {
    Nodo* nodo = malloc(sizeof(Nodo));
    if (!nodo) {
        fprintf(stderr, "Error: No se pudo crear el nodo\n");
        return NULL;
    }
    
    nodo->dato = dato;
    nodo->siguiente = NULL;
    nodo->anterior = NULL;
    return nodo;
}

void nodo_destruir(Nodo* nodo) {
    if (nodo) {
        free(nodo);
    }
}

/* ================================
 * FUNCIONES DE INSERCIÓN
 * ================================ */

bool lista_insertar_inicio(ListaDoble* lista, int dato) {
    if (!lista) return false;
    
    Nodo* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    
    if (lista_vacia(lista)) {
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    } else {
        nuevo->siguiente = lista->cabeza;
        lista->cabeza->anterior = nuevo;
        lista->cabeza = nuevo;
    }
    
    lista->tamanio++;
    return true;
}

bool lista_insertar_final(ListaDoble* lista, int dato) {
    if (!lista) return false;
    
    Nodo* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    
    if (lista_vacia(lista)) {
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    } else {
        nuevo->anterior = lista->cola;
        lista->cola->siguiente = nuevo;
        lista->cola = nuevo;
    }
    
    lista->tamanio++;
    return true;
}

bool lista_insertar_posicion(ListaDoble* lista, size_t posicion, int dato) {
    if (!lista) return false;
    
    // Casos especiales
    if (posicion == 0) {
        return lista_insertar_inicio(lista, dato);
    }
    
    if (posicion >= lista->tamanio) {
        return lista_insertar_final(lista, dato);
    }
    
    // Determinar si es más eficiente buscar desde el inicio o el final
    Nodo* actual;
    if (posicion < lista->tamanio / 2) {
        // Buscar desde el inicio
        actual = lista->cabeza;
        for (size_t i = 0; i < posicion; i++) {
            actual = actual->siguiente;
        }
    } else {
        // Buscar desde el final
        actual = lista->cola;
        for (size_t i = lista->tamanio - 1; i > posicion; i--) {
            actual = actual->anterior;
        }
    }
    
    return lista_insertar_antes(lista, actual, dato);
}

bool lista_insertar_antes(ListaDoble* lista, Nodo* nodo_referencia, int dato) {
    if (!lista || !nodo_referencia) return false;
    
    // Si es el primer nodo, usar insertar_inicio
    if (nodo_referencia == lista->cabeza) {
        return lista_insertar_inicio(lista, dato);
    }
    
    Nodo* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    
    nuevo->siguiente = nodo_referencia;
    nuevo->anterior = nodo_referencia->anterior;
    
    nodo_referencia->anterior->siguiente = nuevo;
    nodo_referencia->anterior = nuevo;
    
    lista->tamanio++;
    return true;
}

bool lista_insertar_despues(ListaDoble* lista, Nodo* nodo_referencia, int dato) {
    if (!lista || !nodo_referencia) return false;
    
    // Si es el último nodo, usar insertar_final
    if (nodo_referencia == lista->cola) {
        return lista_insertar_final(lista, dato);
    }
    
    Nodo* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    
    nuevo->anterior = nodo_referencia;
    nuevo->siguiente = nodo_referencia->siguiente;
    
    nodo_referencia->siguiente->anterior = nuevo;
    nodo_referencia->siguiente = nuevo;
    
    lista->tamanio++;
    return true;
}

size_t lista_insertar_array(ListaDoble* lista, int* datos, size_t cantidad, bool al_final) {
    if (!lista || !datos) return 0;
    
    size_t insertados = 0;
    for (size_t i = 0; i < cantidad; i++) {
        bool exito = al_final ? 
            lista_insertar_final(lista, datos[i]) : 
            lista_insertar_inicio(lista, datos[i]);
        
        if (exito) {
            insertados++;
        }
    }
    
    return insertados;
}

/* ================================
 * FUNCIONES DE ELIMINACIÓN
 * ================================ */

bool lista_eliminar_inicio(ListaDoble* lista) {
    if (!lista || lista_vacia(lista)) return false;
    
    Nodo* a_eliminar = lista->cabeza;
    
    if (lista->tamanio == 1) {
        lista->cabeza = NULL;
        lista->cola = NULL;
    } else {
        lista->cabeza = lista->cabeza->siguiente;
        lista->cabeza->anterior = NULL;
    }
    
    nodo_destruir(a_eliminar);
    lista->tamanio--;
    return true;
}

bool lista_eliminar_final(ListaDoble* lista) {
    if (!lista || lista_vacia(lista)) return false;
    
    Nodo* a_eliminar = lista->cola;
    
    if (lista->tamanio == 1) {
        lista->cabeza = NULL;
        lista->cola = NULL;
    } else {
        lista->cola = lista->cola->anterior;
        lista->cola->siguiente = NULL;
    }
    
    nodo_destruir(a_eliminar);
    lista->tamanio--;
    return true;
}

bool lista_eliminar_posicion(ListaDoble* lista, size_t posicion) {
    if (!lista || posicion >= lista->tamanio) return false;
    
    // Casos especiales
    if (posicion == 0) {
        return lista_eliminar_inicio(lista);
    }
    
    if (posicion == lista->tamanio - 1) {
        return lista_eliminar_final(lista);
    }
    
    // Buscar el nodo a eliminar
    Nodo* a_eliminar = lista_obtener_posicion(lista, posicion);
    if (!a_eliminar) return false;
    
    return lista_eliminar_nodo(lista, a_eliminar);
}

bool lista_eliminar_valor(ListaDoble* lista, int dato) {
    if (!lista) return false;
    
    Nodo* nodo = lista_buscar(lista, dato);
    if (!nodo) return false;
    
    return lista_eliminar_nodo(lista, nodo);
}

size_t lista_eliminar_todas_ocurrencias(ListaDoble* lista, int dato) {
    if (!lista) return 0;
    
    size_t eliminados = 0;
    Nodo* actual = lista->cabeza;
    
    while (actual) {
        Nodo* siguiente = actual->siguiente;  // Guardar siguiente antes de eliminar
        
        if (actual->dato == dato) {
            if (lista_eliminar_nodo(lista, actual)) {
                eliminados++;
            }
        }
        
        actual = siguiente;
    }
    
    return eliminados;
}

bool lista_eliminar_nodo(ListaDoble* lista, Nodo* nodo) {
    if (!lista || !nodo) return false;
    
    // Verificar que el nodo pertenece a la lista
    if (!lista_contiene_nodo(lista, nodo)) return false;
    
    // Casos especiales
    if (nodo == lista->cabeza && nodo == lista->cola) {
        // Único nodo en la lista
        lista->cabeza = NULL;
        lista->cola = NULL;
    } else if (nodo == lista->cabeza) {
        // Primer nodo
        lista->cabeza = nodo->siguiente;
        lista->cabeza->anterior = NULL;
    } else if (nodo == lista->cola) {
        // Último nodo
        lista->cola = nodo->anterior;
        lista->cola->siguiente = NULL;
    } else {
        // Nodo intermedio
        nodo->anterior->siguiente = nodo->siguiente;
        nodo->siguiente->anterior = nodo->anterior;
    }
    
    nodo_destruir(nodo);
    lista->tamanio--;
    return true;
}

void lista_limpiar(ListaDoble* lista) {
    if (!lista) return;
    
    while (!lista_vacia(lista)) {
        lista_eliminar_inicio(lista);
    }
}

/* ================================
 * FUNCIONES DE BÚSQUEDA Y ACCESO
 * ================================ */

Nodo* lista_buscar(ListaDoble* lista, int dato) {
    if (!lista) return NULL;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        if (actual->dato == dato) {
            return actual;
        }
        actual = actual->siguiente;
    }
    
    return NULL;
}

Nodo* lista_buscar_desde_final(ListaDoble* lista, int dato) {
    if (!lista) return NULL;
    
    Nodo* actual = lista->cola;
    while (actual) {
        if (actual->dato == dato) {
            return actual;
        }
        actual = actual->anterior;
    }
    
    return NULL;
}

Nodo* lista_obtener_posicion(ListaDoble* lista, size_t posicion) {
    if (!lista || posicion >= lista->tamanio) return NULL;
    
    Nodo* actual;
    
    // Optimización: elegir la dirección más eficiente
    if (posicion < lista->tamanio / 2) {
        // Buscar desde el inicio
        actual = lista->cabeza;
        for (size_t i = 0; i < posicion; i++) {
            actual = actual->siguiente;
        }
    } else {
        // Buscar desde el final
        actual = lista->cola;
        for (size_t i = lista->tamanio - 1; i > posicion; i--) {
            actual = actual->anterior;
        }
    }
    
    return actual;
}

bool lista_obtener_valor(ListaDoble* lista, size_t posicion, int* valor) {
    if (!valor) return false;
    
    Nodo* nodo = lista_obtener_posicion(lista, posicion);
    if (!nodo) return false;
    
    *valor = nodo->dato;
    return true;
}

size_t lista_encontrar_posicion(ListaDoble* lista, int dato) {
    if (!lista) return SIZE_MAX;
    
    Nodo* actual = lista->cabeza;
    size_t posicion = 0;
    
    while (actual) {
        if (actual->dato == dato) {
            return posicion;
        }
        actual = actual->siguiente;
        posicion++;
    }
    
    return SIZE_MAX;  // No encontrado
}

/* ================================
 * FUNCIONES DE RECORRIDO
 * ================================ */

void lista_imprimir_adelante(ListaDoble* lista) {
    if (!lista) {
        printf("Lista NULL\n");
        return;
    }
    
    if (lista_vacia(lista)) {
        printf("Lista vacía\n");
        return;
    }
    
    printf("Recorrido hacia adelante: ");
    Nodo* actual = lista->cabeza;
    while (actual) {
        printf("%d", actual->dato);
        if (actual->siguiente) {
            printf(" <-> ");
        }
        actual = actual->siguiente;
    }
    printf("\n");
}

void lista_imprimir_atras(ListaDoble* lista) {
    if (!lista) {
        printf("Lista NULL\n");
        return;
    }
    
    if (lista_vacia(lista)) {
        printf("Lista vacía\n");
        return;
    }
    
    printf("Recorrido hacia atrás: ");
    Nodo* actual = lista->cola;
    while (actual) {
        printf("%d", actual->dato);
        if (actual->anterior) {
            printf(" <-> ");
        }
        actual = actual->anterior;
    }
    printf("\n");
}

void lista_recorrer_adelante(ListaDoble* lista, void (*funcion)(int dato, void* contexto), void* contexto) {
    if (!lista || !funcion) return;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        funcion(actual->dato, contexto);
        actual = actual->siguiente;
    }
}

void lista_recorrer_atras(ListaDoble* lista, void (*funcion)(int dato, void* contexto), void* contexto) {
    if (!lista || !funcion) return;
    
    Nodo* actual = lista->cola;
    while (actual) {
        funcion(actual->dato, contexto);
        actual = actual->anterior;
    }
}

/* ================================
 * FUNCIONES DE ITERADOR
 * ================================ */

IteradorDoble iterador_crear(ListaDoble* lista, bool adelante) {
    IteradorDoble iter;
    iter.lista = lista;
    iter.adelante = adelante;
    
    if (lista && !lista_vacia(lista)) {
        iter.actual = adelante ? lista->cabeza : lista->cola;
    } else {
        iter.actual = NULL;
    }
    
    return iter;
}

bool iterador_tiene_siguiente(IteradorDoble* iter) {
    return iter && iter->actual != NULL;
}

int iterador_siguiente(IteradorDoble* iter) {
    if (!iterador_tiene_siguiente(iter)) return 0;
    
    int valor = iter->actual->dato;
    iter->actual = iter->adelante ? iter->actual->siguiente : iter->actual->anterior;
    
    return valor;
}

void iterador_reiniciar(IteradorDoble* iter) {
    if (!iter || !iter->lista || lista_vacia(iter->lista)) {
        if (iter) iter->actual = NULL;
        return;
    }
    
    iter->actual = iter->adelante ? iter->lista->cabeza : iter->lista->cola;
}

void iterador_cambiar_direccion(IteradorDoble* iter) {
    if (!iter) return;
    
    iter->adelante = !iter->adelante;
    iterador_reiniciar(iter);
}

/* ================================
 * FUNCIONES DE UTILIDAD
 * ================================ */

bool lista_vacia(ListaDoble* lista) {
    return !lista || lista->tamanio == 0;
}

size_t lista_tamanio(ListaDoble* lista) {
    return lista ? lista->tamanio : 0;
}

bool lista_primero(ListaDoble* lista, int* valor) {
    if (!valor || lista_vacia(lista)) return false;
    
    *valor = lista->cabeza->dato;
    return true;
}

bool lista_ultimo(ListaDoble* lista, int* valor) {
    if (!valor || lista_vacia(lista)) return false;
    
    *valor = lista->cola->dato;
    return true;
}

/* ================================
 * FUNCIONES DE VALIDACIÓN
 * ================================ */

bool lista_validar_integridad(ListaDoble* lista) {
    if (!lista) return false;
    
    // Lista vacía es válida
    if (lista_vacia(lista)) {
        return lista->cabeza == NULL && lista->cola == NULL;
    }
    
    // Verificar que cabeza y cola no sean NULL
    if (!lista->cabeza || !lista->cola) return false;
    
    // Verificar consistencia del tamaño
    size_t conteo_adelante = lista_contar_adelante(lista);
    size_t conteo_atras = lista_contar_atras(lista);
    
    if (conteo_adelante != lista->tamanio || conteo_atras != lista->tamanio) {
        return false;
    }
    
    // Verificar enlaces bidireccionales
    Nodo* actual = lista->cabeza;
    
    // El primer nodo no debe tener anterior
    if (actual->anterior != NULL) return false;
    
    while (actual->siguiente) {
        // Verificar enlace bidireccional
        if (actual->siguiente->anterior != actual) return false;
        actual = actual->siguiente;
    }
    
    // El último nodo debe ser la cola
    if (actual != lista->cola) return false;
    
    return true;
}

bool lista_contiene_nodo(ListaDoble* lista, Nodo* nodo) {
    if (!lista || !nodo) return false;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        if (actual == nodo) return true;
        actual = actual->siguiente;
    }
    
    return false;
}

size_t lista_contar_adelante(ListaDoble* lista) {
    if (!lista) return 0;
    
    size_t contador = 0;
    Nodo* actual = lista->cabeza;
    
    while (actual) {
        contador++;
        actual = actual->siguiente;
    }
    
    return contador;
}

size_t lista_contar_atras(ListaDoble* lista) {
    if (!lista) return 0;
    
    size_t contador = 0;
    Nodo* actual = lista->cola;
    
    while (actual) {
        contador++;
        actual = actual->anterior;
    }
    
    return contador;
}

/* ================================
 * FUNCIONES AVANZADAS
 * ================================ */

void lista_invertir(ListaDoble* lista) {
    if (!lista || lista->tamanio <= 1) return;
    
    Nodo* actual = lista->cabeza;
    Nodo* temp = NULL;
    
    // Intercambiar punteros siguiente y anterior para cada nodo
    while (actual) {
        temp = actual->anterior;
        actual->anterior = actual->siguiente;
        actual->siguiente = temp;
        actual = actual->anterior;  // Avanzar (ahora anterior apunta al siguiente)
    }
    
    // Intercambiar cabeza y cola
    temp = lista->cabeza;
    lista->cabeza = lista->cola;
    lista->cola = temp;
}

void lista_ordenar(ListaDoble* lista, bool ascendente) {
    if (!lista || lista->tamanio <= 1) return;
    
    // Algoritmo de ordenamiento burbuja optimizado para listas doblemente enlazadas
    bool hubo_cambio;
    Nodo* ultimo = NULL;
    
    do {
        hubo_cambio = false;
        Nodo* actual = lista->cabeza;
        
        while (actual->siguiente != ultimo) {
            bool debe_intercambiar = ascendente ? 
                (actual->dato > actual->siguiente->dato) : 
                (actual->dato < actual->siguiente->dato);
            
            if (debe_intercambiar) {
                // Intercambiar valores (más simple que intercambiar nodos)
                int temp = actual->dato;
                actual->dato = actual->siguiente->dato;
                actual->siguiente->dato = temp;
                hubo_cambio = true;
            }
            
            actual = actual->siguiente;
        }
        ultimo = actual;
    } while (hubo_cambio);
}

ListaDoble* lista_clonar(ListaDoble* lista) {
    if (!lista) return NULL;
    
    ListaDoble* clon = lista_crear();
    if (!clon) return NULL;
    
    Nodo* actual = lista->cabeza;
    while (actual) {
        if (!lista_insertar_final(clon, actual->dato)) {
            lista_destruir(clon);
            return NULL;
        }
        actual = actual->siguiente;
    }
    
    return clon;
}

size_t lista_eliminar_duplicados(ListaDoble* lista) {
    if (!lista || lista->tamanio <= 1) return 0;
    
    size_t eliminados = 0;
    Nodo* actual = lista->cabeza;
    
    while (actual) {
        Nodo* comparar = actual->siguiente;
        
        while (comparar) {
            Nodo* siguiente = comparar->siguiente;
            
            if (comparar->dato == actual->dato) {
                if (lista_eliminar_nodo(lista, comparar)) {
                    eliminados++;
                }
            }
            
            comparar = siguiente;
        }
        
        actual = actual->siguiente;
    }
    
    return eliminados;
}

/* ================================
 * FUNCIONES DE VISUALIZACIÓN
 * ================================ */

void lista_mostrar_estructura(ListaDoble* lista) {
    if (!lista) {
        printf("Lista es NULL\n");
        return;
    }
    
    printf("\n=== ESTRUCTURA DE LISTA DOBLEMENTE ENLAZADA ===\n");
    printf("Tamaño: %zu\n", lista->tamanio);
    printf("Cabeza: %p\n", (void*)lista->cabeza);
    printf("Cola: %p\n", (void*)lista->cola);
    
    if (lista_vacia(lista)) {
        printf("Lista vacía\n");
        printf("=============================================\n");
        return;
    }
    
    printf("\nEstructura detallada:\n");
    Nodo* actual = lista->cabeza;
    int posicion = 0;
    
    while (actual) {
        printf("[%d] Nodo %p:\n", posicion, (void*)actual);
        printf("    Dato: %d\n", actual->dato);
        printf("    Anterior: %p\n", (void*)actual->anterior);
        printf("    Siguiente: %p\n", (void*)actual->siguiente);
        printf("\n");
        
        actual = actual->siguiente;
        posicion++;
    }
    
    printf("=============================================\n");
}

void lista_mostrar_estadisticas(ListaDoble* lista) {
    if (!lista) {
        printf("Lista es NULL\n");
        return;
    }
    
    printf("\n=== ESTADÍSTICAS DE LISTA ===\n");
    printf("Tamaño: %zu elementos\n", lista->tamanio);
    printf("Estado: %s\n", lista_vacia(lista) ? "Vacía" : "Con elementos");
    printf("Integridad: %s\n", lista_validar_integridad(lista) ? "Válida" : "Inválida");
    
    if (!lista_vacia(lista)) {
        int primero, ultimo;
        lista_primero(lista, &primero);
        lista_ultimo(lista, &ultimo);
        
        printf("Primer elemento: %d\n", primero);
        printf("Último elemento: %d\n", ultimo);
        
        // Calcular estadísticas adicionales
        int suma = 0, minimo = primero, maximo = primero;
        Nodo* actual = lista->cabeza;
        
        while (actual) {
            suma += actual->dato;
            if (actual->dato < minimo) minimo = actual->dato;
            if (actual->dato > maximo) maximo = actual->dato;
            actual = actual->siguiente;
        }
        
        printf("Suma total: %d\n", suma);
        printf("Promedio: %.2f\n", (float)suma / lista->tamanio);
        printf("Mínimo: %d\n", minimo);
        printf("Máximo: %d\n", maximo);
    }
    
    printf("=============================\n");
}

void lista_imprimir_json(ListaDoble* lista) {
    if (!lista) {
        printf("null\n");
        return;
    }
    
    printf("{\n");
    printf("  \"tamanio\": %zu,\n", lista->tamanio);
    printf("  \"elementos\": [");
    
    if (!lista_vacia(lista)) {
        Nodo* actual = lista->cabeza;
        bool primero = true;
        
        while (actual) {
            if (!primero) printf(", ");
            printf("%d", actual->dato);
            primero = false;
            actual = actual->siguiente;
        }
    }
    
    printf("],\n");
    printf("  \"vacia\": %s\n", lista_vacia(lista) ? "true" : "false");
    printf("}\n");
}

/* ================================
 * FUNCIONES AUXILIARES PARA DEMOSTRACIONES
 * ================================ */

// Función auxiliar para imprimir con formato
static void imprimir_elemento(int dato, void* contexto) {
    char* prefijo = (char*)contexto;
    printf("%s%d ", prefijo, dato);
}

/* ================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ================================ */

void demo_operaciones_basicas(ListaDoble* lista) {
    if (!lista) return;
    
    printf("\n=== DEMO: OPERACIONES BÁSICAS ===\n");
    
    // Insertar elementos
    printf("Insertando elementos...\n");
    lista_insertar_final(lista, 10);
    lista_insertar_final(lista, 20);
    lista_insertar_final(lista, 30);
    lista_insertar_inicio(lista, 5);
    
    lista_imprimir_adelante(lista);
    lista_imprimir_atras(lista);
    
    // Buscar elementos
    printf("\nBuscando elemento 20...\n");
    Nodo* encontrado = lista_buscar(lista, 20);
    printf("Encontrado: %s\n", encontrado ? "Sí" : "No");
    
    // Eliminar elementos
    printf("\nEliminando primer elemento...\n");
    lista_eliminar_inicio(lista);
    lista_imprimir_adelante(lista);
    
    printf("Eliminando último elemento...\n");
    lista_eliminar_final(lista);
    lista_imprimir_adelante(lista);
    
    printf("================================\n");
}

void demo_recorridos_bidireccionales(ListaDoble* lista) {
    if (!lista) return;
    
    printf("\n=== DEMO: RECORRIDOS BIDIRECCIONALES ===\n");
    
    // Llenar lista con algunos elementos
    int valores[] = {1, 2, 3, 4, 5};
    lista_insertar_array(lista, valores, 5, true);
    
    printf("Lista original:\n");
    lista_imprimir_adelante(lista);
    lista_imprimir_atras(lista);
    
    printf("\nRecorrido personalizado hacia adelante:\n");
    lista_recorrer_adelante(lista, imprimir_elemento, "-> ");
    printf("\n");
    
    printf("Recorrido personalizado hacia atrás:\n");
    lista_recorrer_atras(lista, imprimir_elemento, "<- ");
    printf("\n");
    
    printf("=========================================\n");
}

void demo_iteradores(ListaDoble* lista) {
    if (!lista) return;
    
    printf("\n=== DEMO: ITERADORES ===\n");
    
    // Preparar lista
    int valores[] = {10, 20, 30, 40, 50};
    lista_insertar_array(lista, valores, 5, true);
    
    // Iterador hacia adelante
    printf("Iterador hacia adelante:\n");
    IteradorDoble iter_adelante = iterador_crear(lista, true);
    while (iterador_tiene_siguiente(&iter_adelante)) {
        printf("%d ", iterador_siguiente(&iter_adelante));
    }
    printf("\n");
    
    // Iterador hacia atrás
    printf("Iterador hacia atrás:\n");
    IteradorDoble iter_atras = iterador_crear(lista, false);
    while (iterador_tiene_siguiente(&iter_atras)) {
        printf("%d ", iterador_siguiente(&iter_atras));
    }
    printf("\n");
    
    // Cambiar dirección
    printf("Cambiando dirección del iterador:\n");
    iterador_cambiar_direccion(&iter_adelante);
    while (iterador_tiene_siguiente(&iter_adelante)) {
        printf("%d ", iterador_siguiente(&iter_adelante));
    }
    printf("\n");
    
    printf("========================\n");
}

void demo_operaciones_avanzadas(ListaDoble* lista) {
    if (!lista) return;
    
    printf("\n=== DEMO: OPERACIONES AVANZADAS ===\n");
    
    // Crear lista con duplicados
    int valores[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    lista_insertar_array(lista, valores, 10, true);
    
    printf("Lista original:\n");
    lista_imprimir_adelante(lista);
    
    // Clonar lista
    ListaDoble* clon = lista_clonar(lista);
    printf("Lista clonada:\n");
    lista_imprimir_adelante(clon);
    
    // Eliminar duplicados de la original
    size_t eliminados = lista_eliminar_duplicados(lista);
    printf("Eliminados %zu duplicados:\n", eliminados);
    lista_imprimir_adelante(lista);
    
    // Ordenar la lista clonada
    lista_ordenar(clon, true);
    printf("Lista clonada ordenada:\n");
    lista_imprimir_adelante(clon);
    
    // Invertir lista
    lista_invertir(lista);
    printf("Lista original invertida:\n");
    lista_imprimir_adelante(lista);
    
    // Limpiar clon
    lista_destruir(clon);
    
    printf("===================================\n");
}