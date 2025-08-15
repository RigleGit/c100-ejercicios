cmake_minimum_required(VERSION 3.10)
project(union_reinterpretar)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(union_reinterpretar src/main.c src/union_reinterpretar.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(union_reinterpretar PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_union_reinterpretar tests/test_union_reinterpretar.c src/main.c src/union_reinterpretar.c)
    target_link_libraries(test_union_reinterpretar ${CRITERION_LIBRARIES})
    target_include_directories(test_union_reinterpretar PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_union_reinterpretar PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_union_reinterpretar PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_union_reinterpretar PRIVATE include)
    endif()
    add_test(NAME union_reinterpretar_tests COMMAND test_union_reinterpretar)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(union_reinterpretar PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_union_reinterpretar)
        target_compile_options(test_union_reinterpretar PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_union_reinterpretar PRIVATE UNIT_TESTING)
    endif()
endif()
