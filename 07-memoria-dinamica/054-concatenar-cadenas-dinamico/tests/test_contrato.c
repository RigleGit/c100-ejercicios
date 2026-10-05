#ifdef NDEBUG
#undef NDEBUG
#endif
#include "concatenar_cadenas.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    char *s = concatenarCadenas("Hola ", "C");
    assert(s && strcmp(s, "Hola C") == 0);
    free(s);
    s = concatenarCadenas("", "");
    assert(s && s[0] == 0);
    free(s);
    assert(concatenarCadenas(NULL, "x") == NULL);
    CadenaDinamica *d = crearCadenaDinamica(1);
    assert(d);
    assert(asignarContenido(d, "abc"));
    assert(concatenarACadena(d, " def"));
    assert(strcmp(d->contenido, "abc def") == 0 && d->longitud == 7);
    assert(concatenarACadena(d, ""));
    assert(d->longitud == 7);
    liberarCadenaDinamica(d);
    return 0;
}
