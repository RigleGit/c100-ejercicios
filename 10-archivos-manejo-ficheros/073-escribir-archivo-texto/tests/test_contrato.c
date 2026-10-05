#ifdef NDEBUG
#undef NDEBUG
#endif
#include "escribir_archivo.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void bytes(const char *name, const void *expected, size_t n) {
    FILE *f = fopen(name, "rb");
    assert(f);
    unsigned char b[128];
    assert(n <= sizeof b);
    assert(fread(b, 1, n, f) == n);
    assert(memcmp(b, expected, n) == 0);
    assert(fgetc(f) == EOF);
    assert(fclose(f) == 0);
}
int main(void) {
    const char *p = "c100_073_fixture.txt";
    assert(escribir_archivo_texto(p, "Hola\n").exito);
    bytes(p, "Hola\n", 5);
    assert(anexar_contenido(p, "C"));
    bytes(p, "Hola\nC", 6);
    const char *lines[] = {"uno", "", "tres"};
    assert(escribir_multiples_lineas(p, lines, 3).exito);
    bytes(p, "uno\n\ntres\n", 10);
    assert(!escribir_archivo_texto(NULL, "x").exito);
    assert(escribir_archivo_texto(p, "").exito);
    bytes(p, "", 0);
    assert(remove(p) == 0);
    return 0;
}
