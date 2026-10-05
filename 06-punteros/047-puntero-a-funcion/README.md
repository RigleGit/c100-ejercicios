# Ejercicio 047: Puntero a Función

## 📋 Descripción

Este ejercicio introduce los punteros a funciones, una característica avanzada del lenguaje C que permite almacenar la dirección de funciones en variables, pasarlas como parámetros o seleccionarlas dinámicamente. Es fundamental para implementar callbacks, tablas de funciones, interfaces genéricas y sistemas de plugins.

## 🎯 Objetivo

Implementar y demostrar el uso de punteros a funciones en C, explorando diferentes formas de declaración, asignación y uso, así como aplicaciones prácticas como calculadoras dinámicas, sistemas de callbacks y algoritmos parametrizables.

## 📝 Enunciado

**Punteros a Funciones Básico:**

Declarar un puntero a una función que reciba dos enteros y devuelva su suma. Usar el puntero para llamar a la función desde main.

**Ejemplo de código base:**
```c
// Función que suma dos enteros
int sumar(int a, int b) {
    return a + b;
}

int main() {
    // Declaración de un puntero a función que recibe dos int y devuelve un int
    int (*ptrFuncion)(int, int);

    // Asignación de la dirección de la función 'sumar'
    ptrFuncion = &sumar;

    // Llamada a la función a través del puntero
    int resultado = ptrFuncion(10, 5);

    printf("Resultado: %d\n", resultado);

    return 0;
}
```

**Salida esperada:**
```
Resultado: 15
```

## 🔧 Funciones Implementadas

### Funciones Básicas de Operaciones

1. **`sumar(int a, int b)`** - Suma dos enteros
2. **`restar(int a, int b)`** - Resta dos enteros
3. **`multiplicar(int a, int b)`** - Multiplica dos enteros
4. **`dividir(int a, int b)`** - División entera (maneja división por cero)
5. **`modulo(int a, int b)`** - Operación módulo
6. **`potencia(int base, int exponente)`** - Potenciación entera
7. **`maximo(int a, int b)`** - Encuentra el máximo
8. **`minimo(int a, int b)`** - Encuentra el mínimo
9. **`mcd(int a, int b)`** - Máximo Común Divisor (Euclides)
10. **`mcm(int a, int b)`** - Mínimo Común Múltiplo

### Funciones que Usan Punteros a Funciones

11. **`calculadora(int a, int b, int (*operacion)(int, int), const char *nombre)`**
    - Calculadora genérica que acepta cualquier operación

12. **`aplicar_operacion_array(int pares[][2], int num_pares, int (*operacion)(int, int), const char *nombre)`**
    - Aplica una operación a un array de pares

13. **`ejecutar_funciones(int a, int b, int (*funciones[])(int, int), const char *nombres[], int num_funciones)`**
    - Ejecuta múltiples funciones con los mismos parámetros

14. **`calculadora_dinamica(int a, int b, char operador)`**
    - Selecciona dinámicamente la operación según un carácter

### Funciones de Ordenamiento y Comparación

15. **`comparar(int a, int b)`** - Función de comparación
16. **`ordenar_con_funcion(int array[], int tamaño, int (*comp)(int, int))`**
    - Ordenamiento usando función de comparación personalizada

### Sistema de Callbacks

17. **`procesar_numero(int numero, int (*callback)(int, int))`**
    - Aplica un callback a un número y su doble

18. **`demo_callbacks(int numeros[], int tamaño, int (*callback)(int, int), const char *nombre)`**
    - Demuestra el uso de callbacks

19. **`buscar_par_condicion(int array[][2], int tamaño, int (*condicion)(int, int), int *resultado)`**
    - Busca el primer par que cumple una condición

### Funciones de Demostración

20. **`demo_declaraciones()`** - Muestra diferentes formas de declarar punteros a funciones

## 📊 Ejemplo de Salida

```
=== PUNTEROS A FUNCIONES ===

🔹 EJEMPLO BÁSICO:
Resultado de suma(10, 5): 15

🔹 ANÁLISIS DETALLADO:
Dirección de la función sumar: 0x100003f30
Valor del puntero ptrFuncion: 0x100003f30
¿Apuntan al mismo lugar? Sí

Diferentes formas de llamar la función:
  sumar(3, 7) = 10
  ptrFuncion(3, 7) = 10
  (*ptrFuncion)(3, 7) = 10

🔹 DIFERENTES FORMAS DE DECLARAR PUNTEROS A FUNCIONES:

Forma 1 - Declaración directa:
  int (*ptr1)(int, int) = sumar;
  ptr1(3, 4) = 7

Forma 2 - Declaración y asignación separadas:
  int (*ptr2)(int, int);
  ptr2 = restar;
  ptr2(10, 3) = 7

Forma 3 - Con typedef:
  typedef int (*OperacionBinaria)(int, int);
  OperacionBinaria ptr3 = multiplicar;
  ptr3(5, 6) = 30

Forma 4 - Array de punteros a funciones:
  int (*operaciones[])(int, int) = {sumar, restar, multiplicar, dividir};
  operaciones[0](8, 2) = 10
  operaciones[1](8, 2) = 6
  operaciones[2](8, 2) = 16
  operaciones[3](8, 2) = 4

🔹 CALCULADORA CON PUNTEROS A FUNCIONES:
Ejecutando suma(20, 4):
  Resultado: 24

Ejecutando resta(20, 4):
  Resultado: 16

🔹 CALCULADORA DINÁMICA:
Operación seleccionada: suma
Operador '+': 20
Operación seleccionada: resta
Operador '-': 4
Operación seleccionada: multiplicación
Operador '*': 96
```

## 🔍 Conceptos Clave

### 1. **Sintaxis de Declaración**
```c
// Formato general
tipo_retorno (*nombre_puntero)(tipos_parametros);

// Ejemplos
int (*ptr)(int, int);                    // Puntero a función que retorna int
void (*callback)(char *);                // Puntero a función void
double (*operacion)(double, double);     // Puntero a función que retorna double
```

### 2. **Asignación de Funciones**
```c
int (*ptr)(int, int);

ptr = sumar;        // Asignación directa
ptr = &sumar;       // Con operador de dirección (equivalente)
```

### 3. **Llamada a Funciones**
```c
int resultado;

resultado = ptr(5, 3);      // Llamada directa
resultado = (*ptr)(5, 3);   // Con desreferenciación (equivalente)
```

### 4. **Arrays de Punteros a Funciones**
```c
int (*operaciones[])(int, int) = {sumar, restar, multiplicar};

int resultado = operaciones[0](10, 5);  // Llama a sumar
```

### 5. **Typedef para Simplificar**
```c
typedef int (*OperacionBinaria)(int, int);

OperacionBinaria op = sumar;
int resultado = op(7, 3);
```

### 6. **Funciones como Parámetros**
```c
int aplicar_operacion(int a, int b, int (*func)(int, int)) {
    return func(a, b);
}

int resultado = aplicar_operacion(5, 3, sumar);
```

## ⚠️ Consideraciones Importantes

### Seguridad
- **Verificar punteros nulos** antes de llamar funciones
- **Coincidir firmas** exactamente (tipos de parámetros y retorno)
- **Inicializar punteros** antes de usarlos

### Rendimiento
- Llamadas a través de punteros tienen overhead mínimo
- El compilador puede optimizar muchas llamadas
- Útil para evitar código duplicado

### Portabilidad
- Las direcciones de funciones varían entre compilaciones
- El comportamiento es estándar en C11
- Compatible con diferentes arquitecturas

### Debugging
- Más difícil de debuggear que llamadas directas
- Use nombres descriptivos para punteros
- Documente el propósito de cada puntero a función

## 🎓 Aplicaciones Prácticas

### 1. **Sistemas de Callbacks**
```c
// Registro de eventos
void registrar_callback(void (*callback)(int));

// Notificación de eventos
callback(evento_data);
```

### 2. **Tablas de Funciones**
```c
// Dispatcher de comandos
struct Comando {
    char *nombre;
    int (*ejecutar)(int argc, char *argv[]);
};
```

### 3. **Algoritmos Parametrizables**
```c
// Ordenamiento con función de comparación personalizada
void quicksort(int array[], int tamaño, int (*comparar)(int, int));
```

### 4. **Máquinas de Estado**
```c
// Estados como funciones
typedef Estado (*FuncionEstado)(Evento evento);

Estado estado_actual = estado_inicial;
estado_actual = estado_actual(evento);
```

### 5. **Interfaces Genéricas**
```c
// Procesador de datos genérico
void procesar_datos(void *datos, void (*procesador)(void *));
```

### 6. **Sistemas de Plugins**
```c
// Cargar funciones dinámicamente
int (*plugin_init)(void) = dlsym(handle, "init");
```

## 🔗 Relación con Otros Conceptos

### Programación Orientada a Objetos
- Emula métodos virtuales
- Polimorfismo en C
- Implementación de interfaces

### Programación Funcional
- Funciones de orden superior
- Funciones como datos
- Composición de funciones

### Sistemas Operativos
- Handlers de señales
- Drivers de dispositivos
- Syscalls

### Arquitectura de Software
- Patrón Strategy
- Patrón Observer
- Inversión de dependencias

## 📚 Referencias

- **Kernighan & Ritchie**: "The C Programming Language", Capítulo 5.11
- **C11 Standard**: ISO/IEC 9899:2011, Sección 6.3.2.1
- **Expert C Programming**: Peter van der Linden

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/puntero_a_funcion
```

## Pruebas de esta revisión

Contratos de la API probados contra la implementación real.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

Los ejemplos de ampliación son propuestas educativas; esta revisión compila el programa y las pruebas de [CMakeLists.txt](CMakeLists.txt).
