#ifdef NDEBUG
#undef NDEBUG
#endif
#include "enumeracion.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(diaSiguiente(DOM) == LUN);
    assert(diaAnterior(LUN) == DOM);
    assert(diaSiguiente(MIE) == JUE);
    assert(esFinDeSemana(SAB) && !esFinDeSemana(VIE));
    assert(esDiaLaboral(LUN) && !esDiaLaboral(DOM));
    assert(!esDiaValido(0) && !esDiaValido(8));
    assert(diasDelMes(FEBRERO, false) == 28);
    assert(diasDelMes(FEBRERO, true) == 29);
    assert(diasDelMes(ABRIL, false) == 30);
    assert(diasDelMes(ENERO, false) == 31);
    return 0;
}
