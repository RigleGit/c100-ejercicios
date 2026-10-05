#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int *crearArray(int);
void llenarArrayAutomatico(int *, int, int);
int modificarElemento(int *, int, int, int);
int buscarValor(int *, int, int);
void liberarArray(int **, int);
int main(void) {
    assert(crearArray(0) == NULL);
    assert(crearArray(-2) == NULL);
    int *a = crearArray(4);
    assert(a);
    llenarArrayAutomatico(a, 4, 0);
    const int expected[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++)
        assert(a[i] == expected[i]);
    assert(modificarElemento(a, 4, 1, -9) == 1);
    assert(a[1] == -9);
    assert(modificarElemento(a, 4, 4, 88) == 0 && a[3] == 4);
    assert(buscarValor(a, 4, -9) == 1);
    assert(buscarValor(a, 4, 88) == -1);
    liberarArray(&a, 4);
    assert(a == NULL);
    return 0;
}
