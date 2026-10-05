#ifdef NDEBUG
#undef NDEBUG
#endif
#include "array_estructuras.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    ListaPersonas l;
    inicializar_lista(&l);
    assert(l.cantidad == 0);
    Persona p = {"Persona", 20};
    for (int i = 0; i < 5; i++) {
        p.edad = i * 10;
        assert(agregar_persona(&l, &p));
    }
    assert(!agregar_persona(&l, &p) && l.cantidad == 5);
    assert(eliminar_persona(&l, 0));
    assert(l.cantidad == 4 && l.personas[0].edad == 10 && l.personas[3].edad == 40);
    assert(!eliminar_persona(&l, 4));
    p.edad = -1;
    assert(!agregar_persona(&l, &p));
    assert(contar_en_decada(&l.personas[0], l.cantidad, 20) == 1);
    return 0;
}
