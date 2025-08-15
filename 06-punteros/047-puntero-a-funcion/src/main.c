#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Función básica que suma dos enteros
 * @param a Primer entero
 * @param b Segundo entero
 * @return La suma de a y b
 */
int sumar(int a, int b) {
    return a + b;
}

/**
 * Función que resta dos enteros
 * @param a Primer entero
 * @param b Segundo entero
 * @return La resta de a - b
 */
int restar(int a, int b) {
    return a - b;
}

/**
 * Función que multiplica dos enteros
 * @param a Primer entero
 * @param b Segundo entero
 * @return El producto de a * b
 */
int multiplicar(int a, int b) {
    return a * b;
}

/**
 * Función que divide dos enteros (división entera)
 * @param a Dividendo
 * @param b Divisor
 * @return El cociente de a / b (0 si b es 0)
 */
int dividir(int a, int b) {
    if (b == 0) {
        printf("Error: División por cero\n");
        return 0;
    }
    return a / b;
}

/**
 * Función que calcula el módulo
 * @param a Dividendo
 * @param b Divisor
 * @return El resto de a % b (0 si b es 0)
 */
int modulo(int a, int b) {
    if (b == 0) {
        printf("Error: División por cero\n");
        return 0;
    }
    return a % b;
}

/**
 * Función que calcula la potencia (base^exponente)
 * @param base Base
 * @param exponente Exponente
 * @return base elevado a exponente
 */
int potencia(int base, int exponente) {
    if (exponente < 0) return 0;  // Para enteros, potencias negativas son 0
    
    int resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    return resultado;
}

/**
 * Función que calcula el máximo de dos números
 * @param a Primer número
 * @param b Segundo número
 * @return El mayor de a y b
 */
int maximo(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * Función que calcula el mínimo de dos números
 * @param a Primer número
 * @param b Segundo número
 * @return El menor de a y b
 */
int minimo(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * Función que calcula el MCD usando algoritmo de Euclides
 * @param a Primer número
 * @param b Segundo número
 * @return El máximo común divisor de a y b
 */
int mcd(int a, int b) {
    a = abs(a);  // Trabajar con valores absolutos
    b = abs(b);
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/**
 * Función que calcula el MCM (Mínimo Común Múltiplo)
 * @param a Primer número
 * @param b Segundo número
 * @return El mínimo común múltiplo de a y b
 */
int mcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return abs(a * b) / mcd(a, b);
}

/**
 * Estructura para asociar operaciones con sus nombres
 */
typedef struct {
    char nombre[20];
    int (*funcion)(int, int);
    char simbolo;
} Operacion;

/**
 * Calculadora que usa punteros a funciones
 * @param a Primer operando
 * @param b Segundo operando
 * @param operacion Puntero a la función de operación
 * @param nombre Nombre de la operación
 * @return El resultado de la operación
 */
int calculadora(int a, int b, int (*operacion)(int, int), const char *nombre) {
    printf("Ejecutando %s(%d, %d):\n", nombre, a, b);
    int resultado = operacion(a, b);
    printf("  Resultado: %d\n\n", resultado);
    return resultado;
}

/**
 * Aplica una operación a un array de pares de números
 * @param pares Array de pares [a1, b1, a2, b2, ...]
 * @param num_pares Número de pares
 * @param operacion Puntero a la función de operación
 * @param nombre Nombre de la operación
 */
void aplicar_operacion_array(int pares[][2], int num_pares, 
                           int (*operacion)(int, int), const char *nombre) {
    printf("Aplicando %s a array de pares:\n", nombre);
    for (int i = 0; i < num_pares; i++) {
        int resultado = operacion(pares[i][0], pares[i][1]);
        printf("  %s(%d, %d) = %d\n", nombre, pares[i][0], pares[i][1], resultado);
    }
    printf("\n");
}

/**
 * Función que recibe un array de punteros a funciones y los ejecuta
 * @param a Primer operando
 * @param b Segundo operando
 * @param funciones Array de punteros a funciones
 * @param nombres Array de nombres de las funciones
 * @param num_funciones Número de funciones
 */
void ejecutar_funciones(int a, int b, int (*funciones[])(int, int), 
                       const char *nombres[], int num_funciones) {
    printf("Ejecutando múltiples operaciones con %d y %d:\n", a, b);
    for (int i = 0; i < num_funciones; i++) {
        int resultado = funciones[i](a, b);
        printf("  %s: %d\n", nombres[i], resultado);
    }
    printf("\n");
}

/**
 * Función que selecciona dinámicamente una operación según un carácter
 * @param a Primer operando
 * @param b Segundo operando
 * @param operador Carácter que indica la operación
 * @return El resultado de la operación
 */
int calculadora_dinamica(int a, int b, char operador) {
    // Array de operaciones disponibles
    Operacion operaciones[] = {
        {"+", sumar, '+'},
        {"-", restar, '-'},
        {"*", multiplicar, '*'},
        {"/", dividir, '/'},
        {"%", modulo, '%'},
        {"^", potencia, '^'},
        {"max", maximo, 'M'},
        {"min", minimo, 'm'},
        {"mcd", mcd, 'g'},
        {"mcm", mcm, 'l'}
    };
    
    int num_operaciones = sizeof(operaciones) / sizeof(operaciones[0]);
    
    for (int i = 0; i < num_operaciones; i++) {
        if (operaciones[i].simbolo == operador) {
            printf("Operación seleccionada: %s\n", operaciones[i].nombre);
            return operaciones[i].funcion(a, b);
        }
    }
    
    printf("Error: Operador '%c' no reconocido\n", operador);
    return 0;
}

/**
 * Función de comparación para ordenamiento
 * @param a Primer número
 * @param b Segundo número
 * @return 1 si a > b, -1 si a < b, 0 si son iguales
 */
int comparar(int a, int b) {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}

/**
 * Función que ordena un array usando una función de comparación
 * @param array Array a ordenar
 * @param tamaño Tamaño del array
 * @param comp Función de comparación
 */
void ordenar_con_funcion(int array[], int tamaño, int (*comp)(int, int)) {
    printf("Ordenando array con función de comparación personalizada:\n");
    printf("Array original: ");
    for (int i = 0; i < tamaño; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // Ordenamiento burbuja usando la función de comparación
    for (int i = 0; i < tamaño - 1; i++) {
        for (int j = 0; j < tamaño - i - 1; j++) {
            if (comp(array[j], array[j + 1]) > 0) {
                // Intercambiar
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    
    printf("Array ordenado: ");
    for (int i = 0; i < tamaño; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");
}

/**
 * Callback para procesamiento de números
 * @param numero Número a procesar
 * @param callback Función callback a aplicar
 * @return El resultado del callback
 */
int procesar_numero(int numero, int (*callback)(int, int)) {
    // Aplicar callback con el número y su doble
    return callback(numero, numero * 2);
}

/**
 * Función que demuestra el uso de callbacks
 * @param numeros Array de números
 * @param tamaño Tamaño del array
 * @param callback Función callback
 * @param nombre Nombre del callback
 */
void demo_callbacks(int numeros[], int tamaño, int (*callback)(int, int), 
                   const char *nombre) {
    printf("Demostración de callbacks - %s:\n", nombre);
    for (int i = 0; i < tamaño; i++) {
        int resultado = procesar_numero(numeros[i], callback);
        printf("  %s(%d, %d) = %d\n", nombre, numeros[i], numeros[i] * 2, resultado);
    }
    printf("\n");
}

/**
 * Función que busca el primer par que cumple una condición
 * @param array Array de pares
 * @param tamaño Número de pares
 * @param condicion Función que evalúa la condición
 * @param resultado Puntero donde guardar el resultado
 * @return 1 si encuentra un par, 0 si no
 */
int buscar_par_condicion(int array[][2], int tamaño, 
                        int (*condicion)(int, int), int *resultado) {
    for (int i = 0; i < tamaño; i++) {
        int valor = condicion(array[i][0], array[i][1]);
        if (valor != 0) {  // Condición cumplida
            *resultado = valor;
            return 1;
        }
    }
    return 0;
}

/**
 * Función que demuestra diferentes formas de declarar punteros a funciones
 */
void demo_declaraciones(void) {
    printf("🔹 DIFERENTES FORMAS DE DECLARAR PUNTEROS A FUNCIONES:\n\n");
    
    // Forma 1: Declaración directa
    int (*ptr1)(int, int) = sumar;
    printf("Forma 1 - Declaración directa:\n");
    printf("  int (*ptr1)(int, int) = sumar;\n");
    printf("  ptr1(3, 4) = %d\n\n", ptr1(3, 4));
    
    // Forma 2: Declaración y asignación separadas
    int (*ptr2)(int, int);
    ptr2 = restar;
    printf("Forma 2 - Declaración y asignación separadas:\n");
    printf("  int (*ptr2)(int, int);\n");
    printf("  ptr2 = restar;\n");
    printf("  ptr2(10, 3) = %d\n\n", ptr2(10, 3));
    
    // Forma 3: Con typedef
    typedef int (*OperacionBinaria)(int, int);
    OperacionBinaria ptr3 = multiplicar;
    printf("Forma 3 - Con typedef:\n");
    printf("  typedef int (*OperacionBinaria)(int, int);\n");
    printf("  OperacionBinaria ptr3 = multiplicar;\n");
    printf("  ptr3(5, 6) = %d\n\n", ptr3(5, 6));
    
    // Forma 4: Array de punteros a funciones
    int (*operaciones[])(int, int) = {sumar, restar, multiplicar, dividir};
    printf("Forma 4 - Array de punteros a funciones:\n");
    printf("  int (*operaciones[])(int, int) = {sumar, restar, multiplicar, dividir};\n");
    printf("  operaciones[0](8, 2) = %d\n", operaciones[0](8, 2));
    printf("  operaciones[1](8, 2) = %d\n", operaciones[1](8, 2));
    printf("  operaciones[2](8, 2) = %d\n", operaciones[2](8, 2));
    printf("  operaciones[3](8, 2) = %d\n\n", operaciones[3](8, 2));
}

int main(void) {
    printf("=== PUNTEROS A FUNCIONES ===\n\n");
    
    // Ejemplo básico del enunciado
    printf("🔹 EJEMPLO BÁSICO:\n");
    
    // Declaración de un puntero a función que recibe dos int y devuelve un int
    int (*ptrFuncion)(int, int);

    // Asignación de la dirección de la función 'sumar'
    ptrFuncion = &sumar;  // También válido: ptrFuncion = sumar;

    // Llamada a la función a través del puntero
    int resultado = ptrFuncion(10, 5);

    printf("Resultado de suma(10, 5): %d\n\n", resultado);
    
    // Demostración detallada
    printf("🔹 ANÁLISIS DETALLADO:\n");
    printf("Dirección de la función sumar: %p\n", (void *)sumar);
    printf("Valor del puntero ptrFuncion: %p\n", (void *)ptrFuncion);
    printf("¿Apuntan al mismo lugar? %s\n\n", 
           (sumar == ptrFuncion) ? "Sí" : "No");
    
    // Diferentes formas de llamar la función
    printf("Diferentes formas de llamar la función:\n");
    printf("  sumar(3, 7) = %d\n", sumar(3, 7));
    printf("  ptrFuncion(3, 7) = %d\n", ptrFuncion(3, 7));
    printf("  (*ptrFuncion)(3, 7) = %d\n", (*ptrFuncion)(3, 7));
    printf("\n");
    
    // Demostración de declaraciones
    demo_declaraciones();
    
    // Calculadora con punteros a funciones
    printf("🔹 CALCULADORA CON PUNTEROS A FUNCIONES:\n");
    int a = 20, b = 4;
    
    calculadora(a, b, sumar, "suma");
    calculadora(a, b, restar, "resta");
    calculadora(a, b, multiplicar, "multiplicación");
    calculadora(a, b, dividir, "división");
    calculadora(a, b, modulo, "módulo");
    calculadora(a, b, potencia, "potencia");
    calculadora(a, b, maximo, "máximo");
    calculadora(a, b, minimo, "mínimo");
    calculadora(a, b, mcd, "MCD");
    calculadora(a, b, mcm, "MCM");
    
    // Array de punteros a funciones
    printf("🔹 ARRAY DE PUNTEROS A FUNCIONES:\n");
    int (*funciones[])(int, int) = {sumar, restar, multiplicar, dividir};
    const char *nombres[] = {"suma", "resta", "multiplicación", "división"};
    
    ejecutar_funciones(15, 3, funciones, nombres, 4);
    
    // Calculadora dinámica
    printf("🔹 CALCULADORA DINÁMICA:\n");
    char operadores[] = {'+', '-', '*', '/', '%', '^', 'M', 'm', 'g', 'l'};
    int num_ops = sizeof(operadores) / sizeof(operadores[0]);
    
    for (int i = 0; i < num_ops; i++) {
        printf("Operador '%c': %d\n", operadores[i], 
               calculadora_dinamica(12, 8, operadores[i]));
    }
    printf("\n");
    
    // Aplicación a arrays
    printf("🔹 APLICACIÓN A ARRAYS:\n");
    int pares[][2] = {{10, 5}, {7, 3}, {20, 4}, {15, 6}};
    int num_pares = sizeof(pares) / sizeof(pares[0]);
    
    aplicar_operacion_array(pares, num_pares, sumar, "suma");
    aplicar_operacion_array(pares, num_pares, multiplicar, "multiplicación");
    aplicar_operacion_array(pares, num_pares, mcd, "MCD");
    
    // Ordenamiento con función de comparación
    printf("🔹 ORDENAMIENTO CON FUNCIÓN DE COMPARACIÓN:\n");
    int array[] = {64, 34, 25, 12, 22, 11, 90};
    int tamaño_array = sizeof(array) / sizeof(array[0]);
    
    ordenar_con_funcion(array, tamaño_array, comparar);
    
    // Demostración de callbacks
    printf("🔹 DEMOSTRACIÓN DE CALLBACKS:\n");
    int numeros[] = {5, 10, 15, 20};
    int tamaño_nums = sizeof(numeros) / sizeof(numeros[0]);
    
    demo_callbacks(numeros, tamaño_nums, sumar, "suma");
    demo_callbacks(numeros, tamaño_nums, multiplicar, "producto");
    demo_callbacks(numeros, tamaño_nums, maximo, "máximo");
    
    // Búsqueda con condiciones
    printf("🔹 BÚSQUEDA CON CONDICIONES:\n");
    int pares_busqueda[][2] = {{3, 7}, {5, 5}, {8, 4}, {9, 2}};
    int num_pares_busqueda = sizeof(pares_busqueda) / sizeof(pares_busqueda[0]);
    
    int resultado_busqueda;
    
    printf("Buscando primer par con suma > 10:\n");
    if (buscar_par_condicion(pares_busqueda, num_pares_busqueda, 
                            sumar, &resultado_busqueda)) {
        printf("  Encontrado: suma = %d\n", resultado_busqueda);
    } else {
        printf("  No encontrado\n");
    }
    
    printf("Buscando primer par con producto > 30:\n");
    if (buscar_par_condicion(pares_busqueda, num_pares_busqueda, 
                            multiplicar, &resultado_busqueda)) {
        printf("  Encontrado: producto = %d\n", resultado_busqueda);
    } else {
        printf("  No encontrado\n");
    }
    printf("\n");
    
    // Casos especiales
    printf("🔹 CASOS ESPECIALES:\n");
    
    // Puntero nulo
    int (*ptr_nulo)(int, int) = NULL;
    printf("Puntero nulo: %p\n", (void *)ptr_nulo);
    
    // Verificación antes de usar
    if (ptr_nulo != NULL) {
        printf("Llamando función a través de puntero nulo\n");
    } else {
        printf("Puntero nulo detectado - no se puede llamar\n");
    }
    
    // Asignación múltiple
    int (*ptr_multiple)(int, int) = sumar;
    printf("Usando el mismo puntero para diferentes funciones:\n");
    printf("  Como suma: %d\n", ptr_multiple(5, 3));
    
    ptr_multiple = restar;
    printf("  Como resta: %d\n", ptr_multiple(5, 3));
    
    ptr_multiple = multiplicar;
    printf("  Como multiplicación: %d\n", ptr_multiple(5, 3));
    printf("\n");
    
    // Funciones como parámetros de otras funciones
    printf("🔹 FUNCIONES COMO PARÁMETROS:\n");
    printf("Pasando diferentes funciones al mismo marco de trabajo:\n");
    
    typedef struct {
        const char *operacion;
        int (*func)(int, int);
    } OperacionNombrada;
    
    OperacionNombrada ops[] = {
        {"Suma", sumar},
        {"Resta", restar},
        {"Producto", multiplicar},
        {"División", dividir},
        {"Máximo", maximo},
        {"Mínimo", minimo}
    };
    
    int x = 24, y = 6;
    int num_operaciones = sizeof(ops) / sizeof(ops[0]);
    
    for (int i = 0; i < num_operaciones; i++) {
        printf("  %s de %d y %d: %d\n", ops[i].operacion, x, y, 
               ops[i].func(x, y));
    }
    
    printf("\n🔹 CONCEPTOS CLAVE DEMOSTRADOS:\n");
    printf("✓ Declaración de punteros a funciones\n");
    printf("✓ Asignación de direcciones de funciones\n");
    printf("✓ Llamada a funciones a través de punteros\n");
    printf("✓ Arrays de punteros a funciones\n");
    printf("✓ Funciones como parámetros (callbacks)\n");
    printf("✓ Selección dinámica de funciones\n");
    printf("✓ Typedef para simplificar declaraciones\n");
    printf("✓ Verificación de punteros nulos\n");
    printf("✓ Aplicaciones prácticas (calculadora, ordenamiento)\n");

    return 0;
}
