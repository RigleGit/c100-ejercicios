#include "funciones_seguras_cadenas.h"
#include <assert.h>
#include <string.h>
int main(void) {
    struct {
        char b[5];
        unsigned char sentinel;
    } d = {{0}, 0x5a};
    assert(copia_segura(d.b, "abcd", sizeof(d.b), NULL) == CADENA_OK);
    assert(strcmp(d.b, "abcd") == 0 && d.sentinel == 0x5a);
    assert(copia_segura(d.b, "abcdef", sizeof(d.b), NULL) == CADENA_ERROR_CADENA_TRUNCADA);
    assert(strcmp(d.b, "abcd") == 0 && d.sentinel == 0x5a);
    strcpy(d.b, "a");
    assert(concatena_segura(d.b, "bcdef", sizeof(d.b), NULL) == CADENA_ERROR_CADENA_TRUNCADA);
    assert(strcmp(d.b, "abcd") == 0 && d.sentinel == 0x5a);
    assert(copia_segura(NULL, "x", 5, NULL) == CADENA_ERROR_PARAMETRO_NULO);
    char path[32];
    assert(construye_ruta_segura(path, sizeof(path), "/tmp/", "a.txt") == CADENA_OK);
    assert(strcmp(path, "/tmp/a.txt") == 0);
    assert(formatea_seguro(path, sizeof(path), "%s=%d", "total", 42) == CADENA_OK);
    assert(strcmp(path, "total=42") == 0);
    return 0;
}
