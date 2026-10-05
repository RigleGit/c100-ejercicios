#include "contacto.h"
#include "persistencia.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    nodo_t *lista = NULL;
    const char *caso = argc > 1 ? argv[1] : "control";
    const char *nombre = strcmp(caso, "comillas") == 0 ? "Ana \"QA\"" : "Ana";
    contacto_t c = crear_contacto(nombre, "123456789", "ana@example.com",
                                  "Calle \"A\", piso 2\nMadrid", CATEGORIA_PERSONAL);
    assert(agregar_contacto(&lista, &c) == RESULTADO_EXITO);
    if (strcmp(caso, "importacion") == 0) {
        assert(importar_csv(&lista, "archivo-inexistente.csv", true) != RESULTADO_EXITO);
        assert(lista && strcmp(lista->contacto.nombre, nombre) == 0);
    } else if (strcmp(caso, "escritura") == 0) {
        assert(guardar_contactos(lista, "/dev/full") != RESULTADO_EXITO);
    } else if (strcmp(caso, "invalido") == 0) {
        FILE *f = fopen("invalido.csv", "w");
        assert(f);
        fputs("nombre,telefono,email,categoria,direccion\n\"Otro\",\"123456789\",\"a@b.es\",\"Personal\",\"Calle\"\n\"fila rota\n", f);
        assert(fclose(f) == 0);
        assert(importar_csv(&lista, "invalido.csv", true) != RESULTADO_EXITO);
        assert(lista && !lista->siguiente && strcmp(lista->contacto.nombre, nombre) == 0);
        remove("invalido.csv");
    } else {
        assert(guardar_contactos(lista, "prueba.csv") == RESULTADO_EXITO);
        liberar_lista(&lista);
        assert(cargar_contactos(&lista, "prueba.csv") == RESULTADO_EXITO);
        assert(lista && !lista->siguiente);
        assert(strcmp(lista->contacto.nombre, nombre) == 0);
        assert(strcmp(lista->contacto.direccion, c.direccion) == 0);
        remove("prueba.csv");
    }
    liberar_lista(&lista);
    return 0;
}
