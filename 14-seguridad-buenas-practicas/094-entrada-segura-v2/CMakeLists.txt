cmake_minimum_required(VERSION 3.10)
project(entrada_segura)
set(CMAKE_C_STANDARD 99)

# Buscar dependencias
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

# Biblioteca de entrada segura
add_library(entrada_segura_lib STATIC src/entrada_segura.c)
target_include_directories(entrada_segura_lib PUBLIC include)

# Ejecutable principal
add_executable(entrada_segura src/main.c)
target_link_libraries(entrada_segura entrada_segura_lib)

# Tests
if(CRITERION_FOUND)
    add_executable(test_entrada_segura tests/test_entrada_segura.c)
    target_link_libraries(test_entrada_segura 
        entrada_segura_lib 
        ${CRITERION_LIBRARIES})
    target_include_directories(test_entrada_segura PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_entrada_segura PRIVATE ${CRITERION_LIBRARY_DIRS})
    
    enable_testing()
    add_test(NAME entrada_segura_tests COMMAND test_entrada_segura)
endif()

# Warnings básicos
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(entrada_segura_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(entrada_segura PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_entrada_segura)
        target_compile_options(test_entrada_segura PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_entrada_segura PRIVATE UNIT_TESTING)
    endif()
endif()