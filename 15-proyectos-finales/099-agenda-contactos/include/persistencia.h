#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "contacto.h"
#include <stdio.h>

#define NOMBRE_ARCHIVO_DEFAULT "data/agenda.txt"
#define NOMBRE_ARCHIVO_BACKUP "data/agenda_backup.txt"
#define FORMATO_EXPORTACION_CSV "data/agenda.csv"

/* Ambos valores usan el mismo formato CSV de texto; la extensión es libre. */
typedef enum {
    FORMATO_TEXTO = 0,
    FORMATO_CSV = 1
} formato_archivo_t;

/* NULL como ruta selecciona NOMBRE_ARCHIVO_DEFAULT.
 * Los archivos regulares se reemplazan solo tras escribir y cerrar sin error. */
resultado_t guardar_contactos(const nodo_t *cabeza, const char *nombre_archivo);
/* Añade los contactos leídos; cualquier error conserva la lista original. */
resultado_t cargar_contactos(nodo_t **cabeza, const char *nombre_archivo);
resultado_t exportar_csv(const nodo_t *cabeza, const char *nombre_archivo);
/* reemplazar=true sustituye la lista solo cuando el CSV completo es válido. */
resultado_t importar_csv(nodo_t **cabeza, const char *nombre_archivo, bool reemplazar);
resultado_t crear_backup(const nodo_t *cabeza);
resultado_t restaurar_backup(nodo_t **cabeza);

/* Valida el contenido, incluidas las categorías, campos y duplicados. */
bool validar_archivo(const char *nombre_archivo, formato_archivo_t formato);
bool archivo_existe(const char *nombre_archivo);
long obtener_tamaño_archivo(const char *nombre_archivo);
resultado_t crear_directorio_datos(void);
const char *formato_a_string(formato_archivo_t formato);

#endif
