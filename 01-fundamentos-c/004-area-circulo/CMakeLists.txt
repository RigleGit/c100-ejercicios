cmake_minimum_required(VERSION 3.16)

# Configuración del proyecto
project(004-area-circulo LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definir ejecutable principal
add_executable(area_circulo src/main.c)

# Enlazar con la librería matemática
target_link_libraries(area_circulo m)

# Configuración para testing
enable_testing()

# Agregar test
add_executable(test_area_circulo test/test_main.c)
target_link_libraries(test_area_circulo m)
add_test(NAME AreaCirculoTest COMMAND test_area_circulo)

# Configuración adicional para debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(area_circulo PRIVATE -g -O0)
    target_compile_options(test_area_circulo PRIVATE -g -O0)
endif()
