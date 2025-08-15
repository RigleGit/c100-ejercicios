#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Implementación básica del enunciado: puntero void con tipos básicos
 * Esta función demuestra el uso fundamental de void* con casting explícito
 */
void ejemploBasicoVoid(void) {
    printf("=== EJEMPLO BÁSICO DEL ENUNCIADO ===\n");
    printf("Demostración de puntero void* con tipos básicos\n\n");
    
    int a = 42;
    float b = 3.14f;
    char c = 'Z';

    void *ptr;

    printf("Variables originales:\n");
    printf("int a = %d\n", a);
    printf("float b = %.2f\n", b);
    printf("char c = '%c'\n\n", c);

    // Apuntar a un int
    ptr = &a;
    printf("Usando void* para acceder a int:\n");
    printf("  ptr = &a; (dirección: %p)\n", ptr);
    printf("  Valor de a: %d\n", *(int *)ptr);
    printf("  Casting: *(int *)ptr\n\n");

    // Apuntar a un float
    ptr = &b;
    printf("Usando void* para acceder a float:\n");
    printf("  ptr = &b; (dirección: %p)\n", ptr);
    printf("  Valor de b: %.2f\n", *(float *)ptr);
    printf("  Casting: *(float *)ptr\n\n");

    // Apuntar a un char
    ptr = &c;
    printf("Usando void* para acceder a char:\n");
    printf("  ptr = &c; (dirección: %p)\n", ptr);
    printf("  Valor de c: '%c'\n", *(char *)ptr);
    printf("  Casting: *(char *)ptr\n\n");
    
    printf("Conceptos clave demostrados:\n");
    printf("1. void* puede apuntar a cualquier tipo\n");
    printf("2. Requiere casting explícito para desreferenciar\n");
    printf("3. El programador debe recordar el tipo correcto\n");
    printf("4. Base para funciones genéricas como malloc()\n");
    printf("====================================\n\n");
}

/**
 * Función genérica para mostrar contenido de memoria
 * @param ptr Puntero void a los datos
 * @param size Tamaño en bytes de los datos
 * @param tipo_nombre Nombre descriptivo del tipo
 */
void mostrarMemoria(void *ptr, size_t size, const char *tipo_nombre) {
    if (ptr == NULL) {
        printf("Error: Puntero nulo\n");
        return;
    }
    
    printf("=== Inspección de memoria: %s ===\n", tipo_nombre);
    printf("Dirección: %p\n", ptr);
    printf("Tamaño: %zu bytes\n", size);
    printf("Contenido hexadecimal: ");
    
    unsigned char *byte_ptr = (unsigned char*)ptr;
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", byte_ptr[i]);
    }
    
    printf("\nContenido decimal: ");
    for (size_t i = 0; i < size; i++) {
        printf("%3u ", byte_ptr[i]);
    }
    
    printf("\nContenido como caracteres: ");
    for (size_t i = 0; i < size; i++) {
        if (byte_ptr[i] >= 32 && byte_ptr[i] <= 126) {
            printf("%c", byte_ptr[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
    printf("========================================\n\n");
}

/**
 * Función genérica para copiar memoria
 * @param destino Puntero void al destino
 * @param origen Puntero void al origen
 * @param size Número de bytes a copiar
 */
void copiarMemoria(void *destino, const void *origen, size_t size) {
    if (destino == NULL || origen == NULL) {
        printf("Error: Punteros nulos en copiarMemoria\n");
        return;
    }
    
    printf("=== Copia de memoria ===\n");
    printf("Origen: %p\n", origen);
    printf("Destino: %p\n", destino);
    printf("Bytes a copiar: %zu\n", size);
    
    // Implementación manual de memcpy usando void*
    unsigned char *dst = (unsigned char*)destino;
    const unsigned char *src = (const unsigned char*)origen;
    
    for (size_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }
    
    printf("Copia completada\n");
    printf("========================\n\n");
}

/**
 * Función genérica para comparar memoria
 * @param ptr1 Primer puntero void
 * @param ptr2 Segundo puntero void
 * @param size Número de bytes a comparar
 * @return 0 si son iguales, != 0 si diferentes
 */
int compararMemoria(const void *ptr1, const void *ptr2, size_t size) {
    if (ptr1 == NULL || ptr2 == NULL) {
        printf("Error: Punteros nulos en compararMemoria\n");
        return -1;
    }
    
    printf("=== Comparación de memoria ===\n");
    printf("Puntero 1: %p\n", ptr1);
    printf("Puntero 2: %p\n", ptr2);
    printf("Bytes a comparar: %zu\n", size);
    
    const unsigned char *p1 = (const unsigned char*)ptr1;
    const unsigned char *p2 = (const unsigned char*)ptr2;
    
    for (size_t i = 0; i < size; i++) {
        if (p1[i] != p2[i]) {
            printf("Diferencia en byte %zu: 0x%02X vs 0x%02X\n", i, p1[i], p2[i]);
            printf("Resultado: DIFERENTES\n");
            printf("==============================\n\n");
            return (int)(p1[i] - p2[i]);
        }
    }
    
    printf("Resultado: IGUALES\n");
    printf("==============================\n\n");
    return 0;
}

/**
 * Función genérica para intercambiar contenido de memoria
 * @param ptr1 Primer puntero void
 * @param ptr2 Segundo puntero void
 * @param size Número de bytes a intercambiar
 */
void intercambiarMemoria(void *ptr1, void *ptr2, size_t size) {
    if (ptr1 == NULL || ptr2 == NULL) {
        printf("Error: Punteros nulos en intercambiarMemoria\n");
        return;
    }
    
    printf("=== Intercambio de memoria ===\n");
    printf("Puntero 1: %p\n", ptr1);
    printf("Puntero 2: %p\n", ptr2);
    printf("Bytes a intercambiar: %zu\n", size);
    
    unsigned char *p1 = (unsigned char*)ptr1;
    unsigned char *p2 = (unsigned char*)ptr2;
    unsigned char temp;
    
    for (size_t i = 0; i < size; i++) {
        temp = p1[i];
        p1[i] = p2[i];
        p2[i] = temp;
    }
    
    printf("Intercambio completado\n");
    printf("==============================\n\n");
}

/**
 * Función genérica para llenar memoria con un valor
 * @param ptr Puntero void a la memoria
 * @param valor Valor byte a usar para llenar
 * @param size Número de bytes a llenar
 */
void llenarMemoria(void *ptr, int valor, size_t size) {
    if (ptr == NULL) {
        printf("Error: Puntero nulo en llenarMemoria\n");
        return;
    }
    
    printf("=== Llenado de memoria ===\n");
    printf("Dirección: %p\n", ptr);
    printf("Valor de llenado: 0x%02X (%d)\n", valor & 0xFF, valor & 0xFF);
    printf("Bytes a llenar: %zu\n", size);
    
    unsigned char *byte_ptr = (unsigned char*)ptr;
    unsigned char byte_valor = (unsigned char)(valor & 0xFF);
    
    for (size_t i = 0; i < size; i++) {
        byte_ptr[i] = byte_valor;
    }
    
    printf("Llenado completado\n");
    printf("==========================\n\n");
}

/**
 * Función para buscar un patrón de bytes en memoria
 * @param memoria Puntero void a la memoria donde buscar
 * @param size_memoria Tamaño de la memoria
 * @param patron Puntero void al patrón a buscar
 * @param size_patron Tamaño del patrón
 * @return Puntero a la primera ocurrencia o NULL si no se encuentra
 */
void* buscarPatron(const void *memoria, size_t size_memoria, 
                   const void *patron, size_t size_patron) {
    if (memoria == NULL || patron == NULL || size_patron == 0 || size_patron > size_memoria) {
        return NULL;
    }
    
    printf("=== Búsqueda de patrón ===\n");
    printf("Memoria: %p (tamaño: %zu)\n", memoria, size_memoria);
    printf("Patrón: %p (tamaño: %zu)\n", patron, size_patron);
    
    const unsigned char *mem = (const unsigned char*)memoria;
    const unsigned char *pat = (const unsigned char*)patron;
    
    printf("Patrón a buscar: ");
    for (size_t i = 0; i < size_patron; i++) {
        printf("%02X ", pat[i]);
    }
    printf("\n");
    
    for (size_t i = 0; i <= size_memoria - size_patron; i++) {
        int encontrado = 1;
        for (size_t j = 0; j < size_patron; j++) {
            if (mem[i + j] != pat[j]) {
                encontrado = 0;
                break;
            }
        }
        
        if (encontrado) {
            printf("Patrón encontrado en offset %zu\n", i);
            printf("Dirección: %p\n", (void*)(mem + i));
            printf("=========================\n\n");
            return (void*)(mem + i);
        }
    }
    
    printf("Patrón no encontrado\n");
    printf("=========================\n\n");
    return NULL;
}

/**
 * Función de callback para diferentes tipos de datos
 * Usa void* para ser genérica
 */
typedef void (*procesarCallback)(void *dato, size_t size, const char *nombre);

/**
 * Procesa un array de elementos usando callback y void*
 * @param array Puntero void al array
 * @param num_elementos Número de elementos
 * @param size_elemento Tamaño de cada elemento
 * @param callback Función callback para procesar cada elemento
 * @param tipo_nombre Nombre del tipo para descripción
 */
void procesarArray(void *array, size_t num_elementos, size_t size_elemento,
                   procesarCallback callback, const char *tipo_nombre) {
    if (array == NULL || callback == NULL) {
        printf("Error: Parámetros nulos en procesarArray\n");
        return;
    }
    
    printf("=== Procesamiento de array con void* ===\n");
    printf("Tipo: %s\n", tipo_nombre);
    printf("Número de elementos: %zu\n", num_elementos);
    printf("Tamaño por elemento: %zu bytes\n", size_elemento);
    printf("Tamaño total: %zu bytes\n", num_elementos * size_elemento);
    printf("Dirección base: %p\n\n", array);
    
    unsigned char *ptr = (unsigned char*)array;
    
    for (size_t i = 0; i < num_elementos; i++) {
        printf("Elemento %zu:\n", i);
        void *elemento = ptr + (i * size_elemento);
        callback(elemento, size_elemento, tipo_nombre);
    }
    
    printf("========================================\n\n");
}

/**
 * Callback para mostrar elementos como enteros
 */
void mostrarComoEntero(void *dato, size_t size, const char *nombre) {
    if (size == sizeof(int)) {
        int *valor = (int*)dato;
        printf("  Dirección: %p\n", dato);
        printf("  Valor como %s: %d\n", nombre, *valor);
        mostrarMemoria(dato, size, "int");
    }
}

/**
 * Callback para mostrar elementos como flotantes
 */
void mostrarComoFloat(void *dato, size_t size, const char *nombre) {
    if (size == sizeof(float)) {
        float *valor = (float*)dato;
        printf("  Dirección: %p\n", dato);
        printf("  Valor como %s: %.2f\n", nombre, *valor);
        mostrarMemoria(dato, size, "float");
    }
}

/**
 * Función para demostrar aritmética de punteros void (requiere casting)
 */
void demostrarAritmeticaVoid(void) {
    printf("=== Aritmética con punteros void ===\n");
    
    int array[] = {10, 20, 30, 40, 50};
    void *ptr_void = array;
    
    printf("Array original: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");
    
    printf("Acceso usando void* (requiere casting):\n");
    
    // No podemos hacer aritmética directa con void*, necesitamos casting
    for (int i = 0; i < 5; i++) {
        // Casting a char* para aritmética de bytes
        void *elemento = (char*)ptr_void + (i * sizeof(int));
        int *valor = (int*)elemento;
        
        printf("Elemento %d:\n", i);
        printf("  Offset en bytes: %d\n", i * (int)sizeof(int));
        printf("  Dirección: %p\n", elemento);
        printf("  Valor: %d\n", *valor);
        printf("\n");
    }
    
    printf("====================================\n\n");
}

/**
 * Función principal que demuestra el uso de punteros void
 */
int main(void) {
    printf("=== EJERCICIO 050: PUNTEROS VOID ===\n\n");
    
    // 0. Ejemplo básico del enunciado (introducción)
    printf("0. INTRODUCCIÓN: EJEMPLO BÁSICO\n");
    printf("-------------------------------\n");
    ejemploBasicoVoid();
    
    // 1. Demostración básica con diferentes tipos
    printf("1. INSPECCIÓN DE DIFERENTES TIPOS CON VOID*\n");
    printf("-------------------------------------------\n");
    
    int entero = 0x12345678;
    float flotante = 3.14159f;
    double doble = 2.71828;
    char cadena[] = "Hola";
    
    mostrarMemoria(&entero, sizeof(entero), "int");
    mostrarMemoria(&flotante, sizeof(flotante), "float");
    mostrarMemoria(&doble, sizeof(doble), "double");
    mostrarMemoria(cadena, strlen(cadena) + 1, "string");
    
    // 2. Copia genérica de memoria
    printf("2. COPIA GENÉRICA DE MEMORIA\n");
    printf("----------------------------\n");
    
    int original = 42;
    int copia;
    
    printf("Antes de la copia:\n");
    mostrarMemoria(&original, sizeof(original), "original");
    mostrarMemoria(&copia, sizeof(copia), "copia (sin inicializar)");
    
    copiarMemoria(&copia, &original, sizeof(int));
    
    printf("Después de la copia:\n");
    mostrarMemoria(&original, sizeof(original), "original");
    mostrarMemoria(&copia, sizeof(copia), "copia");
    
    // 3. Comparación de memoria
    printf("3. COMPARACIÓN DE MEMORIA\n");
    printf("-------------------------\n");
    
    int a = 100;
    int b = 100;
    int c = 200;
    
    compararMemoria(&a, &b, sizeof(int));
    compararMemoria(&a, &c, sizeof(int));
    
    // 4. Intercambio genérico
    printf("4. INTERCAMBIO GENÉRICO\n");
    printf("-----------------------\n");
    
    int x = 111;
    int y = 222;
    
    printf("Antes del intercambio:\n");
    mostrarMemoria(&x, sizeof(x), "x");
    mostrarMemoria(&y, sizeof(y), "y");
    
    intercambiarMemoria(&x, &y, sizeof(int));
    
    printf("Después del intercambio:\n");
    mostrarMemoria(&x, sizeof(x), "x");
    mostrarMemoria(&y, sizeof(y), "y");
    
    // 5. Llenado de memoria
    printf("5. LLENADO DE MEMORIA\n");
    printf("---------------------\n");
    
    char buffer[10];
    printf("Buffer antes del llenado:\n");
    mostrarMemoria(buffer, sizeof(buffer), "buffer");
    
    llenarMemoria(buffer, 0xAA, sizeof(buffer));
    
    printf("Buffer después del llenado:\n");
    mostrarMemoria(buffer, sizeof(buffer), "buffer");
    
    // 6. Búsqueda de patrones
    printf("6. BÚSQUEDA DE PATRONES\n");
    printf("-----------------------\n");
    
    unsigned char datos[] = {0x01, 0x02, 0x03, 0xAB, 0xCD, 0x04, 0x05, 0xAB, 0xCD, 0x06};
    unsigned char patron[] = {0xAB, 0xCD};
    
    printf("Datos donde buscar:\n");
    mostrarMemoria(datos, sizeof(datos), "datos");
    
    void *encontrado = buscarPatron(datos, sizeof(datos), patron, sizeof(patron));
    if (encontrado) {
        size_t offset = (unsigned char*)encontrado - datos;
        printf("Primera ocurrencia encontrada en offset %zu\n\n", offset);
    }
    
    // 7. Procesamiento de arrays con callbacks
    printf("7. PROCESAMIENTO CON CALLBACKS\n");
    printf("-------------------------------\n");
    
    int array_int[] = {10, 20, 30};
    float array_float[] = {1.5f, 2.5f, 3.5f};
    
    procesarArray(array_int, 3, sizeof(int), mostrarComoEntero, "int");
    procesarArray(array_float, 3, sizeof(float), mostrarComoFloat, "float");
    
    // 8. Aritmética con void*
    printf("8. ARITMÉTICA CON PUNTEROS VOID\n");
    printf("-------------------------------\n");
    demostrarAritmeticaVoid();
    
    // 9. Ejemplo práctico: función genérica de ordenamiento
    printf("9. EJEMPLO PRÁCTICO: FUNCIÓN GENÉRICA\n");
    printf("-------------------------------------\n");
    
    int numeros[] = {5, 2, 8, 1, 9};
    printf("Array antes de procesar:\n");
    mostrarMemoria(numeros, sizeof(numeros), "array de enteros");
    
    // Simular procesamiento genérico
    void *ptr_generico = numeros;
    printf("Acceso genérico al primer elemento: %d\n", *(int*)ptr_generico);
    printf("Acceso genérico al segundo elemento: %d\n", 
           *((int*)((char*)ptr_generico + sizeof(int))));
    
    printf("\nPunteros void permiten:\n");
    printf("- Funciones genéricas que trabajan con cualquier tipo\n");
    printf("- Implementación de estructuras de datos genéricas\n");
    printf("- APIs que no dependen de tipos específicos\n");
    printf("- Interfaz uniforme para malloc/free\n\n");
    
    return 0;
}
