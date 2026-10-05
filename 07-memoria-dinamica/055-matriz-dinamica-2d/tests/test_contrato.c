#ifdef NDEBUG
#undef NDEBUG
#endif
#include "matriz_dinamica_2d.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    assert(crear_matriz_2d(0, 3) == NULL);
    assert(crear_matriz_2d(2, -1) == NULL);
    int **m = crear_matriz_2d(3, 4);
    assert(m);
    rellenar_automatico(m, 3, 4);
    const int e[3][4] = {{0, 0, 0, 0}, {0, 1, 2, 3}, {0, 2, 4, 6}};
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            assert(obtener_elemento(m, i, j) == e[i][j]);
    int **c = copiar_matriz(m, 3, 4);
    assert(c);
    establecer_elemento(m, 2, 3, 99);
    assert(obtener_elemento(c, 2, 3) == 6);
    liberar_matriz_2d(c, 3);
    liberar_matriz_2d(m, 3);
    liberar_matriz_2d(NULL, 0);
    return 0;
}
