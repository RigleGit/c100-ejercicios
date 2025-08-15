cmake_minimum_required(VERSION 3.10)
project(funciones_seguras_cadenas)
set(CMAKE_C_STANDARD 99)

# Buscar dependencias
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

# Biblioteca de funciones seguras
add_library(funciones_seguras_lib STATIC src/funciones_seguras_cadenas.c)
target_include_directories(funciones_seguras_lib PUBLIC include)

# Ejecutable principal
add_executable(funciones_seguras_cadenas src/main.c)
target_link_libraries(funciones_seguras_cadenas funciones_seguras_lib)

# Tests
if(CRITERION_FOUND)
    add_executable(test_funciones_seguras_cadenas tests/test_funciones_seguras_cadenas.c)
    target_link_libraries(test_funciones_seguras_cadenas 
        funciones_seguras_lib 
        ${CRITERION_LIBRARIES})
    target_include_directories(test_funciones_seguras_cadenas PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_funciones_seguras_cadenas PRIVATE ${CRITERION_LIBRARY_DIRS})
    
    enable_testing()
    add_test(NAME funciones_seguras_cadenas_tests COMMAND test_funciones_seguras_cadenas)
endif()

# Warnings básicos
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(funciones_seguras_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(funciones_seguras_cadenas PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_funciones_seguras_cadenas)
        target_compile_options(test_funciones_seguras_cadenas PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_funciones_seguras_cadenas PRIVATE UNIT_TESTING)
    endif()
endif()