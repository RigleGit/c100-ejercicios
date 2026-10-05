#ifdef NDEBUG
#undef NDEBUG
#endif
#include "array_dinamico_cadenas.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(crear_array_cadenas(0) == NULL);
    ArrayCadenas *a = crear_array_cadenas(1);
    assert(a);
    char name[] = "Zoe";
    assert(agregar_nombre(a, name));
    name[0] = 'X';
    assert(agregar_nombre(a, "Ana"));
    assert(agregar_nombre(a, "Luis"));
    ordenar_nombres(a);
    assert(strcmp(obtener_nombre(a, 0), "Ana") == 0);
    assert(strcmp(obtener_nombre(a, 2), "Zoe") == 0);
    assert(buscar_nombre(a, "Luis") == 1);
    assert(buscar_nombre(a, "Xoe") == -1);
    assert(eliminar_nombre_por_indice(a, 1));
    assert(a->size == 2 && strcmp(obtener_nombre(a, 1), "Zoe") == 0);
    assert(!eliminar_nombre_por_indice(a, 2));
    assert(!agregar_nombre(a, ""));
    liberar_array_cadenas(a);
    return 0;
}
