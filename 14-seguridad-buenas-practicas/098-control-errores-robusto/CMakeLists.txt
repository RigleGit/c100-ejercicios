cmake_minimum_required(VERSION 3.15)
project(control_errores_robusto C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Directorios
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")
set(TOOLS_DIR "${CMAKE_SOURCE_DIR}/tools")

# Incluir directorios de cabeceras
include_directories(${INCLUDE_DIR})

# Biblioteca estática
add_library(control_errores_robusto_lib STATIC
    ${SRC_DIR}/control_errores_robusto.c
)

target_include_directories(control_errores_robusto_lib PUBLIC ${INCLUDE_DIR})

# Ejecutable principal
add_executable(control_errores_robusto
    ${SRC_DIR}/main.c
)

target_link_libraries(control_errores_robusto control_errores_robusto_lib)

# Configuración de pruebas con Criterion
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

find_library(CRITERION_LIB criterion)
if(CRITERION_FOUND OR CRITERION_LIB)
    enable_testing()
    
    add_executable(test_control_errores_robusto
        ${TEST_DIR}/test_control_errores_robusto.c
    )
    
    target_link_libraries(test_control_errores_robusto 
        control_errores_robusto_lib 
        ${CRITERION_LIB}
        ${CRITERION_LIBRARIES}
    )
    if (CRITERION_INCLUDE_DIRS)
        target_include_directories(test_control_errores_robusto PRIVATE ${CRITERION_INCLUDE_DIRS})
    endif()
    if (CRITERION_LIBRARY_DIRS)
        target_link_directories(test_control_errores_robusto PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_definitions(test_control_errores_robusto PRIVATE UNIT_TESTING)
    endif()
    if (CRITERION_LDFLAGS)
        target_link_options(test_control_errores_robusto PRIVATE ${CRITERION_LDFLAGS})
    endif()
    
    # Agregar las pruebas
    add_test(NAME test_control_errores_robusto 
             COMMAND test_control_errores_robusto)
    
    # Pruebas individuales por suite
    add_test(NAME test_result_utils 
             COMMAND test_control_errores_robusto --filter=result_utils)
    add_test(NAME test_archivo_seguro 
             COMMAND test_control_errores_robusto --filter=archivo_seguro)
    add_test(NAME test_memoria_segura 
             COMMAND test_control_errores_robusto --filter=memoria_segura)
    add_test(NAME test_entrada_segura 
             COMMAND test_control_errores_robusto --filter=entrada_segura)
    add_test(NAME test_funcion_insegura 
             COMMAND test_control_errores_robusto --filter=funcion_insegura)
    add_test(NAME test_funcion_segura 
             COMMAND test_control_errores_robusto --filter=funcion_segura)
    add_test(NAME test_error_handling 
             COMMAND test_control_errores_robusto --filter=error_handling)
    add_test(NAME test_integracion 
             COMMAND test_control_errores_robusto --filter=integracion)
    
    message(STATUS "Criterion encontrado: ${CRITERION_LIB}${CRITERION_LIBRARIES}")
    message(STATUS "Tests habilitados")
else()
    message(WARNING "Criterion no encontrado. Tests deshabilitados.")
    message(STATUS "Para instalar Criterion:")
    message(STATUS "  macOS: brew install criterion")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libcriterion-dev")
    message(STATUS "  Fedora: sudo dnf install criterion-devel")
endif()

# Herramientas de demostración
add_executable(demo_comparacion
    ${TOOLS_DIR}/demo_comparacion.c
)
target_link_libraries(demo_comparacion control_errores_robusto_lib)

add_executable(error_simulator
    ${TOOLS_DIR}/error_simulator.c
)
target_link_libraries(error_simulator control_errores_robusto_lib)

# Configuración de instalación
install(TARGETS control_errores_robusto DESTINATION bin)
install(FILES ${INCLUDE_DIR}/control_errores_robusto.h DESTINATION include)
install(TARGETS control_errores_robusto_lib DESTINATION lib)

# Linter (si está disponible)
find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)
    add_custom_target(lint
        COMMAND ${CPPCHECK_EXECUTABLE}
        --enable=all
        --std=c99
        --verbose
        --quiet
        --error-exitcode=1
        ${SRC_DIR}
        COMMENT "Ejecutando cppcheck"
    )
endif()

# Formateo de código (si está disponible)
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
         ${SRC_DIR}/*.c ${SRC_DIR}/*.h 
         ${INCLUDE_DIR}/*.h 
         ${TEST_DIR}/*.c)
    
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE}
        -style="{BasedOnStyle: Linux, IndentWidth: 4, TabWidth: 4, UseTab: Never}"
        -i ${ALL_SOURCE_FILES}
        COMMENT "Formateando código con clang-format"
    )
endif()

# Target de limpieza personalizada
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/Testing
    COMMENT "Limpieza completa del proyecto"
)

# Información de compilación
message(STATUS "=== Configuración del Proyecto ===")
message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de compilación: ${CMAKE_BUILD_TYPE}")
message(STATUS "Directorio fuente: ${CMAKE_SOURCE_DIR}")
message(STATUS "Directorio binario: ${CMAKE_BINARY_DIR}")
message(STATUS "================================")

# Targets disponibles
add_custom_target(help-targets
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Targets disponibles:"
    COMMAND ${CMAKE_COMMAND} -E echo "  control_errores_robusto - Programa principal"
    COMMAND ${CMAKE_COMMAND} -E echo "  test_control_errores_robusto - Ejecutar pruebas"
    COMMAND ${CMAKE_COMMAND} -E echo "  demo_comparacion - Demo de comparación seguro vs inseguro"
    COMMAND ${CMAKE_COMMAND} -E echo "  error_simulator - Simulador de errores"
    COMMAND ${CMAKE_COMMAND} -E echo "  lint - Ejecutar cppcheck (si disponible)"
    COMMAND ${CMAKE_COMMAND} -E echo "  format - Formatear código (si disponible)"
    COMMAND ${CMAKE_COMMAND} -E echo "  clean-all - Limpieza completa"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMENT "Mostrando ayuda de targets"
)
