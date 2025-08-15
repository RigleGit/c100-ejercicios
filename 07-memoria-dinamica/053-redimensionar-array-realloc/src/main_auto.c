#include "../include/redimensionar_array.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Versión automática del main para tests sin interacción
 */
int main(void) {
    printf("🔄 EJERCICIO 053 - REDIMENSIONAR ARRAY CON REALLOC (AUTOMÁTICO)\n");
    printf("================================================================\n\n");
    
    // Simular implementación básica con datos predefinidos
    printf("=== SIMULACIÓN IMPLEMENTACIÓN BÁSICA ===\n");
    printf("Simulando entrada de números: 1, 2, 3, 4, 5, -1\n\n");
    
    ArrayRedimensionable *array_basico = crearArrayRedimensionable(10);
    if (!array_basico) {
        printf("❌ Error creando array básico\n");
        return 1;
    }
    
    // Simular entrada de datos
    int datos_simulados[] = {1, 2, 3, 4, 5};
    int num_datos = sizeof(datos_simulados) / sizeof(datos_simulados[0]);
    
    for (int i = 0; i < num_datos; i++) {
        if (necesitaRedimensionar(array_basico)) {
            printf("📦 Array lleno. Redimensionando...\n");
        }
        agregarElemento(array_basico, datos_simulados[i]);
    }
    
    printf("✅ Datos agregados exitosamente\n");
    mostrarEstadisticas(array_basico);
    imprimirResultados(array_basico);
    liberarArrayRedimensionable(array_basico);
    
    printf("=========================================\n\n");
    
    // Demostración avanzada
    printf("=== DEMOSTRACIÓN AVANZADA ===\n\n");
    
    ArrayRedimensionable *array = crearArrayRedimensionable(5);
    if (!array) {
        printf("❌ Error creando array\n");
        return 1;
    }
    
    printf("Array creado con bloque inicial de 5 elementos\n");
    mostrarEstadisticas(array);
    
    // Simular entrada de datos
    printf("📝 Simulando entrada de 18 números...\n");
    for (int i = 1; i <= 18; i++) {
        if (necesitaRedimensionar(array)) {
            printf("(requiere redimensionamiento) ");
        }
        agregarElemento(array, i * 10);
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
    
    // Demostración de estrategias
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
    
    printf("✅ Ejercicio completado exitosamente (versión automática)\n");
    printf("💡 Has aprendido sobre redimensionamiento controlado con realloc\n");
    
    return 0;
}

