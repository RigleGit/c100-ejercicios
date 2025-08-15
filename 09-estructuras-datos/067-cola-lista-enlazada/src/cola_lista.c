#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/cola_lista.h"

#ifndef EXCLUDE_MAIN
int main(void) {
    printf("=== Ejercicio 067: Cola (Queue) con Lista Enlazada ===\n\n");
    
    // Demostración básica según el enunciado
    printf("1. Implementación básica (según enunciado):\n");
    ColaSimple cola_simple = {NULL, NULL};
    
    printf("Insertando elementos: 5, 10, 15\n");
    enqueue_simple(&cola_simple, 5);
    enqueue_simple(&cola_simple, 10);
    enqueue_simple(&cola_simple, 15);
    
    printf("Estado de la cola: ");
    imprimir_cola_simple(&cola_simple);
    
    int valor;
    if (dequeue_simple(&cola_simple, &valor) == COLA_EXITO) {
        printf("Atendiendo: %d\n", valor);
    }
    
    printf("Estado después del dequeue: ");
    imprimir_cola_simple(&cola_simple);
    
    printf("Insertando elemento: 20\n");
    enqueue_simple(&cola_simple, 20);
    
    printf("Estado final: ");
    imprimir_cola_simple(&cola_simple);
    
    // Demostración con estructura Cola completa
    printf("\n2. Implementación completa con estructura Cola:\n");
    Cola* cola = crear_cola(0); // Cola ilimitada
    if (!cola) {
        printf("Error al crear la cola\n");
        return 1;
    }
    
    // Operaciones básicas
    printf("Insertando secuencia: 1, 2, 3, 4, 5\n");
    for (int i = 1; i <= 5; i++) {
        ResultadoCola resultado = enqueue(cola, i);
        if (resultado != COLA_EXITO) {
            printf("Error al insertar %d: %s\n", i, resultado_a_cadena(resultado));
        }
    }
    
    imprimir_cola_detallada(cola, "Cola después de inserciones");
    imprimir_cola_visual(cola);
    
    // Consultar frente y final
    if (front(cola, &valor) == COLA_EXITO) {
        printf("Elemento al frente: %d\n", valor);
    }
    if (back(cola, &valor) == COLA_EXITO) {
        printf("Elemento al final: %d\n", valor);
    }
    
    // Retirar algunos elementos
    printf("\nRetirando elementos (FIFO):\n");
    for (int i = 0; i < 3; i++) {
        if (dequeue(cola, &valor) == COLA_EXITO) {
            printf("Dequeue %d: %d\n", i + 1, valor);
        }
    }
    
    imprimir_cola_detallada(cola, "Cola después de 3 dequeues");
    
    // Demostrar operaciones avanzadas
    printf("\n3. Operaciones avanzadas:\n");
    
    // Agregar más elementos
    for (int i = 10; i <= 14; i++) {
        enqueue(cola, i);
    }
    imprimir_cola_detallada(cola, "Después de agregar 10-14");
    
    // Clonar cola
    printf("\n4. Clonación de cola:\n");
    Cola* cola_clon = clonar_cola(cola);
    if (cola_clon) {
        imprimir_cola_detallada(cola, "Cola original");
        imprimir_cola_detallada(cola_clon, "Cola clonada");
        
        printf("¿Son iguales? %s\n", colas_iguales(cola, cola_clon) ? "Sí" : "No");
    }
    
    // Invertir cola
    printf("\n5. Inversión de cola:\n");
    imprimir_cola_detallada(cola_clon, "Antes de invertir");
    if (invertir_cola(cola_clon) == COLA_EXITO) {
        imprimir_cola_detallada(cola_clon, "Después de invertir");
    }
    
    // Rotar cola
    printf("\n6. Rotación de cola:\n");
    imprimir_cola_detallada(cola, "Antes de rotar 2 posiciones");
    if (rotar_cola(cola, 2) == COLA_EXITO) {
        imprimir_cola_detallada(cola, "Después de rotar 2 posiciones");
    }
    
    // Demostrar cola con capacidad limitada
    printf("\n7. Cola con capacidad limitada:\n");
    Cola* cola_limitada = crear_cola(3); // Capacidad máxima de 3
    
    printf("Intentando insertar 5 elementos en cola de capacidad 3:\n");
    for (int i = 1; i <= 5; i++) {
        ResultadoCola resultado = enqueue(cola_limitada, i * 10);
        printf("Enqueue %d: %s\n", i * 10, resultado_a_cadena(resultado));
        if (resultado == COLA_EXITO) {
            printf("  Tamaño: %zu/%zu\n", obtener_tamaño(cola_limitada), 
                   obtener_capacidad(cola_limitada));
        }
    }
    
    imprimir_cola_detallada(cola_limitada, "Cola limitada final");
    
    // Combinación de colas
    printf("\n8. Combinación de colas:\n");
    Cola* cola_combinada = combinar_colas(cola, cola_limitada);
    if (cola_combinada) {
        imprimir_cola_detallada(cola, "Cola 1");
        imprimir_cola_detallada(cola_limitada, "Cola 2");
        imprimir_cola_detallada(cola_combinada, "Cola combinada");
    }
    
    // Simulaciones prácticas
    printf("\n9. Simulaciones prácticas:\n");
    
    // Simulación de atención al cliente
    printf("a) Simulación de atención al cliente:\n");
    int tiempos_llegada[] = {0, 2, 3, 5, 8, 10};
    int tiempos_atencion[] = {3, 2, 4, 1, 3, 2};
    simular_atencion_clientes(tiempos_llegada, tiempos_atencion, 6);
    
    // Simulación de buffer de impresión
    printf("\nb) Simulación de buffer de impresión:\n");
    int documentos[] = {101, 102, 103, 104, 105};
    simular_buffer_impresion(documentos, 5);
    
    // Simulación de scheduler FIFO
    printf("\nc) Simulación de scheduler FIFO:\n");
    int procesos[] = {1, 2, 3, 4, 5};
    int prioridades[] = {3, 1, 4, 2, 5}; // Para comparación
    simular_scheduler_fifo(procesos, prioridades, 5);
    
    // Comparación Cola vs Pila
    printf("\n10. Comparación Cola (FIFO) vs Pila (LIFO):\n");
    comparar_cola_vs_pila();
    
    // Validación y estadísticas
    printf("\n11. Validación y estadísticas:\n");
    printf("¿Cola original válida? %s\n", validar_integridad_cola(cola) ? "Sí" : "No");
    
    size_t tamaño, capacidad;
    double porcentaje;
    obtener_estadisticas(cola_limitada, &tamaño, &capacidad, &porcentaje);
    printf("Estadísticas cola limitada:\n");
    printf("  Tamaño: %zu\n", tamaño);
    printf("  Capacidad: %zu\n", capacidad);
    printf("  Uso: %.1f%%\n", porcentaje);
    
    // Análisis de rendimiento
    printf("\n12. Análisis de rendimiento:\n");
    analizar_rendimiento_cola();
    
    // Cleanup
    destruir_cola_simple(&cola_simple);
    destruir_cola(cola);
    destruir_cola(cola_clon);
    destruir_cola(cola_limitada);
    destruir_cola(cola_combinada);
    
    printf("\n=== Fin del ejercicio ===\n");
    return 0;
}
#endif // EXCLUDE_MAIN

// === IMPLEMENTACIÓN DE FUNCIONES ===

Cola* crear_cola(size_t capacidad_maxima) {
    Cola* cola = (Cola*)malloc(sizeof(Cola));
    if (!cola) return NULL;
    
    cola->frente = NULL;
    cola->final = NULL;
    cola->tamaño = 0;
    cola->capacidad_maxima = capacidad_maxima;
    
    return cola;
}

Nodo* crear_nodo(int valor) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return NULL;
    
    nuevo->valor = valor;
    nuevo->siguiente = NULL;
    
    return nuevo;
}

void destruir_cola(Cola* cola) {
    if (!cola) return;
    
    vaciar_cola(cola);
    free(cola);
}

void destruir_nodo(Nodo* nodo) {
    if (nodo) {
        free(nodo);
    }
}

// === OPERACIONES PRINCIPALES DE LA COLA ===

ResultadoCola enqueue(Cola* cola, int valor) {
    if (!cola) return COLA_ERROR_PARAMETRO;
    
    // Verificar si está llena (si tiene límite)
    if (cola->capacidad_maxima > 0 && cola->tamaño >= cola->capacidad_maxima) {
        return COLA_LLENA;
    }
    
    Nodo* nuevo = crear_nodo(valor);
    if (!nuevo) return COLA_ERROR_MEMORIA;
    
    if (cola->final != NULL) {
        cola->final->siguiente = nuevo;
    } else {
        // Cola vacía - el nuevo nodo es tanto frente como final
        cola->frente = nuevo;
    }
    cola->final = nuevo;
    cola->tamaño++;
    
    return COLA_EXITO;
}

ResultadoCola dequeue(Cola* cola, int* valor) {
    if (!cola || !valor) return COLA_ERROR_PARAMETRO;
    
    if (cola->frente == NULL) return COLA_VACIA;
    
    Nodo* temp = cola->frente;
    *valor = temp->valor;
    cola->frente = cola->frente->siguiente;
    cola->tamaño--;
    
    // Si la cola queda vacía, también actualizar final
    if (cola->frente == NULL) {
        cola->final = NULL;
    }
    
    destruir_nodo(temp);
    return COLA_EXITO;
}

ResultadoCola front(const Cola* cola, int* valor) {
    if (!cola || !valor) return COLA_ERROR_PARAMETRO;
    
    if (cola->frente == NULL) return COLA_VACIA;
    
    *valor = cola->frente->valor;
    return COLA_EXITO;
}

ResultadoCola back(const Cola* cola, int* valor) {
    if (!cola || !valor) return COLA_ERROR_PARAMETRO;
    
    if (cola->final == NULL) return COLA_VACIA;
    
    *valor = cola->final->valor;
    return COLA_EXITO;
}

// === OPERACIONES DE CONSULTA ===

bool esta_vacia(const Cola* cola) {
    return !cola || cola->frente == NULL;
}

bool esta_llena(const Cola* cola) {
    if (!cola || cola->capacidad_maxima == 0) return false;
    return cola->tamaño >= cola->capacidad_maxima;
}

size_t obtener_tamaño(const Cola* cola) {
    return cola ? cola->tamaño : 0;
}

size_t obtener_capacidad(const Cola* cola) {
    return cola ? cola->capacidad_maxima : 0;
}

bool contiene_valor(const Cola* cola, int valor) {
    if (!cola) return false;
    
    Nodo* actual = cola->frente;
    while (actual) {
        if (actual->valor == valor) return true;
        actual = actual->siguiente;
    }
    
    return false;
}

int obtener_posicion(const Cola* cola, int valor) {
    if (!cola) return -1;
    
    Nodo* actual = cola->frente;
    int posicion = 0;
    
    while (actual) {
        if (actual->valor == valor) return posicion;
        actual = actual->siguiente;
        posicion++;
    }
    
    return -1;
}

// === OPERACIONES AVANZADAS ===

void vaciar_cola(Cola* cola) {
    if (!cola) return;
    
    while (cola->frente) {
        Nodo* temp = cola->frente;
        cola->frente = cola->frente->siguiente;
        destruir_nodo(temp);
    }
    
    cola->final = NULL;
    cola->tamaño = 0;
}

Cola* clonar_cola(const Cola* cola_original) {
    if (!cola_original) return NULL;
    
    Cola* nueva_cola = crear_cola(cola_original->capacidad_maxima);
    if (!nueva_cola) return NULL;
    
    // Copiar elementos manteniendo el orden
    Nodo* actual = cola_original->frente;
    while (actual) {
        if (enqueue(nueva_cola, actual->valor) != COLA_EXITO) {
            destruir_cola(nueva_cola);
            return NULL;
        }
        actual = actual->siguiente;
    }
    
    return nueva_cola;
}

ResultadoCola invertir_cola(Cola* cola) {
    if (!cola) return COLA_ERROR_PARAMETRO;
    
    if (cola->tamaño <= 1) return COLA_EXITO;
    
    // Usar array temporal para invertir
    int* valores = malloc(cola->tamaño * sizeof(int));
    if (!valores) return COLA_ERROR_MEMORIA;
    
    // Extraer todos los elementos
    size_t count = 0;
    while (!esta_vacia(cola)) {
        dequeue(cola, &valores[count]);
        count++;
    }
    
    // Reinsertarlos en orden inverso
    for (int i = (int)count - 1; i >= 0; i--) {
        enqueue(cola, valores[i]);
    }
    
    free(valores);
    return COLA_EXITO;
}

ResultadoCola rotar_cola(Cola* cola, size_t n) {
    if (!cola) return COLA_ERROR_PARAMETRO;
    
    if (cola->tamaño <= 1 || n == 0) return COLA_EXITO;
    
    // Normalizar n al tamaño de la cola
    n = n % cola->tamaño;
    if (n == 0) return COLA_EXITO;
    
    // Extraer n elementos del frente y reinsertarlos al final
    int* valores = malloc(n * sizeof(int));
    if (!valores) return COLA_ERROR_MEMORIA;
    
    for (size_t i = 0; i < n; i++) {
        if (dequeue(cola, &valores[i]) != COLA_EXITO) {
            free(valores);
            return COLA_ERROR_PARAMETRO;
        }
    }
    
    for (size_t i = 0; i < n; i++) {
        if (enqueue(cola, valores[i]) != COLA_EXITO) {
            free(valores);
            return COLA_ERROR_PARAMETRO;
        }
    }
    
    free(valores);
    return COLA_EXITO;
}

Cola* combinar_colas(const Cola* cola1, const Cola* cola2) {
    if (!cola1 && !cola2) return NULL;
    
    Cola* cola_combinada = crear_cola(0); // Ilimitada
    if (!cola_combinada) return NULL;
    
    // Copiar elementos de la primera cola
    if (cola1) {
        Nodo* actual = cola1->frente;
        while (actual) {
            if (enqueue(cola_combinada, actual->valor) != COLA_EXITO) {
                destruir_cola(cola_combinada);
                return NULL;
            }
            actual = actual->siguiente;
        }
    }
    
    // Copiar elementos de la segunda cola
    if (cola2) {
        Nodo* actual = cola2->frente;
        while (actual) {
            if (enqueue(cola_combinada, actual->valor) != COLA_EXITO) {
                destruir_cola(cola_combinada);
                return NULL;
            }
            actual = actual->siguiente;
        }
    }
    
    return cola_combinada;
}

// === FUNCIONES DE VISUALIZACIÓN ===

void imprimir_cola(const Cola* cola) {
    if (!cola || esta_vacia(cola)) {
        printf("[]");
        return;
    }
    
    printf("[");
    Nodo* actual = cola->frente;
    while (actual) {
        printf("%d", actual->valor);
        if (actual->siguiente) {
            printf(" -> ");
        }
        actual = actual->siguiente;
    }
    printf("]");
}

void imprimir_cola_detallada(const Cola* cola, const char* titulo) {
    if (titulo) {
        printf("%s:\n", titulo);
    }
    printf("  Cola: ");
    imprimir_cola(cola);
    printf("\n  Tamaño: %zu", obtener_tamaño(cola));
    if (obtener_capacidad(cola) > 0) {
        printf("/%zu", obtener_capacidad(cola));
    }
    printf("\n");
    if (!esta_vacia(cola)) {
        int valor;
        if (front(cola, &valor) == COLA_EXITO) {
            printf("  Frente: %d\n", valor);
        }
        if (back(cola, &valor) == COLA_EXITO) {
            printf("  Final: %d\n", valor);
        }
    }
    printf("\n");
}

void imprimir_cola_visual(const Cola* cola) {
    if (!cola) {
        printf("Cola: NULL\n");
        return;
    }
    
    printf("Cola (visual):\n");
    if (esta_vacia(cola)) {
        printf("FRENTE -> [  vacía  ] <- FINAL\n");
    } else {
        printf("FRENTE -> [ ");
        Nodo* actual = cola->frente;
        while (actual) {
            printf("%d", actual->valor);
            if (actual->siguiente) {
                printf(" | ");
            }
            actual = actual->siguiente;
        }
        printf(" ] <- FINAL\n");
        
        int frente_val, final_val;
        front(cola, &frente_val);
        back(cola, &final_val);
        printf("          %d%*s%d\n", frente_val, 
               (int)(obtener_tamaño(cola) * 4 - 1), "", final_val);
    }
}

void imprimir_debug_cola(const Cola* cola) {
    if (!cola) {
        printf("Cola: NULL\n");
        return;
    }
    
    printf("=== DEBUG Cola ===\n");
    printf("Dirección cola: %p\n", (void*)cola);
    printf("Frente: %p\n", (void*)cola->frente);
    printf("Final: %p\n", (void*)cola->final);
    printf("Tamaño: %zu\n", cola->tamaño);
    printf("Capacidad máxima: %zu\n", cola->capacidad_maxima);
    
    if (cola->frente) {
        printf("Nodos:\n");
        Nodo* actual = cola->frente;
        size_t i = 0;
        while (actual) {
            printf("  [%zu] %p: valor=%d, siguiente=%p", 
                   i, (void*)actual, actual->valor, (void*)actual->siguiente);
            if (actual == cola->frente) printf(" <- FRENTE");
            if (actual == cola->final) printf(" <- FINAL");
            printf("\n");
            actual = actual->siguiente;
            i++;
        }
    }
    printf("=================\n");
}

// === FUNCIONES VERSIÓN SIMPLE ===

ResultadoCola enqueue_simple(ColaSimple* cola, int valor) {
    if (!cola) return COLA_ERROR_PARAMETRO;
    
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return COLA_ERROR_MEMORIA;
    
    nuevo->valor = valor;
    nuevo->siguiente = NULL;
    
    if (cola->final != NULL) {
        cola->final->siguiente = nuevo;
    } else {
        cola->frente = nuevo;
    }
    cola->final = nuevo;
    
    return COLA_EXITO;
}

ResultadoCola dequeue_simple(ColaSimple* cola, int* valor) {
    if (!cola || !valor) return COLA_ERROR_PARAMETRO;
    
    if (cola->frente == NULL) return COLA_VACIA;
    
    Nodo* temp = cola->frente;
    *valor = temp->valor;
    cola->frente = cola->frente->siguiente;
    
    if (cola->frente == NULL) {
        cola->final = NULL;
    }
    
    free(temp);
    return COLA_EXITO;
}

ResultadoCola front_simple(const ColaSimple* cola, int* valor) {
    if (!cola || !valor) return COLA_ERROR_PARAMETRO;
    
    if (cola->frente == NULL) return COLA_VACIA;
    
    *valor = cola->frente->valor;
    return COLA_EXITO;
}

void imprimir_cola_simple(const ColaSimple* cola) {
    if (!cola || !cola->frente) {
        printf("Cola: NULL\n");
        return;
    }
    
    printf("Cola: ");
    Nodo* actual = cola->frente;
    while (actual != NULL) {
        printf("%d -> ", actual->valor);
        actual = actual->siguiente;
    }
    printf("NULL\n");
}

void destruir_cola_simple(ColaSimple* cola) {
    if (!cola) return;
    
    while (cola->frente) {
        Nodo* temp = cola->frente;
        cola->frente = cola->frente->siguiente;
        free(temp);
    }
    cola->final = NULL;
}

// === FUNCIONES DE VALIDACIÓN ===

bool validar_integridad_cola(const Cola* cola) {
    if (!cola) return false;
    
    // Cola vacía
    if (cola->tamaño == 0) {
        return cola->frente == NULL && cola->final == NULL;
    }
    
    // Cola no vacía debe tener frente y final
    if (!cola->frente || !cola->final) return false;
    
    // Contar nodos manualmente
    size_t contador = 0;
    Nodo* actual = cola->frente;
    Nodo* ultimo = NULL;
    
    while (actual) {
        contador++;
        ultimo = actual;
        actual = actual->siguiente;
    }
    
    // Verificar que el contador coincida con el tamaño reportado
    if (contador != cola->tamaño) return false;
    
    // Verificar que el último nodo sea efectivamente el final
    if (ultimo != cola->final) return false;
    
    // Verificar límites de capacidad
    if (cola->capacidad_maxima > 0 && cola->tamaño > cola->capacidad_maxima) {
        return false;
    }
    
    return true;
}

bool colas_iguales(const Cola* cola1, const Cola* cola2) {
    if (!cola1 || !cola2) return cola1 == cola2;
    
    if (cola1->tamaño != cola2->tamaño) return false;
    
    Nodo* nodo1 = cola1->frente;
    Nodo* nodo2 = cola2->frente;
    
    while (nodo1 && nodo2) {
        if (nodo1->valor != nodo2->valor) return false;
        nodo1 = nodo1->siguiente;
        nodo2 = nodo2->siguiente;
    }
    
    return nodo1 == NULL && nodo2 == NULL;
}

// === SIMULACIONES Y APLICACIONES PRÁCTICAS ===

void simular_atencion_clientes(const int* tiempos_llegada, 
                              const int* tiempos_atencion, 
                              size_t num_clientes) {
    if (!tiempos_llegada || !tiempos_atencion) return;
    
    printf("Simulando sistema de atención al cliente:\n");
    Cola* cola_espera = crear_cola(0);
    
    int tiempo_actual = 0;
    int tiempo_fin_atencion = 0;
    int cliente_en_atencion = -1;
    
    for (size_t i = 0; i < num_clientes; i++) {
        tiempo_actual = tiempos_llegada[i];
        
        printf("Tiempo %d: Cliente %zu llega\n", tiempo_actual, i + 1);
        enqueue(cola_espera, (int)(i + 1));
        
        // Verificar si se puede atender al siguiente cliente
        if (cliente_en_atencion == -1 && !esta_vacia(cola_espera)) {
            int cliente;
            dequeue(cola_espera, &cliente);
            cliente_en_atencion = cliente;
            tiempo_fin_atencion = tiempo_actual + tiempos_atencion[cliente - 1];
            
            printf("  -> Cliente %d entra en atención (terminará en tiempo %d)\n", 
                   cliente, tiempo_fin_atencion);
        }
        
        printf("  Cola de espera: ");
        imprimir_cola(cola_espera);
        printf(" (tamaño: %zu)\n", obtener_tamaño(cola_espera));
        
        // Simular finalización de atención
        if (cliente_en_atencion != -1 && tiempo_actual >= tiempo_fin_atencion) {
            printf("  -> Cliente %d termina atención\n", cliente_en_atencion);
            cliente_en_atencion = -1;
        }
    }
    
    // Procesar clientes restantes
    while (!esta_vacia(cola_espera)) {
        int cliente;
        dequeue(cola_espera, &cliente);
        tiempo_actual += tiempos_atencion[cliente - 1];
        printf("Tiempo %d: Cliente %d termina atención\n", tiempo_actual, cliente);
    }
    
    destruir_cola(cola_espera);
    printf("Simulación completada.\n\n");
}

void simular_buffer_impresion(const int* documentos, size_t num_documentos) {
    if (!documentos) return;
    
    printf("Simulando buffer de impresión:\n");
    Cola* buffer = crear_cola(5); // Buffer limitado
    
    printf("Agregando documentos al buffer:\n");
    for (size_t i = 0; i < num_documentos; i++) {
        ResultadoCola resultado = enqueue(buffer, documentos[i]);
        printf("  Documento %d: %s\n", documentos[i], resultado_a_cadena(resultado));
        
        if (resultado == COLA_EXITO) {
            printf("    Buffer: ");
            imprimir_cola(buffer);
            printf(" (%zu/%zu)\n", obtener_tamaño(buffer), obtener_capacidad(buffer));
        }
    }
    
    printf("\nProcesando impresiones:\n");
    while (!esta_vacia(buffer)) {
        int documento;
        dequeue(buffer, &documento);
        printf("  Imprimiendo documento %d...\n", documento);
        
        // Simular tiempo de impresión
        printf("    Buffer restante: ");
        imprimir_cola(buffer);
        printf(" (%zu elementos)\n", obtener_tamaño(buffer));
    }
    
    destruir_cola(buffer);
    printf("Buffer de impresión vaciado.\n\n");
}

void simular_scheduler_fifo(const int* procesos, const int* prioridades, 
                           size_t num_procesos) {
    if (!procesos) return;
    
    printf("Simulando scheduler FIFO vs Prioridades:\n");
    Cola* cola_fifo = crear_cola(0);
    
    printf("Agregando procesos a la cola FIFO:\n");
    for (size_t i = 0; i < num_procesos; i++) {
        enqueue(cola_fifo, procesos[i]);
        printf("  Proceso %d (prioridad %d) agregado\n", procesos[i], prioridades[i]);
    }
    
    printf("\nEjecución FIFO (First In, First Out):\n");
    int tiempo = 0;
    while (!esta_vacia(cola_fifo)) {
        int proceso;
        dequeue(cola_fifo, &proceso);
        printf("  Tiempo %d: Ejecutando proceso %d\n", tiempo++, proceso);
    }
    
    printf("\nComparación: En un scheduler por prioridades, el orden sería diferente\n");
    printf("(ordenado por prioridad ascendente):\n");
    
    // Mostrar orden por prioridades (solo para comparación)
    for (int prio = 1; prio <= 5; prio++) {
        for (size_t i = 0; i < num_procesos; i++) {
            if (prioridades[i] == prio) {
                printf("  Proceso %d (prioridad %d)\n", procesos[i], prioridades[i]);
            }
        }
    }
    
    destruir_cola(cola_fifo);
    printf("\n");
}

void simular_bfs(int** grafo, size_t num_vertices, int inicio) {
    if (!grafo) return;
    
    printf("Simulando algoritmo BFS desde vértice %d:\n", inicio);
    Cola* cola_bfs = crear_cola(0);
    bool* visitado = calloc(num_vertices, sizeof(bool));
    
    if (!visitado) {
        destruir_cola(cola_bfs);
        return;
    }
    
    enqueue(cola_bfs, inicio);
    visitado[inicio] = true;
    
    printf("Orden de visita: ");
    while (!esta_vacia(cola_bfs)) {
        int vertice_actual;
        dequeue(cola_bfs, &vertice_actual);
        printf("%d ", vertice_actual);
        
        // Agregar vecinos no visitados
        for (size_t i = 0; i < num_vertices; i++) {
            if (grafo[vertice_actual][i] && !visitado[i]) {
                enqueue(cola_bfs, (int)i);
                visitado[i] = true;
            }
        }
    }
    printf("\n\n");
    
    free(visitado);
    destruir_cola(cola_bfs);
}

// === FUNCIONES EDUCATIVAS ===

void analizar_rendimiento_cola(void) {
    printf("Análisis de rendimiento de operaciones de cola:\n");
    printf("- enqueue(): O(1) - Inserción al final\n");
    printf("- dequeue(): O(1) - Eliminación del frente\n");
    printf("- front(): O(1) - Consulta directa del frente\n");
    printf("- back(): O(1) - Consulta directa del final\n");
    printf("- esta_vacia(): O(1) - Verificación directa\n");
    printf("- obtener_tamaño(): O(1) - Acceso a metadato\n");
    printf("- invertir_cola(): O(n) - Requiere procesamiento completo\n");
    printf("- rotar_cola(): O(k) - Donde k es el número de rotaciones\n");
    printf("- clonar_cola(): O(n) - Copiar todos los elementos\n");
    printf("- contiene_valor(): O(n) - Búsqueda lineal\n\n");
    
    printf("Ventajas de implementación con lista enlazada:\n");
    printf("- Tamaño dinámico (limitado solo por memoria disponible)\n");
    printf("- Operaciones principales muy eficientes O(1)\n");
    printf("- No hay despericio de memoria\n");
    printf("- Mantiene dos punteros para acceso directo a extremos\n\n");
    
    printf("Desventajas vs implementación con array circular:\n");
    printf("- Overhead de punteros en cada nodo\n");
    printf("- Peor localidad de referencia\n");
    printf("- Fragmentación potencial de memoria\n");
}

void comparar_cola_vs_pila(void) {
    printf("Comparación Cola (FIFO) vs Pila (LIFO):\n");
    
    // Crear cola y pila con los mismos elementos
    Cola* cola = crear_cola(0);
    // Simulamos pila con cola para demostración
    Cola* pila_simulada = crear_cola(0);
    
    printf("Insertando secuencia: 1, 2, 3, 4, 5\n\n");
    
    // Insertar en cola
    for (int i = 1; i <= 5; i++) {
        enqueue(cola, i);
    }
    
    // Simular pila (insertar al frente usando lógica de pila)
    for (int i = 1; i <= 5; i++) {
        enqueue(pila_simulada, i);
    }
    
    printf("Extracción Cola (FIFO - First In, First Out):\n");
    while (!esta_vacia(cola)) {
        int valor;
        dequeue(cola, &valor);
        printf("  %d", valor);
    }
    printf("\n\n");
    
    printf("Extracción Pila (LIFO - Last In, First Out):\n");
    // Para simular LIFO, invertimos la cola
    invertir_cola(pila_simulada);
    while (!esta_vacia(pila_simulada)) {
        int valor;
        dequeue(pila_simulada, &valor);
        printf("  %d", valor);
    }
    printf("\n\n");
    
    printf("Casos de uso:\n");
    printf("Cola (FIFO):\n");
    printf("- Sistemas de atención al cliente\n");
    printf("- Buffers de comunicación\n");
    printf("- Algoritmo BFS\n");
    printf("- Scheduler de procesos\n\n");
    
    printf("Pila (LIFO):\n");
    printf("- Call stack de funciones\n");
    printf("- Operaciones de deshacer\n");
    printf("- Algoritmo DFS\n");
    printf("- Evaluación de expresiones\n\n");
    
    destruir_cola(cola);
    destruir_cola(pila_simulada);
}

// === FUNCIONES DE UTILIDAD ===

const char* resultado_a_cadena(ResultadoCola resultado) {
    switch (resultado) {
        case COLA_EXITO: return "Éxito";
        case COLA_ERROR_MEMORIA: return "Error de memoria";
        case COLA_VACIA: return "Cola vacía";
        case COLA_LLENA: return "Cola llena";
        case COLA_ERROR_PARAMETRO: return "Parámetro inválido";
        default: return "Error desconocido";
    }
}

void obtener_estadisticas(const Cola* cola, size_t* tamaño_actual, 
                         size_t* capacidad, double* porcentaje_uso) {
    if (!cola || !tamaño_actual || !capacidad || !porcentaje_uso) return;
    
    *tamaño_actual = cola->tamaño;
    *capacidad = cola->capacidad_maxima;
    
    if (cola->capacidad_maxima > 0) {
        *porcentaje_uso = (double)cola->tamaño / cola->capacidad_maxima * 100.0;
    } else {
        *porcentaje_uso = 0.0; // Cola ilimitada
    }
}

size_t cola_a_array(const Cola* cola, int* array, size_t tamaño_max) {
    if (!cola || !array || tamaño_max == 0) return 0;
    
    size_t count = 0;
    Nodo* actual = cola->frente;
    
    while (actual && count < tamaño_max) {
        array[count] = actual->valor;
        actual = actual->siguiente;
        count++;
    }
    
    return count;
}

Cola* array_a_cola(const int* array, size_t tamaño, size_t capacidad_maxima) {
    if (!array || tamaño == 0) return NULL;
    
    Cola* cola = crear_cola(capacidad_maxima);
    if (!cola) return NULL;
    
    for (size_t i = 0; i < tamaño; i++) {
        if (enqueue(cola, array[i]) != COLA_EXITO) {
            destruir_cola(cola);
            return NULL;
        }
    }
    
    return cola;
}

// === FUNCIONES DE FILTRADO Y TRANSFORMACIÓN ===

Cola* filtrar_cola(const Cola* cola, bool (*predicado)(int)) {
    if (!cola || !predicado) return NULL;
    
    Cola* cola_filtrada = crear_cola(cola->capacidad_maxima);
    if (!cola_filtrada) return NULL;
    
    Nodo* actual = cola->frente;
    while (actual) {
        if (predicado(actual->valor)) {
            if (enqueue(cola_filtrada, actual->valor) != COLA_EXITO) {
                destruir_cola(cola_filtrada);
                return NULL;
            }
        }
        actual = actual->siguiente;
    }
    
    return cola_filtrada;
}

Cola* transformar_cola(const Cola* cola, int (*transformador)(int)) {
    if (!cola || !transformador) return NULL;
    
    Cola* cola_transformada = crear_cola(cola->capacidad_maxima);
    if (!cola_transformada) return NULL;
    
    Nodo* actual = cola->frente;
    while (actual) {
        int valor_transformado = transformador(actual->valor);
        if (enqueue(cola_transformada, valor_transformado) != COLA_EXITO) {
            destruir_cola(cola_transformada);
            return NULL;
        }
        actual = actual->siguiente;
    }
    
    return cola_transformada;
}

ResultadoCola reducir_cola(const Cola* cola, int (*reductor)(int, int), 
                          int valor_inicial, int* resultado) {
    if (!cola || !reductor || !resultado) return COLA_ERROR_PARAMETRO;
    
    *resultado = valor_inicial;
    
    Nodo* actual = cola->frente;
    while (actual) {
        *resultado = reductor(*resultado, actual->valor);
        actual = actual->siguiente;
    }
    
    return COLA_EXITO;
}
