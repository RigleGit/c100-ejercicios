#ifndef MACROS_CON_ARGUMENTOS_H
#define MACROS_CON_ARGUMENTOS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// ============================================================================
// MACROS BÁSICAS (del enunciado)
// ============================================================================

/**
 * @brief Macro básica para calcular el cuadrado de un número
 * @param x Valor a elevar al cuadrado
 * @return El cuadrado de x
 * @note Evaluación múltiple: x se evalúa dos veces
 */
#define CUADRADO(x) ((x) * (x))

// ============================================================================
// MACROS MATEMÁTICAS AVANZADAS
// ============================================================================

/**
 * @brief Macros para operaciones matemáticas básicas
 */
#define CUBO(x) ((x) * (x) * (x))
#define CUARTA_POTENCIA(x) (CUADRADO(x) * CUADRADO(x))
#define SUMA(a, b) ((a) + (b))
#define RESTA(a, b) ((a) - (b))
#define MULTIPLICACION(a, b) ((a) * (b))
#define DIVISION_SEGURA(a, b) ((b) != 0 ? (a) / (b) : 0)

/**
 * @brief Macros con múltiples evaluaciones (problemáticas)
 */
#define POTENCIA_ENTERO(base, exp) (pow((base), (exp)))
#define RAIZ_CUADRADA(x) (sqrt((x)))
#define VALOR_ABSOLUTO(x) ((x) < 0 ? -(x) : (x))

/**
 * @brief Macros de comparación
 */
#define MAXIMO(a, b) ((a) > (b) ? (a) : (b))
#define MINIMO(a, b) ((a) < (b) ? (a) : (b))
#define LIMITAR(valor, min_val, max_val) \
    ((valor) < (min_val) ? (min_val) : (valor) > (max_val) ? (max_val) : (valor))

// ============================================================================
// MACROS PROBLEMÁTICAS (para demostrar efectos secundarios)
// ============================================================================

/**
 * @brief Versiones SIN paréntesis (INCORRECTAS - solo para demostración)
 * @warning Estas macros son incorrectas y solo se usan para mostrar problemas
 */
#define CUADRADO_MALO(x) x * x
#define SUMA_MALA(a, b) a + b
#define MAXIMO_MALO(a, b) a > b ? a : b

/**
 * @brief Macros que causan efectos secundarios evidentes
 */
#define INCREMENTAR_Y_CUADRADO(x) (++(x) * ++(x))
#define DOBLE_INCREMENTO(x) ((x)++ + (x)++)
#define PRE_POST_INCREMENTO(x) (++(x) + (x)++)

// ============================================================================
// MACROS DE UTILIDAD
// ============================================================================

/**
 * @brief Macros para trabajar con arrays
 */
#define LONGITUD_ARRAY(arr) (sizeof(arr) / sizeof((arr)[0]))
/* Compatible con C99: pensado para enteros en estas demos */
#define INTERCAMBIAR(a, b) do { int temp = (a); (a) = (b); (b) = temp; } while(0)

/**
 * @brief Macros para strings
 */
#define CONCATENAR_STR(str1, str2) str1 str2
#define STRINGIFY(x) #x
#define PASTE(a, b) a##b

/**
 * @brief Macros condicionales
 */
#define ES_PAR(x) (((x) % 2) == 0)
#define ES_IMPAR(x) (((x) % 2) != 0)
#define ES_POSITIVO(x) ((x) > 0)
#define ES_NEGATIVO(x) ((x) < 0)
#define ES_CERO(x) ((x) == 0)

/**
 * @brief Macros para debugging
 */
#define DEBUG_PRINT(var) printf(#var " = %d\n", (var))
#define DEBUG_FLOAT(var) printf(#var " = %.2f\n", (var))
#define DEBUG_STRING(var) printf(#var " = \"%s\"\n", (var))

/**
 * @brief Macro para medir tiempo de ejecución
 */
#define MEDIR_TIEMPO(codigo) do { \
    clock_t inicio = clock(); \
    codigo; \
    clock_t fin = clock(); \
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC; \
    printf("Tiempo de ejecución: %.6f segundos\n", tiempo); \
} while(0)

// ============================================================================
// MACROS AVANZADAS
// ============================================================================

/**
 * @brief Macros variádicas (C99+)
 */
#define PRINTF_DEBUG(formato, ...) \
    printf("[DEBUG] " formato "\n", __VA_ARGS__)

#define ASSERT_CUSTOM(condicion, mensaje, ...) do { \
    if (!(condicion)) { \
        printf("[ASSERTION FAILED] " mensaje "\n", __VA_ARGS__); \
        exit(1); \
    } \
} while(0)

/**
 * @brief Macros de optimización
 */
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

/**
 * @brief Macros para tipos de datos
 */
#define CREAR_GETTER_SETTER(tipo, nombre) \
    static tipo _##nombre; \
    static inline tipo get_##nombre(void) { return _##nombre; } \
    static inline void set_##nombre(tipo valor) { _##nombre = valor; }

// ============================================================================
// CONSTANTES PARA TESTS
// ============================================================================

#define NUMERO_TESTS 1000
#define VALOR_PEQUENO 5
#define VALOR_GRANDE 100
#define PRECISION_FLOAT 0.0001

// ============================================================================
// CONTADORES GLOBALES (para demostrar efectos secundarios)
// ============================================================================

extern int contador_llamadas_funcion;
extern int contador_evaluaciones;

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// ============================================================================

/**
 * @brief Función que incrementa un contador al ser llamada
 * @return Siempre devuelve 1
 */
int sumar_uno(void);

/**
 * @brief Función que muestra cuándo es llamada
 * @return Un valor incremental
 */
int funcion_con_efecto_secundario(void);

/**
 * @brief Función que retorna un número aleatorio
 * @return Número aleatorio entre 1 y 10
 */
int numero_aleatorio(void);

/**
 * @brief Función que simula una operación costosa
 * @return Un valor calculado tras una pausa
 */
int operacion_costosa(void);

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN EDUCATIVA
// ============================================================================

/**
 * @brief Demuestra el uso básico de macros con argumentos
 */
void demo_macros_basicas(void);

/**
 * @brief Demuestra problemas con macros sin paréntesis
 */
void demo_macros_problematicas(void);

/**
 * @brief Demuestra efectos secundarios en macros
 */
void demo_efectos_secundarios(void);

/**
 * @brief Demuestra macros matemáticas avanzadas
 */
void demo_macros_matematicas(void);

/**
 * @brief Demuestra macros de utilidad
 */
void demo_macros_utilidad(void);

/**
 * @brief Demuestra diferencias entre macros y funciones
 */
void demo_macros_vs_funciones(void);

/**
 * @brief Demuestra macros avanzadas (variádicas, etc.)
 */
void demo_macros_avanzadas(void);

/**
 * @brief Ejecuta el ejemplo básico del enunciado
 */
void ejecutar_ejemplo_enunciado(void);

/**
 * @brief Ejecuta todas las demostraciones
 */
void ejecutar_todas_demos(void);

/**
 * @brief Función principal del usuario (versión básica)
 */
int main_usuario_basico(void);

// ============================================================================
// MACROS DE INFORMACIÓN ÚTIL
// ============================================================================

/**
 * @brief Macro para mostrar expansión de otra macro
 */
#define MOSTRAR_EXPANSION(macro_call) \
    printf("Expansión de " #macro_call ": "); \
    printf("Resultado = %d\n", macro_call)

/**
 * @brief Macro para mostrar información de evaluación
 */
#define EVALUAR_Y_CONTAR(expr) ({ \
    contador_evaluaciones++; \
    printf("Evaluando: " #expr " (evaluación #%d)\n", contador_evaluaciones); \
    (expr); \
})

#endif // MACROS_CON_ARGUMENTOS_H
