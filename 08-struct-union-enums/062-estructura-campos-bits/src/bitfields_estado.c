#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/bitfields_estado.h"

#ifndef EXCLUDE_MAIN
int main(void) {
    printf("=== Ejercicio 062: Estructura con Bit Fields ===\n\n");
    
    // Crear y manipular una estructura básica
    printf("1. Estructura básica con bit fields:\n");
    Estado e = {0}; // Inicializar todos los bits a 0
    
    // Asignar valores individualmente
    e.activo = 1;
    e.conectado = 0;
    e.error = 1;
    e.modo_debug = 1;
    e.reservado = 5; // Usar algunos bits reservados
    
    printf("Estado inicial:\n");
    mostrar_estado(&e);
    
    // Demostrar tamaño de la estructura
    printf("\n2. Análisis de memoria:\n");
    printf("Tamaño de la estructura Estado: %zu bytes\n", sizeof(Estado));
    printf("Tamaño de unsigned int: %zu bytes\n", sizeof(unsigned int));
    printf("Bits totales en la estructura: %zu bits\n", sizeof(Estado) * 8);
    
    // Comparar con estructura sin bit fields
    printf("Tamaño de EstadoSinBitFields: %zu bytes\n", sizeof(EstadoSinBitFields));
    printf("Reducción de memoria: %zu bytes (%.1f%%)\n", 
           sizeof(EstadoSinBitFields) - sizeof(Estado),
           ((float)(sizeof(EstadoSinBitFields) - sizeof(Estado)) / sizeof(EstadoSinBitFields)) * 100);
    
    // Demostrar manipulación de estados
    printf("\n3. Manipulación de estados:\n");
    configurar_estado_inicial(&e);
    printf("Después de configuración inicial:\n");
    mostrar_estado(&e);
    
    activar_modo_debug(&e);
    printf("\nDespués de activar modo debug:\n");
    mostrar_estado(&e);
    
    reportar_error(&e);
    printf("\nDespués de reportar error:\n");
    mostrar_estado(&e);
    
    // Demostrar configuración mediante máscaras
    printf("\n4. Configuración mediante máscara:\n");
    unsigned int mascara = ESTADO_ACTIVO | ESTADO_CONECTADO;
    configurar_con_mascara(&e, mascara);
    printf("Configurado con máscara (ACTIVO | CONECTADO):\n");
    mostrar_estado(&e);
    
    // Verificar estados específicos
    printf("\n5. Verificación de estados:\n");
    printf("¿Está activo? %s\n", esta_activo(&e) ? "Sí" : "No");
    printf("¿Hay error? %s\n", hay_error(&e) ? "Sí" : "No");
    printf("¿Está en modo debug? %s\n", esta_en_debug(&e) ? "Sí" : "No");
    
    // Demostrar copia de estado
    printf("\n6. Copia de estado:\n");
    Estado e2;
    copiar_estado(&e, &e2);
    printf("Estado original:\n");
    mostrar_estado(&e);
    printf("Estado copiado:\n");
    mostrar_estado(&e2);
    
    // Demostrar límites de bit fields
    printf("\n7. Límites de bit fields:\n");
    demostrar_limites();
    
    // Demostrar múltiples configuraciones
    printf("\n8. Diferentes configuraciones:\n");
    Estado estados[] = {
        {.activo = 1, .conectado = 1, .error = 0, .modo_debug = 0, .reservado = 0},
        {.activo = 1, .conectado = 0, .error = 1, .modo_debug = 1, .reservado = 3},
        {.activo = 0, .conectado = 0, .error = 0, .modo_debug = 0, .reservado = 15}
    };
    
    const char* descripciones[] = {
        "Sistema funcionando normalmente",
        "Sistema activo con error en modo debug",
        "Sistema completamente inactivo"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n%s:\n", descripciones[i]);
        mostrar_estado(&estados[i]);
        printf("Representación como entero: 0x%02X\n", *(unsigned int*)&estados[i]);
    }
    
    printf("\n=== Fin del ejercicio ===\n");
    return 0;
}
#endif // EXCLUDE_MAIN

void mostrar_estado(const Estado* e) {
    printf("  Activo: %u\n", e->activo);
    printf("  Conectado: %u\n", e->conectado);
    printf("  Error: %u\n", e->error);
    printf("  Modo debug: %u\n", e->modo_debug);
    printf("  Reservado: %u\n", e->reservado);
    printf("  Estado como string: %s\n", estado_a_string(e));
}

void configurar_estado_inicial(Estado* e) {
    e->activo = 1;
    e->conectado = 1;
    e->error = 0;
    e->modo_debug = 0;
    e->reservado = 0;
}

void activar_modo_debug(Estado* e) {
    e->modo_debug = 1;
}

void desactivar_modo_debug(Estado* e) {
    e->modo_debug = 0;
}

void reportar_error(Estado* e) {
    e->error = 1;
    e->conectado = 0; // Desconectar en caso de error
}

void limpiar_error(Estado* e) {
    e->error = 0;
}

void conectar(Estado* e) {
    if (!e->error) { // Solo conectar si no hay error
        e->conectado = 1;
    }
}

void desconectar(Estado* e) {
    e->conectado = 0;
}

bool esta_activo(const Estado* e) {
    return e->activo == 1;
}

bool esta_conectado(const Estado* e) {
    return e->conectado == 1;
}

bool hay_error(const Estado* e) {
    return e->error == 1;
}

bool esta_en_debug(const Estado* e) {
    return e->modo_debug == 1;
}

bool esta_operativo(const Estado* e) {
    return e->activo && e->conectado && !e->error;
}

void configurar_con_mascara(Estado* e, unsigned int mascara) {
    // Limpiar todos los campos primero
    e->activo = 0;
    e->conectado = 0;
    e->error = 0;
    e->modo_debug = 0;
    
    // Aplicar máscara
    if (mascara & ESTADO_ACTIVO) e->activo = 1;
    if (mascara & ESTADO_CONECTADO) e->conectado = 1;
    if (mascara & ESTADO_ERROR) e->error = 1;
    if (mascara & ESTADO_DEBUG) e->modo_debug = 1;
}

unsigned int estado_a_mascara(const Estado* e) {
    unsigned int mascara = 0;
    if (e->activo) mascara |= ESTADO_ACTIVO;
    if (e->conectado) mascara |= ESTADO_CONECTADO;
    if (e->error) mascara |= ESTADO_ERROR;
    if (e->modo_debug) mascara |= ESTADO_DEBUG;
    return mascara;
}

const char* estado_a_string(const Estado* e) {
    static char buffer[100];
    char temp[20];
    
    strcpy(buffer, "[");
    
    if (e->activo) {
        strcat(buffer, "ACTIVO");
    } else {
        strcat(buffer, "INACTIVO");
    }
    
    if (e->conectado) {
        strcat(buffer, "|CONECTADO");
    } else {
        strcat(buffer, "|DESCONECTADO");
    }
    
    if (e->error) {
        strcat(buffer, "|ERROR");
    }
    
    if (e->modo_debug) {
        strcat(buffer, "|DEBUG");
    }
    
    if (e->reservado > 0) {
        sprintf(temp, "|RES:%u", e->reservado);
        strcat(buffer, temp);
    }
    
    strcat(buffer, "]");
    return buffer;
}

void copiar_estado(const Estado* origen, Estado* destino) {
    destino->activo = origen->activo;
    destino->conectado = origen->conectado;
    destino->error = origen->error;
    destino->modo_debug = origen->modo_debug;
    destino->reservado = origen->reservado;
}

bool comparar_estados(const Estado* e1, const Estado* e2) {
    return (e1->activo == e2->activo) &&
           (e1->conectado == e2->conectado) &&
           (e1->error == e2->error) &&
           (e1->modo_debug == e2->modo_debug) &&
           (e1->reservado == e2->reservado);
}

void demostrar_limites(void) {
    printf("Límites de los campos de bits:\n");
    
    Estado e = {0};
    
    // Campo de 1 bit: valores válidos 0-1
    printf("Campo 'activo' (1 bit): ");
    e.activo = 0; printf("0 ");
    e.activo = 1; printf("1 ");
    // e.activo = 2; // Esto truncaría a 0
    printf("\n");
    
    // Campo de 4 bits: valores válidos 0-15
    printf("Campo 'reservado' (4 bits): ");
    for (int i = 0; i <= 15; i++) {
        e.reservado = i;
        printf("%u ", e.reservado);
    }
    printf("\n");
    
    // Demostrar truncamiento
    printf("Truncamiento: ");
    e.reservado = 16; // Se trunca a 0 (16 & 0xF = 0)
    printf("16 -> %u, ", e.reservado);
    e.reservado = 17; // Se trunca a 1 (17 & 0xF = 1)
    printf("17 -> %u\n", e.reservado);
}
