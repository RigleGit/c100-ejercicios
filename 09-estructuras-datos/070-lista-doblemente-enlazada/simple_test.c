#include "include/lista_doble.h"
#include <stdio.h>

int main(void) {
    ListaDoble* lista = lista_crear();
    
    // Test 1: Empty list
    printf("=== Test 1: Empty list ===\n");
    lista_imprimir_adelante(lista);
    lista_imprimir_atras(lista);
    
    // Test 2: List with elements
    printf("\n=== Test 2: List with elements ===\n");
    int valores[] = {10, 20, 30};
    lista_insertar_array(lista, valores, 3, true);
    lista_imprimir_adelante(lista);
    lista_imprimir_atras(lista);
    
    lista_destruir(lista);
    return 0;
}