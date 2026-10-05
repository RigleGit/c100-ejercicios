#include "productor_consumidor.h"
#include <stdio.h>

enum { NUM_DATOS = 1000 };
typedef struct { buffer_circular_t *buffer; int correcto; } argumentos_t;

static void *producir(void *arg) {
    argumentos_t *a = arg;
    for (int i = 1; i <= NUM_DATOS; ++i) {
        if (producir_item(a->buffer, i) != 0) { a->correcto = 0; break; }
    }
    return NULL;
}
static void *consumir(void *arg) {
    argumentos_t *a = arg;
    for (int i = 1; i <= NUM_DATOS; ++i) {
        int valor;
        if (consumir_item(a->buffer, &valor) != 0 || valor != i) {
            a->correcto = 0;
            break;
        }
    }
    return NULL;
}
int main(void) {
    buffer_circular_t buffer;
    if (!inicializar_buffer_circular(&buffer, 8)) return 1;
    argumentos_t productor = {&buffer, 1}, consumidor = {&buffer, 1};
    pthread_t p, c;
    if (pthread_create(&c, NULL, consumir, &consumidor) != 0) {
        limpiar_buffer_circular(&buffer);
        return 1;
    }
    if (pthread_create(&p, NULL, producir, &productor) != 0) {
        /* El consumidor termina por el timeout definido en la API. */
        pthread_join(c, NULL);
        limpiar_buffer_circular(&buffer);
        return 1;
    }
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    limpiar_buffer_circular(&buffer);
    if (!productor.correcto || !consumidor.correcto) return 1;
    printf("Producidos y consumidos en orden: %d\n", NUM_DATOS);
    return 0;
}
