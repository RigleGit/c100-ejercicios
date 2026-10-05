#ifdef NDEBUG
#undef NDEBUG
#endif
#include "pila_lista.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Pila *p = crear_pila(2);
    int v = 99;
    assert(p && pop(p, &v) == PILA_VACIA);
    assert(push(p, 10) == PILA_EXITO);
    assert(push(p, 20) == PILA_EXITO);
    assert(push(p, 30) == PILA_LLENA);
    assert(obtener_tamaño(p) == 2);
    assert(peek(p, &v) == PILA_EXITO && v == 20);
    assert(pop(p, &v) == PILA_EXITO && v == 20);
    assert(pop(p, &v) == PILA_EXITO && v == 10);
    assert(esta_vacia(p) && pop(p, &v) == PILA_VACIA);
    destruir_pila(p);
    return 0;
}
