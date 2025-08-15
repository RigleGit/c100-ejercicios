cmake_minimum_required(VERSION 3.10)
project(lista_doble)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(lista_doble src/lista_doble.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(lista_doble PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_lista_doble tests/test_lista_doble.c src/lista_doble.c)
    target_link_libraries(test_lista_doble ${CRITERION_LIBRARIES})
    target_include_directories(test_lista_doble PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_lista_doble PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_lista_doble PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_lista_doble PRIVATE include)
    endif()
    add_test(NAME lista_doble_tests COMMAND test_lista_doble)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lista_doble PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_lista_doble)
        target_compile_options(test_lista_doble PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
