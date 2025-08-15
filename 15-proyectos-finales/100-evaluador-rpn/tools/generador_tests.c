#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Generador de casos de prueba para el evaluador RPN

typedef enum {
    TIPO_BASICO,
    TIPO_AVANZADO,
    TIPO_ERROR,
    TIPO_LIMITE,
    TIPO_RENDIMIENTO
} tipo_test_t;

// Estructuras para diferentes tipos de tests
typedef struct {
    char expresion[200];
    double resultado_esperado;
    int deberia_fallar;
    char descripcion[100];
} caso_test_t;

// Generador de números aleatorios
double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// Generar test básico
caso_test_t generar_test_basico(void) {
    caso_test_t test;
    test.deberia_fallar = 0;
    
    char operadores[] = "+-*/";
    char op = operadores[rand() % 4];
    
    double a = random_double(1.0, 100.0);
    double b = random_double(1.0, 100.0);
    
    // Evitar división por cero en tests básicos
    if (op == '/' && b == 0.0) {
        b = 1.0;
    }
    
    snprintf(test.expresion, sizeof(test.expresion), "%.2f %.2f %c", a, b, op);
    
    switch (op) {
        case '+':
            test.resultado_esperado = a + b;
            strcpy(test.descripcion, "Suma básica");
            break;
        case '-':
            test.resultado_esperado = a - b;
            strcpy(test.descripcion, "Resta básica");
            break;
        case '*':
            test.resultado_esperado = a * b;
            strcpy(test.descripcion, "Multiplicación básica");
            break;
        case '/':
            test.resultado_esperado = a / b;
            strcpy(test.descripcion, "División básica");
            break;
    }
    
    return test;
}

// Generar test avanzado
caso_test_t generar_test_avanzado(void) {
    caso_test_t test;
    test.deberia_fallar = 0;
    
    int tipo = rand() % 5;
    
    switch (tipo) {
        case 0: // Potencia
            {
                double base = random_double(1.0, 10.0);
                double exp = random_double(1.0, 4.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f %.2f ^", base, exp);
                test.resultado_esperado = pow(base, exp);
                strcpy(test.descripcion, "Potencia");
            }
            break;
        case 1: // Raíz cuadrada
            {
                double num = random_double(1.0, 100.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f sqrt", num);
                test.resultado_esperado = sqrt(num);
                strcpy(test.descripcion, "Raíz cuadrada");
            }
            break;
        case 2: // Funciones trigonométricas
            {
                double angulo = random_double(0.0, 6.28); // 0 a 2π
                char funciones[][5] = {"sin", "cos"};
                char* func = funciones[rand() % 2];
                snprintf(test.expresion, sizeof(test.expresion), "%.6f %s", angulo, func);
                
                if (strcmp(func, "sin") == 0) {
                    test.resultado_esperado = sin(angulo);
                } else {
                    test.resultado_esperado = cos(angulo);
                }
                snprintf(test.descripcion, sizeof(test.descripcion), "Función %s", func);
            }
            break;
        case 3: // Constantes
            {
                char constantes[][5] = {"pi", "e"};
                char* const_name = constantes[rand() % 2];
                snprintf(test.expresion, sizeof(test.expresion), "%s", const_name);
                
                if (strcmp(const_name, "pi") == 0) {
                    test.resultado_esperado = M_PI;
                } else {
                    test.resultado_esperado = M_E;
                }
                snprintf(test.descripcion, sizeof(test.descripcion), "Constante %s", const_name);
            }
            break;
        case 4: // Expresión compleja
            {
                double a = random_double(1.0, 10.0);
                double b = random_double(1.0, 10.0);
                double c = random_double(1.0, 10.0);
                
                snprintf(test.expresion, sizeof(test.expresion), "%.2f %.2f + %.2f *", a, b, c);
                test.resultado_esperado = (a + b) * c;
                strcpy(test.descripcion, "Expresión compleja (a+b)*c");
            }
            break;
    }
    
    return test;
}

// Generar test de error
caso_test_t generar_test_error(void) {
    caso_test_t test;
    test.deberia_fallar = 1;
    test.resultado_esperado = 0.0; // No importa
    
    int tipo_error = rand() % 5;
    
    switch (tipo_error) {
        case 0: // División por cero
            {
                double a = random_double(1.0, 100.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f 0 /", a);
                strcpy(test.descripcion, "División por cero");
            }
            break;
        case 1: // Operandos insuficientes
            {
                snprintf(test.expresion, sizeof(test.expresion), "5 +");
                strcpy(test.descripcion, "Operandos insuficientes");
            }
            break;
        case 2: // Token inválido
            {
                snprintf(test.expresion, sizeof(test.expresion), "5 xyz +");
                strcpy(test.descripcion, "Token inválido");
            }
            break;
        case 3: // Raíz cuadrada de negativo
            {
                double num = -random_double(1.0, 100.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f sqrt", num);
                strcpy(test.descripcion, "Raíz de número negativo");
            }
            break;
        case 4: // Logaritmo de cero o negativo
            {
                double num = rand() % 2 ? 0.0 : -random_double(1.0, 100.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f ln", num);
                strcpy(test.descripcion, "Logaritmo de cero/negativo");
            }
            break;
    }
    
    return test;
}

// Generar test de caso límite
caso_test_t generar_test_limite(void) {
    caso_test_t test;
    test.deberia_fallar = 0;
    
    int tipo = rand() % 4;
    
    switch (tipo) {
        case 0: // Números muy grandes
            {
                double a = random_double(1e6, 1e8);
                double b = random_double(1e6, 1e8);
                snprintf(test.expresion, sizeof(test.expresion), "%.0f %.0f +", a, b);
                test.resultado_esperado = a + b;
                strcpy(test.descripcion, "Números muy grandes");
            }
            break;
        case 1: // Números muy pequeños
            {
                double a = random_double(1e-6, 1e-4);
                double b = random_double(1e-6, 1e-4);
                snprintf(test.expresion, sizeof(test.expresion), "%.8f %.8f +", a, b);
                test.resultado_esperado = a + b;
                strcpy(test.descripcion, "Números muy pequeños");
            }
            break;
        case 2: // Operaciones con cero
            {
                double a = random_double(1.0, 100.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f 0 +", a);
                test.resultado_esperado = a;
                strcpy(test.descripcion, "Operación con cero");
            }
            break;
        case 3: // Potencias especiales
            {
                double base = random_double(1.0, 10.0);
                snprintf(test.expresion, sizeof(test.expresion), "%.2f 0 ^", base);
                test.resultado_esperado = 1.0;
                strcpy(test.descripcion, "Potencia elevada a cero");
            }
            break;
    }
    
    return test;
}

// Generar test de rendimiento
caso_test_t generar_test_rendimiento(void) {
    caso_test_t test;
    test.deberia_fallar = 0;
    
    // Generar expresión larga
    strcpy(test.expresion, "1");
    double suma_esperada = 1.0;
    
    int num_operaciones = rand() % 50 + 10; // 10-59 operaciones
    
    for (int i = 2; i <= num_operaciones; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), " %d +", i);
        strcat(test.expresion, temp);
        suma_esperada += i;
    }
    
    test.resultado_esperado = suma_esperada;
    snprintf(test.descripcion, sizeof(test.descripcion), 
             "Expresión larga (%d operaciones)", num_operaciones - 1);
    
    return test;
}

// Función principal
int main(int argc, char* argv[]) {
    int num_tests = 100;
    
    if (argc > 1) {
        num_tests = atoi(argv[1]);
        if (num_tests <= 0) {
            fprintf(stderr, "Número de tests debe ser positivo\n");
            return 1;
        }
    }
    
    srand((unsigned int)time(NULL));
    
    printf("# Casos de prueba generados automáticamente para Evaluador RPN\n");
    printf("# Generados el: %s", ctime(&(time_t){time(NULL)}));
    printf("# Número total de tests: %d\n\n", num_tests);
    
    // Distribución de tipos de tests
    int num_basicos = num_tests * 0.4;      // 40%
    int num_avanzados = num_tests * 0.3;    // 30%
    int num_errores = num_tests * 0.15;     // 15%
    int num_limite = num_tests * 0.1;       // 10%
    int num_rendimiento = num_tests * 0.05; // 5%
    
    // Ajustar para que sume exactamente num_tests
    int total_asignado = num_basicos + num_avanzados + num_errores + num_limite + num_rendimiento;
    num_basicos += (num_tests - total_asignado);
    
    printf("# Distribución de tests:\n");
    printf("# - Básicos: %d\n", num_basicos);
    printf("# - Avanzados: %d\n", num_avanzados);
    printf("# - Errores: %d\n", num_errores);
    printf("# - Casos límite: %d\n", num_limite);
    printf("# - Rendimiento: %d\n\n", num_rendimiento);
    
    int test_num = 1;
    
    // Generar tests básicos
    printf("# === TESTS BÁSICOS ===\n");
    for (int i = 0; i < num_basicos; i++) {
        caso_test_t test = generar_test_basico();
        printf("# Test %d: %s\n", test_num++, test.descripcion);
        printf("# Expresión: %s\n", test.expresion);
        printf("# Resultado esperado: %.6f\n", test.resultado_esperado);
        printf("%s\n\n", test.expresion);
    }
    
    // Generar tests avanzados
    printf("# === TESTS AVANZADOS ===\n");
    for (int i = 0; i < num_avanzados; i++) {
        caso_test_t test = generar_test_avanzado();
        printf("# Test %d: %s\n", test_num++, test.descripcion);
        printf("# Expresión: %s\n", test.expresion);
        printf("# Resultado esperado: %.6f\n", test.resultado_esperado);
        printf("%s\n\n", test.expresion);
    }
    
    // Generar tests de casos límite
    printf("# === TESTS DE CASOS LÍMITE ===\n");
    for (int i = 0; i < num_limite; i++) {
        caso_test_t test = generar_test_limite();
        printf("# Test %d: %s\n", test_num++, test.descripcion);
        printf("# Expresión: %s\n", test.expresion);
        printf("# Resultado esperado: %.6f\n", test.resultado_esperado);
        printf("%s\n\n", test.expresion);
    }
    
    // Generar tests de rendimiento
    printf("# === TESTS DE RENDIMIENTO ===\n");
    for (int i = 0; i < num_rendimiento; i++) {
        caso_test_t test = generar_test_rendimiento();
        printf("# Test %d: %s\n", test_num++, test.descripcion);
        printf("# Expresión: %s\n", test.expresion);
        printf("# Resultado esperado: %.0f\n", test.resultado_esperado);
        printf("%s\n\n", test.expresion);
    }
    
    // Generar tests de error (al final para no interferir con la salida)
    printf("# === TESTS DE ERROR (DEBERÍAN FALLAR) ===\n");
    for (int i = 0; i < num_errores; i++) {
        caso_test_t test = generar_test_error();
        printf("# Test %d: %s (DEBE FALLAR)\n", test_num++, test.descripcion);
        printf("# Expresión: %s\n", test.expresion);
        printf("# Este test debe generar un error\n");
        printf("# %s\n\n", test.expresion); // Comentado para no ejecutar
    }
    
    printf("# === RESUMEN ===\n");
    printf("# Total de tests generados: %d\n", test_num - 1);
    printf("# Tests válidos: %d\n", num_basicos + num_avanzados + num_limite + num_rendimiento);
    printf("# Tests de error: %d\n", num_errores);
    printf("#\n");
    printf("# Para ejecutar estos tests:\n");
    printf("# ./generador_tests 50 > tests_generados.txt\n");
    printf("# ./evaluador_rpn tests_generados.txt\n");
    
    return 0;
}
