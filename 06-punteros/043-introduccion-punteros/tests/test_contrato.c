#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
void modificar_via_puntero(int *, int);
void intercambiar(int *, int *);
int validar_puntero(int *);
int main(void) {
    int a = 7, b = -3;
    modificar_via_puntero(&a, 91);
    assert(a == 91 && b == -3);
    intercambiar(&a, &b);
    assert(a == -3 && b == 91);
    assert(validar_puntero(NULL) == 0);
    assert(validar_puntero(&a) == 1);
    return 0;
}
