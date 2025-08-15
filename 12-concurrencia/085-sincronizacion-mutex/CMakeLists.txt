cmake_minimum_required(VERSION 3.10)
project(SincronizacionMutex C)

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
    src/sincronizacion_mutex.c
)

set(HEADERS
    include/sincronizacion_mutex.h
)

# Biblioteca estática
add_library(sincronizacion_mutex_lib STATIC ${SOURCES} ${HEADERS})
target_link_libraries(sincronizacion_mutex_lib Threads::Threads)

# Configurar propiedades de la biblioteca
set_target_properties(sincronizacion_mutex_lib PROPERTIES
    OUTPUT_NAME "sincronizacion_mutex"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Ejecutable principal
add_executable(sincronizacion_mutex src/main.c)
target_link_libraries(sincronizacion_mutex 
    sincronizacion_mutex_lib 
    Threads::Threads
)

# Configurar propiedades del ejecutable principal
set_target_properties(sincronizacion_mutex PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Añadir definiciones para depuración
target_compile_definitions(sincronizacion_mutex_lib PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MUTEX>
    $<$<CONFIG:Debug>:VERBOSE_LOGGING>
)

# Tests con Criterion (solo si está disponible)
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    enable_testing()
    
    add_executable(test_sincronizacion_mutex tests/test_sincronizacion_mutex.c)
    target_link_libraries(test_sincronizacion_mutex 
        sincronizacion_mutex_lib 
        Threads::Threads
        ${CRITERION_LIB}
    )
    target_include_directories(test_sincronizacion_mutex PRIVATE ${CRITERION_INCLUDE_DIR})
    
    # Configurar propiedades del ejecutable de tests
    set_target_properties(test_sincronizacion_mutex PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    
    # Añadir tests al sistema de testing de CMake
    add_test(NAME test_all_sincronizacion_mutex 
             COMMAND test_sincronizacion_mutex)
    
    # Tests específicos con diferentes configuraciones
    add_test(NAME test_mutex_basico 
             COMMAND test_sincronizacion_mutex --filter="sincronizacion_mutex/inicializar*")
    
    add_test(NAME test_mutex_concurrencia 
             COMMAND test_sincronizacion_mutex --filter="sincronizacion_mutex/concurrencia*")
    
    add_test(NAME test_mutex_tipos 
             COMMAND test_sincronizacion_mutex --filter="sincronizacion_mutex/*tipos*")
    
    # Configurar timeouts para tests
    set_tests_properties(test_all_sincronizacion_mutex PROPERTIES TIMEOUT 30)
    set_tests_properties(test_mutex_concurrencia PROPERTIES TIMEOUT 20)
    
    # Test de rendimiento (solo en modo Release)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_test(NAME test_rendimiento_mutex 
                 COMMAND test_sincronizacion_mutex --filter="sincronizacion_mutex/integracion_completa")
        set_tests_properties(test_rendimiento_mutex PROPERTIES TIMEOUT 45)
    endif()
    
    message(STATUS "Tests configurados correctamente con Criterion")
else()
    message(STATUS "Saltando configuración de tests (Criterion no disponible)")
endif()

# Target personalizado para ejecutar tests con salida verbose
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    add_custom_target(test_verbose
        COMMAND ${CMAKE_BINARY_DIR}/tests/test_sincronizacion_mutex --verbose
        DEPENDS test_sincronizacion_mutex
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando tests con salida detallada"
    )
    
    add_custom_target(test_xml
        COMMAND ${CMAKE_BINARY_DIR}/tests/test_sincronizacion_mutex --xml=${CMAKE_BINARY_DIR}/test_results.xml
        DEPENDS test_sincronizacion_mutex
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando tests con salida XML"
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
                ${CMAKE_BINARY_DIR}/bin/sincronizacion_mutex
            DEPENDS sincronizacion_mutex
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
                    ${CMAKE_BINARY_DIR}/tests/test_sincronizacion_mutex
                DEPENDS test_sincronizacion_mutex
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Ejecutando tests bajo Valgrind"
            )
        endif()
        
        message(STATUS "Targets de Valgrind configurados: test_memory, test_memory_tests")
    endif()
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

# Información de configuración
message(STATUS "=== Configuración de Sincronización con Mutex ===")
message(STATUS "Compilador: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de compilación: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Soporte de threads: ${CMAKE_USE_PTHREADS_INIT}")

if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    message(STATUS "Tests: Habilitados (Criterion encontrado)")
else()
    message(STATUS "Tests: Deshabilitados (Criterion no encontrado)")
endif()

# Mostrar targets disponibles
message(STATUS "")
message(STATUS "Targets disponibles:")
message(STATUS "  sincronizacion_mutex     - Compilar programa principal")
message(STATUS "  sincronizacion_mutex_lib - Compilar biblioteca")
if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
    message(STATUS "  test_sincronizacion_mutex - Compilar tests")
    message(STATUS "  test                     - Ejecutar todos los tests")
    message(STATUS "  test_verbose             - Ejecutar tests con salida detallada")
    message(STATUS "  test_xml                 - Ejecutar tests con salida XML")
endif()
if(VALGRIND_PROGRAM)
    message(STATUS "  test_memory              - Análisis de memoria del programa")
    if(CRITERION_LIB)
        message(STATUS "  test_memory_tests        - Análisis de memoria de tests")
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
    message(STATUS "Modo Debug: Símbolos de depuración y logging verbose habilitados")
    target_compile_definitions(sincronizacion_mutex PRIVATE DEBUG_MODE=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Modo Release: Optimizaciones habilitadas")
    target_compile_definitions(sincronizacion_mutex PRIVATE RELEASE_MODE=1)
endif()
