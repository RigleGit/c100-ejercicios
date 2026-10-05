#include "suma_paralela_arrays.h"
#include <assert.h>
#include <limits.h>
#include <unistd.h>
int main(void) {
    alarm(10);
    const int values[] = {INT_MAX, INT_MAX, -9, 4, 2, -2, 8};
    const int64_t expected = (int64_t)INT_MAX * 2 + 3;
    assert(suma_secuencial(values, 7, NULL) == expected);
    const int counts[] = {1, 3, 20};
    for (size_t i = 0; i < 3; ++i) {
        configuracion_suma_t cfg = {values, 7, counts[i], false};
        resultado_suma_paralela_t r = {0};
        assert(suma_paralela(&cfg, &r));
        assert(r.suma_total == expected);
        limpiar_resultado_suma(&r);
    }
    configuracion_suma_t bad = {values, 0, 2, false};
    resultado_suma_paralela_t r = {0};
    assert(!suma_paralela(NULL, &r));
    assert(!suma_paralela(&bad, &r));
    bad.tamano = 7;
    bad.num_hilos = 0;
    assert(!suma_paralela(&bad, &r));
    bad.num_hilos = 2;
    bad.array = NULL;
    assert(!suma_paralela(&bad, &r));
    return 0;
}
