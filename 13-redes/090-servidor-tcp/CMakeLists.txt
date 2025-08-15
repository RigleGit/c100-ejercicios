cmake_minimum_required(VERSION 3.10)
project(ServidorTCP VERSION 1.0.0 LANGUAGES C)

# ============================================================================
# CONFIGURACIÓN DEL PROYECTO
# ============================================================================

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Directorio de inclusión
include_directories(include)

# ============================================================================
# VERIFICACIONES DEL SISTEMA
# ============================================================================

# Verificar que tenemos soporte para sockets
include(CheckIncludeFile)
include(CheckFunctionExists)

check_include_file("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file("arpa/inet.h" HAVE_ARPA_INET_H)
check_include_file("pthread.h" HAVE_PTHREAD_H)

if(NOT HAVE_SYS_SOCKET_H)
    message(FATAL_ERROR "sys/socket.h no encontrado. Se requiere soporte para sockets BSD.")
endif()

if(NOT HAVE_NETINET_IN_H)
    message(FATAL_ERROR "netinet/in.h no encontrado. Se requiere soporte para TCP/IP.")
endif()

if(NOT HAVE_ARPA_INET_H)
    message(FATAL_ERROR "arpa/inet.h no encontrado. Se requiere soporte para funciones de red.")
endif()

if(NOT HAVE_PTHREAD_H)
    message(FATAL_ERROR "pthread.h no encontrado. Se requiere soporte para threads.")
endif()

check_function_exists("socket" HAVE_SOCKET)
check_function_exists("bind" HAVE_BIND)
check_function_exists("listen" HAVE_LISTEN)
check_function_exists("accept" HAVE_ACCEPT)
check_function_exists("pthread_create" HAVE_PTHREAD_CREATE)

if(NOT HAVE_SOCKET OR NOT HAVE_BIND OR NOT HAVE_LISTEN OR NOT HAVE_ACCEPT)
    message(FATAL_ERROR "Funciones de socket BSD no disponibles.")
endif()

message(STATUS "✅ Verificaciones del sistema completadas")
message(STATUS "   - Sockets BSD: Disponible")
message(STATUS "   - TCP/IP: Disponible") 
message(STATUS "   - Threads: Disponible")

# ============================================================================
# OPCIONES DE COMPILACIÓN
# ============================================================================

option(BUILD_TESTS "Compilar tests unitarios" ON)
option(BUILD_TOOLS "Compilar herramientas auxiliares" ON)
option(ENABLE_SANITIZERS "Habilitar sanitizers (AddressSanitizer, UBSan)" OFF)
option(ENABLE_COVERAGE "Habilitar cobertura de código" OFF)
option(BUILD_DOCS "Generar documentación con Doxygen" OFF)
option(BUILD_TESTS_ONLY "Solo compilar tests (para debugging)" OFF)

# ============================================================================
# FLAGS DE COMPILACIÓN
# ============================================================================

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wformat-security")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes -Wmissing-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow -Wpointer-arith -Wcast-qual")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
    message(STATUS "🔧 Compilación en modo DEBUG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
    message(STATUS "🚀 Compilación en modo RELEASE")
endif()

# Sanitizers
if(ENABLE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
        message(STATUS "🛡️ Sanitizers habilitados (AddressSanitizer + UBSan)")
    else()
        message(WARNING "Sanitizers no soportados con este compilador")
    endif()
endif()

# Cobertura de código
if(ENABLE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        message(STATUS "📊 Cobertura de código habilitada")
    else()
        message(WARNING "Cobertura no soportada con este compilador")
    endif()
endif()

# ============================================================================
# DEPENDENCIAS
# ============================================================================

# Threads
find_package(Threads REQUIRED)

# Criterion para tests
if(BUILD_TESTS)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(CRITERION criterion)
    endif()
    
    if(NOT CRITERION_FOUND)
        message(STATUS "⚠️ Criterion no encontrado via pkg-config, buscando manualmente...")
        find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
        find_library(CRITERION_LIBRARY criterion)
        
        if(CRITERION_INCLUDE_DIR AND CRITERION_LIBRARY)
            set(CRITERION_FOUND TRUE)
            set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})
            set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
        endif()
    endif()
    
    if(CRITERION_FOUND)
        message(STATUS "✅ Criterion encontrado: ${CRITERION_LIBRARIES}")
    else()
        message(WARNING "❌ Criterion no encontrado. Tests deshabilitados.")
        message(STATUS "Para instalar Criterion:")
        message(STATUS "  macOS: brew install criterion")
        message(STATUS "  Ubuntu/Debian: sudo apt-get install libcriterion-dev")
        set(BUILD_TESTS OFF)
    endif()
endif()

# Doxygen para documentación
if(BUILD_DOCS)
    find_package(Doxygen)
    if(NOT Doxygen_FOUND)
        message(WARNING "Doxygen no encontrado. Documentación deshabilitada.")
        set(BUILD_DOCS OFF)
    endif()
endif()

# ============================================================================
# ARCHIVOS FUENTE
# ============================================================================

set(SERVIDOR_SOURCES
    src/servidor_tcp.c
)

set(SERVIDOR_HEADERS
    include/servidor_tcp.h
)

# ============================================================================
# BIBLIOTECAS
# ============================================================================

# Biblioteca estática del servidor
add_library(servidor_tcp_lib STATIC ${SERVIDOR_SOURCES})
target_link_libraries(servidor_tcp_lib Threads::Threads)
target_compile_definitions(servidor_tcp_lib PRIVATE _GNU_SOURCE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(servidor_tcp_lib rt)
endif()

# ============================================================================
# EJECUTABLES
# ============================================================================

if(NOT BUILD_TESTS_ONLY)
    # Ejecutable principal del servidor
    add_executable(servidor_tcp src/main.c)
    target_link_libraries(servidor_tcp servidor_tcp_lib)
    
    message(STATUS "🎯 Ejecutable principal: servidor_tcp")
endif()

# ============================================================================
# HERRAMIENTAS
# ============================================================================

if(BUILD_TOOLS AND NOT BUILD_TESTS_ONLY)
    # Cliente de prueba
    add_executable(cliente_prueba tools/cliente_prueba.c)
    target_link_libraries(cliente_prueba Threads::Threads)
    
    message(STATUS "🔧 Herramientas:")
    message(STATUS "   - cliente_prueba")
endif()

# ============================================================================
# TESTS
# ============================================================================

if(BUILD_TESTS AND CRITERION_FOUND)
    enable_testing()
    
    # Tests unitarios
    add_executable(test_servidor_tcp tests/test_servidor_tcp.c)
    if(NOT CRITERION_LIBRARIES)
        find_library(CRITERION_LIBRARY criterion REQUIRED)
        set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
        find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
        if(CRITERION_INCLUDE_DIR)
            list(APPEND CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})
        endif()
    endif()
    # En macOS, asegurar directorios de enlace si pkg-config no los expone
    if(CRITERION_LIBRARY_DIRS)
        target_link_directories(test_servidor_tcp PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_definitions(test_servidor_tcp PRIVATE UNIT_TESTING)
    endif()
    target_link_libraries(test_servidor_tcp 
        servidor_tcp_lib
        ${CRITERION_LIBRARIES}
        Threads::Threads
    )
    target_include_directories(test_servidor_tcp PRIVATE ${CRITERION_INCLUDE_DIRS})
    
    # Agregar test a CTest
    add_test(NAME test_servidor_tcp COMMAND test_servidor_tcp --jobs=1)
    
    # Target personalizado para ejecutar tests con salida detallada
    add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
        DEPENDS test_servidor_tcp
        COMMENT "Ejecutando tests del servidor TCP..."
    )
    
    message(STATUS "🧪 Tests habilitados:")
    message(STATUS "   - test_servidor_tcp")
    message(STATUS "   Ejecutar con: make test o make run_tests")
endif()

# ============================================================================
# DOCUMENTACIÓN
# ============================================================================

if(BUILD_DOCS AND Doxygen_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generando documentación con Doxygen"
        VERBATIM
    )
    
    message(STATUS "📚 Documentación habilitada: make docs")
endif()

# ============================================================================
# INSTALACIÓN
# ============================================================================

if(NOT BUILD_TESTS_ONLY)
    install(TARGETS servidor_tcp
        RUNTIME DESTINATION bin
        COMPONENT servidor
    )
    
    install(FILES ${SERVIDOR_HEADERS}
        DESTINATION include/servidor_tcp
        COMPONENT desarrollo
    )
    
    install(TARGETS servidor_tcp_lib
        ARCHIVE DESTINATION lib
        COMPONENT desarrollo
    )
    
    if(BUILD_TOOLS)
        install(TARGETS cliente_prueba
            RUNTIME DESTINATION bin
            COMPONENT herramientas
        )
    endif()
endif()

# ============================================================================
# TARGETS DE INFORMACIÓN
# ============================================================================

# Target para mostrar información del sistema
add_custom_target(info
    COMMAND ${CMAKE_COMMAND} -E echo "=== INFORMACIÓN DEL SISTEMA ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Sistema: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Compilador: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Arquitectura: ${CMAKE_SYSTEM_PROCESSOR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Build type: ${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "=== CONFIGURACIÓN DEL PROYECTO ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Tests: ${BUILD_TESTS}"
    COMMAND ${CMAKE_COMMAND} -E echo "Herramientas: ${BUILD_TOOLS}"
    COMMAND ${CMAKE_COMMAND} -E echo "Sanitizers: ${ENABLE_SANITIZERS}"
    COMMAND ${CMAKE_COMMAND} -E echo "Cobertura: ${ENABLE_COVERAGE}"
    COMMAND ${CMAKE_COMMAND} -E echo "Documentación: ${BUILD_DOCS}"
    COMMAND ${CMAKE_COMMAND} -E echo "=== FLAGS DE COMPILACIÓN ==="
    COMMAND ${CMAKE_COMMAND} -E echo "CFLAGS: ${CMAKE_C_FLAGS}"
    COMMENT "Información del sistema y configuración"
)

# Target para limpiar archivos generados
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory docs/html
    COMMAND ${CMAKE_COMMAND} -E remove_directory docs/latex
    COMMAND ${CMAKE_COMMAND} -E remove *.gcda *.gcno *.gcov
    COMMENT "Limpieza completa del proyecto"
)

# ============================================================================
# RESUMEN FINAL
# ============================================================================

message(STATUS "")
message(STATUS "🎯 ========================================")
message(STATUS "📦 PROYECTO: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "🎯 ========================================")
message(STATUS "")

if(NOT BUILD_TESTS_ONLY)
    message(STATUS "📋 TARGETS DISPONIBLES:")
    message(STATUS "   make servidor_tcp      - Compilar servidor principal")
    if(BUILD_TOOLS)
        message(STATUS "   make cliente_prueba    - Compilar cliente de prueba")
    endif()
    if(BUILD_TESTS)
        message(STATUS "   make test_servidor_tcp - Compilar tests")
        message(STATUS "   make test             - Ejecutar tests (CTest)")
        message(STATUS "   make run_tests        - Ejecutar tests (detallado)")
    endif()
    if(BUILD_DOCS)
        message(STATUS "   make docs             - Generar documentación")
    endif()
    message(STATUS "   make install          - Instalar proyecto")
    message(STATUS "   make info             - Información del sistema")
    message(STATUS "   make clean-all        - Limpieza completa")
    message(STATUS "")
    message(STATUS "🚀 EJEMPLO DE USO:")
    message(STATUS "   ./servidor_tcp                    # Servidor interactivo")
    message(STATUS "   ./cliente_prueba -m \"Hola\"       # Cliente de prueba")
    message(STATUS "   ./cliente_prueba -b -n 1000      # Benchmark")
else()
    message(STATUS "🧪 MODO TESTS ONLY:")
    message(STATUS "   make test_servidor_tcp - Compilar tests")
    message(STATUS "   make run_tests        - Ejecutar tests")
endif()

message(STATUS "")
message(STATUS "📚 EJERCICIO 090: Servidor TCP de Eco")
message(STATUS "   Implementa un servidor TCP que acepta conexiones,")
message(STATUS "   recibe mensajes de clientes y devuelve el mismo")
message(STATUS "   mensaje (eco) usando sockets BSD.")
message(STATUS "")
message(STATUS "🎯 ========================================")
message(STATUS "")
