#ifdef NDEBUG
#undef NDEBUG
#endif
#include "leer_archivo.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void fixture(const char *name, const void *data, size_t n) {
    FILE *f = fopen(name, "wb");
    assert(f);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}
int main(void) {
    const char *p = "c100_074_fixture.txt";
    fixture(p, "uno dos\n\nfin", 12);
    BufferLineas b = {0};
    ResultadoLectura r = cargar_archivo_en_memoria(p, &b);
    assert(r.exito && r.lineas_leidas == 3 && r.caracteres_leidos == 12);
    assert(b.num_lineas == 3);
    assert(strcmp(b.lineas[0], "uno dos\n") == 0 && strcmp(b.lineas[1], "\n") == 0 &&
           strcmp(b.lineas[2], "fin") == 0);
    liberar_buffer_lineas(&b);
    fixture(p, "", 0);
    r = cargar_archivo_en_memoria(p, &b);
    assert(r.exito && r.lineas_leidas == 0 && r.caracteres_leidos == 0);
    liberar_buffer_lineas(&b);
    assert(remove(p) == 0);
    r = cargar_archivo_en_memoria(p, &b);
    assert(!r.exito);
    return 0;
}
