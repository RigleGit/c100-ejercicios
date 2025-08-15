#ifndef CONST_CORRECTNESS_H
#define CONST_CORRECTNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estructuras de ejemplo para demostrar const-correctness
typedef struct {
    int id;
    char nombre[50];
    double salario;
} empleado_t;

typedef struct {
    const char *titulo;
    const char *autor;
    int paginas;
    double precio;
} libro_t;
// =============================================================================

// Tipos auxiliares usados por las funciones de configuración y estadísticas
typedef struct {
    const int id;                 // Campo inmutable
    char nombre[64];              // Modificable
    const double factor_conversion; // Campo inmutable
    bool activo;                  // Modificable
} configuracion_t;

typedef struct {
    size_t funciones_analizadas;
    size_t parametros_const;
    size_t variables_const;
    size_t mejoras_detectadas;
} estadisticas_const_t;

/**
 * @brief FUNCIÓN INCORRECTA: Calcula longitud sin const
 * @warning Esta función debería usar const char* para el parámetro
 * @details Demuestra cómo la falta de const hace el código menos claro
 *          y puede permitir modificaciones no intencionadas
 * 
 * @param cadena Cadena a medir (NO DEBE MODIFICARSE)
 * @return Longitud de la cadena
 */
int longitud_cadena_incorrecto(char* cadena);

/**
 * @brief FUNCIÓN INCORRECTA: Imprime mensaje sin const
 * @warning Debería usar const char* para claridad
 * 
 * @param mensaje Mensaje a imprimir (NO DEBE MODIFICARSE)
 */
void imprimir_mensaje_incorrecto(char* mensaje);

/**
 * @brief FUNCIÓN INCORRECTA: Busca carácter sin const
 * @warning Los parámetros de entrada deberían ser const
 * 
 * @param cadena Cadena donde buscar
 * @param caracter Carácter a buscar
 * @return Posición del carácter o -1 si no se encuentra
 */
int buscar_caracter_incorrecto(char* cadena, char caracter);
// Prototipos de las versiones correctas usadas por main/tests
size_t longitud_cadena_correcto(const char* cadena);
void imprimir_mensaje_correcto(const char* mensaje);
int buscar_caracter_correcto(const char* cadena, char caracter);
bool comparar_cadenas_correcto(const char* cadena1, const char* cadena2);
bool copiar_cadena_correcto(char* destino, const char* origen, size_t tamaño_destino);
int sumar_array_const_data(const int* ptr_a_const, size_t tamaño);
void llenar_array_const_ptr(int* const array, size_t tamaño, int valor);
int encontrar_maximo_const_completo(const int* const ptr_const_a_const_data, size_t tamaño);
/**
 * @brief Ejemplo de función SIN const-correctness (insegura)
 * @param texto Cadena que puede ser modificada accidentalmente
 * @param empleado Estructura que puede ser alterada
 * @return Longitud del texto
 */
int funcion_sin_const(char *texto, empleado_t *empleado);

/**
 * @brief Ejemplo de función CON const-correctness (segura)
 * @param texto Cadena de solo lectura
 * @param empleado Estructura de solo lectura
 * @return Longitud del texto
 */
int funcion_con_const(const char *texto, const empleado_t *empleado);

/**
 * @brief Función que NO modifica una cadena - debe usar const
 * @param cadena Cadena de entrada (solo lectura)
 * @return Número de vocales en la cadena
 */
int contar_vocales(const char *cadena);

/**
 * @brief Función que SÍ modifica una cadena - NO debe usar const
 * @param cadena Cadena a modificar (convertir a mayúsculas)
 */
void convertir_mayusculas(char *cadena);

/**
 * @brief Función que lee datos de un empleado - debe usar const
 * @param empleado Empleado de solo lectura
 */
void mostrar_empleado(const empleado_t *empleado);

/**
 * @brief Función que modifica datos de un empleado - NO debe usar const
 * @param empleado Empleado a modificar
 * @param nuevo_salario Nuevo salario a asignar
 */
void actualizar_salario(empleado_t *empleado, double nuevo_salario);

/**
 * @brief Función que busca empleado por ID - retorna const para proteger datos
 * @param empleados Array de empleados (solo lectura)
 * @param num_empleados Número de empleados
 * @param id ID a buscar
 * @return Puntero const al empleado encontrado, NULL si no se encuentra
 */
const empleado_t *buscar_empleado(const empleado_t *empleados, int num_empleados, int id);

/**
 * @brief Función que permite modificar empleado encontrado
 * @param empleados Array de empleados (modificable)
 * @param num_empleados Número de empleados
 * @param id ID a buscar
 * @return Puntero modificable al empleado encontrado, NULL si no se encuentra
 */
empleado_t *buscar_empleado_modificable(empleado_t *empleados, int num_empleados, int id);

/**
 * @brief Función con const en el valor de retorno
 * @param libro Información del libro
 * @return Puntero const al título del libro
 */
const char *obtener_titulo_libro(const libro_t *libro);

/**
 * @brief Función que demuestra const con arrays
 * @param numeros Array de números (solo lectura)
 * @param tamaño Tamaño del array
 * @return Suma de todos los números
 */
int sumar_array(const int *numeros, int tamaño);

/**
 * @brief Función que modifica array - sin const
 * @param numeros Array de números a modificar
 * @param tamaño Tamaño del array
 * @param factor Factor por el que multiplicar cada elemento
 */
void multiplicar_array(int *numeros, int tamaño, int factor);

/**
 * @brief Función con parámetro const que retorna no-const
 * @param origen Cadena origen (solo lectura)
 * @return Nueva copia de la cadena (modificable)
 */
char *duplicar_cadena(const char *origen);

/**
 * @brief Función que demuestra const con estructuras anidadas
 * @param libros Array de libros (solo lectura)
 * @param num_libros Número de libros
 * @return Precio promedio de los libros
 */
double calcular_precio_promedio(const libro_t *libros, int num_libros);

/**
 * @brief Función helper para crear empleado
 * @param id ID del empleado
 * @param nombre Nombre del empleado
 * @param salario Salario del empleado
 * @return Empleado inicializado
 */
empleado_t crear_empleado(int id, const char *nombre, double salario);

/**
 * @brief Función helper para crear libro
 * @param titulo Título del libro
 * @param autor Autor del libro
 * @param paginas Número de páginas
 * @param precio Precio del libro
 * @return Libro inicializado
 */
libro_t crear_libro(const char *titulo, const char *autor, int paginas, double precio);

/**
 * @brief Demuestra diferentes usos de const
 */
void demostrar_const_correctness(void);

/**
 * @brief Muestra ejemplos de código inseguro vs seguro
 */
void mostrar_ejemplos_comparativos(void);

// ==== Configuración y utilidades relacionadas ====
bool inicializar_configuracion(configuracion_t* config, int id,
                               const char* nombre, double factor);
void imprimir_configuracion(const configuracion_t* config);
bool validar_configuracion(const configuracion_t* config);
bool cambiar_estado_configuracion(configuracion_t* config, bool nuevo_estado);

// ==== Análisis y demostración ====
void demostrar_problemas_sin_const(void);
void demostrar_beneficios_const_correct(void);
void comparacion_incorrecto_vs_correcto(void);
void demostrar_tipos_const(void);
void analizar_const_correctness(estadisticas_const_t* stats);
void ejercicios_interactivos_const(void);
void demostracion_completa_const_correctness(void);

// ==== Auxiliares ====
void inicializar_estadisticas_const(estadisticas_const_t* stats);
void imprimir_estadisticas_const(const estadisticas_const_t* stats);
bool validar_puntero_no_nulo(const void* ptr, const char* nombre_variable);
size_t contar_caracter(const char* cadena, char caracter);

// ==== Macros de ayuda ====
#define DECLARE_READONLY_FUNC(func_name, param_type, param_name) \
    void func_name(const param_type* param_name)

#define STATIC_ASSERT_CONST(expr) \
    _Static_assert(expr, "Violación de const-correctness detectada")

// ==== Constantes para pruebas y demos ====
extern const char* const CADENAS_PRUEBA[];
extern const size_t NUM_CADENAS_PRUEBA;
extern const configuracion_t CONFIG_DEFAULT;
extern const int VALORES_PRUEBA[];
extern const size_t NUM_VALORES_PRUEBA;

#endif // CONST_CORRECTNESS_H
