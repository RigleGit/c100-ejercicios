#ifdef NDEBUG
#undef NDEBUG
#endif
#include "cola_lista.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Cola *c = crear_cola(2);
    int v;
    assert(c && dequeue(c, &v) == COLA_VACIA);
    assert(enqueue(c, 10) == COLA_EXITO);
    assert(enqueue(c, 20) == COLA_EXITO);
    assert(enqueue(c, 30) == COLA_LLENA);
    assert(front(c, &v) == COLA_EXITO && v == 10);
    assert(back(c, &v) == COLA_EXITO && v == 20);
    assert(dequeue(c, &v) == COLA_EXITO && v == 10);
    assert(dequeue(c, &v) == COLA_EXITO && v == 20);
    assert(esta_vacia(c));
    assert(enqueue(c, 40) == COLA_EXITO);
    assert(dequeue(c, &v) == COLA_EXITO && v == 40);
    destruir_cola(c);
    return 0;
}
