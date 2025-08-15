#include "../include/redimensionar_array.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Función básica del enunciado: redimensionamiento con realloc
 * Implementación directa del ejercicio original
 */
void implementacionBasica(void) {
    printf("=== IMPLEMENTACIÓN BÁSICA DEL ENUNCIADO ===\n\n");
    
    int *datos = NULL;
    int capacidad = 10;
    int cantidad = 0;
    int entrada;
    
    datos = (int *)malloc(capacidad * sizeof(int));
    if (datos == NULL) {
        printf("Error al reservar memoria inicial.\n");
        return;
    }
    
    printf("Capacidad inicial: %d elementos\n", capacidad);
    printf("Introduce números (termina con -1):\n");
    
    while (1) {
        printf("Número %d: ", cantidad + 1);
        if (scanf("%d", &entrada) != 1) {
            printf("Error de entrada\n");
            break;
        }
        
        if (entrada == -1) break;
        
        if (cantidad == capacidad) {
            printf("📦 Array lleno (%d/%d). Redimensionando...\n", cantidad, capacidad);
            capacidad += 10;
            int *nuevo = realloc(datos, capacidad * sizeof(int));
            if (nuevo == NULL) {
                printf("Error al redimensionar el array.\n");
                free(datos);
                return;
            }
            datos = nuevo;
            printf("✅ Nueva capacidad: %d elementos\n", capacidad);
        }
        
        datos[cantidad++] = entrada;
    }
    
    printf("\nHas introducido %d números:\n", cantidad);
    for (int i = 0; i < cantidad; i++) {
        printf("%d ", datos[i]);
    }
    printf("\n\n");
    
    // Mostrar estadísticas finales
    printf("📊 Estadísticas finales:\n");
    printf("   Elementos: %d\n", cantidad);
    printf("   Capacidad: %d\n", capacidad);
    printf("   Factor de uso: %.1f%%\n", cantidad > 0 ? (double)cantidad / capacidad * 100 : 0);
    printf("   Memoria usada: %zu bytes\n", cantidad * sizeof(int));
    printf("   Memoria total: %zu bytes\n", capacidad * sizeof(int));
    
    free(datos);
    datos = NULL;
    
    printf("Memoria liberada correctamente\n");
    printf("============================================\n\n");
}

/**
 * Demostración avanzada con estructura de datos mejorada
 */
void demostracionAvanzada(void) {
    printf("=== DEMOSTRACIÓN AVANZADA ===\n\n");
    
    // Crear array con bloque de 5 elementos para ver más redimensionamientos
    ArrayRedimensionable *array = crearArrayRedimensionable(5);
    if (!array) {
        printf("❌ Error creando array\n");
        return;
    }
    
    printf("Array creado con bloque inicial de 5 elementos\n");
    mostrarEstadisticas(array);
    
    // Simular entrada de datos
    printf("📝 Simulando entrada de 18 números...\n");
    for (int i = 1; i <= 18; i++) {
        printf("Agregando %d... ", i * 10);
        
        if (necesitaRedimensionar(array)) {
            printf("(requiere redimensionamiento) ");
        }
        
        if (agregarElemento(array, i * 10)) {
            printf("✅\n");
        } else {
            printf("❌\n");
            break;
        }
        
        // Mostrar estado cada 5 elementos
        if (i % 5 == 0) {
            printf("\n🔍 Estado después de %d elementos:\n", i);
            mostrarEstadisticas(array);
        }
    }
    
    printf("\n📋 Array final:\n");
    mostrarArray(array);
    imprimirResultados(array);
    
    // Análisis de crecimiento
    analizarPatronCrecimiento(array);
    
    // Demostrar compactación
    printf("🗜️ Demostrando compactación:\n");
    printf("Antes: ");
    mostrarEstadisticas(array);
    compactarArray(array);
    printf("Después: ");
    mostrarEstadisticas(array);
    
    liberarArrayRedimensionable(array);
    printf("✅ Demostración completada\n\n");
}

/**
 * Demostración de diferentes estrategias de redimensionamiento
 */
void demostracionEstrategias(void) {
    printf("=== ESTRATEGIAS DE REDIMENSIONAMIENTO ===\n\n");
    
    // Estrategia 1: Bloques fijos pequeños
    printf("🔹 Estrategia 1: Bloques fijos de 3 elementos\n");
    ArrayRedimensionable *array1 = crearArrayRedimensionable(3);
    for (int i = 1; i <= 10; i++) {
        agregarElemento(array1, i);
    }
    printf("Resultado: ");
    mostrarEstadisticas(array1);
    
    // Estrategia 2: Bloques fijos grandes
    printf("🔹 Estrategia 2: Bloques fijos de 20 elementos\n");
    ArrayRedimensionable *array2 = crearArrayRedimensionable(20);
    for (int i = 1; i <= 10; i++) {
        agregarElemento(array2, i);
    }
    printf("Resultado: ");
    mostrarEstadisticas(array2);
    
    // Estrategia 3: Redimensionamiento por porcentaje
    printf("🔹 Estrategia 3: Redimensionamiento por porcentaje (150%%)\n");
    ArrayRedimensionable *array3 = crearArrayRedimensionable(4);
    for (int i = 1; i <= 5; i++) {
        if (necesitaRedimensionar(array3)) {
            redimensionarPorPorcentaje(array3, 1.5); // Crecer 50%
        }
        agregarElemento(array3, i);
    }
    printf("Resultado: ");
    mostrarEstadisticas(array3);
    
    // Comparación de eficiencia
    printf("\n📊 Comparación de eficiencia:\n");
    printf("Array 1 (bloques de 3): Factor de uso %.1f%%, Redimensionamientos: %zu\n",
           calcularFactorUso(array1), array1->capacidad / array1->bloque_tamaño - 1);
    printf("Array 2 (bloques de 20): Factor de uso %.1f%%, Redimensionamientos: %zu\n",
           calcularFactorUso(array2), array2->capacidad / array2->bloque_tamaño - 1);
    printf("Array 3 (porcentaje): Factor de uso %.1f%%\n",
           calcularFactorUso(array3));
    
    liberarArrayRedimensionable(array1);
    liberarArrayRedimensionable(array2);
    liberarArrayRedimensionable(array3);
    
    printf("======================================\n\n");
}

/**
 * Función para casos de prueba interactivos
 */
void modoInteractivo(void) {
    printf("=== MODO INTERACTIVO ===\n\n");
    
    ArrayRedimensionable *array = crearArrayRedimensionable(BLOQUE_INICIAL);
    if (!array) {
        printf("❌ Error creando array\n");
        return;
    }
    
    printf("🎮 Array creado. Puedes introducir números.\n");
    mostrarEstadisticas(array);
    
    int numeros_leidos = leerNumerosUsuario(array);
    
    if (numeros_leidos >= 0) {
        imprimirResultados(array);
        analizarPatronCrecimiento(array);
    }
    
    liberarArrayRedimensionable(array);
    printf("========================\n\n");
}

int main(void) {
    printf("🔄 EJERCICIO 053 - REDIMENSIONAR ARRAY CON REALLOC\n");
    printf("===================================================\n\n");
    
    // Ejecutar implementación básica del enunciado
    implementacionBasica();
    
    // Demostración avanzada con estructura mejorada
    demostracionAvanzada();
    
    // Comparación de diferentes estrategias
    demostracionEstrategias();
    
    // Modo interactivo (comentado para permitir tests automáticos)
    /*
    printf("¿Quieres probar el modo interactivo? (s/n): ");
    char respuesta;
    if (scanf(" %c", &respuesta) == 1 && (respuesta == 's' || respuesta == 'S')) {
        modoInteractivo();
    }
    */
    
    printf("✅ Ejercicio completado exitosamente\n");
    printf("💡 Has aprendido sobre redimensionamiento controlado con realloc\n");
    
    return 0;
}
