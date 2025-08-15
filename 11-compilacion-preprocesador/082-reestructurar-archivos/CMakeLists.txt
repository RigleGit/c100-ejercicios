cmake_minimum_required(VERSION 3.10)
project(ReestructurarArchivos C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de directorios
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")

# Configuración de flags de compilación
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -g -O0")
    add_definitions(-DDEBUG=1)
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
# BIBLIOTECA MATEMÁTICA (ESTÁTICA)
# ====================================================================

# Crear biblioteca estática mimath
add_library(mimath STATIC ${LIB_DIR}/mimath.c)

# Configurar propiedades de la biblioteca
set_target_properties(mimath PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    PUBLIC_HEADER "${INCLUDE_DIR}/mimath.h"
)

# Especificar directorio de headers para la biblioteca
target_include_directories(mimath PUBLIC ${INCLUDE_DIR})

# ====================================================================
# EJECUTABLE PRINCIPAL
# ====================================================================

# Crear ejecutable principal
add_executable(reestructurar_archivos ${SRC_DIR}/main.c)

# Enlazar biblioteca
target_link_libraries(reestructurar_archivos mimath)

# ====================================================================
# DEMOSTRACIÓN DE COMPILACIÓN MANUAL
# ====================================================================

# Target personalizado para demostrar compilación manual paso a paso
add_custom_target(compilacion_manual
    COMMENT "Demostrando compilación manual paso a paso"
    COMMAND ${CMAKE_COMMAND} -E echo "=== COMPILACIÓN MANUAL PASO A PASO ==="
    COMMAND ${CMAKE_COMMAND} -E echo "1. Compilando biblioteca mimath.c a objeto..."
    COMMAND ${CMAKE_C_COMPILER} -c ${LIB_DIR}/mimath.c -I${INCLUDE_DIR} -o mimath_manual.o
    COMMAND ${CMAKE_COMMAND} -E echo "2. Compilando main.c a objeto..."
    COMMAND ${CMAKE_C_COMPILER} -c ${SRC_DIR}/main.c -I${INCLUDE_DIR} -o main_manual.o
    COMMAND ${CMAKE_COMMAND} -E echo "3. Enlazando objetos para crear ejecutable..."
    COMMAND ${CMAKE_C_COMPILER} main_manual.o mimath_manual.o -o programa_manual
    COMMAND ${CMAKE_COMMAND} -E echo "4. Ejecutable 'programa_manual' creado exitosamente"
    COMMAND ${CMAKE_COMMAND} -E echo "5. Limpiando archivos objeto temporales..."
    COMMAND ${CMAKE_COMMAND} -E remove mimath_manual.o main_manual.o
    COMMAND ${CMAKE_COMMAND} -E echo "=== COMPILACIÓN MANUAL COMPLETADA ==="
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ====================================================================
# BIBLIOTECA DINÁMICA (OPCIONAL)
# ====================================================================

# Crear versión dinámica de la biblioteca (opcional)
if(BUILD_SHARED_LIBS)
    add_library(mimath_shared SHARED ${LIB_DIR}/mimath.c)
    target_include_directories(mimath_shared PUBLIC ${INCLUDE_DIR})
    
    set_target_properties(mimath_shared PROPERTIES
        OUTPUT_NAME mimath
        VERSION 1.0.0
        SOVERSION 1
        PUBLIC_HEADER "${INCLUDE_DIR}/mimath.h"
    )
    
    # Ejecutable usando biblioteca dinámica
    add_executable(reestructurar_archivos_shared ${SRC_DIR}/main.c)
    target_link_libraries(reestructurar_archivos_shared mimath_shared)
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
    add_executable(test_mimath ${TEST_DIR}/test_mimath.c)
    
    # Enlazar con biblioteca y Criterion
    target_link_libraries(test_mimath mimath ${CRITERION_LIBRARY})
    target_include_directories(test_mimath PRIVATE ${CRITERION_INCLUDE_DIR})
    
    # Configurar test
    enable_testing()
    add_test(NAME test_mimath COMMAND test_mimath)
    
    # Target para ejecutar tests
    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS test_mimath
        COMMENT "Ejecutando tests con Criterion"
    )
    
else()
    message(WARNING "Criterion no encontrado. Tests deshabilitados.")
    message(STATUS "Para instalar Criterion:")
    message(STATUS "  macOS: brew install criterion")
    message(STATUS "  Ubuntu: sudo apt-get install libcriterion-dev")
endif()

# ====================================================================
# ANÁLISIS ESTÁTICO Y HERRAMIENTAS
# ====================================================================

# Target para análisis con cppcheck (si está disponible)
find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE} --enable=all --std=c11 
                --suppress=missingIncludeSystem
                ${SRC_DIR} ${LIB_DIR} ${INCLUDE_DIR}
        COMMENT "Ejecutando análisis estático con cppcheck"
    )
endif()

# Target para formateo con clang-format (si está disponible)
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i 
                ${SRC_DIR}/*.c ${LIB_DIR}/*.c ${INCLUDE_DIR}/*.h ${TEST_DIR}/*.c
        COMMENT "Formateando código con clang-format"
    )
endif()

# ====================================================================
# INSTALACIÓN
# ====================================================================

# Configurar instalación de la biblioteca y headers
install(TARGETS mimath
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)

install(TARGETS reestructurar_archivos
        RUNTIME DESTINATION bin
)

# ====================================================================
# INFORMACIÓN DE CONFIGURACIÓN
# ====================================================================

# Mostrar información de configuración
message(STATUS "=== CONFIGURACIÓN DEL PROYECTO ===")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Directorio de instalación: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Criterion: ${CRITERION_LIBRARY}")

# ====================================================================
# TARGETS DE AYUDA
# ====================================================================

# Target de ayuda
add_custom_target(help_compilation
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== TARGETS DISPONIBLES ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make                     - Compilar todo"
    COMMAND ${CMAKE_COMMAND} -E echo "  make reestructurar_archivos - Compilar ejecutable principal"
    COMMAND ${CMAKE_COMMAND} -E echo "  make mimath             - Compilar solo la biblioteca"
    COMMAND ${CMAKE_COMMAND} -E echo "  make test_mimath        - Compilar tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  make check              - Ejecutar tests"
    COMMAND ${CMAKE_COMMAND} -E echo "  make compilacion_manual - Demostrar compilación manual"
    COMMAND ${CMAKE_COMMAND} -E echo "  make cppcheck           - Análisis estático"
    COMMAND ${CMAKE_COMMAND} -E echo "  make format             - Formatear código"
    COMMAND ${CMAKE_COMMAND} -E echo "  make install            - Instalar biblioteca y ejecutable"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== EJEMPLOS DE COMPILACIÓN MANUAL ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  gcc -c lib/mimath.c -Iinclude -o mimath.o"
    COMMAND ${CMAKE_COMMAND} -E echo "  gcc -c src/main.c -Iinclude -o main.o"
    COMMAND ${CMAKE_COMMAND} -E echo "  gcc main.o mimath.o -o programa"
    COMMAND ${CMAKE_COMMAND} -E echo ""
)
