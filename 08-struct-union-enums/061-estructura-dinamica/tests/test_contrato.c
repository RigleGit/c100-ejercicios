#ifdef NDEBUG
#undef NDEBUG
#endif
#include "estructura_dinamica.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    ArrayPersonas *a = crear_array_personas();
    assert(a);
    char nombre[] = "Ana";
    assert(agregar_persona(a, nombre, "a@x", 20, 1000));
    nombre[0] = 'X';
    assert(agregar_persona(a, "Luis", "l@x", 40, 3000));
    assert(strcmp(a->personas[0].nombre, "Ana") == 0);
    EstadisticasPersonas s = calcular_estadisticas(a);
    assert(s.total_personas == 2 && s.edad_promedio == 30 && s.salario_promedio == 2000);
    ArrayPersonas *f = filtrar_por_edad(a, 20, 20);
    assert(f && f->size == 1 && strcmp(f->personas[0].nombre, "Ana") == 0);
    assert(actualizar_nombre_persona(&a->personas[0], "Cambio"));
    assert(strcmp(f->personas[0].nombre, "Ana") == 0);
    assert(eliminar_persona_por_indice(a, 0));
    assert(a->size == 1 && strcmp(a->personas[0].nombre, "Luis") == 0);
    assert(!eliminar_persona_por_indice(a, 1));
    liberar_array_personas(f);
    liberar_array_personas(a);
    return 0;
}
