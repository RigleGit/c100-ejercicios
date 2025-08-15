/**
 * @file assert_validacion.c
 * @brief Implementación de funciones con validación usando assert
 * @author Ejercicios C
 * @date 2025
 */

#include "assert_validacion.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

/* ====================================================================
 * FUNCIONES MATEMÁTICAS CON VALIDACIÓN
 * ==================================================================== */

double raiz_cuadrada(double x) {
    // Assert con mensaje personalizado
    assert(x >= 0.0 && "El argumento no puede ser negativo");
    return sqrt(x);
}

bool raiz_cuadrada_segura(double x, double *resultado) {
    // Validación manual sin assert
    if (x < 0.0 || resultado == NULL) {
        return false;
    }
    
    *resultado = sqrt(x);
    return true;
}

double logaritmo_natural(double x) {
    // El logaritmo solo está definido para números positivos
    assert(x > 0.0 && "El logaritmo solo está definido para números positivos");
    return log(x);
}

double division_segura(double dividendo, double divisor) {
    // Verificar que el divisor no sea cero
    assert(fabs(divisor) > DBL_EPSILON && "El divisor no puede ser cero");
    return dividendo / divisor;
}

double potencia_validada(double base, double exponente) {
    // Para bases negativas, el exponente debe ser un entero
    if (base < 0.0) {
        // Verificar que el exponente sea entero
        assert(exponente == floor(exponente) && 
               "Para bases negativas, el exponente debe ser entero");
    }
    
    // Para exponentes negativos, la base no puede ser cero
    if (exponente < 0.0) {
        assert(fabs(base) > DBL_EPSILON && 
               "Para exponentes negativos, la base no puede ser cero");
    }
    
    return pow(base, exponente);
}

/* ====================================================================
 * FUNCIONES PARA ARRAYS CON VALIDACIÓN
 * ==================================================================== */

double promedio_array(const double array[], int tamaño) {
    // Validar precondiciones
    assert(array != NULL && "El array no puede ser NULL");
    assert(tamaño > 0 && "El tamaño debe ser mayor que cero");
    
    double suma = 0.0;
    for (int i = 0; i < tamaño; i++) {
        suma += array[i];
    }
    
    return suma / tamaño;
}

int buscar_elemento(const double array[], int tamaño, double elemento) {
    // Validar precondiciones
    assert(array != NULL && "El array no puede ser NULL");
    assert(tamaño > 0 && "El tamaño debe ser mayor que cero");
    
    for (int i = 0; i < tamaño; i++) {
        if (fabs(array[i] - elemento) < DBL_EPSILON) {
            return i;
        }
    }
    
    return -1; // No encontrado
}

void copiar_array(const double origen[], double destino[], int tamaño) {
    // Validar precondiciones
    assert(origen != NULL && "El array origen no puede ser NULL");
    assert(destino != NULL && "El array destino no puede ser NULL");
    assert(tamaño > 0 && "El tamaño debe ser mayor que cero");
    assert(origen != destino && "Los arrays no pueden ser el mismo");
    
    for (int i = 0; i < tamaño; i++) {
        destino[i] = origen[i];
    }
}

/* ====================================================================
 * FUNCIONES DE CADENAS CON VALIDACIÓN
 * ==================================================================== */

int longitud_cadena(const char *cadena) {
    assert(cadena != NULL && "La cadena no puede ser NULL");
    
    int longitud = 0;
    while (cadena[longitud] != '\0') {
        longitud++;
    }
    
    return longitud;
}

void copiar_cadena(const char *origen, char *destino) {
    assert(origen != NULL && "La cadena origen no puede ser NULL");
    assert(destino != NULL && "La cadena destino no puede ser NULL");
    
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

/* ====================================================================
 * FUNCIONES DE DEMOSTRACIÓN
 * ==================================================================== */

void demostrar_casos_validos(void) {
    printf("=== CASOS VÁLIDOS (Assert no se activa) ===\n");
    
    // Raíz cuadrada de números positivos
    printf("Raíz cuadrada de 25.0: %.2f\n", raiz_cuadrada(25.0));
    printf("Raíz cuadrada de 0.0: %.2f\n", raiz_cuadrada(0.0));
    printf("Raíz cuadrada de 2.0: %.6f\n", raiz_cuadrada(2.0));
    
    // Logaritmo de números positivos
    printf("Logaritmo natural de e: %.6f\n", logaritmo_natural(M_E));
    printf("Logaritmo natural de 10: %.6f\n", logaritmo_natural(10.0));
    
    // División con divisor no cero
    printf("15.0 / 3.0 = %.2f\n", division_segura(15.0, 3.0));
    printf("10.0 / 4.0 = %.2f\n", division_segura(10.0, 4.0));
    
    // Potencias válidas
    printf("2^3 = %.2f\n", potencia_validada(2.0, 3.0));
    printf("(-2)^4 = %.2f\n", potencia_validada(-2.0, 4.0));
    
    // Arrays válidos
    double numeros[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    int tamaño = sizeof(numeros) / sizeof(numeros[0]);
    
    printf("Promedio de [1,2,3,4,5]: %.2f\n", promedio_array(numeros, tamaño));
    
    int indice = buscar_elemento(numeros, tamaño, 3.0);
    printf("Elemento 3.0 encontrado en índice: %d\n", indice);
    
    // Cadenas válidas
    const char *texto = "Hola mundo";
    printf("Longitud de '%s': %d\n", texto, longitud_cadena(texto));
    
    printf("✅ Todos los casos válidos ejecutados sin problemas\n\n");
}

void demostrar_casos_invalidos(void) {
    printf("=== CASOS INVÁLIDOS (Assert se activará) ===\n");
    printf("⚠️  ADVERTENCIA: Estas llamadas causarán terminación del programa\n");
    printf("    si assert está activo (modo Debug)\n\n");
    
    // Solo mostrar qué haríamos, no ejecutar realmente
    printf("Las siguientes llamadas causarían assert:\n");
    printf("- raiz_cuadrada(-5.0)  // Número negativo\n");
    printf("- logaritmo_natural(0.0)  // Cero no válido\n");
    printf("- logaritmo_natural(-1.0)  // Número negativo\n");
    printf("- division_segura(10.0, 0.0)  // División por cero\n");
    printf("- potencia_validada(-2.0, 1.5)  // Base negativa con exp. no entero\n");
    printf("- promedio_array(NULL, 5)  // Array NULL\n");
    printf("- promedio_array(numeros, 0)  // Tamaño cero\n");
    printf("- longitud_cadena(NULL)  // Cadena NULL\n");
    
    printf("\n💡 Para ver assert en acción, descomenta las llamadas en el código\n");
    printf("   y recompila en modo Debug (sin -DNDEBUG)\n\n");
    
    // Descomentar SOLO UNA de estas líneas para ver assert en acción:
    
    // printf("Intentando raíz cuadrada de -5.0...\n");
    // raiz_cuadrada(-5.0);  // ¡Esto causará abort()!
    
    // printf("Intentando logaritmo de 0.0...\n");
    // logaritmo_natural(0.0);  // ¡Esto causará abort()!
    
    // printf("Intentando división por cero...\n");
    // division_segura(10.0, 0.0);  // ¡Esto causará abort()!
}

void demostrar_alternativas_seguras(void) {
    printf("=== ALTERNATIVAS SEGURAS (Sin assert) ===\n");
    
    // Raíz cuadrada segura
    double resultado;
    bool exito;
    
    exito = raiz_cuadrada_segura(25.0, &resultado);
    printf("Raíz cuadrada segura de 25.0: %s (%.2f)\n", 
           exito ? "éxito" : "error", resultado);
    
    exito = raiz_cuadrada_segura(-5.0, &resultado);
    printf("Raíz cuadrada segura de -5.0: %s\n", 
           exito ? "éxito" : "error (número negativo)");
    
    exito = raiz_cuadrada_segura(16.0, NULL);
    printf("Raíz cuadrada segura con resultado NULL: %s\n", 
           exito ? "éxito" : "error (puntero nulo)");
    
    printf("✅ Las alternativas seguras manejan errores sin terminar el programa\n\n");
}

void mostrar_estado_assert(void) {
    printf("=== ESTADO DE ASSERT ===\n");
    
#ifdef NDEBUG
    printf("🔴 NDEBUG está definido - Assert está DESACTIVADO\n");
    printf("   Las validaciones con assert no se ejecutarán\n");
    printf("   Esto es típico en builds de Release\n");
#else
    printf("🟢 NDEBUG NO está definido - Assert está ACTIVADO\n");
    printf("   Las validaciones con assert se ejecutarán\n");
    printf("   Esto es típico en builds de Debug\n");
#endif
    
    printf("\nPara cambiar el comportamiento:\n");
    printf("- Activar assert: compilar sin -DNDEBUG\n");
    printf("- Desactivar assert: compilar con -DNDEBUG\n");
    printf("- O definir #define NDEBUG antes de #include <assert.h>\n\n");
}

/* ====================================================================
 * FUNCIONES AUXILIARES PARA TESTING
 * ==================================================================== */

bool es_assert_activo(void) {
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
}
