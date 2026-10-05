#include "hilos_basicos.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    pthread_t t;
    void *raw = NULL;
    int limite = 10;
    assert(pthread_create(&t, NULL, funcion_hilo_calculo, &limite) == 0);
    assert(pthread_join(t, &raw) == 0);
    assert(raw && *(long *)raw == 55);
    free(raw);
    hilo_params_t params = crear_parametros_hilo(7, "trabajo");
    params.iteraciones = 3;
    params.delay_ms = 0;
    assert(pthread_create(&t, NULL, funcion_hilo_con_parametros, &params) == 0);
    assert(pthread_join(t, &raw) == 0);
    hilo_resultado_t *r = raw;
    assert(r && r->exito && r->hilo_id == 7 && r->operaciones_realizadas == 3);
    liberar_resultado_hilo(r);
    assert(funcion_hilo_calculo(NULL) == NULL);
    assert(funcion_hilo_con_parametros(NULL) == NULL);
    assert(demostrar_multiples_hilos(0) != 0);
    return 0;
}
