cmake_minimum_required(VERSION 3.10)
project(servidor_tcp_multicliente)
set(CMAKE_C_STANDARD 99)

# Buscar dependencias
find_package(Threads REQUIRED)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

# Biblioteca del servidor
add_library(servidor_tcp_multicliente_lib STATIC src/servidor_tcp_multicliente.c)
target_include_directories(servidor_tcp_multicliente_lib PUBLIC include)
target_link_libraries(servidor_tcp_multicliente_lib PRIVATE Threads::Threads)

# Tests
if(CRITERION_FOUND)
    add_executable(test_servidor_tcp_multicliente tests/test_servidor_tcp_multicliente.c)
    target_link_libraries(test_servidor_tcp_multicliente 
        servidor_tcp_multicliente_lib 
        ${CRITERION_LIBRARIES} 
        Threads::Threads)
    target_include_directories(test_servidor_tcp_multicliente PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_servidor_tcp_multicliente PRIVATE ${CRITERION_LIBRARY_DIRS})
    
    enable_testing()
    add_test(NAME servidor_tcp_multicliente_tests COMMAND test_servidor_tcp_multicliente)
endif()

# Herramientas auxiliares (que sí compilan bien)
add_executable(cliente_prueba tools/cliente_prueba.c)
add_executable(benchmark_servidor tools/benchmark_servidor.c)
target_link_libraries(cliente_prueba Threads::Threads)
target_link_libraries(benchmark_servidor servidor_tcp_multicliente_lib Threads::Threads)

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(servidor_tcp_multicliente_lib PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_servidor_tcp_multicliente)
        target_compile_options(test_servidor_tcp_multicliente PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_servidor_tcp_multicliente PRIVATE UNIT_TESTING)
    endif()
endif()
