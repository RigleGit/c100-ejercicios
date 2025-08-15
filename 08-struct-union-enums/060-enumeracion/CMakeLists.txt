cmake_minimum_required(VERSION 3.10)
project(enumeracion)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(enumeracion src/main.c src/enumeracion.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(enumeracion PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_enumeracion tests/test_enumeracion.c src/main.c src/enumeracion.c)
    target_link_libraries(test_enumeracion ${CRITERION_LIBRARIES})
    target_include_directories(test_enumeracion PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_enumeracion PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_enumeracion PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_enumeracion PRIVATE include)
    endif()
    add_test(NAME enumeracion_tests COMMAND test_enumeracion)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(enumeracion PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_enumeracion)
        target_compile_options(test_enumeracion PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_enumeracion PRIVATE UNIT_TESTING)
    endif()
endif()
