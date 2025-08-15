cmake_minimum_required(VERSION 3.10)
project(lista_enlazada)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(lista_enlazada src/lista_enlazada.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(lista_enlazada PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_lista_enlazada tests/test_lista_enlazada.c src/lista_enlazada.c)
    target_link_libraries(test_lista_enlazada ${CRITERION_LIBRARIES})
    target_include_directories(test_lista_enlazada PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_lista_enlazada PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_lista_enlazada PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_lista_enlazada PRIVATE include)
    endif()
    add_test(NAME lista_enlazada_tests COMMAND test_lista_enlazada)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lista_enlazada PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_lista_enlazada)
        target_compile_options(test_lista_enlazada PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
