cmake_minimum_required(VERSION 3.16)
project(agenda_contactos C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS OFF)

# Biblioteca de la agenda
add_library(agenda_lib STATIC 
    src/contacto.c 
    src/menu.c 
    src/persistencia.c)
target_include_directories(agenda_lib PUBLIC include)

# Ejecutable principal
add_executable(agenda_contactos src/main.c)
target_link_libraries(agenda_contactos agenda_lib)

enable_testing()
add_executable(test_persistencia tests/test_persistencia.c)
target_link_libraries(test_persistencia agenda_lib)
foreach(caso control comillas importacion invalido backup)
    add_test(NAME persistencia_${caso} COMMAND test_persistencia ${caso})
endforeach()
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/casos/escritura")
    add_test(NAME persistencia_escritura COMMAND test_persistencia escritura)
    set_tests_properties(persistencia_escritura PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/casos/escritura" TIMEOUT 15)
endif()

# Warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(agenda_lib PRIVATE -Wall -Wextra -Werror -pedantic)
    target_compile_options(agenda_contactos PRIVATE -Wall -Wextra -Werror -pedantic)

endif()

target_compile_options(test_persistencia PRIVATE -Wall -Wextra -Werror -pedantic)
target_compile_options(test_persistencia PRIVATE -UNDEBUG)
