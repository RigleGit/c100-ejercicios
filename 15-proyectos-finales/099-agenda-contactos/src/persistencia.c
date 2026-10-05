#define _XOPEN_SOURCE 700
#include "persistencia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

resultado_t crear_directorio_datos(void) {
    struct stat st;
    if (stat("data", &st) == 0 && S_ISDIR(st.st_mode)) return RESULTADO_EXITO;
    return mkdir("data", 0755) == 0 ? RESULTADO_EXITO : RESULTADO_PARAMETRO_INVALIDO;
}
bool archivo_existe(const char *nombre_archivo) {
    if (!nombre_archivo) return false;
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return false;
    fclose(f);
    return true;
}
long obtener_tamaño_archivo(const char *nombre_archivo) {
    if (!nombre_archivo) return -1;
    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return -1;
    long size = fseek(f, 0, SEEK_END) == 0 ? ftell(f) : -1;
    if (fclose(f) != 0) return -1;
    return size;
}
static bool escribir_campo(FILE *f, const char *s) {
    if (fputc('"', f) == EOF) return false;
    for (; *s; ++s) {
        if (*s == '"' && fputc('"', f) == EOF) return false;
        if (fputc((unsigned char)*s, f) == EOF) return false;
    }
    return fputc('"', f) != EOF;
}
static bool escribir_agenda(FILE *f, const nodo_t *lista) {
    if (fputs("nombre,telefono,email,categoria,direccion\n", f) == EOF) return false;
    for (; lista; lista = lista->siguiente) {
        const contacto_t *c = &lista->contacto;
        const char *campos[] = {c->nombre,c->telefono,c->email,
                               categoria_a_string(c->categoria),c->direccion};
        for (size_t i = 0; i < 5; ++i) {
            if (!escribir_campo(f, campos[i])) return false;
            if (fputc(i == 4 ? '\n' : ',', f) == EOF) return false;
        }
    }
    return !ferror(f);
}
resultado_t guardar_contactos(const nodo_t *cabeza, const char *nombre_archivo) {
    const char *ruta = nombre_archivo ? nombre_archivo : NOMBRE_ARCHIVO_DEFAULT;
    struct stat st;
    bool regular = stat(ruta, &st) != 0 || S_ISREG(st.st_mode);
    char *temporal = NULL;
    FILE *fp = NULL;
    if (regular) {
        temporal = malloc(strlen(ruta) + 12);
        if (!temporal) return RESULTADO_ERROR_MEMORIA;
        sprintf(temporal, "%s.tmp.XXXXXX", ruta);
        int fd = mkstemp(temporal);
        if (fd >= 0) {
            fp = fdopen(fd, "w");
            if (!fp) close(fd);
        }
    } else {
        fp = fopen(ruta, "w");
    }
    if (!fp) {
        if (temporal) unlink(temporal);
        free(temporal);
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    bool ok = escribir_agenda(fp, cabeza);
    if (fclose(fp) != 0) ok = false;
    if (ok && temporal && rename(temporal, ruta) != 0) ok = false;
    if (temporal) unlink(temporal);
    free(temporal);
    return ok ? RESULTADO_EXITO : RESULTADO_PARAMETRO_INVALIDO;
}
/* 1: registro lógico; 0: fin limpio; -1: CSV inválido o error. */
static int leer_registro(FILE *f, char campos[5][TAM_DIRECCION]) {
    int c = fgetc(f);
    if (c == EOF) return ferror(f) ? -1 : 0;
    for (size_t campo = 0; campo < 5; ++campo) {
        size_t n = 0;
        bool citado = c == '"';
        if (citado) c = fgetc(f);
        while (1) {
            if (citado) {
                if (c == EOF) return -1;
                if (c == '"') {
                    int next = fgetc(f);
                    if (next != '"') { c = next; break; }
                }
            } else if (c == ',' || c == '\n' || c == '\r' || c == EOF) {
                break;
            } else if (c == '"') {
                return -1;
            }
            if (c == 0 || n + 1 >= TAM_DIRECCION) return -1;
            campos[campo][n++] = (char)c;
            c = fgetc(f);
        }
        campos[campo][n] = '\0';
        if (campo < 4) {
            if (c != ',') return -1;
            c = fgetc(f);
        } else {
            if (c == '\r') {
                c = fgetc(f);
                if (c != '\n' && c != EOF) return -1;
            }
            if (c != '\n' && c != EOF) return -1;
        }
    }
    return ferror(f) ? -1 : 1;
}
static resultado_t leer_agenda(FILE *f, nodo_t **lista) {
    char campos[5][TAM_DIRECCION];
    const char *header[] = {"nombre","telefono","email","categoria","direccion"};
    if (leer_registro(f, campos) != 1) return RESULTADO_PARAMETRO_INVALIDO;
    for (size_t i = 0; i < 5; ++i)
        if (strcmp(campos[i], header[i]) != 0) return RESULTADO_PARAMETRO_INVALIDO;
    int estado;
    while ((estado = leer_registro(f, campos)) == 1) {
        if (strlen(campos[0]) >= TAM_NOMBRE || strlen(campos[1]) >= TAM_TELEFONO ||
            strlen(campos[2]) >= TAM_EMAIL) return RESULTADO_PARAMETRO_INVALIDO;
        categoria_t categoria = string_a_categoria(campos[3]);
        if (strcasecmp(campos[3], categoria_a_string(categoria)) != 0)
            return RESULTADO_PARAMETRO_INVALIDO;
        contacto_t c = crear_contacto(campos[0],campos[1],campos[2],campos[4],categoria);
        resultado_t resultado = agregar_contacto(lista, &c);
        if (resultado != RESULTADO_EXITO) return resultado;
    }
    return estado == 0 ? RESULTADO_EXITO : RESULTADO_PARAMETRO_INVALIDO;
}
resultado_t importar_csv(nodo_t **cabeza, const char *nombre_archivo, bool reemplazar) {
    if (!cabeza) return RESULTADO_PARAMETRO_INVALIDO;
    const char *ruta = nombre_archivo ? nombre_archivo : NOMBRE_ARCHIVO_DEFAULT;
    FILE *fp = fopen(ruta, "r");
    if (!fp) return RESULTADO_PARAMETRO_INVALIDO;
    nodo_t *nueva = NULL;
    resultado_t resultado = RESULTADO_EXITO;
    if (!reemplazar) {
        for (const nodo_t *it = *cabeza; it; it = it->siguiente) {
            resultado = agregar_contacto(&nueva, &it->contacto);
            if (resultado != RESULTADO_EXITO) break;
        }
    }
    if (resultado == RESULTADO_EXITO) resultado = leer_agenda(fp, &nueva);
    if (fclose(fp) != 0) resultado = RESULTADO_PARAMETRO_INVALIDO;
    if (resultado == RESULTADO_EXITO) {
        liberar_lista(cabeza);
        *cabeza = nueva;
    } else {
        liberar_lista(&nueva);
    }
    return resultado;
}
resultado_t cargar_contactos(nodo_t **cabeza, const char *nombre_archivo) {
    return importar_csv(cabeza, nombre_archivo, false);
}
resultado_t crear_backup(const nodo_t *cabeza) {
    if (crear_directorio_datos() != RESULTADO_EXITO) return RESULTADO_PARAMETRO_INVALIDO;
    return guardar_contactos(cabeza, NOMBRE_ARCHIVO_BACKUP);
}
resultado_t restaurar_backup(nodo_t **cabeza) {
    return importar_csv(cabeza, NOMBRE_ARCHIVO_BACKUP, true);
}
resultado_t exportar_csv(const nodo_t *cabeza, const char *nombre_archivo) {
    return guardar_contactos(cabeza, nombre_archivo);
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
