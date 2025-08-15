cmake_minimum_required(VERSION 3.10)
project(quicksort)
set(CMAKE_C_STANDARD 99)

# Ejecutable principal
add_executable(quicksort src/quicksort.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(quicksort PRIVATE include)
endif()

# Tests
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_quicksort tests/test_quicksort.c src/quicksort.c)
    target_link_libraries(test_quicksort ${CRITERION_LIBRARIES})
    target_include_directories(test_quicksort PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_quicksort PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_quicksort PRIVATE ${CRITERION_CFLAGS_OTHER})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_quicksort PRIVATE include)
    endif()
    add_test(NAME quicksort_tests COMMAND test_quicksort)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(quicksort PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_quicksort)
        target_compile_options(test_quicksort PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
