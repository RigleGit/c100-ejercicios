#include "contacto.h"
#include <ctype.h>
#include <regex.h>

// Variables globales para estadísticas
static int siguiente_id = 1;

// ========================================
// FUNCIONES AUXILIARES PRIVADAS
// ========================================

static void limpiar_string(char *str) {
    if (!str) return;
    
    // Eliminar espacios al inicio y final
    char *inicio = str;
    while (isspace(*inicio)) inicio++;
    
    if (*inicio == '\0') {
        str[0] = '\0';
        return;
    }
    
    char *final = str + strlen(str) - 1;
    while (final > str && isspace(*final)) final--;
    
    // Mover el string limpio al inicio del buffer
    size_t longitud = final - inicio + 1;
    memmove(str, inicio, longitud);
    str[longitud] = '\0';
}

static bool es_string_vacio(const char *str) {
    if (!str) return true;
    
    while (*str) {
        if (!isspace(*str)) return false;
        str++;
    }
    return true;
}

// ========================================
// FUNCIONES DE CREACIÓN Y VALIDACIÓN
// ========================================

contacto_t crear_contacto(const char *nombre, const char *telefono, 
                         const char *email, const char *direccion, 
                         categoria_t categoria) {
    contacto_t contacto = {0};
    
    // Copiar y limpiar datos
    if (nombre) {
        strncpy(contacto.nombre, nombre, TAM_NOMBRE - 1);
        contacto.nombre[TAM_NOMBRE - 1] = '\0';
        limpiar_string(contacto.nombre);
    }
    
    if (telefono) {
        strncpy(contacto.telefono, telefono, TAM_TELEFONO - 1);
        contacto.telefono[TAM_TELEFONO - 1] = '\0';
        limpiar_string(contacto.telefono);
    }
    
    if (email) {
        strncpy(contacto.email, email, TAM_EMAIL - 1);
        contacto.email[TAM_EMAIL - 1] = '\0';
        limpiar_string(contacto.email);
    }
    
    if (direccion) {
        strncpy(contacto.direccion, direccion, TAM_DIRECCION - 1);
        contacto.direccion[TAM_DIRECCION - 1] = '\0';
        limpiar_string(contacto.direccion);
    }
    
    contacto.categoria = categoria;
    contacto.fecha_creacion = time(NULL);
    contacto.fecha_modificacion = contacto.fecha_creacion;
    contacto.favorito = false;
    contacto.id = siguiente_id++;
    
    return contacto;
}

bool validar_contacto(const contacto_t *contacto) {
    if (!contacto) return false;
    
    // Nombre es obligatorio
    if (es_string_vacio(contacto->nombre)) {
        return false;
    }
    
    // Al menos teléfono o email debe estar presente
    if (es_string_vacio(contacto->telefono) && es_string_vacio(contacto->email)) {
        return false;
    }
    
    // Validar email si está presente
    if (!es_string_vacio(contacto->email) && !validar_email(contacto->email)) {
        return false;
    }
    
    // Validar teléfono si está presente
    if (!es_string_vacio(contacto->telefono) && !validar_telefono(contacto->telefono)) {
        return false;
    }
    
    // Validar categoría
    if (contacto->categoria < CATEGORIA_PERSONAL || contacto->categoria > CATEGORIA_OTROS) {
        return false;
    }
    
    return true;
}

bool validar_email(const char *email) {
    if (!email || es_string_vacio(email)) return false;
    
    // Validación básica de email
    const char *arroba = strchr(email, '@');
    if (!arroba) return false;
    
    // Debe haber al menos un carácter antes de @
    if (arroba == email) return false;
    
    // Debe haber al menos un punto después de @
    const char *punto = strchr(arroba + 1, '.');
    if (!punto) return false;
    
    // Debe haber al menos un carácter después del último punto
    if (punto == email + strlen(email) - 1) return false;
    
    // No debe tener espacios
    if (strchr(email, ' ') || strchr(email, '\t')) return false;
    
    return true;
}

bool validar_telefono(const char *telefono) {
    if (!telefono || es_string_vacio(telefono)) return false;
    
    int digitos = 0;
    bool tiene_solo_digitos_y_especiales = true;
    
    for (int i = 0; telefono[i] != '\0'; i++) {
        char c = telefono[i];
        
        if (isdigit(c)) {
            digitos++;
        } else if (c != '-' && c != '(' && c != ')' && c != ' ' && c != '+') {
            tiene_solo_digitos_y_especiales = false;
            break;
        }
    }
    
    // Debe tener entre 7 y 15 dígitos
    return tiene_solo_digitos_y_especiales && digitos >= 7 && digitos <= 15;
}

void imprimir_contacto(const contacto_t *contacto, bool mostrar_detalle) {
    if (!contacto) {
        printf("Contacto nulo\n");
        return;
    }
    
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ %-55s │\n", contacto->nombre);
    printf("├─────────────────────────────────────────────────────────┤\n");
    
    if (!es_string_vacio(contacto->telefono)) {
        printf("│ 📞 Teléfono: %-42s │\n", contacto->telefono);
    }
    
    if (!es_string_vacio(contacto->email)) {
        printf("│ 📧 Email: %-45s │\n", contacto->email);
    }
    
    if (mostrar_detalle) {
        if (!es_string_vacio(contacto->direccion)) {
            printf("│ 🏠 Dirección: %-41s │\n", contacto->direccion);
        }
        
        printf("│ 📂 Categoría: %-41s │\n", categoria_a_string(contacto->categoria));
        printf("│ ID: %-3d %s                                     │\n", 
               contacto->id, contacto->favorito ? "⭐" : "  ");
        
        char fecha_str[64];
        strftime(fecha_str, sizeof(fecha_str), "%Y-%m-%d %H:%M", 
                localtime(&contacto->fecha_creacion));
        printf("│ 📅 Creado: %-43s │\n", fecha_str);
        
        if (contacto->fecha_modificacion != contacto->fecha_creacion) {
            strftime(fecha_str, sizeof(fecha_str), "%Y-%m-%d %H:%M", 
                    localtime(&contacto->fecha_modificacion));
            printf("│ ✏️  Modificado: %-39s │\n", fecha_str);
        }
    } else {
        printf("│ %s - ID: %d                                    │\n", 
               categoria_a_string(contacto->categoria), contacto->id);
        if (contacto->favorito) {
            printf("│ ⭐ FAVORITO                                      │\n");
        }
    }
    
    printf("└─────────────────────────────────────────────────────────┘\n");
}

const char *categoria_a_string(categoria_t categoria) {
    switch (categoria) {
        case CATEGORIA_PERSONAL: return "Personal";
        case CATEGORIA_TRABAJO:  return "Trabajo";
        case CATEGORIA_FAMILIA:  return "Familia";
        case CATEGORIA_AMIGOS:   return "Amigos";
        case CATEGORIA_OTROS:    return "Otros";
        default:                 return "Desconocido";
    }
}

categoria_t string_a_categoria(const char *str) {
    if (!str) return CATEGORIA_OTROS;
    
    if (strcasecmp(str, "personal") == 0) return CATEGORIA_PERSONAL;
    if (strcasecmp(str, "trabajo") == 0)  return CATEGORIA_TRABAJO;
    if (strcasecmp(str, "familia") == 0)  return CATEGORIA_FAMILIA;
    if (strcasecmp(str, "amigos") == 0)   return CATEGORIA_AMIGOS;
    
    return CATEGORIA_OTROS;
}

// ========================================
// FUNCIONES DE LISTA ENLAZADA
// ========================================

resultado_t agregar_contacto(nodo_t **cabeza, const contacto_t *contacto) {
    if (!cabeza || !contacto) {
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    
    if (!validar_contacto(contacto)) {
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    
    // Verificar si ya existe un contacto con el mismo nombre
    if (buscar_contacto_por_nombre(*cabeza, contacto->nombre)) {
        return RESULTADO_CONTACTO_DUPLICADO;
    }
    
    // Crear nuevo nodo
    nodo_t *nuevo = malloc(sizeof(nodo_t));
    if (!nuevo) {
        return RESULTADO_ERROR_MEMORIA;
    }
    
    nuevo->contacto = *contacto;
    nuevo->contacto.id = generar_id_unico(*cabeza);
    nuevo->contacto.fecha_creacion = time(NULL);
    nuevo->contacto.fecha_modificacion = nuevo->contacto.fecha_creacion;
    
    // Insertar en orden alfabético
    if (!*cabeza || strcasecmp(contacto->nombre, (*cabeza)->contacto.nombre) < 0) {
        nuevo->siguiente = *cabeza;
        *cabeza = nuevo;
    } else {
        nodo_t *actual = *cabeza;
        while (actual->siguiente && 
               strcasecmp(contacto->nombre, actual->siguiente->contacto.nombre) > 0) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
    
    return RESULTADO_EXITO;
}

const contacto_t *buscar_contacto_por_nombre(const nodo_t *cabeza, const char *nombre) {
    if (!cabeza || !nombre) return NULL;
    
    const nodo_t *actual = cabeza;
    while (actual) {
        if (strcasecmp(actual->contacto.nombre, nombre) == 0) {
            return &actual->contacto;
        }
        actual = actual->siguiente;
    }
    
    return NULL;
}

int buscar_contactos_por_nombre_parcial(const nodo_t *cabeza, const char *nombre_parcial,
                                       contacto_t **resultados, int max_resultados) {
    if (!cabeza || !nombre_parcial || !resultados || max_resultados <= 0) {
        return 0;
    }
    
    *resultados = malloc(max_resultados * sizeof(contacto_t));
    if (!*resultados) return 0;
    
    int encontrados = 0;
    const nodo_t *actual = cabeza;
    
    while (actual && encontrados < max_resultados) {
        if (strcasestr(actual->contacto.nombre, nombre_parcial)) {
            (*resultados)[encontrados] = actual->contacto;
            encontrados++;
        }
        actual = actual->siguiente;
    }
    
    if (encontrados == 0) {
        free(*resultados);
        *resultados = NULL;
    }
    
    return encontrados;
}

int buscar_contactos_por_categoria(const nodo_t *cabeza, categoria_t categoria,
                                  contacto_t **resultados, int max_resultados) {
    if (!cabeza || !resultados || max_resultados <= 0) {
        return 0;
    }
    
    *resultados = malloc(max_resultados * sizeof(contacto_t));
    if (!*resultados) return 0;
    
    int encontrados = 0;
    const nodo_t *actual = cabeza;
    
    while (actual && encontrados < max_resultados) {
        if (actual->contacto.categoria == categoria) {
            (*resultados)[encontrados] = actual->contacto;
            encontrados++;
        }
        actual = actual->siguiente;
    }
    
    if (encontrados == 0) {
        free(*resultados);
        *resultados = NULL;
    }
    
    return encontrados;
}

const contacto_t *buscar_contacto_por_id(const nodo_t *cabeza, int id) {
    if (!cabeza) return NULL;
    
    const nodo_t *actual = cabeza;
    while (actual) {
        if (actual->contacto.id == id) {
            return &actual->contacto;
        }
        actual = actual->siguiente;
    }
    
    return NULL;
}

resultado_t eliminar_contacto(nodo_t **cabeza, const char *nombre) {
    if (!cabeza || !nombre) {
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    
    if (!*cabeza) {
        return RESULTADO_LISTA_VACIA;
    }
    
    nodo_t *actual = *cabeza;
    nodo_t *anterior = NULL;
    
    while (actual) {
        if (strcasecmp(actual->contacto.nombre, nombre) == 0) {
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                *cabeza = actual->siguiente;
            }
            
            free(actual);
            return RESULTADO_EXITO;
        }
        
        anterior = actual;
        actual = actual->siguiente;
    }
    
    return RESULTADO_CONTACTO_NO_ENCONTRADO;
}

resultado_t modificar_contacto(nodo_t *cabeza, const char *nombre_original,
                              const contacto_t *contacto_nuevo) {
    if (!cabeza || !nombre_original || !contacto_nuevo) {
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    
    if (!validar_contacto(contacto_nuevo)) {
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    
    nodo_t *actual = cabeza;
    while (actual) {
        if (strcasecmp(actual->contacto.nombre, nombre_original) == 0) {
            // Verificar si el nuevo nombre ya existe (si cambió)
            if (strcasecmp(nombre_original, contacto_nuevo->nombre) != 0) {
                if (buscar_contacto_por_nombre(cabeza, contacto_nuevo->nombre)) {
                    return RESULTADO_CONTACTO_DUPLICADO;
                }
            }
            
            // Preservar algunos campos
            int id_original = actual->contacto.id;
            time_t fecha_creacion_original = actual->contacto.fecha_creacion;
            
            actual->contacto = *contacto_nuevo;
            actual->contacto.id = id_original;
            actual->contacto.fecha_creacion = fecha_creacion_original;
            actual->contacto.fecha_modificacion = time(NULL);
            
            return RESULTADO_EXITO;
        }
        actual = actual->siguiente;
    }
    
    return RESULTADO_CONTACTO_NO_ENCONTRADO;
}

void listar_contactos(const nodo_t *cabeza, bool mostrar_detalle) {
    if (!cabeza) {
        printf("\n📝 La agenda está vacía.\n\n");
        return;
    }
    
    int contador = 0;
    const nodo_t *actual = cabeza;
    
    printf("\n📋 LISTA DE CONTACTOS\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    while (actual) {
        contador++;
        printf("\n[%d] ", contador);
        imprimir_contacto(&actual->contacto, mostrar_detalle);
        actual = actual->siguiente;
        
        if (actual && !mostrar_detalle) {
            printf("\n");
        }
    }
    
    printf("\nTotal de contactos: %d\n\n", contador);
}

void listar_contactos_favoritos(const nodo_t *cabeza) {
    if (!cabeza) {
        printf("\n📝 La agenda está vacía.\n\n");
        return;
    }
    
    int contador = 0;
    const nodo_t *actual = cabeza;
    
    printf("\n⭐ CONTACTOS FAVORITOS\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    while (actual) {
        if (actual->contacto.favorito) {
            contador++;
            printf("\n[%d] ", contador);
            imprimir_contacto(&actual->contacto, true);
            printf("\n");
        }
        actual = actual->siguiente;
    }
    
    if (contador == 0) {
        printf("\n📝 No hay contactos marcados como favoritos.\n\n");
    } else {
        printf("\nTotal de favoritos: %d\n\n", contador);
    }
}

resultado_t marcar_favorito(nodo_t *cabeza, const char *nombre, bool favorito) {
    if (!cabeza || !nombre) {
        return RESULTADO_PARAMETRO_INVALIDO;
    }
    
    nodo_t *actual = cabeza;
    while (actual) {
        if (strcasecmp(actual->contacto.nombre, nombre) == 0) {
            actual->contacto.favorito = favorito;
            actual->contacto.fecha_modificacion = time(NULL);
            return RESULTADO_EXITO;
        }
        actual = actual->siguiente;
    }
    
    return RESULTADO_CONTACTO_NO_ENCONTRADO;
}

int contar_contactos(const nodo_t *cabeza) {
    int contador = 0;
    const nodo_t *actual = cabeza;
    
    while (actual) {
        contador++;
        actual = actual->siguiente;
    }
    
    return contador;
}

void liberar_lista(nodo_t **cabeza) {
    if (!cabeza) return;
    
    nodo_t *actual = *cabeza;
    while (actual) {
        nodo_t *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    
    *cabeza = NULL;
}

estadisticas_t obtener_estadisticas(const nodo_t *cabeza) {
    estadisticas_t stats = {0};
    
    if (!cabeza) return stats;
    
    const nodo_t *actual = cabeza;
    while (actual) {
        stats.total_contactos++;
        
        if (actual->contacto.categoria >= CATEGORIA_PERSONAL && 
            actual->contacto.categoria <= CATEGORIA_OTROS) {
            stats.contactos_por_categoria[actual->contacto.categoria]++;
        }
        
        if (actual->contacto.favorito) {
            stats.contactos_favoritos++;
        }
        
        actual = actual->siguiente;
    }
    
    return stats;
}

void imprimir_estadisticas(const estadisticas_t *stats) {
    if (!stats) return;
    
    printf("\n📊 ESTADÍSTICAS DE LA AGENDA\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("Total de contactos: %d\n", stats->total_contactos);
    printf("Contactos favoritos: %d\n", stats->contactos_favoritos);
    printf("\nPor categoría:\n");
    printf("  📱 Personal: %d\n", stats->contactos_por_categoria[CATEGORIA_PERSONAL]);
    printf("  💼 Trabajo:  %d\n", stats->contactos_por_categoria[CATEGORIA_TRABAJO]);
    printf("  👨‍👩‍👧‍👦 Familia:  %d\n", stats->contactos_por_categoria[CATEGORIA_FAMILIA]);
    printf("  👥 Amigos:   %d\n", stats->contactos_por_categoria[CATEGORIA_AMIGOS]);
    printf("  📂 Otros:    %d\n", stats->contactos_por_categoria[CATEGORIA_OTROS]);
    
    if (stats->total_contactos > 0) {
        double porcentaje_favoritos = (double)stats->contactos_favoritos / 
                                     stats->total_contactos * 100.0;
        printf("\nPorcentaje de favoritos: %.1f%%\n", porcentaje_favoritos);
    }
    
    printf("═══════════════════════════════════════════════════════════\n\n");
}

int generar_id_unico(const nodo_t *cabeza) {
    int max_id = 0;
    const nodo_t *actual = cabeza;
    
    while (actual) {
        if (actual->contacto.id > max_id) {
            max_id = actual->contacto.id;
        }
        actual = actual->siguiente;
    }
    
    return max_id + 1;
}

const char *resultado_a_string(resultado_t resultado) {
    switch (resultado) {
        case RESULTADO_EXITO:
            return "Operación exitosa";
        case RESULTADO_ERROR_MEMORIA:
            return "Error de memoria insuficiente";
        case RESULTADO_CONTACTO_NO_ENCONTRADO:
            return "Contacto no encontrado";
        case RESULTADO_CONTACTO_DUPLICADO:
            return "Ya existe un contacto con ese nombre";
        case RESULTADO_PARAMETRO_INVALIDO:
            return "Parámetro inválido";
        case RESULTADO_LISTA_VACIA:
            return "La agenda está vacía";
        default:
            return "Error desconocido";
    }
}
