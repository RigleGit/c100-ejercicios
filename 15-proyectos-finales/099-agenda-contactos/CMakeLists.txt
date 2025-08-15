cmake_minimum_required(VERSION 3.10)
project(agenda_contactos)
set(CMAKE_C_STANDARD 99)

# Buscar dependencias
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

# Biblioteca de la agenda
add_library(agenda_lib STATIC 
    src/contacto.c 
    src/menu.c 
    src/persistencia.c)
target_include_directories(agenda_lib PUBLIC include)

# Ejecutable principal
add_executable(agenda_contactos src/main.c)
target_link_libraries(agenda_contactos agenda_lib)

# Test dummy (no hay tests reales en este ejercicio)
if(CRITERION_FOUND)
    add_executable(test_agenda_contactos tests/test_dummy.c)
    target_link_libraries(test_agenda_contactos ${CRITERION_LIBRARIES})
    target_include_directories(test_agenda_contactos PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_agenda_contactos PRIVATE ${CRITERION_LIBRARY_DIRS})
    enable_testing()
    add_test(NAME agenda_contactos_tests COMMAND test_agenda_contactos)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(agenda_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(agenda_contactos PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_agenda_contactos)
        target_compile_options(test_agenda_contactos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_agenda_contactos PRIVATE UNIT_TESTING)
    endif()
endif()
