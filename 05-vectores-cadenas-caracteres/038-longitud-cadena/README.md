# Ejercicio 038: Longitud de Cadena

## Descripción
Este ejercicio implementa una función para calcular la longitud de una cadena de caracteres sin utilizar la función `strlen()` de la biblioteca estándar. Es un ejercicio fundamental para entender cómo funcionan las cadenas en C y el concepto de terminador nulo.

## Conceptos Clave

### 1. Cadenas en C
- Las cadenas en C son arrays de caracteres terminados en `'\0'` (nulo)
- El terminador nulo no se cuenta en la longitud de la cadena
- Las cadenas literales incluyen automáticamente el terminador nulo

### 2. Recorrido de Cadenas
- Se puede recorrer carácter por carácter hasta encontrar `'\0'`
- Método fundamental para muchas operaciones con cadenas
- Base para entender funciones como `strlen()`, `strcpy()`, etc.

### 3. Diferencias con `fgets()`
- `fgets()` puede incluir el carácter `'\n'` al final
- Importante distinguir entre longitud visible y longitud total
- Manejo de entrada del usuario con seguridad

## Algoritmo

### Algoritmo Básico
```
1. Inicializar contador = 0
2. MIENTRAS cadena[contador] != '\0':
   a. Incrementar contador
3. RETORNAR contador
```

### Complejidad
- **Tiempo:** O(n) donde n es la longitud de la cadena
- **Espacio:** O(1) - solo variables auxiliares

## Código de Ejemplo

### Implementación Principal
```c
int calcular_longitud_cadena(const char *cadena) {
    int longitud = 0;
    while (cadena[longitud] != '\0') {
        longitud++;
    }
    return longitud;
}
```

### Variante con Punteros
```c
int longitud_con_punteros(const char *cadena) {
    const char *inicio = cadena;
    while (*cadena) {
        cadena++;
    }
    return cadena - inicio;
}
```

## Casos de Prueba

### Casos Básicos
- Cadena vacía: `""` → longitud = 0
- Un carácter: `"a"` → longitud = 1
- Cadena normal: `"Hola"` → longitud = 4

### Casos Especiales
- Cadenas con espacios: `"Hola mundo"` → longitud = 10
- Cadenas con números: `"123"` → longitud = 3
- Cadenas con caracteres especiales: `"¡Hola!"` → longitud = 6

### Casos con `fgets()`
- Entrada con newline: `"test\n"` → longitud sin newline = 4
- Entrada sin newline: `"test"` → longitud = 4

## Compilación y Ejecución

### Usando CMake (Recomendado)
```bash
mkdir build && cd build
cmake ..
make
./longitud_cadena
```

### Compilación Manual
```bash
gcc -Wall -Wextra -std=c99 src/main.c -o longitud_cadena
./longitud_cadena
```

### Ejecutar Tests
```bash
# Instalar Criterion primero (macOS)
brew install criterion

# Compilar y ejecutar tests
cd build
make run_tests
```

## Ejemplo de Uso

### Entrada
```
Introduce una cadena: Hola mundo
```

### Salida
```
=== Calculadora de Longitud de Cadenas ===
Introduce una cadena: Hola mundo
La longitud de la cadena es: 10
Longitud total (incluyendo '\n' si existe): 11

=== Análisis de la cadena ===
Cadena introducida: "Hola mundo
"
Caracteres analizados:
  [0]: 'H' (ASCII: 72)
  [1]: 'o' (ASCII: 111)
  [2]: 'l' (ASCII: 97)
  [3]: 'a' (ASCII: 97)
  [4]: ' ' (ASCII: 32)
  [5]: 'm' (ASCII: 109)
  [6]: 'u' (ASCII: 117)
  [7]: 'n' (ASCII: 110)
  [8]: 'd' (ASCII: 100)
  [9]: 'o' (ASCII: 111)
  [10]: '\n' (ASCII: 10)
  [11]: '\0' (ASCII: 0) <- Terminador nulo
```

## Aspectos Pedagógicos

### Conceptos que Refuerza
1. **Representación de cadenas en memoria**
2. **Importancia del terminador nulo**
3. **Diferencia entre longitud lógica y física**
4. **Recorrido básico de arrays**
5. **Manejo seguro de entrada del usuario**

### Errores Comunes
1. **Incluir el terminador nulo en el conteo**
2. **No manejar cadenas vacías**
3. **No considerar el newline de `fgets()`**
4. **Desbordamiento de buffer en entrada**

### Extensiones Posibles
1. **Implementar con aritmética de punteros**
2. **Versión recursiva**
3. **Contar solo caracteres alfabéticos**
4. **Manejar cadenas Unicode (UTF-8)**

## Aplicaciones Prácticas

### En Sistemas Reales
- **Validación de entrada:** Verificar longitud antes de procesar
- **Interfaz de usuario:** Mostrar contadores de caracteres
- **Protocolos de red:** Calcular longitud para headers
- **Bases de datos:** Validar longitud de campos

### Optimizaciones
- **Cache de longitud:** Almacenar resultado para cadenas estáticas
- **Vectorización:** Usar instrucciones SIMD para cadenas largas
- **Límites:** Versión con longitud máxima para seguridad

## Relación con Otros Ejercicios
- **Prerequisito para:** Inversión de cadenas, búsqueda en cadenas
- **Combina con:** Copia de cadenas, comparación de cadenas
- **Fundamento de:** Todas las operaciones de manipulación de strings

## Recursos Adicionales
- Manual de C: Sección sobre strings y arrays
- Documentación de `string.h` para comparar con implementaciones estándar
- Estudios de rendimiento de diferentes algoritmos de longitud de cadena
