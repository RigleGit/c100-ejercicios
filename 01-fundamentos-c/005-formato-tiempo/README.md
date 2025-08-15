# Ejercicio 005: Formato de Tiempo

## Enunciado

Escribe un programa que reciba una cantidad total de segundos e imprima su equivalente en horas, minutos y segundos.

## Descripción

Este ejercicio te ayuda a practicar operaciones con enteros, especialmente la división y el operador módulo (`%`), que son esenciales para descomponer cantidades en partes. También te introduce al formato de salida más estructurado, útil en aplicaciones como temporizadores, cronómetros o sistemas de registro.

## Conceptos Matemáticos

### Conversiones de Tiempo
- **1 hora** = 3600 segundos
- **1 minuto** = 60 segundos

### Algoritmo de Descomposición
```
horas = total_segundos ÷ 3600
minutos = (total_segundos % 3600) ÷ 60
segundos = total_segundos % 60
```

## Conceptos Clave

- **División entera**: `/` para obtener cociente completo
- **Operador módulo**: `%` para obtener el resto de una división
- **Descomposición secuencial**: Calcular componentes de mayor a menor
- **Formato con ceros**: `%02d` para mostrar siempre dos dígitos
- **Formato estructurado**: Salida tipo `hh:mm:ss`
- **Operaciones aritméticas**: Combinación de división y módulo

## Estructura del Proyecto

```
005-formato-tiempo/
├── CMakeLists.txt          # Configuración de compilación
├── README.md               # Este archivo
├── src/
│   └── main.c             # Código fuente principal
└── test/
    └── test_main.c        # Tests del programa
```

## Compilación y Ejecución

### Usando CMake (Recomendado)

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar el programa
./formato_tiempo

# Ejecutar tests
make test
# o directamente:
./test_formato_tiempo
```

### Compilación Manual

```bash
# Compilar el programa principal
gcc -o formato_tiempo src/main.c

# Ejecutar
./formato_tiempo

# Compilar y ejecutar tests
gcc -o test_formato_tiempo test/test_main.c
./test_formato_tiempo
```

## Ejemplos de Ejecución

```
Introduce el total de segundos: 3725
Equivalente: 01:02:05
```

```
Introduce el total de segundos: 125
Equivalente: 00:02:05
```

```
Introduce el total de segundos: 7265
Equivalente: 02:01:05
```

## Casos de Prueba

Los tests verifican diferentes escenarios temporales:
- ✅ **Cero segundos**: 0 → 00:00:00
- ✅ **Solo segundos**: 45 → 00:00:45
- ✅ **Minutos y segundos**: 125 → 00:02:05
- ✅ **Una hora exacta**: 3600 → 01:00:00
- ✅ **Tiempo mixto**: 3725 → 01:02:05
- ✅ **Múltiples horas**: 7265 → 02:01:05
- ✅ **Formato correcto**: Verificación de `%02d`

## Notas de Aprendizaje

1. **`horas = total_segundos / 3600;`**: División entera para obtener horas completas
2. **`minutos = (total_segundos % 3600) / 60;`**: Resto de horas dividido en minutos
3. **`segundos = total_segundos % 60;`**: Segundos restantes después de horas y minutos
4. **`%02d`**: Formato que garantiza dos dígitos con cero a la izquierda
5. **Paréntesis**: Importante para el orden correcto de operaciones

## Tabla de Conversiones de Referencia

| Total Segundos | Cálculo | Resultado | Formato |
|----------------|---------|-----------|---------|
| 0 | 0÷3600, 0÷60, 0 | 0h 0m 0s | 00:00:00 |
| 59 | 0÷3600, 59÷60, 59 | 0h 0m 59s | 00:00:59 |
| 60 | 60÷3600, 60÷60, 0 | 0h 1m 0s | 00:01:00 |
| 3600 | 3600÷3600, 0÷60, 0 | 1h 0m 0s | 01:00:00 |
| 3661 | 3661÷3600, 61÷60, 1 | 1h 1m 1s | 01:01:01 |

## Desglose del Algoritmo

### Paso 1: Calcular Horas
```c
horas = total_segundos / 3600;
// Ejemplo: 7265 / 3600 = 2 horas
```

### Paso 2: Calcular Minutos
```c
minutos = (total_segundos % 3600) / 60;
// Ejemplo: (7265 % 3600) / 60 = 65 / 60 = 1 minuto
```

### Paso 3: Calcular Segundos Restantes
```c
segundos = total_segundos % 60;
// Ejemplo: 7265 % 60 = 5 segundos
```

## Aplicaciones Prácticas

- **Cronómetros deportivos**
- **Sistemas de tiempo de trabajo**
- **Duración de multimedia**
- **Logs de sistema**
- **Temporizadores de cocina**
- **Medición de rendimiento**

## Posibles Errores Comunes

1. **Orden incorrecto**: Calcular minutos antes que horas
2. **Olvidar paréntesis**: `total_segundos % 3600 / 60` vs `(total_segundos % 3600) / 60`
3. **Formato incorrecto**: Usar `%d` en lugar de `%02d`
4. **Constantes incorrectas**: Usar valores diferentes a 3600 y 60
5. **No manejar el caso 0**: Aunque funciona automáticamente

## Extensiones Posibles

- Agregar validación para números negativos
- Incluir días en el formato (dd:hh:mm:ss)
- Crear función reversa (hh:mm:ss → segundos totales)
- Implementar formato 12 horas con AM/PM
- Agregar milisegundos para mayor precisión
- Crear calculadora de tiempo con suma/resta
- Implementar diferentes formatos de salida
