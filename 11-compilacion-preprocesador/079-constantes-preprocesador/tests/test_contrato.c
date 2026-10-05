#ifdef NDEBUG
#undef NDEBUG
#endif
#include "constantes_preprocesador.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    int a = 2, b = -4;
    SWAP(a, b);
    assert(a == -4 && b == 2);
    assert(MAX(2 + 3, 7) == 7 && MIN(2 + 3, 7) == 5);
    assert(ABS(-7) == 7);
    double area = AREA_CIRCULO_UNITARIO;
    assert(fabs(area - 3.141592653589793) < 1e-10);
    unsigned int flags = 0;
    flags |= BIT_2;
    flags |= BIT_7;
    assert(flags == 132);
    flags &= MASCARA_INFERIOR;
    assert(flags == 4);
    int values[] = {3, 4, 5};
    assert(ARRAY_SIZE(values) == 3 && *(ARRAY_END(values) - 1) == 5);
    return 0;
}
