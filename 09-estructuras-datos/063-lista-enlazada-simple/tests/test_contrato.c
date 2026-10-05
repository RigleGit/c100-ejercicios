#ifdef NDEBUG
#undef NDEBUG
#endif
#include "lista_enlazada.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Lista *l = crear_lista();
    assert(l && esta_vacia(l));
    assert(insertar_final(l, 20));
    assert(insertar_inicio(l, 10));
    assert(insertar_en_posicion(l, 1, 15));
    assert(obtener_longitud(l) == 3);
    const int e[] = {10, 15, 20};
    for (size_t i = 0; i < 3; i++)
        assert(obtener_en_posicion(l, i)->valor == e[i]);
    assert(obtener_en_posicion(l, 3) == NULL);
    assert(contiene_valor(l, 15) && !contiene_valor(l, 99));
    invertir_lista(l);
    const int r[] = {20, 15, 10};
    for (size_t i = 0; i < 3; i++)
        assert(obtener_en_posicion(l, i)->valor == r[i]);
    int last;
    assert(obtener_ultimo(l, &last) && last == 10);
    destruir_lista(l);
    return 0;
}
