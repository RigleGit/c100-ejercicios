#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int *crearEntero(int);
int modificarEntero(int *, int);
void liberarEntero(int **);
int main(void) {
    int *p = crearEntero(-17);
    assert(p && *p == -17);
    assert(modificarEntero(p, 0) == 1 && *p == 0);
    assert(modificarEntero(NULL, 7) == 0);
    liberarEntero(&p);
    assert(p == NULL);
    liberarEntero(&p);
    liberarEntero(NULL);
    return 0;
}
