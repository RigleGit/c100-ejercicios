cmake_minimum_required(VERSION 3.10)
project(operaciones_aritmeticas_seguras)
set(CMAKE_C_STANDARD 99)

# Buscar dependencias
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

# Biblioteca de operaciones seguras
add_library(operaciones_seguras_lib STATIC src/operaciones_aritmeticas_seguras.c)
target_include_directories(operaciones_seguras_lib PUBLIC include)

# Ejecutable principal
add_executable(operaciones_aritmeticas_seguras src/main.c)
target_link_libraries(operaciones_aritmeticas_seguras operaciones_seguras_lib)

# Tests
if(CRITERION_FOUND)
    add_executable(test_operaciones_aritmeticas_seguras tests/test_operaciones_aritmeticas_seguras.c)
    target_link_libraries(test_operaciones_aritmeticas_seguras 
        operaciones_seguras_lib 
        ${CRITERION_LIBRARIES})
    target_include_directories(test_operaciones_aritmeticas_seguras PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_operaciones_aritmeticas_seguras PRIVATE ${CRITERION_LIBRARY_DIRS})
    
    enable_testing()
    add_test(NAME operaciones_aritmeticas_seguras_tests COMMAND test_operaciones_aritmeticas_seguras)
endif()

# Warnings básicos
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(operaciones_seguras_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(operaciones_aritmeticas_seguras PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_operaciones_aritmeticas_seguras)
        target_compile_options(test_operaciones_aritmeticas_seguras PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_operaciones_aritmeticas_seguras PRIVATE UNIT_TESTING)
    endif()
endif()