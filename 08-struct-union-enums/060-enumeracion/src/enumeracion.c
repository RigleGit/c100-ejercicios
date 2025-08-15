#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "enumeracion.h"

// Implementación de la función principal del enunciado
const char* nombreDia(enum DiaSemana dia) {
    switch (dia) {
        case LUN: return "Lunes";
        case MAR: return "Martes";
        case MIE: return "Miércoles";
        case JUE: return "Jueves";
        case VIE: return "Viernes";
        case SAB: return "Sábado";
        case DOM: return "Domingo";
        default: return "Día inválido";
    }
}

enum DiaSemana numeroDia(int numero) {
    if (numero >= LUN && numero <= DOM) {
        return (enum DiaSemana)numero;
    }
    return (enum DiaSemana)0; // Valor inválido
}

bool esDiaValido(int numero) {
    return (numero >= LUN && numero <= DOM);
}

// Funciones extendidas para días

enum DiaSemana diaSiguiente(enum DiaSemana dia) {
    switch (dia) {
        case LUN: return MAR;
        case MAR: return MIE;
        case MIE: return JUE;
        case JUE: return VIE;
        case VIE: return SAB;
        case SAB: return DOM;
        case DOM: return LUN;
        default: return dia; // Mantener día inválido
    }
}

enum DiaSemana diaAnterior(enum DiaSemana dia) {
    switch (dia) {
        case LUN: return DOM;
        case MAR: return LUN;
        case MIE: return MAR;
        case JUE: return MIE;
        case VIE: return JUE;
        case SAB: return VIE;
        case DOM: return SAB;
        default: return dia; // Mantener día inválido
    }
}

bool esFinDeSemana(enum DiaSemana dia) {
    return (dia == SAB || dia == DOM);
}

bool esDiaLaboral(enum DiaSemana dia) {
    return (dia >= LUN && dia <= VIE);
}

const char* nombreDiaAbreviado(enum DiaSemana dia) {
    switch (dia) {
        case LUN: return "Lun";
        case MAR: return "Mar";
        case MIE: return "Mié";
        case JUE: return "Jue";
        case VIE: return "Vie";
        case SAB: return "Sáb";
        case DOM: return "Dom";
        default: return "???";
    }
}

const char* nombreDiaIngles(enum DiaSemana dia) {
    switch (dia) {
        case LUN: return "Monday";
        case MAR: return "Tuesday";
        case MIE: return "Wednesday";
        case JUE: return "Thursday";
        case VIE: return "Friday";
        case SAB: return "Saturday";
        case DOM: return "Sunday";
        default: return "Invalid day";
    }
}

// Funciones para meses

const char* nombreMes(enum Mes mes) {
    switch (mes) {
        case ENERO: return "Enero";
        case FEBRERO: return "Febrero";
        case MARZO: return "Marzo";
        case ABRIL: return "Abril";
        case MAYO: return "Mayo";
        case JUNIO: return "Junio";
        case JULIO: return "Julio";
        case AGOSTO: return "Agosto";
        case SEPTIEMBRE: return "Septiembre";
        case OCTUBRE: return "Octubre";
        case NOVIEMBRE: return "Noviembre";
        case DICIEMBRE: return "Diciembre";
        default: return "Mes inválido";
    }
}

int diasDelMes(enum Mes mes, bool esBisiesto) {
    switch (mes) {
        case ENERO: case MARZO: case MAYO: case JULIO:
        case AGOSTO: case OCTUBRE: case DICIEMBRE:
            return 31;
        case ABRIL: case JUNIO: case SEPTIEMBRE: case NOVIEMBRE:
            return 30;
        case FEBRERO:
            return esBisiesto ? 29 : 28;
        default:
            return 0; // Mes inválido
    }
}

enum Estacion estacionDelMes(enum Mes mes) {
    switch (mes) {
        case MARZO: case ABRIL: case MAYO:
            return PRIMAVERA;
        case JUNIO: case JULIO: case AGOSTO:
            return VERANO;
        case SEPTIEMBRE: case OCTUBRE: case NOVIEMBRE:
            return OTONO;
        case DICIEMBRE: case ENERO: case FEBRERO:
            return INVIERNO;
        default:
            return PRIMAVERA; // Default
    }
}

// Funciones para estaciones

const char* nombreEstacion(enum Estacion estacion) {
    switch (estacion) {
        case PRIMAVERA: return "Primavera";
        case VERANO: return "Verano";
        case OTONO: return "Otoño";
        case INVIERNO: return "Invierno";
        default: return "Estación inválida";
    }
}

enum Estacion estacionSiguiente(enum Estacion estacion) {
    switch (estacion) {
        case PRIMAVERA: return VERANO;
        case VERANO: return OTONO;
        case OTONO: return INVIERNO;
        case INVIERNO: return PRIMAVERA;
        default: return estacion;
    }
}

// Funciones para estados de máquina

const char* descripcionEstado(enum EstadoMaquina estado) {
    switch (estado) {
        case APAGADO: return "Máquina apagada";
        case INICIANDO: return "Iniciando sistema";
        case FUNCIONANDO: return "Funcionando normalmente";
        case PAUSADO: return "Sistema pausado";
        case ERROR: return "Error del sistema";
        default: return "Estado desconocido";
    }
}

bool estadoPermiteTransicion(enum EstadoMaquina estado) {
    return (estado != ERROR); // En estado de error no se permiten transiciones normales
}

enum EstadoMaquina siguienteEstado(enum EstadoMaquina estadoActual) {
    switch (estadoActual) {
        case APAGADO: return INICIANDO;
        case INICIANDO: return FUNCIONANDO;
        case FUNCIONANDO: return PAUSADO;
        case PAUSADO: return FUNCIONANDO;
        case ERROR: return APAGADO; // Reset desde error
        default: return APAGADO;
    }
}

// Funciones para prioridades

const char* descripcionPrioridad(enum Prioridad prioridad) {
    switch (prioridad) {
        case BAJA: return "Prioridad baja";
        case MEDIA: return "Prioridad media";
        case ALTA: return "Prioridad alta";
        case CRITICA: return "Prioridad crítica";
        default: return "Prioridad desconocida";
    }
}

int compararPrioridades(enum Prioridad p1, enum Prioridad p2) {
    return (int)p1 - (int)p2;
}

// Funciones para códigos de error

const char* descripcionError(enum CodigoError codigo) {
    switch (codigo) {
        case SIN_ERROR: return "Operación exitosa";
        case ERROR_ENTRADA: return "Error en datos de entrada";
        case ERROR_MEMORIA: return "Error de memoria";
        case ERROR_ARCHIVO: return "Error de archivo";
        case ERROR_RED: return "Error de red";
        case ERROR_DESCONOCIDO: return "Error desconocido";
        default: return "Código de error inválido";
    }
}

bool esError(enum CodigoError codigo) {
    return (codigo != SIN_ERROR);
}

// Funciones de demostración

void mostrarTodosDiasSemana(void) {
    printf("=== Días de la Semana ===\n");
    printf("Valor | Nombre      | Abrev | Inglés      | Tipo\n");
    printf("------|-------------|-------|-------------|-------------\n");
    
    for (int i = LUN; i <= DOM; i++) {
        enum DiaSemana dia = (enum DiaSemana)i;
        printf("%5d | %-11s | %-5s | %-11s | %s\n",
               i,
               nombreDia(dia),
               nombreDiaAbreviado(dia),
               nombreDiaIngles(dia),
               esDiaLaboral(dia) ? "Laboral" : "Fin semana");
    }
}

void mostrarTodosMeses(void) {
    printf("\n=== Meses del Año ===\n");
    printf("Valor | Nombre      | Días | Días(bis) | Estación\n");
    printf("------|-------------|------|-----------|---------------\n");
    
    for (int i = ENERO; i <= DICIEMBRE; i++) {
        enum Mes mes = (enum Mes)i;
        printf("%5d | %-11s | %4d | %9d | %s\n",
               i,
               nombreMes(mes),
               diasDelMes(mes, false),
               diasDelMes(mes, true),
               nombreEstacion(estacionDelMes(mes)));
    }
}

void demostracionDiasSemana(enum DiaSemana dia) {
    printf("\n=== Demostración con %s ===\n", nombreDia(dia));
    printf("Día actual: %s (valor: %d)\n", nombreDia(dia), dia);
    printf("Día siguiente: %s\n", nombreDia(diaSiguiente(dia)));
    printf("Día anterior: %s\n", nombreDia(diaAnterior(dia)));
    printf("Es fin de semana: %s\n", esFinDeSemana(dia) ? "Sí" : "No");
    printf("Es día laboral: %s\n", esDiaLaboral(dia) ? "Sí" : "No");
    printf("Abreviatura: %s\n", nombreDiaAbreviado(dia));
    printf("En inglés: %s\n", nombreDiaIngles(dia));
}

void demostracionMaquinaEstados(void) {
    printf("\n=== Demostración de Máquina de Estados ===\n");
    
    enum EstadoMaquina estado = APAGADO;
    
    for (int i = 0; i < 8; i++) {
        printf("Estado %d: %-25s (valor: %2d) - %s transición\n",
               i + 1,
               descripcionEstado(estado),
               estado,
               estadoPermiteTransicion(estado) ? "Permite" : "No permite");
        
        if (i < 7) { // No cambiar en la última iteración
            enum EstadoMaquina siguiente = siguienteEstado(estado);
            printf("         -> Transición a: %s\n", descripcionEstado(siguiente));
            estado = siguiente;
        }
    }
}

enum DiaSemana stringADia(const char* nombre) {
    if (nombre == NULL) return (enum DiaSemana)0;
    
    // Comparaciones para nombres completos en español
    if (strcmp(nombre, "Lunes") == 0 || strcmp(nombre, "lunes") == 0) return LUN;
    if (strcmp(nombre, "Martes") == 0 || strcmp(nombre, "martes") == 0) return MAR;
    if (strcmp(nombre, "Miércoles") == 0 || strcmp(nombre, "miércoles") == 0) return MIE;
    if (strcmp(nombre, "Jueves") == 0 || strcmp(nombre, "jueves") == 0) return JUE;
    if (strcmp(nombre, "Viernes") == 0 || strcmp(nombre, "viernes") == 0) return VIE;
    if (strcmp(nombre, "Sábado") == 0 || strcmp(nombre, "sábado") == 0) return SAB;
    if (strcmp(nombre, "Domingo") == 0 || strcmp(nombre, "domingo") == 0) return DOM;
    
    // Comparaciones para abreviaciones
    if (strcmp(nombre, "Lun") == 0 || strcmp(nombre, "lun") == 0) return LUN;
    if (strcmp(nombre, "Mar") == 0 || strcmp(nombre, "mar") == 0) return MAR;
    if (strcmp(nombre, "Mié") == 0 || strcmp(nombre, "mié") == 0) return MIE;
    if (strcmp(nombre, "Jue") == 0 || strcmp(nombre, "jue") == 0) return JUE;
    if (strcmp(nombre, "Vie") == 0 || strcmp(nombre, "vie") == 0) return VIE;
    if (strcmp(nombre, "Sáb") == 0 || strcmp(nombre, "sáb") == 0) return SAB;
    if (strcmp(nombre, "Dom") == 0 || strcmp(nombre, "dom") == 0) return DOM;
    
    return (enum DiaSemana)0; // No encontrado
}

int contarDiasLaborales(enum DiaSemana inicio, enum DiaSemana fin) {
    if (!esDiaValido(inicio) || !esDiaValido(fin)) {
        return 0;
    }
    
    int contador = 0;
    enum DiaSemana actual = inicio;
    
    // Manejar el caso donde fin < inicio (da la vuelta a la semana)
    do {
        if (esDiaLaboral(actual)) {
            contador++;
        }
        
        if (actual == fin) break;
        actual = diaSiguiente(actual);
        
        // Prevenir bucle infinito
        if (contador > 7) break;
        
    } while (true);
    
    return contador;
}

void programaInteractivoEnum(void) {
    int opcion;
    
    printf("=== Explorador de Enumeraciones ===\n");
    
    do {
        printf("\n--- Menú Principal ---\n");
        printf("1.  Convertir número a día (enunciado)\n");
        printf("2.  Mostrar todos los días de la semana\n");
        printf("3.  Mostrar todos los meses del año\n");
        printf("4.  Operaciones con días específicos\n");
        printf("5.  Demostración de máquina de estados\n");
        printf("6.  Buscar día por nombre\n");
        printf("7.  Contar días laborales\n");
        printf("8.  Demostración de prioridades\n");
        printf("9.  Demostración de códigos de error\n");
        printf("10. Análisis de estaciones\n");
        printf("0.  Salir\n");
        printf("Selecciona una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Opción inválida.\n");
            // Limpiar buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        switch (opcion) {
            case 1: {
                int num;
                printf("Introduce un número del 1 al 7: ");
                if (scanf("%d", &num) == 1) {
                    enum DiaSemana dia = numeroDia(num);
                    if (esDiaValido(num)) {
                        printf("Día seleccionado: %s\n", nombreDia(dia));
                    } else {
                        printf("Número inválido. Debe ser del 1 al 7.\n");
                    }
                } else {
                    printf("Error: Número inválido.\n");
                }
                break;
            }
            
            case 2:
                mostrarTodosDiasSemana();
                break;
                
            case 3:
                mostrarTodosMeses();
                break;
                
            case 4: {
                int num;
                printf("Introduce un día (1-7) para análisis: ");
                if (scanf("%d", &num) == 1 && esDiaValido(num)) {
                    demostracionDiasSemana((enum DiaSemana)num);
                } else {
                    printf("Día inválido.\n");
                }
                break;
            }
            
            case 5:
                demostracionMaquinaEstados();
                break;
                
            case 6: {
                char nombre[20];
                printf("Introduce el nombre del día: ");
                if (scanf("%19s", nombre) == 1) {
                    enum DiaSemana dia = stringADia(nombre);
                    if (esDiaValido(dia)) {
                        printf("Día encontrado: %s (valor: %d)\n", nombreDia(dia), dia);
                    } else {
                        printf("Día no encontrado. Intenta con nombres como 'Lunes' o 'Lun'.\n");
                    }
                }
                break;
            }
            
            case 7: {
                int inicio, fin;
                printf("Introduce día de inicio (1-7): ");
                if (scanf("%d", &inicio) == 1 && esDiaValido(inicio)) {
                    printf("Introduce día de fin (1-7): ");
                    if (scanf("%d", &fin) == 1 && esDiaValido(fin)) {
                        int laborales = contarDiasLaborales((enum DiaSemana)inicio, (enum DiaSemana)fin);
                        printf("Días laborales desde %s hasta %s: %d\n",
                               nombreDia((enum DiaSemana)inicio),
                               nombreDia((enum DiaSemana)fin),
                               laborales);
                    } else {
                        printf("Día de fin inválido.\n");
                    }
                } else {
                    printf("Día de inicio inválido.\n");
                }
                break;
            }
            
            case 8: {
                printf("\n=== Demostración de Prioridades ===\n");
                enum Prioridad prioridades[] = {BAJA, MEDIA, ALTA, CRITICA};
                int num_prioridades = sizeof(prioridades) / sizeof(prioridades[0]);
                
                for (int i = 0; i < num_prioridades; i++) {
                    printf("Prioridad %d: %s (valor: %d)\n",
                           i + 1, descripcionPrioridad(prioridades[i]), prioridades[i]);
                }
                
                printf("\nComparaciones:\n");
                printf("MEDIA vs ALTA: %d\n", compararPrioridades(MEDIA, ALTA));
                printf("ALTA vs BAJA: %d\n", compararPrioridades(ALTA, BAJA));
                break;
            }
            
            case 9: {
                printf("\n=== Demostración de Códigos de Error ===\n");
                enum CodigoError errores[] = {SIN_ERROR, ERROR_ENTRADA, ERROR_MEMORIA, ERROR_ARCHIVO, ERROR_RED, ERROR_DESCONOCIDO};
                int num_errores = sizeof(errores) / sizeof(errores[0]);
                
                for (int i = 0; i < num_errores; i++) {
                    printf("Código %d: %s (valor: %d) - %s\n",
                           i + 1,
                           descripcionError(errores[i]),
                           errores[i],
                           esError(errores[i]) ? "Es error" : "Sin error");
                }
                break;
            }
            
            case 10: {
                printf("\n=== Análisis de Estaciones ===\n");
                for (int i = PRIMAVERA; i <= INVIERNO; i++) {
                    enum Estacion estacion = (enum Estacion)i;
                    printf("Estación %d: %s -> Siguiente: %s\n",
                           i, nombreEstacion(estacion), nombreEstacion(estacionSiguiente(estacion)));
                }
                break;
            }
            
            case 0:
                printf("¡Hasta luego!\n");
                break;
                
            default:
                printf("Opción inválida. Selecciona del 0 al 10.\n");
                break;
        }
        
        if (opcion != 0) {
            printf("\nPresiona Enter para continuar...");
            getchar(); // Consumir newline del scanf anterior
            getchar(); // Esperar Enter del usuario
        }
        
    } while (opcion != 0);
}
