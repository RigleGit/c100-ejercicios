cmake_minimum_required(VERSION 3.10)
project(EvaluadorRPN VERSION 1.0.0 LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuraciones del compilador
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Directorios de include
include_directories(include)

# Encontrar bibliotecas matemáticas
find_library(MATH_LIBRARY m)

# Biblioteca principal
set(EVALUADOR_SOURCES
    src/evaluador_rpn.c
    src/funciones_adicionales.c
)

add_library(evaluador_rpn_lib STATIC ${EVALUADOR_SOURCES})
target_link_libraries(evaluador_rpn_lib ${MATH_LIBRARY})

# Ejecutable principal
add_executable(evaluador_rpn src/main.c)
target_link_libraries(evaluador_rpn evaluador_rpn_lib ${MATH_LIBRARY})

# Configuración de tests
enable_testing()

# Tests unitarios
add_executable(test_evaluador_rpn tests/test_evaluador_rpn.c)
target_link_libraries(test_evaluador_rpn evaluador_rpn_lib ${MATH_LIBRARY})

# Registrar test en CTest
add_test(NAME EvaluadorRPN_Tests COMMAND test_evaluador_rpn)

# Herramientas adicionales
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/benchmark.c")
    add_executable(benchmark tools/benchmark.c)
    target_link_libraries(benchmark evaluador_rpn_lib ${MATH_LIBRARY})
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/generador_tests.c")
    add_executable(generador_tests tools/generador_tests.c)
    target_link_libraries(generador_tests evaluador_rpn_lib ${MATH_LIBRARY})
endif()

# Configuración de instalación
install(TARGETS evaluador_rpn DESTINATION bin)
install(FILES include/evaluador_rpn.h DESTINATION include)
install(DIRECTORY ejemplos/ DESTINATION share/evaluador_rpn/ejemplos)

# Configuración de packaging
set(CPACK_PACKAGE_NAME "EvaluadorRPN")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Evaluador de Notación Polaca Reversa")
set(CPACK_PACKAGE_VENDOR "C100 Ejercicios")

include(CPack)

# Target personalizado para limpiar completamente
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove Makefile
)

# Target para ejecutar tests con verbose
add_custom_target(test-verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_evaluador_rpn
)

# Target para análisis de memoria con valgrind (si está disponible)
find_program(VALGRIND valgrind)
if(VALGRIND)
    add_custom_target(test-memory
        COMMAND ${VALGRIND} --tool=memcheck --leak-check=full 
                --show-leak-kinds=all --track-origins=yes 
                $<TARGET_FILE:test_evaluador_rpn>
        DEPENDS test_evaluador_rpn
    )
    
    add_custom_target(test-memory-main
        COMMAND ${VALGRIND} --tool=memcheck --leak-check=full 
                --show-leak-kinds=all --track-origins=yes 
                $<TARGET_FILE:evaluador_rpn> examples/expresiones_basicas.txt
        DEPENDS evaluador_rpn
    )
endif()

# Configuración de documentación (si Doxygen está disponible)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    if(EXISTS ${DOXYGEN_IN})
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generando documentación API con Doxygen"
            VERBATIM
        )
    endif()
endif()

# Información de build
message(STATUS "========================================")
message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Directorio de instalación: ${CMAKE_INSTALL_PREFIX}")
if(MATH_LIBRARY)
    message(STATUS "Biblioteca matemática: ${MATH_LIBRARY}")
else()
    message(STATUS "Biblioteca matemática: incorporada")
endif()
message(STATUS "========================================")
