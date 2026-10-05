#ifdef NDEBUG
#undef NDEBUG
#endif
#include "estructura_persona.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Persona p[] = {crear_persona("Zoe", 40), crear_persona("Ana", 0), crear_persona("Luis", 20)};
    assert(validar_persona(&p[1]));
    Persona bad = crear_persona("", 18);
    assert(!validar_persona(&bad));
    bad = crear_persona("A", 151);
    assert(!validar_persona(&bad));
    assert(fabs(edad_promedio(p, 3) - 20.0) < 1e-12);
    ordenar_por_edad(p, 3);
    assert(strcmp(p[0].nombre, "Ana") == 0 && strcmp(p[2].nombre, "Zoe") == 0);
    assert(buscar_por_nombre(p, 3, "Luis") == 1);
    assert(contar_en_rango_edad(p, 3, 0, 20) == 2);
    return 0;
}
