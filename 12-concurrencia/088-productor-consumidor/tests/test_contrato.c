#include "productor_consumidor.h"
#include <assert.h>
#include <unistd.h>
static void *produce(void *arg) {
    for (int i = 1; i <= 1000; ++i)
        assert(producir_item(arg, i) == 0);
    return NULL;
}
static void *consume(void *arg) {
    for (int i = 1; i <= 1000; ++i) {
        int value = 0;
        assert(consumir_item(arg, &value) == 0);
        assert(value == i);
    }
    return NULL;
}
int main(void) {
    alarm(10);
    buffer_circular_t b;
    pthread_t producer, consumer;
    assert(!inicializar_buffer_circular(NULL, 4));
    assert(!inicializar_buffer_circular(&b, 0));
    assert(inicializar_buffer_circular(&b, 3));
    assert(producir_item(NULL, 1) == -1);
    assert(consumir_item(&b, NULL) == -1);
    assert(pthread_create(&consumer, NULL, consume, &b) == 0);
    assert(pthread_create(&producer, NULL, produce, &b) == 0);
    assert(pthread_join(producer, NULL) == 0);
    assert(pthread_join(consumer, NULL) == 0);
    limpiar_buffer_circular(&b);
    return 0;
}
