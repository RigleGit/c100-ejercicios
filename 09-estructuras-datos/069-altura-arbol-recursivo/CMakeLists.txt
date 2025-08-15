cmake_minimum_required(VERSION 3.10)
project(altura_arbol)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(altura_arbol src/altura_arbol.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(altura_arbol PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_altura_arbol tests/test_altura_arbol.c src/altura_arbol.c)
    target_link_libraries(test_altura_arbol ${CRITERION_LIBRARIES})
    target_include_directories(test_altura_arbol PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_altura_arbol PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_altura_arbol PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_altura_arbol PRIVATE include)
    endif()
    add_test(NAME altura_arbol_tests COMMAND test_altura_arbol)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(altura_arbol PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_altura_arbol)
        target_compile_options(test_altura_arbol PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
