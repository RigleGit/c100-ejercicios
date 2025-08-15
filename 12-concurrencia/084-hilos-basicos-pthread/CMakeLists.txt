cmake_minimum_required(VERSION 3.10)
project(HilosBasicosPthread C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de directorios
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")

# Configuración de flags de compilación
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -g -O0")
    message(STATUS "Configuración Debug activada")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -DNDEBUG")
    message(STATUS "Configuración Release activada")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
    message(STATUS "Configuración por defecto")
endif()

# Directorio de headers
include_directories(${INCLUDE_DIR})

# ====================================================================
# BUSCAR Y CONFIGURAR PTHREAD
# ====================================================================

# Buscar pthread (requerido)
find_package(Threads REQUIRED)

# Verificar que pthread está disponible
if(NOT Threads_FOUND)
    message(FATAL_ERROR "pthread no encontrado. En algunos sistemas necesitas instalar:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libpthread-stubs0-dev")
    message(STATUS "  CentOS/RHEL: sudo yum install glibc-devel")
    message(STATUS "  macOS: pthread viene incluido")
endif()

message(STATUS "pthread encontrado: ${CMAKE_THREAD_LIBS_INIT}")

# Configurar flags específicos para pthread si es necesario
if(CMAKE_USE_PTHREADS_INIT)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
endif()

# ====================================================================
# BIBLIOTECA PRINCIPAL
# ====================================================================

# Crear biblioteca estática para hilos
add_library(hilos_basicos STATIC ${SRC_DIR}/hilos_basicos.c)

# Configurar propiedades de la biblioteca
set_target_properties(hilos_basicos PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    PUBLIC_HEADER "${INCLUDE_DIR}/hilos_basicos.h"
)

# Especificar directorio de headers para la biblioteca
target_include_directories(hilos_basicos PUBLIC ${INCLUDE_DIR})

# Enlazar pthread a la biblioteca
target_link_libraries(hilos_basicos ${CMAKE_THREAD_LIBS_INIT})

# ====================================================================
# EJECUTABLES PRINCIPALES
# ====================================================================

# Ejecutable principal interactivo
add_executable(hilos_basicos_main ${SRC_DIR}/main.c)
target_link_libraries(hilos_basicos_main hilos_basicos ${CMAKE_THREAD_LIBS_INIT})

# Alias más corto
add_executable(demo_hilos ${SRC_DIR}/main.c)
target_link_libraries(demo_hilos hilos_basicos ${CMAKE_THREAD_LIBS_INIT})

# ====================================================================
# EJEMPLO SIMPLE DEL ENUNCIADO
# ====================================================================

# Crear el archivo del ejemplo simple si no existe
set(EJEMPLO_SIMPLE_FILE "${CMAKE_SOURCE_DIR}/ejemplo_simple.c")
if(NOT EXISTS ${EJEMPLO_SIMPLE_FILE})
    file(WRITE ${EJEMPLO_SIMPLE_FILE}
"#include <stdio.h>
#include <pthread.h>

// Función que ejecutará el hilo
void* funcionDelHilo(void* arg) {
    (void)arg; // Suprimir warning
    printf(\"Hola desde el hilo hijo.\\n\");
    return NULL;
}

int main() {
    pthread_t hilo;

    // Crear el hilo
    if (pthread_create(&hilo, NULL, funcionDelHilo, NULL) != 0) {
        perror(\"Error al crear el hilo\");
        return 1;
    }

    printf(\"Hilo principal esperando al hilo hijo...\\n\");

    // Esperar a que el hilo termine
    if (pthread_join(hilo, NULL) != 0) {
        perror(\"Error al esperar el hilo\");
        return 1;
    }

    printf(\"Hilo hijo ha terminado. Fin del programa.\\n\");

    return 0;
}
")
endif()

# Ejecutable del ejemplo simple
add_executable(ejemplo_simple ${EJEMPLO_SIMPLE_FILE})
target_link_libraries(ejemplo_simple ${CMAKE_THREAD_LIBS_INIT})

# ====================================================================
# EJEMPLOS EDUCATIVOS ADICIONALES
# ====================================================================

# Ejemplo de múltiples hilos
add_executable(ejemplo_multiples_hilos
    ${CMAKE_SOURCE_DIR}/ejemplo_multiples.c
)
target_link_libraries(ejemplo_multiples_hilos ${CMAKE_THREAD_LIBS_INIT})

# Crear ejemplo de múltiples hilos si no existe
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/ejemplo_multiples.c")
    file(WRITE "${CMAKE_SOURCE_DIR}/ejemplo_multiples.c"
"#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* trabajador(void* arg) {
    int id = *(int*)arg;
    printf(\"Trabajador %d iniciado\\n\", id);
    sleep(1 + (id % 3)); // Trabajo variable
    printf(\"Trabajador %d terminado\\n\", id);
    return NULL;
}

int main() {
    const int NUM_HILOS = 5;
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    // Crear hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i + 1;
        pthread_create(&hilos[i], NULL, trabajador, &ids[i]);
    }

    // Esperar hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf(\"Todos los trabajadores terminaron\\n\");
    return 0;
}
")
endif()

# ====================================================================
# TESTS CON CRITERION
# ====================================================================

# Buscar Criterion
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
find_library(CRITERION_LIBRARY criterion)

if(CRITERION_INCLUDE_DIR AND CRITERION_LIBRARY)
    message(STATUS "Criterion encontrado: ${CRITERION_LIBRARY}")
    
    # Crear ejecutable de tests
    add_executable(test_hilos_basicos ${TEST_DIR}/test_hilos_basicos.c)
    
    # Enlazar con biblioteca, pthread y Criterion
    target_link_libraries(test_hilos_basicos 
        hilos_basicos 
        ${CMAKE_THREAD_LIBS_INIT} 
        ${CRITERION_LIBRARY}
    )
    target_include_directories(test_hilos_basicos PRIVATE ${CRITERION_INCLUDE_DIR})
    
    # Configurar test
    enable_testing()
    add_test(NAME test_hilos_basicos COMMAND test_hilos_basicos)
    
    # Target para ejecutar tests
    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS test_hilos_basicos
        COMMENT "Ejecutando tests de hilos con Criterion"
    )
    
    # Tests específicos para diferentes aspectos
    add_custom_target(test_basicos
        COMMAND ./test_hilos_basicos --filter=hilos_basicos/test_crear_hilo_simple
        DEPENDS test_hilos_basicos
        COMMENT "Ejecutando tests básicos de creación de hilos"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    add_custom_target(test_sincronizacion
        COMMAND ./test_hilos_basicos --filter=hilos_basicos/test_sincronizacion_mutex
        DEPENDS test_hilos_basicos
        COMMENT "Ejecutando tests de sincronización"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
else()
    message(WARNING "Criterion no encontrado. Tests deshabilitados.")
    message(STATUS "Para instalar Criterion:")
    message(STATUS "  macOS: brew install criterion")
    message(STATUS "  Ubuntu: sudo apt-get install libcriterion-dev")
endif()

# ====================================================================
# TARGETS DE DEMOSTRACIÓN
# ====================================================================

# Target para ejecutar todas las demostraciones
add_custom_target(demo_completo
    COMMAND ${CMAKE_COMMAND} -E echo "=== EJECUTANDO DEMOSTRACIONES DE HILOS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "1. Ejemplo simple del enunciado:"
    COMMAND ./ejemplo_simple
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "2. Ejemplo de múltiples hilos:"
    COMMAND ./ejemplo_multiples_hilos
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "3. Demo interactivo disponible con: ./demo_hilos"
    DEPENDS ejemplo_simple ejemplo_multiples_hilos demo_hilos
    COMMENT "Ejecutando demostraciones completas de hilos"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Target para verificar soporte de pthread
add_custom_target(verificar_pthread
    COMMAND ${CMAKE_COMMAND} -E echo "=== VERIFICACIÓN DE PTHREAD ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Compilador: ${CMAKE_C_COMPILER}"
    COMMAND ${CMAKE_COMMAND} -E echo "Flags pthread: ${CMAKE_THREAD_LIBS_INIT}"
    COMMAND ${CMAKE_COMMAND} -E echo "Threads encontrado: ${Threads_FOUND}"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Probando compilación simple..."
    COMMAND echo 'int main(){return 0;}' | ${CMAKE_C_COMPILER} -pthread -x c - -o test_pthread
    COMMAND ${CMAKE_COMMAND} -E echo "✅ pthread funciona correctamente"
    COMMAND ${CMAKE_COMMAND} -E remove test_pthread
)

# ====================================================================
# ANÁLISIS ESTÁTICO Y HERRAMIENTAS
# ====================================================================

# Target para análisis con cppcheck
find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE} --enable=all --std=c11 
                --suppress=missingIncludeSystem
                --check-config
                ${SRC_DIR} ${INCLUDE_DIR}
        COMMENT "Ejecutando análisis estático con cppcheck"
    )
endif()

# Target para detección de race conditions con ThreadSanitizer
add_custom_target(tsan
    COMMAND ${CMAKE_COMMAND} -E echo "=== COMPILANDO CON THREADSANITIZER ==="
    COMMAND ${CMAKE_C_COMPILER} -fsanitize=thread -g -O1 -pthread
            -I${INCLUDE_DIR} ${SRC_DIR}/hilos_basicos.c ${SRC_DIR}/main.c 
            -o hilos_tsan
    COMMAND ${CMAKE_COMMAND} -E echo "Ejecutable creado: hilos_tsan"
    COMMAND ${CMAKE_COMMAND} -E echo "Ejecuta con: ./hilos_tsan para detectar race conditions"
    COMMENT "Creando versión con ThreadSanitizer"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ====================================================================
# INSTALACIÓN
# ====================================================================

install(TARGETS hilos_basicos hilos_basicos_main
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
)

install(TARGETS ejemplo_simple ejemplo_multiples_hilos
        RUNTIME DESTINATION bin
)

# ====================================================================
# INFORMACIÓN DE CONFIGURACIÓN
# ====================================================================

message(STATUS "=== CONFIGURACIÓN DEL PROYECTO ===")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Soporte pthread: ${Threads_FOUND}")
message(STATUS "pthread flags: ${CMAKE_THREAD_LIBS_INIT}")
message(STATUS "Criterion: ${CRITERION_LIBRARY}")

# ====================================================================
# TARGETS DE AYUDA
# ====================================================================

add_custom_target(help_hilos
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== TARGETS DISPONIBLES ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make                     - Compilar todo"
    COMMAND ${CMAKE_COMMAND} -E echo "  make demo_hilos          - Programa interactivo principal"
    COMMAND ${CMAKE_COMMAND} -E echo "  make ejemplo_simple      - Ejemplo básico del enunciado"
    COMMAND ${CMAKE_COMMAND} -E echo "  make ejemplo_multiples_hilos - Ejemplo de múltiples hilos"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== DEMOSTRACIONES ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make demo_completo       - Ejecutar todas las demostraciones"
    COMMAND ${CMAKE_COMMAND} -E echo "  make verificar_pthread   - Verificar soporte de pthread"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== TESTS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make check              - Ejecutar todos los tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  make test_basicos       - Tests básicos de creación"
    COMMAND ${CMAKE_COMMAND} -E echo "  make test_sincronizacion - Tests de sincronización"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== ANÁLISIS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make cppcheck           - Análisis estático"
    COMMAND ${CMAKE_COMMAND} -E echo "  make tsan               - Compilar con ThreadSanitizer"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== EJEMPLOS DE USO ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  ./demo_hilos            - Programa interactivo"
    COMMAND ${CMAKE_COMMAND} -E echo "  ./ejemplo_simple        - Hilo básico"
    COMMAND ${CMAKE_COMMAND} -E echo "  ./ejemplo_multiples_hilos - Múltiples hilos"
    COMMAND ${CMAKE_COMMAND} -E echo ""
)
