cmake_minimum_required(VERSION 3.10)
project(075-copiar-archivo-texto)
set(CMAKE_C_STANDARD 99)

# Tests dummy
enable_testing()
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    add_executable(test_dummy tests/test_dummy.c)
    target_link_libraries(test_dummy ${CRITERION_LIBRARIES})
    target_include_directories(test_dummy PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_dummy PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_dummy PRIVATE ${CRITERION_CFLAGS_OTHER})
    add_test(NAME dummy_tests COMMAND test_dummy)
endif()
