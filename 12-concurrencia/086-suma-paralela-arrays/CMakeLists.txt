cmake_minimum_required(VERSION 3.10)
project(SumaParalelaArrays C)

# Configuración del compilador
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Opciones de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Detectar el sistema operativo para configuraciones específicas
if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DARWIN_C_SOURCE")
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE")
endif()

# Directorios de inclusión
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Buscar bibliotecas necesarias
find_package(Threads REQUIRED)

# Buscar Criterion para tests
find_library(CRITERION_LIB criterion)
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
if(NOT CRITERION_LIB OR NOT CRITERION_INCLUDE_DIR)
    message(WARNING "Criterion no encontrado. Los tests no estarán disponibles.")
    message(STATUS "Para instalar Criterion:")
    message(STATUS "  macOS: brew install criterion")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libcriterion-dev")
    message(STATUS "  Fedora: sudo dnf install criterion-devel")
endif()

# Archivos fuente
set(SOURCES
    src/suma_paralela_arrays.c
)

set(HEADERS
    include/suma_paralela_arrays.h
)

# Biblioteca estática
add_library(suma_paralela_arrays_lib STATIC ${SOURCES} ${HEADERS})
target_link_libraries(suma_paralela_arrays_lib Threads::Threads)

# Configurar propiedades de la biblioteca
set_target_properties(suma_paralela_arrays_lib PROPERTIES
    OUTPUT_NAME "suma_paralela_arrays"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Añadir definiciones matemáticas
target_compile_definitions(suma_paralela_arrays_lib PRIVATE
    _GNU_SOURCE
    $<$<CONFIG:Debug>:DEBUG_SUMA_PARALELA>
    $<$<CONFIG:Debug>:VERBOSE_TIMING>
)

# Ejecutable principal
add_executable(suma_paralela_arrays src/main.c)
target_link_libraries(suma_paralela_arrays 
    suma_paralela_arrays_lib 
    Threads::Threads
    m  # Biblioteca matemática
)

# Configurar propiedades del ejecutable principal
set_target_properties(suma_paralela_arrays PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Tests con Criterion (solo si está disponible)
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    enable_testing()
    
    add_executable(test_suma_paralela_arrays tests/test_suma_paralela_arrays.c)
    target_link_libraries(test_suma_paralela_arrays 
        suma_paralela_arrays_lib 
        Threads::Threads
        m  # Biblioteca matemática
        ${CRITERION_LIB}
    )
    target_include_directories(test_suma_paralela_arrays PRIVATE ${CRITERION_INCLUDE_DIR})
    
    # Configurar propiedades del ejecutable de tests
    set_target_properties(test_suma_paralela_arrays PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    
    # Añadir tests al sistema de testing de CMake
    add_test(NAME test_all_suma_paralela_arrays 
             COMMAND test_suma_paralela_arrays)
    
    # Tests específicos por categorías
    add_test(NAME test_suma_basica 
             COMMAND test_suma_paralela_arrays --filter="suma_paralela_arrays/suma_*")
    
    add_test(NAME test_creacion_arrays 
             COMMAND test_suma_paralela_arrays --filter="suma_paralela_arrays/crear_*")
    
    add_test(NAME test_concurrencia 
             COMMAND test_suma_paralela_arrays --filter="suma_paralela_arrays/*concurrencia*")
    
    add_test(NAME test_benchmark 
             COMMAND test_suma_paralela_arrays --filter="suma_paralela_arrays/benchmark*")
    
    add_test(NAME test_utilidades 
             COMMAND test_suma_paralela_arrays --filter="suma_paralela_arrays/*tiempo* OR suma_paralela_arrays/*dividir* OR suma_paralela_arrays/*calcular*")
    
    # Configurar timeouts para tests
    set_tests_properties(test_all_suma_paralela_arrays PROPERTIES TIMEOUT 45)
    set_tests_properties(test_concurrencia PROPERTIES TIMEOUT 30)
    set_tests_properties(test_benchmark PROPERTIES TIMEOUT 25)
    
    # Test de rendimiento (solo en modo Release)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_test(NAME test_rendimiento_suma_paralela 
                 COMMAND test_suma_paralela_arrays --filter="suma_paralela_arrays/*rendimiento* OR suma_paralela_arrays/*escalabilidad*")
        set_tests_properties(test_rendimiento_suma_paralela PROPERTIES TIMEOUT 60)
    endif()
    
    message(STATUS "Tests configurados correctamente con Criterion")
else()
    message(STATUS "Saltando configuración de tests (Criterion no disponible)")
endif()

# Target personalizado para ejecutar tests con salida verbose
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    add_custom_target(test_verbose
        COMMAND ${CMAKE_BINARY_DIR}/tests/test_suma_paralela_arrays --verbose
        DEPENDS test_suma_paralela_arrays
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando tests con salida detallada"
    )
    
    add_custom_target(test_xml
        COMMAND ${CMAKE_BINARY_DIR}/tests/test_suma_paralela_arrays --xml=${CMAKE_BINARY_DIR}/test_results.xml
        DEPENDS test_suma_paralela_arrays
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando tests con salida XML"
    )
    
    add_custom_target(test_performance
        COMMAND ${CMAKE_BINARY_DIR}/tests/test_suma_paralela_arrays --filter="*rendimiento*" --verbose
        DEPENDS test_suma_paralela_arrays
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando solo tests de rendimiento"
    )
endif()

# Target para análisis de memoria con Valgrind (solo en sistemas Unix)
if(UNIX AND NOT APPLE)
    find_program(VALGRIND_PROGRAM valgrind)
    if(VALGRIND_PROGRAM)
        add_custom_target(test_memory
            COMMAND ${VALGRIND_PROGRAM} 
                --tool=memcheck 
                --leak-check=full 
                --show-leak-kinds=all 
                --track-origins=yes 
                --verbose 
                --error-exitcode=1
                ${CMAKE_BINARY_DIR}/bin/suma_paralela_arrays
            DEPENDS suma_paralela_arrays
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Ejecutando análisis de memoria con Valgrind"
        )
        
        if(CRITERION_LIB)
            add_custom_target(test_memory_tests
                COMMAND ${VALGRIND_PROGRAM} 
                    --tool=memcheck 
                    --leak-check=full 
                    --show-leak-kinds=all 
                    --track-origins=yes 
                    --error-exitcode=1
                    ${CMAKE_BINARY_DIR}/tests/test_suma_paralela_arrays
                DEPENDS test_suma_paralela_arrays
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Ejecutando tests bajo Valgrind"
            )
            
            # Target especial para detectar race conditions
            add_custom_target(test_helgrind
                COMMAND ${VALGRIND_PROGRAM} 
                    --tool=helgrind 
                    --verbose 
                    --error-exitcode=1
                    ${CMAKE_BINARY_DIR}/tests/test_suma_paralela_arrays --filter="*concurrencia*"
                DEPENDS test_suma_paralela_arrays
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Ejecutando detección de race conditions con Helgrind"
            )
        endif()
        
        message(STATUS "Targets de Valgrind configurados: test_memory, test_memory_tests, test_helgrind")
    endif()
endif()

# Target para análisis con Thread Sanitizer
if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    add_custom_target(build_with_tsan
        COMMAND ${CMAKE_COMMAND} -DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -fsanitize=thread -g" ..
        COMMAND make suma_paralela_arrays
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Compilando con Thread Sanitizer"
    )
    
    if(CRITERION_LIB)
        add_custom_target(test_with_tsan
            COMMAND ${CMAKE_COMMAND} -DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -fsanitize=thread -g" ..
            COMMAND make test_suma_paralela_arrays
            COMMAND ./tests/test_suma_paralela_arrays --filter="*concurrencia*"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Ejecutando tests con Thread Sanitizer"
        )
    endif()
    
    message(STATUS "Targets de Thread Sanitizer configurados: build_with_tsan, test_with_tsan")
endif()

# Target para análisis estático con cppcheck
find_program(CPPCHECK_PROGRAM cppcheck)
if(CPPCHECK_PROGRAM)
    add_custom_target(static_analysis
        COMMAND ${CPPCHECK_PROGRAM}
            --enable=all
            --std=c99
            --verbose
            --error-exitcode=1
            --suppress=missingIncludeSystem
            --inline-suppr
            --check-config
            ${CMAKE_CURRENT_SOURCE_DIR}/src/
            ${CMAKE_CURRENT_SOURCE_DIR}/include/
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando análisis estático con cppcheck"
    )
    message(STATUS "Target de análisis estático configurado: static_analysis")
endif()

# Target para formateo de código con clang-format
find_program(CLANG_FORMAT_PROGRAM clang-format)
if(CLANG_FORMAT_PROGRAM)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_PROGRAM}
            -i
            --style="{BasedOnStyle: GNU, IndentWidth: 4, TabWidth: 4, UseTab: Never}"
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Formateando código fuente"
    )
    message(STATUS "Target de formateo configurado: format")
endif()

# Target para benchmark personalizado
add_custom_target(benchmark
    COMMAND ${CMAKE_BINARY_DIR}/bin/suma_paralela_arrays
    DEPENDS suma_paralela_arrays
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Ejecutando programa de benchmark interactivo"
)

# Target para pruebas de escalabilidad automáticas
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    add_custom_target(escalabilidad
        COMMAND ${CMAKE_BINARY_DIR}/tests/test_suma_paralela_arrays --filter="*escalabilidad*" --verbose
        DEPENDS test_suma_paralela_arrays
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando pruebas de escalabilidad"
    )
endif()

# Información de configuración
message(STATUS "=== Configuración de Suma Paralela de Arrays ===")
message(STATUS "Compilador: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Soporte de threads: ${CMAKE_USE_PTHREADS_INIT}")

# Detectar número de CPUs
if(UNIX)
    execute_process(
        COMMAND nproc
        OUTPUT_VARIABLE NUM_CPUS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(NUM_CPUS)
        message(STATUS "CPUs detectadas: ${NUM_CPUS}")
    endif()
endif()

if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    message(STATUS "Tests: Habilitados (Criterion encontrado)")
else()
    message(STATUS "Tests: Deshabilitados (Criterion no encontrado)")
endif()

# Mostrar targets disponibles
message(STATUS "")
message(STATUS "Targets disponibles:")
message(STATUS "  suma_paralela_arrays     - Compilar programa principal")
message(STATUS "  suma_paralela_arrays_lib - Compilar biblioteca")
message(STATUS "  benchmark                - Ejecutar programa interactivo")
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    message(STATUS "  test_suma_paralela_arrays - Compilar tests")
    message(STATUS "  test                     - Ejecutar todos los tests")
    message(STATUS "  test_verbose             - Ejecutar tests con salida detallada")
    message(STATUS "  test_xml                 - Ejecutar tests con salida XML")
    message(STATUS "  test_performance         - Ejecutar solo tests de rendimiento")
    message(STATUS "  escalabilidad            - Ejecutar pruebas de escalabilidad")
endif()
if(VALGRIND_PROGRAM)
    message(STATUS "  test_memory              - Análisis de memoria del programa")
    if(CRITERION_LIB)
        message(STATUS "  test_memory_tests        - Análisis de memoria de tests")
        message(STATUS "  test_helgrind            - Detección de race conditions")
    endif()
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    message(STATUS "  build_with_tsan          - Compilar con Thread Sanitizer")
    if(CRITERION_LIB)
        message(STATUS "  test_with_tsan           - Tests con Thread Sanitizer")
    endif()
endif()
if(CPPCHECK_PROGRAM)
    message(STATUS "  static_analysis          - Análisis estático del código")
endif()
if(CLANG_FORMAT_PROGRAM)
    message(STATUS "  format                   - Formatear código fuente")
endif()
message(STATUS "")

# Configuración específica para diferentes tipos de build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Modo Debug: Símbolos de depuración y timing verbose habilitados")
    target_compile_definitions(suma_paralela_arrays PRIVATE DEBUG_MODE=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Modo Release: Optimizaciones habilitadas")
    target_compile_definitions(suma_paralela_arrays PRIVATE RELEASE_MODE=1)
    # Habilitar optimizaciones adicionales para arrays grandes
    target_compile_definitions(suma_paralela_arrays_lib PRIVATE OPTIMIZE_LARGE_ARRAYS=1)
endif()

# Warnings adicionales para desarrollo
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
        target_compile_options(suma_paralela_arrays_lib PRIVATE
            -Wconversion
            -Wsign-conversion
            -Wcast-align
            -Wformat-security
            -Wnull-dereference
            -Wdouble-promotion
        )
    endif()
endif()
