# Control de Errores Robusto

Este ejercicio demuestra la importancia del control de errores robusto en C, comparando código inseguro que no verifica errores con implementaciones seguras que incluyen verificación completa de errores.

## Descripción

El ejercicio incluye:

1. **Función insegura** - Ejemplo de código que NO verifica errores
2. **Función segura** - Implementación con verificación completa de errores
3. **Utilities robustas** - Funciones auxiliares para manejo seguro de recursos
4. **Sistema de códigos de error** - Manejo consistente de errores

## Conceptos Demostrados

### Problemas del Código Inseguro

```c
// ❌ INSEGURO - No verifica errores
FILE *archivo = fopen("datos.txt", "w");
char *buffer = malloc(100);
int numero;

fprintf(archivo, "Escribiendo datos...\n");  // Puede fallar
scanf("%d", &numero);                       // Puede fallar
sprintf(buffer, "Número: %d", numero);      // Buffer overflow
fprintf(archivo, "%s\n", buffer);           // Puede fallar

fclose(archivo);
free(buffer);
```

**Problemas:**
- `fopen()` puede fallar (permisos, espacio en disco, etc.)
- `malloc()` puede fallar (memoria insuficiente)
- `scanf()` puede fallar (entrada inválida)
- `fprintf()` puede fallar (error de E/S)
- No hay verificación de ningún error

### Código Seguro

```c
// ✅ SEGURO - Verifica todos los errores
result_t funcion_segura(void) {
    FILE *archivo = NULL;
    char *buffer = NULL;
    int numero;
    result_t resultado;
    
    // Verificar cada operación
    resultado = abrir_archivo_seguro("datos.txt", "w", &archivo);
    if (!resultado.success) {
        return resultado;
    }
    
    resultado = asignar_memoria_seguro(100, (void**)&buffer);
    if (!resultado.success) {
        liberar_recursos_seguro(archivo, NULL);
        return resultado;
    }
    
    // ... más verificaciones ...
    
    liberar_recursos_seguro(archivo, buffer);
    return result_ok();
}
```

## Características del Sistema de Control de Errores

### 1. Códigos de Error Consistentes

```c
typedef enum {
    ERROR_NONE = 0,
    ERROR_FILE_OPEN = 1,
    ERROR_MEMORY_ALLOC = 2,
    ERROR_INVALID_INPUT = 3,
    ERROR_FILE_READ = 4,
    ERROR_FILE_WRITE = 5
} error_code_t;
```

### 2. Estructura de Resultado

```c
typedef struct {
    int success;
    error_code_t error_code;
    char error_message[256];
} result_t;
```

### 3. Funciones Auxiliares Seguras

- `abrir_archivo_seguro()` - Apertura de archivos con verificación
- `asignar_memoria_seguro()` - Asignación de memoria con verificación
- `leer_entero_seguro()` - Lectura de entrada con validación
- `escribir_datos_seguro()` - Escritura con verificación de errores
- `liberar_recursos_seguro()` - Liberación segura de recursos

## Buenas Prácticas Implementadas

### 1. Verificación de Valores de Retorno

```c
FILE *file = fopen("archivo.txt", "r");
if (!file) {
    perror("Error al abrir archivo");
    return ERROR_FILE_OPEN;
}
```

### 2. Validación de Parámetros

```c
if (!filename || !mode || !file) {
    return result_error(ERROR_INVALID_INPUT, "Parámetros nulos");
}
```

### 3. Liberación de Recursos en Todos los Caminos

```c
resultado = operacion1();
if (!resultado.success) {
    liberar_recursos_seguro(archivo, buffer);
    return resultado;
}

resultado = operacion2();
if (!resultado.success) {
    liberar_recursos_seguro(archivo, buffer);
    return resultado;
}

liberar_recursos_seguro(archivo, buffer);
return result_ok();
```

### 4. Mensajes de Error Descriptivos

```c
char mensaje[256];
snprintf(mensaje, sizeof(mensaje), 
         "Error al abrir archivo '%s': %s", 
         filename, strerror(errno));
return result_error(ERROR_FILE_OPEN, mensaje);
```

## Compilación

```bash
mkdir build
cd build
cmake ..
make
```

### Targets Disponibles

- `control_errores_robusto` - Programa principal interactivo
- `test_control_errores_robusto` - Suite de pruebas
- `demo_comparacion` - Demostración comparativa
- `error_simulator` - Simulador de errores

## Ejecución

### Programa Principal

```bash
./control_errores_robusto
```

El programa ofrece un menú interactivo para:

1. Ejecutar función insegura (con advertencias)
2. Ejecutar función segura
3. Demostrar manejo de archivos seguro
4. Demostrar manejo de memoria seguro
5. Demostrar lectura de entrada segura
6. Mostrar información sobre buenas prácticas

### Herramientas de Demostración

```bash
# Comparación lado a lado
./demo_comparacion

# Simulador de errores
./error_simulator
```

### Pruebas

```bash
# Todas las pruebas
make test

# Pruebas específicas
./test_control_errores_robusto --filter=archivo_seguro
./test_control_errores_robusto --filter=memoria_segura
./test_control_errores_robusto --filter=entrada_segura
```

## Ejemplos de Uso

### Apertura Segura de Archivos

```c
FILE *archivo = NULL;
result_t resultado = abrir_archivo_seguro("datos.txt", "r", &archivo);

if (!resultado.success) {
    imprimir_error(&resultado);
    return -1;
}

// Usar archivo...

liberar_recursos_seguro(archivo, NULL);
```

### Asignación Segura de Memoria

```c
char *buffer = NULL;
result_t resultado = asignar_memoria_seguro(1024, (void**)&buffer);

if (!resultado.success) {
    imprimir_error(&resultado);
    return -1;
}

// Usar buffer...

liberar_recursos_seguro(NULL, buffer);
```

### Lectura Segura de Entrada

```c
int valor;
result_t resultado = leer_entero_seguro(&valor);

if (!resultado.success) {
    printf("Error en la entrada: %s\n", resultado.error_message);
    return -1;
}

printf("Valor leído: %d\n", valor);
```

## Cobertura de Pruebas

Las pruebas cubren:

- ✅ Funciones auxiliares de resultado
- ✅ Manejo seguro de archivos
- ✅ Manejo seguro de memoria
- ✅ Lectura segura de entrada
- ✅ Funciones inseguras vs seguras
- ✅ Manejo de errores
- ✅ Pruebas de integración
- ✅ Casos límite y parámetros inválidos

## Lecciones Clave

1. **Siempre verificar valores de retorno** de funciones que pueden fallar
2. **Validar parámetros** antes de usarlos
3. **Liberar recursos** en todos los caminos de ejecución
4. **Proporcionar mensajes de error descriptivos**
5. **Usar códigos de error consistentes**
6. **Implementar principio fail-fast**
7. **Manejar casos límite y errores de sistema**

## Herramientas de Desarrollo

- **Análisis estático**: `make lint` (requiere cppcheck)
- **Formateo**: `make format` (requiere clang-format)
- **Limpieza**: `make clean-all`

## Importancia en Programación Real

El control robusto de errores es fundamental para:

- **Estabilidad** - Prevenir crashes y comportamiento impredecible
- **Seguridad** - Evitar vulnerabilidades por estados inválidos
- **Mantenibilidad** - Facilitar debugging y diagnóstico
- **Confiabilidad** - Garantizar operación correcta en condiciones adversas
- **Experiencia de usuario** - Proporcionar feedback útil sobre errores

Este ejercicio demuestra que el tiempo invertido en verificación de errores siempre vale la pena para crear software robusto y confiable.
