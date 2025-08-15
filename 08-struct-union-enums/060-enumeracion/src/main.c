#include <stdio.h>
#include "enumeracion.h"

// Función básica del enunciado
void ejemplo_basico_enunciado(void) {
    printf("=== Ejercicio 060: Enumeración de Días de la Semana ===\n");
    printf("Ejemplo básico del enunciado:\n\n");
    
    int num;
    printf("Introduce un número del 1 al 7: ");
    scanf("%d", &num);
    
    enum DiaSemana dia = (enum DiaSemana) num;
    printf("Día seleccionado: %s\n", nombreDia(dia));
}

// Demostración completa de enumeraciones
void demostracion_completa(void) {
    printf("=== Demostración Completa de Enumeraciones ===\n\n");
    
    // 1. Ejemplo básico del enunciado con múltiples valores
    printf("1. Conversión de números a días:\n");
    for (int i = 1; i <= 7; i++) {
        enum DiaSemana dia = numeroDia(i);
        printf("   %d -> %s\n", i, nombreDia(dia));
    }
    
    // Ejemplo con número inválido
    printf("   8 -> %s\n", nombreDia(numeroDia(8)));
    
    // 2. Mostrar información completa de días
    printf("\n");
    mostrarTodosDiasSemana();
    
    // 3. Demostración de operaciones con días
    printf("\n2. Operaciones con días:\n");
    enum DiaSemana hoy = MIE; // Miércoles
    demostracionDiasSemana(hoy);
    
    // 4. Contar días laborales
    printf("\n3. Conteo de días laborales:\n");
    printf("   De Lunes a Viernes: %d días laborales\n", 
           contarDiasLaborales(LUN, VIE));
    printf("   De Miércoles a Martes: %d días laborales\n", 
           contarDiasLaborales(MIE, MAR));
    
    // 5. Búsqueda por nombre
    printf("\n4. Búsqueda por nombre:\n");
    const char* nombres[] = {"Lunes", "lun", "Sábado", "Dom", "Miércoles"};
    int num_nombres = sizeof(nombres) / sizeof(nombres[0]);
    
    for (int i = 0; i < num_nombres; i++) {
        enum DiaSemana dia = stringADia(nombres[i]);
        if (esDiaValido(dia)) {
            printf("   '%s' -> %s (valor: %d)\n", nombres[i], nombreDia(dia), dia);
        } else {
            printf("   '%s' -> No encontrado\n", nombres[i]);
        }
    }
    
    // 6. Mostrar meses del año
    mostrarTodosMeses();
    
    // 7. Análisis de estaciones
    printf("\n5. Análisis de estaciones:\n");
    enum Mes meses_ejemplo[] = {MARZO, JUNIO, SEPTIEMBRE, DICIEMBRE};
    int num_meses = sizeof(meses_ejemplo) / sizeof(meses_ejemplo[0]);
    
    for (int i = 0; i < num_meses; i++) {
        enum Mes mes = meses_ejemplo[i];
        enum Estacion estacion = estacionDelMes(mes);
        printf("   %s -> %s (%d días)\n", 
               nombreMes(mes), nombreEstacion(estacion), diasDelMes(mes, false));
    }
    
    // 8. Máquina de estados
    demostracionMaquinaEstados();
    
    // 9. Demostración de prioridades y errores
    printf("\n6. Enumeraciones auxiliares:\n");
    printf("Prioridades: %s < %s < %s < %s\n",
           descripcionPrioridad(BAJA), descripcionPrioridad(MEDIA),
           descripcionPrioridad(ALTA), descripcionPrioridad(CRITICA));
    
    printf("Errores comunes:\n");
    enum CodigoError errores[] = {SIN_ERROR, ERROR_ENTRADA, ERROR_MEMORIA};
    for (int i = 0; i < 3; i++) {
        printf("   %s: %s\n", 
               esError(errores[i]) ? "ERROR" : "OK", 
               descripcionError(errores[i]));
    }
}

// Programa de ejemplo simple (para testing)
void programa_simple(void) {
    printf("=== Programa Simple - Días de la Semana ===\n");
    
    // Usar datos predefinidos para testing automático
    int numeros[] = {1, 3, 6, 7, 0, 8};
    int cantidad = sizeof(numeros) / sizeof(numeros[0]);
    
    for (int i = 0; i < cantidad; i++) {
        printf("Número %d -> %s\n", numeros[i], nombreDia(numeroDia(numeros[i])));
    }
    
    printf("\nCiclo de la semana:\n");
    enum DiaSemana dia = LUN;
    for (int i = 0; i < 7; i++) {
        printf("%s -> %s\n", nombreDia(dia), nombreDia(diaSiguiente(dia)));
        dia = diaSiguiente(dia);
    }
}

#ifndef UNIT_TESTING
int main(void) {
    // Para permitir testing automático, usamos el programa simple por defecto
    programa_simple();
    
    // Para usar el ejemplo del enunciado original, descomenta:
    // ejemplo_basico_enunciado();
    
    // Para la demostración completa, descomenta:
    // demostracion_completa();
    
    // Para el programa interactivo completo, descomenta:
    // programaInteractivoEnum();
    
    return 0;
}
#endif
