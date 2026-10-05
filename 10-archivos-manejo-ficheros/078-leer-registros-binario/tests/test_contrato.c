#ifdef NDEBUG
#undef NDEBUG
#endif
#include "leer_registros_binario.h"
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
    const char *p = "c100_078_records.bin";
    const Producto fixture_values[] = {{11, 1.25f}, {22, 2.5f}, {33, 0.0f}};
    fixture(p, fixture_values, sizeof fixture_values);
    Producto read[3];
    EstadisticasLectura s;
    assert(leer_productos_basico(p, read, 2, &s) == 2);
    assert(s.elementos_leidos == 2 && s.bytes_leidos == 2 * sizeof(Producto));
    assert(read[0].codigo == 11 && read[0].precio == 1.25f && read[1].codigo == 22 &&
           read[1].precio == 2.5f);
    assert(leer_producto_por_indice(p, 2, &read[0]));
    assert(read[0].codigo == 33 && read[0].precio == 0);
    assert(!leer_producto_por_indice(p, 3, &read[0]));
    ResultadoLectura dynamic;
    assert(leer_productos_dinamico(p, &dynamic));
    assert(dynamic.num_productos == 3 && dynamic.productos[2].codigo == 33);
    liberar_resultado_lectura(&dynamic);
    assert(remove(p) == 0);
    assert(leer_productos_basico(p, read, 3, &s) == 0 && !s.archivo_existe);
    return 0;
}
