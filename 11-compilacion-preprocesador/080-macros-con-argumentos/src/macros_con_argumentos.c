#include "macros_con_argumentos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Función equivalente a la macro CUADRADO para comparación */
static int cuadrado_funcion(int x) { return x * x; }

/* Getters/setters generados a nivel de archivo (no dentro de funciones) */
static int _temperatura; 
static inline int get_temperatura(void) { return _temperatura; }
static inline void set_temperatura(int valor) { _temperatura = valor; }

// Variables globales para demostrar efectos secundarios
int contador_llamadas_funcion = 0;
int contador_evaluaciones = 0;

// ============================================================================
// FUNCIONES AUXILIARES PARA DEMOSTRACIÓN
// ============================================================================

int sumar_uno(void) {
    contador_llamadas_funcion++;
    printf("  sumarUno() llamado (llamada #%d)\n", contador_llamadas_funcion);
    return 1;
}

int funcion_con_efecto_secundario(void) {
    static int valor = 0;
    valor++;
    printf("  funcion_con_efecto_secundario() llamada, retorna %d\n", valor);
    return valor;
}

int numero_aleatorio(void) {
    int num = (rand() % 10) + 1;
    printf("  numero_aleatorio() generó: %d\n", num);
    return num;
}

int operacion_costosa(void) {
    printf("  operacion_costosa() iniciada...\n");
    usleep(100000); // Pausa de 0.1 segundos
    printf("  operacion_costosa() completada\n");
    return 42;
}

// ============================================================================
// FUNCIÓN PRINCIPAL DEL ENUNCIADO
// ============================================================================

int main_usuario_basico(void) {
    printf("=== EJEMPLO BÁSICO DEL ENUNCIADO ===\n\n");
    
    int a = 5;
    printf("CUADRADO(a) = %d\n", CUADRADO(a));         // 25
    printf("CUADRADO(3 + 1) = %d\n", CUADRADO(3 + 1)); // ((3 + 1) * (3 + 1)) = 16
    
    printf("\nAhora probando con función que tiene efecto secundario:\n");
    contador_llamadas_funcion = 0; // Resetear contador
    printf("CUADRADO(sumar_uno()) = %d\n", CUADRADO(sumar_uno())); // Llama sumar_uno() dos veces
    
    return 0;
}

void ejecutar_ejemplo_enunciado(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    EJEMPLO DEL ENUNCIADO                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Macro definida: #define CUADRADO(x) ((x)*(x))\n\n");

    // Resetear contadores
    contador_llamadas_funcion = 0;
    
    // Caso 1: Variable simple
    printf("1. Probando con variable simple:\n");
    int a = 5;
    printf("   int a = %d;\n", a);
    printf("   CUADRADO(a) = %d\n", CUADRADO(a));
    printf("   ✓ Resultado correcto: %d * %d = %d\n\n", a, a, a*a);

    // Caso 2: Expresión aritmética
    printf("2. Probando con expresión aritmética:\n");
    printf("   CUADRADO(3 + 1) expande a ((3 + 1) * (3 + 1))\n");
    printf("   CUADRADO(3 + 1) = %d\n", CUADRADO(3 + 1));
    printf("   ✓ Resultado correcto: 4 * 4 = 16\n\n");

    // Caso 3: Función con efecto secundario
    printf("3. Probando con función que tiene efecto secundario:\n");
    printf("   CUADRADO(sumar_uno()) expande a ((sumar_uno()) * (sumar_uno()))\n");
    printf("   Esto causa que sumar_uno() se llame DOS veces:\n");
    printf("   CUADRADO(sumar_uno()) = %d\n", CUADRADO(sumar_uno()));
    printf("   ⚠️  PROBLEMA: La función se llamó %d veces en lugar de 1\n\n", contador_llamadas_funcion);

    // Caso 4: Expresión con incremento (muy problemático)
    printf("4. Probando con incremento (MUY PROBLEMÁTICO):\n");
    int b = 5;
    printf("   int b = %d;\n", b);
    printf("   CUADRADO(++b) expande a ((++b) * (++b))\n");
    printf("   Antes: b = %d\n", b);
    /* Demostración segura para evitar UB en compilación estricta */
    int bb = b + 1;
    int resultado = CUADRADO(bb);
    b = bb;
    printf("   CUADRADO(++b) (simulado seguro) = %d\n", resultado);
    printf("   Después: b = %d\n", b);
    printf("   Nota: en la macro real se evaluaría dos veces.\n\n");
}

// ============================================================================
// DEMOSTRACIONES EDUCATIVAS
// ============================================================================

void demo_macros_basicas(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      MACROS BÁSICAS                             ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Comparando diferentes macros matemáticas:\n\n");

    int x = 3;
    printf("Con x = %d:\n", x);
    printf("  CUADRADO(x) = %d\n", CUADRADO(x));
    printf("  CUBO(x) = %d\n", CUBO(x));
    printf("  CUARTA_POTENCIA(x) = %d\n", CUARTA_POTENCIA(x));

    printf("\nCon expresiones:\n");
    printf("  CUADRADO(2 + 1) = %d (esperado: 9)\n", CUADRADO(2 + 1));
    printf("  CUBO(1 + 2) = %d (esperado: 27)\n", CUBO(1 + 2));

    printf("\nMacros de comparación:\n");
    int a = 10, b = 7;
    printf("  a = %d, b = %d\n", a, b);
    printf("  MAXIMO(a, b) = %d\n", MAXIMO(a, b));
    printf("  MINIMO(a, b) = %d\n", MINIMO(a, b));
    printf("  LIMITAR(15, 5, 12) = %d\n", LIMITAR(15, 5, 12));
}

void demo_macros_problematicas(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                  MACROS PROBLEMÁTICAS                           ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Demostrando por qué necesitamos paréntesis en las macros:\n\n");

    printf("1. Macro SIN paréntesis (INCORRECTA):\n");
    printf("   #define CUADRADO_MALO(x) x * x\n");
    printf("   CUADRADO_MALO(3 + 1) expande a: 3 + 1 * 3 + 1 = 3 + 3 + 1 = 7\n");
    printf("   CUADRADO_MALO(3 + 1) = %d ❌ (esperado: 16)\n\n", CUADRADO_MALO(3 + 1));

    printf("2. Macro CON paréntesis (CORRECTA):\n");
    printf("   #define CUADRADO(x) ((x) * (x))\n");
    printf("   CUADRADO(3 + 1) expande a: ((3 + 1) * (3 + 1)) = 4 * 4 = 16\n");
    printf("   CUADRADO(3 + 1) = %d ✓\n\n", CUADRADO(3 + 1));

    printf("3. Problema con operadores de precedencia:\n");
    printf("   SUMA_MALA(2, 3) * 4 con #define SUMA_MALA(a, b) a + b\n");
    printf("   Expande a: 2 + 3 * 4 = 2 + 12 = 14\n");
    printf("   SUMA_MALA(2, 3) * 4 = %d ❌ (esperado: 20)\n", SUMA_MALA(2, 3) * 4);
    printf("   SUMA(2, 3) * 4 = %d ✓ (con paréntesis)\n\n", SUMA(2, 3) * 4);

    printf("4. Problema con operadores ternarios:\n");
    printf("   !MAXIMO_MALO(3, 5) con #define MAXIMO_MALO(a, b) a > b ? a : b\n");
    printf("   Expande a: !3 > 5 ? 3 : 5 = 0 > 5 ? 3 : 5 = 5\n");
    int x3 = 3, y5 = 5; /* evitar evaluación constante en compilación */
    printf("   !(MAXIMO_MALO(3, 5)) = %d ❌ (esperado: 0)\n", !(MAXIMO_MALO(x3, y5)));
    printf("   !MAXIMO(3, 5) = %d ✓ (con paréntesis)\n", !MAXIMO(3, 5));
}

void demo_efectos_secundarios(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   EFECTOS SECUNDARIOS                           ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Las macros evalúan sus argumentos MÚLTIPLES VECES.\n");
    printf("Esto puede causar problemas con:\n");
    printf("- Funciones con efectos secundarios\n");
    printf("- Operadores de incremento/decremento\n");
    printf("- Funciones costosas\n\n");

    // Resetear contadores
    contador_llamadas_funcion = 0;

    printf("1. Función con efecto secundario:\n");
    printf("   CUADRADO(funcion_con_efecto_secundario())\n");
    int resultado1 = CUADRADO(funcion_con_efecto_secundario());
    printf("   Resultado: %d\n", resultado1);
    printf("   Problema: La función se llamó múltiples veces\n\n");

    printf("2. Incremento/decremento:\n");
    int i = 5;
    printf("   i = %d\n", i);
    printf("   CUADRADO(i++) evalúa i++ dos veces (simulado seguro):\n");
    int resultado2 = CUADRADO(i);
    i++;
    printf("   Resultado: %d\n", resultado2);
    printf("   i después: %d (se incrementó %d veces)\n\n", i, i - 5);

    printf("3. Función aleatoria:\n");
    srand(time(NULL));
    printf("   CUADRADO(numero_aleatorio()) - cada llamada da un número diferente:\n");
    int resultado3 = CUADRADO(numero_aleatorio());
    printf("   Resultado: %d (¿es realmente un cuadrado perfecto?)\n\n", resultado3);

    printf("4. Operación costosa:\n");
    printf("   CUADRADO(operacion_costosa()) - la función se ejecuta múltiples veces:\n");
    int resultado4 = CUADRADO(operacion_costosa());
    printf("   Resultado: %d\n", resultado4);
    printf("   Problema: La operación costosa se ejecutó dos veces\n");
}

void demo_macros_matematicas(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                 MACROS MATEMÁTICAS AVANZADAS                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    double x = 4.0;
    printf("Con x = %.1f:\n", x);
    printf("  CUADRADO(x) = %.1f\n", CUADRADO(x));
    printf("  CUBO(x) = %.1f\n", CUBO(x));
    printf("  RAIZ_CUADRADA(x) = %.2f\n", RAIZ_CUADRADA(x));
    printf("  POTENCIA_ENTERO(x, 3) = %.1f\n", POTENCIA_ENTERO(x, 3));

    printf("\nMacros con múltiples argumentos:\n");
    printf("  SUMA(3, 7) = %d\n", SUMA(3, 7));
    printf("  RESTA(10, 4) = %d\n", RESTA(10, 4));
    printf("  MULTIPLICACION(6, 7) = %d\n", MULTIPLICACION(6, 7));
    printf("  DIVISION_SEGURA(15, 3) = %d\n", DIVISION_SEGURA(15, 3));
    printf("  DIVISION_SEGURA(15, 0) = %d (división por cero protegida)\n", DIVISION_SEGURA(15, 0));

    printf("\nMacros de valor absoluto:\n");
    printf("  VALOR_ABSOLUTO(-5) = %d\n", VALOR_ABSOLUTO(-5));
    printf("  VALOR_ABSOLUTO(3) = %d\n", VALOR_ABSOLUTO(3));
    
    // Demostrar problema con valor absoluto
    printf("\n  ⚠️  Problema con VALOR_ABSOLUTO(i++):\n");
    int i = -2;
    printf("     i = %d\n", i);
    printf("     VALOR_ABSOLUTO(i++) = %d\n", VALOR_ABSOLUTO(i++));
    printf("     i después = %d (incrementado múltiples veces)\n", i);
}

void demo_macros_utilidad(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   MACROS DE UTILIDAD                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("1. Macros para arrays:\n");
    int numeros[] = {1, 2, 3, 4, 5};
    printf("   Array: {1, 2, 3, 4, 5}\n");
    printf("   LONGITUD_ARRAY(numeros) = %zu\n\n", LONGITUD_ARRAY(numeros));

    printf("2. Intercambio de variables:\n");
    int a = 10, b = 20;
    printf("   Antes: a = %d, b = %d\n", a, b);
    INTERCAMBIAR(a, b);
    printf("   Después: a = %d, b = %d\n\n", a, b);

    printf("3. Macros condicionales:\n");
    int num = 8;
    printf("   num = %d\n", num);
    printf("   ES_PAR(num) = %s\n", ES_PAR(num) ? "true" : "false");
    printf("   ES_IMPAR(num) = %s\n", ES_IMPAR(num) ? "true" : "false");
    printf("   ES_POSITIVO(num) = %s\n", ES_POSITIVO(num) ? "true" : "false");

    printf("\n4. Macros de debugging:\n");
    int variable_debug = 42;
    float valor_float = 3.14159f;
    char* cadena = "Hola mundo";
    
    printf("   Usando macros de debug:\n");
    DEBUG_PRINT(variable_debug);
    DEBUG_FLOAT(valor_float);
    DEBUG_STRING(cadena);

    printf("\n5. Stringify y concatenación:\n");
    printf("   STRINGIFY(CUADRADO) = \"%s\"\n", STRINGIFY(CUADRADO));
    printf("   CONCATENAR_STR(\"Hola \", \"mundo\") = \"%s\"\n", CONCATENAR_STR("Hola ", "mundo"));
}

void demo_macros_vs_funciones(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                  MACROS vs FUNCIONES                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("Diferencias principales:\n\n");
    
    printf("1. EVALUACIÓN:\n");
    printf("   - Macros: Evaluación múltiple de argumentos\n");
    printf("   - Funciones: Evaluación única de argumentos\n\n");

    printf("2. TIEMPO DE RESOLUCIÓN:\n");
    printf("   - Macros: Tiempo de compilación (preprocesador)\n");
    printf("   - Funciones: Tiempo de ejecución\n\n");

    printf("3. TIPO DE DATOS:\n");
    printf("   - Macros: Sin verificación de tipos\n");
    printf("   - Funciones: Verificación estricta de tipos\n\n");

    printf("4. DEPURACIÓN:\n");
    printf("   - Macros: Más difícil de depurar\n");
    printf("   - Funciones: Fácil de depurar\n\n");

    printf("5. RENDIMIENTO:\n");
    printf("   - Macros: Sin overhead de llamada\n");
    printf("   - Funciones: Overhead de llamada (stack)\n\n");

    // Demostración práctica
    printf("Demostración práctica:\n\n");
    
    printf("Comparando macro vs función:\n");
    int val = 5;
    printf("  Valor: %d\n", val);
    printf("  CUADRADO(val) = %d (macro)\n", CUADRADO(val));
    printf("  cuadrado_funcion(val) = %d (función)\n", cuadrado_funcion(val));

    printf("\nCon función con efecto secundario:\n");
    contador_llamadas_funcion = 0;
    printf("  CUADRADO(sumar_uno()):\n");
    int resultado_macro = CUADRADO(sumar_uno());
    printf("    Resultado: %d, Llamadas: %d\n", resultado_macro, contador_llamadas_funcion);
    
    contador_llamadas_funcion = 0;
    printf("  cuadrado_funcion(sumar_uno()):\n");
    int resultado_funcion = cuadrado_funcion(sumar_uno());
    printf("    Resultado: %d, Llamadas: %d\n", resultado_funcion, contador_llamadas_funcion);
}

void demo_macros_avanzadas(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   MACROS AVANZADAS                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("1. Macros variádicas (C99+):\n");
    PRINTF_DEBUG("%s", "Este es un mensaje de debug");
    PRINTF_DEBUG("Valor: %d, Texto: %s", 42, "prueba");

    printf("\n2. Macro de medición de tiempo:\n");
    printf("   Midiendo tiempo de operación simple:\n");
    MEDIR_TIEMPO({
        int suma = 0;
        for (int i = 0; i < 1000000; i++) {
            suma += i;
        }
        printf("   Suma calculada: %d\n", suma);
    });

    printf("\n3. Macro de assertion personalizada:\n");
    printf("   ASSERT_CUSTOM(5 > 3, \"Cinco debería ser mayor que tres\"):\n");
    ASSERT_CUSTOM(5 > 3, "%s", "Cinco debería ser mayor que tres");
    printf("   ✓ Assertion pasó correctamente\n");

    printf("\n4. Macro para crear getters/setters:\n");
    set_temperatura(25);
    printf("   Temperatura establecida: %d°C\n", get_temperatura());

    printf("\n5. Expansión de macros complejas:\n");
    printf("   Mostrando cómo se expande una macro:\n");
    MOSTRAR_EXPANSION(CUADRADO(3 + 2));
}

void ejecutar_todas_demos(void) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║              MACROS CON ARGUMENTOS - DEMOSTRACIÓN COMPLETA      ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");

    ejecutar_ejemplo_enunciado();
    demo_macros_basicas();
    demo_macros_problematicas();
    demo_efectos_secundarios();
    demo_macros_matematicas();
    demo_macros_utilidad();
    demo_macros_vs_funciones();
    demo_macros_avanzadas();

    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                           CONCLUSIONES                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("LECCIONES APRENDIDAS:\n\n");
    printf("✓ Las macros realizan sustitución textual\n");
    printf("✓ Los argumentos se evalúan múltiples veces\n");
    printf("✓ Siempre usar paréntesis en macros\n");
    printf("✓ Cuidado con efectos secundarios\n");
    printf("✓ Las macros no tienen tipo\n");
    printf("✓ Son más rápidas pero menos seguras que funciones\n");
    printf("✓ Útiles para código genérico y optimizaciones\n\n");
    
    printf("¡Demostración completada!\n");
}

// Función principal
#ifdef COMPILAR_MAIN_BASICO
int main(void) {
    return main_usuario_basico();
}
#elif defined(COMPILAR_DEMO_COMPLETO)
int main(void) {
    ejecutar_todas_demos();
    return 0;
}
#else
// Por defecto, usar la demostración completa
int main(void) {
    ejecutar_todas_demos();
    return 0;
}
#endif
