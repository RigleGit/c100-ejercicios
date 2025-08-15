cmake_minimum_required(VERSION 3.10)
project(pila_lista)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(pila_lista src/pila_lista.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(pila_lista PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_pila_lista tests/test_pila_lista.c src/pila_lista.c)
    target_link_libraries(test_pila_lista ${CRITERION_LIBRARIES})
    target_include_directories(test_pila_lista PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_pila_lista PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_pila_lista PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_pila_lista PRIVATE include)
    endif()
    add_test(NAME pila_lista_tests COMMAND test_pila_lista)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pila_lista PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_pila_lista)
        target_compile_options(test_pila_lista PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
