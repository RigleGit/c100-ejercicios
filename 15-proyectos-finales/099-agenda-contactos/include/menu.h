#ifndef MENU_H
#define MENU_H

#include "contacto.h"
#include "persistencia.h"
#include <stdbool.h>

// Constantes del menú
#define TAM_OPCION 10
#define TAM_CONFIRMACION 5

// Opciones del menú principal
typedef enum {
    OPCION_AGREGAR = 1,
    OPCION_BUSCAR = 2,
    OPCION_ELIMINAR = 3,
    OPCION_LISTAR = 4,
    OPCION_MODIFICAR = 5,
    OPCION_FAVORITOS = 6,
    OPCION_ESTADISTICAS = 7,
    OPCION_GUARDAR = 8,
    OPCION_CARGAR = 9,
    OPCION_EXPORTAR = 10,
    OPCION_IMPORTAR = 11,
    OPCION_BACKUP = 12,
    OPCION_CONFIGURACION = 13,
    OPCION_AYUDA = 14,
    OPCION_SALIR = 0
} opcion_menu_t;

// Configuración de la aplicación
typedef struct {
    bool auto_guardar;
    bool confirmaciones;
    bool mostrar_estadisticas_inicio;
    formato_archivo_t formato_default;
    char directorio_datos[256];
    int max_resultados_busqueda;
} configuracion_app_t;

// ========================================
// FUNCIONES DEL MENÚ PRINCIPAL
// ========================================

/**
 * @brief Muestra el menú principal
 */
void mostrar_menu_principal(void);

/**
 * @brief Muestra el banner de la aplicación
 */
void mostrar_banner(void);

/**
 * @brief Procesa la entrada del usuario del menú principal
 * @param agenda Puntero a la agenda de contactos
 * @param config Configuración de la aplicación
 * @return true si debe continuar, false para salir
 */
bool procesar_opcion_menu(nodo_t **agenda, configuracion_app_t *config);

/**
 * @brief Lee una opción del menú de forma segura
 * @return Opción seleccionada
 */
opcion_menu_t leer_opcion_menu(void);

/**
 * @brief Pausa la ejecución hasta que el usuario presione Enter
 */
void pausar_para_continuar(void);

/**
 * @brief Limpia la pantalla (multiplataforma)
 */
void limpiar_pantalla(void);

// ========================================
// FUNCIONES DE ENTRADA DE DATOS
// ========================================

/**
 * @brief Lee una línea de forma segura eliminando el salto de línea
 * @param buffer Buffer donde almacenar la línea
 * @param tamaño Tamaño del buffer
 * @return true si se leyó correctamente
 */
bool leer_linea_segura(char *buffer, size_t tamaño);

/**
 * @brief Lee un número entero de forma segura
 * @param prompt Mensaje a mostrar
 * @param min Valor mínimo permitido
 * @param max Valor máximo permitido
 * @return Número leído
 */
int leer_entero_seguro(const char *prompt, int min, int max);

/**
 * @brief Lee un string no vacío
 * @param prompt Mensaje a mostrar
 * @param buffer Buffer donde almacenar el string
 * @param tamaño Tamaño del buffer
 * @return true si se leyó un string válido
 */
bool leer_string_no_vacio(const char *prompt, char *buffer, size_t tamaño);

/**
 * @brief Lee confirmación (s/n) del usuario
 * @param prompt Mensaje a mostrar
 * @return true si confirma, false en caso contrario
 */
bool leer_confirmacion(const char *prompt);

/**
 * @brief Lee una categoría del usuario
 * @return Categoría seleccionada
 */
categoria_t leer_categoria(void);

// ========================================
// FUNCIONES ESPECÍFICAS DEL MENÚ
// ========================================

/**
 * @brief Maneja la opción de agregar contacto
 * @param agenda Puntero a la agenda
 * @param config Configuración de la aplicación
 */
void menu_agregar_contacto(nodo_t **agenda, const configuracion_app_t *config);

/**
 * @brief Maneja la opción de buscar contacto
 * @param agenda Agenda de contactos
 */
void menu_buscar_contacto(const nodo_t *agenda);

/**
 * @brief Maneja la opción de eliminar contacto
 * @param agenda Puntero a la agenda
 * @param config Configuración de la aplicación
 */
void menu_eliminar_contacto(nodo_t **agenda, const configuracion_app_t *config);

/**
 * @brief Maneja la opción de listar contactos
 * @param agenda Agenda de contactos
 */
void menu_listar_contactos(const nodo_t *agenda);

/**
 * @brief Maneja la opción de modificar contacto
 * @param agenda Agenda de contactos
 * @param config Configuración de la aplicación
 */
void menu_modificar_contacto(nodo_t *agenda, const configuracion_app_t *config);

/**
 * @brief Maneja la opción de favoritos
 * @param agenda Agenda de contactos
 */
void menu_favoritos(nodo_t *agenda);

/**
 * @brief Maneja la opción de estadísticas
 * @param agenda Agenda de contactos
 */
void menu_estadisticas(const nodo_t *agenda);

/**
 * @brief Maneja la opción de guardar
 * @param agenda Agenda de contactos
 * @param config Configuración de la aplicación
 */
void menu_guardar(const nodo_t *agenda, const configuracion_app_t *config);

/**
 * @brief Maneja la opción de cargar
 * @param agenda Puntero a la agenda
 * @param config Configuración de la aplicación
 */
void menu_cargar(nodo_t **agenda, const configuracion_app_t *config);

/**
 * @brief Maneja la opción de exportar
 * @param agenda Agenda de contactos
 */
void menu_exportar(const nodo_t *agenda);

/**
 * @brief Maneja la opción de importar
 * @param agenda Puntero a la agenda
 * @param config Configuración de la aplicación
 */
void menu_importar(nodo_t **agenda, const configuracion_app_t *config);

/**
 * @brief Maneja la opción de backup
 * @param agenda Agenda de contactos
 */
void menu_backup(const nodo_t *agenda);

/**
 * @brief Maneja la opción de configuración
 * @param config Puntero a la configuración
 */
void menu_configuracion(configuracion_app_t *config);

/**
 * @brief Muestra la ayuda de la aplicación
 */
void menu_ayuda(void);

// ========================================
// FUNCIONES DE CONFIGURACIÓN
// ========================================

/**
 * @brief Inicializa la configuración con valores por defecto
 * @param config Configuración a inicializar
 */
void inicializar_configuracion(configuracion_app_t *config);

/**
 * @brief Carga la configuración desde archivo
 * @param config Configuración a cargar
 * @return Código de resultado
 */
resultado_t cargar_configuracion(configuracion_app_t *config);

/**
 * @brief Guarda la configuración en archivo
 * @param config Configuración a guardar
 * @return Código de resultado
 */
resultado_t guardar_configuracion(const configuracion_app_t *config);

/**
 * @brief Muestra la configuración actual
 * @param config Configuración a mostrar
 */
void mostrar_configuracion(const configuracion_app_t *config);

// ========================================
// FUNCIONES DE UTILIDAD DE MENÚ
// ========================================

/**
 * @brief Muestra un mensaje de error formateado
 * @param mensaje Mensaje de error
 */
void mostrar_error(const char *mensaje);

/**
 * @brief Muestra un mensaje de éxito formateado
 * @param mensaje Mensaje de éxito
 */
void mostrar_exito(const char *mensaje);

/**
 * @brief Muestra un mensaje de información formateado
 * @param mensaje Mensaje de información
 */
void mostrar_info(const char *mensaje);

/**
 * @brief Muestra un mensaje de advertencia formateado
 * @param mensaje Mensaje de advertencia
 */
void mostrar_advertencia(const char *mensaje);

/**
 * @brief Imprime una línea separadora
 */
void imprimir_separador(void);

/**
 * @brief Imprime un encabezado formateado
 * @param titulo Título del encabezado
 */
void imprimir_encabezado(const char *titulo);

/**
 * @brief Valida que la agenda no esté vacía antes de una operación
 * @param agenda Agenda a validar
 * @return true si tiene contactos, false si está vacía
 */
bool validar_agenda_no_vacia(const nodo_t *agenda);

/**
 * @brief Maneja el auto-guardado si está habilitado
 * @param agenda Agenda de contactos
 * @param config Configuración de la aplicación
 */
void manejar_auto_guardado(const nodo_t *agenda, const configuracion_app_t *config);

#endif // MENU_H
