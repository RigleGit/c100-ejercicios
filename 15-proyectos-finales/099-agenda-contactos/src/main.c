/**
 * @file main.c
 * @brief Programa principal de la agenda de contactos (API moderna)
 */

#include "contacto.h"
#include "persistencia.h"
#include "menu.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static nodo_t *g_agenda = NULL;

static void manejar_sig(int sig) {
    (void)sig;
    printf("\nInterrupción detectada. Guardando y saliendo...\n");
    crear_directorio_datos();
    guardar_contactos(g_agenda, "data/agenda.csv");
    liberar_lista(&g_agenda);
    exit(0);
}

#ifndef UNIT_TESTING
int main(int argc, char *argv[]) {
    configuracion_app_t config = {
        .auto_guardar = true,
        .confirmaciones = true,
        .mostrar_estadisticas_inicio = false,
        .formato_default = FORMATO_CSV,
        .directorio_datos = "data",
        .max_resultados_busqueda = 50
    };

    // Señales
    signal(SIGINT, manejar_sig);
    signal(SIGTERM, manejar_sig);

    // Args
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Uso: %s [--demo] [--version]\n", argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf("Agenda de Contactos v1.0\n");
            return 0;
        } else if (strcmp(argv[i], "--demo") == 0) {
            config.mostrar_estadisticas_inicio = true;
            // Sembrar algunos contactos de ejemplo si la agenda está vacía
            if (g_agenda == NULL) {
                contacto_t c1 = crear_contacto("Juan Pérez", "+34-123-456-789", "juan@example.com", "Calle Falsa 123", CATEGORIA_TRABAJO);
                contacto_t c2 = crear_contacto("María García", "+34-987-654-321", "maria@example.com", "Av. Central 45", CATEGORIA_FAMILIA);
                contacto_t c3 = crear_contacto("Carlos López", "+34-555-111-222", "carlos@example.com", "C/ Sol 7", CATEGORIA_AMIGOS);
                (void)agregar_contacto(&g_agenda, &c1);
                (void)agregar_contacto(&g_agenda, &c2);
                (void)agregar_contacto(&g_agenda, &c3);
            }
        }
    }

    crear_directorio_datos();
    mostrar_banner();

    // Intentar cargar datos previos
    if (archivo_existe("data/agenda.csv")) {
        (void)cargar_contactos(&g_agenda, "data/agenda.csv");
    }

    // Mostrar estadísticas iniciales en modo demo
    if (config.mostrar_estadisticas_inicio && g_agenda) {
        estadisticas_t st = obtener_estadisticas(g_agenda);
        imprimir_estadisticas(&st);
        printf("\n");
    }

    // Bucle principal
    bool continuar = true;
    while (continuar) {
        mostrar_menu_principal();
        continuar = procesar_opcion_menu(&g_agenda, &config);
    }

    // Salida
    if (config.auto_guardar) {
        guardar_contactos(g_agenda, "data/agenda.csv");
    }
    liberar_lista(&g_agenda);
    return 0;
}
#endif
