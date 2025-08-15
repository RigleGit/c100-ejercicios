# Ejercicio 099: Agenda de Contactos

## Descripción

Proyecto final que implementa una agenda de contactos completa usando estructuras de datos dinámicas, listas enlazadas, manejo de archivos y un sistema de menús interactivo. Este ejercicio integra múltiples conceptos fundamentales de programación en C.

## Características Principales

### 🔧 Funcionalidades Core
- **Gestión completa de contactos**: Agregar, buscar, modificar, eliminar
- **Sistema de favoritos**: Marcar contactos importantes
- **Categorización**: Organizar por familia, trabajo, amigos, otros
- **Búsqueda avanzada**: Por nombre, teléfono, email, categoría
- **Validación robusta**: Control de datos de entrada

### 💾 Persistencia y Archivos
- **Formato CSV**: Almacenamiento estándar e intercambiable
- **Auto-guardado**: Prevención de pérdida de datos
- **Sistema de backups**: Copias automáticas con timestamp
- **Importación/Exportación**: Intercambio de datos por categorías
- **Recuperación de emergencia**: Guardado automático en interrupciones

### 📊 Estadísticas y Reportes
- **Contadores por categoría**: Distribución de contactos
- **Análisis de favoritos**: Seguimiento de contactos importantes
- **Estadísticas de archivos**: Información sobre tamaño y contenido
- **Reportes de uso**: Fechas de creación y modificación

### 🛡️ Seguridad y Robustez
- **Manejo seguro de memoria**: Prevención de memory leaks
- **Validación de entrada**: Funciones seguras (fgets, snprintf)
- **Control de errores**: Manejo robusto de fallos
- **Límites de buffer**: Protección contra overflow

## Estructura del Proyecto

```
099-agenda-contactos/
├── src/
│   ├── main.c              # Programa principal
│   ├── contacto.c          # Gestión de contactos
│   ├── persistencia.c      # Manejo de archivos
│   └── menu.c              # Sistema de menús
├── include/
│   ├── contacto.h          # Definiciones de contacto
│   ├── persistencia.h      # API de persistencia
│   └── menu.h              # API de menús
├── tests/
│   └── (tests opcionales)
├── tools/
│   └── (herramientas adicionales)
├── data/
│   ├── agenda.csv          # Archivo principal
│   └── backups/            # Copias de seguridad
├── Makefile                # Sistema de compilación
├── README.md               # Esta documentación
└── .gitignore              # Archivos a ignorar
```

## Estructura de Datos

### Contacto
```c
typedef struct {
    char nombre[MAX_NOMBRE];
    char telefono[MAX_TELEFONO];
    char email[MAX_EMAIL];
    Fecha fecha_nacimiento;
    char categoria[MAX_CATEGORIA];
    char notas[MAX_NOTAS];
    int es_favorito;
    time_t fecha_creacion;
    time_t fecha_modificacion;
} Contacto;
```

### Lista Enlazada
```c
typedef struct Nodo {
    Contacto contacto;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *cabeza;
    int tamaño;
} Lista;
```

## Compilación y Uso

### Compilar
```bash
# Compilación básica
make

# Compilación con debug
make debug

# Limpiar archivos objeto
make clean
```

### Ejecutar
```bash
# Ejecución normal
./agenda_contactos

# Con datos de prueba
./agenda_contactos --demo

# Mostrar ayuda
./agenda_contactos --help

# Mostrar versión
./agenda_contactos --version
```

### Comandos Make Disponibles
- `make` o `make all`: Compilar el programa
- `make debug`: Compilar con información de debug
- `make clean`: Limpiar archivos generados
- `make install`: Instalar en el sistema (opcional)
- `make test`: Ejecutar tests (si están implementados)

## Formato de Archivos

### CSV (agenda.csv)
```csv
nombre,telefono,email,fecha_nacimiento,categoria,notas
"Juan Pérez","+34-123-456-789","juan@email.com","1985-03-15","trabajo","Compañero"
"María García","+34-987-654-321","maria@email.com","1990-07-22","familia","Prima"
```

### Backups Automáticos
- **Formato**: `agenda_backup_YYYYMMDD_HHMMSS.csv`
- **Ubicación**: `data/`
- **Frecuencia**: Manual o en interrupciones

## Funcionalidades del Menú

### 1. Gestión de Contactos
- **Agregar**: Nuevo contacto con validación completa
- **Buscar**: Por múltiples criterios (nombre, teléfono, email, categoría)
- **Listar**: Todos los contactos o por categorías
- **Modificar**: Actualizar información existente
- **Eliminar**: Remover contactos con confirmación

### 2. Sistema de Favoritos
- **Marcar/Desmarcar**: Gestión de contactos importantes
- **Listar favoritos**: Vista filtrada
- **Exportar favoritos**: Archivo separado

### 3. Gestión de Archivos
- **Guardar/Cargar**: Persistencia manual
- **Backup**: Copias de seguridad
- **Importar/Exportar**: Intercambio de datos
- **Estadísticas**: Análisis de archivos

### 4. Búsqueda Avanzada
- **Por nombre**: Búsqueda exacta
- **Por teléfono**: Localización por número
- **Por email**: Búsqueda de direcciones
- **Por categoría**: Filtrado organizacional
- **Favoritos**: Vista de contactos marcados

## Validaciones Implementadas

### Datos de Entrada
- **Nombres**: No vacíos, longitud máxima
- **Teléfonos**: Formato básico, no vacíos
- **Emails**: Formato simple (opcional)
- **Fechas**: Rangos válidos (1900-2024)
- **Categorías**: Lista predefinida

### Archivos
- **Existencia**: Verificación antes de operaciones
- **Permisos**: Lectura/escritura
- **Formato**: Validación de cabeceras CSV
- **Integridad**: Control de datos corruptos

## Ejemplos de Uso

### Agregar Contacto
```
=== AGREGAR CONTACTO ===
Nombre: Juan Pérez
Teléfono: +34-123-456-789
Email (opcional): juan.perez@empresa.com
Fecha de nacimiento:
  Año (1900-2024): 1985
  Mes (1-12): 3
  Día: 15
Categorías disponibles: familia, trabajo, amigos, otros
Categoría: trabajo
Notas (opcional): Compañero de departamento
¿Marcar como favorito? (s/N): s
```

### Buscar Contacto
```
=== BUSCAR CONTACTOS ===
1. Buscar por nombre
2. Buscar por teléfono
3. Buscar por email
4. Buscar por categoría
5. Buscar favoritos
Seleccione una opción: 1
Nombre a buscar: Juan
```

### Estadísticas
```
=== ESTADÍSTICAS ===
Total de contactos: 15
  - Trabajo: 8
  - Familia: 4
  - Amigos: 2
  - Otros: 1
Favoritos: 5
Archivo actual: data/agenda.csv
Último guardado: 2024-01-15 14:30:22
```

## Manejo de Errores

### Memoria
- **Verificación de malloc**: Control de asignación
- **Liberación adecuada**: Prevención de memory leaks
- **Límites de buffer**: Protección contra overflow

### Archivos
- **Permisos**: Verificación de acceso
- **Espacio en disco**: Control de escritura
- **Corrupción**: Validación de formato

### Entrada de Usuario
- **Datos inválidos**: Solicitud de reingreso
- **Campos obligatorios**: Validación de completitud
- **Rangos**: Verificación de límites

## Características Avanzadas

### Manejo de Señales
- **SIGINT (Ctrl+C)**: Guardado de emergencia
- **SIGTERM**: Finalización controlada
- **Recuperación**: Archivo de emergencia

### Optimizaciones
- **Búsqueda eficiente**: Algoritmos optimizados
- **Memoria dinámica**: Uso eficiente de recursos
- **Carga diferida**: Lectura bajo demanda

### Extensibilidad
- **Modular**: Separación clara de responsabilidades
- **Configurable**: Parámetros ajustables
- **Escalable**: Arquitectura preparada para crecimiento

## Ejercicios Integrados

Este proyecto integra conceptos de ejercicios anteriores:

- **08-struct-union-enums**: Estructuras para contactos
- **09-estructuras-datos**: Listas enlazadas
- **07-memoria-dinamica**: Gestión dinámica de memoria
- **10-archivos-manejo-ficheros**: Persistencia CSV
- **05-vectores-cadenas-caracteres**: Manipulación de strings
- **14-seguridad-buenas-practicas**: Programación segura

## Extensiones Posibles

### Funcionalidades Adicionales
- **Grupos**: Organización jerárquica
- **Sincronización**: Backup en la nube
- **Búsqueda difusa**: Aproximación de términos
- **Historial**: Seguimiento de cambios
- **Validación avanzada**: Formatos internacionales

### Mejoras Técnicas
- **Base de datos**: SQLite para mejor rendimiento
- **GUI**: Interfaz gráfica
- **Red**: Sincronización entre dispositivos
- **Cifrado**: Protección de datos sensibles

## Conclusión

Este ejercicio demuestra el uso integral de conceptos fundamentales de C en un proyecto real y funcional. La agenda de contactos proporciona una base sólida para entender la programación estructurada, el manejo de memoria, la persistencia de datos y el diseño de interfaces de usuario en consola.

El código enfatiza las buenas prácticas de programación, incluyendo manejo robusto de errores, validación de datos, documentación completa y arquitectura modular y extensible.
