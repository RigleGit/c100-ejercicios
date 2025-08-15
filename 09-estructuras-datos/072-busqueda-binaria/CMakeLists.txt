cmake_minimum_required(VERSION 3.10)
project(busqueda_binaria)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(busqueda_binaria src/busqueda_binaria.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(busqueda_binaria PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_busqueda_binaria tests/test_busqueda_binaria.c src/busqueda_binaria.c)
    target_link_libraries(test_busqueda_binaria ${CRITERION_LIBRARIES})
    target_include_directories(test_busqueda_binaria PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_busqueda_binaria PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_busqueda_binaria PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_busqueda_binaria PRIVATE include)
    endif()
    add_test(NAME busqueda_binaria_tests COMMAND test_busqueda_binaria)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(busqueda_binaria PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_busqueda_binaria)
        target_compile_options(test_busqueda_binaria PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
