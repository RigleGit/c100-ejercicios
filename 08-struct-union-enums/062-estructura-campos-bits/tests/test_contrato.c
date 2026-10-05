#ifdef NDEBUG
#undef NDEBUG
#endif
#include "bitfields_estado.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Estado e;
    configurar_estado_inicial(&e);
    assert(esta_operativo(&e));
    reportar_error(&e);
    assert(hay_error(&e) && !esta_conectado(&e) && !esta_operativo(&e));
    conectar(&e);
    assert(!esta_conectado(&e));
    limpiar_error(&e);
    conectar(&e);
    assert(esta_operativo(&e));
    configurar_con_mascara(&e, 9);
    assert(esta_activo(&e) && esta_en_debug(&e) && !esta_conectado(&e) && !hay_error(&e));
    assert(estado_a_mascara(&e) == 9);
    return 0;
}
