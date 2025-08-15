#include "constantes_preprocesador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Función principal del usuario (versión básica del enunciado)
int main_usuario_basico(void) {
    int numeros[TAMANIO];

    for (int i = 0; i < TAMANIO; i++) {
        numeros[i] = (i + 1) * 10;
    }

    printf("Contenido del array:\n");
    for (int i = 0; i < TAMANIO; i++) {
        printf("numeros[%d] = %d\n", i, numeros[i]);
    }

    return 0;
}

void demo_constantes_basicas(void) {
    printf("%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Constantes Básicas\n");
    printf("%s\n", SEPARADOR_SECCION);

    // Mostrar el valor de las constantes
    MOSTRAR_CONSTANTE(TAMANIO);
    MOSTRAR_CONSTANTE(TAMANIO_PEQUENO);
    MOSTRAR_CONSTANTE(TAMANIO_MEDIANO);
    MOSTRAR_CONSTANTE(TAMANIO_GRANDE);

    printf("\n");

    // Crear array usando la constante
    int numeros[TAMANIO];
    printf("Array declarado con TAMANIO = %d:\n", TAMANIO);

    // Llenar el array
    for (int i = 0; i < TAMANIO; i++) {
        numeros[i] = (i + 1) * 10;
    }

    // Mostrar contenido
    for (int i = 0; i < TAMANIO; i++) {
        printf("numeros[%d] = %d\n", i, numeros[i]);
    }

    printf("\nNota: El preprocesador sustituyó TAMANIO por %d antes de la compilación.\n", TAMANIO);
}

void demo_constantes_numericas(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Constantes Numéricas\n");
    printf("%s\n", SEPARADOR_SECCION);

    // Constantes matemáticas
    printf("Constantes matemáticas:\n");
    MOSTRAR_CONSTANTE_FLOAT(PI);
    MOSTRAR_CONSTANTE_FLOAT(E);
    MOSTRAR_CONSTANTE_FLOAT(GOLDEN_RATIO);
    MOSTRAR_CONSTANTE_FLOAT(SQRT_2);

    printf("\nCálculos usando constantes:\n");
    printf("Área del círculo unitario: %.6f\n", AREA_CIRCULO_UNITARIO);
    printf("Perímetro del círculo unitario: %.6f\n", PERIMETRO_CIRCULO_UNITARIO);

    // Límites
    printf("\nLímites de datos:\n");
    MOSTRAR_CONSTANTE(MAX_NOMBRE);
    MOSTRAR_CONSTANTE(MAX_LINEA);
    MOSTRAR_CONSTANTE(MIN_EDAD);
    MOSTRAR_CONSTANTE(MAX_EDAD);

    // Códigos de error
    printf("\nCódigos de error:\n");
    MOSTRAR_CONSTANTE(ERROR_NINGUNO);
    MOSTRAR_CONSTANTE(ERROR_MEMORIA);
    MOSTRAR_CONSTANTE(ERROR_ARCHIVO);
    MOSTRAR_CONSTANTE(ERROR_PARAMETRO);
}

void demo_constantes_cadenas(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Constantes de Cadenas\n");
    printf("%s\n", SEPARADOR_SECCION);

    // Mensajes estándar
    printf("Mensajes estándar:\n");
    MOSTRAR_CONSTANTE_STR(MSG_EXITO);
    MOSTRAR_CONSTANTE_STR(MSG_ERROR);
    MOSTRAR_CONSTANTE_STR(MSG_BIENVENIDA);
    MOSTRAR_CONSTANTE_STR(MSG_DESPEDIDA);

    printf("\nFormatos de salida:\n");
    MOSTRAR_CONSTANTE_STR(FORMATO_ENTERO);
    MOSTRAR_CONSTANTE_STR(FORMATO_FLOAT);
    MOSTRAR_CONSTANTE_STR(FORMATO_CADENA);

    printf("\nEjemplo de uso de formatos:\n");
    int numero = 42;
    float valor = 3.14159f;
    char* texto = "Hola mundo";

    printf("Usando FORMATO_ENTERO: ");
    printf(FORMATO_ENTERO "\n", numero);
    
    printf("Usando FORMATO_FLOAT: ");
    printf(FORMATO_FLOAT "\n", valor);
    
    printf("Usando FORMATO_CADENA: ");
    printf(FORMATO_CADENA "\n", texto);

    printf("\nSeparadores:\n");
    printf("SEPARADOR_LINEA:\n%s\n", SEPARADOR_LINEA);
    printf("SEPARADOR_SECCION:\n%s\n", SEPARADOR_SECCION);
}

void demo_constantes_calculadas(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Constantes Calculadas\n");
    printf("%s\n", SEPARADOR_SECCION);

    printf("Constantes derivadas de TAMANIO (%d):\n", TAMANIO);
    MOSTRAR_CONSTANTE(TAMANIO_DOBLE);
    MOSTRAR_CONSTANTE(TAMANIO_MITAD);

    printf("\nDemostración con arrays de diferentes tamaños:\n");
    
    // Array original
    int array_original[TAMANIO];
    printf("Array original (TAMANIO = %d): ", TAMANIO);
    for (int i = 0; i < TAMANIO; i++) {
        array_original[i] = i + 1;
        printf("%d ", array_original[i]);
    }
    printf("\n");

    // Array doble
    int array_doble[TAMANIO_DOBLE];
    printf("Array doble (TAMANIO_DOBLE = %d): ", TAMANIO_DOBLE);
    for (int i = 0; i < TAMANIO_DOBLE; i++) {
        array_doble[i] = i + 1;
        printf("%d ", array_doble[i]);
    }
    printf("\n");

    // Ejemplo de cálculos con constantes
    printf("\nCálculos con constantes matemáticas:\n");
    double radio = 5.0;
    printf("Radio: %.2f\n", radio);
    printf("Área: %.2f (usando PI = %.6f)\n", PI * radio * radio, PI);
    printf("Perímetro: %.2f\n", 2 * PI * radio);
}

void demo_macros_condicionales(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Macros Condicionales\n");
    printf("%s\n", SEPARADOR_SECCION);

    printf("Configuración de compilación:\n");
    MOSTRAR_CONSTANTE(DEBUG);
    MOSTRAR_CONSTANTE(VERSION_MAYOR);
    MOSTRAR_CONSTANTE(VERSION_MENOR);
    MOSTRAR_CONSTANTE(VERSION_PATCH);

    printf("\nCaracterísticas habilitadas:\n");
    MOSTRAR_CONSTANTE(HABILITAR_LOGGING);
    MOSTRAR_CONSTANTE(HABILITAR_ESTADISTICAS);
    MOSTRAR_CONSTANTE(HABILITAR_VALIDACION);

    printf("\nVersionado:\n");
    printf("Versión: %d.%d.%d\n", VERSION_MAYOR, VERSION_MENOR, VERSION_PATCH);

#if DEBUG
    printf("Modo DEBUG habilitado - información adicional disponible\n");
#else
    printf("Modo RELEASE - optimizaciones habilitadas\n");
#endif

    // Demostrar logging condicional
    printf("\nDemostración de logging:\n");
    LOG("Este es un mensaje de log normal");
    LOG_DEBUG("Este es un mensaje de debug (solo se muestra si DEBUG=1)");
    LOG_ERROR("Este es un mensaje de error");
}

void demo_pseudo_funciones(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Pseudo-funciones (Macros)\n");
    printf("%s\n", SEPARADOR_SECCION);

    // Demostrar macros de utilidad
    int a = -5, b = 3, c = 7;
    
    printf("Valores iniciales: a=%d, b=%d, c=%d\n", a, b, c);
    
    printf("ABS(a) = ABS(%d) = %d\n", a, ABS(a));
    printf("MAX(b, c) = MAX(%d, %d) = %d\n", b, c, MAX(b, c));
    printf("MIN(b, c) = MIN(%d, %d) = %d\n", b, c, MIN(b, c));

    printf("\nAntes del intercambio: b=%d, c=%d\n", b, c);
    SWAP(b, c);
    printf("Después del intercambio: b=%d, c=%d\n", b, c);

    // Demostrar macros de arrays
    int test_array[] = {10, 20, 30, 40, 50};
    printf("\nInformación del array:\n");
    printf("Tamaño del array: %zu elementos\n", ARRAY_SIZE(test_array));
    
    printf("Contenido del array: ");
    for (int* ptr = test_array; ptr < ARRAY_END(test_array); ptr++) {
        printf("%d ", *ptr);
    }
    printf("\n");
}

void demo_arrays_con_constantes(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Arrays con Constantes\n");
    printf("%s\n", SEPARADOR_SECCION);

    // Arrays de diferentes tamaños usando constantes
    int array_pequeno[TAMANIO_PEQUENO];
    int array_mediano[TAMANIO_MEDIANO];
    
    printf("Inicializando arrays de diferentes tamaños:\n");
    
    // Llenar array pequeño
    for (int i = 0; i < TAMANIO_PEQUENO; i++) {
        array_pequeno[i] = i * i; // Cuadrados
    }
    
    printf("Array pequeño (TAMANIO_PEQUENO = %d) - primeros 5 elementos:\n", TAMANIO_PEQUENO);
    for (int i = 0; i < 5 && i < TAMANIO_PEQUENO; i++) {
        printf("  [%d] = %d\n", i, array_pequeno[i]);
    }

    // Llenar array mediano (solo algunos elementos para demostración)
    for (int i = 0; i < 10 && i < TAMANIO_MEDIANO; i++) {
        array_mediano[i] = (i + 1) * 100;
    }
    
    printf("\nArray mediano (TAMANIO_MEDIANO = %d) - primeros 10 elementos:\n", TAMANIO_MEDIANO);
    for (int i = 0; i < 10 && i < TAMANIO_MEDIANO; i++) {
        printf("  [%d] = %d\n", i, array_mediano[i]);
    }

    // Demostrar buffers con constantes
    char buffer[MAX_LINEA];
    printf("\nBuffer de texto (MAX_LINEA = %d caracteres):\n", MAX_LINEA);
    snprintf(buffer, MAX_LINEA, "Este buffer puede contener hasta %d caracteres", MAX_LINEA - 1);
    printf("Contenido: %s\n", buffer);
    printf("Longitud actual: %zu caracteres\n", strlen(buffer));
}

void demo_mascaras_bits(void) {
    printf("\n%s\n", SEPARADOR_SECCION);
    printf("DEMOSTRACIÓN: Máscaras de Bits\n");
    printf("%s\n", SEPARADOR_SECCION);

    printf("Bits individuales:\n");
    for (int i = 0; i < 8; i++) {
        printf("BIT_%d = %d (binario: ", i, 1 << i);
        for (int j = 7; j >= 0; j--) {
            printf("%c", ((1 << i) & (1 << j)) ? '1' : '0');
        }
        printf(")\n");
    }

    printf("\nMáscaras combinadas:\n");
    printf("MASCARA_INFERIOR = %d (binario: ", MASCARA_INFERIOR);
    for (int i = 7; i >= 0; i--) {
        printf("%c", (MASCARA_INFERIOR & (1 << i)) ? '1' : '0');
    }
    printf(")\n");

    printf("MASCARA_SUPERIOR = %d (binario: ", MASCARA_SUPERIOR);
    for (int i = 7; i >= 0; i--) {
        printf("%c", (MASCARA_SUPERIOR & (1 << i)) ? '1' : '0');
    }
    printf(")\n");

    // Ejemplo de uso
    unsigned char valor = 0xB5; // 181 en decimal (binario 10110101)
    printf("\nEjemplo con valor = %d (binario: ", valor);
    for (int i = 7; i >= 0; i--) {
        printf("%c", (valor & (1 << i)) ? '1' : '0');
    }
    printf("):\n");

    printf("valor & MASCARA_INFERIOR = %d\n", valor & MASCARA_INFERIOR);
    printf("valor & MASCARA_SUPERIOR = %d\n", valor & MASCARA_SUPERIOR);
    printf("valor | MASCARA_INFERIOR = %d\n", valor | MASCARA_INFERIOR);
}

void mostrar_info_compilacion(void) {
    printf("\n%s\n", SEPARADOR_LINEA);
    printf("INFORMACIÓN DE COMPILACIÓN\n");
    printf("%s\n", SEPARADOR_LINEA);

    MOSTRAR_INFO_COMPILADOR();
    
    printf("\nConstantes de preprocesador definidas:\n");
    printf("__STDC__ = %d\n", __STDC__);
    printf("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);
    
#ifdef __GNUC__
    printf("Compilador GCC versión: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

#ifdef __clang__
    printf("Compilador Clang\n");
#endif

    printf("\nTamaños de tipos de datos:\n");
    printf("sizeof(int) = %zu bytes\n", sizeof(int));
    printf("sizeof(float) = %zu bytes\n", sizeof(float));
    printf("sizeof(double) = %zu bytes\n", sizeof(double));
    printf("sizeof(char) = %zu bytes\n", sizeof(char));
    printf("sizeof(void*) = %zu bytes\n", sizeof(void*));

    printf("\nArquitectura:\n");
#ifdef __x86_64__
    printf("Arquitectura: x86_64 (64 bits)\n");
#elif defined(__i386__)
    printf("Arquitectura: i386 (32 bits)\n");
#elif defined(__aarch64__)
    printf("Arquitectura: ARM64 (64 bits)\n");
#else
    printf("Arquitectura: Desconocida\n");
#endif
}

void ejecutar_todas_demos(void) {
    printf("%s\n", MSG_BIENVENIDA);
    printf("%s\n", SEPARADOR_LINEA);
    printf("DEMOSTRACIÓN COMPLETA: Constantes de Preprocesador\n");
    printf("%s\n", SEPARADOR_LINEA);

    demo_constantes_basicas();
    demo_constantes_numericas();
    demo_constantes_cadenas();
    demo_constantes_calculadas();
    
#if HABILITAR_DEMOS_AVANZADAS
    demo_macros_condicionales();
    demo_pseudo_funciones();
    demo_arrays_con_constantes();
    demo_mascaras_bits();
#endif

    mostrar_info_compilacion();

    printf("\n%s\n", SEPARADOR_LINEA);
    printf("%s\n", MSG_DESPEDIDA);
    printf("%s\n", SEPARADOR_LINEA);
}

// Función principal
int main(void) {
    // Ejecutar demostración completa
    ejecutar_todas_demos();
    
    return 0;
}
