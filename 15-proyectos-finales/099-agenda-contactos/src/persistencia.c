/**
 * Persistencia - implementación alineada con persistencia.h y contacto.h
 */

#include "persistencia.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static void generar_nombre_backup(char *out, size_t n) {
    time_t now = time(NULL);
    struct tm tmv; localtime_r(&now, &tmv);
    snprintf(out, n, "data/agenda_backup_%04d%02d%02d_%02d%02d%02d.csv",
             tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday,
             tmv.tm_hour, tmv.tm_min, tmv.tm_sec);
}

resultado_t crear_directorio_datos(void) {
    struct stat st;
    if (stat("data", &st) == 0 && S_ISDIR(st.st_mode)) return RESULTADO_EXITO;
    if (mkdir("data", 0755) == 0) return RESULTADO_EXITO;
    return RESULTADO_PARAMETRO_INVALIDO;
}

bool archivo_existe(const char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return false; fclose(f); return true;
}

long obtener_tamaño_archivo(const char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return -1; fseek(f, 0, SEEK_END); long s = ftell(f); fclose(f); return s;
}

static void escribir_csv_header(FILE *fp) {
    fprintf(fp, "nombre,telefono,email,categoria,direccion\n");
}

static void escribir_contacto_csv(FILE *fp, const contacto_t *c) {
    fprintf(fp, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
            c->nombre, c->telefono, c->email, categoria_a_string(c->categoria), c->direccion);
}

resultado_t guardar_contactos(const nodo_t *cabeza, const char *nombre_archivo) {
    const char *ruta = nombre_archivo ? nombre_archivo : NOMBRE_ARCHIVO_DEFAULT;
    FILE *fp = fopen(ruta, "w"); if (!fp) return RESULTADO_PARAMETRO_INVALIDO;
    escribir_csv_header(fp);
    for (const nodo_t *it = cabeza; it; it = it->siguiente) escribir_contacto_csv(fp, &it->contacto);
    fclose(fp); return RESULTADO_EXITO;
}

static bool leer_campo_csv(char **cursor, char *out, size_t n) {
    if (!*cursor) return false; char *p = *cursor;
    if (*p == '"') {
        p++; char *q = strchr(p, '"'); if (!q) return false; size_t len = (size_t)(q - p);
        if (len >= n) len = n - 1; memcpy(out, p, len); out[len] = '\0'; q++; if (*q == ',') q++;
        *cursor = q; return true;
    } else {
        char *q = strchr(p, ','); size_t len = q ? (size_t)(q - p) : strlen(p);
        if (len >= n) len = n - 1; memcpy(out, p, len); out[len] = '\0'; *cursor = q ? q + 1 : NULL; return true;
    }
}

resultado_t cargar_contactos(nodo_t **cabeza, const char *nombre_archivo) {
    const char *ruta = nombre_archivo ? nombre_archivo : NOMBRE_ARCHIVO_DEFAULT;
    FILE *fp = fopen(ruta, "r"); if (!fp) return RESULTADO_PARAMETRO_INVALIDO;
    char linea[1024]; if (!fgets(linea, sizeof linea, fp)) { fclose(fp); return RESULTADO_PARAMETRO_INVALIDO; }
    while (fgets(linea, sizeof linea, fp)) {
        size_t len = strlen(linea); if (len && linea[len-1] == '\n') linea[len-1] = '\0';
        char *cur = linea; char nombre[TAM_NOMBRE] = {0}, tel[TAM_TELEFONO] = {0}, email[TAM_EMAIL] = {0}, cat[32] = {0}, dir[TAM_DIRECCION] = {0};
        if (!leer_campo_csv(&cur, nombre, sizeof nombre)) continue;
        if (!leer_campo_csv(&cur, tel, sizeof tel)) continue;
        if (!leer_campo_csv(&cur, email, sizeof email)) continue;
        if (!leer_campo_csv(&cur, cat, sizeof cat)) continue;
        (void)leer_campo_csv(&cur, dir, sizeof dir);
        contacto_t c = crear_contacto(nombre, tel, email, dir, string_a_categoria(cat));
        (void)agregar_contacto(cabeza, &c);
    }
    fclose(fp); return RESULTADO_EXITO;
}

resultado_t crear_backup(const nodo_t *cabeza) {
    char ruta[256]; generar_nombre_backup(ruta, sizeof ruta); return guardar_contactos(cabeza, ruta);
}

resultado_t restaurar_backup(nodo_t **cabeza) { (void)cabeza; return RESULTADO_PARAMETRO_INVALIDO; }

resultado_t exportar_csv(const nodo_t *cabeza, const char *nombre_archivo) { return guardar_contactos(cabeza, nombre_archivo); }

resultado_t importar_csv(nodo_t **cabeza, const char *nombre_archivo, bool reemplazar) {
    if (reemplazar) liberar_lista(cabeza); return cargar_contactos(cabeza, nombre_archivo);
}

resultado_t guardar_binario(const nodo_t *cabeza, const char *nombre_archivo) { (void)cabeza; (void)nombre_archivo; return RESULTADO_PARAMETRO_INVALIDO; }

resultado_t cargar_binario(nodo_t **cabeza, const char *nombre_archivo) { (void)cabeza; (void)nombre_archivo; return RESULTADO_PARAMETRO_INVALIDO; }

bool validar_archivo(const char *nombre_archivo, formato_archivo_t formato) { (void)formato; return archivo_existe(nombre_archivo); }

metadatos_archivo_t obtener_metadatos_archivo(const char *nombre_archivo) {
    metadatos_archivo_t m = {0}; m.version = 1; m.fecha_creacion = time(NULL); m.ultima_modificacion = m.fecha_creacion; strncpy(m.aplicacion, "agenda-contactos", sizeof m.aplicacion - 1); (void)nombre_archivo; return m;
}

void log_operacion(const char *operacion, resultado_t resultado) { fprintf(stderr, "[LOG] %s -> %d\n", operacion, (int)resultado); }

int limpiar_archivos_antiguos(int dias_antiguedad) { (void)dias_antiguedad; return 0; }

void tiempo_a_string(time_t tiempo, char *buffer, size_t tamaño_buffer) { struct tm tmv; localtime_r(&tiempo, &tmv); strftime(buffer, tamaño_buffer, FORMATO_FECHA, &tmv); }

time_t string_a_tiempo(const char *str) { struct tm tmv = {0}; strptime(str, FORMATO_FECHA, &tmv); return mktime(&tmv); }

void escapar_csv(const char *entrada, char *salida, size_t tamaño_salida) { strncpy(salida, entrada, tamaño_salida - 1); salida[tamaño_salida - 1] = '\0'; }

void desescapar_csv(const char *entrada, char *salida, size_t tamaño_salida) { strncpy(salida, entrada, tamaño_salida - 1); salida[tamaño_salida - 1] = '\0'; }

const char *formato_a_string(formato_archivo_t formato) {
    switch (formato) { case FORMATO_TEXTO: return "texto"; case FORMATO_CSV: return "csv"; case FORMATO_JSON: return "json"; case FORMATO_BINARIO: return "binario"; default: return "desconocido"; }
}
