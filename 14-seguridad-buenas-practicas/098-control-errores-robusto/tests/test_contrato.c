#include "control_errores_robusto.h"
#include <assert.h>
#include <string.h>
int main(void) {
    void *p = NULL;
    result_t r = asignar_memoria_seguro(16, &p);
    assert(r.success && r.error_code == ERROR_NONE && p);
    free(p);
    r = asignar_memoria_seguro(0, &p);
    assert(!r.success && r.error_code == ERROR_INVALID_INPUT);
    FILE *f = tmpfile();
    assert(f);
    const char data[] = "hello\n";
    r = escribir_datos_seguro(f, data, 6);
    assert(r.success);
    rewind(f);
    char buffer[16];
    r = leer_linea_seguro(f, buffer, sizeof(buffer));
    assert(r.success && strcmp(buffer, data) == 0);
    r = leer_linea_seguro(f, buffer, sizeof(buffer));
    assert(!r.success && r.error_code == ERROR_FILE_READ);
    r = escribir_datos_seguro(NULL, data, 6);
    assert(!r.success && r.error_code == ERROR_INVALID_INPUT);
    assert(fclose(f) == 0);
    r = abrir_archivo_seguro("/this/path/cannot/exist/c100.txt", "r", &f);
    assert(!r.success && r.error_code == ERROR_FILE_OPEN && f == NULL);
    return 0;
}
