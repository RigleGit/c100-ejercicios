cmake_minimum_required(VERSION 3.10)
project(arbol_bst)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(arbol_bst src/main.c src/arbol_bst.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(arbol_bst PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_arbol_bst tests/test_arbol_bst.c src/arbol_bst.c)
    target_link_libraries(test_arbol_bst ${CRITERION_LIBRARIES})
    target_include_directories(test_arbol_bst PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_arbol_bst PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_arbol_bst PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_arbol_bst PRIVATE include)
    endif()
    add_test(NAME arbol_bst_tests COMMAND test_arbol_bst)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(arbol_bst PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_arbol_bst)
        target_compile_options(test_arbol_bst PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
