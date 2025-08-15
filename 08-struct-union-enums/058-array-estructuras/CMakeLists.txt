cmake_minimum_required(VERSION 3.10)
project(array_estructuras)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(array_estructuras src/main.c src/array_estructuras.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(array_estructuras PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_array_estructuras tests/test_array_estructuras.c src/main.c src/array_estructuras.c)
    target_link_libraries(test_array_estructuras ${CRITERION_LIBRARIES})
    target_include_directories(test_array_estructuras PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_array_estructuras PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_array_estructuras PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_array_estructuras PRIVATE include)
    endif()
    add_test(NAME array_estructuras_tests COMMAND test_array_estructuras)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(array_estructuras PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_array_estructuras)
        target_compile_options(test_array_estructuras PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_array_estructuras PRIVATE UNIT_TESTING)
    endif()
endif()
