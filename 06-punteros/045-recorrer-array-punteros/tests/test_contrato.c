#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
int suma_array_punteros(int *, size_t);
int suma_array_reverso(int *, size_t);
void encontrar_max_min_punteros(int *, size_t, int *, int *);
int contar_con_condicion(int *, size_t, int (*)(int));
int es_par(int);
void copiar_array_punteros(int *, int *, size_t);
int main(void) {
    int a[] = {-5, 8, 0, -2, 11}, c[5] = {0}, max = 0, min = 0;
    assert(suma_array_punteros(a, 5) == 12);
    assert(suma_array_reverso(a, 5) == 12);
    assert(suma_array_reverso(a, 0) == 0);
    encontrar_max_min_punteros(a, 5, &max, &min);
    assert(max == 11 && min == -5);
    assert(contar_con_condicion(a, 5, es_par) == 3);
    copiar_array_punteros(a, c, 5);
    const int expected[] = {-5, 8, 0, -2, 11};
    assert(memcmp(c, expected, sizeof c) == 0);
    return 0;
}
