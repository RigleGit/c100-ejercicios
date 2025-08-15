cmake_minimum_required(VERSION 3.10)
project(estructura_dinamica)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(estructura_dinamica src/main.c src/estructura_dinamica.c src/csv_utils.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(estructura_dinamica PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_estructura_dinamica tests/test_estructura_dinamica.c src/main.c src/estructura_dinamica.c src/csv_utils.c)
    target_link_libraries(test_estructura_dinamica ${CRITERION_LIBRARIES})
    target_include_directories(test_estructura_dinamica PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_estructura_dinamica PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_estructura_dinamica PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_estructura_dinamica PRIVATE include)
    endif()
    add_test(NAME estructura_dinamica_tests COMMAND test_estructura_dinamica)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(estructura_dinamica PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_estructura_dinamica)
        target_compile_options(test_estructura_dinamica PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_estructura_dinamica PRIVATE UNIT_TESTING)
    endif()
endif()
