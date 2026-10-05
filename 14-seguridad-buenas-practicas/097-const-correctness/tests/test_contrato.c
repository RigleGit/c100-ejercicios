#include "const_correctness.h"
#include <assert.h>
#include <string.h>
int main(void) {
    const char text[] = "a b";
    char copy[sizeof(text)];
    memcpy(copy, text, sizeof(text));
    assert(longitud_cadena_correcto(text) == 3);
    assert(buscar_caracter_correcto(text, 'b') == 2);
    assert(memcmp(text, copy, sizeof(text)) == 0);
    const int values[] = {-8, -2, -10};
    assert(sumar_array_const_data(values, 3) == -20);
    assert(encontrar_maximo_const_completo(values, 3) == -2);
    char buffer[3];
    assert(!copiar_cadena_correcto(buffer, "abcd", sizeof(buffer)));
    assert(strcmp(buffer, "ab") == 0);
    configuracion_t cfg = {7, "original", 2.5, false};
    assert(!inicializar_configuracion(&cfg, 8, "otro", 2.5));
    assert(strcmp(cfg.nombre, "original") == 0 && !cfg.activo);
    assert(!inicializar_configuracion(&cfg, 7, "otro", 3.0));
    assert(inicializar_configuracion(&cfg, 7, "listo", 2.5));
    assert(cfg.id == 7 && cfg.factor_conversion == 2.5 && cfg.activo &&
           strcmp(cfg.nombre, "listo") == 0);
    return 0;
}
