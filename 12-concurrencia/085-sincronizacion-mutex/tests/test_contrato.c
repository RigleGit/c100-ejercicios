#include "sincronizacion_mutex.h"
#include <assert.h>
#include <unistd.h>
static void *worker(void *arg) {
    recurso_compartido_t *r = arg;
    for (int i = 0; i < 1000; ++i)
        assert(incrementar_recurso_seguro(r, 1));
    return NULL;
}
int main(void) {
    alarm(10);
    recurso_compartido_t r = {0};
    int64_t value;
    pthread_t threads[4];
    assert(!inicializar_recurso_compartido(NULL, 0));
    assert(!leer_recurso_seguro(&r, &value));
    assert(inicializar_recurso_compartido(&r, 17));
    for (int i = 0; i < 4; ++i)
        assert(pthread_create(&threads[i], NULL, worker, &r) == 0);
    for (int i = 0; i < 4; ++i)
        assert(pthread_join(threads[i], NULL) == 0);
    assert(leer_recurso_seguro(&r, &value) && value == 4017);
    assert(establecer_recurso_seguro(&r, -8));
    assert(leer_recurso_seguro(&r, &value) && value == -8);
    assert(!leer_recurso_seguro(&r, NULL));
    assert(destruir_recurso_compartido(&r));
    assert(!incrementar_recurso_seguro(&r, 1));
    return 0;
}
