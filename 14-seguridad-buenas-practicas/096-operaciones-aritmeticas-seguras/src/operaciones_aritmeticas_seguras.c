/**
 * @file operaciones_aritmeticas_seguras.c
 * @brief Implementación de operaciones aritméticas seguras con detección de overflow
 * @details Implementa funciones que detectan y previenen overflow/underflow
 *          aritmético, un problema crítico de seguridad en programas C.
 */

#include "../include/operaciones_aritmeticas_seguras.h"
#include <stdlib.h>
#include <string.h>

// =============================================================================
// CONSTANTES Y VALORES DE PRUEBA
// =============================================================================

const int VALORES_PRUEBA_OVERFLOW[] = {
    INT_MAX, INT_MAX - 1, INT_MAX - 10,
    INT_MIN, INT_MIN + 1, INT_MIN + 10,
    0, 1, -1, 10, -10, 1000, -1000
};

const size_t NUM_VALORES_PRUEBA = sizeof(VALORES_PRUEBA_OVERFLOW) / sizeof(VALORES_PRUEBA_OVERFLOW[0]);

const int CASOS_LIMITE_INT[] = {
    INT_MAX, INT_MIN, INT_MAX - 1, INT_MIN + 1,
    INT_MAX / 2, INT_MIN / 2, 0, 1, -1
};

const size_t NUM_CASOS_LIMITE = sizeof(CASOS_LIMITE_INT) / sizeof(CASOS_LIMITE_INT[0]);

// =============================================================================
// OPERACIONES BÁSICAS SEGURAS PARA INT
// =============================================================================

resultado_aritmetica_t suma_segura(int a, int b, int* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Verificar overflow positivo: b > 0 && a > INT_MAX - b
    if (b > 0 && a > INT_MAX - b) {
        return ARITMETICA_OVERFLOW;
    }
    
    // Verificar underflow (overflow negativo): b < 0 && a < INT_MIN - b
    if (b < 0 && a < INT_MIN - b) {
        return ARITMETICA_UNDERFLOW;
    }
    
    // Operación segura
    *resultado = a + b;
    return ARITMETICA_EXITOSA;
}

resultado_aritmetica_t resta_segura(int a, int b, int* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Verificar overflow: b < 0 && a > INT_MAX + b
    // Equivalente a: a - b > INT_MAX
    if (b < 0 && a > INT_MAX + b) {
        return ARITMETICA_OVERFLOW;
    }
    
    // Verificar underflow: b > 0 && a < INT_MIN + b
    // Equivalente a: a - b < INT_MIN
    if (b > 0 && a < INT_MIN + b) {
        return ARITMETICA_UNDERFLOW;
    }
    
    // Operación segura
    *resultado = a - b;
    return ARITMETICA_EXITOSA;
}

resultado_aritmetica_t multiplicacion_segura(int a, int b, int* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Casos especiales que no pueden causar overflow
    if (a == 0 || b == 0) {
        *resultado = 0;
        return ARITMETICA_EXITOSA;
    }
    
    if (a == 1) {
        *resultado = b;
        return ARITMETICA_EXITOSA;
    }
    
    if (b == 1) {
        *resultado = a;
        return ARITMETICA_EXITOSA;
    }
    
    // Caso especial: -1 * -1 = 1 (seguro)
    if (a == -1 && b == -1) {
        *resultado = 1;
        return ARITMETICA_EXITOSA;
    }
    
    // Caso especial problemático: INT_MIN * -1 = overflow
    if ((a == INT_MIN && b == -1) || (a == -1 && b == INT_MIN)) {
        return ARITMETICA_OVERFLOW;
    }
    
    // Para otros casos, usar división para verificar overflow
    // Si a * b causaría overflow, entonces |a| > INT_MAX / |b|
    
    // Calcular valores absolutos de forma segura
    int abs_a = (a == INT_MIN) ? INT_MAX : abs(a);
    int abs_b = (b == INT_MIN) ? INT_MAX : abs(b);
    
    // Verificar si el resultado cabría en un int
    if (abs_a > INT_MAX / abs_b) {
        return (a > 0) == (b > 0) ? ARITMETICA_OVERFLOW : ARITMETICA_UNDERFLOW;
    }
    
    // Operación segura
    *resultado = a * b;
    return ARITMETICA_EXITOSA;
}

resultado_aritmetica_t division_segura(int a, int b, int* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Verificar división por cero
    if (b == 0) {
        return ARITMETICA_DIVISION_CERO;
    }
    
    // Caso especial: INT_MIN / -1 = overflow (daría INT_MAX + 1)
    if (a == INT_MIN && b == -1) {
        return ARITMETICA_OVERFLOW;
    }
    
    // Operación segura
    *resultado = a / b;
    return ARITMETICA_EXITOSA;
}

resultado_aritmetica_t modulo_seguro(int a, int b, int* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Verificar división por cero
    if (b == 0) {
        return ARITMETICA_DIVISION_CERO;
    }
    
    // El módulo no puede causar overflow, pero verificamos el caso especial
    // INT_MIN % -1 en algunos sistemas puede ser problemático
    if (a == INT_MIN && b == -1) {
        *resultado = 0;  // Matemáticamente correcto
        return ARITMETICA_EXITOSA;
    }
    
    // Operación segura
    *resultado = a % b;
    return ARITMETICA_EXITOSA;
}

// =============================================================================
// OPERACIONES SEGURAS PARA OTROS TIPOS
// =============================================================================

resultado_aritmetica_t suma_segura_long(long long a, long long b, long long* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Verificar overflow: b > 0 && a > LLONG_MAX - b
    if (b > 0 && a > LLONG_MAX - b) {
        return ARITMETICA_OVERFLOW;
    }
    
    // Verificar underflow: b < 0 && a < LLONG_MIN - b
    if (b < 0 && a < LLONG_MIN - b) {
        return ARITMETICA_UNDERFLOW;
    }
    
    *resultado = a + b;
    return ARITMETICA_EXITOSA;
}

resultado_aritmetica_t suma_segura_uint(unsigned int a, unsigned int b, unsigned int* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Para unsigned, solo verificar overflow (no underflow en suma)
    if (a > UINT_MAX - b) {
        return ARITMETICA_OVERFLOW;
    }
    
    *resultado = a + b;
    return ARITMETICA_EXITOSA;
}

resultado_aritmetica_t suma_segura_size_t(size_t a, size_t b, size_t* resultado) {
    if (resultado == NULL) {
        return ARITMETICA_ERROR_PARAMETRO;
    }
    
    // Verificar overflow en size_t
    if (a > SIZE_MAX - b) {
        return ARITMETICA_OVERFLOW;
    }
    
    *resultado = a + b;
    return ARITMETICA_EXITOSA;
}

// =============================================================================
// FUNCIONES DE VALIDACIÓN Y VERIFICACIÓN
// =============================================================================

bool verificar_suma_segura(int a, int b) {
    return !((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b));
}

bool verificar_multiplicacion_segura(int a, int b) {
    int dummy;
    return multiplicacion_segura(a, b, &dummy) == ARITMETICA_EXITOSA;
}

bool calcular_rango_suma_segura(int base, int* min_suma, int* max_suma) {
    if (min_suma == NULL || max_suma == NULL) {
        return false;
    }
    
    // Calcular el rango seguro para sumar a 'base'
    *max_suma = INT_MAX - base;  // máximo valor que se puede sumar
    *min_suma = INT_MIN - base;  // mínimo valor que se puede sumar
    
    return true;
}

// =============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// =============================================================================

void demostrar_vulnerabilidades_overflow(void) {
    putchar('\n');
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    printf("🚨 DEMOSTRACIÓN: VULNERABILIDADES DE OVERFLOW ARITMÉTICO\n");
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    
    printf("\n💣 PROBLEMA 1: Overflow silencioso en validación de tamaño\n");
    printf("Código vulnerable típico:\n");
    printf("```c\n");
    printf("if (size + offset < buffer_size) {\n");
    printf("    // ¡VULNERABLE! Si size + offset causa overflow,\n");
    printf("    // el resultado puede ser negativo y pasar la validación\n");
    printf("}\n```\n");
    
    // Demostración práctica
    unsigned int size = UINT_MAX - 10;
    unsigned int offset = 20;
    unsigned int buffer_size = 1000;
    
    printf("\nEjemplo práctico:\n");
    printf("   size = %u (muy grande)\n", size);
    printf("   offset = %u\n", offset);
    printf("   buffer_size = %u\n", buffer_size);
    
    // Aritmética insegura (demuestra el problema)
    unsigned int suma_insegura = size + offset;  // ¡Overflow!
    printf("\n❌ Aritmética insegura:\n");
    printf("   size + offset = %u (¡overflow!)\n", suma_insegura);
    printf("   %u < %u? %s\n", suma_insegura, buffer_size, 
           (suma_insegura < buffer_size) ? "SÍ (¡PELIGRO!)" : "NO");
    
    // Aritmética segura
    unsigned int suma_segura_result;
    resultado_aritmetica_t resultado = suma_segura_uint(size, offset, &suma_segura_result);
    printf("\n✅ Aritmética segura:\n");
    printf("   resultado: %s\n", resultado_aritmetica_a_string(resultado));
    if (resultado == ARITMETICA_OVERFLOW) {
        printf("   ¡Overflow detectado y prevención activada!\n");
    }
    
    printf("\n💣 PROBLEMA 2: Overflow en cálculo de memoria\n");
    int num_elementos = 1000000;
    int tamaño_elemento = 3000;
    
    printf("Calculando memoria necesaria:\n");
    printf("   elementos = %d\n", num_elementos);
    printf("   tamaño por elemento = %d bytes\n", tamaño_elemento);
    
    // Inseguro
    int memoria_insegura = num_elementos * tamaño_elemento;  // ¡Overflow!
    printf("\n❌ Cálculo inseguro: %d * %d = %d\n", 
           num_elementos, tamaño_elemento, memoria_insegura);
    printf("   ¿Es realista? %s\n", 
           (memoria_insegura > 0 && memoria_insegura < 100000) ? "SÍ (¡ERROR!)" : "NO");
    
    // Seguro
    int memoria_segura_result;
    resultado = multiplicacion_segura(num_elementos, tamaño_elemento, &memoria_segura_result);
    printf("\n✅ Cálculo seguro: %s\n", resultado_aritmetica_a_string(resultado));
}

void demostrar_operaciones_seguras(void) {
    putchar('\n');
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    printf("🛡️ DEMOSTRACIÓN: OPERACIONES ARITMÉTICAS SEGURAS\n");
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    
    printf("\n🧪 Pruebas con valores límite:\n");
    
    struct {
        int a, b;
        const char* descripcion;
    } casos_prueba[] = {
        {INT_MAX, 1, "INT_MAX + 1 (overflow)"},
        {INT_MIN, -1, "INT_MIN - 1 (underflow)"},
        {INT_MAX, -1, "INT_MAX - 1 (seguro)"},
        {INT_MIN, 1, "INT_MIN + 1 (seguro)"},
        {INT_MAX / 2, INT_MAX / 2, "Suma grande pero segura"},
        {INT_MIN / 2, INT_MIN / 2, "Suma negativa grande"}
    };
    
    size_t num_casos = sizeof(casos_prueba) / sizeof(casos_prueba[0]);
    
    for (size_t i = 0; i < num_casos; i++) {
        printf("\n📊 Caso %zu: %s\n", i + 1, casos_prueba[i].descripcion);
        printf("   Operación: %d + %d\n", casos_prueba[i].a, casos_prueba[i].b);
        
        int resultado_suma;
        resultado_aritmetica_t resultado = suma_segura(casos_prueba[i].a, casos_prueba[i].b, &resultado_suma);
        
        printf("   Estado: %s\n", resultado_aritmetica_a_string(resultado));
        if (resultado == ARITMETICA_EXITOSA) {
            printf("   Resultado: %d\n", resultado_suma);
        } else {
            printf("   ⚠️  Operación insegura detectada y bloqueada\n");
        }
    }
    
    printf("\n🔢 Pruebas de multiplicación:\n");
    
    struct {
        int a, b;
        const char* descripcion;
    } casos_mult[] = {
        {INT_MAX, 2, "INT_MAX * 2 (overflow)"},
        {INT_MIN, -1, "INT_MIN * -1 (overflow especial)"},
        {46341, 46341, "Límite aproximado de sqrt(INT_MAX)"},
        {1000, 1000, "Multiplicación segura"},
        {0, INT_MAX, "Multiplicación por cero"}
    };
    
    size_t num_casos_mult = sizeof(casos_mult) / sizeof(casos_mult[0]);
    
    for (size_t i = 0; i < num_casos_mult; i++) {
        printf("\n📊 Caso %zu: %s\n", i + 1, casos_mult[i].descripcion);
        printf("   Operación: %d * %d\n", casos_mult[i].a, casos_mult[i].b);
        
        int resultado_mult;
        resultado_aritmetica_t resultado = multiplicacion_segura(casos_mult[i].a, casos_mult[i].b, &resultado_mult);
        
        printf("   Estado: %s\n", resultado_aritmetica_a_string(resultado));
        if (resultado == ARITMETICA_EXITOSA) {
            printf("   Resultado: %d\n", resultado_mult);
        }
    }
}

void ejecutar_pruebas_casos_limite(estadisticas_aritmetica_t* stats) {
    if (stats) {
        inicializar_estadisticas_aritmetica(stats);
    }
    
    putchar('\n');
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    printf("🧪 PRUEBAS EXHAUSTIVAS CON CASOS LÍMITE\n");
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    
    printf("\n📋 Ejecutando %zu × %zu = %zu combinaciones de prueba...\n", 
           NUM_CASOS_LIMITE, NUM_CASOS_LIMITE, NUM_CASOS_LIMITE * NUM_CASOS_LIMITE);
    
    size_t pruebas_exitosas = 0;
    size_t overflows_detectados = 0;
    size_t underflows_detectados = 0;
    
    for (size_t i = 0; i < NUM_CASOS_LIMITE; i++) {
        for (size_t j = 0; j < NUM_CASOS_LIMITE; j++) {
            int a = CASOS_LIMITE_INT[i];
            int b = CASOS_LIMITE_INT[j];
            int resultado;
            
            resultado_aritmetica_t estado = suma_segura(a, b, &resultado);
            
            if (stats) {
                actualizar_estadisticas_aritmetica(stats, estado);
            }
            
            switch (estado) {
                case ARITMETICA_EXITOSA:
                    pruebas_exitosas++;
                    break;
                case ARITMETICA_OVERFLOW:
                    overflows_detectados++;
                    printf("   ⚠️  Overflow: %d + %d\n", a, b);
                    break;
                case ARITMETICA_UNDERFLOW:
                    underflows_detectados++;
                    printf("   ⚠️  Underflow: %d + %d\n", a, b);
                    break;
                default:
                    break;
            }
        }
    }
    
    printf("\n📈 Resultados de las pruebas:\n");
    printf("   ✅ Operaciones exitosas: %zu\n", pruebas_exitosas);
    printf("   🔺 Overflows detectados: %zu\n", overflows_detectados);
    printf("   🔻 Underflows detectados: %zu\n", underflows_detectados);
    printf("   📊 Total de pruebas: %zu\n", NUM_CASOS_LIMITE * NUM_CASOS_LIMITE);
}

void comparacion_aritmetica_normal_vs_segura(void) {
    putchar('\n');
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    printf("⚖️ COMPARACIÓN: ARITMÉTICA NORMAL vs SEGURA\n");
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    
    printf("\n📋 Tabla comparativa:\n");
    printf("┌─────────────────────┬─────────────────────┬─────────────────────┐\n");
    printf("│ Característica      │ Aritmética Normal   │ Aritmética Segura   │\n");
    printf("├─────────────────────┼─────────────────────┼─────────────────────┤\n");
    printf("│ Detección overflow  │ ❌ NO               │ ✅ SÍ               │\n");
    printf("│ Comportamiento def. │ ❌ INDEFINIDO       │ ✅ PREDECIBLE       │\n");
    printf("│ Feedback de errores │ ❌ SILENCIOSO       │ ✅ EXPLÍCITO        │\n");
    printf("│ Seguridad           │ ❌ VULNERABLE       │ ✅ SEGURO           │\n");
    printf("│ Rendimiento         │ ✅ RÁPIDO           │ ⚠️  LIGERO OVERHEAD │\n");
    printf("│ Uso en producción   │ ❌ PELIGROSO        │ ✅ RECOMENDADO      │\n");
    printf("└─────────────────────┴─────────────────────┴─────────────────────┘\n");
    
    printf("\n🧪 Ejemplo lado a lado:\n");
    
    int a = INT_MAX;
    int b = 1;
    
    printf("\nOperación: %d + %d\n", a, b);
    
    // Aritmética normal (insegura)
    printf("\n❌ Aritmética normal:\n");
    int resultado_normal = a + b;  // ¡Comportamiento indefinido!
    printf("   resultado = %d\n", resultado_normal);
    printf("   ¿Es correcto? %s\n", 
           (resultado_normal > a) ? "SÍ" : "NO (¡overflow silencioso!)");
    
    // Aritmética segura
    printf("\n✅ Aritmética segura:\n");
    int resultado_seguro;
    resultado_aritmetica_t estado = suma_segura(a, b, &resultado_seguro);
    printf("   estado = %s\n", resultado_aritmetica_a_string(estado));
    printf("   acción = %s\n", 
           (estado == ARITMETICA_OVERFLOW) ? "Overflow detectado, operación bloqueada" : "Resultado válido");
}

void demostracion_completa_aritmetica_segura(void) {
    printf("\n" "🔢 EJERCICIO 096: OPERACIONES ARITMÉTICAS SEGURAS" "\n");
    for (int i = 0; i < 70; i++) putchar('=');
    putchar('\n');
    
    printf("\n📚 OBJETIVOS DE APRENDIZAJE:\n");
    printf("   • Entender los riesgos de overflow aritmético\n");
    printf("   • Aprender a detectar overflow antes de que ocurra\n");
    printf("   • Implementar operaciones aritméticas seguras\n");
    printf("   • Prevenir vulnerabilidades relacionadas con enteros\n");
    
    printf("\n⚠️  CONCEPTOS CLAVE:\n");
    printf("   • Integer Overflow: Resultado excede capacidad del tipo\n");
    printf("   • Comportamiento Indefinido: C no define qué pasa en overflow\n");
    printf("   • Verificación Preventiva: Comprobar límites antes de operar\n");
    printf("   • Validación de Entrada: Verificar rangos válidos\n");
    
    imprimir_limites_tipos_datos();
    
    printf("\n¿Desea ejecutar la demostración completa? (s/n): ");
    char respuesta;
    scanf(" %c", &respuesta);
    
    if (respuesta == 's' || respuesta == 'S') {
        demostrar_vulnerabilidades_overflow();
        
        printf("\nPresione Enter para continuar...");
        getchar();
        getchar();
        
        demostrar_operaciones_seguras();
        
        printf("\nPresione Enter para continuar...");
        getchar();
        
        estadisticas_aritmetica_t stats;
        ejecutar_pruebas_casos_limite(&stats);
        
        printf("\nPresione Enter para continuar...");
        getchar();
        
        comparacion_aritmetica_normal_vs_segura();
        
        printf("\n🎓 LECCIONES APRENDIDAS:\n");
        printf("   ✅ Overflow aritmético puede crear vulnerabilidades serias\n");
        printf("   ✅ Siempre verificar límites antes de operaciones críticas\n");
        printf("   ✅ Usar funciones seguras en código de producción\n");
        printf("   ✅ El overhead de verificación vale la pena por la seguridad\n");
        
        printf("\n💡 MEJORES PRÁCTICAS:\n");
        printf("   • Verificar límites antes de operaciones aritméticas\n");
        printf("   • Usar tipos de datos apropiados (unsigned para tamaños)\n");
        printf("   • Implementar manejo de errores para overflow\n");
        printf("   • Probar con valores límite durante desarrollo\n");
        
        printf("\n📊 Estadísticas finales:\n");
        imprimir_estadisticas_aritmetica(&stats);
    }
}

// =============================================================================
// FUNCIONES AUXILIARES
// =============================================================================

const char* resultado_aritmetica_a_string(resultado_aritmetica_t resultado) {
    switch (resultado) {
        case ARITMETICA_EXITOSA:
            return "Operación exitosa";
        case ARITMETICA_OVERFLOW:
            return "Overflow detectado";
        case ARITMETICA_UNDERFLOW:
            return "Underflow detectado";
        case ARITMETICA_DIVISION_CERO:
            return "División por cero";
        case ARITMETICA_ERROR_PARAMETRO:
            return "Error en parámetros";
        default:
            return "Resultado desconocido";
    }
}

void inicializar_estadisticas_aritmetica(estadisticas_aritmetica_t* stats) {
    if (stats == NULL) {
        return;
    }
    
    stats->operaciones_exitosas = 0;
    stats->overflows_detectados = 0;
    stats->underflows_detectados = 0;
    stats->divisiones_cero = 0;
    stats->errores_parametro = 0;
}

void actualizar_estadisticas_aritmetica(estadisticas_aritmetica_t* stats, 
                                       resultado_aritmetica_t resultado) {
    if (stats == NULL) {
        return;
    }
    
    switch (resultado) {
        case ARITMETICA_EXITOSA:
            stats->operaciones_exitosas++;
            break;
        case ARITMETICA_OVERFLOW:
            stats->overflows_detectados++;
            break;
        case ARITMETICA_UNDERFLOW:
            stats->underflows_detectados++;
            break;
        case ARITMETICA_DIVISION_CERO:
            stats->divisiones_cero++;
            break;
        case ARITMETICA_ERROR_PARAMETRO:
            stats->errores_parametro++;
            break;
    }
}

void imprimir_estadisticas_aritmetica(const estadisticas_aritmetica_t* stats) {
    if (stats == NULL) {
        return;
    }
    
    size_t total = stats->operaciones_exitosas + stats->overflows_detectados + 
                   stats->underflows_detectados + stats->divisiones_cero + 
                   stats->errores_parametro;
    
    printf("\n📈 Estadísticas de operaciones aritméticas:\n");
    printf("   ✅ Operaciones exitosas: %zu\n", stats->operaciones_exitosas);
    printf("   🔺 Overflows detectados: %zu\n", stats->overflows_detectados);
    printf("   🔻 Underflows detectados: %zu\n", stats->underflows_detectados);
    printf("   ➗ Divisiones por cero: %zu\n", stats->divisiones_cero);
    printf("   ❌ Errores de parámetro: %zu\n", stats->errores_parametro);
    printf("   📊 Total de operaciones: %zu\n", total);
    
    if (total > 0) {
        double porcentaje_exitoso = (double)stats->operaciones_exitosas / total * 100.0;
        printf("   📈 Tasa de éxito: %.1f%%\n", porcentaje_exitoso);
    }
}

void imprimir_limites_tipos_datos(void) {
    printf("\n📊 LÍMITES DE TIPOS DE DATOS:\n");
    printf("┌─────────────────┬─────────────────────┬─────────────────────┐\n");
    printf("│ Tipo            │ Mínimo              │ Máximo              │\n");
    printf("├─────────────────┼─────────────────────┼─────────────────────┤\n");
    printf("│ int             │ %20d │ %20d │\n", INT_MIN, INT_MAX);
    printf("│ unsigned int    │ %20u │ %20u │\n", 0U, UINT_MAX);
    printf("│ long long       │ %20lld │ %20lld │\n", LLONG_MIN, LLONG_MAX);
    printf("│ size_t          │ %20zu │ %20zu │\n", (size_t)0, SIZE_MAX);
    printf("└─────────────────┴─────────────────────┴─────────────────────┘\n");
}

void calcular_distancia_limites(int valor, int* distancia_max, int* distancia_min) {
    if (distancia_max) {
        *distancia_max = INT_MAX - valor;
    }
    if (distancia_min) {
        *distancia_min = valor - INT_MIN;
    }
}
