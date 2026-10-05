#ifdef NDEBUG
#undef NDEBUG
#endif
#include "copiar_archivo.h"
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
    const char *src = "c100_075_source.txt", *dst = "c100_075_copy.txt";
    const unsigned char content[] = {65, 10, 0, 66, 255, 67, 10};
    fixture(src, content, sizeof content);
    ResultadoCopia r = copiar_por_bloques(src, dst, 2);
    assert(r.exito && r.bytes_copiados == 7);
    bytes(dst, content, 7);
    assert(!copiar_por_bloques(src, dst, 0).exito);
    fixture(src, "uno\ndos", 7);
    r = copiar_archivo_basico(src, dst);
    assert(r.exito && r.bytes_copiados == 7);
    bytes(dst, "uno\ndos", 7);
    assert(!copiar_archivo_basico(src, src).exito);
    bytes(src, "uno\ndos", 7);
    assert(remove(src) == 0);
    assert(!copiar_archivo_basico(src, dst).exito);
    bytes(dst, "uno\ndos", 7);
    fixture(src, "", 0);
    r = copiar_por_bloques(src, dst, 3);
    assert(r.exito && r.bytes_copiados == 0);
    bytes(dst, "", 0);
    assert(remove(src) == 0 && remove(dst) == 0);
    assert(!copiar_por_bloques(src, dst, 3).exito);
    return 0;
}
