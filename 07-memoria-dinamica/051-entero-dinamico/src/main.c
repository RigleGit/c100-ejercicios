#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * Función básica del enunciado: gestión básica de memoria dinámica
 * Implementación directa del ejercicio original
 */
void gestionBasica(void) {
    printf("=== IMPLEMENTACIÓN BÁSICA DEL ENUNCIADO ===\n");
    
    int *ptr = (int *)malloc(sizeof(int));

    if (ptr == NULL) {
        printf("Error al reservar memoria.\n");
        return;
    }

    *ptr = 2025;
    printf("Valor almacenado: %d\n", *ptr);

    free(ptr);
    ptr = NULL;
    
    printf("Memoria liberada y puntero establecido a NULL\n");
    printf("============================================\n\n");
}

/**
 * Función avanzada para crear un entero dinámico con verificaciones
 * @param valor Valor inicial para el entero
 * @return Puntero al entero creado o NULL si hay error
 */
int* crearEntero(int valor) {
    printf("=== Creando entero dinámico ===\n");
    printf("Solicitando memoria para un entero...\n");
    
    int *ptr = (int *)malloc(sizeof(int));
    
    if (ptr == NULL) {
        printf("Error: No se pudo reservar memoria para el entero\n");
        printf("Posible causa: Memoria insuficiente\n");
        printf("errno: %d (%s)\n", errno, strerror(errno));
        return NULL;
    }
    
    printf("Memoria reservada exitosamente\n");
    printf("Dirección asignada: %p\n", (void*)ptr);
    printf("Tamaño reservado: %zu bytes\n", sizeof(int));
    
    *ptr = valor;
    printf("Valor %d asignado correctamente\n", valor);
    printf("==============================\n\n");
    
    return ptr;
}

/**
 * Función para mostrar información detallada de un entero dinámico
 * @param ptr Puntero al entero dinámico
 * @param nombre Nombre descriptivo del entero
 */
void mostrarEntero(int *ptr, const char *nombre) {
    if (ptr == NULL) {
        printf("=== Información de %s ===\n", nombre);
        printf("Estado: PUNTERO NULO\n");
        printf("No se puede acceder al valor\n");
        printf("===============================\n\n");
        return;
    }
    
    printf("=== Información de %s ===\n", nombre);
    printf("Dirección: %p\n", (void*)ptr);
    printf("Valor: %d\n", *ptr);
    printf("Tamaño: %zu bytes\n", sizeof(int));
    printf("Valor en hexadecimal: 0x%08X\n", *ptr);
    
    // Mostrar representación en bytes
    unsigned char *bytes = (unsigned char*)ptr;
    printf("Representación en bytes: ");
    for (size_t i = 0; i < sizeof(int); i++) {
        printf("0x%02X ", bytes[i]);
    }
    printf("\n");
    printf("===============================\n\n");
}

/**
 * Función para modificar un entero dinámico de forma segura
 * @param ptr Puntero al entero dinámico
 * @param nuevo_valor Nuevo valor a asignar
 * @return 1 si exitoso, 0 si error
 */
int modificarEntero(int *ptr, int nuevo_valor) {
    if (ptr == NULL) {
        printf("=== Error en modificación ===\n");
        printf("No se puede modificar: puntero nulo\n");
        printf("=============================\n\n");
        return 0;
    }
    
    printf("=== Modificando entero ===\n");
    printf("Valor anterior: %d\n", *ptr);
    printf("Nuevo valor: %d\n", nuevo_valor);
    
    *ptr = nuevo_valor;
    
    printf("Modificación exitosa\n");
    printf("Valor actual: %d\n", *ptr);
    printf("=========================\n\n");
    
    return 1;
}

/**
 * Función para liberar memoria de forma segura
 * @param ptr Puntero al puntero del entero (para poder establecerlo a NULL)
 */
void liberarEntero(int **ptr) {
    if (ptr == NULL || *ptr == NULL) {
        printf("=== Liberación de memoria ===\n");
        printf("No hay memoria que liberar (puntero ya es NULL)\n");
        printf("=============================\n\n");
        return;
    }
    
    printf("=== Liberando memoria ===\n");
    printf("Dirección a liberar: %p\n", (void*)*ptr);
    printf("Valor actual: %d\n", **ptr);
    
    free(*ptr);
    *ptr = NULL;
    
    printf("Memoria liberada exitosamente\n");
    printf("Puntero establecido a NULL\n");
    printf("=========================\n\n");
}

/**
 * Función para demostrar errores comunes con memoria dinámica
 */
void demostrarErroresComunes(void) {
    printf("=== DEMOSTRACIÓN DE ERRORES COMUNES ===\n");
    
    // 1. Uso después de liberar (Use After Free)
    printf("1. Ejemplo de 'Use After Free' (ERROR):\n");
    int *ptr1 = crearEntero(100);
    printf("Liberando memoria...\n");
    free(ptr1);
    
    printf("¡PELIGRO! Intentar usar ptr1 después de free() causaría:\n");
    printf("- Comportamiento indefinido\n");
    printf("- Posible corrupción de memoria\n");
    printf("- Crashes del programa\n\n");
    
    // Establecer a NULL para evitar uso accidental
    ptr1 = NULL;
    printf("Solución: Establecer ptr1 = NULL después de free()\n\n");
    
    // 2. Double Free
    printf("2. Ejemplo de 'Double Free' (ERROR):\n");
    int *ptr2 = crearEntero(200);
    printf("Primera liberación...\n");
    free(ptr2);
    
    printf("¡PELIGRO! Llamar free(ptr2) de nuevo causaría:\n");
    printf("- Corrupción del heap\n");
    printf("- Crash del programa\n");
    printf("- Comportamiento impredecible\n\n");
    
    ptr2 = NULL;
    printf("Solución: Establecer ptr2 = NULL después del primer free()\n\n");
    
    // 3. Memory Leak
    printf("3. Ejemplo de 'Memory Leak' (ERROR):\n");
    printf("int *ptr3 = malloc(sizeof(int)); // Reservar memoria\n");
    printf("ptr3 = NULL; // ¡LEAK! Perdemos la referencia sin liberar\n");
    printf("¡PELIGRO! Esto causa:\n");
    printf("- Pérdida de memoria\n");
    printf("- El programa consume más memoria de la necesaria\n");
    printf("- En bucles, puede agotar toda la memoria disponible\n\n");
    
    printf("Solución: Siempre free() antes de perder la referencia\n");
    printf("======================================\n\n");
}

/**
 * Función para demostrar buenas prácticas
 */
void demostrarBuenasPracticas(void) {
    printf("=== BUENAS PRÁCTICAS DE MEMORIA DINÁMICA ===\n");
    
    // 1. Verificación de malloc
    printf("1. Siempre verificar el retorno de malloc:\n");
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr == NULL) {
        printf("Error: malloc falló\n");
        return;
    }
    printf("✓ Verificación exitosa\n\n");
    
    // 2. Inicialización
    printf("2. Inicializar la memoria asignada:\n");
    *ptr = 0; // O el valor que necesites
    printf("✓ Memoria inicializada con valor: %d\n\n", *ptr);
    
    // 3. Uso normal
    printf("3. Usar la memoria normalmente:\n");
    *ptr = 42;
    printf("✓ Valor asignado: %d\n\n", *ptr);
    
    // 4. Liberación y anulación
    printf("4. Liberar y anular el puntero:\n");
    free(ptr);
    ptr = NULL;
    printf("✓ Memoria liberada y puntero anulado\n\n");
    
    // 5. Verificación de puntero nulo
    printf("5. Verificar puntero antes de usar:\n");
    if (ptr != NULL) {
        printf("Usando ptr...\n");
    } else {
        printf("✓ ptr es NULL, no se puede usar de forma segura\n");
    }
    
    printf("==========================================\n\n");
}

/**
 * Función para demostrar múltiples enteros dinámicos
 */
void demostrarMultiplesEnteros(void) {
    printf("=== MÚLTIPLES ENTEROS DINÁMICOS ===\n");
    
    const int NUM_ENTEROS = 5;
    int *enteros[NUM_ENTEROS];
    
    // Crear múltiples enteros
    printf("Creando %d enteros dinámicos:\n", NUM_ENTEROS);
    for (int i = 0; i < NUM_ENTEROS; i++) {
        enteros[i] = crearEntero(i * 10);
        if (enteros[i] == NULL) {
            printf("Error creando entero %d\n", i);
            // Liberar los que ya se crearon
            for (int j = 0; j < i; j++) {
                liberarEntero(&enteros[j]);
            }
            return;
        }
        printf("Entero %d creado en %p con valor %d\n", 
               i, (void*)enteros[i], *enteros[i]);
    }
    printf("\n");
    
    // Mostrar todos los enteros
    printf("Valores almacenados:\n");
    for (int i = 0; i < NUM_ENTEROS; i++) {
        printf("enteros[%d] = %d (dirección: %p)\n", 
               i, *enteros[i], (void*)enteros[i]);
    }
    printf("\n");
    
    // Modificar algunos valores
    printf("Modificando algunos valores:\n");
    for (int i = 0; i < NUM_ENTEROS; i += 2) {
        int nuevo_valor = *enteros[i] + 1000;
        printf("Modificando enteros[%d] de %d a %d\n", 
               i, *enteros[i], nuevo_valor);
        modificarEntero(enteros[i], nuevo_valor);
    }
    printf("\n");
    
    // Liberar todos los enteros
    printf("Liberando todos los enteros:\n");
    for (int i = 0; i < NUM_ENTEROS; i++) {
        printf("Liberando enteros[%d]...\n", i);
        liberarEntero(&enteros[i]);
    }
    
    printf("===================================\n\n");
}

/**
 * Función para analizar el uso de memoria
 */
void analizarUsoMemoria(void) {
    printf("=== ANÁLISIS DE USO DE MEMORIA ===\n");
    
    printf("Información sobre malloc:\n");
    printf("- malloc(n) reserva n bytes en el heap\n");
    printf("- Retorna NULL si no hay memoria suficiente\n");
    printf("- La memoria no está inicializada\n");
    printf("- Debe liberarse con free()\n\n");
    
    printf("Tamaños de tipos básicos en este sistema:\n");
    printf("sizeof(int): %zu bytes\n", sizeof(int));
    printf("sizeof(float): %zu bytes\n", sizeof(float));
    printf("sizeof(double): %zu bytes\n", sizeof(double));
    printf("sizeof(char): %zu bytes\n", sizeof(char));
    printf("sizeof(void*): %zu bytes\n", sizeof(void*));
    printf("\n");
    
    printf("Ejemplo de cálculo de memoria:\n");
    int num_enteros = 1000;
    size_t memoria_total = num_enteros * sizeof(int);
    printf("Para %d enteros: %d × %zu = %zu bytes\n", 
           num_enteros, num_enteros, sizeof(int), memoria_total);
    printf("Equivalente a: %.2f KB\n", memoria_total / 1024.0);
    
    printf("==================================\n\n");
}

/**
 * Función principal que demuestra todos los conceptos
 */
int main(void) {
    printf("=== EJERCICIO 051: ENTERO DINÁMICO ===\n\n");
    
    // 0. Implementación básica del enunciado
    gestionBasica();
    
    // 1. Demostración avanzada
    printf("1. GESTIÓN AVANZADA DE ENTERO DINÁMICO\n");
    printf("--------------------------------------\n");
    
    int *mi_entero = crearEntero(42);
    if (mi_entero == NULL) {
        printf("Error: No se pudo crear el entero dinámico\n");
        return 1;
    }
    
    mostrarEntero(mi_entero, "mi_entero");
    
    // 2. Modificación
    printf("2. MODIFICACIÓN DE VALORES\n");
    printf("--------------------------\n");
    modificarEntero(mi_entero, 1234);
    mostrarEntero(mi_entero, "mi_entero");
    
    modificarEntero(mi_entero, -999);
    mostrarEntero(mi_entero, "mi_entero");
    
    // 3. Liberación segura
    printf("3. LIBERACIÓN SEGURA\n");
    printf("--------------------\n");
    liberarEntero(&mi_entero);
    mostrarEntero(mi_entero, "mi_entero");
    
    // 4. Errores comunes
    printf("4. ERRORES COMUNES A EVITAR\n");
    printf("---------------------------\n");
    demostrarErroresComunes();
    
    // 5. Buenas prácticas
    printf("5. BUENAS PRÁCTICAS\n");
    printf("-------------------\n");
    demostrarBuenasPracticas();
    
    // 6. Múltiples enteros
    printf("6. MÚLTIPLES ENTEROS DINÁMICOS\n");
    printf("------------------------------\n");
    demostrarMultiplesEnteros();
    
    // 7. Análisis de memoria
    printf("7. ANÁLISIS DE USO DE MEMORIA\n");
    printf("-----------------------------\n");
    analizarUsoMemoria();
    
    printf("=== FIN DEL EJERCICIO ===\n");
    
    return 0;
}
