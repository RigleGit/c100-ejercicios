#ifndef BITFIELDS_ESTADO_H
#define BITFIELDS_ESTADO_H

#include <stdbool.h>

/**
 * @brief Estructura que demuestra el uso de bit fields en C
 * 
 * Los bit fields permiten especificar el número exacto de bits
 * que debe usar cada campo, optimizando el uso de memoria.
 * 
 * En esta estructura:
 * - activo: 1 bit (0 o 1)
 * - conectado: 1 bit (0 o 1)  
 * - error: 1 bit (0 o 1)
 * - modo_debug: 1 bit (0 o 1)
 * - reservado: 4 bits (0 a 15)
 * 
 * Total: 8 bits = 1 byte (más padding del compilador si es necesario)
 */
typedef struct {
    unsigned int activo : 1;        // Estado activo/inactivo del sistema
    unsigned int conectado : 1;     // Estado de conexión
    unsigned int error : 1;         // Indica si hay un error
    unsigned int modo_debug : 1;    // Modo de depuración activado
    unsigned int reservado : 4;     // Bits reservados para uso futuro
} Estado;

/**
 * @brief Estructura equivalente sin bit fields para comparación
 * 
 * Esta estructura usa variables individuales para cada campo,
 * mostrando la diferencia en uso de memoria.
 */
typedef struct {
    unsigned int activo;        // 4 bytes
    unsigned int conectado;     // 4 bytes
    unsigned int error;         // 4 bytes
    unsigned int modo_debug;    // 4 bytes
    unsigned int reservado;     // 4 bytes
} EstadoSinBitFields;          // Total: 20 bytes

// Máscaras para manipulación mediante operaciones bit a bit
#define ESTADO_ACTIVO     0x01  // Bit 0
#define ESTADO_CONECTADO  0x02  // Bit 1
#define ESTADO_ERROR      0x04  // Bit 2
#define ESTADO_DEBUG      0x08  // Bit 3

// Funciones de visualización y análisis
void mostrar_estado(const Estado* e);
const char* estado_a_string(const Estado* e);

// Funciones de configuración básica
void configurar_estado_inicial(Estado* e);
void activar_modo_debug(Estado* e);
void desactivar_modo_debug(Estado* e);
void reportar_error(Estado* e);
void limpiar_error(Estado* e);
void conectar(Estado* e);
void desconectar(Estado* e);

// Funciones de consulta
bool esta_activo(const Estado* e);
bool esta_conectado(const Estado* e);
bool hay_error(const Estado* e);
bool esta_en_debug(const Estado* e);
bool esta_operativo(const Estado* e);

// Funciones de manipulación avanzada
void configurar_con_mascara(Estado* e, unsigned int mascara);
unsigned int estado_a_mascara(const Estado* e);

// Funciones de utilidad
void copiar_estado(const Estado* origen, Estado* destino);
bool comparar_estados(const Estado* e1, const Estado* e2);

// Funciones educativas
void demostrar_limites(void);

#endif // BITFIELDS_ESTADO_H
