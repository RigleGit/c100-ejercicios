#include "../include/concatenar_cadenas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Función básica del enunciado: concatenación simple de dos cadenas
 * Implementación directa del ejercicio original
 */
void implementacionBasica(void) {
    printf("=== IMPLEMENTACIÓN BÁSICA DEL ENUNCIADO ===\n\n");
    
    char cadena1[100];
    char cadena2[100];
    
    printf("Introduce la primera cadena: ");
    fgets(cadena1, sizeof(cadena1), stdin);
    cadena1[strcspn(cadena1, "\n")] = '\0';  // Elimina el salto de línea

    printf("Introduce la segunda cadena: ");
    fgets(cadena2, sizeof(cadena2), stdin);
    cadena2[strcspn(cadena2, "\n")] = '\0';  // Elimina el salto de línea

    printf("\n📏 Análisis de longitudes:\n");
    printf("   Cadena 1: \"%s\" (%zu caracteres)\n", cadena1, strlen(cadena1));
    printf("   Cadena 2: \"%s\" (%zu caracteres)\n", cadena2, strlen(cadena2));
    
    size_t longitud_total = strlen(cadena1) + strlen(cadena2) + 1;
    printf("   Memoria necesaria: %zu bytes\n", longitud_total);

    printf("\n🔧 Reservando memoria dinámica...\n");
    char *resultado = (char *)malloc(longitud_total * sizeof(char));
    if (resultado == NULL) {
        printf("❌ Error al reservar memoria.\n");
        return;
    }
    
    printf("✅ Memoria reservada: %zu bytes en dirección %p\n", 
           longitud_total, (void*)resultado);

    printf("\n🔗 Concatenando cadenas...\n");
    printf("   1. Copiando primera cadena con strcpy()\n");
    strcpy(resultado, cadena1);
    printf("   2. Concatenando segunda cadena con strcat()\n");
    strcat(resultado, cadena2);

    printf("\n🎯 Resultado:\n");
    printf("   Cadena concatenada: \"%s\"\n", resultado);
    printf("   Longitud final: %zu caracteres\n", strlen(resultado));
    printf("   Memoria utilizada: %zu/%zu bytes\n", strlen(resultado) + 1, longitud_total);

    printf("\n🧹 Liberando memoria...\n");
    free(resultado);
    resultado = NULL;
    printf("✅ Memoria liberada correctamente\n");
    
    printf("============================================\n\n");
}

/**
 * Demostración avanzada con funciones helper
 */
void demostracionAvanzada(void) {
    printf("=== DEMOSTRACIÓN AVANZADA ===\n\n");
    
    printf("📝 Usando funciones helper para entrada segura...\n");
    
    char *cadena1 = leerCadenaUsuario("Introduce la primera cadena: ");
    if (!cadena1) {
        printf("❌ Error leyendo primera cadena\n");
        return;
    }
    
    char *cadena2 = leerCadenaUsuario("Introduce la segunda cadena: ");
    if (!cadena2) {
        printf("❌ Error leyendo segunda cadena\n");
        liberarCadena(cadena1);
        return;
    }
    
    printf("\n📊 Análisis detallado:\n");
    mostrarEstadisticasCadena(cadena1);
    printf("\n");
    mostrarEstadisticasCadena(cadena2);
    
    printf("\n🔗 Concatenando con función avanzada...\n");
    char *resultado = concatenarCadenas(cadena1, cadena2);
    
    if (resultado) {
        printf("✅ Concatenación exitosa:\n");
        mostrarEstadisticasCadena(resultado);
        
        // Operaciones adicionales
        printf("\n🔄 Operaciones adicionales:\n");
        
        char *invertida = invertirCadena(resultado);
        if (invertida) {
            imprimirCadenaConInfo(invertida, "Cadena invertida");
            liberarCadena(invertida);
        }
        
        char *mayusculas = convertirAMayusculas(resultado);
        if (mayusculas) {
            imprimirCadenaConInfo(mayusculas, "En mayúsculas");
            liberarCadena(mayusculas);
        }
        
        char *sin_espacios = eliminarEspacios(resultado);
        if (sin_espacios) {
            imprimirCadenaConInfo(sin_espacios, "Sin espacios");
            liberarCadena(sin_espacios);
        }
        
        liberarCadena(resultado);
    } else {
        printf("❌ Error en la concatenación\n");
    }
    
    liberarCadena(cadena1);
    liberarCadena(cadena2);
    
    printf("==============================\n\n");
}

/**
 * Demostración de concatenación múltiple
 */
void demostracionMultiple(void) {
    printf("=== CONCATENACIÓN MÚLTIPLE ===\n\n");
    
    const char *cadenas[] = {
        "Hola", " ", "mundo", " ", "desde", " ", "C"
    };
    size_t cantidad = sizeof(cadenas) / sizeof(cadenas[0]);
    
    printf("📝 Concatenando %zu cadenas:\n", cantidad);
    for (size_t i = 0; i < cantidad; i++) {
        printf("   [%zu]: \"%s\"\n", i, cadenas[i]);
    }
    
    char *resultado = concatenarMultiples(cadenas, cantidad);
    if (resultado) {
        printf("\n✅ Resultado: \"%s\"\n", resultado);
        printf("📏 Longitud total: %zu caracteres\n", strlen(resultado));
        liberarCadena(resultado);
    } else {
        printf("❌ Error en concatenación múltiple\n");
    }
    
    // Concatenación con separador
    printf("\n🔗 Concatenación con separador:\n");
    char *con_separador = concatenarConSeparador("usuario", "ejemplo.com", "@");
    if (con_separador) {
        imprimirCadenaConInfo(con_separador, "Email");
        liberarCadena(con_separador);
    }
    
    // Concatenación con formato
    printf("\n📋 Concatenación con formato:\n");
    char *con_formato = concatenarConFormato("El usuario %s tiene %d años", "Juan", 25);
    if (con_formato) {
        imprimirCadenaConInfo(con_formato, "Mensaje formateado");
        liberarCadena(con_formato);
    }
    
    printf("===============================\n\n");
}

/**
 * Demostración de cadenas dinámicas
 */
void demostracionCadenasDinamicas(void) {
    printf("=== CADENAS DINÁMICAS ===\n\n");
    
    // Crear cadena dinámica
    CadenaDinamica *cadena = crearCadenaDinamica(20);
    if (!cadena) {
        printf("❌ Error creando cadena dinámica\n");
        return;
    }
    
    printf("✅ Cadena dinámica creada (capacidad: %zu)\n", cadena->capacidad);
    
    // Asignar contenido inicial
    printf("\n📝 Asignando contenido inicial...\n");
    asignarContenido(cadena, "Hola");
    printf("Contenido: \"%s\" (longitud: %zu/%zu)\n", 
           cadena->contenido, cadena->longitud, cadena->capacidad);
    
    // Concatenar más contenido
    printf("\n🔗 Concatenando más contenido...\n");
    concatenarACadena(cadena, " mundo");
    printf("Contenido: \"%s\" (longitud: %zu/%zu)\n", 
           cadena->contenido, cadena->longitud, cadena->capacidad);
    
    // Forzar redimensionamiento
    printf("\n📈 Forzando redimensionamiento...\n");
    concatenarACadena(cadena, " con redimensionamiento automático");
    printf("Contenido: \"%s\"\n", cadena->contenido);
    printf("Nueva capacidad: %zu (redimensionamiento automático)\n", cadena->capacidad);
    
    liberarCadenaDinamica(cadena);
    printf("\n✅ Cadena dinámica liberada\n");
    
    printf("==========================\n\n");
}

/**
 * Demostración de análisis de cadenas
 */
void demostracionAnalisis(void) {
    printf("=== ANÁLISIS DE CADENAS ===\n\n");
    
    const char *texto = "Programando en C es divertido. C es poderoso.";
    printf("🔍 Analizando: \"%s\"\n\n", texto);
    
    // Búsquedas
    printf("🔎 Búsquedas:\n");
    int pos_c = buscarSubcadena(texto, "C");
    printf("   Primera ocurrencia de 'C': posición %d\n", pos_c);
    
    size_t count_espacios = contarOcurrencias(texto, ' ');
    printf("   Espacios encontrados: %zu\n", count_espacios);
    
    printf("   ¿Empieza con 'Programando'? %s\n", 
           empiezaCon(texto, "Programando") ? "Sí" : "No");
    printf("   ¿Termina con 'poderoso.'? %s\n", 
           terminaCon(texto, "poderoso.") ? "Sí" : "No");
    
    // Extraer subcadena
    printf("\n✂️ Extracción de subcadena:\n");
    char *sub = subcadena(texto, 14, 7); // "en C es"
    if (sub) {
        imprimirCadenaConInfo(sub, "Subcadena");
        liberarCadena(sub);
    }
    
    // Duplicar y manipular
    printf("\n🔄 Manipulaciones:\n");
    char *copia = duplicarCadena(texto);
    if (copia) {
        imprimirCadenaConInfo(copia, "Copia original");
        
        char *mayus = convertirAMayusculas(copia);
        if (mayus) {
            imprimirCadenaConInfo(mayus, "En mayúsculas");
            liberarCadena(mayus);
        }
        
        liberarCadena(copia);
    }
    
    printf("===========================\n\n");
}

/**
 * Pruebas de rendimiento
 */
void pruebasRendimiento(void) {
    printf("=== PRUEBAS DE RENDIMIENTO ===\n\n");
    
    printf("⏱️ Inicializando generador aleatorio...\n");
    srand((unsigned int)time(NULL));
    
    // Pruebas con diferentes tamaños
    size_t tamaños[] = {100, 1000, 10000, 100000};
    size_t num_pruebas = sizeof(tamaños) / sizeof(tamaños[0]);
    
    for (size_t i = 0; i < num_pruebas; i++) {
        printf("\n");
        pruebaRendimiento(tamaños[i]);
    }
    
    printf("\n===============================\n\n");
}

int main(void) {
    printf("🔗 EJERCICIO 054 - CONCATENAR CADENAS DINÁMICO\n");
    printf("===============================================\n\n");
    
    // Implementación básica del enunciado (comentada para tests automáticos)
    /*
    printf("¿Ejecutar implementación básica? (s/n): ");
    char respuesta;
    if (scanf(" %c", &respuesta) == 1 && (respuesta == 's' || respuesta == 'S')) {
        // Limpiar buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        implementacionBasica();
    } else {
        // Limpiar buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    */
    
    // Demos automáticas con datos predefinidos
    printf("🚀 Ejecutando demostraciones automáticas...\n\n");
    
    // Simular implementación básica con datos predefinidos
    printf("=== SIMULACIÓN IMPLEMENTACIÓN BÁSICA ===\n");
    printf("Simulando entrada: \"Hola\" y \" mundo\"\n\n");
    
    char *resultado_basico = concatenarCadenas("Hola", " mundo");
    if (resultado_basico) {
        printf("✅ Resultado básico: \"%s\"\n", resultado_basico);
        printf("📏 Longitud: %zu caracteres\n", strlen(resultado_basico));
        printf("💾 Memoria: %zu bytes\n", strlen(resultado_basico) + 1);
        liberarCadena(resultado_basico);
    }
    printf("=========================================\n\n");
    
    demostracionMultiple();
    demostracionCadenasDinamicas();
    demostracionAnalisis();
    pruebasRendimiento();
    
    printf("✅ Ejercicio completado exitosamente\n");
    printf("💡 Has aprendido sobre concatenación de cadenas con memoria dinámica\n");
    
    return 0;
}
