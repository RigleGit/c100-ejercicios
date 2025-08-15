/**
 * Menú de Agenda - implementación moderna alineada con menu.h/contacto.h
 */

#include "menu.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mostrar_banner(void) {
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    AGENDA DE CONTACTOS                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

void mostrar_menu_principal(void) {
    printf("\n");
    printf("1) Agregar  2) Buscar  3) Eliminar  4) Listar  5) Modificar\n");
    printf("6) Favoritos  7) Estadísticas  8) Guardar  9) Cargar\n");
    printf("10) Exportar  11) Importar  12) Backup  13) Configuración  14) Ayuda\n");
    printf("0) Salir\n");
    printf("Seleccione una opción: ");
}

opcion_menu_t leer_opcion_menu(void) {
    char buf[32]; int v;
    if (!fgets(buf, sizeof buf, stdin)) return (opcion_menu_t)-1;
    if (sscanf(buf, "%d", &v) != 1) return (opcion_menu_t)-1;
    return (opcion_menu_t)v;
}

void pausar_para_continuar(void) { printf("\nPresione Enter para continuar..."); getchar(); }
void limpiar_pantalla(void) { printf("\033[2J\033[H"); }

bool leer_linea_segura(char *buffer, size_t tamaño) {
    if (!fgets(buffer, tamaño, stdin)) return false;
    size_t n = strlen(buffer); if (n && buffer[n-1] == '\n') buffer[n-1] = '\0';
    return true;
}

int leer_entero_seguro(const char *prompt, int min, int max) {
    char buf[32]; int v;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof buf, stdin)) continue;
        if (sscanf(buf, "%d", &v) == 1 && v >= min && v <= max) return v;
        printf("Valor inválido. Rango [%d-%d]\n", min, max);
    }
}

bool leer_string_no_vacio(const char *prompt, char *buffer, size_t tamaño) {
    while (1) {
        printf("%s", prompt);
        if (!leer_linea_segura(buffer, tamaño)) return false;
        if (buffer[0] != '\0') return true;
        printf("El campo es obligatorio.\n");
    }
}

bool leer_confirmacion(const char *prompt) {
    char r[8]; printf("%s (s/N): ", prompt);
    if (!fgets(r, sizeof r, stdin)) return false;
    return r[0] == 's' || r[0] == 'S';
}

categoria_t leer_categoria(void) {
    char c[32];
    printf("Categorías: personal, trabajo, familia, amigos, otros\n");
    while (1) {
        printf("Categoría: ");
        if (!leer_linea_segura(c, sizeof c)) continue;
        if (!strcasecmp(c, "personal")) return CATEGORIA_PERSONAL;
        if (!strcasecmp(c, "trabajo")) return CATEGORIA_TRABAJO;
        if (!strcasecmp(c, "familia")) return CATEGORIA_FAMILIA;
        if (!strcasecmp(c, "amigos")) return CATEGORIA_AMIGOS;
        if (!strcasecmp(c, "otros")) return CATEGORIA_OTROS;
        printf("Categoría inválida.\n");
    }
}

static contacto_t leer_contacto_interactivo(void) {
    char nombre[TAM_NOMBRE] = {0}, tel[TAM_TELEFONO] = {0}, email[TAM_EMAIL] = {0}, dir[TAM_DIRECCION] = {0};
    leer_string_no_vacio("Nombre: ", nombre, sizeof nombre);
    leer_string_no_vacio("Teléfono: ", tel, sizeof tel);
    printf("Email (opcional): "); leer_linea_segura(email, sizeof email);
    printf("Dirección (opcional): "); leer_linea_segura(dir, sizeof dir);
    categoria_t cat = leer_categoria();
    contacto_t c = crear_contacto(nombre, tel, email, dir, cat);
    return c;
}

void menu_agregar_contacto(nodo_t **agenda, const configuracion_app_t *config) {
    contacto_t c = leer_contacto_interactivo();
    resultado_t r = agregar_contacto(agenda, &c);
    if (r == RESULTADO_EXITO) printf("[OK] Contacto agregado\n"); else printf("[Error] %s\n", resultado_a_string(r));
    (void)config;
}

void menu_buscar_contacto(const nodo_t *agenda) {
    char nombre[TAM_NOMBRE]; if (!leer_string_no_vacio("Nombre: ", nombre, sizeof nombre)) return;
    const contacto_t *c = buscar_contacto_por_nombre(agenda, nombre);
    if (c) imprimir_contacto(c, true); else printf("No encontrado\n");
}

void menu_eliminar_contacto(nodo_t **agenda, const configuracion_app_t *config) {
    char nombre[TAM_NOMBRE]; if (!leer_string_no_vacio("Nombre: ", nombre, sizeof nombre)) return;
    if (!leer_confirmacion("¿Confirmar eliminación?")) return;
    resultado_t r = eliminar_contacto(agenda, nombre);
    if (r == RESULTADO_EXITO) printf("Eliminado\n"); else printf("Error: %s\n", resultado_a_string(r));
    (void)config;
}

void menu_listar_contactos(const nodo_t *agenda) { listar_contactos(agenda, true); }

void menu_modificar_contacto(nodo_t *agenda, const configuracion_app_t *config) {
    (void)config; char nombre[TAM_NOMBRE]; if (!leer_string_no_vacio("Nombre actual: ", nombre, sizeof nombre)) return;
    contacto_t nuevo = leer_contacto_interactivo();
    resultado_t r = modificar_contacto(agenda, nombre, &nuevo);
    if (r == RESULTADO_EXITO) printf("Modificado\n"); else printf("Error: %s\n", resultado_a_string(r));
}

void menu_favoritos(nodo_t *agenda) { listar_contactos_favoritos(agenda); }

void menu_estadisticas(const nodo_t *agenda) { estadisticas_t s = obtener_estadisticas(agenda); imprimir_estadisticas(&s); }

void menu_guardar(const nodo_t *agenda, const configuracion_app_t *config) {
    (void)config; char ruta[256] = NOMBRE_ARCHIVO_DEFAULT; printf("Archivo destino [%s]: ", ruta);
    char in[256]; if (leer_linea_segura(in, sizeof in) && in[0]) strncpy(ruta, in, sizeof ruta - 1);
    (void)guardar_contactos(agenda, ruta);
}

void menu_cargar(nodo_t **agenda, const configuracion_app_t *config) {
    (void)config; char ruta[256] = NOMBRE_ARCHIVO_DEFAULT; printf("Archivo a cargar [%s]: ", ruta);
    char in[256]; if (leer_linea_segura(in, sizeof in) && in[0]) strncpy(ruta, in, sizeof ruta - 1);
    (void)cargar_contactos(agenda, ruta);
}

void menu_exportar(const nodo_t *agenda) {
    char ruta[256] = FORMATO_EXPORTACION_CSV; printf("Exportar CSV ruta [%s]: ", ruta);
    char in[256]; if (leer_linea_segura(in, sizeof in) && in[0]) strncpy(ruta, in, sizeof ruta - 1);
    (void)exportar_csv(agenda, ruta);
}

void menu_importar(nodo_t **agenda, const configuracion_app_t *config) {
    (void)config; char ruta[256] = FORMATO_EXPORTACION_CSV; printf("Importar CSV ruta [%s]: ", ruta);
    char in[256]; if (leer_linea_segura(in, sizeof in) && in[0]) strncpy(ruta, in, sizeof ruta - 1);
    (void)importar_csv(agenda, ruta, false);
}

void menu_backup(const nodo_t *agenda) { (void)crear_backup(agenda); }

void menu_configuracion(configuracion_app_t *config) {
    printf("Auto-guardado: %s\n", config->auto_guardar ? "ON" : "OFF");
    if (leer_confirmacion("¿Alternar auto-guardado?")) config->auto_guardar = !config->auto_guardar;
}

void menu_ayuda(void) { printf("Use las opciones del menú para gestionar contactos.\n"); }

bool procesar_opcion_menu(nodo_t **agenda, configuracion_app_t *config) {
    opcion_menu_t op = leer_opcion_menu();
    switch (op) {
        case OPCION_AGREGAR: menu_agregar_contacto(agenda, config); break;
        case OPCION_BUSCAR: menu_buscar_contacto(*agenda); break;
        case OPCION_ELIMINAR: menu_eliminar_contacto(agenda, config); break;
        case OPCION_LISTAR: menu_listar_contactos(*agenda); break;
        case OPCION_MODIFICAR: menu_modificar_contacto(*agenda, config); break;
        case OPCION_FAVORITOS: menu_favoritos(*agenda); break;
        case OPCION_ESTADISTICAS: menu_estadisticas(*agenda); break;
        case OPCION_GUARDAR: menu_guardar(*agenda, config); break;
        case OPCION_CARGAR: menu_cargar(agenda, config); break;
        case OPCION_EXPORTAR: menu_exportar(*agenda); break;
        case OPCION_IMPORTAR: menu_importar(agenda, config); break;
        case OPCION_BACKUP: menu_backup(*agenda); break;
        case OPCION_CONFIGURACION: menu_configuracion(config); break;
        case OPCION_AYUDA: menu_ayuda(); break;
        case OPCION_SALIR: return false;
        default: printf("Opción inválida\n"); break;
    }
    return true;
}


