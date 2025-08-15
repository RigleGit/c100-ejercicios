cmake_minimum_required(VERSION 3.10)
project(productor_consumidor)
set(CMAKE_C_STANDARD 99)

# Buscar dependencias
find_package(Threads REQUIRED)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

# Biblioteca de productor-consumidor
add_library(productor_consumidor_lib STATIC src/productor_consumidor.c)
target_include_directories(productor_consumidor_lib PUBLIC include)
target_link_libraries(productor_consumidor_lib PRIVATE Threads::Threads)

# Tests
if(CRITERION_FOUND)
    add_executable(test_productor_consumidor tests/test_productor_consumidor.c)
    target_link_libraries(test_productor_consumidor 
        productor_consumidor_lib 
        ${CRITERION_LIBRARIES} 
        Threads::Threads)
    target_include_directories(test_productor_consumidor PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_productor_consumidor PRIVATE ${CRITERION_LIBRARY_DIRS})
    
    enable_testing()
    add_test(NAME productor_consumidor_tests COMMAND test_productor_consumidor)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(productor_consumidor_lib PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_productor_consumidor)
        target_compile_options(test_productor_consumidor PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_productor_consumidor PRIVATE UNIT_TESTING)
    endif()
endif()
