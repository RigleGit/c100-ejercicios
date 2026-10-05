#ifdef NDEBUG
#undef NDEBUG
#endif
#include "escribir_registros_binario.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    const char *p = "c100_077_records.bin";
    const Producto products[] = {{101, 9.5f}, {202, 0.0f}};
    EstadisticasEscritura s;
    assert(escribir_productos_basico(products, 2, p, &s));
    assert(s.operacion_exitosa && s.elementos_escritos == 2 &&
           s.bytes_escritos == 2 * sizeof(Producto));
    FILE *f = fopen(p, "rb");
    assert(f);
    Producto read[3];
    assert(fread(read, sizeof *read, 3, f) == 2);
    assert(fclose(f) == 0);
    assert(read[0].codigo == 101 && read[0].precio == 9.5f && read[1].codigo == 202 &&
           read[1].precio == 0);
    Producto third = {303, 3.25f};
    assert(anexar_producto(&third, p));
    f = fopen(p, "rb");
    assert(f);
    assert(fread(read, sizeof *read, 3, f) == 3);
    assert(read[2].codigo == 303 && read[2].precio == 3.25f);
    assert(fclose(f) == 0);
    Producto bad = {0, 2};
    assert(!validar_producto(&bad));
    assert(!escribir_producto_individual(&bad, p, false));
    assert(!escribir_productos_basico(&bad, 1, p, &s));
    f = fopen(p, "rb");
    assert(f);
    assert(fread(read, sizeof *read, 3, f) == 3);
    assert(fclose(f) == 0);
    assert(remove(p) == 0);
    return 0;
}
