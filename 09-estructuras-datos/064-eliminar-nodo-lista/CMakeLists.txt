cmake_minimum_required(VERSION 3.10)
project(eliminar_nodo)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(eliminar_nodo src/eliminar_nodo.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(eliminar_nodo PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_eliminar_nodo tests/test_eliminar_nodo.c src/eliminar_nodo.c)
    target_link_libraries(test_eliminar_nodo ${CRITERION_LIBRARIES})
    target_include_directories(test_eliminar_nodo PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_eliminar_nodo PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_eliminar_nodo PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_eliminar_nodo PRIVATE include)
    endif()
    add_test(NAME eliminar_nodo_tests COMMAND test_eliminar_nodo)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(eliminar_nodo PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_eliminar_nodo)
        target_compile_options(test_eliminar_nodo PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
