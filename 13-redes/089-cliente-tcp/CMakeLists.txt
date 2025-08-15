cmake_minimum_required(VERSION 3.10)
project(cliente_tcp)
set(CMAKE_C_STANDARD 99)

# Configuración básica de compilación (suprimir warnings problemáticos)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-gnu-zero-variadic-macro-arguments -Wno-cast-align -Wno-sign-conversion -Wno-unreachable-code")

# Buscar librerías del sistema
find_library(SOCKET_LIB socket)
find_library(NSL_LIB nsl)

# Archivos fuente
set(SOURCES
    src/cliente_tcp.c
)

# Biblioteca estática
add_library(cliente_tcp_lib STATIC ${SOURCES})
target_include_directories(cliente_tcp_lib PUBLIC include)

# Enlazar librerías del sistema si están disponibles
if(SOCKET_LIB)
    target_link_libraries(cliente_tcp_lib ${SOCKET_LIB})
endif()
if(NSL_LIB)
    target_link_libraries(cliente_tcp_lib ${NSL_LIB})
endif()

# Ejecutable principal
add_executable(cliente_tcp_main src/main.c)
target_link_libraries(cliente_tcp_main cliente_tcp_lib)

# Tests con Criterion
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    # Tests unitarios (sin red) - por defecto
    add_executable(test_cliente_tcp tests/test_cliente_tcp_unit.c)
    target_link_libraries(test_cliente_tcp 
        cliente_tcp_lib 
        ${CRITERION_LIBRARIES}
    )
    target_include_directories(test_cliente_tcp PRIVATE 
        ${CRITERION_INCLUDE_DIRS}
        include
    )
    target_link_directories(test_cliente_tcp PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_cliente_tcp PRIVATE ${CRITERION_CFLAGS_OTHER})
    target_compile_definitions(test_cliente_tcp PRIVATE UNIT_TESTING)
    
    # Enlazar librerías del sistema al test también
    if(SOCKET_LIB)
        target_link_libraries(test_cliente_tcp ${SOCKET_LIB})
    endif()
    if(NSL_LIB)
        target_link_libraries(test_cliente_tcp ${NSL_LIB})
    endif()
    
    add_test(NAME cliente_tcp_unit_tests COMMAND test_cliente_tcp)
    
    # Tests de integración (con red) - opcionales
    option(BUILD_INTEGRATION_TESTS "Build integration tests requiring network" OFF)
    if(BUILD_INTEGRATION_TESTS)
        add_executable(test_cliente_tcp_integration tests/test_cliente_tcp_integration.c)
        target_link_libraries(test_cliente_tcp_integration 
            cliente_tcp_lib 
            ${CRITERION_LIBRARIES}
        )
        target_include_directories(test_cliente_tcp_integration PRIVATE 
            ${CRITERION_INCLUDE_DIRS}
            include
        )
        target_link_directories(test_cliente_tcp_integration PRIVATE ${CRITERION_LIBRARY_DIRS})
        target_compile_options(test_cliente_tcp_integration PRIVATE ${CRITERION_CFLAGS_OTHER})
        target_compile_definitions(test_cliente_tcp_integration PRIVATE UNIT_TESTING)
        
        if(SOCKET_LIB)
            target_link_libraries(test_cliente_tcp_integration ${SOCKET_LIB})
        endif()
        if(NSL_LIB)
            target_link_libraries(test_cliente_tcp_integration ${NSL_LIB})
        endif()
        
        add_test(NAME cliente_tcp_integration_tests COMMAND test_cliente_tcp_integration)
    endif()
endif()

# Servidor de prueba (opcional, solo si existe)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/servidor_prueba.c")
    add_executable(servidor_prueba tools/servidor_prueba.c)
    target_include_directories(servidor_prueba PRIVATE include)
    if(SOCKET_LIB)
        target_link_libraries(servidor_prueba ${SOCKET_LIB})
    endif()
    if(NSL_LIB)
        target_link_libraries(servidor_prueba ${NSL_LIB})
    endif()
endif()