cmake_minimum_required(VERSION 3.10)
project([NOMBRE_PROYECTO] VERSION 1.0.0 LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuraciones del compilador
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Directorios de include
include_directories(include)

# Encontrar bibliotecas necesarias
find_library(MATH_LIBRARY m)

# Verificar si estamos en modo debug para habilitar sanitizers
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

# Archivos fuente de la biblioteca
set(LIB_SOURCES
    src/nombre_ejercicio.c
)

# Crear biblioteca estática
add_library(nombre_ejercicio_lib STATIC ${LIB_SOURCES})
target_link_libraries(nombre_ejercicio_lib ${MATH_LIBRARY})

# Ejecutable principal
add_executable(nombre_ejercicio src/main.c)
target_link_libraries(nombre_ejercicio nombre_ejercicio_lib ${MATH_LIBRARY})

# Configuración de tests
enable_testing()

# Executable de tests
add_executable(test_nombre_ejercicio tests/test_nombre_ejercicio.c)
target_link_libraries(test_nombre_ejercicio nombre_ejercicio_lib ${MATH_LIBRARY})

# Registrar test en CTest
add_test(NAME NombreEjercicio_Tests COMMAND test_nombre_ejercicio)

# Tests con verbose
add_test(NAME NombreEjercicio_Tests_Verbose COMMAND test_nombre_ejercicio -v)

# Herramientas adicionales (si existen)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/benchmark.c")
    add_executable(benchmark tools/benchmark.c)
    target_link_libraries(benchmark nombre_ejercicio_lib ${MATH_LIBRARY})
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/demo.c")
    add_executable(demo tools/demo.c)
    target_link_libraries(demo nombre_ejercicio_lib ${MATH_LIBRARY})
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/generador_datos.c")
    add_executable(generador_datos tools/generador_datos.c)
    target_link_libraries(generador_datos nombre_ejercicio_lib ${MATH_LIBRARY})
endif()

# Configuración de instalación
install(TARGETS nombre_ejercicio DESTINATION bin)
install(FILES include/nombre_ejercicio.h DESTINATION include)

# Instalar archivos de ejemplo si existen
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ejemplos/")
    install(DIRECTORY ejemplos/ DESTINATION share/nombre_ejercicio/ejemplos
            FILES_MATCHING PATTERN "*.txt" PATTERN "*.dat")
endif()

# Configuración de packaging
set(CPACK_PACKAGE_NAME "[NOMBRE_EJERCICIO]")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "[DESCRIPCIÓN BREVE]")
set(CPACK_PACKAGE_VENDOR "C100 Ejercicios")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

include(CPack)

# Targets personalizados

# Target para limpiar completamente
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove Makefile
    COMMENT "Limpieza completa del proyecto"
)

# Target para ejecutar tests con verbose
add_custom_target(test-verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_nombre_ejercicio
    COMMENT "Ejecutar tests con salida detallada"
)

# Target para ejecutar demo si existe
if(TARGET demo)
    add_custom_target(run-demo
        COMMAND $<TARGET_FILE:demo>
        DEPENDS demo
        COMMENT "Ejecutar demostración"
    )
endif()

# Target para ejecutar benchmark si existe
if(TARGET benchmark)
    add_custom_target(run-benchmark
        COMMAND $<TARGET_FILE:benchmark>
        DEPENDS benchmark
        COMMENT "Ejecutar benchmark de rendimiento"
    )
endif()

# Análisis de memoria con valgrind (si está disponible)
find_program(VALGRIND valgrind)
if(VALGRIND)
    add_custom_target(test-memory
        COMMAND ${VALGRIND} --tool=memcheck --leak-check=full 
                --show-leak-kinds=all --track-origins=yes 
                --error-exitcode=1
                $<TARGET_FILE:test_nombre_ejercicio>
        DEPENDS test_nombre_ejercicio
        COMMENT "Análisis de memoria con Valgrind"
    )
    
    add_custom_target(test-memory-main
        COMMAND ${VALGRIND} --tool=memcheck --leak-check=full 
                --show-leak-kinds=all --track-origins=yes 
                --error-exitcode=1
                $<TARGET_FILE:nombre_ejercicio> --demo
        DEPENDS nombre_ejercicio
        COMMENT "Análisis de memoria del programa principal con Valgrind"
    )
endif()

# Análisis estático con cppcheck (si está disponible)
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(static-analysis
        COMMAND ${CPPCHECK} --enable=all --std=c99 
                --include=${CMAKE_CURRENT_SOURCE_DIR}/include
                ${CMAKE_CURRENT_SOURCE_DIR}/src/
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/
        COMMENT "Análisis estático con cppcheck"
    )
endif()

# Cobertura de código con gcov (solo en modo debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    
    if(LCOV AND GENHTML)
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} -E make_directory coverage
            COMMAND ${LCOV} --directory . --zerocounters
            COMMAND ${CMAKE_CTEST_COMMAND}
            COMMAND ${LCOV} --directory . --capture --output-file coverage/app.info
            COMMAND ${LCOV} --remove coverage/app.info '/usr/*' --output-file coverage/app.info
            COMMAND ${GENHTML} coverage/app.info --output-directory coverage
            DEPENDS test_nombre_ejercicio
            COMMENT "Generar reporte de cobertura de código"
        )
    endif()
endif()

# Formateo de código con clang-format (si está disponible)
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
         ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
         ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
         ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
         ${CMAKE_CURRENT_SOURCE_DIR}/tools/*.c)
    
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatear código con clang-format"
    )
    
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT} --dry-run --Werror ${ALL_SOURCE_FILES}
        COMMENT "Verificar formato de código"
    )
endif()

# Configuración de documentación con Doxygen (si está disponible)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    # Verificar si existe archivo de configuración Doxygen
    if(EXISTS ${DOXYGEN_IN})
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        
        add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generar documentación API con Doxygen"
            VERBATIM
        )
    else()
        # Crear configuración básica de Doxygen
        add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} -g
            COMMAND ${DOXYGEN_EXECUTABLE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generar documentación API con Doxygen (configuración por defecto)"
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

if(VALGRIND)
    message(STATUS "Valgrind encontrado: ${VALGRIND}")
endif()

if(CPPCHECK)
    message(STATUS "Cppcheck encontrado: ${CPPCHECK}")
endif()

if(CLANG_FORMAT)
    message(STATUS "Clang-format encontrado: ${CLANG_FORMAT}")
endif()

if(DOXYGEN_FOUND)
    message(STATUS "Doxygen encontrado: ${DOXYGEN_EXECUTABLE}")
endif()

message(STATUS "========================================")
