#include "entrada_segura.h"
#include <assert.h>
#include <string.h>
static FILE *input(const char *s) {
    FILE *f = tmpfile();
    assert(f);
    assert(fputs(s, f) >= 0);
    rewind(f);
    return f;
}
int main(void) {
    char tiny[1] = {0};
    FILE *tiny_input = input("z\n");
    assert(entrada_segura_fgets(tiny, sizeof(tiny), tiny_input) == ENTRADA_ERROR);
    assert(fgetc(tiny_input) == 'z');
    assert(fclose(tiny_input) == 0);
    char buffer[5];
    FILE *f = input("abcdef\nxy\n");
    assert(entrada_segura_linea_limpia(buffer, sizeof(buffer), f) == ENTRADA_TRUNCADA);
    assert(strcmp(buffer, "abcd") == 0);
    assert(entrada_segura_linea_limpia(buffer, sizeof(buffer), f) == ENTRADA_EXITOSA);
    assert(strcmp(buffer, "xy") == 0);
    assert(entrada_segura_linea_limpia(buffer, sizeof(buffer), f) == ENTRADA_EOF);
    fclose(f);
    assert(entrada_segura_fgets(NULL, sizeof(buffer), stdin) == ENTRADA_ERROR);
    f = input("abc\n");
    estadisticas_entrada_t stats;
    inicializar_estadisticas(&stats);
    assert(entrada_segura_validada(buffer, sizeof(buffer), f, &stats) == ENTRADA_EXITOSA);
    assert(strcmp(buffer, "abc") == 0);
    assert(stats.caracteres_leidos == 3 && stats.lineas_procesadas == 1);
    assert(entrada_segura_validada(buffer, sizeof(buffer), f, &stats) == ENTRADA_EOF);
    assert(stats.caracteres_leidos == 0 && stats.lineas_procesadas == 1);
    fclose(f);
    return 0;
}
