cmake_minimum_required(VERSION 3.10)
project(AssertValidacion C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de directorios
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")

# Configuración de flags de compilación según el tipo de build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -g -O0")
    # En Debug, assert está ACTIVO (no definir NDEBUG)
    message(STATUS "Configuración Debug: assert ACTIVO")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -DNDEBUG")
    # En Release, assert está DESACTIVADO (definir NDEBUG)
    message(STATUS "Configuración Release: assert DESACTIVADO")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
    message(STATUS "Configuración por defecto: assert ACTIVO")
endif()

# Opción para forzar activación/desactivación de assert
option(FORCE_ASSERT_ON "Forzar activación de assert (ignorar NDEBUG)" OFF)
option(FORCE_ASSERT_OFF "Forzar desactivación de assert (definir NDEBUG)" OFF)

if(FORCE_ASSERT_ON AND FORCE_ASSERT_OFF)
    message(FATAL_ERROR "No se pueden activar ambas opciones FORCE_ASSERT_ON y FORCE_ASSERT_OFF")
endif()

if(FORCE_ASSERT_ON)
    # Remover NDEBUG si estaba definido
    string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    message(STATUS "Assert FORZADO ACTIVO")
elseif(FORCE_ASSERT_OFF)
    # Asegurar que NDEBUG esté definido
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG")
    message(STATUS "Assert FORZADO DESACTIVADO")
endif()

# Directorio de headers
include_directories(${INCLUDE_DIR})

# ====================================================================
# BIBLIOTECA PRINCIPAL
# ====================================================================

# Crear biblioteca estática
add_library(assert_validacion STATIC ${SRC_DIR}/assert_validacion.c)

# Configurar propiedades de la biblioteca
set_target_properties(assert_validacion PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    PUBLIC_HEADER "${INCLUDE_DIR}/assert_validacion.h"
)

# Especificar directorio de headers para la biblioteca
target_include_directories(assert_validacion PUBLIC ${INCLUDE_DIR})

# Enlazar biblioteca matemática
target_link_libraries(assert_validacion m)

# ====================================================================
# EJECUTABLES PRINCIPALES
# ====================================================================

# Ejecutable principal interactivo
add_executable(assert_validacion_main ${SRC_DIR}/main.c)
target_link_libraries(assert_validacion_main assert_validacion m)

# Alias más corto
add_executable(demo_assert ${SRC_DIR}/main.c)
target_link_libraries(demo_assert assert_validacion m)

# ====================================================================
# EJECUTABLES PARA DEMOSTRACIÓN ESPECÍFICA
# ====================================================================

# Versión simple del ejemplo original
add_executable(ejemplo_simple
    ${CMAKE_SOURCE_DIR}/ejemplo_simple.c
)
target_link_libraries(ejemplo_simple m)

# Crear el archivo del ejemplo simple si no existe
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/ejemplo_simple.c")
    file(WRITE "${CMAKE_SOURCE_DIR}/ejemplo_simple.c" 
"#include <stdio.h>
#include <math.h>
#include <assert.h>

double raizCuadrada(double x) {
    assert(x >= 0 && \"El argumento no puede ser negativo\");
    return sqrt(x);
}

int main() {
    double valor1 = 25.0;
    double valor2 = -9.0;

    printf(\"Raíz de %.2f: %.2f\\n\", valor1, raizCuadrada(valor1));

    // Esta llamada provocará la interrupción del programa si las aserciones están activadas
    printf(\"Raíz de %.2f: %.2f\\n\", valor2, raizCuadrada(valor2));

    return 0;
}
")
endif()

# ====================================================================
# TARGETS PARA DIFERENTES CONFIGURACIONES DE ASSERT
# ====================================================================

# Target para demostrar assert activo
add_custom_target(demo_assert_activo
    COMMAND ${CMAKE_COMMAND} -E echo "=== COMPILANDO CON ASSERT ACTIVO ==="
    COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -I${INCLUDE_DIR} 
            ${SRC_DIR}/assert_validacion.c ${SRC_DIR}/main.c -lm -o demo_activo
    COMMAND ${CMAKE_COMMAND} -E echo "Ejecutable creado: demo_activo"
    COMMAND ${CMAKE_COMMAND} -E echo "Assert está ACTIVO - las validaciones se ejecutarán"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Target para demostrar assert desactivado
add_custom_target(demo_assert_desactivado
    COMMAND ${CMAKE_COMMAND} -E echo "=== COMPILANDO CON ASSERT DESACTIVADO ==="
    COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -DNDEBUG -I${INCLUDE_DIR} 
            ${SRC_DIR}/assert_validacion.c ${SRC_DIR}/main.c -lm -o demo_desactivado
    COMMAND ${CMAKE_COMMAND} -E echo "Ejecutable creado: demo_desactivado"
    COMMAND ${CMAKE_COMMAND} -E echo "Assert está DESACTIVADO - las validaciones se ignorarán"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Target para comparar ambas versiones
add_custom_target(comparar_assert
    COMMAND ${CMAKE_COMMAND} -E echo "=== GENERANDO AMBAS VERSIONES ==="
    COMMAND make demo_assert_activo
    COMMAND make demo_assert_desactivado
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Versiones generadas:"
    COMMAND ${CMAKE_COMMAND} -E echo "  ./demo_activo    - Assert ACTIVO"
    COMMAND ${CMAKE_COMMAND} -E echo "  ./demo_desactivado - Assert DESACTIVADO"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Prueba ejecutar ambas para ver la diferencia"
    DEPENDS demo_assert_activo demo_assert_desactivado
)

# ====================================================================
# TESTS CON CRITERION
# ====================================================================

# Buscar Criterion
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
find_library(CRITERION_LIBRARY criterion)

if(CRITERION_INCLUDE_DIR AND CRITERION_LIBRARY)
    message(STATUS "Criterion encontrado: ${CRITERION_LIBRARY}")
    
    # Tests con assert activo
    add_executable(test_assert_activo ${TEST_DIR}/test_assert_validacion.c)
    target_link_libraries(test_assert_activo assert_validacion ${CRITERION_LIBRARY} m)
    target_include_directories(test_assert_activo PRIVATE ${CRITERION_INCLUDE_DIR})
    
    # Tests con assert desactivado (forzar NDEBUG). Compilar unidad bajo test con NDEBUG también.
    add_executable(test_assert_desactivado 
        ${SRC_DIR}/assert_validacion.c
        ${TEST_DIR}/test_assert_validacion.c)
    target_link_libraries(test_assert_desactivado ${CRITERION_LIBRARY} m)
    target_include_directories(test_assert_desactivado PRIVATE ${CRITERION_INCLUDE_DIR} ${INCLUDE_DIR})
    target_compile_definitions(test_assert_desactivado PRIVATE NDEBUG)
    
    # Configurar tests
    enable_testing()
    add_test(NAME test_assert_activo COMMAND test_assert_activo)
    add_test(NAME test_assert_desactivado COMMAND test_assert_desactivado)
    
    # Target para ejecutar ambos tipos de tests
    add_custom_target(check_todos
        COMMAND ${CMAKE_COMMAND} -E echo "=== EJECUTANDO TESTS CON ASSERT ACTIVO ==="
        COMMAND ./test_assert_activo
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${CMAKE_COMMAND} -E echo "=== EJECUTANDO TESTS CON ASSERT DESACTIVADO ==="
        COMMAND ./test_assert_desactivado
        DEPENDS test_assert_activo test_assert_desactivado
        COMMENT "Ejecutando tests con ambas configuraciones de assert"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    # Target estándar de tests
    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS test_assert_activo test_assert_desactivado
        COMMENT "Ejecutando todos los tests"
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

# Target para análisis con cppcheck
find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE} --enable=all --std=c11 
                --suppress=missingIncludeSystem
                ${SRC_DIR} ${INCLUDE_DIR}
        COMMENT "Ejecutando análisis estático con cppcheck"
    )
endif()

# Target para verificar que assert funciona correctamente
add_custom_target(verificar_assert
    COMMAND ${CMAKE_COMMAND} -E echo "=== VERIFICACIÓN DE ASSERT ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Estado actual de NDEBUG:"
    COMMAND echo '${CMAKE_C_FLAGS}' | grep -q NDEBUG && echo "  NDEBUG definido (assert DESACTIVADO)" || echo "  NDEBUG no definido (assert ACTIVO)"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Para cambiar:"
    COMMAND ${CMAKE_COMMAND} -E echo "  Assert activo:    cmake -DCMAKE_BUILD_TYPE=Debug .."
    COMMAND ${CMAKE_COMMAND} -E echo "  Assert desactivado: cmake -DCMAKE_BUILD_TYPE=Release .."
    COMMAND ${CMAKE_COMMAND} -E echo "  Forzar activo:    cmake -DFORCE_ASSERT_ON=ON .."
    COMMAND ${CMAKE_COMMAND} -E echo "  Forzar desactivado: cmake -DFORCE_ASSERT_OFF=ON .."
)

# ====================================================================
# INSTALACIÓN
# ====================================================================

install(TARGETS assert_validacion assert_validacion_main
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
)

# ====================================================================
# INFORMACIÓN DE CONFIGURACIÓN
# ====================================================================

message(STATUS "=== CONFIGURACIÓN DEL PROYECTO ===")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")

# Verificar estado de NDEBUG
string(FIND "${CMAKE_C_FLAGS}" "NDEBUG" NDEBUG_POS)
if(NDEBUG_POS GREATER -1)
    message(STATUS "Estado de assert: DESACTIVADO (-DNDEBUG presente)")
else()
    message(STATUS "Estado de assert: ACTIVO (-DNDEBUG ausente)")
endif()

message(STATUS "Criterion: ${CRITERION_LIBRARY}")

# ====================================================================
# TARGETS DE AYUDA
# ====================================================================

add_custom_target(help_assert
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== TARGETS DISPONIBLES ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make                      - Compilar todo"
    COMMAND ${CMAKE_COMMAND} -E echo "  make demo_assert          - Ejecutable principal interactivo"
    COMMAND ${CMAKE_COMMAND} -E echo "  make ejemplo_simple       - Ejemplo básico del enunciado"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== DEMOSTRACIONES DE ASSERT ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make demo_assert_activo   - Compilar con assert ACTIVO"
    COMMAND ${CMAKE_COMMAND} -E echo "  make demo_assert_desactivado - Compilar con assert DESACTIVADO"
    COMMAND ${CMAKE_COMMAND} -E echo "  make comparar_assert      - Generar ambas versiones"
    COMMAND ${CMAKE_COMMAND} -E echo "  make verificar_assert     - Mostrar estado actual de assert"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== TESTS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  make check               - Ejecutar tests estándar"
    COMMAND ${CMAKE_COMMAND} -E echo "  make check_todos         - Ejecutar tests con ambas configuraciones"
    COMMAND ${CMAKE_COMMAND} -E echo "  make test_assert_activo  - Tests solo con assert activo"
    COMMAND ${CMAKE_COMMAND} -E echo "  make test_assert_desactivado - Tests solo con assert desactivado"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== CONFIGURACIONES ==="
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake -DCMAKE_BUILD_TYPE=Debug ..    - Assert ACTIVO"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake -DCMAKE_BUILD_TYPE=Release ..  - Assert DESACTIVADO"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake -DFORCE_ASSERT_ON=ON ..        - Forzar assert activo"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake -DFORCE_ASSERT_OFF=ON ..       - Forzar assert desactivado"
    COMMAND ${CMAKE_COMMAND} -E echo ""
)
