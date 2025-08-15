cmake_minimum_required(VERSION 3.10)
project(invertir_lista)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(invertir_lista src/invertir_lista.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(invertir_lista PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_invertir_lista tests/test_invertir_lista.c src/invertir_lista.c)
    target_link_libraries(test_invertir_lista ${CRITERION_LIBRARIES})
    target_include_directories(test_invertir_lista PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_invertir_lista PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_invertir_lista PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_invertir_lista PRIVATE include)
    endif()
    add_test(NAME invertir_lista_tests COMMAND test_invertir_lista)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(invertir_lista PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_invertir_lista)
        target_compile_options(test_invertir_lista PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
