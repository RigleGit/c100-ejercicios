# CMakeLists.txt para Ejercicio 091: Comunicación UDP
# Configuración avanzada con soporte para tests, herramientas y documentación

cmake_minimum_required(VERSION 3.10)
project(ComunicacionUDP
    VERSION 1.0.0
    DESCRIPTION "Ejercicio 091: Comunicación UDP completa con emisor y receptor"
    LANGUAGES C
)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Configuración de build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

# Opciones de configuración
option(BUILD_TESTS "Construir tests unitarios con Criterion" ON)
option(BUILD_TOOLS "Construir herramientas auxiliares" OFF)
option(BUILD_DOCS "Generar documentación con Doxygen" OFF)
option(ENABLE_SANITIZERS "Habilitar Address y UB Sanitizers en Debug" ON)
option(ENABLE_COVERAGE "Habilitar cobertura de código" OFF)
option(ENABLE_STATIC_ANALYSIS "Habilitar análisis estático" OFF)

# Configuración de directorios
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tools")

# Incluir directorios
include_directories(${INCLUDE_DIR})

# Flags de compilación comunes
set(COMMON_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wformat=2
    -Wno-unused-parameter
    -Wshadow
    -Wwrite-strings
    -Wstrict-prototypes
    -Wold-style-definition
    -Wredundant-decls
    -Wnested-externs
    -Wmissing-include-dirs
)

# Sanitizers para Debug
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT APPLE)
    list(APPEND COMMON_FLAGS
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )
    # En macOS, los sanitizers vienen integrados y no se linkan como librerías separadas
    # No linkear sanitizers como libs en ningún caso; en macOS vienen integrados, en otros enlazaremos por flags
    set(SANITIZER_LIBS "")
endif()

# Cobertura de código
if(ENABLE_COVERAGE)
    list(APPEND COMMON_FLAGS --coverage)
    set(COVERAGE_LIBS gcov)
endif()

# Verificar disponibilidad de funciones de red
include(CheckFunctionExists)
include(CheckIncludeFile)

check_include_file("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file("arpa/inet.h" HAVE_ARPA_INET_H)
check_function_exists(socket HAVE_SOCKET)
check_function_exists(bind HAVE_BIND)
check_function_exists(sendto HAVE_SENDTO)
check_function_exists(recvfrom HAVE_RECVFROM)

if(NOT HAVE_SYS_SOCKET_H OR NOT HAVE_NETINET_IN_H OR NOT HAVE_ARPA_INET_H)
    message(FATAL_ERROR "Headers de red requeridos no encontrados")
endif()

if(NOT HAVE_SOCKET OR NOT HAVE_BIND OR NOT HAVE_SENDTO OR NOT HAVE_RECVFROM)
    message(FATAL_ERROR "Funciones de red requeridas no encontradas")
endif()

# Configurar archivos de configuración
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
    @ONLY
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Librería principal de comunicación UDP
add_library(comunicacion_udp STATIC
    ${SRC_DIR}/comunicacion_udp.c
)

target_include_directories(comunicacion_udp
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(comunicacion_udp PRIVATE ${COMMON_FLAGS})

if(BUILD_TOOLS)
    # Programa principal de demostración
    add_executable(main_comunicacion_udp
        ${SRC_DIR}/main.c
    )
    target_link_libraries(main_comunicacion_udp 
        comunicacion_udp
        ${SANITIZER_LIBS}
        ${COVERAGE_LIBS}
    )
    target_compile_options(main_comunicacion_udp PRIVATE ${COMMON_FLAGS})
endif()

# Herramientas auxiliares
if(BUILD_TOOLS)
    # Emisor UDP
    add_executable(emisor_udp
        ${TOOLS_DIR}/emisor_udp.c
    )
    
    target_compile_options(emisor_udp PRIVATE ${COMMON_FLAGS})
    target_link_libraries(emisor_udp ${SANITIZER_LIBS})
    
    # Receptor UDP
    add_executable(receptor_udp
        ${TOOLS_DIR}/receptor_udp.c
    )
    
    target_compile_options(receptor_udp PRIVATE ${COMMON_FLAGS})
    target_link_libraries(receptor_udp ${SANITIZER_LIBS})
    
    message(STATUS "Herramientas auxiliares habilitadas")
endif()

# Tests con Criterion
if(BUILD_TESTS)
    find_library(CRITERION_LIB criterion)
    find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
    if(CRITERION_LIB AND CRITERION_INCLUDE_DIR)
        add_executable(test_comunicacion_udp
            ${TESTS_DIR}/test_comunicacion_udp.c
        )
        
        target_link_libraries(test_comunicacion_udp 
            comunicacion_udp
            ${CRITERION_LIB}
            ${SANITIZER_LIBS}
            ${COVERAGE_LIBS}
        )
        if(APPLE)
            # Evitar problemas de link con ASan/UBSan en macOS para los tests
            target_compile_options(test_comunicacion_udp PRIVATE -fno-sanitize=address -fno-sanitize=undefined)
            target_link_options(test_comunicacion_udp PRIVATE -fno-sanitize=address -fno-sanitize=undefined)
        endif()
        target_include_directories(test_comunicacion_udp PRIVATE ${CRITERION_INCLUDE_DIR})
        
        target_compile_options(test_comunicacion_udp PRIVATE ${COMMON_FLAGS})
    target_compile_definitions(test_comunicacion_udp PRIVATE UNIT_TESTING)
        
        # Configurar CTest
        enable_testing()
        add_test(
            NAME UDPCommunicationTests
            COMMAND test_comunicacion_udp
        )
        
        # Tests adicionales con diferentes configuraciones
        add_test(
            NAME UDPTestsVerbose
            COMMAND test_comunicacion_udp --verbose
        )
        
        add_test(
            NAME UDPTestsSingleThread
            COMMAND test_comunicacion_udp --jobs=1
        )
        
        message(STATUS "Tests con Criterion habilitados")
    else()
        message(WARNING "Criterion no encontrado. Tests deshabilitados.")
        message(STATUS "Para instalar Criterion:")
        message(STATUS "  macOS: brew install criterion")
        message(STATUS "  Ubuntu: sudo apt-get install libcriterion-dev")
    endif()
endif()

# Documentación con Doxygen
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        
        if(EXISTS ${DOXYGEN_IN})
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            
            add_custom_target(docs
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generando documentación con Doxygen"
                VERBATIM
            )
            
            message(STATUS "Documentación Doxygen habilitada")
        else()
            message(WARNING "Archivo Doxyfile.in no encontrado")
        endif()
    else()
        message(WARNING "Doxygen no encontrado. Documentación deshabilitada.")
    endif()
endif()

# Análisis estático
if(ENABLE_STATIC_ANALYSIS)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set_target_properties(comunicacion_udp PROPERTIES
            C_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=-*,readability-*,performance-*,portability-*"
        )
        message(STATUS "Análisis estático con clang-tidy habilitado")
    else()
        message(WARNING "clang-tidy no encontrado")
    endif()
endif()

# Targets personalizados

# Target para limpiar completamente
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMENT "Limpieza completa del build"
)

# Target para ejecutar demos
add_custom_target(demo
    COMMAND echo "=== DEMO EMISOR UDP ==="
    COMMAND echo "Ejecuta: ./emisor_udp 127.0.0.1 9090 \"Mensaje de prueba\""
    COMMAND echo ""
    COMMAND echo "=== DEMO RECEPTOR UDP ==="
    COMMAND echo "Ejecuta: ./receptor_udp 9090"
    COMMAND echo ""
    COMMAND echo "=== DEMO INTERACTIVO ==="
    COMMAND echo "Ejecuta: ./main_comunicacion_udp"
    COMMENT "Instrucciones para ejecutar demos"
)

# Target para benchmark
if(BUILD_TOOLS)
    add_custom_target(benchmark
        COMMAND echo "Iniciando benchmark UDP..."
        COMMAND echo "Ejecuta el receptor en otra terminal: ./receptor_udp -s 9090"
        COMMAND sleep 2
        COMMAND ./emisor_udp -n 1000 -i 1 127.0.0.1 9090 "Benchmark message"
        DEPENDS emisor_udp
        COMMENT "Ejecutando benchmark de rendimiento UDP"
    )
endif()

# Target de instalación
install(TARGETS comunicacion_udp
    ARCHIVE DESTINATION lib
)

install(FILES ${INCLUDE_DIR}/comunicacion_udp.h
    DESTINATION include
)

if(BUILD_TOOLS)
    install(TARGETS emisor_udp receptor_udp main_comunicacion_udp
        RUNTIME DESTINATION bin
    )
endif()

# Información de build
add_custom_target(info
    COMMAND echo "=== INFORMACIÓN DEL BUILD ==="
    COMMAND echo "Tipo de build: ${CMAKE_BUILD_TYPE}"
    COMMAND echo "Compilador: ${CMAKE_C_COMPILER}"
    COMMAND echo "Flags: ${CMAKE_C_FLAGS} ${COMMON_FLAGS}"
    COMMAND echo "Tests: ${BUILD_TESTS}"
    COMMAND echo "Herramientas: ${BUILD_TOOLS}"
    COMMAND echo "Documentación: ${BUILD_DOCS}"
    COMMAND echo "Sanitizers: ${ENABLE_SANITIZERS}"
    COMMAND echo "Cobertura: ${ENABLE_COVERAGE}"
    COMMAND echo "============================="
    COMMENT "Información de configuración del build"
)

# Resumen final
message(STATUS "")
message(STATUS "===== CONFIGURACIÓN COMUNICACIÓN UDP =====")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compilador: ${CMAKE_C_COMPILER}")
message(STATUS "Tests: ${BUILD_TESTS}")
message(STATUS "Herramientas: ${BUILD_TOOLS}")
message(STATUS "Documentación: ${BUILD_DOCS}")
message(STATUS "Sanitizers: ${ENABLE_SANITIZERS}")
message(STATUS "Cobertura: ${ENABLE_COVERAGE}")
message(STATUS "=========================================")
