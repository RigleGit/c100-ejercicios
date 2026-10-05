#include "race_conditions.h"
#include <assert.h>
#include <math.h>
#include <unistd.h>
int main(void) {
    alarm(10);
    configuracion_experimento_t cfg = {4, 1000, false, 0, false};
    resultado_experimento_t r = {0};
    assert(!ejecutar_experimento_sincronizado(NULL, &r));
    assert(ejecutar_experimento_sincronizado(&cfg, &r));
    assert(r.valor_obtenido == 4000 && r.valor_esperado == 4000);
    assert(r.diferencia == 0 && r.porcentaje_error == 0.0);
    limpiar_resultado_experimento(&r);
    /* One worker has no data race; do not require lost increments with multiple workers. */
    cfg.num_hilos = 1;
    assert(ejecutar_experimento_race_condition(&cfg, &r));
    assert(r.valor_obtenido == 1000);
    limpiar_resultado_experimento(&r);
    const int values[] = {1, 3, 5};
    int min, max;
    double mean, deviation;
    calcular_estadisticas(values, 3, &min, &max, &mean, &deviation);
    assert(min == 1 && max == 5 && mean == 3);
    /* Population versus sample variance is not specified; assert only min/max/mean. */
    return 0;
}
