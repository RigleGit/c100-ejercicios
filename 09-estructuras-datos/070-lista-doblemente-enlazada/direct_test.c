#include <stdio.h>
#include <string.h>
#include "include/lista_doble.h"

int main(void) {
    printf("=== Testing empty list output ===\n");
    
    ListaDoble* lista = lista_crear();
    
    // Test empty list
    printf("Calling lista_imprimir_adelante:\n");
    lista_imprimir_adelante(lista);
    printf("Calling lista_imprimir_atras:\n");
    lista_imprimir_atras(lista);
    
    printf("\n=== Testing what the test expects ===\n");
    printf("Expected string: 'Lista vacía\\nLista vacía\\n'\n");
    printf("Expected string with newlines shown:\n");
    printf("Lista vacía\n");
    printf("Lista vacía\n");
    printf("(end of expected string)\n");
    
    printf("\n=== Testing list with elements output ===\n");
    
    // Add elements
    int valores[] = {10, 20, 30};
    lista_insertar_array(lista, valores, 3, true);
    
    printf("Calling lista_imprimir_adelante with elements:\n");
    lista_imprimir_adelante(lista);
    printf("Calling lista_imprimir_atras with elements:\n");
    lista_imprimir_atras(lista);
    
    printf("\n=== Testing what the test expects for elements ===\n");
    printf("Expected string: 'Recorrido hacia adelante: 10 <-> 20 <-> 30\\nRecorrido hacia atrás: 30 <-> 20 <-> 10\\n'\n");
    printf("Expected string with newlines shown:\n");
    printf("Recorrido hacia adelante: 10 <-> 20 <-> 30\n");
    printf("Recorrido hacia atrás: 30 <-> 20 <-> 10\n");
    printf("(end of expected string)\n");
    
    lista_destruir(lista);
    
    return 0;
}