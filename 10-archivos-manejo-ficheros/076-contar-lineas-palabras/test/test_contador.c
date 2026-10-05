#include "contar_lineas_palabras.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    FILE *f = fopen("entrada-larga.txt", "wb");
    assert(f);
    for (int i = 0; i < 2000; ++i) assert(fputc('a', f) != EOF);
    assert(fputc('\n', f) != EOF);
    assert(fclose(f) == 0);
    EstadisticasArchivo s;
    assert(contar_palabras_robusto("entrada-larga.txt", &s));
    assert(s.lineas == 1 && s.palabras == 1 && s.caracteres == 2001);
    assert(s.bytes == 2001);
    f = fopen("entrada-larga.txt", "wb");
    assert(f);
    assert(fputs("uno dos", f) >= 0);
    assert(fclose(f) == 0);
    assert(contar_palabras_robusto("entrada-larga.txt", &s));
    assert(s.lineas == 0 && s.palabras == 2 && s.bytes == 7);
    assert(contar_archivo_basico("entrada-larga.txt", &s));
    assert(s.lineas == 0 && s.palabras == 2);
    f = fopen("entrada-larga.txt", "wb");
    assert(f && fclose(f) == 0);
    assert(contar_palabras_robusto("entrada-larga.txt", &s));
    assert(s.lineas == 0 && s.palabras == 0 && s.bytes == 0);
    assert(!contar_palabras_robusto(NULL, &s));
    remove("entrada-larga.txt");
    return 0;
}
