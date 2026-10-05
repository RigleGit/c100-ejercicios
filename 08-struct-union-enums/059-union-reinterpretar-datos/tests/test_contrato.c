#ifdef NDEBUG
#undef NDEBUG
#endif
#include "union_reinterpretar.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    char b[9], h[11];
    byte_a_binario(0xA5, b);
    assert(strcmp(b, "10100101") == 0);
    byte_a_binario(0, b);
    assert(strcmp(b, "00000000") == 0);
    entero_a_hex(0x1234ABCD, h, sizeof h);
    assert(strcmp(h, "0x1234ABCD") == 0);
    assert(intercambiar_endianness(0x12345678) == 0x78563412);
    union Datos d = crear_datos(0x12345678);
    unsigned char raw[4];
    memcpy(raw, &d.entero, 4);
    if (es_little_endian()) {
        assert(raw[0] == 0x78 && raw[3] == 0x12);
    } else {
        assert(raw[0] == 0x12 && raw[3] == 0x78);
    }
    return 0;
}
