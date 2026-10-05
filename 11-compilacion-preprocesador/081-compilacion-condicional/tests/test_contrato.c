#ifdef NDEBUG
#undef NDEBUG
#endif
#include "compilacion_condicional.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(operacion_que_puede_fallar(-1) == -1);
    assert(operacion_que_puede_fallar(0) == 0);
    assert(operacion_que_puede_fallar(3) == 12);
    assert(operacion_que_puede_fallar(1000) == 1001000);
    assert(operacion_que_puede_fallar(1001) == -2);
    void *p = asignar_memoria_instrumentada(8);
    assert(p);
    memset(p, 42, 8);
    assert(((unsigned char *)p)[7] == 42);
    liberar_memoria_instrumentada(p);
    const char *path = "c100_081_stdout.txt";
    assert(freopen(path, "w", stdout));
    procesar_datos(4);
    assert(fflush(stdout) == 0);
    assert(fclose(stdout) == 0);
    FILE *f = fopen(path, "r");
    assert(f);
    char text[256];
    size_t n = fread(text, 1, sizeof text - 1, f);
    text[n] = 0;
    assert(fclose(f) == 0);
#ifdef DEBUG
    assert(strcmp(text, "[DEBUG] Entrando en procesarDatos con x = 4\n[DEBUG] Resultado intermedio "
                        "= 11\nResultado final: 11\n") == 0);
#else
    assert(strcmp(text, "Resultado final: 11\n") == 0);
#endif
    assert(remove(path) == 0);
    return 0;
}
