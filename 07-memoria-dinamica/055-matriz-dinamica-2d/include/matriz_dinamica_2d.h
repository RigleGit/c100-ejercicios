#ifndef MATRIZ_DINAMICA_2D_H
#define MATRIZ_DINAMICA_2D_H

/**
 * @file matriz_dinamica_2d.h
 * @brief Biblioteca para manejo de matrices dinámicas 2D
 * 
 * Esta biblioteca proporciona funciones para crear, manipular y liberar
 * matrices bidimensionales de enteros usando memoria dinámica.
 */

/**
 * @brief Crea una matriz dinámica 2D de enteros
 * 
 * Reserva memoria dinámicamente para una matriz de dimensiones especificadas.
 * La matriz se implementa como un array de punteros, donde cada puntero
 * apunta a una fila de la matriz.
 * 
 * @param filas Número de filas de la matriz (debe ser > 0)
 * @param columnas Número de columnas de la matriz (debe ser > 0)
 * @return Puntero a la matriz creada, o NULL si hay error
 * 
 * @note El llamador es responsable de liberar la memoria con liberar_matriz_2d()
 */
int** crear_matriz_2d(int filas, int columnas);

/**
 * @brief Libera la memoria de una matriz dinámica 2D
 * 
 * Libera toda la memoria asociada con la matriz, incluyendo las filas
 * individuales y el array de punteros.
 * 
 * @param matriz Puntero a la matriz a liberar
 * @param filas Número de filas de la matriz
 * 
 * @note Es seguro pasar NULL como matriz
 */
void liberar_matriz_2d(int** matriz, int filas);

/**
 * @brief Rellena la matriz con valores introducidos manualmente
 * 
 * Solicita al usuario que introduzca cada elemento de la matriz.
 * 
 * @param matriz Puntero a la matriz
 * @param filas Número de filas
 * @param columnas Número de columnas
 */
void rellenar_manual(int** matriz, int filas, int columnas);

/**
 * @brief Rellena la matriz automáticamente con valores i*j
 * 
 * Cada elemento [i][j] se establece al valor i*j.
 * 
 * @param matriz Puntero a la matriz
 * @param filas Número de filas
 * @param columnas Número de columnas
 */
void rellenar_automatico(int** matriz, int filas, int columnas);

/**
 * @brief Rellena la matriz con valores aleatorios
 * 
 * Cada elemento se establece a un valor aleatorio entre 0 y 99.
 * 
 * @param matriz Puntero a la matriz
 * @param filas Número de filas
 * @param columnas Número de columnas
 */
void rellenar_aleatorio(int** matriz, int filas, int columnas);

/**
 * @brief Muestra la matriz en formato tabular
 * 
 * Imprime la matriz con formateo apropiado para facilitar la lectura.
 * 
 * @param matriz Puntero a la matriz
 * @param filas Número de filas
 * @param columnas Número de columnas
 */
void mostrar_matriz(int** matriz, int filas, int columnas);

/**
 * @brief Calcula y muestra estadísticas de la matriz
 * 
 * Calcula suma, promedio, valores mínimo y máximo con sus posiciones.
 * 
 * @param matriz Puntero a la matriz
 * @param filas Número de filas
 * @param columnas Número de columnas
 */
void mostrar_estadisticas(int** matriz, int filas, int columnas);

/**
 * @brief Busca un valor específico en la matriz
 * 
 * Busca todas las ocurrencias de un valor e imprime sus posiciones.
 * 
 * @param matriz Puntero a la matriz
 * @param filas Número de filas
 * @param columnas Número de columnas
 * @param valor Valor a buscar
 */
void buscar_valor(int** matriz, int filas, int columnas, int valor);

/**
 * @brief Crea una matriz básica con valor inicial (para tests)
 * 
 * Versión simplificada que crea una matriz y la inicializa con un valor.
 * 
 * @param filas Número de filas
 * @param columnas Número de columnas
 * @param valor_inicial Valor para inicializar todos los elementos
 * @return Puntero a la matriz creada, o NULL si hay error
 */
int** crear_matriz_basica(int filas, int columnas, int valor_inicial);

/**
 * @brief Obtiene un elemento de la matriz
 * 
 * @param matriz Puntero a la matriz
 * @param fila Índice de la fila
 * @param columna Índice de la columna
 * @return Valor del elemento en la posición especificada
 */
int obtener_elemento(int** matriz, int fila, int columna);

/**
 * @brief Establece un elemento de la matriz
 * 
 * @param matriz Puntero a la matriz
 * @param fila Índice de la fila
 * @param columna Índice de la columna
 * @param valor Nuevo valor para el elemento
 */
void establecer_elemento(int** matriz, int fila, int columna, int valor);

/**
 * @brief Copia una matriz a otra nueva
 * 
 * @param origen Matriz origen a copiar
 * @param filas Número de filas
 * @param columnas Número de columnas
 * @return Puntero a la nueva matriz copiada, o NULL si hay error
 */
int** copiar_matriz(int** origen, int filas, int columnas);

#endif // MATRIZ_DINAMICA_2D_H
