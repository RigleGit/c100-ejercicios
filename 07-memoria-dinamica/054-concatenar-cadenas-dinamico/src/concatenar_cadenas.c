#include "../include/concatenar_cadenas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// Función principal del enunciado: concatenar dos cadenas
char* concatenarCadenas(const char *cadena1, const char *cadena2) {
    if (!cadena1 || !cadena2) {
        return NULL;
    }
    
    // Calcular longitud total necesaria
    size_t longitud_total = strlen(cadena1) + strlen(cadena2) + 1;
    
    // Reservar memoria dinámicamente
    char *resultado = (char *)malloc(longitud_total * sizeof(char));
    if (resultado == NULL) {
        return NULL;
    }
    
    // Copiar primera cadena
    strcpy(resultado, cadena1);
    
    // Concatenar segunda cadena
    strcat(resultado, cadena2);
    
    return resultado;
}

// Leer cadena del usuario con prompt
char* leerCadenaUsuario(const char *prompt) {
    if (!prompt) {
        prompt = "Introduce cadena: ";
    }
    
    char buffer[MAX_ENTRADA];
    
    printf("%s", prompt);
    fflush(stdout);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    
    // Eliminar el salto de línea si existe
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Crear copia dinámica
    return duplicarCadena(buffer);
}

// Liberar memoria de cadena
void liberarCadena(char *cadena) {
    free(cadena);
}

// Crear cadena dinámica con capacidad inicial
CadenaDinamica* crearCadenaDinamica(size_t capacidad_inicial) {
    if (capacidad_inicial == 0) {
        capacidad_inicial = BUFFER_INICIAL;
    }
    
    CadenaDinamica *cadena = malloc(sizeof(CadenaDinamica));
    if (!cadena) {
        return NULL;
    }
    
    cadena->contenido = malloc(capacidad_inicial * sizeof(char));
    if (!cadena->contenido) {
        free(cadena);
        return NULL;
    }
    
    cadena->contenido[0] = '\0';
    cadena->longitud = 0;
    cadena->capacidad = capacidad_inicial;
    
    return cadena;
}

// Liberar cadena dinámica
void liberarCadenaDinamica(CadenaDinamica *cadena) {
    if (cadena) {
        free(cadena->contenido);
        free(cadena);
    }
}

// Asignar contenido a cadena dinámica
bool asignarContenido(CadenaDinamica *cadena, const char *contenido) {
    if (!cadena || !contenido) {
        return false;
    }
    
    size_t longitud_necesaria = strlen(contenido) + 1;
    
    // Redimensionar si es necesario
    if (longitud_necesaria > cadena->capacidad) {
        char *nuevo_contenido = realloc(cadena->contenido, longitud_necesaria);
        if (!nuevo_contenido) {
            return false;
        }
        cadena->contenido = nuevo_contenido;
        cadena->capacidad = longitud_necesaria;
    }
    
    strcpy(cadena->contenido, contenido);
    cadena->longitud = strlen(contenido);
    
    return true;
}

// Concatenar a cadena dinámica
bool concatenarACadena(CadenaDinamica *cadena, const char *sufijo) {
    if (!cadena || !sufijo) {
        return false;
    }
    
    size_t longitud_sufijo = strlen(sufijo);
    size_t longitud_total = cadena->longitud + longitud_sufijo + 1;
    
    // Redimensionar si es necesario
    if (longitud_total > cadena->capacidad) {
        char *nuevo_contenido = realloc(cadena->contenido, longitud_total);
        if (!nuevo_contenido) {
            return false;
        }
        cadena->contenido = nuevo_contenido;
        cadena->capacidad = longitud_total;
    }
    
    strcat(cadena->contenido, sufijo);
    cadena->longitud += longitud_sufijo;
    
    return true;
}

// Concatenar múltiples cadenas
char* concatenarMultiples(const char **cadenas, size_t cantidad) {
    if (!cadenas || cantidad == 0) {
        return NULL;
    }
    
    // Calcular longitud total
    size_t longitud_total = 1; // Para el '\0'
    for (size_t i = 0; i < cantidad; i++) {
        if (cadenas[i]) {
            longitud_total += strlen(cadenas[i]);
        }
    }
    
    // Reservar memoria
    char *resultado = malloc(longitud_total);
    if (!resultado) {
        return NULL;
    }
    
    // Concatenar todas las cadenas
    resultado[0] = '\0';
    for (size_t i = 0; i < cantidad; i++) {
        if (cadenas[i]) {
            strcat(resultado, cadenas[i]);
        }
    }
    
    return resultado;
}

// Concatenar con separador
char* concatenarConSeparador(const char *cadena1, const char *cadena2, const char *separador) {
    if (!cadena1 || !cadena2) {
        return NULL;
    }
    
    const char *sep = separador ? separador : "";
    size_t longitud_total = strlen(cadena1) + strlen(cadena2) + strlen(sep) + 1;
    
    char *resultado = malloc(longitud_total);
    if (!resultado) {
        return NULL;
    }
    
    strcpy(resultado, cadena1);
    strcat(resultado, sep);
    strcat(resultado, cadena2);
    
    return resultado;
}

// Concatenar con formato (similar a sprintf pero dinámico)
char* concatenarConFormato(const char *formato, ...) {
    if (!formato) {
        return NULL;
    }
    
    va_list args;
    
    // Primer paso: calcular longitud necesaria
    va_start(args, formato);
    int longitud = vsnprintf(NULL, 0, formato, args);
    va_end(args);
    
    if (longitud < 0) {
        return NULL;
    }
    
    // Reservar memoria
    char *resultado = malloc(longitud + 1);
    if (!resultado) {
        return NULL;
    }
    
    // Segundo paso: formatear la cadena
    va_start(args, formato);
    vsnprintf(resultado, longitud + 1, formato, args);
    va_end(args);
    
    return resultado;
}

// Calcular longitud total de concatenación
size_t calcularLongitudTotal(const char *cadena1, const char *cadena2) {
    if (!cadena1 || !cadena2) {
        return 0;
    }
    return strlen(cadena1) + strlen(cadena2) + 1;
}

// Validar que la cadena no sea NULL y tenga contenido válido
bool validarCadena(const char *cadena) {
    return cadena != NULL;
}

// Duplicar cadena (crear copia dinámica)
char* duplicarCadena(const char *cadena) {
    if (!cadena) {
        return NULL;
    }
    
    size_t longitud = strlen(cadena) + 1;
    char *copia = malloc(longitud);
    if (!copia) {
        return NULL;
    }
    
    strcpy(copia, cadena);
    return copia;
}

// Extraer subcadena
char* subcadena(const char *cadena, size_t inicio, size_t longitud) {
    if (!cadena) {
        return NULL;
    }
    
    size_t len_cadena = strlen(cadena);
    if (inicio >= len_cadena) {
        return duplicarCadena(""); // Cadena vacía
    }
    
    if (inicio + longitud > len_cadena) {
        longitud = len_cadena - inicio;
    }
    
    char *resultado = malloc(longitud + 1);
    if (!resultado) {
        return NULL;
    }
    
    strncpy(resultado, cadena + inicio, longitud);
    resultado[longitud] = '\0';
    
    return resultado;
}

// Invertir cadena
char* invertirCadena(const char *cadena) {
    if (!cadena) {
        return NULL;
    }
    
    size_t longitud = strlen(cadena);
    char *resultado = malloc(longitud + 1);
    if (!resultado) {
        return NULL;
    }
    
    for (size_t i = 0; i < longitud; i++) {
        resultado[i] = cadena[longitud - 1 - i];
    }
    resultado[longitud] = '\0';
    
    return resultado;
}

// Convertir a mayúsculas
char* convertirAMayusculas(const char *cadena) {
    if (!cadena) {
        return NULL;
    }
    
    char *resultado = duplicarCadena(cadena);
    if (!resultado) {
        return NULL;
    }
    
    for (size_t i = 0; resultado[i]; i++) {
        resultado[i] = toupper((unsigned char)resultado[i]);
    }
    
    return resultado;
}

// Convertir a minúsculas
char* convertirAMinusculas(const char *cadena) {
    if (!cadena) {
        return NULL;
    }
    
    char *resultado = duplicarCadena(cadena);
    if (!resultado) {
        return NULL;
    }
    
    for (size_t i = 0; resultado[i]; i++) {
        resultado[i] = tolower((unsigned char)resultado[i]);
    }
    
    return resultado;
}

// Eliminar espacios en blanco
char* eliminarEspacios(const char *cadena) {
    if (!cadena) {
        return NULL;
    }
    
    size_t longitud = strlen(cadena);
    char *resultado = malloc(longitud + 1);
    if (!resultado) {
        return NULL;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < longitud; i++) {
        if (!isspace((unsigned char)cadena[i])) {
            resultado[j++] = cadena[i];
        }
    }
    resultado[j] = '\0';
    
    return resultado;
}

// Buscar subcadena (devuelve índice o -1)
int buscarSubcadena(const char *cadena, const char *subcadena) {
    if (!cadena || !subcadena) {
        return -1;
    }
    
    char *posicion = strstr(cadena, subcadena);
    if (posicion) {
        return (int)(posicion - cadena);
    }
    
    return -1;
}

// Contar ocurrencias de un carácter
size_t contarOcurrencias(const char *cadena, char caracter) {
    if (!cadena) {
        return 0;
    }
    
    size_t contador = 0;
    for (size_t i = 0; cadena[i]; i++) {
        if (cadena[i] == caracter) {
            contador++;
        }
    }
    
    return contador;
}

// Verificar si cadena empieza con prefijo
bool empiezaCon(const char *cadena, const char *prefijo) {
    if (!cadena || !prefijo) {
        return false;
    }
    
    return strncmp(cadena, prefijo, strlen(prefijo)) == 0;
}

// Verificar si cadena termina con sufijo
bool terminaCon(const char *cadena, const char *sufijo) {
    if (!cadena || !sufijo) {
        return false;
    }
    
    size_t len_cadena = strlen(cadena);
    size_t len_sufijo = strlen(sufijo);
    
    if (len_sufijo > len_cadena) {
        return false;
    }
    
    return strcmp(cadena + len_cadena - len_sufijo, sufijo) == 0;
}

// Leer cadena de forma segura
char* leerCadenaSegura(const char *prompt, size_t max_longitud) {
    if (max_longitud == 0) {
        max_longitud = MAX_ENTRADA;
    }
    
    char *buffer = malloc(max_longitud + 1);
    if (!buffer) {
        return NULL;
    }
    
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    
    if (fgets(buffer, max_longitud + 1, stdin) == NULL) {
        free(buffer);
        return NULL;
    }
    
    // Eliminar salto de línea
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Redimensionar al tamaño exacto
    size_t longitud_real = strlen(buffer);
    char *resultado = realloc(buffer, longitud_real + 1);
    
    return resultado ? resultado : buffer;
}

// Imprimir cadena con información adicional
void imprimirCadenaConInfo(const char *cadena, const char *etiqueta) {
    if (!etiqueta) {
        etiqueta = "Cadena";
    }
    
    if (!cadena) {
        printf("%s: (NULL)\n", etiqueta);
        return;
    }
    
    printf("%s: \"%s\" (longitud: %zu)\n", etiqueta, cadena, strlen(cadena));
}

// Mostrar estadísticas detalladas de la cadena
void mostrarEstadisticasCadena(const char *cadena) {
    if (!cadena) {
        printf("Cadena: (NULL)\n");
        return;
    }
    
    size_t longitud = strlen(cadena);
    size_t espacios = contarOcurrencias(cadena, ' ');
    size_t digitos = 0, letras = 0, otros = 0;
    
    for (size_t i = 0; i < longitud; i++) {
        if (isdigit((unsigned char)cadena[i])) {
            digitos++;
        } else if (isalpha((unsigned char)cadena[i])) {
            letras++;
        } else {
            otros++;
        }
    }
    
    printf("📊 Estadísticas de la cadena:\n");
    printf("   Contenido: \"%s\"\n", cadena);
    printf("   Longitud total: %zu caracteres\n", longitud);
    printf("   Letras: %zu\n", letras);
    printf("   Dígitos: %zu\n", digitos);
    printf("   Espacios: %zu\n", espacios);
    printf("   Otros caracteres: %zu\n", otros);
    printf("   Memoria usada: %zu bytes\n", longitud + 1);
}

// Crear cadena aleatoria para testing
char* crearCadenaAleatoria(size_t longitud) {
    if (longitud == 0) {
        return duplicarCadena("");
    }
    
    char *cadena = malloc(longitud + 1);
    if (!cadena) {
        return NULL;
    }
    
    const char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
    size_t num_caracteres = sizeof(caracteres) - 1;
    
    for (size_t i = 0; i < longitud; i++) {
        cadena[i] = caracteres[rand() % num_caracteres];
    }
    cadena[longitud] = '\0';
    
    return cadena;
}

// Comparar cadenas
bool compararCadenas(const char *cadena1, const char *cadena2) {
    if (!cadena1 && !cadena2) {
        return true;
    }
    if (!cadena1 || !cadena2) {
        return false;
    }
    
    return strcmp(cadena1, cadena2) == 0;
}

// Prueba de rendimiento
void pruebaRendimiento(size_t tamaño_cadena) {
    printf("🚀 Prueba de rendimiento con cadenas de %zu caracteres\n", tamaño_cadena);
    
    char *cadena1 = crearCadenaAleatoria(tamaño_cadena);
    char *cadena2 = crearCadenaAleatoria(tamaño_cadena);
    
    if (!cadena1 || !cadena2) {
        printf("❌ Error creando cadenas de prueba\n");
        free(cadena1);
        free(cadena2);
        return;
    }
    
    printf("📊 Memoria antes de concatenar: %zu bytes\n", 
           (tamaño_cadena + 1) * 2);
    
    char *resultado = concatenarCadenas(cadena1, cadena2);
    
    if (resultado) {
        printf("✅ Concatenación exitosa\n");
        printf("📊 Memoria después: %zu bytes\n", 
               strlen(resultado) + 1);
        printf("📏 Longitud resultado: %zu caracteres\n", 
               strlen(resultado));
        
        free(resultado);
    } else {
        printf("❌ Error en concatenación\n");
    }
    
    free(cadena1);
    free(cadena2);
}

// Obtener mensaje de error
const char* obtenerMensajeError(ErrorCadena error) {
    switch (error) {
        case CADENA_OK:
            return "Operación exitosa";
        case CADENA_ERROR_MEMORIA:
            return "Error de memoria insuficiente";
        case CADENA_ERROR_PARAMETRO_NULO:
            return "Parámetro nulo inesperado";
        case CADENA_ERROR_LONGITUD_INVALIDA:
            return "Longitud inválida";
        case CADENA_ERROR_ENTRADA:
            return "Error en entrada de datos";
        default:
            return "Error desconocido";
    }
}
