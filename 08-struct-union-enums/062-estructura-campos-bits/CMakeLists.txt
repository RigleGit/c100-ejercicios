cmake_minimum_required(VERSION 3.10)
project(bitfields_estado)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(bitfields_estado src/bitfields_estado.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(bitfields_estado PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_bitfields_estado tests/test_bitfields_estado.c src/bitfields_estado.c)
    target_link_libraries(test_bitfields_estado ${CRITERION_LIBRARIES})
    target_include_directories(test_bitfields_estado PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_bitfields_estado PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_bitfields_estado PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_bitfields_estado PRIVATE include)
    endif()
    add_test(NAME bitfields_estado_tests COMMAND test_bitfields_estado)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bitfields_estado PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_bitfields_estado)
        target_compile_options(test_bitfields_estado PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_bitfields_estado PRIVATE UNIT_TESTING)
    endif()
endif()
